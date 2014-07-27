#pragma once
#include <vector>

namespace MagicML
{
    class WeakClassifier
    {
    public:
        WeakClassifier();
        virtual ~WeakClassifier() = 0;

        virtual int Learn(const std::vector<double>& dataX, const std::vector<double>& dataWeights, const std::vector<int>& dataY,
            double* trainError) = 0;
        virtual int Predict(const std::vector<double>& dataX) const = 0;
        virtual int Predict(const std::vector<double>& dataX, int dataId) const = 0;
    };

    class AdaBoost
    {
    public:
        AdaBoost();
        virtual ~AdaBoost();

        int Learn(const std::vector<double>& dataX, const std::vector<int>& dataY, int levelCount);
        int Predict(const std::vector<double>& dataX) const;
        double GetThreshold(void) const;
        void SetThreshold(double thred);

    protected:
        void Reset(void);
        virtual WeakClassifier* TrainWeakClassifier(const std::vector<double>& dataX, const std::vector<int>& dataY, 
            const std::vector<double>& dataWeights) const = 0;

    protected:
        std::vector<WeakClassifier*> mClassifiers;
        std::vector<double> mClassifierWeights;
        double mThreshold;
    };

}
