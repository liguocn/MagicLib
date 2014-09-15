#include "GaussianMixtureModel.h"
#include "Clustering.h"
#include "Eigen/Dense"
#include "../Tool/LogSystem.h"
#include "../Tool/ErrorCodes.h"

namespace MagicML
{
    GaussianModel::GaussianModel()
    {
    }

    GaussianModel::~GaussianModel()
    {
    }

    void GaussianModel::CalParameter(const std::vector<double>& inputData, int dim, const std::vector<int>& inputIndex)
    {
        int dataCount = inputData.size() / dim;
        int clusterDataCount = inputIndex.size();
        DebugLog << "dataCount: " << dataCount << " clusterDataCount:" << clusterDataCount << std::endl;
        mWeight = (double)clusterDataCount / (double)dataCount;
        mMeanVector = std::vector<double>(dim, 0.0);
        for (int vid = 0; vid < clusterDataCount; vid++)
        {
            int baseIndex = inputIndex.at(vid) * dim;
            for (int did = 0; did < dim; did++)
            {
                mMeanVector.at(did) += inputData.at(baseIndex + did);
            }
        }
        for (int did = 0; did < dim; did++)
        {
            mMeanVector.at(did) /= (double)clusterDataCount;
        }
        Eigen::MatrixXd covMat(dim, dim);
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                covMat(rid, cid) = 0.0;
            }
        }
        for (int vid = 0; vid < clusterDataCount; vid++)
        {
            int baseIndex = inputIndex.at(vid) * dim;
            for (int rid = 0; rid < dim; rid++)
            {
                for (int cid = 0; cid < dim; cid++)
                {
                    covMat(rid, cid) += ( (inputData.at(baseIndex + rid) - mMeanVector.at(rid)) 
                        * (inputData.at(baseIndex + cid) - mMeanVector.at(cid)) );
                }
            }
        }
        DebugLog << "covMat:" << std::endl;
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                covMat(rid, cid) /= (double)clusterDataCount;
                DebugLog << covMat(rid, cid) << " ";
            }
            DebugLog << std::endl;
        }
        double epsilon = 1.0e-7;
        for (int did = 0; did < dim; did++)
        {
            covMat(did, did) += epsilon;
        }
        mCovDet = covMat.determinant();
        Eigen::MatrixXd inverseCovMat = covMat.inverse();
        mInverseCovMat.clear();
        mInverseCovMat.resize(dim * dim);
        int matIndex = 0;
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                mInverseCovMat.at(matIndex) = inverseCovMat(rid, cid);
                matIndex++;
            }
        }
        PrintParameter();
    }

    double GaussianModel::Pro(const std::vector<double>& data) const
    {
        int dim = mMeanVector.size();
        std::vector<double> difVector(dim);
        for (int did = 0; did < dim; did++)
        {
            difVector.at(did) = data.at(did) - mMeanVector.at(did);
        }
        std::vector<double> interVec(dim, 0.0);
        for (int rid = 0; rid < dim; rid++)
        {
            int baseIndex = rid * dim;
            for (int did = 0; did < dim; did++)
            {
                interVec.at(rid) += mInverseCovMat.at(baseIndex + did) * difVector.at(did);
            }
        }
        double interValue = 0.0;
        for (int rid = 0; rid < dim; rid++)
        {
            interValue += interVec.at(rid) * difVector.at(rid);
        }
        double res = exp(interValue * (-0.5)) / sqrt(2 * 3.14159265 * fabs(mCovDet));
        if (res > 1)
        {
            DebugLog << "pro: " << res << std::endl;
        }
        return res;
    }

    double GaussianModel::WeightPro(const std::vector<double>& data) const
    {
        return Pro(data) * mWeight;
    }

    double GaussianModel::GetWeight(void) const
    {
        return mWeight;
    }

    const std::vector<double>& GaussianModel::GetMeanVector(void) const
    {
        return mMeanVector;
    }

    void GaussianModel::PrintParameter()
    {
        DebugLog << "GMM:" << std::endl;
        DebugLog << "Weight: " << mWeight << " Det: " << mCovDet << std::endl;
        DebugLog << "MeanVector: ";
        int dim = mMeanVector.size();
        for (int did = 0; did < dim; did++)
        {
            DebugLog << mMeanVector.at(did) << " ";
        }
        DebugLog << std::endl << "InverseMat: " << std::endl;
        int matIndex = 0;
        for (int rid = 0; rid < dim; rid++)
        {
            for (int cid = 0; cid < dim; cid++)
            {
                DebugLog << mInverseCovMat.at(matIndex) << " ";
                matIndex++;
            }
            DebugLog << std::endl;
        }
    }

    GaussianMixtureModel::GaussianMixtureModel()
    {
    }

    GaussianMixtureModel::~GaussianMixtureModel()
    {
        Clear();
    }

    int GaussianMixtureModel::CalParameter(const std::vector<double>& inputData, int dim, int k, const std::vector<int>& clusterIndex)
    {
        Clear();
        int dataCount = clusterIndex.size();
        std::vector<std::vector<int> > clusterIndexList(k);
        for (int vid = 0; vid < dataCount; vid++)
        {
            clusterIndexList.at(clusterIndex.at(vid)).push_back(vid);
        }
        mGaussianList.resize(k);
        for (int cid = 0; cid < k; cid++)
        {
            GaussianModel* pGM = new GaussianModel;
            pGM->CalParameter(inputData, dim, clusterIndexList.at(cid));
            mGaussianList.at(cid) = pGM;
        }
        return MAGIC_NO_ERROR;
    }

    double GaussianMixtureModel::Pro(const std::vector<double>& data)
    {
        double proV = 0.0;
        for (std::vector<GaussianModel* >::iterator itr = mGaussianList.begin(); itr != mGaussianList.end(); ++itr)
        {
            proV += (*itr)->WeightPro(data);
        }
        return proV;
    }

    double GaussianMixtureModel::ComponentPro(const std::vector<double>& data, int componentId)
    {
        return mGaussianList.at(componentId)->Pro(data);
    }

    int GaussianMixtureModel::GetClusterId(const std::vector<double>& data)
    {
        double maxPro = -1;
        int maxIndex = -1;
        int k = mGaussianList.size();
        for (int cid = 0; cid < k; cid++)
        {
            double proV = mGaussianList.at(cid)->Pro(data);
           // double proV = mGaussianList.at(cid)->WeightPro(data);
            if (proV > maxPro)
            {
                maxPro = proV;
                maxIndex = cid;
            }
        }
        return maxIndex;
    }

    void GaussianMixtureModel::Clear()
    {
        for (std::vector<GaussianModel* >::iterator itr = mGaussianList.begin(); itr != mGaussianList.end(); ++itr)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mGaussianList.clear();
    }
}
