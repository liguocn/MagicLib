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
            int vecBRes = CalVectorBList(dataX, rotatedDataY, dataCount, lamda, iterId != 0, vecBList);
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

    int RotatedSparseRegression::CalInitVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        int dataCount, double lamda, std::vector<double>& vecBList) const
    {
        long long xDim = dataX.size() / dataCount;
        long long yDim = dataY.size() / dataCount;
        vecBList = std::vector<double>(xDim * yDim , 0);
        for (int yid = 0; yid < yDim; yid++)
        {
            //Calculate LamdaMax
            double lamdaMax = lamda;
            for (int xid = 0; xid < xDim; xid++)
            {
                double xDimLamda = 0;
                for (long long dataId = 0; dataId < dataCount; dataId++)
                {
                    xDimLamda += dataX.at(dataId * xDim + xid) * dataY.at(dataId * yDim + yid);
                }
                xDimLamda = fabs(xDimLamda / dataCount);
                if (xDimLamda > lamdaMax)
                {
                    lamdaMax = xDimLamda;
                }
            }

            //Iterate lamda
            int lamdaIterCount = 100;
            double lamdaDelta = (lamdaMax - lamda) / lamdaIterCount;
            long long vecBBaseIndex = yid * xDim;
            for (int lamdaIterId = 0; lamdaIterId < lamdaIterCount; lamdaIterId++)
            {
                int maxInnerIterCount = 10;
                for (int innerIterId = 0; innerIterId < maxInnerIterCount; innerIterId++)
                {
                    UpdateVectorBList(dataX, dataY, yid, dataCount, lamdaMax - lamdaDelta * innerIterId, vecBList);
                }
            }
        }

        return MAGIC_NO_ERROR;
    }

    void RotatedSparseRegression::UpdateVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int yId, 
        int dataCount, double lamda, std::vector<double>& vecBList) const
    {
        long long xDim = dataX.size() / dataCount;
        long long yDim = dataY.size() / dataCount;
        std::vector<double> newVecB;
        newVecB.reserve(xDim);
        long long vecBBaseIndex = yId * xDim;
        for (int xid = 0; xid < xDim; xid++)
        {
            double beta = 0;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                beta += dataX.at(dataId * xDim + xid) * dataY.at(dataId * yDim + yId);
                for (int kid = 0; kid < xDim; kid++)
                {
                    beta -= dataX.at(dataId * xDim + xid) * dataX.at(dataId * xDim + kid) * vecBList.at(vecBBaseIndex + kid);
                }
            }
            beta = beta / dataCount + vecBList.at(vecBBaseIndex + xid);
            beta = FunctionS(beta, lamda);
            newVecB.push_back(beta);
        }
        for (int xid = 0; xid < xDim; xid++)
        {
            vecBList.at(vecBBaseIndex + xid) = newVecB.at(xid);
        }
    }

    double RotatedSparseRegression::FunctionS(double z, double gama) const
    {
        double absZ = fabs(z);
        if (gama >= absZ)
        {
            return 0;
        }
        else if (z > 0)
        {
            return z - gama;
        }
        else
        {
            return z + gama;
        }
    }

    int RotatedSparseRegression::CalVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, 
            double lamda, bool isInitialized, std::vector<double>& vecBList) const
    {
        if (!isInitialized)
        {
            int initRes = CalInitVectorBList(dataX, dataY, dataCount, lamda, vecBList);
            if (initRes != MAGIC_NO_ERROR)
            {
                DebugLog << "CalInitVectorBList failed" << std::endl;
                return MAGIC_INVALID_RESULT;
            }
        }

        int xDim = dataX.size() / dataCount;
        int yDim = dataY.size() / dataCount;
        for (int yid = 0; yid < yDim; yid++)
        {
            int maxIterCount = 100;
            double lastEnerge = -1;
            for (int iterId = 0; iterId < maxIterCount; iterId++)
            {
                UpdateVectorBList(dataX, dataY, yid, dataCount, lamda, vecBList);

                //Check whether converged
                if (IsVectorBListConverged(dataX, dataY, yid, dataCount, lamda, vecBList, lastEnerge))
                {
                    break;
                }
            }
        }

        return MAGIC_NO_ERROR;
    }

    bool RotatedSparseRegression::IsVectorBListConverged(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        int yId, int dataCount, double lamda, const std::vector<double>& vecBList, double& lastEnerge) const
    {
        if (fabs(lastEnerge) < 1.0e-15)
        {
            return true;
        }

        double curEnerge = 0;
        long long xDim = dataX.size() / dataCount;
        long long yDim = dataY.size() / dataCount;
        long long vecBBaseIndex = yId * xDim;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            double xB = 0;
            long long xBaseIndex = dataId * xDim;
            for (int xid = 0; xid < xDim; xid++)
            {
                xB += vecBList.at(vecBBaseIndex + xid) * dataX.at(xBaseIndex + xid);
            }
            xB -= dataY.at(dataId * yDim + yId);
            curEnerge += xB * xB / dataCount;
        }
        double vecBNorm = 0;
        for (int xid = 0; xid < xDim; xid++)
        {
            vecBNorm += fabs(vecBList.at(vecBBaseIndex + xid));
        }
        curEnerge += vecBNorm * lamda;

        if (lastEnerge < 0)
        {
            lastEnerge = curEnerge;
            return false;
        }
        else
        {
            double deltaEnerge = curEnerge - lastEnerge;
            if (fabs(deltaEnerge) / lastEnerge < 1.0e-5)
            {
                lastEnerge = curEnerge;
                return true;
            }
            else
            {
                lastEnerge = curEnerge;
                return false;
            }
        }
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
