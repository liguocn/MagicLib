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
                  int multiScaleCount, int targetDim);
        int GetHighDimensionalFeature(const cv::Mat& img, const std::vector<int>& marksList, std::vector<int>& feature) const;
        int GetCompressedFeature(const cv::Mat& img, const std::vector<int>& marksList, std::vector<int>& feature) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
        void Reset(void);
        
    private:
        int mMarkCount;
        int mMultiScaleCount;
        MagicMath::SparseMatrix* mpProjectMat;
    };
}

