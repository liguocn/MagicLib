#pragma once
#include "opencv2/opencv.hpp"
#include <vector>

namespace MagicDIP
{
    class Deformation
    {
    public:
        Deformation();
        ~Deformation();

        //index: w0, h0, w1, h1, w2, h2, .......
        static cv::Mat DeformByMovingLeastSquares(const cv::Mat& inputImg, 
            const std::vector<int>& originIndex, const std::vector<int>& targetIndex);

    };

}
