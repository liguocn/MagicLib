#include "FaceRecognition.h"
#include "../Math/HomoMatrix3.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicApp
{
    FaceRecognition::FaceRecognition()
    {
        
    }
    
    FaceRecognition::~FaceRecognition()
    {
        
    }
    
    int FaceRecognition::AlignFace(const std::string& faceImgFiles, const std::string& alignedFilePath)
    {
        DebugLog << "FaceRecognition::AlignFace..." << std::endl;
        std::string imgPath = faceImgFiles;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        imgPath += "/";
        
        //read image and mark data
        std::ifstream fin(faceImgFiles);
        int dataSize;
        fin >> dataSize;
        std::vector<std::string> imgFileNames(dataSize);
        std::vector<double> markList;
        int markCount = 0;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgName = imgPath + imgName;
            imgFileNames.at(dataId) = imgName;
            
            std::string markFileName = imgName;
            std::string::size_type pos = markFileName.rfind(".");
            markFileName.replace(pos, 4, ".mark");
            std::ifstream markFin(markFileName);
            markFin >> markCount;
            if (dataId == 0)
            {
                markList.reserve(markCount * dataSize * 2);
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                double row, col;
                markFin >> col >> row;
                markList.push_back(col);
                markList.push_back(row);
            }
            markFin.close();
        }
        fin.close();
        
        //Calculate mean marks
        DebugLog << "Calculate mean marks" << std::endl;
        int iterCount = 2;
        std::vector<cv::Point2f> cvMeanMarks(markCount);
        std::vector<cv::Point2f> cvSumMarks(markCount);
        std::vector<cv::Point2f> cvCurMarks(markCount);
        for (int iterIndex = 0; iterIndex < iterCount; iterIndex++)
        {
            if (iterIndex == 0)
            {
                for (int markId = 0; markId < markCount; markId++)
                {
                    cvMeanMarks.at(markId).x = markList.at(markId * 2);
                    cvMeanMarks.at(markId).y = markList.at(markId * 2 + 1);
                }
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                cvSumMarks.at(markId).x = 0;
                cvSumMarks.at(markId).y = 0;
            }
            int validSumNum = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                int markBaseIndex = dataId * markCount * 2;
                for (int markId = 0; markId < markCount; markId++)
                {
                    cvCurMarks.at(markId).x = markList.at(markBaseIndex + markId * 2);
                    cvCurMarks.at(markId).y = markList.at(markBaseIndex + markId * 2 + 1);
                }
                cv::Mat transMat = cv::estimateRigidTransform(cvCurMarks, cvMeanMarks, false);
                if (transMat.data == NULL)
                {
                    DebugLog << "    transMat.data == NULL: " << dataId << " " << iterIndex << std::endl;
                    continue;
                }
                MagicMath::HomoMatrix3 homoMat;
                homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
                homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
                homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
                homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
                homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
                homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
                for (int markId = 0; markId < markCount; markId++)
                {
                    double xRes, yRes;
                    homoMat.TransformPoint(cvCurMarks.at(markId).x, cvCurMarks.at(markId).y, xRes, yRes);
                    cvSumMarks.at(markId).x += xRes;
                    cvSumMarks.at(markId).y += yRes;
                }
                validSumNum++;
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                cvMeanMarks.at(markId).x = cvSumMarks.at(markId).x / validSumNum;
                cvMeanMarks.at(markId).y = cvSumMarks.at(markId).y / validSumNum;
            }
        }
        
        DebugLog << "Align each face to mean face and extract it" << std::endl;
        //Align each face to mean face and extract it
        int validAlignedId = 0;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int markBaseIndex = dataId * markCount * 2;
            for (int markId = 0; markId < markCount; markId++)
            {
                cvCurMarks.at(markId).x = markList.at(markBaseIndex + markId * 2);
                cvCurMarks.at(markId).y = markList.at(markBaseIndex + markId * 2 + 1);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurMarks, cvMeanMarks, false);
            if (transMat.data == NULL)
            {
                DebugLog << "miss face: " << dataId << " : " << imgFileNames.at(dataId) << std::endl;
                continue;
            }
            cv::Mat img = cv::imread(imgFileNames.at(dataId));
            cv::Mat alignedImg(img.rows, img.cols, img.type());
            cv::warpAffine(img, alignedImg, transMat, alignedImg.size());
            img.release();
            std::stringstream ss;
            ss << alignedFilePath << "_" << validAlignedId << ".jpg";
            std::string alignedImgName;
            ss >> alignedImgName;
            cv::imwrite(alignedImgName, alignedImg);
            alignedImg.release();
            
            MagicMath::HomoMatrix3 homoMat;
            homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
            ss.clear();
            ss << alignedFilePath << "_" << validAlignedId << ".mark";
            std::string alignedMarkName;
            ss >> alignedMarkName;
            std::ofstream markFout(alignedMarkName);
            markFout << markCount << std::endl;
            for (int markId = 0; markId < markCount; markId++)
            {
                double xRes, yRes;
                homoMat.TransformPoint(cvCurMarks.at(markId).x, cvCurMarks.at(markId).y, xRes, yRes);
                cvSumMarks.at(markId).x += xRes;
                cvSumMarks.at(markId).y += yRes;
                markFout << xRes << " " << yRes << std::endl;
            }
            markFout.close();
            
            validAlignedId++;
        }
        DebugLog << "Finished" << std::endl;
        
        return MAGIC_NO_ERROR;
    }
}
