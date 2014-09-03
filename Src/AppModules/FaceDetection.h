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
        int DetectFace(const cv::Mat& img, std::vector<int>& faces) const;
        void Save(const std::string& fileName, const std::string& detectFileName) const;
        void SaveByEnhanceThreshold(const std::string& fileName, const std::string& detectFileName, double percentage) const;
        void Load(const std::string& fileName);
        void SaveFeatureAsImage(const std::string& filePath) const;

        //Tools for experiments
        void GenerateFalsePositivesFromLandFile(const std::string& fileName, const std::string& outputPath) const;
        void GenerateFalsePositivesFromNonFace(const std::string& fileName, const std::string& outputPath) const;
        void TestFaceDetectionInFile(const std::string& fileName, const std::string& outputPath) const;

        //Tools for experiments
        static void TuneGrayValues(const std::string& fileName);
        static void GenerateTrainingFacesFromLandFile(const std::string& fileName, const std::string& outputPath);

    private:
        int LearnRealTimeDetector(const std::string& faceFile, const std::string& nonFaceFile);
        int GetImageNames(const std::string& imgfile, std::vector<std::string>& imgNames) const;

    private:
        DetectionMethod mDm;
        MagicDIP::RealTimeFaceDetection* mpRealTimeDetector;
    };

    
}
