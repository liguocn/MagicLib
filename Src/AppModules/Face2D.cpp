#include "Face2D.h"
#include <fstream>
#include "../Math/HomoMatrix3.h"
#include "../DIP/Deformation.h"
#include "../Tool/LogSystem.h"
#include "../DIP/FeatureDetection.h"

namespace MagicApp
{
    FaceFeaturePoint::FaceFeaturePoint() :
        mSelectIndex(-1),
        mSelectType(FT_None)
    {
    }

    FaceFeaturePoint::~FaceFeaturePoint()
    {
    }

    /*void FaceFeaturePoint::Load()
    {
        std::string fileName;
        char filterName[] = "Feature Point Files(*.fp)\0*.fp\0";
        if (MagicCore::ToolKit::FileOpenDlg(fileName, filterName))
        {
            Load(fileName);
        }
    }*/

    bool FaceFeaturePoint::Load(const std::string& fileName)
    {
        std::ifstream fin(fileName);
        if (!fin.is_open())
        {
            return false;
        }
        int leftBrowNum;
        fin >> leftBrowNum;
        mLeftBrowFPs.clear();
        mLeftBrowFPs.resize(leftBrowNum * 2);
        for (int fid = 0; fid < leftBrowNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mLeftBrowFPs.at(fid * 2) = hid;
            mLeftBrowFPs.at(fid * 2 + 1) = wid;
        }
        int rightBrowNum;
        fin >> rightBrowNum;
        mRightBrowFPs.clear();
        mRightBrowFPs.resize(rightBrowNum * 2);
        for (int fid = 0; fid < rightBrowNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mRightBrowFPs.at(fid * 2) = hid;
            mRightBrowFPs.at(fid * 2 + 1) = wid;
        }
        int leftEyeNum;
        fin >> leftEyeNum;
        mLeftEyeFPs.clear();
        mLeftEyeFPs.resize(leftEyeNum * 2);
        for (int fid = 0; fid < leftEyeNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mLeftEyeFPs.at(fid * 2) = hid;
            mLeftEyeFPs.at(fid * 2 + 1) = wid;
        }
        int rightEyeNum;
        fin >> rightEyeNum;
        mRightEyeFPs.clear();
        mRightEyeFPs.resize(rightEyeNum * 2);
        for (int fid = 0; fid < rightEyeNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mRightEyeFPs.at(fid * 2) = hid;
            mRightEyeFPs.at(fid * 2 + 1) = wid;
        }
        int noseNum;
        fin >> noseNum;
        mNoseFPs.clear();
        mNoseFPs.resize(noseNum * 2);
        for (int fid = 0; fid < noseNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mNoseFPs.at(fid * 2) = hid;
            mNoseFPs.at(fid * 2 + 1) = wid;
        }
        int mouseNum;
        fin >> mouseNum;
        mMouseFPs.clear();
        mMouseFPs.resize(mouseNum * 2);
        for (int fid = 0; fid < mouseNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mMouseFPs.at(fid * 2) = hid;
            mMouseFPs.at(fid * 2 + 1) = wid;
        }
        int borderNum;
        fin >> borderNum;
        mBorderFPs.clear();
        mBorderFPs.resize(borderNum * 2);
        for (int fid = 0; fid < borderNum; fid++)
        {
            int hid, wid;
            fin >> hid >> wid;
            mBorderFPs.at(fid * 2) = hid;
            mBorderFPs.at(fid * 2 + 1) = wid;
        }
        fin.close();
        UpdateDPs();

        return true;
    }

