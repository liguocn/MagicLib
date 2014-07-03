#include "SimpleMLObj.h"
#include "../MachineLearning/NaiveBayes.h"
#include "../MachineLearning/KernelFunction.h"
#include "../MachineLearning/SupportVectorMachine.h"
#include "../MachineLearning/LinearDiscriminantAnalysis.h"
#include "../MachineLearning/PrincipalComponentAnalysis.h"
#include "../MachineLearning/LogisticRegression.h"
#include "../MachineLearning/LinearRegression.h"
#include "../MachineLearning/DecisionTree.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicApp
{
    SimpleMLObj::SimpleMLObj(int dataDim) :
        mDataDim(dataDim),
        mDataX(),
        mDataY(),
        mpNaiveBayes(NULL),
        mpSVM(NULL),
        mpLDA(NULL),
        mpPCA(NULL),
        mpLR(NULL),
        mpLinearRegression(NULL),
        mDTCutX0(0),
        mDTCutX1(0),
        mpDecisionTree(NULL)
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
        if (mpLDA != NULL)
        {
            delete mpLDA;
            mpLDA = NULL;
        }
        if (mpPCA != NULL)
        {
            delete mpPCA;
            mpPCA = NULL;
        }
        if (mpLR != NULL)
        {
            delete mpLR;
            mpLR = NULL;
        }
        if (mpLinearRegression != NULL)
        {
            delete mpLinearRegression;
            mpLinearRegression = NULL;
        }
        if (mpDecisionTree != NULL)
        {
            delete mpDecisionTree;
            mpDecisionTree = NULL;
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

    void SimpleMLObj::LearnLR(void)
    {
        if (mpLR == NULL)
        {
            mpLR = new MagicML::LogisticRegression;
        }
        int errorCode = mpLR->Learn(mDataX, mDataY);
        if (errorCode == MAGIC_NO_ERROR)
        {
            DebugLog << "SimpleMLObj::LearnLR success" << std::endl;
        }
        else
        {
            DebugLog << "SimpleMLObj::LearnLR fail, error code: " << errorCode << std::endl;
        }
    }
        
    int SimpleMLObj::PrediectByLR(double x0, double x1)
    {
        if (mpLR == NULL)
        {
            DebugLog << "Error: Logistic Regression has not been trained." << std::endl;
            return MAGIC_NON_INITIAL;
        }
        std::vector<double> dataX(2);
        dataX.at(0) = x0;
        dataX.at(1) = x1;
        if (mpLR->Predict(dataX) > 0.5)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void SimpleMLObj::LearnDT(double cutX0, double cutX1)
    {
        mDTCutX0 = cutX0;
        mDTCutX1 = cutX1;
        if (mpDecisionTree == NULL)
        {
            mpDecisionTree = new MagicML::DecisionTree;
        }
        int dataSize = mDataX.size() / mDataDim;
        std::vector<bool> dataX(mDataX.size());
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            dataX.at(dataId * mDataDim) = mDataX.at(dataId * mDataDim) > cutX0;
            dataX.at(dataId * mDataDim + 1) = mDataX.at(dataId * mDataDim + 1) > cutX1;
        }
        int errorCode = mpDecisionTree->Learn(dataX, mDataY);
        if (errorCode == MAGIC_NO_ERROR)
        {
            DebugLog << "SimpleMLObj::LearnDT success" << std::endl;
        }
        else
        {
            DebugLog << "SimpleMLObj::LearnDT fail, error code: " << errorCode << std::endl;
        }
    }
        
    int SimpleMLObj::PrediectByDT(double x0, double x1)
    {
        if (mpDecisionTree == NULL)
        {
            DebugLog << "Error: Decision Tree has not been trained." << std::endl;
            return MAGIC_NON_INITIAL;
        }
        std::vector<bool> dataX(2);
        dataX.at(0) = x0 > mDTCutX0;
        dataX.at(1) = x1 > mDTCutX1;
        return mpDecisionTree->Predict(dataX);
    }

    void SimpleMLObj::LearnLinearRegression(void)
    {
        if (mpLinearRegression == NULL)
        {
            mpLinearRegression = new MagicML::LinearRegression;
        }
        std::vector<double> dataY(mDataY.size());
        for (int dataId = 0; dataId < mDataY.size(); dataId++)
        {
            dataY.at(dataId) = mDataY.at(dataId);
        }
        mpLinearRegression->Learn(mDataX, dataY, dataY.size());
    }

    int SimpleMLObj::PrediectByLinearRegression(double x0, double x1)
    {
        if (mpLinearRegression == NULL)
        {
            DebugLog << "Error: Linear Regression has not been trained." << std::endl;
            return MAGIC_NON_INITIAL;
        }
        std::vector<double> dataX(2);
        dataX.at(0) = x0;
        dataX.at(1) = x1;
        std::vector<double> res = mpLinearRegression->Predict(dataX);
        if (res.at(0) > 0.5)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void SimpleMLObj::LearnLDA(void)
    {
        if (mpLDA == NULL)
        {
            mpLDA = new MagicML::LinearDiscriminantAnalysis;
        }
        int errorCode = mpLDA->Analyse(mDataX, mDataY, 1);
        if (errorCode == MAGIC_NO_ERROR)
        {
            DebugLog << "SimpleMLObj::LearnLDA success" << std::endl;
        }
        else
        {
            DebugLog << "SimpleMLObj::LearnLDA fail, error code: " << errorCode << std::endl;
        }
    }
    
    std::vector<double> SimpleMLObj::GetLdaVector(void)
    {
        return mpLDA->GetLdaVector(0);
    }

    void SimpleMLObj::LearnPCA(void)
    {
        if (mpPCA == NULL)
        {
            mpPCA = new MagicML::PrincipalComponentAnalysis;
        }
        mpPCA->Analyse(mDataX, 2, 1);
    }

    std::vector<double> SimpleMLObj::GetPcaVector(void)
    {
        return mpPCA->GetEigenVector(0);
    }
}