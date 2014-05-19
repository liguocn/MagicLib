#pragma once
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class SaliencyDetection
    {
    public:
        SaliencyDetection();
        ~SaliencyDetection();

        static cv::Mat DoGBandSaliency(const cv::Mat& inputImg);
        static cv::Mat GradientSaliency(const cv::Mat& inputImg);
        static cv::Mat DoGAndGradientSaliency(const cv::Mat& inputImg);
        static cv::Mat MultiScaleDoGBandSaliency(const cv::Mat& inputImg, int wNum, int hNum);

    private:
        static int MaxNumber(int n0, int n1, int n2);
    };
}
