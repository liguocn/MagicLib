#include "SupportVectorMachine.h"
#include "KernelFunction.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <set>
#include <math.h>

namespace MagicML
{
    SupportVectorMachine::SupportVectorMachine() :
        mpKernel(NULL),
        mDataDim(0),
        mB(0),
        mAlpha(),
        mSupportVecX(),
        mSupportVecY(),
        mInnerValid(),
        mInnerCache(),
        mOmiga()
    {
    }

    SupportVectorMachine::~SupportVectorMachine()
    {
        if (mpKernel != NULL)
        {
            delete mpKernel;
            mpKernel = NULL;
        }
    }

    void SupportVectorMachine::Reset(void)
    {
        if (mpKernel != NULL)
        {
            delete mpKernel;
            mpKernel = NULL;
        }
        mDataDim = 0;
        mB = 0;
        mAlpha.clear();
        mSupportVecX.clear();
        mSupportVecY.clear();
        //clear cache
        mInnerValid.clear();
        mInnerCache.clear();
        mOmiga.clear();
    }

    void SupportVectorMachine::Learn(const std::vector<double>& dataX, const std::vector<double>& dataY, 
        KernelFunction* kernel, double softCoef)
    {
        Reset();
        mpKernel = kernel;
        SequentialMinimalOptimization(dataX, dataY, softCoef);
    }

    double SupportVectorMachine::CalF(int index) // Could be optimized
    {
        /*double res = 0;
        for (int i = 0; i < mAlpha.size(); i++)
        {
            res += mAlpha.at(i) * mSupportVecY.at(i) * KernelInnerProductX(index, i);
        }*/
        double res = mpKernel->InnerProduct(mSupportVecX, index, mOmiga);
        res -= mB;
        return res;
    }

    bool SupportVectorMachine::IsKTT(int index, double softCoef)
    {
        double F = CalF(index);
        if ((mSupportVecY.at(index) * F < 1 && mAlpha.at(index) < softCoef) ||
            (mSupportVecY.at(index) * F > 1 && mAlpha.at(index) > 0))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    int SupportVectorMachine::ChooseIndexJ(int index_i, const std::set<int>& nonBoundSet)
    {
        int index_j = -1;
        double maxV = -1;
        double E_i = CalF(index_i) - mSupportVecY.at(index_i);
        for (std::set<int>::iterator itr = nonBoundSet.begin(); itr != nonBoundSet.end(); itr++)
        {
            double E_j = CalF(*itr) - mSupportVecY.at(*itr);
            if (fabs(E_i - E_j) > maxV)
            {
                maxV = fabs(E_i - E_j);
                index_j = *itr;
            }
        }
        if (index_j == -1)
        {
            for (int jDataid = 0; jDataid < mSupportVecY.size(); jDataid++)
            {
                if (jDataid == index_i)
                {
                    continue;
                }
                double E_j = CalF(jDataid) - mSupportVecY.at(jDataid);
                if (fabs(E_i - E_j) > maxV)
                {
                    maxV = fabs(E_i - E_j);
                    index_j = jDataid;
                }
            }
        }
        return index_j;
    }

    void SupportVectorMachine::SequentialMinimalOptimization(const std::vector<double>& dataX, const std::vector<double>& dataY, double softCoef)
    {
        //Initialization
        mSupportVecX = dataX;
        mSupportVecY = dataY;
        int dataCount = mSupportVecY.size();
        mDataDim = mSupportVecX.size() / mSupportVecY.size();
        mAlpha.clear();
        mAlpha = std::vector<double>(dataCount, 0);
        mB = 0.0;
        double localEpsilon = 1.0e-15;
        //init cache
        mInnerValid = std::vector<bool>(dataCount * dataCount, 0);
        mInnerCache.clear();
        mInnerCache.resize(dataCount * dataCount);
        mOmiga = std::vector<double>(mDataDim, 0);

        int maxIterCount = 1000;

        std::set<int> kktList;
        for (int iterId = 0; iterId < maxIterCount; iterId++)
        {
            //first pass all variables
            int kttCount = 0;
            std::set<int> nonBoundSet;
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                //choose index_i
                int index_i = -1;
                if (IsKTT(dataId, softCoef))
                {
                    kttCount++;
                    continue;
                }
                else
                {
                    index_i = dataId;
                }
                //choose index_j
                int index_j = ChooseIndexJ(index_i, nonBoundSet);
                if (index_j == -1)
                {
                    continue;
                }
                OptimizeOneStep(index_i, index_j, softCoef);
                //Update nonBoundSet
                if (mAlpha.at(index_i) > localEpsilon && mAlpha.at(index_i) < softCoef - localEpsilon)
                {
                    nonBoundSet.insert(index_i);
                }
                if (mAlpha.at(index_j) > localEpsilon && mAlpha.at(index_j) < softCoef - localEpsilon)
                {
                    nonBoundSet.insert(index_j);
                }
            } //first pass

            //DebugLog << "Ktt proportion: " << double(kttCount) / double(dataCount) << std::endl;
            if (kttCount == dataCount)
            {
                DebugLog << "Ktt condition is satisfied" << std::endl;
                break;
            }

            //second pass nonBoundSet
            int maxNonBoundIterNum = 100;
            for (int nonBoundIterId = 0; nonBoundIterId < maxNonBoundIterNum; nonBoundIterId++)
            {
                if (nonBoundSet.size() == 0)
                {
                    break;
                }
                int nonBoundKttCount = 0;
                for (std::set<int>::iterator itr = nonBoundSet.begin(); itr != nonBoundSet.end(); itr++)
                {
                    //choose index_i
                    int index_i = -1;
                    if (IsKTT(*itr, softCoef))
                    {
                        nonBoundKttCount++;
                        continue;
                    }
                    else
                    {
                        index_i = *itr;
                    }
                    //choose index_j
                    int index_j = ChooseIndexJ(index_i, nonBoundSet);
                    if (index_j == -1)
                    {
                        continue;
                    }
                    OptimizeOneStep(index_i, index_j, softCoef);
                    //Update nonBoundSet
                    if (mAlpha.at(index_i) > localEpsilon && mAlpha.at(index_i) < softCoef - localEpsilon)
                    {
                        nonBoundSet.insert(index_i);
                    }
                    if (mAlpha.at(index_j) > localEpsilon && mAlpha.at(index_j) < softCoef - localEpsilon)
                    {
                        nonBoundSet.insert(index_j);
                    }
                } //nonBound pass
                if (nonBoundKttCount == nonBoundSet.size())
                {
                    break;
                }
            } //second pass
        }
        //DebugLog << "SupportVectorMachine::SequentialMinimalOptimization finished" << std::endl;
        int nonZeroAlpha = 0;
        for (int dataId = 0; dataId < mAlpha.size(); dataId++)
        {
            if (mAlpha.at(dataId) > 0)
            {
                nonZeroAlpha++;
                //DebugLog << "alpha " << dataId << " : " << mAlpha.at(dataId) << std::endl;
            }
        }
        DebugLog << "Alpha Non Zero: " << nonZeroAlpha << "   total: " << mAlpha.size() << std::endl;
        RemoveZeroAlpha();
    }

