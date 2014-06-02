#pragma once
#include <vector>

namespace MagicML
{
    class Clustering
    {
    public:
        Clustering();
        ~Clustering();

        static void OrchardBoumanClustering(const std::vector<double>& inputData, int dim, int k, std::vector<int>& clusterRes);
        static void MeanshiftValue(const std::vector<double>& sourceData, int dim, double h, 
                                   const std::vector<double>& inputData, std::vector<double>& resData);
        static void KMeansClustering(const std::vector<double>& sourceData, int dim, int k, std::vector<int>& clusterRes);
        static void Spectral(const std::vector<double>& weights, int dim, int k, std::vector<int>& res);
        static void SparseSubspace(const std::vector<double>& sourceData, int dim, int k, std::vector<int>& res);

    private:
        static void CalEigenVector(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex, 
            std::vector<double>& eigenVector, double& eigenValue, std::vector<double>& meanVector);
        static void SplitSetByEigenVector(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex,
            const std::vector<double>& eigenVector, const std::vector<double>& meanVector,
            std::vector<int>& clusterA, std::vector<int>& clusterB);
        static double GaussianValue(double dist, double h);
        static void FindKMeansSeeds(const std::vector<double>& sourceData, int dim, int k, std::vector<double>& seedData);
        static double KMeansDistance(const std::vector<double>& sourcedata, int dim, int dataIndex, const std::vector<double>& centerData, int centerIndex);
    };

}