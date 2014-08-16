#include "CorrelationAnalysis.h"
#include <math.h>

namespace MagicMath
{
    CorrelationAnalysis::CorrelationAnalysis() :
        mFeatureCount(0),
        mCorrelationValues(),
        mExceptionsX(),
        mVariancesX(),
        mExceptionsXY(),
        mVariancesXY(),
        mExceptionY(0),
        mVarianceY(0)
    {
    }

    CorrelationAnalysis::~CorrelationAnalysis()
    {
    }

    void CorrelationAnalysis::Analysis(int featureCount, const std::vector<double>& dataX, const std::vector<double>& dataY)
    {
        Reset();
        mFeatureCount = featureCount;
        UpdateStatisticX(dataX);
        UpdateStatisticY(dataY);
        UpdateStatisticXY(dataX, dataY);
        UpdateCorrelations();
    }
        
    void CorrelationAnalysis::UpdateDataY(const std::vector<double>& dataX, const std::vector<double>& dataY)
    {
        UpdateStatisticY(dataY);
        UpdateStatisticXY(dataX, dataY);
        UpdateCorrelations();
    }
        
    void CorrelationAnalysis::UpdateDataX(const std::vector<double>& dataX, const std::vector<double>& dataY)
    {
        UpdateStatisticX(dataX);
        UpdateStatisticXY(dataX, dataY);
        UpdateCorrelations();
    }
        
    std::vector<double> CorrelationAnalysis::GetCorrelations(void) const
    {
        return mCorrelationValues;
    }

    void CorrelationAnalysis::Reset(void)
    {
        mFeatureCount = 0;
        mCorrelationValues.clear();
        mExceptionsX.clear();
        mVariancesX.clear();
        mExceptionsXY.clear();
        mVariancesXY.clear();
        mExceptionY = 0;
        mVarianceY = 0;
    }

    void CorrelationAnalysis::UpdateStatisticX(const std::vector<double>& dataX)
    {  
        mExceptionsX.clear();
        mExceptionsX.reserve(mFeatureCount);
        mVariancesX.clear();
        mVariancesX.reserve(mFeatureCount);
        int dataCount = dataX.size() / mFeatureCount;
        for (int featureId = 0; featureId < mFeatureCount; featureId++)
        {
            double exception = 0;
            int baseIndex = featureId * dataCount;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                exception += dataX.at(baseIndex + dataId);
            }
            exception /= dataCount;
            mExceptionsX.push_back(exception);
            double variance = 0;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                double dTemp = dataX.at(baseIndex + dataId) - exception;
                variance += dTemp * dTemp;
            }
            variance /= dataCount;
            mVariancesX.push_back(variance);
        }
    }
        
    void CorrelationAnalysis::UpdateStatisticY(const std::vector<double>& dataY)
    {
        mExceptionY = 0;
        for (std::vector<double>::const_iterator itr = dataY.begin(); itr != dataY.end(); itr++)
        {
            mExceptionY += *itr;
        }
        mExceptionY /= dataY.size();
        mVarianceY = 0;
        for (std::vector<double>::const_iterator itr = dataY.begin(); itr != dataY.end(); itr++)
        {
            double dTemp = *itr - mExceptionY;
            mVarianceY += dTemp * dTemp;
        }
        mVarianceY /= dataY.size();
    }
        
    void CorrelationAnalysis::UpdateStatisticXY(const std::vector<double>& dataX, const std::vector<double>& dataY)
    {
        mExceptionsXY.clear();
        mExceptionsXY.reserve(mFeatureCount);
        mVariancesXY.clear();
        mVariancesXY.reserve(mFeatureCount);
        int dataCount = dataX.size() / mFeatureCount;
        for (int featureId = 0; featureId < mFeatureCount; featureId++)
        {
            double exception = 0;
            int baseIndex = featureId * dataCount;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                exception += dataX.at(baseIndex + dataId) * dataY.at(dataId);
            }
            exception /= dataCount;
            mExceptionsXY.push_back(exception);
            double variance = 0;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                double dTemp = dataX.at(baseIndex + dataId) * dataY.at(dataId) - exception;
                variance += dTemp * dTemp;
            }
            variance /= dataCount;
            mVariancesXY.push_back(variance);
        }
    }

    void CorrelationAnalysis::UpdateCorrelations(void)
    {
        mCorrelationValues.clear();
        mCorrelationValues.reserve(mFeatureCount);
        for (int featureId = 0; featureId < mFeatureCount; featureId++)
        {
            double dTemp = sqrt(mVariancesX.at(featureId) * mVarianceY);
            if (dTemp < 1.0e-15)
            {
                mCorrelationValues.push_back(0);
            }
            else
            {
                mCorrelationValues.push_back( (mExceptionsXY.at(featureId) - mExceptionsX.at(featureId) * mExceptionY) / dTemp );
            }
        }
    }
}
