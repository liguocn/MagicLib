#include "Optimization.h"
#include "Eigen/Dense"

namespace MagicMath
{
    GradientDescent::GradientDescent() :
        mMaxIterCount(10000)
    {
    }

    GradientDescent::GradientDescent(int maxIterCount) :
        mMaxIterCount(maxIterCount)
    {
    }

    GradientDescent::~GradientDescent()
    {
    }

    void GradientDescent::Run()
    {
        CalInitValue();
        for (int iterId = 0; iterId < mMaxIterCount; iterId++)
        {
            CalGradient();
            if (IsStop())
            {
                break;
            }
            CalStepLength();
            UpdateResult();
        }
    }

    NewtonMethod::NewtonMethod() :
        mMaxIterCount(100),
        mGradVec(),
        mHessMat(),
        mStepVec()
    {
    }

    NewtonMethod::NewtonMethod(int maxIterCount, int coefDim) :
        mMaxIterCount(maxIterCount),
        mGradVec(coefDim, 0),
        mHessMat(coefDim * coefDim, 0),
        mStepVec(coefDim, 0)
    {
    }

    NewtonMethod::~NewtonMethod()
    {
    }

    void NewtonMethod::Run()
    {
        CalInitValue();
        for (int iterId = 0; iterId < mMaxIterCount; iterId++)
        {
            CalGradient();
            if (IsStop())
            {
                break;
            }
            CalHessian();
            CalStep();
            UpdateResult();
        }
    }

    void NewtonMethod::CalStep()
    {
        int coefDim = mGradVec.size();
        Eigen::MatrixXd matH(coefDim, coefDim);
        Eigen::VectorXd vecG(coefDim, 1);
        for (int rid = 0; rid < coefDim; rid++)
        {
            int baseIndex = rid * coefDim;
            for (int cid = 0; cid < coefDim; cid++)
            {
                matH(rid, cid) = mHessMat.at(baseIndex + cid);
            }
            vecG(rid) = -mGradVec.at(rid);
        }
        Eigen::VectorXd res = matH.ldlt().solve(vecG);
        for (int coefId = 0; coefId < coefDim; coefId++)
        {
            mStepVec.at(coefId) = res(coefId);
        }
    }
}
