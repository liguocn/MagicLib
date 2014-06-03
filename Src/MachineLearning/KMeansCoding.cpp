#include "KMeansCoding.h"
#include "Clustering.h"

namespace MagicML
{
    KMeansCoding::KMeansCoding() :
        mDataDim(0),
        mBaseNum(0),
        mBaseList()
    {
        
    }
    
    KMeansCoding::~KMeansCoding()
    {
        
    }
    
    void KMeansCoding::LearnDictionary(const std::vector<double>& data, int dim, int k)
    {
        mDataDim = dim;
        mBaseNum = k;
        std::vector<int> clusterRes;
        Clustering::KMeans(data, dim, k, clusterRes);
        int dataCount = data.size() / dim;
        mBaseList = std::vector<double>(mDataDim * mBaseNum, 0);
        std::vector<int> clusterCount(mBaseNum, 0);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int clusterId = clusterRes.at(dataId);
            int baseIndex = mDataDim * clusterId;
            int sourceIndex = mDataDim * dataId;
            for (int did = 0; did < mDataDim; did++)
            {
                mBaseList.at(baseIndex + did) += data.at(sourceIndex + did);
            }
            clusterCount.at(clusterId)++;
        }
        for (int clusterId = 0; clusterId < mBaseNum; clusterId++)
        {
            if (clusterCount.at(clusterId) > 0)
            {
                int baseIndex = mDataDim * clusterId;
                for (int did = 0; did < mDataDim; did++)
                {
                    mBaseList.at(baseIndex + did) /= clusterCount.at(clusterId);
                }
            }
        }
    }
    
    std::vector<double> KMeansCoding::Project(const std::vector<double>& data) const
    {
        std::vector<double> res;
        return res;
    }
    
    int KMeansCoding::GetCoefficient(const std::vector<double>& data) const
    {
        return 0;
    }
    
    void KMeansCoding::Load(const std::string& fileName)
    {
        
    }
    
    void KMeansCoding::Save(const std::string& fileName) const
    {
        
    }
}