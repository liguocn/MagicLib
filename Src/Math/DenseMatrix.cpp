#include "DenseMatrix.h"

namespace MagicMath
{
    DenseMatrix::DenseMatrix() :
        mRowNum(0),
        mColNum(0),
        mValues()
    {
    }

    DenseMatrix::DenseMatrix(int rowNum, int colNum, double initValue) :
        mRowNum(rowNum),
        mColNum(colNum),
        mValues(rowNum * colNum, initValue)
    {
    }

    DenseMatrix::DenseMatrix(int rowNum, int colNum, const std::vector<double>& data) :
        mRowNum(rowNum),
        mColNum(colNum),
        mValues(data)
    {

    }

    DenseMatrix::~DenseMatrix()
    {
    }

    DenseMatrix DenseMatrix::operator + (const DenseMatrix& mat) const
    {
        DenseMatrix matRes(mRowNum, mColNum, 0);
        int dataCount = mRowNum * mColNum;
        std::vector<double>& dataRes = matRes.GetValues();
        const std::vector<double>& dataMat = mat.GetValues();
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            dataRes.at(dataId) = mValues.at(dataId) + dataMat.at(dataId);
        }
        return matRes;
    }

    DenseMatrix DenseMatrix::operator - (const DenseMatrix& mat) const
    {
        DenseMatrix matRes(mRowNum, mColNum, 0);
        int dataCount = mRowNum * mColNum;
        std::vector<double>& dataRes = matRes.GetValues();
        const std::vector<double>& dataMat = mat.GetValues();
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            dataRes.at(dataId) = mValues.at(dataId) - dataMat.at(dataId);
        }
        return matRes;
    }

    DenseMatrix DenseMatrix::operator * (double s) const
    {
        DenseMatrix matRes(mRowNum, mColNum, 0);
        int dataCount = mRowNum * mColNum;
        std::vector<double>& dataRes = matRes.GetValues();
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            dataRes.at(dataId) = mValues.at(dataId) * s;
        }
        return matRes;
    }

    DenseMatrix DenseMatrix::operator * (const DenseMatrix& mat) const
    {
        int rightColNum = mat.GetColNum();
        DenseMatrix matRes(mRowNum, rightColNum, 0);
        for (int rid = 0; rid < mRowNum; rid++)
        {
            for (int cid = 0; cid < rightColNum; cid++)
            {
                double v = 0.0;
                int rBaseIndex = rid * mColNum;
                for (int did = 0; did < mColNum; did++)
                {
                    v += mValues.at(rBaseIndex + did) * mat.at(did, cid);
                }
                matRes.at(rid, cid) = v;
            }
        }
        return matRes;
    }

    DenseMatrix& DenseMatrix::operator += (const DenseMatrix& mat)
    {
        int dataCount = mRowNum * mColNum;
        const std::vector<double>& matValues = mat.GetValues();
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            mValues.at(dataId) += matValues.at(dataId);
        }
        return *this;
    }

    DenseMatrix& DenseMatrix::operator -= (const DenseMatrix& mat)
    {
        int dataCount = mRowNum * mColNum;
        const std::vector<double>& matValues = mat.GetValues();
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            mValues.at(dataId) -= matValues.at(dataId);
        }
        return *this;
    }

    DenseMatrix& DenseMatrix::operator *= (double s)
    {
        int dataCount = mRowNum * mColNum;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            mValues.at(dataId) *= s;
        }
        return *this;
    }

    DenseMatrix& DenseMatrix::operator *= (const DenseMatrix& mat)
    {
        int rightColNum = mat.GetColNum();
        DenseMatrix matRes(mRowNum, rightColNum, 0);
        for (int rid = 0; rid < mRowNum; rid++)
        {
            for (int cid = 0; cid < rightColNum; cid++)
            {
                double v = 0.0;
                int rBaseIndex = rid * mColNum;
                for (int did = 0; did < mColNum; did++)
                {
                    v += mValues.at(rBaseIndex + did) * mat.at(did, cid);
                }
                matRes.at(rid, cid) = v;
            }
        }
        *this = matRes;
        return *this;
    }

    double DenseMatrix::InnerProduct(const DenseMatrix& mat) const
    {
        double res = 0;
        const std::vector<double>& dataMat = mat.GetValues();
        for (int dataId = 0; dataId < mValues.size(); dataId++)
        {
            res += mValues.at(dataId) * dataMat.at(dataId);
        }
        return res;
    }

    void DenseMatrix::Reset(void)
    {
        mRowNum = 0;
        mColNum = 0;
        mValues.clear();
    }

    void DenseMatrix::UnitMatrix(int matSize)
    {
        Reset();
        mRowNum = matSize;
        mColNum = matSize;
        mValues = std::vector<double>(matSize * matSize, 0);
        for (int rid = 0; rid < matSize; rid++)
        {
            mValues.at(rid * matSize + rid) = 1.0;
        }
    }

    DenseMatrix DenseMatrix::Transpose(void) const
    {
        DenseMatrix transMat(mColNum, mRowNum, 0);
        for (int rid = 0; rid < mRowNum; rid++)
        {
            int rBaseIndex = rid * mColNum;
            for (int cid = 0; cid < mColNum; cid++)
            {
                transMat.at(cid, rid) = mValues.at(rBaseIndex + cid);
            }
        }
        return transMat;
    }

    int DenseMatrix::GetRowNum(void) const
    {
        return mRowNum;
    }

    int DenseMatrix::GetColNum(void) const
    {
        return mColNum;
    }

    double DenseMatrix::at(int rid, int cid) const
    {
        return mValues.at(rid * mColNum + cid);
    }

    double& DenseMatrix::at(int rid, int cid)
    {
        return mValues.at(rid * mColNum + cid);
    }

    std::vector<double>& DenseMatrix::GetValues(void)
    {
        return mValues;
    }

    const std::vector<double>& DenseMatrix::GetValues(void) const
    {
        return mValues;
    }

    double DenseMatrix::Trace(void) const
    {
        double trace = 0;
        for (int rid = 0; rid < mRowNum; rid++)
        {
            trace += mValues.at(rid * mColNum + rid);
        }
        return trace;
    }
}