    void SupportVectorMachine::RemoveZeroAlpha(void)
    {
        int validCount = 0;
        for (int dataId = 0; dataId < mAlpha.size(); dataId++)
        {
            if (mAlpha.at(dataId) > 0)
            {
                validCount++;
            }
        }
        int dataDim = mSupportVecX.size() / mSupportVecY.size();
        std::vector<double> validAlpha(validCount);
        std::vector<double> validSupX(validCount * dataDim);
        std::vector<double> validSupY(validCount);
        int validId = 0;
        for (int dataId = 0; dataId < mAlpha.size(); dataId++)
        {
            if (mAlpha.at(dataId) > 0)
            {
                validAlpha.at(validId) = mAlpha.at(dataId);
                validSupY.at(validId) = mSupportVecY.at(dataId);
                int validBaseIndex = validId * dataDim;
                int originBaseIndex = dataId * dataDim;
                for (int dimId = 0; dimId < dataDim; dimId++)
                {
                    validSupX.at(validBaseIndex + dimId) = mSupportVecX.at(originBaseIndex + dimId);
                }
                validId++;
            }
        }
        mAlpha = validAlpha;
        mSupportVecX = validSupX;
        mSupportVecY = validSupY;
    }

    double SupportVectorMachine::KernelInnerProductX(int index0, int index1)
    {
        //return mpKernel->InnerProduct(mSupportVecX, index0, index1, mDataDim);
        int innerIndex = index0 * mSupportVecY.size() + index1;
        if (mInnerValid.at(innerIndex))
        {
            return mInnerCache.at(innerIndex);
        }
        else
        {
            int symIndex = index1 * mSupportVecY.size() + index0;
            mInnerValid.at(innerIndex) = 1;
            mInnerValid.at(symIndex) = 1;
            double res = mpKernel->InnerProduct(mSupportVecX, index0, index1, mDataDim);
            mInnerCache.at(innerIndex) = res;
            mInnerCache.at(symIndex) = res;
            return res;
        }
    }

