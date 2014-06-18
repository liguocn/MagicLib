#include "LinearDiscriminantAnalysis.h"
#include <map>
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "Eigen/Eigenvalues"
#include "Eigen/Dense"

namespace MagicML
{
    LinearDiscriminantAnalysis::LinearDiscriminantAnalysis() :
        mDataDim(0),
        mLdaDim(0),
        mLdaVectors()
    {
        
    }
    
    LinearDiscriminantAnalysis::~LinearDiscriminantAnalysis()
    {
        Reset();
    }
    
    void LinearDiscriminantAnalysis::Reset(void)
    {
        mDataDim = 0;
        mLdaDim = 0;
        mLdaVectors.clear();
    }
    
    int LinearDiscriminantAnalysis::Analyse(const std::vector<double>& dataX, const std::vector<int>& dataY, int ldaDim)
    {
        Reset();
        int dataCount = dataY.size();
        if (dataCount == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        mDataDim = dataX.size() / dataCount;
        mLdaDim = ldaDim;
        if (mDataDim * dataY.size() != dataX.size())
        {
            return MAGIC_INVALID_INPUT;
        }
        std::map<int, std::vector<int> > catogeryMap;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            catogeryMap[dataY.at(dataId)].push_back(dataId);
        }
        int catCount = catogeryMap.size();
        if (ldaDim > catCount - 1)
        {
            return MAGIC_INVALID_INPUT;
        }
        Eigen::MatrixXd matB(mDataDim, mDataDim);
        Eigen::MatrixXd matW(mDataDim, mDataDim);
        for (int rid = 0; rid < mDataDim; rid++)
        {
            for (int cid = 0; cid < mDataDim; cid++)
            {
                matB(rid, cid) = 0.0;
                matW(rid, cid) = 0.0;
            }
        }
        std::vector<double> meanVecList(mDataDim * catCount, 0);
        std::vector<int> catoNumList(catCount);
        int catId = 0;
        for (std::map<int, std::vector<int> >::iterator mapItr = catogeryMap.begin(); mapItr != catogeryMap.end(); mapItr++)
        {
            int meanBaseId = catId * mDataDim;
            for (std::vector<int>::iterator vecItr = mapItr->second.begin(); vecItr != mapItr->second.end(); vecItr++)
            {
                int vecBaseIndex = *vecItr * mDataDim;
                for (int did = 0; did < mDataDim; did++)
                {
                    meanVecList.at(meanBaseId + did) += dataX.at(vecBaseIndex + did);
                }
            }
            catoNumList.at(catId) = mapItr->second.size();
            for (int did = 0; did < mDataDim; did++)
            {
                meanVecList.at(meanBaseId + did) /= catoNumList.at(catId);
            }
            std::vector<double> deltaData(mDataDim);
            for (std::vector<int>::iterator vecItr = mapItr->second.begin(); vecItr != mapItr->second.end(); vecItr++)
            {
                int vecBaseIndex = *vecItr * mDataDim;
                for (int did = 0; did < mDataDim; did++)
                {
                    deltaData.at(did) = dataX.at(vecBaseIndex + did) - meanVecList.at(meanBaseId + did);
                }
                for (int rid = 0; rid < mDataDim; rid++)
                {
                    for (int cid = 0; cid < mDataDim; cid++)
                    {
                        matW(rid, cid) += ( deltaData.at(rid) * deltaData.at(cid) );
                    }
                }
            }
            catId++;
        }
        matW = matW / dataCount;
        //regularization of matW
        double lamda = 1.0e-15;
        for (int did = 0; did < mDataDim; did++)
        {
            matW(did, did) += lamda;
        }

        std::vector<double> totalMeanVec(mDataDim, 0);
        for (int catoId = 0; catoId < catCount; catoId++)
        {
            int meanBaseId = catoId * mDataDim;
            for (int did = 0; did < mDataDim; did++)
            {
                totalMeanVec.at(did) += meanVecList.at(meanBaseId + did) * catoNumList.at(catoId);
            }
        }
        for (int did = 0; did < mDataDim; did++)
        {
            totalMeanVec.at(did) /= dataCount;
        }
        std::vector<double> meanDeltaData(mDataDim);
        for (int catoId = 0; catoId < catCount; catoId++)
        {
            int meanBaseId = catoId * mDataDim;
            for (int did = 0; did < mDataDim; did++)
            {
                meanDeltaData.at(did) = meanVecList.at(meanBaseId + did) - totalMeanVec.at(did);
            }
            for (int rid = 0; rid < mDataDim; rid++)
            {
                for (int cid = 0; cid < mDataDim; cid++)
                {
                    matB(rid, cid) += ( meanDeltaData.at(rid) * meanDeltaData.at(cid) * catoNumList.at(catoId) );
                }
            }
        }
        matB = matB / dataCount;
        
        Eigen::MatrixXd matWRev = matW.inverse();
        Eigen::MatrixXd matEigen = matWRev * matB;
        Eigen::EigenSolver<Eigen::MatrixXd> es(matEigen);
        //sort eigen values
        std::vector<int> sortIndex(mDataDim);
        for (int dataId = 0; dataId < mDataDim; dataId++)
        {
            sortIndex.at(dataId) = dataId;
        }
        for (int sortId = 0; sortId < mLdaDim; sortId++)
        {
            int maxId = sortId;
            double maxV = -1;
            for (int dataId = sortId; dataId < mDataDim; dataId++)
            {
                if (es.eigenvalues()(sortIndex.at(dataId)).real() > maxV)
                {
                    maxV = es.eigenvalues()(sortIndex.at(dataId)).real();
                    maxId = dataId;
                }
            }
            int nTemp = sortIndex.at(sortId);
            sortIndex.at(sortId) = sortIndex.at(maxId);
            sortIndex.at(maxId) = nTemp;
        }
        mLdaVectors.resize(mLdaDim * mDataDim);
        for (int ldaId = 0; ldaId < mLdaDim; ldaId++)
        {
            Eigen::VectorXcd eigVec = es.eigenvectors().col(sortIndex.at(ldaId));
            int baseIndex = ldaId * mDataDim;
            for (int did = 0; did < mDataDim; did++)
            {
                mLdaVectors.at(baseIndex + did) = eigVec(did).real();
            }
        }

        return MAGIC_NO_ERROR;
    }
    
    std::vector<double> LinearDiscriminantAnalysis::GetLdaVector(int k) const
    {
        std::vector<double> ldaVec(mDataDim);
        int baseIndex = mDataDim * k;
        for (int did = 0; did < mDataDim; did++)
        {
            ldaVec.at(did) = mLdaVectors.at(baseIndex + did);
        }
        return ldaVec;
    }

    std::vector<double> LinearDiscriminantAnalysis::Project(const std::vector<double>& data) const
    {
        std::vector<double> projVec(mLdaDim, 0);
        for (int ldaId = 0; ldaId < mLdaDim; ldaId++)
        {
            int baseIndex = ldaId * mDataDim;
            for (int did = 0; did < mDataDim; did++)
            {
                projVec.at(ldaId) += data.at(did) * mLdaVectors.at(baseIndex + did);
            }
        }
        return projVec;
    }
}