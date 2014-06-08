#include "LinearDiscriminantAnalysis.h"
#include "KernelFunction.h"

namespace MagicML
{
    LinearDiscriminantAnalysis::LinearDiscriminantAnalysis() :
        mpKernel(NULL)
    {
        
    }
    
    LinearDiscriminantAnalysis::~LinearDiscriminantAnalysis()
    {
        Reset();
    }
    
    void LinearDiscriminantAnalysis::Reset(void)
    {
        if (mpKernel != NULL)
        {
            delete mpKernel;
            mpKernel = NULL;
        }
    }
    
    void LinearDiscriminantAnalysis::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int categoryCount, KernelFunction* kernel)
    {
        
    }
    
    int LinearDiscriminantAnalysis::Predict(const std::vector<double>& dataX) const
    {
        return 0;
    }
}