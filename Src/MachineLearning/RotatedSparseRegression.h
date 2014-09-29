#pragma once
#include <vector>

namespace MagicMath
{
    class DenseMatrix;
    class SparseMatrix;
}

namespace MagicML
{
    class RotatedSparseRegression
    {
    public:
        RotatedSparseRegression();
        ~RotatedSparseRegression();

        int Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, double lamda);
        int Predict(const std::vector<double>& dataX, std::vector<double>& dataY) const;
        void Reset(void);

    private:
        int StanderdizeDataX(const std::vector<double>& dataX, int dataCount, std::vector<double>& standDataX);
        int CalVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, double lamda, 
            bool isInitialized, std::vector<double>& vecBList) const;
        int CalInitVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, double lamda,
            std::vector<double>& vecBList) const;
        void UpdateVectorBList(const std::vector<double>& dataX, const std::vector<double>& dataY, int yId, int dataCount, 
            double lamda, std::vector<double>& vecBList) const;
        bool IsVectorBListConverged(const std::vector<double>& dataX, const std::vector<double>& dataY, int yId, int dataCount,
            double lamda, const std::vector<double>& vecBList, double& lastEnerge) const;
        double FunctionS(double z, double gama) const;
        bool IsTrainingErrorConverged(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, double lamda, 
            std::vector<double>& vecList, const MagicMath::DenseMatrix* pMatR) const;
        int CalMatirxR(const std::vector<double>& dataX, const std::vector<double>& dataY, int dataCount, 
            const std::vector<double>& vecBList, MagicMath::DenseMatrix* pMatR) const;

    private:
        MagicMath::SparseMatrix* mpRegressionMat;
        std::vector<double> mMeans;
        std::vector<double> mVariances;
    };
}
