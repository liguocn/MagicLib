#include "NeuralNetworks.h"

namespace MagicML
{
    LinkFunction::LinkFunction()
    {
    }

    NeuralNetworks::NeuralNetworks() :
        mpLinkFunc(NULL),
        mLinkCount(0),
        mDataDim(0),
        mFirstLayerW(),
        mSecondLayerW()
    {
    }

    NeuralNetworks::~NeuralNetworks()
    {
        Reset();
    }

    void NeuralNetworks::Reset(void)
    {
        if (mpLinkFunc != NULL)
        {
            delete mpLinkFunc;
            mpLinkFunc = NULL;
        }
        int mLinkCount = 0;
        int mDataDim = 0;
        mFirstLayerW.clear();
        mSecondLayerW.clear();
    }

    void NeuralNetworks::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, LinkFunction* linkFunc, int linkCount)
    {

    }

    double NeuralNetworks::Predict(const std::vector<double>& dataX) const
    {
        return 0;

    }
}
