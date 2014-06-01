#include "FaceFeaturePoints.h"
#include <fstream>
#include <math.h>
#include "../Tool/LogSystem.h"

namespace MagicApp
{
    FaceFeaturePoints::FaceFeaturePoints() :
        mSelectIndex(-1),
        mDim(0),
        mFps(),
        mIsClose(),
        mStartIndex()
    {
    }

    FaceFeaturePoints::FaceFeaturePoints(int dim) :
        mSelectIndex(-1),
        mDim(dim),
        mFps(),
        mIsClose(),
        mStartIndex()
    {
    }

    FaceFeaturePoints::~FaceFeaturePoints()
    {
    }

    bool FaceFeaturePoints::Load(const std::string& fileName)
    {
        std::ifstream fin(fileName);
        if (!fin.is_open())
        {
            return false;
        }
        fin >> mDim;
        int fpsSize;
        fin >> fpsSize;
        mFps.clear();
        mFps.resize(fpsSize * mDim);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            for (int dimId = 0; dimId < mDim; dimId++)
            {
                int fpsValue;
                fin >> fpsValue;
                mFps.at(fpsId * mDim + dimId) = fpsValue;
            }
        }
        int groupSize;
        fin >> groupSize;
        mIsClose.clear();
        mIsClose.resize(groupSize);
        for (int groupId = 0; groupId < groupSize; groupId++)
        {
            int isClose;
            fin >> isClose;
            mIsClose.at(groupId) = isClose;
        }
        mStartIndex.clear();
        mStartIndex.resize(groupSize + 1);
        for (int groupId = 0; groupId < groupSize + 1; groupId++)
        {
            int startIndex;
            fin >> startIndex;
            mStartIndex.at(groupId) = startIndex;
        }
        fin.close();
        return true;
    }

    void FaceFeaturePoints::Set(int dim, const std::vector<double>& fpsList, const FaceFeaturePoints* pRefFfp)
    {
        mDim = dim;
        mFps = fpsList;
        if (pRefFfp != NULL)
        {
            pRefFfp->GetParameter(&mStartIndex, &mIsClose);
        }
    }

    void FaceFeaturePoints::Save(const std::string& fileName) const
    {
        std::ofstream fout(fileName);
        fout << mDim << std::endl;
        fout << mFps.size() / mDim << std::endl;
        for (int fpsId = 0; fpsId < mFps.size(); fpsId++)
        {
            fout << mFps.at(fpsId) << " ";
        }
        fout << mIsClose.size() << std::endl;
        for (int groupId = 0; groupId < mIsClose.size(); groupId++)
        {
            fout << mIsClose.at(groupId) << " ";
        }
        for (int groupId = 0; groupId <= mStartIndex.size(); groupId++)
        {
            fout << mStartIndex.at(groupId) << " ";
        }
        fout.close();
    }

    void FaceFeaturePoints::GetDefaultDps(std::vector<double>* dpsList) const 
    {
        std::vector<int> addSizeList(7);
        addSizeList.at(0) = 3;
        addSizeList.at(1) = 3;
        addSizeList.at(2) = 2;
        addSizeList.at(3) = 2;
        addSizeList.at(4) = 2;
        addSizeList.at(5) = 2;
        addSizeList.at(6) = 10;
        GetDps(addSizeList, dpsList);
    }

    void FaceFeaturePoints::GetDps(const std::vector<int>& addSizeList, std::vector<double>* dpsList) const
    {
        dpsList->clear();
        int groupSize = mIsClose.size();
        for (int groupId = 0; groupId < groupSize; groupId++)
        {
            double addDelta = 1.0 / (addSizeList.at(groupId) + 1);
            int oneGroupSize = mStartIndex.at(groupId + 1) - mStartIndex.at(groupId);
            DebugLog << "groupId: " << groupId << " groupSize: " << oneGroupSize << std::endl;
            for (int localFpsId = 0; localFpsId < oneGroupSize; localFpsId++)
            {
                if (!mIsClose.at(groupId) && localFpsId == oneGroupSize - 1)
                {
                    for (int dimId = 0; dimId < mDim; dimId++)
                    {
                        dpsList->push_back( mFps.at( (mStartIndex.at(groupId) + localFpsId) * mDim + dimId ) );
                    }
                    break;
                }
                for (int addId = 0; addId <= addSizeList.at(groupId); addId++)
                {
                    for (int dimId = 0; dimId < mDim; dimId++)
                    {
                        double curCord = mFps.at( (mStartIndex.at(groupId) + localFpsId) * mDim + dimId );
                        double nextCord = mFps.at( (mStartIndex.at(groupId) + (localFpsId + 1) % oneGroupSize) * mDim + dimId);
                        double interCord = curCord * (1.0 - addId * addDelta) + nextCord * addId * addDelta;
                        dpsList->push_back(interCord);
                    }
                }
            }
        }
    }

    void FaceFeaturePoints::GetFps(std::vector<double>* fpsList) const
    {
        *fpsList = mFps;
    }

    void FaceFeaturePoints::GetParameter(std::vector<int>* startIndex, std::vector<bool>* isClose) const
    {
        *startIndex = mStartIndex;
        *isClose = mIsClose;
    }

    Face2DFeaturePoints::Face2DFeaturePoints() :
        FaceFeaturePoints(2)
    {
    }

    Face2DFeaturePoints::~Face2DFeaturePoints()
    {
    }

    void Face2DFeaturePoints::GetFPsNormal(std::vector<double>* norList) const
    {
        norList->clear();
        norList->resize(mFps.size());
        int groupSize = mIsClose.size();
        for (int groupId = 0; groupId < groupSize; groupId++)
        {
            int oneGroupSize = mStartIndex.at(groupId + 1) - mStartIndex.at(groupId);
            for (int localFpsId = 0; localFpsId < oneGroupSize; localFpsId++)
            {
                int curId = mStartIndex.at(groupId) + localFpsId;
                int preId, nextId;
                if (mIsClose.at(groupId))
                {
                    int preId = mStartIndex.at(groupId) + (localFpsId - 1 + oneGroupSize) % oneGroupSize;
                    int nextId = mStartIndex.at(groupId) + (localFpsId + 1) % oneGroupSize;
                }
                else
                {
                    int preId = curId - 1;
                    int nextId = curId + 1;
                    if (localFpsId == 0)
                    {
                        preId = curId;
                        nextId = curId + 1;
                    }
                    if (localFpsId == oneGroupSize - 1)
                    {
                        preId = curId - 1;
                        nextId = curId;
                    }
                }
                norList->at(curId * mDim) = mFps.at(nextId * mDim + 1) - mFps.at(preId * mDim + 1);
                norList->at(curId * mDim + 1) = mFps.at(preId * mDim) - mFps.at(nextId * mDim);
            }
        }
        //normalize normal
        int fpsSize = mFps.size() / 2;
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            double norX = norList->at(fpsId * mDim + 1);
            double norY = norList->at(fpsId * mDim);
            double normLen = sqrt( norX * norX + norY + norY );
            if (normLen > 1.0e-15)
            {
                norList->at(fpsId * mDim) /= normLen;
                norList->at(fpsId * mDim + 1) /= normLen;
            }
            else
            {
                DebugLog << "fps normal length is too small: fpsId: " << fpsId << " Length: " << normLen << std::endl;
            }
        }
    }

    void Face2DFeaturePoints::GetSalientFeatures(std::vector<double>* salientFps) const
    {
        int groupSize = mIsClose.size();
        salientFps->clear();
        salientFps->resize(groupSize * 2);
        for (int groupId = 0; groupId < groupSize - 1; groupId++)
        {
            double cenPosX = 0;
            double cenPosY = 0;
            for (int globalFpsId = mStartIndex.at(groupId); globalFpsId < mStartIndex.at(groupId + 1) - 1; globalFpsId++)
            {
                cenPosX += mFps.at(globalFpsId * 2 + 1);
                cenPosY += mFps.at(globalFpsId * 2);
            }
            int oneGroupSize = mStartIndex.at(groupId + 1) - mStartIndex.at(groupId);
            salientFps->at(groupId * 2 + 1) = cenPosX / oneGroupSize;
            salientFps->at(groupId * 2) = cenPosY / oneGroupSize;
        }
        int lastGroupSize = mStartIndex.at(groupSize) - mStartIndex.at(groupSize - 1);
        int midFpsId = mStartIndex.at(groupSize - 1) + lastGroupSize / 2 + 1;
        salientFps->at((groupSize - 1) * 2 + 1) = mFps.at(midFpsId * 2 + 1);
        salientFps->at((groupSize - 1) * 2) = mFps.at(midFpsId * 2);
    }

    bool Face2DFeaturePoints::Select(double hPos, double wPos)
    {
        int tol = 5;
        int fpsSize = mFps.size() / mDim;
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            if (fabs(mFps.at(mDim * fpsId) - hPos) < tol &&
                fabs(mFps.at(mDim * fpsId + 1) - wPos) < tol)
            {
                mSelectIndex = fpsId;
                return true;
            }
        }
        return false;
    }

    void Face2DFeaturePoints::MoveTo(double hPos, double wPos)
    {
        mFps.at(mSelectIndex * mDim) = hPos;
        mFps.at(mSelectIndex * mDim + 1) = wPos;
    }

    void Face2DFeaturePoints::MoveDelta(double hDelta, double wDelta)
    {
        mFps.at(mSelectIndex * mDim) += hDelta;
        mFps.at(mSelectIndex * mDim + 1) += wDelta;
    }
}
