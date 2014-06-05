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
        
        void Learn(const std::vector<double>& dataX, const std::vector<int>& dataY);
        int Predict(const std::vector<double>& dataX);

    private:
        std::vector<double> mCategoryProList;
        std::vector<GaussianModel*> mGaussianList;
    };
}
