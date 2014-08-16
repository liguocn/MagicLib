#pragma once
#include "MagicObject.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

namespace MagicDIP
{
    class RealTimeFaceDetection;
}

namespace MagicApp
{
    class FaceDetection : public MagicObject
    {
    public:
        enum DetectionMethod
        {
            DM_Default
        };

        FaceDetection();
        ~FaceDetection();

        int LearnDetector(const std::string& faceFile, const std::string& nonFaceFile, DetectionMethod dm);
        int DetectFace(const cv::Mat& img, std::vector<int>& faces);
        void Save(const std::string& fileName, const std::string& detectFileName) const;
        void Load(const std::string& fileName);
        void SaveFeatureAsImage(const std::string& filePath) const;

    private:
        int LearnRealTimeDetector(const std::string& faceFile, const std::string& nonFaceFile);
        int GetImageNames(const std::string& imgfile, std::vector<std::string>& imgNames) const;

    private:
        DetectionMethod mDm;
        MagicDIP::RealTimeFaceDetection* mpRealTimeDetector;
    };

    
}
