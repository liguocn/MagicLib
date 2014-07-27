#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "ImageLoader.h"

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
            const std::vector<double>& finalTheta, int dataPerImgCount, int thetaDim, int fernCount, int fernSize, int featureSize);
        int PoseRegression(const cv::Mat& img, const std::vector<double>& initTheta, std::vector<double>&finalTheta) const;
        virtual void Save(const std::string& fileName) const = 0;
        virtual void Load(const std::string& fileName) = 0;

    protected:
        virtual void FeaturePatternGeneration(const std::vector<double>& theta, const std::vector<double>& dataY, 
            int dataPerImgCount, int dataCount, int featureSize, std::vector<bool>& features) = 0;
        virtual void UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds) = 0;
        virtual void ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const = 0;
        virtual void ValidFeatureGenerationByImageLoader(int imgId, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const = 0;
        virtual void Reset(void);

    protected:
        std::vector<MagicML::RandomFern* > mRandomFerns;
        //cache
        ImageLoader mImageLoader;
    };

    class SimpleCascadedPoseRegression : public CascadedPoseRegression
    {
    public:
        SimpleCascadedPoseRegression();
        SimpleCascadedPoseRegression(int patchSize);
        virtual ~SimpleCascadedPoseRegression();

        virtual void Save(const std::string& fileName) const;
        virtual void Load(const std::string& fileName);

    protected:
        virtual void FeaturePatternGeneration(const std::vector<double>& theta, const std::vector<double>& dataY, 
            int dataPerImgCount, int dataCount, int featureSize, std::vector<bool>& features);
        virtual void UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds);
        virtual void ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const;
        virtual void ValidFeatureGenerationByImageLoader(int imgId, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const;
        virtual void Reset(void);

    private:
        void ScaleToPatchCoord(int pos, int& imgRow, int& imgCol) const;

    private:
        int mValidFeatureSize;
        int mImgPatchSize; //odd number
        std::vector<int> mValidFeaturePosPairs;
        //temporary cache
        std::vector<int> mFeaturePosPairs;
    };

}
