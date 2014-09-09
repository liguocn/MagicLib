#include "FaceFeatureDetection.h"
#include "../DIP/CascadedPoseRegression.h"
#include "../DIP/ExplicitShapeRegression.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <stdio.h>
#include <time.h>
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
        dataSize = 5448;
        std::vector<std::string> imgFiles(dataSize);
        int dataPerImgCount = 16;
        std::vector<double> targetTheta;
        targetTheta.reserve(dataSize * 2 * dataPerImgCount);
        std::vector<double> initialTheta;
        initialTheta.reserve(dataSize * 2 * dataPerImgCount);
        int imgH, imgW;
        //double avgThetaX = 0;
        //double avgThetaY = 0;
        /*int wStart = 150;
        int hStart = 100;
        int wSize = 320;
        int hSize = 300;*/
        srand(time(NULL));
        int randomSize = 50;
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
                imgW = img.cols;
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
            landFin.close();
            //generate initial data
            int initialNum = 0;
            while (initialNum < dataPerImgCount)
            {
                int xRand = rand() % (randomSize * 2) - randomSize;
                int yRand = rand() % (randomSize * 2) - randomSize;
                if (xRand == 0 && yRand == 0)
                {
                    continue;
                }
                double initialX = thetaX + xRand;
                initialX = initialX < 0 ? 0 : (initialX > imgW - 1 ? imgW - 1 : initialX);
                double initialY = thetaY + yRand;
                initialY = initialY < 0 ? 0 : (initialY > imgH - 1 ? imgH - 1 : initialY);
                initialTheta.push_back(initialY);
                initialTheta.push_back(initialX);
                targetTheta.push_back(thetaY);
                targetTheta.push_back(thetaX);
                initialNum++;
            }
            //
            //avgThetaX += thetaX;
            //avgThetaY += thetaY;
            
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
        //avgThetaX /= dataSize;
        //avgThetaY /= dataSize;
        //DebugLog << "avg: " << avgThetaX << " " << avgThetaY << std::endl;
        /*std::vector<double> initialTheta;
        initialTheta.reserve(dataSize * 2);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            initialTheta.push_back(avgThetaY);
            initialTheta.push_back(avgThetaX);
        }*/
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::SimpleCascadedPoseRegression;
        }
        DebugLog << "LearnRegression" << std::endl;
        return mpRegression->LearnRegression(imgFiles, initialTheta, targetTheta, dataPerImgCount, 2, 10000, 5, 128);
    }
        
    int CascadedFaceFeatureDetection::PoseRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const
    {
        if (mpRegression == NULL)
        {
            return MAGIC_NON_INITIAL;
        }
        return mpRegression->PoseRegression(img, initPos, finalPos);
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

    ShapeFaceFeatureDetection::ShapeFaceFeatureDetection() :
        mpRegression(NULL),
        mMeanFace()
    {
    }

    ShapeFaceFeatureDetection::~ShapeFaceFeatureDetection()
    {
        if (mpRegression != NULL)
        {
            delete mpRegression;
            mpRegression = NULL;
        }
    }

    int ShapeFaceFeatureDetection::LearnRegression(const std::string& landFile)
    {
        CalMeanFace(landFile);
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
        //landPath.erase(pos);

        int dataPerImgCount = 20;
        srand(time(NULL));
        int randomSize = 80;
        std::vector<double> initTheta;
        std::vector<double> finalTheta;
        std::vector<double> interTheta;
        std::ifstream fin(landFile);
        int dataSize;
        fin >> dataSize;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        std::vector<std::string> imgFiles(dataSize);
        int keyPointCount;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string featureName(pLine);
            featureName = landPath + featureName;
            std::string imgName = featureName;
            std::string::size_type pos = imgName.rfind(".");
            imgName.replace(pos, 5, ".jpg");         
            imgFiles.at(dataId) = imgName;
            std::ifstream landFin(featureName);
            int markCount;
            landFin >> markCount;
            if (dataId == 0)
            {
                keyPointCount = markCount;
                initTheta.reserve(2 * markCount * dataSize * dataPerImgCount);
                finalTheta.reserve(2 * markCount * dataSize * dataPerImgCount);
                interTheta = std::vector<double>(markCount * 2, 0);
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                double row, col;
                landFin >> col >> row;
                interTheta.at(markId * 2) = row;
                interTheta.at(markId * 2 + 1) = col;
            }
            landFin.close();
            int initialNum = 0;
            while (initialNum < dataPerImgCount)
            {
                int xRand = rand() % (randomSize * 2) - randomSize;
                int yRand = rand() % (randomSize * 2) - randomSize;
                if (xRand == 0 && yRand == 0)
                {
                    continue;
                }
                for (int markId = 0; markId < markCount; markId++)
                {
                    initTheta.push_back( mMeanFace.at(markId * 2) + yRand );
                    initTheta.push_back( mMeanFace.at(markId * 2 + 1) + xRand );
                    finalTheta.push_back( interTheta.at(markId * 2) );
                    finalTheta.push_back( interTheta.at(markId * 2 + 1) );
                }
                initialNum++;
            }
        }
        fin.close();
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::ExplicitShapeRegression;
        }
        DebugLog << "Load Image Files" << std::endl;
        return mpRegression->LearnRegression(imgFiles, initTheta, finalTheta, dataPerImgCount, keyPointCount, 100, 100, 5, 2);
    }
    
    int ShapeFaceFeatureDetection::ShapeRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const
    {
        if (mpRegression == NULL)
        {
            return MAGIC_NON_INITIAL;
        }
        return mpRegression->ShapeRegression(img, initPos, finalPos);
    }

    void ShapeFaceFeatureDetection::Save(const std::string& fileName) const
    {
        mpRegression->Save(fileName);
    }

    void ShapeFaceFeatureDetection::Load(const std::string& fileName)
    {
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::ExplicitShapeRegression;
        }
        mpRegression->Load(fileName);
    }

    std::vector<double> ShapeFaceFeatureDetection::GetMeanFace(void) const
    {
        return mMeanFace;
    }

    int ShapeFaceFeatureDetection::CalMeanFace(const std::string& landFile)
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
        //landPath.erase(pos);
        std::ifstream fin(landFile);
        int dataSize;
        fin >> dataSize;
        mMeanFace.clear();
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        int imgH;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string featureName(pLine);
            featureName = landPath + featureName;       
            std::ifstream landFin(featureName);
            int markCount;
            landFin >> markCount;
            if (dataId == 0)
            {
                mMeanFace = std::vector<double>(markCount * 2, 0.0);
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                double row, col;
                landFin >> col >> row;
                mMeanFace.at(markId * 2) += row;
                mMeanFace.at(markId * 2 + 1) += col;
            }
            landFin.close();
        }
        for (std::vector<double>::iterator itr = mMeanFace.begin(); itr != mMeanFace.end(); itr++)
        {
            (*itr) /= dataSize;
        }
        fin.close();

        return MAGIC_NO_ERROR;
    }

    bool ShapeFaceFeatureDetection::IsMeanFaceExist(void) const
    {
        return (mMeanFace.size() > 0);
    }

}
