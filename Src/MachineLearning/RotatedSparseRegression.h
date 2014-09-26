#pragma once
#include <vector>

namespace MagicMath
{
    class SparseMatrix;
}

namespace MagicML
{
    class RotatedSparseRegression
    {
    public:
        RotatedSparseRegression();
        ~RotatedSparseRegression();

        int Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount);
        int Predict(const std::vector<double>& dataX, std::vector<double>& dataY) const;
        void Reset(void);

    private:
        MagicMath::SparseMatrix* mpRegressionMat;
    };
}
