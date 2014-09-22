#pragma once
#include "MagicObject.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

namespace MagicApp
{
    class FaceRecognition : public MagicApp::MagicObject
    {
    public:
        enum RecognitionMethod
        {
            RM_Default
        };
        
        FaceRecognition();
        ~FaceRecognition();
        
        //Preprocess face data
        int AlignFace(const std::string& faceImgFiles, const std::string& alignedFilePath);
        
    private:
        RecognitionMethod mRecognitionMethod;
        
    };
}
