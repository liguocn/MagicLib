#pragma once
#include <vector>

namespace MagicML
{
    class LinkFunction
    {
    public:
        LinkFunction();
        virtual ~LinkFunction() = 0;
        virtual double GetValue(double x, int order) = 0;
    };

    
    //Two layers
    class NeuralNetworks
    {
    public:
        NeuralNetworks();
        ~NeuralNetworks();

        void Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, LinkFunction* linkFunc, int linkCount);
        double Predict(const std::vector<double>& dataX) const;

    private:
        void Reset(void);

    private:
        LinkFunction* mpLinkFunc;
        int mLinkCount;
        int mDataDim;
        std::vector<double> mFirstLayerW;
        std::vector<double> mSecondLayerW;
    };
}
