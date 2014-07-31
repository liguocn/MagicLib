#include "FaceDetection.h"
#include "../DIP/RealTimeFaceDetection.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <fstream>

namespace MagicApp
{
    FaceDetection::FaceDetection() :
        mDm(DM_Default),
        mpRealTimeDetector(NULL)
    {
    }

    FaceDetection::~FaceDetection()
    {
        if (mpRealTimeDetector != NULL)
        {
            delete mpRealTimeDetector;
            mpRealTimeDetector = NULL;
        }
    }

    int FaceDetection::LearnDetector(const std::string& faceFile, const std::string& nonFaceFile, DetectionMethod dm)
    {
        switch (dm)
        {
        case DM_Default:
            return LearnRealTimeDetector(faceFile, nonFaceFile);
        default:
            return MAGIC_INVALID_INPUT;
        }
    }
     
    int FaceDetection::DetectFace(const cv::Mat& img, std::vector<int>& faces)
    {
        switch (mDm)
        {
        case MagicApp::FaceDetection::DM_Default:
            if (mpRealTimeDetector == NULL)
            {
                return 0;
            }
            else
            {
                return mpRealTimeDetector->Detect(img, faces);
            }
        default:
            return 0;
        }
    }
     
    void FaceDetection::Save(const std::string& fileName) const
    {
        std::ofstream fout(fileName);
        fout << mDm << std::endl;
        std::string detectorFile = fileName;
        std::string::size_type pos = detectorFile.rfind(".");
        switch (mDm)
        {
        case DM_Default:
            detectorFile.replace(pos, 4, ".abfd");
            fout << detectorFile << std::endl;
            if (mpRealTimeDetector != NULL)
            {
                mpRealTimeDetector->Save(detectorFile);
            }
            else
            {
                DebugLog << "Error: AdaBoostDetector is NULL" << std::endl;
            }
            break;
        default:
            break;
        }
        fout.close();
    }
     
    void FaceDetection::Load(const std::string& fileName)
    {
        std::ifstream fin(fileName);
        int dm;
        fin >> dm;
        mDm = DetectionMethod(dm);
        std::string detectorFile;
        fin >> detectorFile;
        fin.close();
        switch (mDm)
        {
        case DM_Default:
            if (mpRealTimeDetector == NULL)
            {
                mpRealTimeDetector = new MagicDIP::RealTimeFaceDetection;
            }
            mpRealTimeDetector->Load(detectorFile);
            break;
        default:
            break;
        }
    }

    int FaceDetection::LearnRealTimeDetector(const std::string& faceFile, const std::string& nonFaceFile)
    {
        std::vector<std::string> faceImgNames, nonFaceImgNames;
        int res = GetImageNames(faceFile, faceImgNames);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }
        res = GetImageNames(nonFaceFile, nonFaceImgNames);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }
        if (mpRealTimeDetector == NULL)
        {
            mpRealTimeDetector = new MagicDIP::RealTimeFaceDetection;
        }
        std::vector<int> layerCount;
        layerCount.push_back(2);
        layerCount.push_back(5);
        //layerCount.push_back(100);
        /*layerCount.reserve(64);
        layerCount.push_back(2);
        layerCount.push_back(10);
        layerCount.push_back(25);
        layerCount.push_back(25);
        for (int layerId = 0; layerId < 4; layerId++)
        {
            layerCount.push_back(50);
        }
        for (int layerId = 0; layerId < 8; layerId++)
        {
            layerCount.push_back(100);
        }
        for (int layerId = 0; layerId < 16; layerId++)
        {
            layerCount.push_back(200);
        }
        for (int layerId = 0; layerId < 32; layerId++)
        {
            layerCount.push_back(400);
        }*/
        return mpRealTimeDetector->Learn(faceImgNames, nonFaceImgNames, layerCount);
    }

    int FaceDetection::GetImageNames(const std::string& imgfile, std::vector<std::string>& imgNames) const
    {
        std::string imgPath = imgfile;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
            if (pos == std::string::npos)
            {
                return MAGIC_INVALID_INPUT;
            }
        }
        imgPath.erase(pos);
        imgPath += "/";
        std::ifstream fin(imgfile);
        int dataSize;
        fin >> dataSize;
        imgNames.clear();
        imgNames.resize(dataSize);
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgNames.at(dataId) = imgPath + imgName;
        }
        fin.close();
        return MAGIC_NO_ERROR;
    }
}
