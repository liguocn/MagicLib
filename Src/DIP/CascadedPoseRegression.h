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
        virtual ~CascadedPoseRegression();

        int LearnRegression(const std::vector<std::string>& imgFiles, const std::vector<double>& initTheta, 
            const std::vector<double>& finalTheta, int thetaDim, int fernCount, int fernSize, int featureSize);
        int PoseRegression(const cv::Mat& img, const std::vector<double>& initTheta, std::vector<double>&finalTheta) const;
        virtual void FeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, std::vector<bool>& features) const;

    private:
        void Reset(void);

    private:
        int mFeatureSize;
        std::vector<MagicML::RandomFern* > mRandomFerns;
    };

    
}
