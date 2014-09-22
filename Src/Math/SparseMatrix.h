#pragma once
#include <vector>

namespace MagicMath
{
    class SparseMatrix
    {
    public:
        SparseMatrix();
        explicit SparseMatrix(const std::vector<double>& values, const std::vector<int>& colIndex,
                              const std::vector<int>& rowIndexOfLastValid);
        ~SparseMatrix();
        
        void Reset(void);
        void Set(const std::vector<double>& values, const std::vector<int>& colIndex,
                 const std::vector<int>& rowIndexOfLastValid);
        std::vector<double> Multiply(const std::vector<double>& vec) const;
        
    private:
        std::vector<int> mColIndex;
        std::vector<int> mRowIndexOfLastValid;
        std::vector<double> mValues;
    };
}
