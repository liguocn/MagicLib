#include "NeuralNetworks.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicML
{
    NeuralNetworks::NeuralNetworks() :
        mHiddenCount(0),
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
        mHiddenCount = 0;
        mDataDim = 0;
        mFirstLayerW.clear();
        mSecondLayerW.clear();
    }

    int NeuralNetworks::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int hiddenCount)
    {
        if (dataX.size() == 0 || dataY.size() == 0 || hiddenCount < 1)
        {
            return MAGIC_EMPTY_INPUT;
        }
        if (dataX.size() % dataY.size() != 0)
        {
            return MAGIC_INVALID_INPUT;
        }
        Reset();
        mHiddenCount = hiddenCount;
        int dataCount = dataY.size();
        mDataDim = dataX.size() / dataY.size();
        InitializeW();
        int maxIterCount = 1000;
        std::vector<double> firstLayerGrad(mDataDim * mHiddenCount);
        std::vector<double> secondLayerGrad(mHiddenCount);
        std::vector<double> hiddenValue(mHiddenCount);
        for (int iterId = 0; iterId < maxIterCount; iterId++)
        {
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                //Calculate hidden value
                
                //Update gradients: BP algorithm
            }
            //Update W
        }
        
        return MAGIC_NO_ERROR;
    }
    
    void NeuralNetworks::InitializeW(void)
    {
        
    }

    double NeuralNetworks::Predict(const std::vector<double>& dataX) const
    {
        return 0;
    }
}
