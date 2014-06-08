#pragma once
#include <vector>

namespace MagicML
{
    class KernelFunction
    {
    public:
        KernelFunction();
        virtual ~KernelFunction() = 0;
        virtual double InnerProduct(const std::vector<double>& vec1, const std::vector<double>& vec2) = 0;
        virtual double InnerProduct(const std::vector<double>& vec1, int startId, const std::vector<double>& vec2) = 0;
    };
}
