#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class FisherVectorFace
    {
    public:
        FisherVectorFace();
        ~FisherVectorFace();
        
        int LearnRepresentation(const std::vector<std::string>& imgFiles);
        std::vector<double> GetRepresentation(const cv::Mat& img) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
    };
}

