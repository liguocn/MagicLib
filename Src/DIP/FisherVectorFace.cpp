#include "FisherVectorFace.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Tool/Profiler.h"

namespace MagicDIP
{
    FisherVectorFace::FisherVectorFace() : 
        mpGMM(NULL),
        mpDimReductionMat(NULL),
        mpBinaryCompressMat(NULL)
    {
        
    }
    
    FisherVectorFace::~FisherVectorFace()
    {
        Reset();
    }
    
    int FisherVectorFace::LearnRepresentation(const std::vector<std::string>& imgFiles, const std::vector<int>& imgIds, 
        int gmmK, int fisherDim, int binaryDim)
    {
        if (imgFiles.size() ==0 || imgFiles.size() != imgIds.size() || gmmK < 1)
        {
            return MAGIC_INVALID_INPUT;
        }
        Reset();

        int imgCount = imgFiles.size();
        std::vector<double> siftFeatures;
        if (CalFaceSIFTFeature(imgFiles, siftFeatures) != MAGIC_NO_ERROR)
        {
            InfoLog << "error: CalFaceSIFTFeatures failed" << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }
        int siftDim = siftFeatures.size() / imgCount;

        mpGMM = new MagicML::GaussianMixtureModel;
        std::vector<int> gmmClusterIndex;
        if (mpGMM->CalParameter(siftFeatures, siftDim, gmmK, gmmClusterIndex) != MAGIC_NO_ERROR)
        {
            InfoLog << "error: CalGMM Failed " << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }

        std::vector<double> rawFisherVec;
        if (CalRawFisherVector(siftFeatures, imgCount, rawFisherVec) != MAGIC_NO_ERROR)
        {
            InfoLog << "error: CalRawFisherVector failed" << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }
        siftFeatures.clear();

        std::vector<double> reducedFisherVec;
        if (DimReduction(rawFisherVec, imgIds, fisherDim, reducedFisherVec) != MAGIC_NO_ERROR)
        {
            InfoLog << "error: DimReduction failed" << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }
        rawFisherVec.clear();

        if (BinaryCompression(reducedFisherVec, imgCount, binaryDim) != MAGIC_NO_ERROR)
        {
            InfoLog << "error: BinaryCompression failed" << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }

        return MAGIC_NO_ERROR;
    }
    
    int FisherVectorFace::GetRepresentation(const cv::Mat& img, std::vector<double>& fisherRep) const
    {
        std::vector<double> siftFeatures;
        if (CalFaceSIFTFeature(img, siftFeatures) != MAGIC_NO_ERROR)
        {
            return MAGIC_INVALID_RESULT;
        }

        std::vector<double> rawFisherVec;
        if (CalRawFisherVector(siftFeatures, rawFisherVec) != MAGIC_NO_ERROR)
        {
            return MAGIC_INVALID_RESULT;
        }
        siftFeatures.clear();

        fisherRep.clear();
        return DimReduction(rawFisherVec, fisherRep);
    }
        
    int FisherVectorFace::GetBinaryRepresentation(const cv::Mat& img, std::vector<bool>& binaryRep) const
    {
        std::vector<double> fisherRep;
        if (GetRepresentation(img, fisherRep) != MAGIC_NO_ERROR)
        {
            return MAGIC_INVALID_RESULT;
        }

        binaryRep.clear();
        return BinaryCompression(fisherRep, binaryRep);
    }
    
    void FisherVectorFace::Save(const std::string& fileName) const
    {
        
    }
    
    void FisherVectorFace::Load(const std::string& fileName)
    {
        
    }

    void FisherVectorFace::Reset(void)
    {
        if (mpGMM != NULL)
        {
            delete mpGMM;
            mpGMM = NULL;
        }
        if (mpDimReductionMat != NULL)
        {
            delete mpDimReductionMat;
            mpDimReductionMat = NULL;
        }
        if (mpBinaryCompressMat != NULL)
        {
            delete mpBinaryCompressMat;
            mpBinaryCompressMat = NULL;
        }
    }

    int FisherVectorFace::CalFaceSIFTFeature(const std::vector<std::string>& imgFiles, std::vector<double>& siftFeatures) const
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::CalFaceSIFTFeature(const cv::Mat& img, std::vector<double>& siftFeatures) const
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::CalRawFisherVector(const std::vector<double>& siftFeatures, int dataCount,
        std::vector<double>& rawFisherVec) const
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::CalRawFisherVector(const std::vector<double>& siftFeatures, std::vector<double>& rawFisherVec) const
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::DimReduction(const std::vector<double>& rawFisherVec, const std::vector<int>& imgIds, int fisherDim, 
        std::vector<double>& reducedFisherVec)
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::DimReduction(const std::vector<double>& rawFisherVec, std::vector<double>& reducedFisherVec) const
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::BinaryCompression(const std::vector<double>& reducedFisherVec, int dataCount, int binaryDim)
    {
        return MAGIC_NO_ERROR;
    }

    int FisherVectorFace::BinaryCompression(const std::vector<double>& reducedFisherVec, std::vector<bool>& binaryRep) const
    {
        return MAGIC_NO_ERROR;
    }

}
