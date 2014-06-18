#pragma once
#include "MagicObject.h"
#include <vector>

namespace MagicML
{
    class NaiveBayes;
    class SupportVectorMachine;
    class LinearDiscriminantAnalysis;
    class PrincipalComponentAnalysis;
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

    private:
        int mDataDim;
        std::vector<double> mDataX;
        std::vector<int> mDataY;
        MagicML::NaiveBayes* mpNaiveBayes;
        MagicML::SupportVectorMachine* mpSVM;
        MagicML::LinearDiscriminantAnalysis* mpLDA;
        MagicML::PrincipalComponentAnalysis* mpPCA;
    };

}