#pragma once
#include <vector>
 
namespace MagicMath
{
    class GradientDescent
    {
    public:
        GradientDescent();
        GradientDescent(int maxIterCount);
        ~GradientDescent();

        void Run();

    protected:
        virtual void CalInitValue() = 0;
        virtual void CalGradient() = 0;
        virtual bool IsStop() = 0;
        virtual void CalStepLength() = 0;
        virtual void UpdateResult() = 0;

    protected:
        int mMaxIterCount;
    };

    class NewtonMethod
    {
    public:
        NewtonMethod();
        NewtonMethod(int maxIterCount, int coefDim);
        ~NewtonMethod();

        void Run();

    protected:
        void CalStep();

        virtual void CalInitValue() = 0;
        virtual void CalGradient() = 0;
        virtual bool IsStop() = 0;
        virtual void CalHessian() = 0;
        virtual void UpdateResult() = 0;

    protected:
        int mMaxIterCount;
        //cache values
        std::vector<double> mGradVec;
        std::vector<double> mHessMat;
        std::vector<double> mStepVec;
    };

    
    
}
