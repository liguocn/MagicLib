#pragma once
#include "MagicObject.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

namespace MagicDIP
{
    class CascadedPoseRegression;
    class ExplicitShapeRegression;
}

namespace MagicApp
{
    class CascadedFaceFeatureDetection : public MagicObject
    {
    public:
        CascadedFaceFeatureDetection();
        ~CascadedFaceFeatureDetection();

        int LearnRegression(const std::string& landFile);
        int PoseRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);

    private:
        MagicDIP::CascadedPoseRegression* mpRegression;
    };

    class ShapeFaceFeatureDetection : public MagicObject
    {
    public:
        ShapeFaceFeatureDetection();
        ~ShapeFaceFeatureDetection();

        int LearnRegression(const std::string& landFile);
        int ShapeRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
        std::vector<double> GetMeanFace(void) const;
        int CalMeanFace(const std::string& landFile);
        bool IsMeanFaceExist(void) const;

    private:
        MagicDIP::ExplicitShapeRegression* mpRegression;
        std::vector<double> mMeanFace; //row, col
    };

}