    void FaceFeaturePoint::Load(int browNum, int eyeNum, int noseNum, int mouseNum, int borderNum, const std::vector<int>& posList)
    {
        int baseIndex = 0;
        mLeftBrowFPs.clear();
        mLeftBrowFPs.resize(browNum * 2);
        for (int pointId = 0; pointId < browNum; pointId++)
        {
            mLeftBrowFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mLeftBrowFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (browNum * 2);

        mRightBrowFPs.clear();
        mRightBrowFPs.resize(browNum * 2);
        for (int pointId = 0; pointId < browNum; pointId++)
        {
            mRightBrowFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mRightBrowFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (browNum * 2);

        mLeftEyeFPs.clear();
        mLeftEyeFPs.resize(eyeNum * 2);
        for (int pointId = 0; pointId < eyeNum; pointId++)
        {
            mLeftEyeFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mLeftEyeFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (eyeNum * 2);

        mRightEyeFPs.clear();
        mRightEyeFPs.resize(eyeNum * 2);
        for (int pointId = 0; pointId < eyeNum; pointId++)
        {
            mRightEyeFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mRightEyeFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (eyeNum * 2);

        mNoseFPs.clear();
        mNoseFPs.resize(noseNum * 2);
        for (int pointId = 0; pointId < noseNum; pointId++)
        {
            mNoseFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mNoseFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (noseNum * 2);

        mMouseFPs.clear();
        mMouseFPs.resize(mouseNum * 2);
        for (int pointId = 0; pointId < mouseNum; pointId++)
        {
            mMouseFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mMouseFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (mouseNum * 2);

        mBorderFPs.clear();
        mBorderFPs.resize(borderNum * 2);
        for (int pointId = 0; pointId < borderNum; pointId++)
        {
            mBorderFPs.at(pointId * 2) = posList.at(baseIndex + pointId * 2);
            mBorderFPs.at(pointId * 2 + 1) = posList.at(baseIndex + pointId * 2 + 1);
        }
        baseIndex += (borderNum * 2);

        UpdateDPs();
    }

    void FaceFeaturePoint::Set(const std::vector<int>& posList, const FaceFeaturePoint* refFps)
    {
        int browNum, eyeNum, noseNum, mouseNum, borderNum;
        if (refFps == NULL)
        {
            GetParameter(browNum, eyeNum, noseNum, mouseNum, borderNum);
        }
        else
        {
            refFps->GetParameter(browNum, eyeNum, noseNum, mouseNum, borderNum);
        }
        Load(browNum, eyeNum, noseNum, mouseNum, borderNum, posList);
    }

    void FaceFeaturePoint::UpdateDPs()
    {
        ConstructOneDPs(mLeftBrowFPs, true, 3, mLeftBrowDPs);
        ConstructOneDPs(mRightBrowFPs, true, 3, mRightBrowDPs);
        ConstructOneDPs(mLeftEyeFPs, true, 2, mLeftEyeDPs);
        ConstructOneDPs(mRightEyeFPs, true, 2, mRightEyeDPs);
        ConstructOneDPs(mNoseFPs, false, 2, mNoseDPs);
        ConstructOneDPs(mMouseFPs, true, 2, mMouseDPs);
        ConstructOneDPs(mBorderFPs, false, 10, mBorderDPs);
    }

    void FaceFeaturePoint::ConstructOneDPs(const std::vector<int>& fps, bool isClosed, int addSize, std::vector<int>& dps)
    {
        double addDelta = 1.0 / (addSize + 1);
        int fpsSize = fps.size() / 2;
        dps.clear();
        for (int fid = 0; fid < fpsSize; fid++)
        {
            int hid = fps.at(fid * 2);
            int wid = fps.at(fid * 2 + 1);
            if (!isClosed && fid == fpsSize - 1)
            {
                dps.push_back(hid);
                dps.push_back(wid);
                break;
            }
            int hidNext = fps.at((fid + 1) % fpsSize * 2);
            int widNext = fps.at((fid + 1) % fpsSize * 2 + 1);
            for (int addId = 0; addId <= addSize; addId++)
            {
                int addHid = int(hid * (1.0 - addId * addDelta) + hidNext * addId * addDelta);
                int addWid = int(wid * (1.0 - addId * addDelta) + widNext * addId * addDelta);
                dps.push_back(addHid);
                dps.push_back(addWid);
            }
        }
    }

    void FaceFeaturePoint::Save(const std::string& fileName)
    {
        std::ofstream fout(fileName);
        fout << mLeftBrowFPs.size() / 2 << " ";
        for (int fid = 0; fid < mLeftBrowFPs.size(); fid++)
        {
            fout << mLeftBrowFPs.at(fid) << " ";
        }
        fout << mRightBrowFPs.size() / 2 << " ";
        for (int fid = 0; fid < mRightBrowFPs.size(); fid++)
        {
            fout << mRightBrowFPs.at(fid) << " ";
        }
        fout << mLeftEyeFPs.size() / 2 << " ";
        for (int fid = 0; fid < mLeftEyeFPs.size(); fid++)
        {
            fout << mLeftEyeFPs.at(fid) << " ";
        }
        fout << mRightEyeFPs.size() / 2 << " ";
        for (int fid = 0; fid < mRightEyeFPs.size(); fid++)
        {
            fout << mRightEyeFPs.at(fid) << " ";
        }
        fout << mNoseFPs.size() / 2 << " ";
        for (int fid = 0; fid < mNoseFPs.size(); fid++)
        {
            fout << mNoseFPs.at(fid) << " ";
        }
        fout << mMouseFPs.size() / 2 << " ";
        for (int fid = 0; fid < mMouseFPs.size(); fid++)
        {
            fout << mMouseFPs.at(fid) << " ";
        }
        fout << mBorderFPs.size() / 2 << " ";
        for (int fid = 0; fid < mBorderFPs.size(); fid++)
        {
            fout << mBorderFPs.at(fid) << " ";
        }
        fout.close();
    }

    bool FaceFeaturePoint::Select(int hid, int wid)
    {
        int tol = 5;
        for (int fid = 0; fid < mLeftBrowFPs.size() / 2; fid++)
        {
            if (abs(hid - mLeftBrowFPs.at(2 * fid)) < tol &&
                abs(wid - mLeftBrowFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Left_Brow;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mRightBrowFPs.size() / 2; fid++)
        {
            if (abs(hid - mRightBrowFPs.at(2 * fid)) < tol &&
                abs(wid - mRightBrowFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Right_Brow;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mLeftEyeFPs.size() / 2; fid++)
        {
            if (abs(hid - mLeftEyeFPs.at(2 * fid)) < tol &&
                abs(wid - mLeftEyeFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Left_Eye;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mRightEyeFPs.size() / 2; fid++)
        {
            if (abs(hid - mRightEyeFPs.at(2 * fid)) < tol &&
                abs(wid - mRightEyeFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Right_Eye;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mNoseFPs.size() / 2; fid++)
        {
            if (abs(hid - mNoseFPs.at(2 * fid)) < tol &&
                abs(wid - mNoseFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Nose;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mMouseFPs.size() / 2; fid++)
        {
            if (abs(hid - mMouseFPs.at(2 * fid)) < tol &&
                abs(wid - mMouseFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Mouse;
                mSelectIndex = fid;
                return true;
            }
        }
        for (int fid = 0; fid < mBorderFPs.size() / 2; fid++)
        {
            if (abs(hid - mBorderFPs.at(2 * fid)) < tol &&
                abs(wid - mBorderFPs.at(2 * fid + 1)) < tol)
            {
                mSelectType = FT_Border;
                mSelectIndex = fid;
                return true;
            }
        }

        mSelectType = FT_None;
        mSelectIndex = -1;
        return false;
    }

    void FaceFeaturePoint::MoveTo(int hid, int wid)
    {
        if (mSelectType == FT_Left_Brow)
        {
            mLeftBrowFPs.at(mSelectIndex * 2) = hid;
            mLeftBrowFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Right_Brow)
        {
            mRightBrowFPs.at(mSelectIndex * 2) = hid;
            mRightBrowFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Left_Eye)
        {
            mLeftEyeFPs.at(mSelectIndex * 2) = hid;
            mLeftEyeFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Right_Eye)
        {
            mRightEyeFPs.at(mSelectIndex * 2) = hid;
            mRightEyeFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Nose)
        {
            mNoseFPs.at(mSelectIndex * 2) = hid;
            mNoseFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Mouse)
        {
            mMouseFPs.at(mSelectIndex * 2) = hid;
            mMouseFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        else if (mSelectType == FT_Border)
        {
            mBorderFPs.at(mSelectIndex * 2) = hid;
            mBorderFPs.at(mSelectIndex * 2 + 1) = wid;
        }
        UpdateDPs();
    }

    void FaceFeaturePoint::MoveDelta(int deltaH, int deltaW)
    {
        if (mSelectType == FT_Left_Brow)
        {
            mLeftBrowFPs.at(mSelectIndex * 2) += deltaH;
            mLeftBrowFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Right_Brow)
        {
            mRightBrowFPs.at(mSelectIndex * 2) += deltaH;
            mRightBrowFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Left_Eye)
        {
            mLeftEyeFPs.at(mSelectIndex * 2) += deltaH;
            mLeftEyeFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Right_Eye)
        {
            mRightEyeFPs.at(mSelectIndex * 2) += deltaH;
            mRightEyeFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Nose)
        {
            mNoseFPs.at(mSelectIndex * 2) += deltaH;
            mNoseFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Mouse)
        {
            mMouseFPs.at(mSelectIndex * 2) += deltaH;
            mMouseFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        else if (mSelectType == FT_Border)
        {
            mBorderFPs.at(mSelectIndex * 2) += deltaH;
            mBorderFPs.at(mSelectIndex * 2 + 1) += deltaW;
        }
        UpdateDPs();
    }

    void FaceFeaturePoint::GetDPs(std::vector<int>& posList) const
    {
        posList.clear();
        for (std::vector<int>::const_iterator itr = mLeftBrowDPs.begin(); itr != mLeftBrowDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mRightBrowDPs.begin(); itr != mRightBrowDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mLeftEyeDPs.begin(); itr != mLeftEyeDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mRightEyeDPs.begin(); itr != mRightEyeDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mNoseDPs.begin(); itr != mNoseDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mMouseDPs.begin(); itr != mMouseDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mBorderDPs.begin(); itr != mBorderDPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
    }

    void FaceFeaturePoint::GetFPs(std::vector<int>& posList) const
    {
        posList.clear();
        for (std::vector<int>::const_iterator itr = mLeftBrowFPs.begin(); itr != mLeftBrowFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mRightBrowFPs.begin(); itr != mRightBrowFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mLeftEyeFPs.begin(); itr != mLeftEyeFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mRightEyeFPs.begin(); itr != mRightEyeFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mNoseFPs.begin(); itr != mNoseFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mMouseFPs.begin(); itr != mMouseFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
        for (std::vector<int>::const_iterator itr = mBorderFPs.begin(); itr != mBorderFPs.end(); itr++)
        {
            posList.push_back(*itr);
        }
    }

    void FaceFeaturePoint::GetFPsNormal(std::vector<int>& norList) const
    {
        int browNum = mLeftBrowFPs.size() / 2;
        int eyeNum = mLeftEyeFPs.size() / 2;
        int noseNum = mNoseFPs.size() / 2;
        int mouseNum = mMouseFPs.size() / 2;
        int borderNum = mBorderFPs.size() / 2;
        norList.clear();
        norList.resize(2 * (browNum * 2 + eyeNum * 2 + noseNum + mouseNum + borderNum));
        int rightBrowBaseId = browNum * 2;
        for (int fid = 0; fid < browNum; fid++)
        {
            int preId = (fid - 1 + browNum) % browNum;
            int nextId = (fid + 1) % browNum;
            norList.at(fid * 2) = mLeftBrowFPs.at(nextId * 2 + 1) - mLeftBrowFPs.at(preId * 2 + 1);
            norList.at(fid * 2 + 1) = mLeftBrowFPs.at(preId * 2) - mLeftBrowFPs.at(nextId * 2);

            norList.at(rightBrowBaseId + fid * 2) = mRightBrowFPs.at(nextId * 2 + 1) - mRightBrowFPs.at(preId * 2 + 1);
            norList.at(rightBrowBaseId + fid * 2 + 1) = mRightBrowFPs.at(preId * 2) - mRightBrowFPs.at(nextId * 2);
        }
        int leftEysBaseId = browNum * 4;
        int rightEyeBaseId = leftEysBaseId + eyeNum * 2;
        for (int fid = 0; fid < eyeNum; fid++)
        {
            int preId = (fid - 1 + eyeNum) % eyeNum;
            int nextId = (fid + 1 + eyeNum) % eyeNum;
            norList.at(leftEysBaseId + fid * 2) = mLeftEyeFPs.at(nextId * 2 + 1) - mLeftEyeFPs.at(preId * 2 + 1);
            norList.at(leftEysBaseId + fid * 2 + 1) = mLeftEyeFPs.at(preId * 2) - mLeftEyeFPs.at(nextId * 2);

            norList.at(rightEyeBaseId + fid * 2) = mRightEyeFPs.at(nextId * 2 + 1) - mRightEyeFPs.at(preId * 2 + 1);
            norList.at(rightEyeBaseId + fid * 2 + 1) = mRightEyeFPs.at(preId * 2) - mRightEyeFPs.at(nextId * 2);
        }
        int noseBaseId = browNum * 4 + eyeNum * 4;
        for (int fid = 0; fid < noseNum; fid++)
        {
            int preId = fid - 1;
            int nextId = fid + 1;
            if (fid == 0)
            {
                preId = 0;
                nextId = 1;
            }
            if (fid == noseNum - 1)
            {
                preId = noseNum - 2;
                nextId = noseNum - 1;
            }
            norList.at(noseBaseId + fid * 2) = mNoseFPs.at(nextId * 2 + 1) - mNoseFPs.at(preId * 2 + 1);
            norList.at(noseBaseId + fid * 2 + 1) = mNoseFPs.at(preId * 2) - mNoseFPs.at(nextId * 2);
        }
        int mouseBaseId = browNum * 4 + eyeNum * 4 + noseNum * 2;
        for (int fid = 0; fid < mouseNum; fid++)
        {
            int preId = (fid - 1 + mouseNum) % mouseNum;
            int nextId = (fid + 1 + mouseNum) % mouseNum;
            norList.at(mouseBaseId + fid * 2) = mMouseFPs.at(nextId * 2 + 1) - mMouseFPs.at(preId * 2 + 1);
            norList.at(mouseBaseId + fid * 2 + 1) = mMouseFPs.at(preId * 2) - mMouseFPs.at(nextId * 2);
        }
        int borderBaseId = browNum * 4 + eyeNum * 4 + noseNum * 2 + mouseNum * 2;
        for (int fid = 0; fid < borderNum; fid++)
        {
            int preId = fid - 1;
            int nextId = fid + 1;
            if (fid == 0)
            {
                preId = 0;
                nextId = 1;
            }
            if (fid == borderNum - 1)
            {
                preId = borderNum - 2;
                nextId = borderNum - 1;
            }
            norList.at(borderBaseId + fid * 2) = mBorderFPs.at(nextId * 2 + 1) - mBorderFPs.at(preId * 2 + 1);
            norList.at(borderBaseId + fid * 2 + 1) = mBorderFPs.at(preId * 2) - mBorderFPs.at(nextId * 2);
        }
    }

    void FaceFeaturePoint::GetParameter(int& browNum, int& eyeNum, int& noseNum, int& mouseNum, int& borderNum) const
    {
        browNum = mLeftBrowFPs.size() / 2;
        eyeNum = mLeftEyeFPs.size() / 2;
        noseNum = mNoseFPs.size() / 2;
        mouseNum = mMouseFPs.size() / 2;
        borderNum = mBorderFPs.size() / 2;
    }

    void FaceFeaturePoint::GetEyeCenter(double& leftX, double& leftY, double& rightX, double& rightY) const
    {
        leftX = 0;
        leftY = 0;
        rightX = 0;
        rightY = 0;
        int eyeSize = mLeftEyeFPs.size() / 2;
        for (int eyeId = 0; eyeId < eyeSize; eyeId++)
        {
            leftX += mLeftEyeFPs.at(eyeId * 2 + 1);
            leftY += mLeftEyeFPs.at(eyeId * 2);
            rightX += mRightEyeFPs.at(eyeId * 2 + 1);
            rightY += mRightEyeFPs.at(eyeId * 2);
        }
        leftX /= eyeSize;
        leftY /= eyeSize;
        rightX /= eyeSize;
        rightY /= eyeSize;
    }

    void FaceFeaturePoint::GetMouseCenter(double& x, double& y) const
    {
        x = 0;
        y = 0;
        int mouseSize = mMouseFPs.size() / 2;
        for (int fpsId = 0; fpsId < mouseSize; fpsId++)
        {
            x += mMouseFPs.at(fpsId * 2 + 1);
            y += mMouseFPs.at(fpsId * 2);
        }
        x /= mouseSize;
        y /= mouseSize;
    }

    Face2D::Face2D() : 
        mpImage(NULL),
        mpFps(NULL),
        mpRefImage(NULL),
        mpRefFps(NULL),
        mpFeaturePca(NULL)
    {
    }

    Face2D::~Face2D()
    {
        if (mpImage != NULL)
        {
            delete mpImage;
        }
        if (mpFps != NULL)
        {
            delete mpFps;
        }
        if (mpRefImage != NULL)
        {
            delete mpRefImage;
        }
        if (mpRefFps != NULL)
        {
            delete mpRefFps;
        }
        if (mpFeaturePca != NULL)
        {
            delete mpFeaturePca;
        }
    }

    bool Face2D::LoadImage(const std::string& fileName)
    {
        if (mpImage == NULL)
        {
            mpImage = new cv::Mat;
        }
        mpImage->release();
        *mpImage = cv::imread(fileName);
        if (mpImage->data != NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    cv::Mat Face2D::GetImage(void)
    {
        return *mpImage;
    }

    void Face2D::GetImageSize(int* imgW, int* imgH)
    {
        *imgW = mpImage->cols;
        *imgH = mpImage->rows;
    }

    bool Face2D::LoadFps(const std::string& fileName)
    {
        if (mpFps == NULL)
        {
            mpFps = new FaceFeaturePoint;
        }
        return mpFps->Load(fileName);
    }

    void Face2D::AutoAlignFps(const std::vector<int>& markPosList)
    {
        //Calculate initial fps according to markPosList
        double eyeLeftX, eyeLeftY, eyeRightX, eyeRightY;
        mpFps->GetEyeCenter(eyeLeftX, eyeLeftY, eyeRightX, eyeRightY);
        double mouseX, mouseY;
        mpFps->GetMouseCenter(mouseX, mouseY);
        std::vector<cv::Point2f> cvTargetFps(3);
        cvTargetFps.at(0) = cv::Point2f(eyeLeftX, eyeLeftY);
        cvTargetFps.at(1) = cv::Point2f(eyeRightX, eyeRightY);
        cvTargetFps.at(2) = cv::Point2f(mouseX, mouseY);
        std::vector<cv::Point2f> cvMarkFps(3);
        cvMarkFps.at(0) = cv::Point2f(markPosList.at(1), markPosList.at(0));
        cvMarkFps.at(1) = cv::Point2f(markPosList.at(3), markPosList.at(2));
        cvMarkFps.at(2) = cv::Point2f(markPosList.at(5), markPosList.at(4));
        std::vector<int> fpsList;
        mpFps->GetFPs(fpsList);
        RigidFittingFps(cvTargetFps, cvMarkFps, fpsList);

        //Align to feature: feature image, fps pca.
        /*cv::Mat featureImg = MagicDIP::FeatureDetection::CannyEdgeDetection(*mpImage);
        int imgW = mpImage->cols;
        int imgH = mpImage->rows;

        int markGrayThre = 10;
        int neigSize = 5;
        int fpsSize = fpsList.size() / 2;
        std::vector<cv::Point2f> cvFaceFps;
        std::vector<cv::Point2f> cvFaceRefFps;
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            int posX = fpsList.at(fpsId * 2 + 1);
            int posY = fpsList.at(fpsId * 2);
            int maxGray = -1;
            int maxX, maxY;
            for (int neigId = 0; neigId < neigSize; neigId++)
            {
                int hStart = posY - neigId;
                hStart = hStart < 0 ? 0 : hStart;
                int hEnd = posY + neigId;
                hEnd = hEnd >= imgH ? imgH - 1 : hEnd;
                int wLeft = posX - neigId;
                if (wLeft >= 0)
                {
                    for (int hid = hStart; hid <= hEnd; hid++)
                    {
                        if (featureImg.ptr(hid, wLeft)[0] > maxGray)
                        {
                            maxGray = featureImg.ptr(hid, wLeft)[0];
                            maxX = wLeft;
                            maxY = hid;
                        }
                    }
                }
                int wRight = posX + neigId;
                if (wRight < imgW)
                {
                    for (int hid = hStart; hid <= hEnd; hid++)
                    {
                        if (featureImg.ptr(hid, wRight)[0] > maxGray)
                        {
                            maxGray = featureImg.ptr(hid, wRight)[0];
                            maxX = wRight;
                            maxY = hid;
                        }
                    }
                }
                int wStart = posX - neigId + 1;
                wStart = wStart < 0 ? 0 : wStart;
                int wEnd = posX + neigId - 1;
                wEnd = wEnd >= imgW ? imgW - 1 : wEnd;
                int hTop = posY - neigId;
                if (hTop >= 0)
                {
                    for (int wid = wStart; wid <= wEnd; wid++)
                    {
                        if (featureImg.ptr(hTop, wid)[0] > maxGray)
                        {
                            maxGray = featureImg.ptr(hTop, wid)[0];
                            maxX = wid;
                            maxY = hTop;
                        }
                    }
                }
                int hDown = posY + neigId;
                if (hDown < imgH)
                {
                    for (int wid = wStart; wid <= wEnd; wid++)
                    {
                        if (featureImg.ptr(hDown, wid)[0] > maxGray)
                        {
                            maxGray = featureImg.ptr(hDown, wid)[0];
                            maxX = wid;
                            maxY = hDown;
                        }
                    }
                }
            }
            if (maxGray >= markGrayThre)
            {
                cvFaceFps.push_back(cv::Point2f(posX, posY));
                cvFaceRefFps.push_back(cv::Point2f(maxX, maxY));
            }
        }
        if (cvFaceFps.size() > 2)
        {
            RigidFittingFps(cvFaceFps, cvFaceRefFps, fpsList);
            DebugLog << "Rigid fitting: " << cvFaceFps.size() << " " << fpsSize << std::endl;
        }*/

        //Update fps
        mpFps->Set(fpsList, NULL);
    }

    void Face2D::AutoAlignFps()
    {
        //Align to feature: feature image, fps pca.
        cv::Mat featureImg = MagicDIP::FeatureDetection::CannyEdgeDetection(*mpImage);
        int imgW = mpImage->cols;
        int imgH = mpImage->rows;

        std::vector<int> fpsList;
        mpFps->GetFPs(fpsList);
        int browNum, eyeNum, noseNum, mouseNum, borderNum;
        mpFps->GetParameter(browNum, eyeNum, noseNum, mouseNum, borderNum);
        int markGrayThre = 100;
        int neigSize = 5;
        int fpsSize = fpsList.size() / 2;
        std::vector<cv::Point2f> cvFaceFps;
        std::vector<cv::Point2f> cvFaceRefFps;
        std::vector<int> fpsNorList;
        mpFps->GetFPsNormal(fpsNorList);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            int posX = fpsList.at(fpsId * 2 + 1);
            int posY = fpsList.at(fpsId * 2);
            int maxGray = -1;
            int maxX, maxY;
            double norX = fpsNorList.at(fpsId * 2 + 1);
            double norY = fpsNorList.at(fpsId * 2);
            double norLen = sqrt(norX * norX + norY * norY);
            if (norLen < 1.0e-15)
            {
                DebugLog << "norLen is too small" << std::endl;
            }
            else
            {
                norX /= norLen;
                norY /= norLen;
            }
            for (int neigId = 0; neigId < neigSize; neigId++)
            {
                int curPosiPosX = floor(posX + norX * neigId + 0.5);
                int curPosiPosY = floor(posY + norY * neigId + 0.5);
                if (curPosiPosX >= 0 && curPosiPosX < imgW && curPosiPosY >= 0 && curPosiPosY < imgH)
                {
                    if (featureImg.ptr(curPosiPosY, curPosiPosX)[0] > maxGray)
                    {
                        maxGray = featureImg.ptr(curPosiPosY, curPosiPosX)[0];
                        maxX = curPosiPosX;
                        maxY = curPosiPosY;
                    }
                }
                int curNegPosX = floor(posX - norX * neigId + 0.5);
                int curNegPosY = floor(posY - norY * neigId + 0.5);
                if (curNegPosX >= 0 && curNegPosX < imgW && curNegPosY >= 0 && curNegPosY < imgH)
                {
                    if (featureImg.ptr(curNegPosY, curNegPosX)[0] > maxGray)
                    {
                        maxGray = featureImg.ptr(curNegPosY, curNegPosX)[0];
                        maxX = curNegPosX;
                        maxY = curNegPosY;
                    }
                }
                if (maxGray >= markGrayThre)
                {
                    break;
                }
            }
            if (maxGray >= markGrayThre)
            {
                cvFaceFps.push_back(cv::Point2f(posX, posY));
                cvFaceRefFps.push_back(cv::Point2f(maxX, maxY));
            }
        }
        if (cvFaceFps.size() > 2)
        {
            DebugLog << "Rigid fitting: " << cvFaceFps.size() << " " << fpsSize << std::endl;
            RigidFittingFps(cvFaceFps, cvFaceRefFps, fpsList);
        }
        else
        {
            DebugLog << "cvFaceFps.size = " << cvFaceFps.size() << std::endl;
        }

        //Update fps
        mpFps->Set(fpsList, NULL);
    }

    //void Face2D::AutoAlignFps()
    //{
    //    //Align to feature: feature image, fps pca.
    //    cv::Mat featureImg = MagicDIP::FeatureDetection::CannyEdgeDetection(*mpImage);
    //    int imgW = mpImage->cols;
    //    int imgH = mpImage->rows;

    //    std::vector<int> fpsList;
    //    mpFps->GetFPs(fpsList);
    //    int markGrayThre = 10;
    //    int neigSize = 5;
    //    int fpsSize = fpsList.size() / 2;
    //    std::vector<cv::Point2f> cvFaceFps;
    //    std::vector<cv::Point2f> cvFaceRefFps;
    //    for (int fpsId = 0; fpsId < fpsSize; fpsId++)
    //    {
    //        int posX = fpsList.at(fpsId * 2 + 1);
    //        int posY = fpsList.at(fpsId * 2);
    //        int maxGray = -1;
    //        int maxX, maxY;
    //        for (int neigId = 0; neigId < neigSize; neigId++)
    //        {
    //            int hStart = posY - neigId;
    //            hStart = hStart < 0 ? 0 : hStart;
    //            int hEnd = posY + neigId;
    //            hEnd = hEnd >= imgH ? imgH - 1 : hEnd;
    //            int wLeft = posX - neigId;
    //            if (wLeft >= 0)
    //            {
    //                for (int hid = hStart; hid <= hEnd; hid++)
    //                {
    //                    if (featureImg.ptr(hid, wLeft)[0] > maxGray)
    //                    {
    //                        maxGray = featureImg.ptr(hid, wLeft)[0];
    //                        maxX = wLeft;
    //                        maxY = hid;
    //                    }
    //                }
    //            }
    //            int wRight = posX + neigId;
    //            if (wRight < imgW)
    //            {
    //                for (int hid = hStart; hid <= hEnd; hid++)
    //                {
    //                    if (featureImg.ptr(hid, wRight)[0] > maxGray)
    //                    {
    //                        maxGray = featureImg.ptr(hid, wRight)[0];
    //                        maxX = wRight;
    //                        maxY = hid;
    //                    }
    //                }
    //            }
    //            int wStart = posX - neigId + 1;
    //            wStart = wStart < 0 ? 0 : wStart;
    //            int wEnd = posX + neigId - 1;
    //            wEnd = wEnd >= imgW ? imgW - 1 : wEnd;
    //            int hTop = posY - neigId;
    //            if (hTop >= 0)
    //            {
    //                for (int wid = wStart; wid <= wEnd; wid++)
    //                {
    //                    if (featureImg.ptr(hTop, wid)[0] > maxGray)
    //                    {
    //                        maxGray = featureImg.ptr(hTop, wid)[0];
    //                        maxX = wid;
    //                        maxY = hTop;
    //                    }
    //                }
    //            }
    //            int hDown = posY + neigId;
    //            if (hDown < imgH)
    //            {
    //                for (int wid = wStart; wid <= wEnd; wid++)
    //                {
    //                    if (featureImg.ptr(hDown, wid)[0] > maxGray)
    //                    {
    //                        maxGray = featureImg.ptr(hDown, wid)[0];
    //                        maxX = wid;
    //                        maxY = hDown;
    //                    }
    //                }
    //            }
    //        }
    //        if (maxGray >= markGrayThre)
    //        {
    //            cvFaceFps.push_back(cv::Point2f(posX, posY));
    //            cvFaceRefFps.push_back(cv::Point2f(maxX, maxY));
    //        }
    //    }
    //    if (cvFaceFps.size() > 2)
    //    {
    //        RigidFittingFps(cvFaceFps, cvFaceRefFps, fpsList);
    //        DebugLog << "Rigid fitting: " << cvFaceFps.size() << " " << fpsSize << std::endl;
    //    }

    //    //Update fps
    //    mpFps->Set(fpsList, NULL);
    //}

    void Face2D::RigidFittingFps(const std::vector<cv::Point2f>& cvSrcList, const std::vector<cv::Point2f>& cvTargetList, 
            std::vector<int>& fps)
    {
        cv::Mat transMat = cv::estimateRigidTransform(cvSrcList, cvTargetList, false);
        MagicMath::HomoMatrix3 fpsTransform;
        fpsTransform.SetValue(0, 0, transMat.at<double>(0, 0));
        fpsTransform.SetValue(0, 1, transMat.at<double>(0, 1));
        fpsTransform.SetValue(0, 2, transMat.at<double>(0, 2));
        fpsTransform.SetValue(1, 0, transMat.at<double>(1, 0));
        fpsTransform.SetValue(1, 1, transMat.at<double>(1, 1));
        fpsTransform.SetValue(1, 2, transMat.at<double>(1, 2));
        int fpsSize = fps.size() / 2;
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            double resX, resY;
            fpsTransform.TransformPoint(fps.at(fpsId * 2 + 1), fps.at(fpsId * 2), resX, resY);
            fps.at(fpsId * 2 + 1) = floor(resX + 0.5);
            fps.at(fpsId * 2) = floor(resY + 0.5);
        }
    }

    FaceFeaturePoint* Face2D::GetFps(void)
    {
        return mpFps;
    }

    bool Face2D::LoadRefImage(const std::string& fileName)
    {
        if (mpRefImage == NULL)
        {
            mpRefImage = new cv::Mat;
        }
        mpRefImage->release();
        *mpRefImage = cv::imread(fileName);
        if (mpRefImage->data != NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    cv::Mat Face2D::GetRefImage(void)
    {
        return *mpRefImage;
    }

    bool Face2D::LoadRefFps(const std::string& fileName)
    {
        if (mpRefFps == NULL)
        {
            mpRefFps = new FaceFeaturePoint;
        }
        return mpRefFps->Load(fileName);
    }

    void Face2D::CalRefFpsByProjectPca(const std::string& path, const std::vector<int>& imgIndex)
    {
        DoFeaturePca(path, imgIndex);
        //Transfer to mean feature
        std::vector<int> fps;
        mpFps->GetFPs(fps);
        int fpsSize = fps.size() / 2;
        std::vector<double> meanFps = mpFeaturePca->GetMeanVector();
        std::vector<cv::Point2f> cvFps(fpsSize);
        std::vector<cv::Point2f> cvMeanFps(fpsSize);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            cvFps.at(fpsId).x = fps.at(fpsId * 2 + 1);
            cvFps.at(fpsId).y = fps.at(fpsId * 2);
            cvMeanFps.at(fpsId).x = meanFps.at(fpsId * 2 + 1);
            cvMeanFps.at(fpsId).y = meanFps.at(fpsId * 2);
        }
        cv::Mat transMat = cv::estimateRigidTransform(cvFps, cvMeanFps, false);
        MagicMath::HomoMatrix3 fpsTransform;
        fpsTransform.SetValue(0, 0, transMat.at<double>(0, 0));
        fpsTransform.SetValue(0, 1, transMat.at<double>(0, 1));
        fpsTransform.SetValue(0, 2, transMat.at<double>(0, 2));
        fpsTransform.SetValue(1, 0, transMat.at<double>(1, 0));
        fpsTransform.SetValue(1, 1, transMat.at<double>(1, 1));
        fpsTransform.SetValue(1, 2, transMat.at<double>(1, 2));
        std::vector<double> fps_d(fpsSize * 2);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            double resX, resY;
            fpsTransform.TransformPoint(cvFps.at(fpsId).x, cvFps.at(fpsId).y, resX, resY);
            fps_d.at(fpsId * 2 + 1) = resX;
            fps_d.at(fpsId * 2) = resY;
        }
        //Project
        std::vector<double> projFps_d = mpFeaturePca->TruncateProject(fps_d, 3);
        //Transfer back
        MagicMath::HomoMatrix3 revMat = fpsTransform.ReverseRigidTransform();
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            double resX, resY;
            revMat.TransformPoint(projFps_d.at(fpsId * 2 + 1), projFps_d.at(fpsId * 2), resX, resY);
            projFps_d.at(fpsId * 2 + 1) = resX;
            projFps_d.at(fpsId * 2) = resY;
        }
        std::vector<int> projFps(projFps_d.size());
        for (int fpsId = 0; fpsId < fpsSize * 2; fpsId++)
        {
            projFps.at(fpsId) = floor(projFps_d.at(fpsId) + 0.5);
        }
        int browNum, eyeNum, noseNum, mouseNum, borderNum;
        mpFps->GetParameter(browNum, eyeNum, noseNum, mouseNum, borderNum);
        if (mpRefFps == NULL)
        {
            mpRefFps = new FaceFeaturePoint;
        }
        mpRefFps->Load(browNum, eyeNum, noseNum, mouseNum, borderNum, projFps);
    }

    FaceFeaturePoint* Face2D::GetRefFps(void)
    {
        return mpRefFps;
    }

    cv::Mat Face2D::DeformImageByFeature(void)
    {
        //Calculate transform first
        std::vector<int> fpsList, refFpsList;
        mpFps->GetFPs(fpsList);
        mpRefFps->GetFPs(refFpsList);
        int featureSize = fpsList.size() / 2;
        std::vector<cv::Point2f> cvFpsList(featureSize);
        std::vector<cv::Point2f> cvRefFpsList(featureSize);
        for (int mid = 0; mid < featureSize; mid++)
        {
            cvFpsList.at(mid).x = fpsList.at(mid * 2 + 1);
            cvFpsList.at(mid).y = fpsList.at(mid * 2);
            cvRefFpsList.at(mid).x = refFpsList.at(mid * 2 + 1);
            cvRefFpsList.at(mid).y = refFpsList.at(mid * 2);
        }
        cv::Mat transMat = cv::estimateRigidTransform(cvRefFpsList, cvFpsList, false);
        MagicMath::HomoMatrix3 fpsTransform;
        fpsTransform.SetValue(0, 0, transMat.at<double>(0, 0));
        fpsTransform.SetValue(0, 1, transMat.at<double>(0, 1));
        fpsTransform.SetValue(0, 2, transMat.at<double>(0, 2));
        fpsTransform.SetValue(1, 0, transMat.at<double>(1, 0));
        fpsTransform.SetValue(1, 1, transMat.at<double>(1, 1));
        fpsTransform.SetValue(1, 2, transMat.at<double>(1, 2));
        
        //Deform image
        std::vector<int> dpsList, refDpsList;
        mpFps->GetDPs(dpsList);
        mpRefFps->GetDPs(refDpsList);
        int dpsSize = dpsList.size() / 2;
        for (int mid = 0; mid < dpsSize; mid++)
        {
            int temp = dpsList.at(mid * 2);
            dpsList.at(mid * 2) = dpsList.at(mid * 2 + 1);
            dpsList.at(mid * 2 + 1) = temp;

            double xRes, yRes;
            fpsTransform.TransformPoint(refDpsList.at(mid * 2 + 1), refDpsList.at(mid * 2), xRes, yRes);
            refDpsList.at(mid * 2) = xRes;
            refDpsList.at(mid * 2 + 1) = yRes;
        }
        cv::Mat deformImg = MagicDIP::Deformation::DeformByMovingLeastSquares(*mpImage, dpsList, refDpsList);

        return deformImg;
    }

    void Face2D::SetMaxImageSize(int width, int height)
    {
        int imgW = mpImage->cols;
        int imgH = mpImage->rows;
        bool resized = false;
        if (imgW > width)
        {
            imgH = int(imgH * float(width) / imgW);
            imgW = width;
            resized = true;
        }
        if (imgH > height)
        {
            imgW = int(imgW * float(height) / imgH);
            imgH = height;
            resized = true;
        }
        if (resized)
        {
            cv::Size vcSize(imgW, imgH);
            cv::Mat resizedImg(vcSize, CV_8UC3);
            cv::resize(*mpImage, resizedImg, vcSize);
            mpImage->release();
            *mpImage = resizedImg.clone();
        }
    }

    void Face2D::SetMaxRefImageSize(int width, int height)
    {
        int imgW = mpRefImage->cols;
        int imgH = mpRefImage->rows;
        bool resized = false;
        if (imgW > width)
        {
            imgH = int(imgH * float(width) / imgW);
            imgW = width;
            resized = true;
        }
        if (imgH > height)
        {
            imgW = int(imgW * float(height) / imgH);
            imgH = height;
            resized = true;
        }
        if (resized)
        {
            cv::Size vcSize(imgW, imgH);
            cv::Mat resizedImg(vcSize, CV_8UC3);
            cv::resize(*mpRefImage, resizedImg, vcSize);
            mpRefImage->release();
            *mpRefImage = resizedImg.clone();
        }
    }

    void Face2D::CalMeanFeature(const std::string& path, const std::vector<int>& imgIndex, std::vector<FaceFeaturePoint*>* fpsList,
            std::vector<cv::Point2f>* cvMeanFps)
    {
        //Load feature data
        int imgCount = imgIndex.size();
        fpsList->clear();
        fpsList->resize(imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            std::stringstream ss;
            ss << path << imgIndex.at(imgId) << ".fp";
            std::string fileName;
            ss >> fileName;
            fpsList->at(imgId) = new FaceFeaturePoint;
            fpsList->at(imgId)->Load(fileName);
        }

        //Calculate mean face
        std::vector<int> firstFps;
        fpsList->at(0)->GetFPs(firstFps);
        int fpsSize = firstFps.size() / 2;
        cvMeanFps->clear();
        cvMeanFps->resize(fpsSize);
        std::vector<cv::Point2f> cvSumFps(fpsSize);
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int iterCount = 3;
        for (int iterIndex = 0; iterIndex < iterCount; iterIndex++)
        {
            if (iterIndex == 0)
            {
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    cvMeanFps->at(fpsId).x = firstFps.at(fpsId * 2 + 1);
                    cvMeanFps->at(fpsId).y = firstFps.at(fpsId * 2);
                }
            }
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvSumFps.at(fpsId).x = 0;
                cvSumFps.at(fpsId).y = 0;
            }
            for (int imgId = 0; imgId < imgCount; imgId++)
            {
                std::vector<int> curFps;
                fpsList->at(imgId)->GetFPs(curFps);
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                    cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
                }
                cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, *cvMeanFps, false);
                MagicMath::HomoMatrix3 homoMat;
                homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
                homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
                homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
                homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
                homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
                homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    double xRes, yRes;
                    homoMat.TransformPoint(cvCurFps.at(fpsId).x, cvCurFps.at(fpsId).y, xRes, yRes);
                    cvSumFps.at(fpsId).x += xRes;
                    cvSumFps.at(fpsId).y += yRes;
                }
            }
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvMeanFps->at(fpsId).x = cvSumFps.at(fpsId).x / imgCount;
                cvMeanFps->at(fpsId).y = cvSumFps.at(fpsId).y / imgCount;
            }
        }
    }

    void Face2D::DoFeaturePca(const std::string& path, const std::vector<int>& imgIndex)
    {
        std::vector<FaceFeaturePoint*> fpsList;
        std::vector<cv::Point2f> cvMeanFps;
        CalMeanFeature(path, imgIndex, &fpsList, &cvMeanFps);
        int fpsSize = cvMeanFps.size();

        //align to mean features and collect pca data
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int dataDim = fpsSize * 2;
        int imgCount = imgIndex.size();
        std::vector<double> pcaData(dataDim * imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            std::vector<int> curFps;
            fpsList.at(imgId)->GetFPs(curFps);
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, cvMeanFps, false);
            MagicMath::HomoMatrix3 homoMat;
            homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
            int baseIndex = imgId * dataDim;
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                double xRes, yRes;
                homoMat.TransformPoint(cvCurFps.at(fpsId).x, cvCurFps.at(fpsId).y, xRes, yRes);
                pcaData.at(baseIndex + fpsId * 2) = yRes;
                pcaData.at(baseIndex + fpsId * 2 + 1) = xRes;
            }
        }

        //Do Pca
        int pcaDim = imgCount - 1;
        if (mpFeaturePca == NULL)
        {
            mpFeaturePca = new MagicML::PrincipalComponentAnalysis;
        }
        mpFeaturePca->Analyse(pcaData, dataDim, pcaDim);

        //Save Pca
        std::string pcaFileName = path + ".pca";
        mpFeaturePca->Save(pcaFileName);

        //free memory
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            delete fpsList.at(imgId);
            fpsList.at(imgId) = NULL;
        }
    }

    void Face2D::DoFeaturePca(const std::string& path, int imgCount)
    {
        std::vector<int> imgIndex(imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            imgIndex.at(imgId) = imgId;
        }
        DoFeaturePca(path, imgIndex);
    }

    MagicML::PrincipalComponentAnalysis* Face2D::GetFeaturePca(void)
    {
        return mpFeaturePca;
    }

    void Face2D::DeformFeatureToMeanFace(const std::string& path, std::vector<int>& imgIndex)
    {
        //Calculate mean feature
        std::vector<FaceFeaturePoint*> fpsList;
        std::vector<cv::Point2f> cvMeanFps;
        CalMeanFeature(path, imgIndex, &fpsList, &cvMeanFps);

        //Calculate meanDps
        int fpsSize = cvMeanFps.size();
        int browNum, eyeNum, noseNum, mouseNum, borderNum;
        fpsList.at(0)->GetParameter(browNum, eyeNum, noseNum, mouseNum, borderNum);
        std::vector<int> meanFps(fpsSize * 2);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            meanFps.at(fpsId * 2) = floor(cvMeanFps.at(fpsId).y + 0.5);
            meanFps.at(fpsId * 2 + 1) = floor(cvMeanFps.at(fpsId).x + 0.5);
        }
        FaceFeaturePoint meanFfp;
        meanFfp.Load(browNum, eyeNum, noseNum, mouseNum, borderNum, meanFps);
        std::vector<int> meanDps;
        meanFfp.GetDPs(meanDps);
        int dpsSize = meanDps.size() / 2;
        for (int dpsId = 0; dpsId < dpsSize; dpsId++)
        {
            int temp = meanDps.at(dpsId * 2);
            meanDps.at(dpsId * 2) = meanDps.at(dpsId * 2 + 1);
            meanDps.at(dpsId * 2 + 1) = temp;
        }

        //Deform to mean
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int imgCount = imgIndex.size();
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            //Calculate transformation
            std::vector<int> curFps;
            fpsList.at(imgId)->GetFPs(curFps);
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, cvMeanFps, false);
            MagicMath::HomoMatrix3 homoTransMat;
            homoTransMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoTransMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoTransMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoTransMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoTransMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoTransMat.SetValue(1, 2, transMat.at<double>(1, 2));

            //Load image
            std::stringstream ss;
            ss << path << imgIndex.at(imgId) << ".jpg";
            std::string imgName;
            ss >> imgName;
            ss.clear();
            cv::Mat img = cv::imread(imgName);
            cv::Mat uniformImg(img.rows, img.cols, img.type());
            cv::warpAffine(img, uniformImg, transMat, uniformImg.size());

            //Deform
            std::vector<int> curDps;
            fpsList.at(imgId)->GetDPs(curDps);
            for (int dpsId = 0; dpsId < dpsSize; dpsId++)
            {
                double xRes, yRes;
                homoTransMat.TransformPoint(curDps.at(dpsId * 2 + 1), curDps.at(dpsId * 2), xRes, yRes);
                curDps.at(dpsId * 2) = floor(xRes + 0.5);
                curDps.at(dpsId * 2 + 1) = floor(yRes + 0.5);
            }
            cv::Mat deformImg = MagicDIP::Deformation::DeformByMovingLeastSquares(uniformImg, curDps, meanDps);

            //Write image
            ss << path << "ToMean" << imgIndex.at(imgId) << ".jpg";
            std::string transImgName;
            ss >> transImgName;
            ss.clear();
            cv::imwrite(transImgName, deformImg);
        }
    }

    void Face2D::DeformFeatureToMeanFace(const std::string& path, int imgCount)
    {
        std::vector<int> imgIndex(imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            imgIndex.at(imgId) = imgId;
        }
        DeformFeatureToMeanFace(path, imgIndex);
    }

    void Face2D::CalMeanFace(const std::string& path, std::vector<int>& imgIndex)
    {
        DeformFeatureToMeanFace(path, imgIndex);
        int imgW, imgH;
        std::vector<int> sumBlue, sumGreen, sumRed;
        int imgCount = imgIndex.size();
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            //Load file
            std::stringstream ss;
            ss << path << "ToMean" << imgIndex.at(imgId) << ".jpg";
            std::string imgName;
            ss >> imgName;
            cv::Mat img = cv::imread(imgName);
            if (imgId == 0)
            {
                imgW = img.cols;
                imgH = img.rows;
                int imgSize = imgW * imgH;
                sumBlue = std::vector<int>(imgSize, 0);
                sumGreen = std::vector<int>(imgSize, 0);
                sumRed = std::vector<int>(imgSize, 0);
            }
            for (int hid = 0; hid < imgH; hid++)
            {
                int baseIndex = hid * imgW;
                for (int wid = 0; wid < imgW; wid++)
                {
                    unsigned char* pixel = img.ptr(hid, wid);
                    sumBlue.at(baseIndex + wid) += pixel[0];
                    sumGreen.at(baseIndex + wid) += pixel[1];
                    sumRed.at(baseIndex + wid) += pixel[2];
                }
            }
        }
        cv::Mat meanImg(imgH, imgW, CV_8UC3);
        for (int hid = 0; hid < imgH; hid++)
        {
            int baseIndex = hid * imgW;
            for (int wid = 0; wid < imgW; wid++)
            {
                unsigned char* pixel = meanImg.ptr(hid, wid);
                pixel[0] = float(sumBlue.at(baseIndex + wid)) / imgCount;
                pixel[1] = float(sumGreen.at(baseIndex + wid)) / imgCount;
                pixel[2] = float(sumRed.at(baseIndex + wid)) / imgCount;
            }
        }
        std::string meanImgName = path + "Mean.jpg";
        cv::imwrite(meanImgName, meanImg); 
    }

    void Face2D::CalMeanFace(const std::string& path, int imgCount)
    {
        std::vector<int> imgIndex(imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            imgIndex.at(imgId) = imgId;
        }
        CalMeanFace(path, imgIndex);
    }
}
