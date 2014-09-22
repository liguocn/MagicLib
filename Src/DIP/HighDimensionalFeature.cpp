#include "HighDimensionalFeature.h"
#include "../Math/SparseMatrix.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    HighDimensionalFeature::HighDimensionalFeature() :
        mMarkCount(0),
        mMultiScaleCount(0),
        mpProjectMat(NULL)
    {
    }
    
    HighDimensionalFeature::~HighDimensionalFeature()
    {
        Reset();
    }
    
    int HighDimensionalFeature::Learn(const std::vector<std::string>& imgFiles, const std::vector<int>& marksList,
                                      int markCountPerImg, int multiScaleCount, int targetDim)
    {
        return MAGIC_NO_ERROR;
    }
    
    int HighDimensionalFeature::GetHighDimensionalFeature(const cv::Mat& img, const std::vector<int>& marksList,
                                                          std::vector<int>& feature) const
    {
        return MAGIC_NO_ERROR;
    }
    
    int HighDimensionalFeature::GetCompressedFeature(const cv::Mat& img, const std::vector<int>& marksList,
                                                     std::vector<int>& feature) const
    {
        return MAGIC_NO_ERROR;
    }
    
    void HighDimensionalFeature::Save(const std::string& fileName) const
    {
        
    }
    
    void HighDimensionalFeature::Load(const std::string& fileName)
    {
        
    }
    
    void HighDimensionalFeature::Reset(void)
    {
        mMarkCount = 0;
        mMultiScaleCount = 0;
        if (mpProjectMat != NULL)
        {
            delete mpProjectMat;
            mpProjectMat = NULL;
        }
    }
}
