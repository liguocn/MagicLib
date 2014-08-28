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
        HaarClassifier(int id, const HaarFeature& feature);
        ~HaarClassifier();

        void SetFeature(const HaarFeature& feature);
        HaarFeature GetFeature(void) const;
        double GetThreshold(void) const;
        bool IsLess(void) const;
        int Learn(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, const std::vector<int>& faceIndex, 
            const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex, 
            double* trainError, int boxSize, int avgImgGray);
        int Predict(const std::vector<unsigned int>& integralImg, int imgW, 
            int sRow, int sCol, int boxSize, float scale, int avgImgGray) const;
        int Predict(const ImageLoader& imgLoader, int dataId, int boxSize, int avgImgGray) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);
        double CalFeatureSimilarity(const HaarFeature& hf) const;
        void SaveFeatureAsImage(const std::string& imgName, int baseSize) const;

    private:
        //int CalFeatureValue(const ImageLoader& imgLoader, int dataId) const;
        //int ImgBoxValue(const ImageLoader& imgLoader, int dataId, int sRow, int sCol, int eRow, int eCol) const;
        /*int CalFeatureValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, float scale) const;
        int ImgBoxValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, int eRow, int eCol) const;
        unsigned int GetIntegralValue(const std::vector<unsigned int>& integralImg, int imgW, int hid, int wid) const;*/
        //imgType: 0-non face, 1-face
        int CalFeature(const ImageLoader& imgLoader, int imgType, int dataId, int boxSize, int avgImgGray) const;

    private:
        int mId;
        HaarFeature mFeature;
        double mThreshold;
        bool mIsLess;
    };

    class AdaBoostFaceDetection
    {
    public:
        AdaBoostFaceDetection();
        AdaBoostFaceDetection(double detectionRate);
        ~AdaBoostFaceDetection();

        int Learn(const ImageLoader& faceImgLoader, const std::vector<bool>& faceValidFlag,
            const ImageLoader& nonFaceImgLoader, const std::vector<bool>& nonFaceValidFlag,
            int levelCount, int boxSize, int avgImgGray);
        int Predict(const std::vector<unsigned int>& integralImg, int imgW, 
            int sRow, int sCol, int boxSize, double scale, int avgImgGray) const;
        int Predict(const ImageLoader& imgLoader, int dataId, int boxSize, int avgImgGray) const;
        void Save(std::ofstream& fout) const;
        void Load(std::ifstream& fin);
        void SaveFeatureAsImage(const std::string& filePath, int baseSize, int detectorId) const;

    private:
        void GenerateClassifierCadidates(int baseImgSize);
        std::vector<int> SampleHaarFeatures(const std::vector<HaarFeature> features, double sampleRate) const;
        void GenerateFeatureValueCache(const ImageLoader* pFaceImgLoader, const std::vector<int>& faceIndex,
            const ImageLoader* pNonFaceImgLoader, const std::vector<int>& nonFaceIndex, int boxSize, int avgImgGray) const;
        void ClearFeatureValueCache(void) const;
        int RemoveSimilarClassifierCandidates(const HaarFeature& hf);
        void ClearClassifierCadidates(void);
        int TrainWeakClassifier(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
            const std::vector<int>& faceIndex,
            const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, 
            const std::vector<int>& nonFaceIndex, int boxSize, int avgImgGray);
        void Reset(void);

    private:
        double mDetectionRate;
        std::vector<HaarClassifier*> mClassifiers;
        std::vector<double> mClassifierWeights;
        double mThreshold;
        //Cache classifier candidates
        std::vector<HaarClassifier*> mClassifierCandidates;
    };

    class RealTimeFaceDetection
    {
    public:
        RealTimeFaceDetection();
        ~RealTimeFaceDetection();

        int Learn(const std::vector<std::string>& faceImages, const std::vector<std::string>& nonFaceImages);
        int Detect(const cv::Mat& img, std::vector<int>& faces) const;
        int DetectSpecialLocation(const cv::Mat& img, int rid, int cid, int subSize, double scale, std::vector<int>& faces) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
        void SaveFeatureAsImage(const std::string& filePath) const;

    private:
        int DetectOneFace(const std::vector<unsigned int>& integralImg, int imgW, 
            int sRow, int sCol, int boxSize, double scale, int avgImgGray) const;
        void PostProcessFaces(std::vector<int>& faces) const;
        bool IsTheSameFace(int sRowA, int sColA, int lRowA, int lColA, int sRowB, int sColB, int lRowB, int lColB) const;
        void Reset(void);

    private:
        int mBaseImgSize;
        int mAvgImgGray; // This is a hard code here, we use 100 in training data.
        std::vector<AdaBoostFaceDetection* > mCascadedDetectors;
    };
}
