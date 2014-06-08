#pragma once
#include <vector>

namespace MagicML
{
    class HiddenMarkovModels
    {
    public:
        HiddenMarkovModels();
        ~HiddenMarkovModels();
        
        void Learn(const std::vector<int>& observMat, int observCount);
        double ObservationPro(const std::vector<int>& observList) const;
        std::vector<int> Decode(const std::vector<int>& observList) const;
        
    private:
        int mHiddenStateCount;
        int mObservationCount;
        std::vector<double> mTransitionMat;
        std::vector<double> mObservationMat;
        std::vector<double> mInitialProMat;
    };
}
