#include "FisherVectorFace.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Tool/Profiler.h"

namespace MagicDIP
{
    FisherVectorFace::FisherVectorFace()
    {
        
    }
    
    FisherVectorFace::~FisherVectorFace()
    {
        
    }
    
    int FisherVectorFace::LearnRepresentation(const std::vector<std::string>& imgFiles)
    {
        return MAGIC_NO_ERROR;
    }
    
    std::vector<double> FisherVectorFace::GetRepresentation(const cv::Mat& img) const
    {
        std::vector<double> fisherRep;
        
        return fisherRep;
    }
    
    void FisherVectorFace::Save(const std::string& fileName) const
    {
        
    }
    
    void FisherVectorFace::Load(const std::string& fileName)
    {
        
    }
}