#include "SimpleMLObj.h"
#include "../MachineLearning/NaiveBayes.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicApp
{
    SimpleMLObj::SimpleMLObj(int dataDim) :
        mDataDim(dataDim),
        mDataX(),
        mDataY(),
        mpNaiveBayes(NULL)
    {
    }

    SimpleMLObj::~SimpleMLObj()
    {
    }

    void SimpleMLObj::Reset(void)
    {
        mDataX.clear();
        mDataY.clear();
        if (mpNaiveBayes != NULL)
        {
            delete mpNaiveBayes;
            mpNaiveBayes = NULL;
        }
    }

    void SimpleMLObj::InsertTrainingData(const std::vector<double>& dataX, int dataY)
    {
        mDataY.push_back(dataY);
        for (int dataId = 0; dataId < dataX.size(); dataId++)
        {
            mDataX.push_back(dataX.at(dataId));
        }
    }

    void SimpleMLObj::InsertTrainingData(double x0, double x1, int dataY)
    {
        mDataX.push_back(x0);
        mDataX.push_back(x1);
        mDataY.push_back(dataY);
    }

    void SimpleMLObj::GetTrainingData(std::vector<double>& dataX, std::vector<int>& dataY) const
    {
        dataX = mDataX;
        dataY = mDataY;
    }
        
    void SimpleMLObj::LearnNaiveBayes(int categoryCount)
    {
        if (mpNaiveBayes == NULL)
        {
            mpNaiveBayes = new MagicML::NaiveBayes;
        }
        mpNaiveBayes->Learn(mDataX, mDataY, categoryCount);
    }

    int SimpleMLObj::PredictByNaiveBayes(double x0, double x1)
    {
        if (mpNaiveBayes == NULL)
        {
            DebugLog << "Error: Naive Bayes has not been trained." << std::endl;
            return MAGIC_NON_INITIAL;
        }
        std::vector<double> dataX(2);
        dataX.at(0) = x0;
        dataX.at(1) = x1;
        return mpNaiveBayes->Predict(dataX);
    }
}