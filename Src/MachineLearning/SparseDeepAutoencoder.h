#pragma once
#include <vector>

namespace MagicML
{
    class SparseDeepAutoencoder
    {
    public:
        SparseDeepAutoencoder();
        ~SparseDeepAutoencoder();

        void Learn(const std::vector<double>& data, int dim);
        std::vector<double> Encode(const std::vector<double>& data) const;
        std::vector<double> Decode(const std::vector<double>& code) const;

    };
}
