#pragma once
#include <vector>

namespace MagicML
{
    class RestrictedBoltzmannMachines
    {
    public:
        RestrictedBoltzmannMachines();
        ~RestrictedBoltzmannMachines();

        void Learn(const std::vector<double>& data, int dim, int hiddenCount);
        std::vector<double> Encode(const std::vector<double>& data) const;
        std::vector<double> Decode(const std::vector<double>& code) const;
        
    };
}
