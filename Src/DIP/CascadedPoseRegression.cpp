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
            for (int thetaId = 0; thetaId < curTheta.size(); thetaId++)
            {
                deltaTheta.at(thetaId) = finalTheta.at(thetaId) - curTheta.at(thetaId);
            }

            //Generate Feature Ids and dataX
            FeaturePatternGeneration(imgFiles, curTheta, deltaTheta, dataCount, featureSize, dataX);
            DebugLog << "  FeaturePatternGeneration done" << std::endl;
            //Learn random fern
            MagicML::RandomFern* pFern = new MagicML::RandomFern;
            pFern->Learn(dataX, featureSize, deltaTheta, thetaDim, fernSize);
            DebugLog << "  Fern Learn done" << std::endl;
            mRandomFerns.push_back(pFern);
            //Update curTheta
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                //Load image
                cv::Mat img = cv::imread(imgFiles.at(dataId));
                //Generate feature from theta
                int baseIndex = dataId * thetaDim;
                for (int thetaId = 0; thetaId < thetaDim; thetaId++)
                {
                    theta.at(thetaId) = curTheta.at(baseIndex + thetaId);
                }
                std::vector<bool> features;
                ValidFeatureGeneration(img, theta, fernId, features);
                //Predict delta theta
                std::vector<double> deltaTheta = pFern->PredictWithValidFeature(features);
                //Update curTheta
                /*baseIndex = dataId * featureSize;
                for (int featureId = 0; featureId < featureSize; featureId++)
                {
                    curTheta.at(baseIndex + featureId) += deltaTheta.at(featureId);
                }*/
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
        mFeatureSize(0),
        mImgPatchSize(31),
        mFeaturePosPairs()
    {

    }

    SimpleCascadedPoseRegression::SimpleCascadedPoseRegression(int patchSize) :
        mFeatureSize(0),
        mImgPatchSize(patchSize),
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
        std::vector<int> featurePosPair;
        featurePosPair.reserve(featureSize * 2);
        srand(time(NULL));
        int maxIndex = mImgPatchSize * mImgPatchSize;
        mFeaturePosPairs.reserve(mFeaturePosPairs.size() + featureSize * 2);
        for (int featureId = 0; featureId < featureSize; featureId++)
        {
            int indexX = rand() % maxIndex;
            int indexY = rand() % maxIndex;
            while (indexX == indexY)
            {
                indexY = rand() % maxIndex;
            }
            featurePosPair.push_back(indexX);
            featurePosPair.push_back(indexY);
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
                ScaleToPatchCoord(featurePosPair.at(featureId * 2), imgRowX, imgColX);
                imgRowX += theta.at(0);
                imgColX += theta.at(1);
                int imgRowY, imgColY;
                ScaleToPatchCoord(featurePosPair.at(featureId * 2 + 1), imgRowY, imgColY);
                imgRowY += theta.at(0);
                imgColY += theta.at(1);
                if (img.ptr(imgColX, imgRowX)[0] > img.ptr(imgColY, imgRowY)[0])
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
     
    void SimpleCascadedPoseRegression::ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, int fernId, 
        std::vector<bool>& features) const
    {
        features.clear();
        features.resize(mFeatureSize);
        int baseIndex = fernId * 2 * mFeatureSize;
        for (int featureId = 0; featureId < mFeatureSize; featureId++)
        {
            int imgRowX, imgColX;
            ScaleToPatchCoord(mFeaturePosPairs.at(baseIndex + 2 * featureId), imgRowX, imgColX);
            imgRowX += theta.at(0);
            imgColX += theta.at(1);
            int imgRowY, imgColY;
            ScaleToPatchCoord(mFeaturePosPairs.at(baseIndex + 2 * featureId + 1), imgRowY, imgColY);
            imgRowY += theta.at(0);
            imgColY += theta.at(1);
            if (img.ptr(imgColX, imgRowX)[0] > img.ptr(imgColY, imgRowY)[0])
            {
                features.at(featureId) = 1;
            }
            else
            {
                features.at(featureId) = 0;
            }
        }
    }

    void SimpleCascadedPoseRegression::Reset(void)
    {
        CascadedPoseRegression::Reset();
        mFeaturePosPairs.clear();
        mFeatureSize = 0;
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
