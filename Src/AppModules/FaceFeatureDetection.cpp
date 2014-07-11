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
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            std::string featureName;
            fin >> featureName;
            featureName = landPath + featureName;
            std::string imgName = featureName;
            std::string::size_type pos = imgName.rfind(".");
            imgName.replace(pos, 5, ".jpg");
            imgFiles.at(dataId) = imgName;
            //imgFiles.at(dataId) = testImgName;
            if (dataId == 0)
            {
                cv::Mat img = cv::imread(imgName);
                imgH = img.rows;
                img.release();
            }
            std::ifstream landFin(featureName);
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
            //DebugLog << "pos: " << thetaX << " " << thetaY << std::endl;
            avgThetaX += thetaX;
            avgThetaY += thetaY;
            landFin.close();
        }
        fin.close();
        avgThetaX /= dataSize;
        avgThetaY /= dataSize;
        //DebugLog << "avg: " << avgThetaX << " " << avgThetaY << std::endl;
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
        return mpRegression->LearnRegression(imgFiles, initialTheta, targetTheta, 2, 1000, 10, 1000);
    }
        
    int CascadedFaceFeatureDetection::PoseRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const
    {
        return MAGIC_NO_ERROR;
    }
}
