#pragma once
#include <vector>

namespace MagicML
{
    class KernelFunction;
    class SupportVectorMachine
    {
    public:
        SupportVectorMachine();
        ~SupportVectorMachine();

        void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, KernelFunction* kernel, double softCoef);
        double Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);
        void SequentialMinimalOptimization(const std::vector<double>& dataX, const std::vector<double>& dataY, double softCoef);
    private:
        KernelFunction* mpKernel;
        double mB;
        std::vector<double> mAlpha;
        std::vector<double> mSupportVecX;
        std::vector<double> mSupportVecY;
    };
}
