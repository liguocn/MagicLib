#include "LinearRegression.h"
#include "Eigen/Dense"

namespace MagicML
{
    LinearRegression::LinearRegression() :
        mInputDim(0),
        mOutputDim(0),
        mRegMat()
    {
    }

    LinearRegression::~LinearRegression()
    {
    }

    void LinearRegression::Learn(const std::vector<double>& input, const std::vector<double>& output, int dataSize)
    {
        Reset();
        mInputDim = input.size() / dataSize;
        mOutputDim = output.size() / dataSize;
        Eigen::MatrixXd matA(dataSize, mInputDim + 1);
        Eigen::MatrixXd matB(dataSize, mOutputDim);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int inputBase = dataId * mInputDim;
            for (int dimId = 0; dimId < mInputDim; dimId++)
            {
                matA(dataId, dimId) = input.at(inputBase + dimId);
            }
            matA(dataId, mInputDim) = 1.0;

            int outputBase = dataId * mOutputDim;
            for (int dimId = 0; dimId < mOutputDim; dimId++)
            {
                matB(dataId, dimId) = output.at(outputBase + dimId);
            }
        }
        Eigen::MatrixXd matAT = matA.transpose();
        Eigen::MatrixXd matCoefA = matAT * matA;
        Eigen::MatrixXd matCoefB = matAT * matB;
        Eigen::LDLT<Eigen::MatrixXd> solver;
        solver.compute(matCoefA);
        Eigen::MatrixXd matRes = solver.solve(matCoefB);
        //copy result
        mRegMat.resize((mInputDim + 1) * mOutputDim);
        for (int colId = 0; colId < mOutputDim; colId++)
        {
            int baseIndex = colId * (mInputDim + 1);
            for (int rowId = 0; rowId <= mInputDim; rowId++)
            {
                mRegMat.at(baseIndex + rowId) = matRes(rowId, colId);
            }
        }
    }

    std::vector<double> LinearRegression::Predict(const std::vector<double>& input)
    {
        std::vector<double> output(mOutputDim);
        for (int outDim = 0; outDim < mOutputDim; outDim++)
        {
            double tempV = 0;
            int baseIndex = outDim * mInputDim;
            for (int inDim = 0; inDim < mInputDim; inDim++)
            {
                tempV += (input.at(inDim) * mRegMat.at(baseIndex + inDim));
            }
            tempV += mRegMat.at(baseIndex + mInputDim);
            output.at(outDim) = tempV;
        }
        return output;
    }

    int LinearRegression::GetInputDim()
    {
        return mInputDim;
    }

    int LinearRegression::GetOutputDim()
    {
        return mOutputDim;
    }

    std::vector<double> LinearRegression::GetRegMat(void) const
    {
        return mRegMat;
    }

    void LinearRegression::Reset()
    {
        mInputDim = 0;
        mOutputDim = 0;
        mRegMat.clear();
    }
}
