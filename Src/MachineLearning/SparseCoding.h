#pragma once
#include <vector>
#include <string>

namespace MagicML
{
    class SparseCoding
    {
    public:
        SparseCoding();
        ~SparseCoding();
        
        void LearnDictionary(const std::vector<double>& initialDic, const std::vector<double>& trainingData, int dataDim);
        std::vector<double> Project(const std::vector<double>& data) const;
        void GetCoefficients(const std::vector<double>& data, std::vector<double>* coefs, std::vector<int>* coefIndex) const;
        void Load(const std::string& fileName);
        void Save(const std::string& fileName) const;
        
    private:
        int mDataDim;
        int mBaseNum;
        std::vector<double> mBaseList;
    };
}
