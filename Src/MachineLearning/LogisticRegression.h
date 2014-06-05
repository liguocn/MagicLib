#pragma once
#include <vector>

namespace MagicML
{
    class LogisticRegression
    {
    public:
        LogisticRegression();
        ~LogisticRegression();

        void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY);
        double Predict(const std::vector<double>& dataX) const;

    private:
        std::vector<double> mCoef;
    };

}
