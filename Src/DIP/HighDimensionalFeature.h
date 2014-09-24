#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

namespace MagicMath
{
    class SparseMatrix;
}

namespace MagicDIP
{
    class HighDimensionalFeature
    {
    public:
        HighDimensionalFeature();
        ~HighDimensionalFeature();
        
        int Learn(const std::vector<std::string>& imgFiles, const std::vector<int>& marksList, int markCountPerImg, 
            const std::vector<int> faceIds);
        int GetHighDimensionalFeature(const cv::Mat& img, const std::vector<int>& marksList, std::vector<double>& feature) const;
        int GetCompressedFeature(const cv::Mat& img, const std::vector<int>& marksList, std::vector<double>& feature) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
        void Reset(void);

    private:
        void ConstructUniformPatternMap(void);
        std::vector<double> CalFaceFeature(const cv::Mat& img, const std::vector<int>& marksList) const;
        void CalLocalMarkFeature(const cv::Mat& img, int markRow, int markCol, std::vector<double>& features) const;
        void CalPixelLbpValue(const cv::Mat& img, int pixelRow, int pixelCol, int& lbpValue) const;
        
    private:
        MagicMath::SparseMatrix* mpProjectMat;
        //Algorithm parameters
        int mTargetDim;
        int mMultiScaleCount;
        double mMultiScaleValue;
        int mPatchSize;
        int mCellSize;
        //cache
        std::vector<int> mUniformPatternMap;
    };
}

