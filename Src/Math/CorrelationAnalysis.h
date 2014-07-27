#pragma once
#include <vector>

namespace MagicMath
{
    class CorrelationAnalysis
    {
    public:
        CorrelationAnalysis();
        ~CorrelationAnalysis();

        void Analysis(int featureCount, const std::vector<double>& dataX, const std::vector<double>& dataY);
        void UpdateDataY(const std::vector<double>& dataX, const std::vector<double>& dataY);
        void UpdateDataX(const std::vector<double>& dataX, const std::vector<double>& dataY);
        std::vector<double> GetCorrelations(void) const;
        void Reset(void);

    private:
        void UpdateStatisticX(const std::vector<double>& dataX);
        void UpdateStatisticY(const std::vector<double>& dataY);
        void UpdateStatisticXY(const std::vector<double>& dataX, const std::vector<double>& dataY);
        void UpdateCorrelations(void);

    private:
        int mFeatureCount;
        std::vector<double> mCorrelationValues;
        //cache
        std::vector<double> mExceptionsX;
        std::vector<double> mVariancesX;
        std::vector<double> mExceptionsXY;
        std::vector<double> mVariancesXY;
        double mExceptionY;
        double mVarianceY;
    };
}
