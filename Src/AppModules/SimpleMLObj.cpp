#include "SimpleMLObj.h"
#include "../MachineLearning/NaiveBayes.h"
#include "../MachineLearning/KernelFunction.h"
#include "../MachineLearning/SupportVectorMachine.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicApp
{
    SimpleMLObj::SimpleMLObj(int dataDim) :
        mDataDim(dataDim),
        mDataX(),
        mDataY(),
        mpNaiveBayes(NULL),
        mpSVM(NULL)
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
        if (mpSVM != NULL)
        {
            delete mpSVM;
            mpSVM = NULL;
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

    void SimpleMLObj::Load(const std::string& fileName)
    {
        Reset();
        std::ifstream fin(fileName);
        fin >> mDataDim;
        int dataCount;
        fin >> dataCount;
        mDataX.resize(dataCount * mDataDim);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            for (int dim = 0; dim < mDataDim; dim++)
            {
                fin >> mDataX.at(dataId * mDataDim + dim);
            }
        }
        mDataY.resize(dataCount);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            fin >> mDataY.at(dataId);
        }
        fin.close();
    }
    
    void SimpleMLObj::Save(const std::string& fileName) const
    {
        std::ofstream fout(fileName);
        fout << mDataDim << std::endl;
        int dataCount = mDataY.size();
        fout << dataCount << std::endl;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            for (int dim = 0; dim < mDataDim; dim++)
            {
                fout << mDataX.at(dataId * mDataDim + dim) << " ";
            }
        }
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            fout << mDataY.at(dataId) << " ";
        }
        fout.close();
    }
        
    void SimpleMLObj::LearnNaiveBayes(int categoryCount)
    {
        if (mpNaiveBayes == NULL)
        {
            mpNaiveBayes = new MagicML::NaiveBayes;
        }
        int errorCode = mpNaiveBayes->Learn(mDataX, mDataY, categoryCount);
        if (errorCode == MAGIC_NO_ERROR)
        {
            DebugLog << "SimpleMLObj::LearnNaiveBayes success" << std::endl;
        }
        else
        {
            DebugLog << "SimpleMLObj::LearnNaiveBayes fail, error code: " << errorCode << std::endl;
        }
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

    void SimpleMLObj::LearnSVM(void)
    {
        if (mpSVM == NULL)
        {
            mpSVM = new MagicML::SupportVectorMachine;
        }
        MagicML::KernelFunction* pKernel = new MagicML::EuclidKernel;
        //MagicML::KernelFunction* pKernel = new MagicML::PolynomialKernel(1, 2);
        int dataCount = mDataY.size();
        std::vector<double> dataY(dataCount);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            if (mDataY.at(dataId) == 0)
            {
                dataY.at(dataId) = -1;
            }
            else
            {
                dataY.at(dataId) = 1;
            }
        }
        mpSVM->Learn(mDataX, dataY, pKernel, 1);
    }

    int SimpleMLObj::PrediectBySVM(double x0, double x1)
    {
        if (mpSVM == NULL)
        {
            DebugLog << "Error: SVM has not been trained." << std::endl;
            return MAGIC_NON_INITIAL;
        }
        std::vector<double> dataX(2);
        dataX.at(0) = x0;
        dataX.at(1) = x1;
        double res = mpSVM->Predict(dataX);
        if (res > 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}