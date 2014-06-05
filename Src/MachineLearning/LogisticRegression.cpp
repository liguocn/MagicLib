#include "LogisticRegression.h"
#include "../Math/BasicFunction.h"

namespace MagicML
{
    LogisticRegression::LogisticRegression() :
        mCoef()
    {
    }

    LogisticRegression::~LogisticRegression()
    {
    }

    void LogisticRegression::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY)
    {

    }

    double LogisticRegression::Predict(const std::vector<double>& dataX) const
    {
        double res = 0.0;
        int dataDim = dataX.size();
        for (int did = 0; did < dataDim; did++)
        {
            res += dataX.at(did) * mCoef.at(did);
        }
        res += mCoef.at(dataDim);
        return MagicMath::BasicFunction::Sigmoid(res);
    }
}
