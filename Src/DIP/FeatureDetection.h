#pragma once
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class FeatureDetection
    {
    public:
        FeatureDetection();
        ~FeatureDetection();

        static cv::Mat CannyEdgeDetection(const cv::Mat& img);

    };

}
