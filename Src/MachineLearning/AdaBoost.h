#pragma once
#include <vector>

namespace MagicML
{
    class SimpleClassifier
    {
    public:
        SimpleClassifier();
        virtual ~SimpleClassifier() = 0;

        virtual void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY) = 0;
        virtual double Predict(const std::vector<double>& dataX) const = 0;
    };

    class AdaBoost
    {
    public:
        AdaBoost();
        ~AdaBoost();

        void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, const std::vector<SimpleClassifier*>& classifierList);
        double Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);

    private:
        std::vector<SimpleClassifier*> mClassifiers;
        std::vector<double> mWeights;
    };

}
