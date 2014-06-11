#include "NaiveBayes.h"
#include "GaussianMixtureModel.h"
#include "../Tool/ErrorCodes.h"

namespace MagicML
{
    NaiveBayes::NaiveBayes() :
        mCategoryProList(),
        mGaussianList()
    {
    }

    NaiveBayes::~NaiveBayes()
    {
        Reset();
    }
    
    void NaiveBayes::Reset(void)
    {
        mCategoryProList.clear();
        for (std::vector<GaussianModel*>::iterator itr = mGaussianList.begin(); itr != mGaussianList.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mGaussianList.clear();
    }

    int NaiveBayes::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int categoryCount)
    {
        Reset();
        int dataDim = dataX.size() / dataY.size();
        int dataCount = dataY.size();
        //check invalid input
        if (categoryCount < 2 || dataDim * dataY.size() != dataX.size() || dataCount == 0)
        {
            return MAGIC_INVALID_INPUT;
        }
        mCategoryProList = std::vector<double>(categoryCount, 0.0);
        std::vector<std::vector<int> > catIndexList(categoryCount);
        double deltaPro = 1.0 / dataCount;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int catId = dataY.at(dataId);
            catIndexList.at(catId).push_back(dataId);
            mCategoryProList.at(catId) += deltaPro;
        }
        mGaussianList.resize(categoryCount);
        for (int catId = 0; catId < categoryCount; catId++)
        {
            GaussianModel* pGM = new GaussianModel;
            pGM->CalParameter(dataX, dataDim, catIndexList.at(catId));
            mGaussianList.at(catId) = pGM;
        }
        return MAGIC_NO_ERROR;
    }
    
    int NaiveBayes::Predict(const std::vector<double>& dataX) const
    {
        int maxIndex = -1;
        double maxPro = 0;
        int catCount = mCategoryProList.size();
        for (int catId = 0; catId < catCount; catId++)
        {
            double pro = mCategoryProList.at(catId) * mGaussianList.at(catId)->Pro(dataX);
            if (pro > maxPro)
            {
                maxPro = pro;
                maxIndex = catId;
            }
        }
        return maxIndex;
    }
}
