#include "AdaBoost.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <math.h>

namespace MagicML
{
    WeakClassifier::WeakClassifier()
    {
    }

    AdaBoost::AdaBoost() :
        mClassifiers(),
        mClassifierWeights(),
        mThreshold(0)
    {
    }

    AdaBoost::~AdaBoost()
    {
        Reset();
    }

    int AdaBoost::Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int levelCount)
    {
        if (dataX.size() == 0 || dataY.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        Reset();
        mClassifiers.reserve(levelCount);
        mClassifierWeights.reserve(levelCount);
        int dataCount = dataY.size();
        std::vector<double> dataWeights(dataCount, 1.0 / dataCount);
        std::vector<int> resFlag(dataCount);
        double epsilon = 1.0e-5;
        mThreshold = 0.0;
        for (int levelId = 0; levelId < levelCount; levelId++)
        {
            WeakClassifier* pWeakClassifier = TrainWeakClassifier(dataX, dataY, dataWeights);
            if (pWeakClassifier != NULL)
            {
                mClassifiers.push_back(pWeakClassifier);
            }
            else
            {
                Reset();
                DebugLog << "Error: NULL Weak Classifier" << std::endl;
                return MAGIC_INVALID_RESULT;
            }
            double trainingError = 0.0;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                resFlag.at(dataId) = abs(dataY.at(dataId) - pWeakClassifier->Predict(dataX, dataId));
                trainingError += dataWeights.at(dataId) * resFlag.at(dataId);
            }
            if (trainingError < 0.5)
            {
                DebugLog << "Weak Classifier " << levelId << ": " << trainingError << std::endl;
            }

            if (fabs(trainingError - 1.0) < epsilon)
            {
                trainingError = 1.0 - epsilon;
                DebugLog << "Weak Classifier " << levelId << ": is too weak!" << std::endl;
            }
            else if (fabs(trainingError) < epsilon)
            {
                trainingError = epsilon;
                DebugLog << "Weak Classifier " << levelId << ": is too good!" << std::endl;
            }

            double beta = trainingError / (1.0 - trainingError);
            double weight = log(1.0 / beta);
            mClassifierWeights.push_back(weight);
            mThreshold += weight;

            //Update dataWeights
            double weightSum = 0.0;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                dataWeights.at(dataId) *= pow(beta, resFlag.at(dataId));
                weightSum += dataWeights.at(dataId);
            }
            for (std::vector<double>::iterator itr = dataWeights.begin(); itr != dataWeights.end(); itr++)
            {
                *itr /= weightSum;
            }
        }
        mThreshold *= 0.5;

        return MAGIC_NO_ERROR;
    }

    int AdaBoost::Predict(const std::vector<double>& dataX) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            res += mClassifiers.at(cid)->Predict(dataX) * mClassifierWeights.at(cid);
        }
        if (res > mThreshold)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    double AdaBoost::GetThreshold(void) const
    {
        return mThreshold;
    }

    void AdaBoost::SetThreshold(double thred)
    {
        mThreshold = thred;
    }

    void AdaBoost::Reset(void)
    {
        mClassifierWeights.clear();
        for (int cid = 0; cid < mClassifiers.size(); cid++)
        {
            if (mClassifiers.at(cid) != NULL)
            {
                delete mClassifiers.at(cid);
                mClassifiers.at(cid) = NULL;
            }
        }
        mClassifiers.clear();
        mThreshold = 0.0;
    }
}
