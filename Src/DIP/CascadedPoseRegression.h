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

    protected:
        virtual void FeaturePatternGeneration(const std::vector<std::string>& imgFiles, const std::vector<double>& theta, 
            const std::vector<double>& dataY, int dataCount, int featureSize, std::vector<bool>& features) = 0;
        virtual void UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds) = 0;
        virtual void ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const = 0;
        virtual void Reset(void);

    private:
        std::vector<MagicML::RandomFern* > mRandomFerns;
    };

    class SimpleCascadedPoseRegression : public CascadedPoseRegression
    {
    public:
        SimpleCascadedPoseRegression();
        SimpleCascadedPoseRegression(int patchSize);
        virtual ~SimpleCascadedPoseRegression();

    protected:
        virtual void FeaturePatternGeneration(const std::vector<std::string>& imgFiles, const std::vector<double>& theta, 
            const std::vector<double>& dataY, int dataCount, int featureSize, std::vector<bool>& features);
        virtual void UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds);
        virtual void ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const;
        virtual void Reset(void);

    private:
        void ScaleToPatchCoord(int pos, int& imgRow, int& imgCol) const;

    private:
        int mValidFeatureSize;
        int mImgPatchSize; //odd number
        std::vector<int> mValidFeaturePosPairs;
        //cache
        std::vector<int> mFeaturePosPairs;
    };

}
