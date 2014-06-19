#include "Clustering.h"
#include "../Tool/LogSystem.h"
#include "../Math/DenseMatrix.h"
#include "Eigen/Eigenvalues"
#include "flann/flann.h"

namespace MagicML
{
    Clustering::Clustering()
    {
    }

    Clustering::~Clustering()
    {
    }

    void Clustering::OrchardBouman(const std::vector<double>& inputData, int dim, int k, std::vector<int>& clusterRes)
    {
        std::vector<std::vector<double> > eigenVectorList(k);
        std::vector<double> eigenValueList(k);
        std::vector<std::vector<double> > meanVectorList(k);
        std::vector<std::vector<int> > clusterIndexList(k);
        int dataCount = inputData.size() / dim;
        std::vector<int> clusterStart(dataCount);
        for (int did = 0; did < dataCount; did++)
        {
            clusterStart.at(did) = did;
        }
        clusterIndexList.at(0) = clusterStart;
        int splitIndex = 0;
        std::vector<double> eigenVector;
        double eigenValue;
        std::vector<double> meanVector;
        CalEigenVector(inputData, dim, clusterIndexList.at(splitIndex), eigenVector, eigenValue, meanVector);
        eigenValueList.at(splitIndex) = eigenValue;
        eigenVectorList.at(splitIndex) = eigenVector;
        meanVectorList.at(splitIndex) = meanVector;
        for (int cid = 1; cid < k; cid++)
        {
            std::vector<int> clusterA, clusterB;
            SplitSetByEigenVector(inputData, dim, clusterIndexList.at(splitIndex), eigenVectorList.at(splitIndex), 
                meanVectorList.at(splitIndex), clusterA, clusterB);
            
            //update clusterIndexList
            clusterIndexList.at(splitIndex) = clusterA;
            clusterIndexList.at(cid) = clusterB;

            if (cid == k - 1)
            {
                break;
            }

            //update eigenVectorList, eigenValueList, meanVectorList
            CalEigenVector(inputData, dim, clusterA, eigenVector, eigenValue, meanVector);
            eigenValueList.at(splitIndex) = eigenValue;
            eigenVectorList.at(splitIndex) = eigenVector;
            meanVectorList.at(splitIndex) = meanVector;
            CalEigenVector(inputData, dim, clusterB, eigenVector, eigenValue, meanVector);
            eigenValueList.at(cid) = eigenValue;
            eigenVectorList.at(cid) = eigenVector;
            meanVectorList.at(cid) = meanVector;

            //update splitIndex
            double largestEigenValue = eigenValueList.at(0);
            splitIndex = 0;
            for (int sid = 0; sid <= cid; sid++)
            {
                if (eigenValueList.at(sid) > largestEigenValue)
                {
                    largestEigenValue = eigenValueList.at(sid);
                    splitIndex = sid;
                }
            }
        }
        //copy result
        clusterRes.clear();
        clusterRes.resize(dataCount);
        for (int cid = 0; cid < k; cid++)
        {
            std::vector<int>& clusterIndex = clusterIndexList.at(cid);
            int clusterSize = clusterIndex.size();
            for (int i = 0; i < clusterSize; i++)
            {
                clusterRes.at(clusterIndex.at(i)) = cid;
            }
        }
    }

