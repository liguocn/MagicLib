#pragma once
#include <vector>

namespace MagicML
{
    class DeepBeliefNets
    {
    public:
        DeepBeliefNets();
        ~DeepBeliefNets();
        
        void Learn(const std::vector<double>& data, int dim, const std::vector<int>& hiddenCountList);
        std::vector<double> Encode(const std::vector<double>& data) const;
        std::vector<double> Decode(const std::vector<double>& data) const;
    };
}