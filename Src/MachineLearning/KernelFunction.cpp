#include "KernelFunction.h"
#include <math.h>

namespace MagicML
{
    KernelFunction::KernelFunction()
    {
    }

    KernelFunction::~KernelFunction()
    {
    }

    EuclidKernel::EuclidKernel()
    {
    }

    EuclidKernel::~EuclidKernel()
    {
    }

    double EuclidKernel::InnerProduct(const std::vector<double>& vec1, const std::vector<double>& vec2)
    {
        double res = 0;
        for (int i = 0; i < vec1.size(); i++)
        {
            res += vec1.at(i) * vec2.at(i);
        }
        return res;
    }

    double EuclidKernel::InnerProduct(const std::vector<double>& vec1, int startId, const std::vector<double>& vec2)
    {
        double res = 0;
        int dataDim = vec2.size();
        int startIndex1 = startId * dataDim;
        for (int i = 0; i < dataDim; i++)
        {
            res += vec1.at(startIndex1 + i) * vec2.at(i);
        }
        return res;
    }

    double EuclidKernel::InnerProduct(const std::vector<double>& vec, int startId1, int startId2, int dataDim)
    {
        double res = 0;
        int startIndex1 = startId1 * dataDim;
        int startIndex2 = startId2 * dataDim;
        for (int i = 0; i < dataDim; i++)
        {
            res += vec.at(startIndex1 + i) * vec.at(startIndex2 + i);
        }
        return res;
    }

    PolynomialKernel::PolynomialKernel(double offset, double degree) :
        mOffset(offset),
        mDegree(degree)
    {

    }

    PolynomialKernel::~PolynomialKernel()
    {

    }

    double PolynomialKernel::InnerProduct(const std::vector<double>& vec1, const std::vector<double>& vec2)
    {
        double res = 0;
        for (int i = 0; i < vec1.size(); i++)
        {
            res += vec1.at(i) * vec2.at(i);
        }
        res += mOffset; 
        res = pow(res, mDegree);
        return res;
    }

    double PolynomialKernel::InnerProduct(const std::vector<double>& vec1, int startId, const std::vector<double>& vec2)
    {
        double res = 0;
        int dataDim = vec2.size();
        int startIndex1 = startId * dataDim;
        for (int i = 0; i < dataDim; i++)
        {
            res += vec1.at(startIndex1 + i) * vec2.at(i);
        }
        res += mOffset; 
        res = pow(res, mDegree);
        return res;
    }

    double PolynomialKernel::InnerProduct(const std::vector<double>& vec, int startId1, int startId2, int dataDim)
    {
        double res = 0;
        int startIndex1 = startId1 * dataDim;
        int startIndex2 = startId2 * dataDim;
        for (int i = 0; i < dataDim; i++)
        {
            res += vec.at(startIndex1 + i) * vec.at(startIndex2 + i);
        }
        res += mOffset; 
        res = pow(res, mDegree);
        return res;
    }
}
