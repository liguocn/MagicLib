#include "SupportVectorMachine.h"
#include "KernelFunction.h"

namespace MagicML
{
    SupportVectorMachine::SupportVectorMachine() :
        mpKernel(NULL),
        mB(0),
        mAlpha(),
        mSupportVecX(),
        mSupportVecY()
    {
    }

    SupportVectorMachine::~SupportVectorMachine()
    {
        if (mpKernel != NULL)
        {
            delete mpKernel;
            mpKernel = NULL;
        }
    }

    void SupportVectorMachine::Reset(void)
    {
        if (mpKernel != NULL)
        {
            delete mpKernel;
            mpKernel = NULL;
        }
        mB = 0;
        mAlpha.clear();
        mSupportVecX.clear();
        mSupportVecY.clear();
    }

    void SupportVectorMachine::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        KernelFunction* kernel, double softCoef)
    {
        Reset();
        mpKernel = kernel;
        SequentialMinimalOptimization(dataX, dataY, softCoef);
    }

    void SupportVectorMachine::SequentialMinimalOptimization(const std::vector<double>& dataX, const std::vector<double>& dataY, double softCoef)
    {

    }
        
    double SupportVectorMachine::Predict(const std::vector<double>& dataX) const
    {
        int supCount = mAlpha.size();
        double res = 0;
        int dataDim = mSupportVecX.size() / mSupportVecY.size();
        for (int supId = 0; supId < supCount; supId++)
        {
            res += mAlpha.at(supId) * mSupportVecY.at(supId) * mpKernel->InnerProduct(mSupportVecX, supId * dataDim, dataX);
        }
        res += mB;

        return res;
    }
}
