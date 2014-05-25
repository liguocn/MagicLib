#pragma once
#include <string>
#include <vector>
#include "FaceFeaturePoints.h"
#include "../MachineLearning/PrincipalComponentAnalysis.h"
#include "opencv2/opencv.hpp"
#include "MagicObject.h"

namespace MagicApp
{
    class Face2DPCA : public MagicObject
    {
    public:
        Face2DPCA();
        ~Face2DPCA();

        void DoFeaturePca(const std::string& path, const std::vector<int>& imgIndex);
        MagicML::PrincipalComponentAnalysis* GetFeaturePca(void);
        void CalMeanFace(const std::string& path, std::vector<int>& imgIndex);

    private:
        void DeformFeatureToMeanFace(const std::string& path, std::vector<int>& imgIndex);
        void CalMeanFeature(const std::string& path, const std::vector<int>& imgIndex, 
            std::vector<Face2DFeaturePoints*>* ffpList, std::vector<cv::Point2f>* cvMeanFps);

    private:
        MagicML::PrincipalComponentAnalysis* mpFeaturePca;
    };

    class Face2DObj : public MagicObject
    {
    public:
        Face2DObj();
        ~Face2DObj();

        bool LoadFaceImage(const std::string& fileName);
        cv::Mat GetFaceImage(void);
        void ResizeImage(int width, int height, bool keepRatio);
        bool LoadFfp(const std::string& fileName);
        Face2DFeaturePoints* GetFfp(void);
        void SaveFfp(const std::string& fileName);

        void AutoAlignFfp(const std::vector<double>& markPos, const cv::Mat& featureImg,
            const MagicML::PrincipalComponentAnalysis& featurePca); //3 mark point: 2 eyes, 1 mouse.
        void DeformToFeaturePcaSpace(const MagicML::PrincipalComponentAnalysis& featurePca);
        void DeformToFeature(const Face2DFeaturePoints& refFfp);

    public:
        cv::Mat* mpImage;
        Face2DFeaturePoints* mpFfp;
    };
}
