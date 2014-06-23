#pragma once
#include <vector>

namespace MagicML
{
    class LogisticRegression
    {
    public:
        LogisticRegression();
        ~LogisticRegression();

        int Learn(const std::vector<double>& dataX, const std::vector<int>& dataY);
        double Predict(const std::vector<double>& dataX) const;

    private:
        double EnergeValue(const std::vector<double>& dataX, const std::vector<int>& dataY, const std::vector<double>& coef) const;

    private:
        std::vector<double> mCoef;
    };

}
