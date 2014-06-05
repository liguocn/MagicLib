#include "NaiveBayes.h"
#include "GaussianMixtureModel.h"

namespace MagicML
{
    NaiveBayes::NaiveBayes() :
        mCategoryProList(),
        mGaussianList()
    {
    }

    NaiveBayes::~NaiveBayes()
    {
    }
    
    void Learn(const std::vector<double>& dataX, const std::vector<int>& dataY)
    {
        
        
    }
    
    int Predict(const std::vector<double>& dataX)
    {
        return 0;
    }
}
