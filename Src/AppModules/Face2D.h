#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "../MachineLearning/PrincipalComponentAnalysis.h"

namespace MagicApp
{
    class FaceFeaturePoint
    {
    public:
        enum FeatureType
        {
            FT_None = 0,
            FT_Left_Brow,
            FT_Right_Brow,
            FT_Left_Eye,
            FT_Right_Eye,
            FT_Mouse,
            FT_Nose,
            FT_Border
        };

        FaceFeaturePoint();
        
        //void Load();
        bool Load(const std::string& fileName);
        void Load(int browNum, int eyeNum, int noseNum, int mouseNum, int borderNum, const std::vector<int>& posList);
        void Set(const std::vector<int>& posList, const FaceFeaturePoint* refFps);
        void Save(const std::string& fileName);
        bool Select(int hid, int wid);
        void MoveTo(int hid, int wid);
        void MoveDelta(int deltaH, int deltaW);
        void GetDPs(std::vector<int>& posList) const; // h, w
        void GetFPs(std::vector<int>& posList) const;
        void GetParameter(int& browNum, int& eyeNum, int& noseNum, int& mouseNum, int& borderNum) const;
        void GetEyeCenter(double& leftX, double& leftY, double& rightX, double& rightY) const;
        void GetMouseCenter(double& x, double& y) const;

        ~FaceFeaturePoint();

    private:
        void UpdateDPs();
        void ConstructOneDPs(const std::vector<int>& fps, bool isClosed, int addSize, std::vector<int>& dps);

    private:
        int mSelectIndex;
        FeatureType mSelectType;
        std::vector<int> mLeftBrowFPs;
        std::vector<int> mLeftBrowDPs;
        std::vector<int> mRightBrowFPs;
        std::vector<int> mRightBrowDPs;
        std::vector<int> mLeftEyeFPs;
        std::vector<int> mLeftEyeDPs;
        std::vector<int> mRightEyeFPs;
        std::vector<int> mRightEyeDPs;
        std::vector<int> mNoseFPs;
        std::vector<int> mNoseDPs;
        std::vector<int> mMouseFPs;
        std::vector<int> mMouseDPs;
        std::vector<int> mBorderFPs;
        std::vector<int> mBorderDPs;
    };

    class Face2D
    {
    public:
        Face2D();
        ~Face2D();

        bool LoadImage(const std::string& fileName);
        cv::Mat GetImage(void);
        void GetImageSize(int* imgW, int* imgH);
        bool LoadFps(const std::string& fileName);
        void AutoAlignFps(const std::vector<int>& markPosList); //3 mark point: 2 eyes, 1 mouse.
        void AutoAlignFps(void);
        FaceFeaturePoint* GetFps(void);
        bool LoadRefImage(const std::string& fileName);
        cv::Mat GetRefImage(void);
        bool LoadRefFps(const std::string& fileName);
        void CalRefFpsByProjectPca(const std::string& path, const std::vector<int>& imgIndex);
        FaceFeaturePoint* GetRefFps(void);
        cv::Mat DeformImageByFeature(void);
        void SetMaxImageSize(int width, int height);
        void SetMaxRefImageSize(int width, int height);
        void DoFeaturePca(const std::string& path, int imgCount);
        void DoFeaturePca(const std::string& path, const std::vector<int>& imgIndex);
        MagicML::PrincipalComponentAnalysis* GetFeaturePca(void);
        void DeformFeatureToMeanFace(const std::string& path, int imgCount);
        void DeformFeatureToMeanFace(const std::string& path, std::vector<int>& imgIndex);
        void CalMeanFace(const std::string& path, int imgCount);
        void CalMeanFace(const std::string& path, std::vector<int>& imgIndex);

    private:
        void CalMeanFeature(const std::string& path, const std::vector<int>& imgIndex, std::vector<FaceFeaturePoint*>* fpsList,
            std::vector<cv::Point2f>* cvMeanFps);
        void RigidFittingFps(const std::vector<cv::Point2f>& cvSrcList, const std::vector<cv::Point2f>& cvTargetList, 
            std::vector<int>& fps);

    private:
        cv::Mat* mpImage;
        FaceFeaturePoint* mpFps;
        cv::Mat* mpRefImage;
        FaceFeaturePoint* mpRefFps;
        MagicML::PrincipalComponentAnalysis* mpFeaturePca;
    };

}
