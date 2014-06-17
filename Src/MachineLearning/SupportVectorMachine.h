#pragma once
#include <vector>
#include <set>

namespace MagicML
{
    class KernelFunction;
    class SupportVectorMachine  //Kernel SVM is not done.
    {
    public:
        SupportVectorMachine();
        ~SupportVectorMachine();

        void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, KernelFunction* kernel, double softCoef);
        double Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);
        void SequentialMinimalOptimization(const std::vector<double>& dataX, const std::vector<double>& dataY, double softCoef);
        double CalF(int index);
        int OptimizeOneStep(int index_i, int index_j, double softCoef);
        bool IsKTT(int index, double softCoef);
        int ChooseIndexJ(int index_i, const std::set<int>& nonBoundSet);
        void RemoveZeroAlpha(void);
        double KernelInnerProductX(int index0, int index1);
        void UpdateOmiga(int index_i, double lastV_i, int index_j, double lastV_j);

    private:
        KernelFunction* mpKernel;
        int mDataDim;
        double mB;
        std::vector<double> mAlpha;
        std::vector<double> mSupportVecX;
        std::vector<double> mSupportVecY;
        //Cache
        std::vector<bool> mInnerValid;
        std::vector<double> mInnerCache;
        std::vector<double> mOmiga;
    };
}
