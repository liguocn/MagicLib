#pragma once
#include <vector>

namespace MagicMath
{
    class DenseMatrix
    {
    public:
        DenseMatrix();
        DenseMatrix(int rowNum, int colNum, double initValue);
        DenseMatrix(int rowNum, int colNum, const std::vector<double>& data);
        ~DenseMatrix();

        DenseMatrix  operator + (const DenseMatrix& mat) const;
        DenseMatrix  operator - (const DenseMatrix& mat) const;
        DenseMatrix  operator * (double s) const;
        DenseMatrix  operator * (const DenseMatrix& mat) const;
        DenseMatrix& operator += (const DenseMatrix& mat);
        DenseMatrix& operator -= (const DenseMatrix& mat);
        DenseMatrix& operator *= (double s);
        DenseMatrix& operator *= (const DenseMatrix& mat);
        double InnerProduct(const DenseMatrix& mat) const;

        void Reset(void);
        void UnitMatrix(int matSize);
        DenseMatrix Transpose(void) const;
        int GetRowNum(void) const;
        int GetColNum(void) const;
        double at(int rid, int cid) const;
        double& at(int rid, int cid);
        const std::vector<double>& GetValues(void) const;
        std::vector<double>& GetValues(void);
        double Trace(void) const;

    private:
        int mRowNum;
        int mColNum;
        std::vector<double> mValues;
    };


}
