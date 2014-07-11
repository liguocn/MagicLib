#include "CascadedPoseRegression.h"
#include "../MachineLearning/RandomMethod.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <stdio.h>
#include <time.h>

namespace MagicDIP
{
    CascadedPoseRegression::CascadedPoseRegression() :
        mRandomFerns()
    {
    }

    CascadedPoseRegression::~CascadedPoseRegression()
    {
        Reset();
    }

    int CascadedPoseRegression::LearnRegression(const std::vector<std::string>& imgFiles, const std::vector<double>& initTheta, 
            const std::vector<double>& finalTheta, int thetaDim, int fernCount, int fernSize, int featureSize)
    {
        if (thetaDim < 1 || imgFiles.size() == 0)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (imgFiles.size() != finalTheta.size() / thetaDim)
        {
            return MAGIC_INVALID_INPUT;
        }
        Reset();

        mRandomFerns.reserve(fernCount);
        int dataCount = initTheta.size() / thetaDim;
        std::vector<bool> dataX(featureSize * dataCount);
        std::vector<double> theta(thetaDim); //one theta
        std::vector<double> curTheta = initTheta;
        std::vector<double> deltaTheta(curTheta.size());
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
            DebugLog << "fernId: " << fernId << std::endl; 
            //Calculate deltaTheta
            double avgDelta = 0;
            for (int thetaId = 0; thetaId < curTheta.size(); thetaId++)
            {
                deltaTheta.at(thetaId) = finalTheta.at(thetaId) - curTheta.at(thetaId);
                avgDelta += fabs(deltaTheta.at(thetaId));
            }
            avgDelta /= curTheta.size();
            DebugLog << "  AvgDelta: " << avgDelta << std::endl;
            //Generate Feature Ids and dataX
            FeaturePatternGeneration(imgFiles, curTheta, deltaTheta, dataCount, featureSize, dataX);
            DebugLog << "  FeaturePatternGeneration done" << std::endl;
            //Learn random fern
            MagicML::RandomFern* pFern = new MagicML::RandomFern;
            pFern->Learn(dataX, featureSize, deltaTheta, thetaDim, fernSize);
            DebugLog << "  Fern Learn done" << std::endl;
            mRandomFerns.push_back(pFern);
            //Update Valid Features
            std::vector<int> validFeatureIds = pFern->GetFeatureIds();
            UpdateValidFeaturePosPair(validFeatureIds);
            DebugLog << "  UpdateValidFeaturePosPair done" << std::endl;
            //Update curTheta
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                //Load image
                //cv::Mat img = cv::imread(imgFiles.at(dataId));
                cv::Mat imgOrigin = cv::imread(imgFiles.at(dataId));
                //DebugLog << "    imgOrigin susscess : " << imgFiles.at(dataId) << std::endl;
                if (imgOrigin.data == NULL)
                {
                    DebugLog << "    error: imgOrigin.data == NULL" << std::endl;
                }
                cv::Mat img;
                cv::cvtColor(imgOrigin, img, CV_BGR2GRAY);
                //DebugLog << "    img convert color" << std::endl;
                if (img.data == NULL)
                {
                    DebugLog << "    error: img.data == NULL" << std::endl;
                }
                imgOrigin.release();
                //DebugLog << "    imgOrigin.release" << std::endl;
                //Generate feature from theta
                int baseIndex = dataId * thetaDim;
                for (int thetaId = 0; thetaId < thetaDim; thetaId++)
                {
                    theta.at(thetaId) = curTheta.at(baseIndex + thetaId);
                }
                std::vector<bool> features;
                ValidFeatureGeneration(img, theta, fernId, features);
                //DebugLog << "    ValidFeatureGeneration" << std::endl;
                img.release();
                //DebugLog << "    img release" << std::endl;
                //Predict delta theta
                std::vector<double> deltaTheta = pFern->PredictWithValidFeature(features);
                //DebugLog << "    Fern Prediction " << deltaTheta.at(0) << " " << deltaTheta.at(1) << std::endl;
                //Update curTheta
                baseIndex = dataId * thetaDim;
                for (int thetaId = 0; thetaId < thetaDim; thetaId++)
                {
                    curTheta.at(baseIndex + thetaId) += deltaTheta.at(thetaId);
                }
            }
        }

        return MAGIC_NO_ERROR;
    }
        
    int CascadedPoseRegression::PoseRegression(const cv::Mat& img, const std::vector<double>& initTheta, std::vector<double>& finalTheta) const
    {
        if (mRandomFerns.size() == 0)
        {
            return MAGIC_NON_INITIAL;
        }
        finalTheta = initTheta;
        int fernCount = mRandomFerns.size();
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
            std::vector<bool> features;
            ValidFeatureGeneration(img, finalTheta, fernId, features);
            std::vector<double> deltaTheta = mRandomFerns.at(fernId)->PredictWithValidFeature(features);
            for (int thetaId = 0; thetaId < finalTheta.size(); thetaId++)
            {
                finalTheta.at(thetaId) += deltaTheta.at(thetaId);
            }
        }

        return MAGIC_NO_ERROR;
    }

    void CascadedPoseRegression::Reset(void)
    {
        for (std::vector<MagicML::RandomFern* >::iterator itr = mRandomFerns.begin(); itr != mRandomFerns.end(); itr++)
        {
            if (*itr != NULL)
            {
                delete (*itr);
                *itr = NULL;
            }
        }
        mRandomFerns.clear();
    }

    SimpleCascadedPoseRegression::SimpleCascadedPoseRegression() : 
        mValidFeatureSize(0),
        mImgPatchSize(31),
        mFeaturePosPairs()
    {

    }

    SimpleCascadedPoseRegression::SimpleCascadedPoseRegression(int patchSize) :
        mValidFeatureSize(0),
        mImgPatchSize(patchSize),
        mValidFeaturePosPairs(),
        mFeaturePosPairs()
    {
    }

    SimpleCascadedPoseRegression::~SimpleCascadedPoseRegression()
    {
    }

    void SimpleCascadedPoseRegression::FeaturePatternGeneration(const std::vector<std::string>& imgFiles, 
        const std::vector<double>& theta, const std::vector<double>& dataY, int dataCount, int featureSize, 
        std::vector<bool>& features)
    {
        //Random feature postions
        srand(time(NULL));
        int maxIndex = mImgPatchSize * mImgPatchSize;
        mFeaturePosPairs.clear();
        mFeaturePosPairs.reserve(featureSize * 2);
        for (int featureId = 0; featureId < featureSize; featureId++)
        {
            int indexX = rand() % maxIndex;
            int indexY = rand() % maxIndex;
            while (indexX == indexY)
            {
                indexY = rand() % maxIndex;
            }
            mFeaturePosPairs.push_back(indexX);
            mFeaturePosPairs.push_back(indexY);
        }
        
        //Calculate features
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            cv::Mat imgOrigin = cv::imread(imgFiles.at(dataId));
            cv::Mat img;
            cv::cvtColor(imgOrigin, img, CV_BGR2GRAY);
            imgOrigin.release();

            int featureBase = featureSize * dataId;
            for (int featureId = 0; featureId < featureSize; featureId++)
            {
                int imgRowX, imgColX;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2), imgRowX, imgColX);
                imgRowX += theta.at(0);
                imgColX += theta.at(1);
                imgRowX = imgRowX < 0 ? 0 : (imgRowX > img.rows - 1 ? img.rows - 1 : imgRowX);
                imgColX = imgColX < 0 ? 0 : (imgColX > img.cols - 1 ? img.cols - 1 : imgColX);
                int imgRowY, imgColY;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2 + 1), imgRowY, imgColY);
                imgRowY += theta.at(0);
                imgColY += theta.at(1);
                imgRowY = imgRowY < 0 ? 0 : (imgRowY > img.rows - 1 ? img.rows - 1 : imgRowY);
                imgColY = imgColY < 0 ? 0 : (imgColY > img.cols - 1 ? img.cols - 1 : imgColY);
                if (img.ptr(imgRowX, imgColX)[0] > img.ptr(imgRowY, imgColY)[0])
                {
                    features.at(featureBase + featureId) = 1;
                }
                else
                {
                    features.at(featureBase + featureId) = 0;
                }
            }
            img.release();
        }
    }

    void SimpleCascadedPoseRegression::UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds)
    {
        if (mFeaturePosPairs.size() < validFeatureIds.size() * 2)
        {
            DebugLog << "UpdateValidFeaturePosPair error: invalid validFeatureIds" << std::endl;
        }
        mValidFeatureSize = validFeatureIds.size();
        mValidFeaturePosPairs.reserve(mValidFeaturePosPairs.size() + validFeatureIds.size() * 2);
        for (std::vector<int>::const_iterator itr = validFeatureIds.begin(); itr != validFeatureIds.end(); itr++)
        {
            mValidFeaturePosPairs.push_back( mFeaturePosPairs.at(*itr * 2) );
            mValidFeaturePosPairs.push_back( mFeaturePosPairs.at(*itr * 2 + 1) );
        }
    }
     
    void SimpleCascadedPoseRegression::ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, 
        std::vector<bool>& features) const
    {
        //DebugLog << "    Enter ValidFeatureGeneration : " << mValidFeatureSize << std::endl;
        features.clear();
        features.resize(mValidFeatureSize);
        int baseIndex = fernId * 2 * mValidFeatureSize;
        for (int featureId = 0; featureId < mValidFeatureSize; featureId++)
        {
            //DebugLog << "    " << featureId << " ";
            int imgRowX, imgColX;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId), imgRowX, imgColX);
            imgRowX += theta.at(0);
            imgColX += theta.at(1);
            imgRowX = imgRowX < 0 ? 0 : (imgRowX > img.rows - 1 ? img.rows - 1 : imgRowX);
            imgColX = imgColX < 0 ? 0 : (imgColX > img.cols - 1 ? img.cols - 1 : imgColX);
            int imgRowY, imgColY;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId + 1), imgRowY, imgColY);
            imgRowY += theta.at(0);
            imgColY += theta.at(1);
            imgRowY = imgRowY < 0 ? 0 : (imgRowY > img.rows - 1 ? img.rows - 1 : imgRowY);
            imgColY = imgColY < 0 ? 0 : (imgColY > img.cols - 1 ? img.cols - 1 : imgColY);
            //DebugLog << "imgWidth: " << img.cols << " imgHeight: " << img.rows << " " << imgColX << " " << imgRowX << " " 
            //    << imgColY << " " << imgRowY;
            if (img.ptr(imgRowX, imgColX)[0] > img.ptr(imgRowY, imgColY)[0])
            {
                features.at(featureId) = 1;
            }
            else
            {
                features.at(featureId) = 0;
            }
            //DebugLog << " done" << std::endl;
        }
    }

    void SimpleCascadedPoseRegression::Reset(void)
    {
        CascadedPoseRegression::Reset();
        mValidFeaturePosPairs.clear();
        mFeaturePosPairs.clear();
        mValidFeatureSize = 0;
    }

    void SimpleCascadedPoseRegression::ScaleToPatchCoord(int pos, int& imgRow, int& imgCol) const
    {
        imgRow = pos / mImgPatchSize;
        imgCol = pos % mImgPatchSize;
        int halfPatchSize = mImgPatchSize / 2;
        imgRow = imgRow - halfPatchSize;
        imgCol = imgCol - halfPatchSize;
    }
}