    void Clustering::CalEigenVector(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex, 
            std::vector<double>& eigenVector, double& eigenValue, std::vector<double>& meanVector)
    {
        int dataCount = inputIndex.size();
        meanVector = std::vector<double>(dim, 0.0);
        for (int sid = 0; sid < dataCount; sid++)
        {
            int baseIndex = inputIndex.at(sid) * dim;
            for (int did = 0; did < dim; did++)
            {
                meanVector.at(did) += inputData.at(baseIndex + did);
            }
        }
        for (int did = 0; did < dim; did++)
        {
            meanVector.at(did) /= dataCount;
        }
        Eigen::MatrixXd RMat(dim, dim);
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                RMat(rid, cid) = 0.0;
            }
        }
        /*for (int sid = 0; sid < dataCount; sid++)
        {
            int baseIndex = inputIndex.at(sid) * dim;
            for (int rid= 0; rid < dim; rid++)
            {
                for (int cid = 0; cid < dim; cid++)
                {
                    RMat(rid, cid) += (inputData.at(baseIndex + rid) * inputData.at(baseIndex + cid));
                }
            }
        }
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                RMat(rid, cid) -= (meanVector.at(rid) * meanVector.at(cid) * dataCount);
            }
        }*/
        std::vector<double> deltaVec(dim);
        for (int sid = 0; sid < dataCount; sid++)
        {
            int baseIndex = inputIndex.at(sid) * dim;
            for (int did = 0; did < dim; did++)
            {
                deltaVec.at(did) = inputData.at(baseIndex + did) - meanVector.at(did);
            }
            for (int rid= 0; rid < dim; rid++)
            {
                for (int cid = 0; cid < dim; cid++)
                {
                    RMat(rid, cid) += (deltaVec.at(rid) * deltaVec.at(cid));
                }
            }
        }
        RMat /= dataCount;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(RMat);
        eigenValue = es.eigenvalues()(dim - 1);
        Eigen::VectorXd maxVec = es.eigenvectors().col(dim - 1);
        eigenVector = std::vector<double>(dim);
        for (int did = 0; did < dim; did++)
        {
            eigenVector.at(did) = maxVec(did);
        }
    }

    void Clustering::SplitSetByEigenVector(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex,
            const std::vector<double>& eigenVector, const std::vector<double>& meanVector,
            std::vector<int>& clusterA, std::vector<int>& clusterB)
    {
        clusterA.clear();
        clusterB.clear();
        int dataCount = inputIndex.size();
        double meanValue = 0.0;
        for (int did = 0; did < dim; did++)
        {
            meanValue += (eigenVector.at(did) * meanVector.at(did));
        }
        for (int sid = 0; sid < dataCount; sid++)
        {
            int baseIndex = inputIndex.at(sid) * dim;
            double dataProjectValue = 0.0;
            for (int did = 0; did < dim; did++)
            {
                dataProjectValue += (inputData.at(baseIndex + did) * eigenVector.at(did));
            }
            if (dataProjectValue <= meanValue)
            {
                clusterA.push_back(inputIndex.at(sid));
            }
            else
            {
                clusterB.push_back(inputIndex.at(sid));
            }
        }
    }

    void Clustering::MeanshiftValue(const std::vector<double>& sourceData, int dim, double h, 
                                    const std::vector<double>& inputData, std::vector<double>& resData)
    {
        //double radius = h * 7 * 2.3; //according to h
        double radius = h * h;
        //calculate near neighbors by radius
        double* pSrcData = new double[sourceData.size()];
        for (int i = 0; i < sourceData.size(); i++)
        {
            pSrcData[i] = sourceData.at(i);
        }
        int dataCount = sourceData.size() / dim;
        int queryCount = inputData.size() / dim;
        flann::Matrix<double> dataSet(pSrcData, dataCount, dim);
        flann::Index<L2<double> > index(dataSet, flann::KDTreeIndexParams());
        index.buildIndex();
        //Mean shift every query
        resData.clear();
        resData.resize(sourceData.size());
        double* pQueryData = new double[dim];
        double* pMeanData = new double[dim];
        //double epsilon = h * 1.0e-7;
        for (int qid = 0; qid < queryCount; qid++)
        {
            int baseIndex = qid * dim;
            for (int did = 0; did < dim; did++)
            {
                pQueryData[did] = inputData.at(baseIndex + did);
            }
            for (int iterId = 0; iterId < 5; iterId++)
            {
                //DebugLog << "pQuery" << iterId << " : " << pQueryData[0] << " " << pQueryData[1] << " " << pQueryData[2] << std::endl;
                std::vector<std::vector<int> > nearIndex;
                std::vector<std::vector<double> > nearDist;
                flann::Matrix<double> querySet(pQueryData, 1, dim);
                index.radiusSearch(querySet, nearIndex, nearDist, radius, flann::SearchParams(-1));
                int nearCount = nearIndex.at(0).size();
                if (nearIndex.size () != 1 || nearDist.size() != 1) 
                {
                    DebugLog << "error: nearIndex.size () != 1 || nearDist.size() != 1" << std::endl;
                }
                if (nearCount == 0)
                {
                    DebugLog << "nearCount == 0, iterId: " << iterId << std::endl;
                    break;
                }
                /*else 
                {
                    DebugLog << "iterId: " << iterId << " count: " << nearCount << std::endl;
                }*/
                double coefSum = 0.0;
                for (int did = 0; did < dim; did++)
                {
                    pMeanData[did] = 0.0;
                }
                for (int nid = 0; nid < nearCount; nid++)
                {
                    double coef = GaussianValue(nearDist.at(0).at(nid), h);
                    //DebugLog << "nearDist" << nid << ": " << nearDist.at(0).at(nid) << " index: " << nearIndex.at(0).at(nid) << std::endl;
                    coefSum += coef;
                    int baseIndex = nearIndex.at(0).at(nid) * dim;
                    for (int did = 0; did < dim; did++)
                    {
                        pMeanData[did] += coef * sourceData.at(baseIndex + did);
                    }
                }
                for (int did = 0; did < dim; did++)
                {
                    pMeanData[did] /= coefSum;
                }
                //check if pMeanData is near pQueryData enough

                double* pTemp = pMeanData;
                pMeanData = pQueryData;
                pQueryData = pTemp;
            }
            //copy result
            for (int did = 0; did < dim; did++)
            {
                resData.at(baseIndex + did) = pQueryData[did];
            }
        }
        delete []pQueryData;
        pQueryData = NULL;
        delete []pMeanData;
        pMeanData = NULL;
        delete []pSrcData;
        pSrcData = NULL;
    }

    double Clustering::GaussianValue(double dist, double h)
    {
        //return 1.0 / exp(dist / h);
        return 1.0;
    }

    void Clustering::KMeans(const std::vector<double>& sourceData, int dim, int k, std::vector<int>& clusterRes)
    {
        //computer data bounding box size
        std::vector<double> BBoxMin(dim);
        std::vector<double> BBoxMax(dim);
        for (int did = 0; did < dim; did++)
        {
            BBoxMax.at(did) = sourceData.at(did);
            BBoxMin.at(did) = sourceData.at(did);
        }
        int dataCount = sourceData.size() / dim;
        for (int sid = 1; sid < dataCount; sid++)
        {
            int sourceBase = sid * dim;
            for (int did = 0; did < dim; did++)
            {
                int srcIdx = sourceBase + did;
                if (sourceData.at(srcIdx) > BBoxMax.at(did))
                {
                    BBoxMax.at(did) = sourceData.at(srcIdx);
                }
                if (sourceData.at(srcIdx) < BBoxMin.at(did))
                {
                    BBoxMin.at(did) = sourceData.at(srcIdx);
                }
            }
        }
        double bboxSize = 0;
        for (int did = 0; did < dim; did++)
        {
            double dTemp = BBoxMax.at(did) - BBoxMin.at(did);
            bboxSize += dTemp * dTemp;
        }
        bboxSize = sqrt(bboxSize);
        double stopEpsilon = bboxSize * 1.0e-5;
        //
        std::vector<double> centerData;
        FindKMeansSeeds(sourceData, dim, k, centerData);
        clusterRes.clear();
        clusterRes.resize(dataCount);
        int maxIterCount = 100;
        for (int iterId = 0; iterId < maxIterCount; iterId++)
        {
            //Cluster
            for (int sid = 0; sid < dataCount; sid++)
            {
                int nearIndex = 0;
                double nearDist = KMeansDistance(sourceData, dim, sid, centerData, 0);
                for (int cid = 1; cid < k; cid++)
                {
                    double distTemp = KMeansDistance(sourceData, dim, sid, centerData, cid);
                    if (distTemp < nearDist)
                    {
                        nearDist = distTemp;
                        nearIndex = cid;
                    }
                }
                clusterRes.at(sid) = nearIndex;
            }

            //Find new centers
            std::vector<double> newCenterData(dim * k, 0);
            std::vector<int> clusterCount(k, 0);
            for (int sid = 0; sid < dataCount; sid++)
            {
                int sourceBase = sid * dim;
                int centerId = clusterRes.at(sid);
                int centerBase = centerId * dim;
                for (int did = 0; did < dim; did++)
                {
                    newCenterData.at(centerBase + did) += sourceData.at(sourceBase + did);
                }
                clusterCount.at(centerId)++;
            }
            for (int cid = 0; cid < k; cid++)
            {
                if (clusterCount.at(cid) > 0)
                {
                    int centerBase = cid * dim;
                    for (int did = 0; did < dim; did++)
                    {
                        newCenterData.at(centerBase + did) /= clusterCount.at(cid);
                    }
                }
            }

            //Judge whether to stop
            double maxDif = 0;
            for (int cid = 0; cid < k; cid++)
            {
                double dif = 0;
                int baseIndex = cid * dim;
                for (int did = 0; did < dim; did++)
                {
                    double dTemp = newCenterData.at(baseIndex + did) - centerData.at(baseIndex + did);
                    dif += dTemp * dTemp;
                }
                if (maxDif < dif)
                {
                    maxDif = dif;
                }
            }
            maxDif = sqrt(maxDif);
            if (maxDif < stopEpsilon)
            {
                DebugLog << "K-Means lucky break at " << iterId << std::endl;
                break;
            }
            //else
            //{
            //    DebugLog << "Iter" << iterId << ": " << maxDif << " stopEpsilon: " << stopEpsilon << std::endl;
            //}
            //Update centerData
            centerData = newCenterData;
        }
    }
    
    void Clustering::Spectral(const std::vector<double>& weights, int dim, int k, std::vector<int>& res)
    {
        //Construct matrix L = D - W
        Eigen::MatrixXd LMat(dim, dim);
        for (int rid = 0; rid < dim; rid++)
        {
            double rowSum = 0.0;
            int baseIndex = rid * dim;
            for (int cid = 0; cid < dim; cid++)
            {
                rowSum += weights.at(baseIndex + cid);
            }
            for (int cid = 0; cid < dim; cid++)
            {
                LMat(rid, cid) = rowSum - weights.at(baseIndex + cid);
            }
        }

        //Compute the first k eigen vectors
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(LMat);

        //Construct matrix U
        std::vector<double> eigenSource(dim * k);
        for (int dimId = 0; dimId < dim; dimId++)
        {
            int baseIndex = dimId * k;
            for (int eigenId = 0; eigenId < k; eigenId++)
            {
                eigenSource.at(baseIndex + eigenId) = es.eigenvectors().col(dim - 1 - eigenId)(dimId);
            }
        }

        //Cluster using k-means
        KMeans(eigenSource, k, k, res);
    }
    
    void Clustering::SparseSubspace(const std::vector<double>& sourceData, int dim, int k, std::vector<int>& res)
    {
        //Construct adjacency matrix
        int dataCount = sourceData.size() / dim;
        std::vector<double> adjMat;
        SpectralProjectGradient(sourceData, dim, 1, adjMat);
        //Cluster using spectral method
        Spectral(adjMat, dataCount, k, res);
    }

    void Clustering::MultiFeatureFuse(const std::vector<double>& sourceData, const std::vector<int>& dimList, 
            int k, std::vector<int>& res)
    {

    }

    void Clustering::SpectralProjectGradient(const std::vector<double>& sourceData, int dim, double lamda, std::vector<double>& adjMat)
    {
        int dataCount = sourceData.size() / dim;
        std::vector<double> square2X(dataCount * dataCount);
        for (int rid = 0; rid < dataCount; rid++)
        {
            for (int cid = 0; cid <= rid; cid++)
            {
                double v = 0.0;
                int rBaseIndex = rid * dim;
                int cBaseIndex = cid * dim;
                for (int did = 0; did < dim; did++)
                {
                    v += sourceData.at(rBaseIndex + did) * sourceData.at(cBaseIndex + did) * 2.0;
                }
                square2X.at(rid * dataCount + cid) = v;
                square2X.at(cid * dataCount + rid) = v;
            }
        }
        MagicMath::DenseMatrix square2XMat(dataCount, dataCount, square2X);
        square2X.clear();
        MagicMath::DenseMatrix eMat(dataCount, dataCount, 1);
        MagicMath::DenseMatrix zMat(dataCount, dataCount, 0);
        MagicMath::DenseMatrix zNewMat(dataCount, dataCount, 0);
        MagicMath::DenseMatrix dMat(dataCount, dataCount, 0);
        MagicMath::DenseMatrix deltaFMat(dataCount, dataCount, 0);
        MagicMath::DenseMatrix deltaNewFMat(dataCount, dataCount, 0);
        double theta = 1.0;
        double tao1 = 1.0;
        double tao2 = 1.0;
        int maxIterCount = 100;
        for (int iterId = 0; iterId < maxIterCount; iterId++)
        {
            //Calculate matDeltaF
            deltaFMat = square2XMat * zMat - square2XMat + zMat * eMat * (2 * lamda); 
            //Calculate matD
            MagicMath::DenseMatrix matTemp = zMat - deltaFMat * theta;
            std::vector<double> projData = matTemp.GetValues();
            for (std::vector<double>::iterator itr = projData.begin(); itr != projData.end(); itr++)
            {
                if (*itr < 0)
                {
                    *itr = 0;
                }
            }
            for (int digId = 0; digId < dataCount; digId++)
            {
                projData.at(digId * dataCount + digId) = 0;
            }
            dMat = matTemp - zMat;
            //Calculate ro
            //double ro = 0;
            MagicMath::DenseMatrix xxdMat = square2XMat * dMat * 0.5;
            MagicMath::DenseMatrix deMat = dMat * eMat;
            MagicMath::DenseMatrix dTMat = dMat.Transpose();
            double a = (dTMat * xxdMat).Trace() + lamda * (dTMat * deMat).Trace();
            dTMat.Reset();
            MagicMath::DenseMatrix zTMat = zMat.Transpose();
            double b = 2 * (zTMat * xxdMat).Trace() - 2 * xxdMat.Trace() + 2 * lamda * (zTMat * deMat).Trace();
            zTMat.Reset();
            double ro = -b / (2.0 * a);
            //Calculate adjNew
            zNewMat = zMat + dMat * ro;
            //Update theta
            deltaNewFMat = square2XMat * zNewMat - square2XMat + zNewMat * eMat * (2 * lamda); 
            MagicMath::DenseMatrix sMat = zNewMat - zMat;
            MagicMath::DenseMatrix yMat = deltaNewFMat - deltaFMat;
            theta = yMat.InnerProduct(yMat) / sMat.InnerProduct(yMat);
            //test whether to stop
        }
        //copy result
        adjMat = zNewMat.GetValues();
    }

    void Clustering::FindKMeansSeeds(const std::vector<double>& sourceData, int dim, int k, std::vector<double>& seedData)
    {
        int sourceCount = sourceData.size() / dim;
        std::vector<bool> sampleFlag(sourceCount, 0);
        std::vector<int> sampleIndex(k);
        sampleFlag.at(0) = true;
        sampleIndex.at(0) = 0;
        std::vector<double> minDist(sourceCount, 1.0e100);
        int curIndex = 0;
        for (int sid = 1; sid < k; ++sid)
        {
            double maxDist = -1;
            int pos = -1;
            int baseCur = curIndex * dim;
            for (int vid = 0; vid < sourceCount; ++vid)
            {
                if (sampleFlag.at(vid))
                {
                    continue;
                }
                double dist = 0;
                int baseVid = vid * dim; 
                for (int did = 0; did < dim; did++)
                {
                    double dTemp = sourceData.at(baseCur + did) - sourceData.at(baseVid + did);
                    dist += dTemp * dTemp;
                }
                if (dist < minDist.at(vid))
                {
                    minDist.at(vid) = dist;
                }
                if (minDist.at(vid) > maxDist)
                {
                    maxDist = minDist.at(vid);
                    pos = vid;
                }
            }
            sampleIndex.at(sid) = pos;
            curIndex = pos;
            sampleFlag.at(pos) = true;
        }
        seedData.clear();
        seedData.resize(k * dim);
        for (int sid = 0; sid < k; sid++)
        {
            int baseSource = sampleIndex.at(sid) * dim;
            int baseSeed = sid * dim;
            for (int did = 0; did < dim; did++)
            {
                seedData.at(baseSeed + did) = sourceData.at(baseSource + did);
            }
        }
    }

    double Clustering::KMeansDistance(const std::vector<double>& sourcedata, int dim, int dataIndex, const std::vector<double>& centerData, int centerIndex)
    {
        double dist = 0;
        int sourceBase = dataIndex * dim;
        int centerBase = centerIndex * dim;
        for (int did = 0; did < dim; did++)
        {
            double dTemp = sourcedata.at(sourceBase + did) - centerData.at(centerBase + did);
            dist += dTemp * dTemp;
        }
        return dist;
    }
}
