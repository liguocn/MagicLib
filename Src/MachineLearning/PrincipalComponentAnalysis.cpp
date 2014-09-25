#include "PrincipalComponentAnalysis.h"
#include "Eigen/Eigenvalues"
#include "../Tool/LogSystem.h"
#include "../Tool/Profiler.h"
#include "../Tool/ErrorCodes.h"

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

    int PrincipalComponentAnalysis::ComputePcaData(const std::vector<double>& data, int dataDim, Eigen::MatrixXd& mat)
    {
        if (data.size() < dataDim || dataDim < 1 )
        {
            return MAGIC_INVALID_INPUT;
        }
        Clear();
        mDataDim = dataDim;
        int dataCount = data.size() / dataDim;
        mMeanVector = std::vector<double>(dataDim, 0.0);
        for (long long dataId = 0; dataId < dataCount; dataId++)
        {
            long long baseIndex = dataId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                mMeanVector.at(dim) += data.at(baseIndex + dim);
            }
        }
        for (int dim = 0; dim < dataDim; dim++)
        {
            mMeanVector.at(dim) /= dataCount;
        }
        //Eigen::MatrixXd mat(dataDim, dataDim);
        double emptyStartTime = MagicTool::Profiler::GetTime();
        for (int rid = 0; rid < dataDim; rid++)
        {
            for (int cid = 0; cid < dataDim; cid++)
            {
                mat(rid, cid) = 0.0;
            }
        }
        DebugLog << "pca calculate deltaData: " << MagicTool::Profiler::GetTime() - emptyStartTime << std::endl;
        std::vector<double> deltaData(dataDim);
        for (long long dataId = 0; dataId < dataCount; dataId++)
        {
            double startTime = MagicTool::Profiler::GetTime();
            long long baseIndex = dataId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                deltaData.at(dim) = data.at(baseIndex + dim) - mMeanVector.at(dim);
            }
            for (int rid = 0; rid < dataDim; rid++)
            {
                for (int cid = rid; cid < dataDim; cid++)
                {
                    double dTemp = deltaData.at(rid) * deltaData.at(cid);
                    mat(rid, cid) += dTemp;
                }
            }
            DebugLog << "pca dataId: " << dataId << " time: " << MagicTool::Profiler::GetTime() - startTime << std::endl;
        }
        for (int rid = 0; rid < dataDim; rid++)
        {
            for (int cid = 0; cid < rid; cid++)
            {
                mat(rid, cid) = mat(cid, rid);
            }
        }
        mat = mat / dataCount;

        return MAGIC_NO_ERROR;
    }

    int PrincipalComponentAnalysis::Analyse(const std::vector<double>& data, int dataDim, int pcaDim)
    { 
        Eigen::MatrixXd mat(dataDim, dataDim);
        int res = ComputePcaData(data, dataDim, mat);
        if (res != MAGIC_NO_ERROR)
        {
            Clear();
            return res;
        }
        DebugLog << "pca EigenSoler..." << std::endl;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(mat);
        DebugLog << "              finished" << std::endl;
        
        mPcaDim = pcaDim;
        mEigenValues.resize(pcaDim);
        mEigenVectors.resize(pcaDim * dataDim);
        for (int pcaId = 0; pcaId < dataDim; pcaId++)
        {
            DebugLog << "pca eigen value " << pcaId << ": " << es.eigenvalues()(dataDim - 1 - pcaId) << std::endl;
        } 
        for (long long pcaId = 0; pcaId < pcaDim; pcaId++)
        {
            mEigenValues.at(pcaId) = es.eigenvalues()(dataDim - 1 - pcaId);
            Eigen::VectorXd eigVec = es.eigenvectors().col(dataDim - 1 - pcaId);
            long long baseIndex = pcaId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                mEigenVectors.at(baseIndex + dim) = eigVec(dim);
            }
        }

        return MAGIC_NO_ERROR;
    }

    int PrincipalComponentAnalysis::Analyse(const std::vector<double>& data, int dataDim, double pcaPercentage, int& pcaDim)
    {
        Eigen::MatrixXd mat(dataDim, dataDim);
        int res = ComputePcaData(data, dataDim, mat);
        if (res != MAGIC_NO_ERROR)
        {
            Clear();
            return res;
        }
        DebugLog << "pca EigenSoler..." << std::endl;
        Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(mat);
        DebugLog << "              finished" << std::endl;

        double eigenValueSum = 0;
        for (int pcaId = 0; pcaId < dataDim; pcaId++)
        {
            eigenValueSum += es.eigenvalues()(pcaId);
        }
        double eigenValueSumFlag = eigenValueSum * pcaPercentage;
        double eigenValueAcum = 0;
        pcaDim = -1;
        for (int pcaId = 0; pcaId < dataDim; pcaId++)
        {
            eigenValueAcum += es.eigenvalues()(dataDim - 1 - pcaId);
            if (eigenValueAcum > eigenValueSumFlag)
            {
                mPcaDim = pcaId + 1;
                pcaDim = mPcaDim;
                break;
            }
        }
        if (pcaDim == -1)
        {
            DebugLog << "Error: pcaDim == -1 " << std::endl;
        }
        mEigenValues.resize(pcaDim);
        mEigenVectors.resize(pcaDim * dataDim);
        for (int pcaId = 0; pcaId < pcaDim; pcaId++)
        {
            DebugLog << "pca eigen value " << pcaId << ": " << es.eigenvalues()(dataDim - 1 - pcaId) << std::endl;
        } 
        for (long long pcaId = 0; pcaId < pcaDim; pcaId++)
        {
            mEigenValues.at(pcaId) = es.eigenvalues()(dataDim - 1 - pcaId);
            Eigen::VectorXd eigVec = es.eigenvectors().col(dataDim - 1 - pcaId);
            long long baseIndex = pcaId * dataDim;
            for (int dim = 0; dim < dataDim; dim++)
            {
                mEigenVectors.at(baseIndex + dim) = eigVec(dim);
            }
        }

        return MAGIC_NO_ERROR;
    }

    std::vector<double> PrincipalComponentAnalysis::GetEigenVector(int k)
    {
        std::vector<double> eigenVec(mDataDim);
        long long baseIndex = static_cast<long long>(mDataDim) * k;
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
        for (long long pcaId = 0; pcaId < mPcaDim; pcaId++)
        {
            double coef = 0.0;
            long long baseIndex = pcaId * mDataDim;
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
        for (long long pcaId = 0; pcaId < mPcaDim; pcaId++)
        {
            double coef = 0.0;
            long long baseIndex = pcaId * mDataDim;
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

    std::vector<double> PrincipalComponentAnalysis::TruncateFitting(const std::vector<double>& data, 
        const std::vector<int>& dataIndex, double truncateCoef)
    {
        //fitting
        int dataDim = dataIndex.size();
        int solverDim = mPcaDim > dataDim ? dataDim : mPcaDim;
        Eigen::MatrixXd matA(dataDim, solverDim);
        Eigen::VectorXd vecB(dataDim, 1);
        for (long long rowId = 0; rowId < dataDim; rowId++)
        {
            int dataId = dataIndex.at(rowId);
            for (long long colId = 0; colId < solverDim; colId++)
            {
                matA(rowId, colId) = mEigenVectors.at(colId * mDataDim + dataId);
            }
            vecB(rowId) = data.at(rowId) - mMeanVector.at(dataId);
        }
        Eigen::MatrixXd matAT = matA.transpose();
        Eigen::MatrixXd matCoefA = matAT * matA;
        Eigen::MatrixXd vecCoefB = matAT * vecB;
        Eigen::VectorXd res = matCoefA.ldlt().solve(vecCoefB);
        
        //copy result
        std::vector<double> fitRes = mMeanVector;
        for (int pcaId = 0; pcaId < solverDim; pcaId++)
        {
            double maxCoef = truncateCoef * sqrt(mEigenValues.at(pcaId));
            double coef = res(pcaId) > maxCoef ? maxCoef : (res(pcaId) < -maxCoef ? -maxCoef : res(pcaId));
            long long baseIndex = pcaId * mDataDim;
            for (int dimId = 0; dimId < mDataDim; dimId++)
            {
                fitRes.at(dimId) += mEigenVectors.at(baseIndex + dimId) * coef;
            }
        }

        return fitRes;
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

        long long vSize = mDataDim * mPcaDim;
        mEigenVectors.resize(vSize);
        for (long long vid = 0; vid < vSize; vid++)
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

        long long vSize = mDataDim * mPcaDim;
        for (long long vid = 0; vid < vSize; vid++)
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
