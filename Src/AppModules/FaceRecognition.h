#pragma once
#include "MagicObject.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

namespace MagicDIP
{
    class HighDimensionalFeature;
}

namespace MagicApp
{
    class FaceRecognition : public MagicApp::MagicObject
    {
    public:
        FaceRecognition();
        ~FaceRecognition();
        void Reset();

        //Preprocess face data
        int AlignFace(const std::string& faceImgFiles, const std::string& alignedFilePath);

        //Learn face features
        int LearnHighDimensionalFeature(const std::string& fileListName);

        //Unsurpervised clustering high dimensional features
        int KMeansClusteringHighDimensionalFeature(const std::string& fileListName, const std::string& resPath);
        
    private:
        MagicDIP::HighDimensionalFeature* mpHdFeature;
    };
}
