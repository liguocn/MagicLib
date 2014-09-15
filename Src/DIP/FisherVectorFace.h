#pragma once
#include <vector>
#include <string>
#include "../MachineLearning/GaussianMixtureModel.h"
#include "../Math/DenseMatrix.h"
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class FisherVectorFace
    {
    public:
        FisherVectorFace();
        ~FisherVectorFace();
        
        int LearnRepresentation(const std::vector<std::string>& imgFiles, const std::vector<int>& imgIds, 
            int gmmK, int fisherDim, int binaryDim);
        int GetRepresentation(const cv::Mat& img, std::vector<double>& fisherRep) const;
        int GetBinaryRepresentation(const cv::Mat& img, std::vector<bool>& binaryRep) const;
        void Save(const std::string& fileName) const;
        void Load(const std::string& fileName);
        void Reset(void);

    private:
        int CalFaceSIFTFeature(const std::vector<std::string>& imgFiles, std::vector<double>& siftFeatures) const;
        int CalFaceSIFTFeature(const cv::Mat& img, std::vector<double>& siftFeatures) const;

        int CalRawFisherVector(const std::vector<double>& siftFeatures, int dataCount, std::vector<double>& rawFisherVec) const;
        int CalRawFisherVector(const std::vector<double>& siftFeatures, std::vector<double>& rawFisherVec) const;

        int DimReduction(const std::vector<double>& rawFisherVec, const std::vector<int>& imgIds, int fisherDim, 
            std::vector<double>& reducedFisherVec);
        int DimReduction(const std::vector<double>& rawFisherVec, std::vector<double>& reducedFisherVec) const;

        int BinaryCompression(const std::vector<double>& reducedFisherVec, int dataCount, int binaryDim); 
        int BinaryCompression(const std::vector<double>& reducedFisherVec, std::vector<bool>& binaryRep) const;       

    private:
        MagicML::GaussianMixtureModel* mpGMM;
        MagicMath::DenseMatrix* mpDimReductionMat;
        MagicMath::DenseMatrix* mpBinaryCompressMat;
    };
}

