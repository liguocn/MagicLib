#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

namespace MagicML
{
    class RandomFern;
}

namespace MagicDIP
{
    class CascadedPoseRegression
    {
    public:
        CascadedPoseRegression();
        ~CascadedPoseRegression();

        int LearnRegression(const std::string& landFileName);
        void PoseRegression(const cv::Mat& img, const std::vector<double>& dataX, std::vector<double>&dataY) const;

    private:
        void Reset(void);

    private:
        std::vector<MagicML::RandomFern* > mRandomFerns;
    };

    
}
