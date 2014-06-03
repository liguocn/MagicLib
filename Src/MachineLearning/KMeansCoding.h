#pragma once
#include <vector>
#include <string>

namespace MagicML
{
    class KMeansCoding
    {
    public:
        KMeansCoding();
        ~KMeansCoding();
        
        void LearnDictionary(const std::vector<double>& data, int dim, int k);
        std::vector<double> Project(const std::vector<double>& data) const;
        int GetCoefficient(const std::vector<double>& data) const;
        void Load(const std::string& fileName);
        void Save(const std::string& fileName) const;
        
    private:
        int mDataDim;
        int mBaseNum;
        std::vector<double> mBaseList;
    };
}
