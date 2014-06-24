#pragma once
#include <vector>

namespace MagicML
{
    class LinearRegression
    {
    public:
        LinearRegression();
        ~LinearRegression();

        void Learn(const std::vector<double>& input, const std::vector<double>& output, int dataSize);
        std::vector<double> Predict(const std::vector<double>& input);
        int GetInputDim(void);
        int GetOutputDim(void);
        std::vector<double> GetRegMat(void) const;

    private:
        void Reset(void);
    private:
        int mInputDim;
        int mOutputDim;
        std::vector<double> mRegMat; //(mInputDim + 1) * mOutputDim; col first: col0, col1, col2......
    };

}
