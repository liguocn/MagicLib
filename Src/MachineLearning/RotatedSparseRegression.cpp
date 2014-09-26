#include "RotatedSparseRegression.h"
#include "../Tool/LogSystem.h"
#include "../Tool/ErrorCodes.h"
#include "../Math/SparseMatrix.h"

namespace MagicML
{
    RotatedSparseRegression::RotatedSparseRegression() : 
        mpRegressionMat(NULL)
    {
    }

    RotatedSparseRegression::~RotatedSparseRegression()
    {
        Reset();
    }

    int RotatedSparseRegression::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount)
    {
        return MAGIC_NO_ERROR;
    }
    
    int RotatedSparseRegression::Predict(const std::vector<double>& dataX, std::vector<double>& dataY) const
    {
        return MAGIC_NO_ERROR;
    }

    void RotatedSparseRegression::Reset(void)
    {
        if (mpRegressionMat != NULL)
        {
            delete mpRegressionMat;
            mpRegressionMat = NULL;
        }
    }
}
