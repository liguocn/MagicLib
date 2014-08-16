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
     
    void FaceDetection::Save(const std::string& fileName, const std::string& detectFileName) const
    {
        std::ofstream fout(fileName);
        fout << mDm << std::endl;
        fout << detectFileName << std::endl;
        std::string filePath = fileName;
        std::string::size_type pos = filePath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = filePath.rfind("\\");
        }
        filePath.erase(pos);
        filePath += "/";
        std::string detectFileFullName = filePath + detectFileName;
        switch (mDm)
        {
        case DM_Default:
            if (mpRealTimeDetector != NULL)
            {
                mpRealTimeDetector->Save(detectFileFullName);
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
        //
        std::string filePath = fileName;
        std::string::size_type pos = filePath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = filePath.rfind("\\");
        }
        filePath.erase(pos);
        filePath += "/";
        detectorFile = filePath + detectorFile;
        //
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

    void FaceDetection::SaveFeatureAsImage(const std::string& filePath) const
    {
        mpRealTimeDetector->SaveFeatureAsImage(filePath);
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
        /*std::vector<int> layerCount(4, 5);
        for (int layerId = 0; layerId < 1000; layerId++)
        {
            layerCount.push_back(10);
        }*/
        int iterCount = 100;
        int levelCount = 8;
        int maxLevelCount = 512;
        int duplicateCount = 2;
        int increaseNum = 2;
        int maxDuplicateCount = 32;
        std::vector<int> layerCount;
        for (int iterId = 0; iterId < iterCount; iterId++)
        {
            for (int duplicateId = 0; duplicateId < duplicateCount; duplicateId++)
            {
                layerCount.push_back(levelCount);
            }
            levelCount *= 2;
            levelCount = (levelCount > maxLevelCount ? maxLevelCount : levelCount);
            if (iterId % increaseNum == 0)
            {
                duplicateCount *= 2;
                duplicateCount = (duplicateCount > maxDuplicateCount ? maxDuplicateCount : duplicateCount);
            }
        }
        /*std::vector<int> layerCount;
        layerCount.push_back(10);
        layerCount.push_back(10);
        layerCount.push_back(10);
        layerCount.push_back(10);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(20);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(40);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(80);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(160);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(240);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(320);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);
        layerCount.push_back(400);*/
        
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
