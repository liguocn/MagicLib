#include "FaceFeatureDetection.h"
#include "../DIP/CascadedPoseRegression.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <fstream>

namespace MagicApp
{
    CascadedFaceFeatureDetection::CascadedFaceFeatureDetection() :
        mpRegression(NULL)
    {
    }

    CascadedFaceFeatureDetection::~CascadedFaceFeatureDetection()
    {
        if (mpRegression != NULL)
        {
            delete mpRegression;
            mpRegression = NULL;
        }
    }

    int CascadedFaceFeatureDetection::LearnRegression(const std::string& landFile)
    {
        std::string landPath = landFile;
        std::string::size_type pos = landPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = landPath.rfind("\\");
            if (pos == std::string::npos)
            {
                return MAGIC_INVALID_INPUT;
            }
        }
        landPath.erase(pos);
        std::ifstream fin(landFile);
        int dataSize;
        fin >> dataSize;
        //dataSize = 100;
        std::vector<std::string> imgFiles(dataSize);
        std::vector<double> targetTheta;
        targetTheta.reserve(dataSize * 2);
        int imgH;
        double avgThetaX = 0;
        double avgThetaY = 0;
        //std::string testImgName = "D:\\fun\\magic3d\\data\\face\\FaceWarehouse\\Tester_133\\TrainingPose\\pose_10.jpg";
        int wStart = 150;
        int hStart = 100;
        int wSize = 320;
        int hSize = 300;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            std::string featureName;
            fin >> featureName;
            featureName = landPath + featureName;
            std::string imgName = featureName;
            std::string::size_type pos = imgName.rfind(".");
            imgName.replace(pos, 5, ".jpg");
            //Use gray image
            std::string grayImgName = imgName;
            pos = grayImgName.rfind(".");
            grayImgName.replace(pos, 4, "_gray.jpg");
            /*cv::Mat img = cv::imread(imgName);
            cv::Mat grayImg;
            cv::cvtColor(img, grayImg, CV_BGR2GRAY);
            cv::imwrite(grayImgName, grayImg);
            img.release();*/
            /*cv::Mat smallImg(hSize, wSize, CV_8UC1);
            for (int wid = 0; wid < wSize; wid++)
            {
                for (int hid = 0; hid < hSize; hid++)
                {
                    unsigned char* smallPixel = smallImg.ptr(hid, wid);
                    unsigned char* grayPixel = grayImg.ptr(hid + hStart, wid + wStart);
                    smallPixel[0] = grayPixel[0];
                }
            }
            cv::imwrite(grayImgName, smallImg);
            grayImg.release();
            smallImg.release();*/
            //
            imgFiles.at(dataId) = grayImgName;// imgName;
            //imgFiles.at(dataId) = testImgName;
            if (dataId == 0)
            {
                cv::Mat img = cv::imread(grayImgName);
                imgH = img.rows;
                img.release();
            }
            std::ifstream landFin(featureName);
            /*int markSize;
            landFin >> markSize;
            std::vector<double> markPosList;
            for (int markId = 0; markId < markSize; markId++)
            {
                double x, y;
                landFin >> x >> y;
                markPosList.push_back(x - wStart);
                markPosList.push_back(hSize - (imgH - y - hStart));
            }*/
            double dTemp;
            landFin >> dTemp;
            for (int lineId = 1; lineId <= 65 ; lineId++)
            {
                landFin >> dTemp >> dTemp;
            }
            double thetaX, thetaY;
            landFin >> thetaX >> thetaY;
            thetaY = imgH - thetaY;
            targetTheta.push_back(thetaY);
            targetTheta.push_back(thetaX);
            avgThetaX += thetaX;
            avgThetaY += thetaY;
            landFin.close();
            /*std::ofstream landFout(featureName);
            landFout << markSize << std::endl;
            for (int markId = 0; markId < markSize; markId++)
            {
                landFout << markPosList.at(markId * 2) << " " << markPosList.at(markId * 2 + 1) << std::endl;
            }
            landFout.close();*/
        }
        fin.close();
        //DebugLog << "boundary: " << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
        avgThetaX /= dataSize;
        avgThetaY /= dataSize;
        DebugLog << "avg: " << avgThetaX << " " << avgThetaY << std::endl;
        std::vector<double> initialTheta;
        initialTheta.reserve(dataSize * 2);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            initialTheta.push_back(avgThetaY);
            initialTheta.push_back(avgThetaX);
        }
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::SimpleCascadedPoseRegression;
        }
        return mpRegression->LearnRegression(imgFiles, initialTheta, targetTheta, 2, 2000, 10, 1000);
    }
        
    int CascadedFaceFeatureDetection::PoseRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const
    {
        return MAGIC_NO_ERROR;
    }

    void CascadedFaceFeatureDetection::Save(const std::string& fileName) const
    {
        mpRegression->Save(fileName);
    }
     
    void CascadedFaceFeatureDetection::Load(const std::string& fileName)
    {
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::SimpleCascadedPoseRegression;
        }
        mpRegression->Load(fileName);
    }
}
