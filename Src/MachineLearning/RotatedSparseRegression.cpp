#include "RotatedSparseRegression.h"
#include "../Tool/LogSystem.h"
#include "../Tool/ErrorCodes.h"
#include "../Math/SparseMatrix.h"
#include "../Math/DenseMatrix.h"
#include <math.h>

namespace MagicML
{
    RotatedSparseRegression::RotatedSparseRegression() : 
        mpRegressionMat(NULL),
        mMeans(),
        mVariances()
    {
    }

    RotatedSparseRegression::~RotatedSparseRegression()
    {
        Reset();
    }

    int RotatedSparseRegression::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, double lamda)
    {
        if (dataX.size() == 0 || dataY.size() == 0 || dataCount < 1)
        {
            return MAGIC_EMPTY_INPUT;
        }
        int xDim = dataX.size() / dataCount;
        int yDim = dataY.size() / dataCount;
        if (xDim * dataCount != dataX.size() || yDim * dataCount != dataY.size())
        {
            return MAGIC_INVALID_INPUT;
        }

        std::vector<double> standDataX;
        int standRes = StanderdizeDataX(dataX, dataCount, standDataX);
        if (standRes != MAGIC_NO_ERROR)
        {
            DebugLog << "StanderdizeDataX failed" << std::endl;
            return MAGIC_INVALID_INPUT;
        }

        MagicMath::DenseMatrix matR;
        matR.UnitMatrix(yDim);
        std::vector<double> vecBList(yDim * xDim);
        std::vector<double> rotatedDataY = dataY;
        int maxIterCount = 100;
        for (int iterId = 0; iterId < maxIterCount; iterId++)
        {
            int vecBRes = CalVectorBList(dataX, rotatedDataY, dataCount, lamda, vecBList);
            if (vecBRes != MAGIC_NO_ERROR)
            {
                Reset();
                DebugLog << "CalVectorBList failed" << std::endl;
                return MAGIC_INVALID_RESULT;
            }
            if (IsTrainingErrorConverged(dataX, rotatedDataY, dataCount, lamda, vecBList, &matR))
            {
                DebugLog << "TrainingErrorConverged: " << iterId << std::endl;
                break;
            }
            int matRRes = CalMatirxR(dataX, rotatedDataY, dataCount, vecBList, &matR);
            if (matRRes != MAGIC_NO_ERROR)
            {
                Reset();
                DebugLog << "CalMatrixR failed" << std::endl;
                return MAGIC_INVALID_RESULT;
            }
        }
        matR.Reset();
        rotatedDataY.clear();

        double minValueThreshold = 1.0e-15;
        std::vector<double> matBValues;
        std::vector<int> matBColIndex;
        std::vector<int> matBRowIndexOfLastValid;
        for (int rowId = 0; rowId < yDim; rowId++)
        {
            long long baseIndex = xDim * rowId;
            for (int colId = 0; colId < xDim; colId++)
            {
                if (fabs(vecBList.at(baseIndex + colId)) > minValueThreshold)
                {
                    matBValues.push_back(vecBList.at(baseIndex + colId));
                    matBColIndex.push_back(colId);
                }
                matBRowIndexOfLastValid.push_back(matBValues.size() - 1);
            }
        }
        mpRegressionMat = new MagicMath::SparseMatrix(matBValues, matBColIndex, matBRowIndexOfLastValid);

        return MAGIC_NO_ERROR;
    }
    
    int RotatedSparseRegression::Predict(const std::vector<double>& dataX, std::vector<double>& dataY) const
    {
        if (mpRegressionMat == NULL || mMeans.size() == 0 || mVariances.size() == 0)
        {
            return MAGIC_NON_INITIAL;
        }
        if (mMeans.size() != dataX.size())
        {
            return MAGIC_INVALID_INPUT;
        }
        int xDim = dataX.size();
        std::vector<double> standDataX(xDim);
        for (int xId = 0; xId < xDim; xId++)
        {
            standDataX.at(xId) = (dataX.at(xId) - mMeans.at(xId)) / mVariances.at(xId);
        }

        dataY = mpRegressionMat->Multiply(standDataX);

        return MAGIC_NO_ERROR;
    }

    void RotatedSparseRegression::Reset(void)
    {
        if (mpRegressionMat != NULL)
        {
            delete mpRegressionMat;
            mpRegressionMat = NULL;
        }
        mMeans.clear();
        mVariances.clear();
    }

    int RotatedSparseRegression::StanderdizeDataX(const std::vector<double>& dataX, int dataCount, std::vector<double>& standDataX)
    {
        long long xDim = dataX.size() / dataCount;
        mMeans = std::vector<double>(xDim, 0);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            long long baseIndex = dataId * xDim;
            for (int xId = 0; xId < xDim; xId++)
            {
                mMeans.at(xId) += dataX.at(baseIndex + xId);
            }
        }
        for (int xId = 0; xId < xDim; xId++)
        {
            mMeans.at(xId) /= dataCount;
        }

        mVariances = std::vector<double>(xDim, 0);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            long long baseIndex = dataId * xDim;
            for (int xId = 0; xId < xDim; xId++)
            {
                double dTemp = dataX.at(baseIndex + xId) - mMeans.at(xId);
                mVariances.at(xId) += dTemp * dTemp;
            }
        }
        for (int xId = 0; xId < xDim; xId++)
        {
            if (mVariances.at(xId) < 1.0e-15)
            {
                mMeans.clear();
                mVariances.clear();
                return MAGIC_INVALID_INPUT;
            }
            mVariances.at(xId) = sqrt(mVariances.at(xId) /= dataCount);
        }

        standDataX.clear();
        standDataX.reserve(dataX.size());
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            long long baseIndex = dataId * xDim;
            for (int xId = 0; xId < xDim; xId++)
            {
                standDataX.push_back( (dataX.at(baseIndex + xId) - mMeans.at(xId)) / mVariances.at(xId) );
            }
        }

        return MAGIC_NO_ERROR;
    }

    int RotatedSparseRegression::CalVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, 
            double lamda, std::vector<double>& vecBList) const
    {
        int xDim = dataX.size() / dataCount;
        int yDim = dataY.size() / dataCount;
        for (int yid = 0; yid < yDim; yid++)
        {

        }

        return MAGIC_NO_ERROR;
    }

    bool RotatedSparseRegression::IsTrainingErrorConverged(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        int dataCount, double lamda, std::vector<double>& vecList, const MagicMath::DenseMatrix* pMatR) const
    {
        return true;
    }

    int RotatedSparseRegression::CalMatirxR(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, 
            const std::vector<double>& vecBList, MagicMath::DenseMatrix* pMatR) const
    {
        return MAGIC_NO_ERROR;
    }
}
