#include "PrincipalComponentAnalysis.h"
#include "Eigen/Eigenvalues"
#include "../Tool/LogSystem.h"

namespace MagicML
{
    PrincipalComponentAnalysis::PrincipalComponentAnalysis() :
        mDataDim(0),
        mPcaDim(0),
        mEigenVectors(),
        mEigenValues(),
        mMeanVector()
    {
    }

    PrincipalComponentAnalysis::~PrincipalComponentAnalysis()
    {
    }

    void PrincipalComponentAnalysis::Analyse(const std::vector<double>& data, int dataDim, int pcaDim)
    {
        Clear();
        mDataDim = dataDim;
        mPcaDim = pcaDim;
        int dataCount = data.size() / dataDim;
        DebugLog << "dataCount: " << dataCount << std::endl;
        mMeanVector = std::vector<double>(dataDim, 0.0);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int baseIndex = dataId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                mMeanVector.at(dim) += data.at(baseIndex + dim);
            }
        }
        for (int dim = 0; dim < dataDim; dim++)
        {
            mMeanVector.at(dim) /= dataCount;
        }
        Eigen::MatrixXd mat(dataDim, dataDim);
        for (int rid = 0; rid < dataDim; rid++)
        {
            for (int cid = 0; cid < dataDim; cid++)
            {
                mat(rid, cid) = 0.0;
            }
        }
        std::vector<double> deltaData(dataDim);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int baseIndex = dataId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                deltaData.at(dim) = data.at(baseIndex + dim) - mMeanVector.at(dim);
            }
            for (int rid = 0; rid < dataDim; rid++)
            {
                for (int cid = 0; cid < dataDim; cid++)
                {
                    mat(rid, cid) += ( deltaData.at(rid) * deltaData.at(cid) );
                }
            }
        }
        mat = mat / dataCount;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(mat);
        mEigenValues.resize(pcaDim);
        mEigenVectors.resize(pcaDim * dataDim);
        for (int pcaId = 0; pcaId < pcaDim; pcaId++)
        {
            mEigenValues.at(pcaId) = es.eigenvalues()(dataDim - 1 - pcaId);
            Eigen::VectorXd eigVec = es.eigenvectors().col(dataDim - 1 - pcaId);
            int baseIndex = pcaId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                mEigenVectors.at(baseIndex + dim) = eigVec(dim);
            }
        }
    }

    std::vector<double> PrincipalComponentAnalysis::GetEigenVector(int k)
    {
        std::vector<double> eigenVec(mDataDim);
        int baseIndex = mDataDim * k;
        for (int did = 0; did < mDataDim; did++)
        {
            eigenVec.at(did) = mEigenVectors.at(baseIndex + did);
        }
        return eigenVec;
    }

    double PrincipalComponentAnalysis::GetEigenValue(int k)
    {
        return mEigenValues.at(k);
    }

    std::vector<double> PrincipalComponentAnalysis::GetMeanVector(void)
    {
        return mMeanVector;
    }

    void PrincipalComponentAnalysis::Clear(void)
    {
        mDataDim = 0;
        mPcaDim = 0;
        mEigenVectors.clear();
        mEigenValues.clear();
        mMeanVector.clear();
    }

    std::vector<double> PrincipalComponentAnalysis::Project(const std::vector<double>& data)
    {
        std::vector<double> deltaVec(mDataDim);
        std::vector<double> projectVec(mDataDim);
        for (int dim = 0; dim < mDataDim; dim++)
        {
            projectVec.at(dim) = mMeanVector.at(dim);
            deltaVec.at(dim) = data.at(dim) - mMeanVector.at(dim);
        }
        for (int pcaId = 0; pcaId < mPcaDim; pcaId++)
        {
            double coef = 0.0;
            int baseIndex = pcaId * mDataDim;
            for (int dim = 0; dim < mDataDim; dim++)
            {
                coef += deltaVec.at(dim) * mEigenVectors.at(baseIndex + dim);
            }
            for (int dim = 0; dim < mDataDim; dim++)
            {
                projectVec.at(dim) += mEigenVectors.at(baseIndex + dim) * coef;
            }
        }
        return projectVec;
    }

    std::vector<double> PrincipalComponentAnalysis::TruncateProject(const std::vector<double>& data, double truncateCoef)
    {
        std::vector<double> deltaVec(mDataDim);
        std::vector<double> projectVec(mDataDim);
        for (int dim = 0; dim < mDataDim; dim++)
        {
            projectVec.at(dim) = mMeanVector.at(dim);
            deltaVec.at(dim) = data.at(dim) - mMeanVector.at(dim);
        }
        for (int pcaId = 0; pcaId < mPcaDim; pcaId++)
        {
            double coef = 0.0;
            int baseIndex = pcaId * mDataDim;
            for (int dim = 0; dim < mDataDim; dim++)
            {
                coef += deltaVec.at(dim) * mEigenVectors.at(baseIndex + dim);
            }
            double maxCoef = truncateCoef * sqrt(mEigenValues.at(pcaId));
            coef = coef > maxCoef ? maxCoef : (coef < -maxCoef ? -maxCoef : coef);
            for (int dim = 0; dim < mDataDim; dim++)
            {
                projectVec.at(dim) += mEigenVectors.at(baseIndex + dim) * coef;
            }
        }
        return projectVec;
    }

    void PrincipalComponentAnalysis::Load(const std::string& fileName)
    {
        Clear();
        std::ifstream fin(fileName);
        fin >> mDataDim >> mPcaDim;
            
        mEigenValues.resize(mPcaDim);
        for (int vid = 0; vid < mPcaDim; vid++)
        {
            fin >> mEigenValues.at(vid);
        }

        int vSize = mDataDim * mPcaDim;
        mEigenVectors.resize(vSize);
        for (int vid = 0; vid < vSize; vid++)
        {
            fin >> mEigenVectors.at(vid);
        }
            
        mMeanVector.resize(mDataDim);
        for (int vid = 0; vid < mDataDim; vid++)
        {
            fin >> mMeanVector.at(vid);
        }
        fin.close();
    }

    void PrincipalComponentAnalysis::Save(const std::string& fileName)
    {
        std::ofstream fout(fileName);
        fout << mDataDim << " " << mPcaDim << std::endl;
            
        for (int vid = 0; vid < mPcaDim; vid++)
        {
            fout << mEigenValues.at(vid) << std::endl;
        }

        int vSize = mDataDim * mPcaDim;
        for (int vid = 0; vid < vSize; vid++)
        {
            fout << mEigenVectors.at(vid) << " ";
        }
            
        for (int vid = 0; vid < mDataDim; vid++)
        {
            fout << mMeanVector.at(vid) << " ";
        }
        fout.close();
    }
}
