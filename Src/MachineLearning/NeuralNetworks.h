#pragma once
#include <vector>

namespace MagicML
{
    //Two layers
    class NeuralNetworks
    {
    public:
        NeuralNetworks();
        ~NeuralNetworks();

        int Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int hiddenCount);
        double Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);
        void InitializeW(void);

    private:
        int mHiddenCount;
        int mDataDim;
        std::vector<double> mFirstLayerW;
        std::vector<double> mSecondLayerW;
    };
}
