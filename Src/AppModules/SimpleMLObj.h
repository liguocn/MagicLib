#pragma once
#include "MagicObject.h"
#include <vector>

namespace MagicML
{
    class NaiveBayes;
    class SupportVectorMachine;
    class LinearDiscriminantAnalysis;
    class PrincipalComponentAnalysis;
    class LogisticRegression;
    class LinearRegression;
    class DecisionTree;
}

namespace MagicApp
{
    class SimpleMLObj : public MagicObject
    {
    public:
        SimpleMLObj(int dataDim);
        ~SimpleMLObj();

        void Reset(void);
        void InsertTrainingData(const std::vector<double>& dataX, int dataY);
        void InsertTrainingData(double x0, double x1, int dataY);
        void GetTrainingData(std::vector<double>& dataX, std::vector<int>& dataY) const;
        void Load(const std::string& fileName);
        void Save(const std::string& fileName) const;
        
        void LearnNaiveBayes(int categoryCount);
        int PredictByNaiveBayes(double x0, double x1);

        void LearnSVM(void);
        int PrediectBySVM(double x0, double x1);

        void LearnLDA(void);
        std::vector<double> GetLdaVector(void);

        void LearnPCA(void);
        std::vector<double> GetPcaVector(void);

        void LearnLR(void);
        int PrediectByLR(double x0, double x1);

        void LearnLinearRegression(void);
        int PrediectByLinearRegression(double x0, double x1);

        void LearnDT(double cutX0, double cutX1);
        int PrediectByDT(double x0, double x1);

    private:
        int mDataDim;
        std::vector<double> mDataX;
        std::vector<int> mDataY;
        MagicML::NaiveBayes* mpNaiveBayes;
        MagicML::SupportVectorMachine* mpSVM;
        MagicML::LinearDiscriminantAnalysis* mpLDA;
        MagicML::PrincipalComponentAnalysis* mpPCA;
        MagicML::LogisticRegression* mpLR;
        MagicML::LinearRegression* mpLinearRegression;
        int mDTCutX0, mDTCutX1;
        MagicML::DecisionTree* mpDecisionTree;
    };

}