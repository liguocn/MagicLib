#pragma once
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class Segmentation
    {
    public:
        Segmentation();
        ~Segmentation();

        //return mat: (0, 0, 0) for the front, (255, 0, 0) for the back.
        static cv::Mat SegmentByGraphCut(const cv::Mat& inputImg, const cv::Mat& markImg);

    private:
        static double PixelDistance2(const unsigned char* pixel1, const unsigned char* pixel2); 
    };

}
