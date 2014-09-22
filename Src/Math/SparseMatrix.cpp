#include "SparseMatrix.h"

namespace MagicMath
{
    SparseMatrix::SparseMatrix() :
        mColIndex(),
        mRowIndexOfLastValid(),
        mValues()
    {
    }
    
    SparseMatrix::SparseMatrix(const std::vector<double>& values, const std::vector<int>& colIndex,
                               const std::vector<int>& rowIndexOfLastValid) :
        mColIndex(colIndex),
        mRowIndexOfLastValid(rowIndexOfLastValid),
        mValues(values)
    {
    }

    SparseMatrix::~SparseMatrix()
    {
        Reset();
    }
    
    void SparseMatrix::Reset(void)
    {
        mColIndex.clear();
        mRowIndexOfLastValid.clear();
        mValues.clear();
    }
    
    void SparseMatrix::Set(const std::vector<double>& values, const std::vector<int>& colIndex,
             const std::vector<int>& rowIndexOfLastValid)
    {
        mColIndex = colIndex;
        mRowIndexOfLastValid = rowIndexOfLastValid;
        mValues = values;
    }
    
    std::vector<double> SparseMatrix::Multiply(const std::vector<double>& vec) const
    {
        int rowNum = mRowIndexOfLastValid.size();
        std::vector<double> res(rowNum, 0.0);
        if (rowNum > 0)
        {
            double rowValue = 0.0;
            for (int colId = 0; colId <= mRowIndexOfLastValid.at(0); colId++)
            {
                rowValue += mValues.at(colId) * vec.at(mColIndex.at(colId));
            }
            res.at(0) = rowValue;
        }
        for (int rowId = 1; rowId < rowNum; rowId++)
        {
            int startId = mRowIndexOfLastValid.at(rowId - 1) + 1;
            int endId = mRowIndexOfLastValid.at(rowId);
            double rowValue = 0.0;
            for (int colId = startId; colId <= endId; colId++)
            {
                rowValue += mValues.at(colId) * vec.at(mColIndex.at(colId));
            }
            res.at(rowId) = rowValue;
        }
        return res;
    }

}