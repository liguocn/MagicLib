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
    
    std::vector<int> ShapeFaceFeatureDetection::GenerateRandomInitDelta(int sampleCount, int randomSize) const
    {
        std::vector<int> gridList;
        for (int rid = -randomSize; rid <= randomSize; rid++)
        {
            for (int cid = -randomSize; cid <= randomSize; cid++)
            {
                gridList.push_back(rid);
                gridList.push_back(cid);
            }
        }
        int gridCount = gridList.size() / 2;
        std::vector<bool> sampleFlag(gridCount, 0);
        std::vector<int> sampleIndex(sampleCount);
        int startIndex = rand() % gridCount;
        sampleFlag.at(startIndex) = true;
        sampleIndex.at(0) = startIndex;
        std::vector<int> minDist(gridCount, 1.0e10);
        int curIndex = startIndex;
        for (int sid = 1; sid < sampleCount; ++sid)
        {
            int curRow = gridList.at(curIndex * 2);
            int curCol = gridList.at(curIndex * 2 + 1);
            int maxDist = -1;
            int pos = -1;
            for (int vid = 0; vid < gridCount; ++vid)
            {
                if (sampleFlag.at(vid) == 1)
                {
                    continue;
                }
                int vRow = gridList.at(vid * 2);
                int vCol = gridList.at(vid * 2 + 1);
                int dist = (curRow - vRow) * (curRow - vRow) + (curCol - vCol) * (curCol - vCol);
                if (dist < minDist.at(vid))
                {
                    minDist.at(vid) = dist;
                }
                if (minDist.at(vid) > maxDist)
                {
                    maxDist = minDist.at(vid);
                    pos = vid;
                }
            }
            sampleIndex.at(sid) = pos;
            curIndex = pos;
            sampleFlag.at(pos) = 1;
        }
        std::vector<int> dataList(sampleCount * 2);
        for (int sampleId = 0; sampleId < sampleCount; sampleId++)
        {
            dataList.at(sampleId * 2) = gridList.at(sampleIndex.at(sampleId) * 2);
            dataList.at(sampleId * 2 + 1) = gridList.at(sampleIndex.at(sampleId) * 2 + 1);
        }
        return dataList;
    }

    int ShapeFaceFeatureDetection::LearnRegression(const std::string& landFile)
    {
        DebugLog << "ShapeFaceFeatureDetection::LearnRegression" << std::endl;
        CalMeanFace(landFile);
        DebugLog << "CalMeanFace" << std::endl;
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
        landPath += "/";

        int dataPerImgCount = 200; //modify_flag
        int randomSize = 30; //modify_flag
        srand(time(NULL));
        std::vector<int> randomDeltaList = GenerateRandomInitDelta(dataPerImgCount, randomSize);
        for (int randomId = 0; randomId < randomDeltaList.size() / 2; randomId++)
        {
            DebugLog << "random delta: " << randomDeltaList.at(randomId * 2) << " " << randomDeltaList.at(randomId * 2 + 1) << std::endl;
        }
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
        DebugLog << "dataSize: " << dataSize << std::endl;
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
            for (int randomId = 0; randomId < dataPerImgCount; randomId++)
            {
                int randomRow = randomDeltaList.at(randomId * 2);
                int randomCol = randomDeltaList.at(randomId * 2 + 1);
                for (int markId = 0; markId < markCount; markId++)
                {
                    initTheta.push_back( mMeanFace.at(markId * 2) + randomRow );
                    initTheta.push_back( mMeanFace.at(markId * 2 + 1) + randomCol );
                    finalTheta.push_back( interTheta.at(markId * 2) );
                    finalTheta.push_back( interTheta.at(markId * 2 + 1) );
                }
            }
        }
        fin.close();
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::ExplicitShapeRegression;
        }
        DebugLog << "Load Image Files" << std::endl;
        //modify_flag
        return mpRegression->LearnRegression(imgFiles, initTheta, finalTheta, dataPerImgCount, keyPointCount, 100, 100, 5, 30);
    }
    
    int ShapeFaceFeatureDetection::ShapeRegression(const cv::Mat& img, const std::vector<double>& initPos, std::vector<double>& finalPos) const
    {
        if (mpRegression == NULL)
        {
            return MAGIC_NON_INITIAL;
        }
        return mpRegression->ShapeRegression(img, initPos, finalPos);
    }
    
    int ShapeFaceFeatureDetection::ShapeRegressionFromMeanFace(const cv::Mat& img, std::vector<double>& finalPos) const
    {
        if (mpRegression == NULL)
        {
            return MAGIC_NON_INITIAL;
        }
        return mpRegression->ShapeRegression(img, mMeanFace, finalPos);
    }
    
    int ShapeFaceFeatureDetection::TestShapeRegression(const std::string& imgFiles, const std::string& resPath) const
    {
        if (mpRegression == NULL)
        {
            return MAGIC_INVALID_RESULT;
        }
        std::string imgPath = imgFiles;
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
        std::ifstream imgFin(imgFiles);
        int imgCount;
        imgFin >> imgCount;
        DebugLog << "ShapeFaceFeatureDetection::TestShapeRegression: " << imgPath << "  imgCount: " << imgCount << std::endl;
        DebugLog << "Test result path: " << resPath << std::endl;
        const int maxSize = 512;
        char pLine[maxSize];
        imgFin.getline(pLine, maxSize);
        int markSize = 1;
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            imgFin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgName = imgPath + imgName;
            cv::Mat img = cv::imread(imgName);
            int imgH = img.rows;
            int imgW = img.cols;
            std::vector<double> featurePos;
            if (ShapeRegressionFromMeanFace(img, featurePos) == MAGIC_NO_ERROR)
            {
                int featureSize = featurePos.size() / 2;
                for (int featureId = 0; featureId < featureSize; featureId++)
                {
                    int row = featurePos.at(featureId * 2);
                    //row = row > 0 ? (row < imgH ? row : imgH - 1) : 0;
                    int col = featurePos.at(featureId * 2 + 1);
                    //col = col > 0 ? (col < imgW ? col : imgW - 1) : 0;
                    for (int rid = row - markSize; rid <= row + markSize; rid++)
                    {
                        for (int cid = col - markSize; cid <= col + markSize; cid++)
                        {
                            int rid_safe = rid > 0 ? (rid < imgH ? rid : imgH - 1) : 0;
                            int cid_safe = cid > 0 ? (cid < imgW ? cid : imgW - 1) : 0;
                            img.ptr(rid_safe, cid_safe)[0] = 0;
                            img.ptr(rid_safe, cid_safe)[1] = 255;
                            img.ptr(rid_safe, cid_safe)[2] = 255;
                        }
                    }
                }
                std::stringstream ss;
                ss << resPath << imgId << ".jpg";
                std::string featureImgName;
                ss >> featureImgName;
                cv::imwrite(featureImgName, img);
                DebugLog << "test: " << imgId << " imgName: " << featureImgName << std::endl;
            }
            else
            {
                DebugLog << "img" << imgId << " error: no detect" << std::endl;
            }
            img.release();
        }
        imgFin.close();
        
        return MAGIC_NO_ERROR;
    }

    void ShapeFaceFeatureDetection::Save(const std::string& regFileName, const std::string& meanFaceName) const
    {
        mpRegression->Save(regFileName);
        
        //save mean face
        std::ofstream fout(meanFaceName);
        int markCount = mMeanFace.size() / 2;
        fout << markCount << std::endl;
        for (int markId = 0; markId < markCount; markId++)
        {
            fout << mMeanFace.at(markId * 2) << " " << mMeanFace.at(markId * 2 + 1) << std::endl;
        }
        fout.close();
    }

    void ShapeFaceFeatureDetection::Load(const std::string& regFileName, const std::string& meanFaceName)
    {
        DebugLog << "ShapeFaceFeatureDetection::Load: " << std::endl;
        DebugLog << "  " << regFileName << std::endl;
        DebugLog << "  " << meanFaceName << std::endl; 
        if (mpRegression == NULL)
        {
            mpRegression = new MagicDIP::ExplicitShapeRegression;
        }
        mpRegression->Load(regFileName);
        
        //load mean face
        mMeanFace.clear();
        std::ifstream fin(meanFaceName);
        int markCount;
        fin >> markCount;
        mMeanFace.reserve(markCount * 2);
        for (int markId = 0; markId < markCount; markId++)
        {
            double row, col;
            fin >> row >> col;
            mMeanFace.push_back(row);
            mMeanFace.push_back(col);
        }
        fin.close();
        //Move mean face to center, test image size is 128 * 128
        double minX = 1.0e10;
        double maxX = -1.0e10;
        double minY = 1.0e10;
        double maxY = -1.0e10;
        for (int markId = 0; markId < markCount; markId++)
        {
            if (mMeanFace.at(markId * 2) < minY)
            {
                minY = mMeanFace.at(markId * 2);
            }
            if (mMeanFace.at(markId * 2) > maxY)
            {
                maxY = mMeanFace.at(markId * 2);
            }
            if (mMeanFace.at(markId * 2 + 1) < minX)
            {
                minX = mMeanFace.at(markId * 2 + 1);
            }
            if (mMeanFace.at(markId * 2 + 1) > maxX)
            {
                maxX = mMeanFace.at(markId * 2 + 1);
            }
        }
        double cenX = (minX + maxX) / 2.0;
        double cenY = (minY + maxY) / 2.0;
        double deltaX = 64 - cenX;
        double deltaY = 64 - cenY;
        for (int markId = 0; markId < markCount; markId++)
        {
            mMeanFace.at(markId * 2) = mMeanFace.at(markId * 2) + deltaY;
            mMeanFace.at(markId * 2 + 1) = mMeanFace.at(markId * 2 + 1) + deltaX;
        }
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
        landPath.erase(pos);
        landPath += "/";
        DebugLog << "landPath: " << landPath << std::endl;
        std::ifstream fin(landFile);
        int dataSize;
        fin >> dataSize;
        mMeanFace.clear();
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
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

}
