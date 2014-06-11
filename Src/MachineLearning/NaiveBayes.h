#pragma once
#include <vector>

namespace MagicML
{
    class GaussianModel;
    class NaiveBayes
    {
    public:
        NaiveBayes();
        ~NaiveBayes();
        
        int Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int categoryCount);
        int Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);

    private:
        std::vector<double> mCategoryProList;
        std::vector<GaussianModel*> mGaussianList;
    };
}