    int SupportVectorMachine::OptimizeOneStep(int index_i, int index_j, double softCoef)
    {
        int dataDim = mSupportVecX.size() / mSupportVecY.size();
        //Update alpha i and j
        double lastAlpha_i = mAlpha.at(index_i);
        double lastAlpha_j = mAlpha.at(index_j);
        double L, H;
        if (mSupportVecY.at(index_i) * mSupportVecY.at(index_j) < 0)
        {
            double deltaTemp = mAlpha.at(index_j) - mAlpha.at(index_i);
            L = deltaTemp > 0 ? deltaTemp : 0;
            H = deltaTemp > 0 ? softCoef : softCoef + deltaTemp;
        }
        else
        {
            double deltaTemp = mAlpha.at(index_j) + mAlpha.at(index_i);
            L = deltaTemp > softCoef ? deltaTemp - softCoef : 0;
            H = deltaTemp > softCoef ? softCoef : deltaTemp;
        }
        if (L > H)
        {
            DebugLog << "Error: L > H" << std::endl;
            return 1;
        }

        double eta = KernelInnerProductX(index_i, index_i) + KernelInnerProductX(index_j, index_j) 
            - 2 * KernelInnerProductX(index_i, index_j);
        if (fabs(eta) < 1.0e-15)
        {
            eta += 1.0e-15;
        }
        double E_i = CalF(index_i) - mSupportVecY.at(index_i);
        double E_j = CalF(index_j) - mSupportVecY.at(index_j);
        mAlpha.at(index_j) = lastAlpha_j + mSupportVecY.at(index_j) * (E_i - E_j) / eta;
        if (mAlpha.at(index_j) < L)
        {
            mAlpha.at(index_j) = L;
        }
        else if (mAlpha.at(index_j) > H)
        {
            mAlpha.at(index_j) = H;
        }
        mAlpha.at(index_i) = lastAlpha_i + mSupportVecY.at(index_i) * mSupportVecY.at(index_j) * (lastAlpha_j - mAlpha.at(index_j));

        UpdateOmiga(index_i, lastAlpha_i, index_j, lastAlpha_j);

        //Update b
        if (mAlpha.at(index_i) > 0 && mAlpha.at(index_i) < softCoef)
        {
            mB = mB + E_i + mSupportVecY.at(index_i) * (mAlpha.at(index_i) - lastAlpha_i) * KernelInnerProductX(index_i, index_i) 
                + mSupportVecY.at(index_j) * (mAlpha.at(index_j) - lastAlpha_j) * KernelInnerProductX(index_i, index_j);
        }
        else if (mAlpha.at(index_j) > 0 && mAlpha.at(index_j) < softCoef)
        {
            mB = mB + E_j + mSupportVecY.at(index_i) * (mAlpha.at(index_i) - lastAlpha_i) * KernelInnerProductX(index_i, index_j) 
                + mSupportVecY.at(index_j) * (mAlpha.at(index_j) - lastAlpha_j) * KernelInnerProductX(index_j, index_j);
        }
        else
        {
            double b1 = mB + E_i + mSupportVecY.at(index_i) * (mAlpha.at(index_i) - lastAlpha_i) * KernelInnerProductX(index_i, index_i) 
                + mSupportVecY.at(index_j) * (mAlpha.at(index_j) - lastAlpha_j) * KernelInnerProductX(index_i, index_j);
            double b2 = mB + E_j + mSupportVecY.at(index_i) * (mAlpha.at(index_i) - lastAlpha_i) * KernelInnerProductX(index_i, index_j) 
                + mSupportVecY.at(index_j) * (mAlpha.at(index_j) - lastAlpha_j) * KernelInnerProductX(index_j, index_j);
            mB = (b1 + b2) / 2;
        }
        return 0;
    }

    void SupportVectorMachine::UpdateOmiga(int index_i, double lastV_i, int index_j, double lastV_j)
    {
        int baseIndex_i = index_i * mDataDim;
        int baseIndex_j = index_j * mDataDim;
        double delta_i = mSupportVecY.at(index_i) * (mAlpha.at(index_i) - lastV_i);
        double delta_j = mSupportVecY.at(index_j) * (mAlpha.at(index_j) - lastV_j);
        for (int dimId = 0; dimId < mDataDim; dimId++)
        {
            mOmiga.at(dimId) += (delta_i * mSupportVecX.at(baseIndex_i + dimId) + delta_j * mSupportVecX.at(baseIndex_j + dimId));
        }
    }
        
    double SupportVectorMachine::Predict(const std::vector<double>& dataX) const
    {
        int supCount = mAlpha.size();
        double res = 0;
        for (int supId = 0; supId < supCount; supId++)
        {
            res += mAlpha.at(supId) * mSupportVecY.at(supId) * mpKernel->InnerProduct(mSupportVecX, supId, dataX);
        }
        res -= mB;

        return res;
    }
}
