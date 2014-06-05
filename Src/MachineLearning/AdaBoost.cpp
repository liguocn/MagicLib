#include "AdaBoost.h"

namespace MagicML
{
    SimpleClassifier::SimpleClassifier()
    {
    }

    AdaBoost::AdaBoost() :
        mClassifiers(),
        mWeights()
    {
    }

    AdaBoost::~AdaBoost()
    {
        Reset();
    }

    void AdaBoost::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        const std::vector<SimpleClassifier*>& classifierList)
    {

    }

    double AdaBoost::Predict(const std::vector<double>& dataX) const
    {
        double res = 0.0;
        int classifierCount = mWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            res += mClassifiers.at(cid)->Predict(dataX) * mWeights.at(cid);
        }
        return res;
    }

    void AdaBoost::Reset(void)
    {
        mWeights.clear();
        for (int cid = 0; cid < mClassifiers.size(); cid++)
        {
            if (mClassifiers.at(cid) != NULL)
            {
                delete mClassifiers.at(cid);
                mClassifiers.at(cid) = NULL;
            }
        }
        mClassifiers.clear();
    }
}
