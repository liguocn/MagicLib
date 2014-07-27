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
    class ExplicitShapeRegression
    {
    public:
        ExplicitShapeRegression();
        ExplicitShapeRegression(int patchSize);
        ~ExplicitShapeRegression();

        int LearnRegression(const std::vector<std::string>& imgFiles, const std::vector<double>& initTheta, 
            const std::vector<double>& finalTheta, int dataPerImgCount, int keyPointCount, int outerCount, int innerCount, 
            int fernSize, int featureSizePerKey);
        int ShapeRegression(const cv::Mat& img, const std::vector<double>& initTheta, std::vector<double>&finalTheta) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);

    private:
        void FeaturePatternGeneration(const std::vector<double>& theta, const std::vector<double>& dataY, int dataPerImgCount, 
            int dataCount, int featureSizePerKey, int keyPointCount, std::vector<bool>& features);
        void ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const;
        void ValidFeatureGenerationByImageLoader(int imgId, const std::vector<double>& theta, int fernId, std::vector<bool>& features) const;
        void UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds);
        void ScaleToPatchCoord(int pos, int& keyId, int& imgRow, int& imgCol) const;
        void Reset(void);

    private:
        int mInnerFernCount;
        int mValidFeatureSize;
        int mImgPatchSize; //odd number
        std::vector<int> mValidFeaturePosPairs;
        std::vector<MagicML::RandomFern* > mRandomFerns;
        ImageLoader mImageLoader;
        //temporary cache
        std::vector<int> mFeaturePosPairs;
    };
}
