#pragma once
#include <vector>

namespace MagicML
{
    class LinearDiscriminantAnalysis
    {
    public:
        LinearDiscriminantAnalysis();
        ~LinearDiscriminantAnalysis();
        
        int Analyse(const std::vector<double>& dataX, const std::vector<int>& dataY, int ldaDim);
        std::vector<double> GetLdaVector(int k) const;
        std::vector<double> Project(const std::vector<double>& data) const;
        
    private:
        void Reset(void);
        
    private:
        int mDataDim;
        int mLdaDim;
        std::vector<double> mLdaVectors;
    };
}
