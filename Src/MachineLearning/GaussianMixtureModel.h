#pragma once
#include <vector>

namespace MagicML
{
    class GaussianModel
    {
    public:
        GaussianModel();
        ~GaussianModel();

        void CalParameter(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex);
        double Pro(const std::vector<double>& data) const;
        double WeightPro(const std::vector<double>& data) const;
        double GetWeight(void) const;
        const std::vector<double>& GetMeanVector(void) const;
        void PrintParameter();

    private:
        double mWeight;
        double mCovDet;
        std::vector<double> mMeanVector;
        std::vector<double> mInverseCovMat;
    };

    class GaussianMixtureModel
    {
    public:
        GaussianMixtureModel();
        ~GaussianMixtureModel();

        void CalParameter(const std::vector<double>& inputData, int dim, int k, const std::vector<int>& clusterIndex); 
        double Pro(const std::vector<double>& data);
        double ComponentPro(const std::vector<double>& data, int componentId);
        int GetClusterId(const std::vector<double>& data);
        
    private:
        void Clear();

    private:
        std::vector<GaussianModel* > mGaussianList;
    };

}
