#pragma once
#include <vector>

namespace MagicML
{
    class KernelFunction;
    class LinearDiscriminantAnalysis
    {
    public:
        LinearDiscriminantAnalysis();
        ~LinearDiscriminantAnalysis();
        
        void Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int categoryCount, KernelFunction* kernel);
        int Predict(const std::vector<double>& dataX) const;
        
    private:
        void Reset(void);
        
    private:
        KernelFunction* mpKernel;
    };
}
