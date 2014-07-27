#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "ImageLoader.h"

namespace MagicDIP
{
    struct HaarFeature
    {
        int sRow;
        int sCol;
        int lRow;
        int lCol;
        int type;
    };

    class HaarClassifier
    {
    public:
        HaarClassifier();
        ~HaarClassifier();

        int Learn(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, const ImageLoader& nonFaceImgLoader,
            const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex, double* trainError);
        int Predict(const cv::Mat& img, int sRow, int sCol, float scale) const;
        int Predict(const ImageLoader& imgLoader, int dataId) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);

    private:
        HaarFeature mFeature;
        int mThreshold;
        bool mIsLess;
        //cache
    };

    class AdaBoostFaceDetection
    {
    public:
        AdaBoostFaceDetection();
        ~AdaBoostFaceDetection();

        int Learn(const ImageLoader& faceImgLoader, const ImageLoader& nonFaceImgLoader, const std::vector<bool>& nonFaceValidFlag,
            int levelCount);
        int Predict(const cv::Mat& img, int sRow, int sCol, double scale) const;
        int Predict(const ImageLoader& imgLoader, int dataId) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);

    private:
        HaarClassifier* TrainWeakClassifier(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
            const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, 
            const std::vector<int>& nonFaceIndex) const;
        void Reset(void);

    private:
        std::vector<HaarClassifier*> mClassifiers;
        std::vector<double> mClassifierWeights;
        double mThreshold;
    };

    class RealTimeFaceDetection
    {
    public:
        RealTimeFaceDetection();
        ~RealTimeFaceDetection();

        int Learn(const std::vector<std::string>& faceImages, const std::vector<std::string>& nonFaceImages, 
            const std::vector<int>& layerCounts);
        int Detect(const cv::Mat& img, std::vector<int>& faces) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);

    private:
        int DetectOneFace(const cv::Mat& img, int sRow, int sCol, double scale) const;
        void Reset(void);

    private:
        int mBaseImgSize;
        std::vector<AdaBoostFaceDetection* > mCascadedDetectors;
    };
}
