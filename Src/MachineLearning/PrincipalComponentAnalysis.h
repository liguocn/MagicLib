#pragma once
#include <vector>
#include <string>
#include "Eigen/Dense"

namespace MagicML
{
    class PrincipalComponentAnalysis
    {
    public:
        PrincipalComponentAnalysis();
        ~PrincipalComponentAnalysis();

        int Analyse(const std::vector<double>& data, int dataDim, int pcaDim);
        int Analyse(const std::vector<double>& data, int dataDim, double pcaPercentage, int& pcaDim);
        std::vector<double> GetEigenVector(int k);
        double GetEigenValue(int k);
        std::vector<double> GetMeanVector(void);
        std::vector<double> Project(const std::vector<double>& data);
        std::vector<double> TruncateProject(const std::vector<double>& data, double truncateCoef);
        std::vector<double> TruncateFitting(const std::vector<double>& data, const std::vector<int>& dataIndex, double truncateCoef);
        void Load(const std::string& fileName);
        void Save(const std::string& fileName);
        void Clear(void);

    private:
        int ComputePcaData(const std::vector<double>& data, int dataDim, Eigen::MatrixXd& mat);

    private:
        int mDataDim;
        int mPcaDim;
        std::vector<double> mEigenVectors;
        std::vector<double> mEigenValues;
        std::vector<double> mMeanVector;
    };
}
