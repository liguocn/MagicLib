#pragma once
#include "MagicObject.h"
#include <vector>

namespace MagicML
{
    class NaiveBayes;
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
        
        void LearnNaiveBayes(int categoryCount);
        int PredictByNaiveBayes(double x0, double x1);

    private:
        int mDataDim;
        std::vector<double> mDataX;
        std::vector<int> mDataY;
        MagicML::NaiveBayes* mpNaiveBayes;
    };

}