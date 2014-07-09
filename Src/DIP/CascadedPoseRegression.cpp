#include "CascadedPoseRegression.h"
#include "../MachineLearning/RandomMethod.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    CascadedPoseRegression::CascadedPoseRegression() :
        mFeatureSize(0),
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
        mFeatureSize = featureSize;
        int dataCount = initTheta.size() / thetaDim;
        std::vector<bool> dataX(mFeatureSize * dataCount);
        std::vector<double> theta(thetaDim); //one theta
        std::vector<double> curTheta = initTheta;
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
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
                FeatureGeneration(img, theta, features);
                img.release();
                baseIndex = dataId * featureSize;
                for (int featureId = 0; featureId < mFeatureSize; featureId++)
                {
                    dataX.at(baseIndex + featureId) = features.at(featureId);
                }
            }
            //Learn random fern
            MagicML::RandomFern* pFern = new MagicML::RandomFern;
            pFern->Learn(dataX, mFeatureSize, finalTheta, thetaDim, fernSize);
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
                FeatureGeneration(img, theta, features);
                //Predict delta theta
                std::vector<double> deltaTheta = pFern->Predict(features);
                //Update curTheta
                baseIndex = dataId * featureSize;
                for (int featureId = 0; featureId < featureSize; featureId++)
                {
                    curTheta.at(baseIndex + featureId) += deltaTheta.at(featureId);
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
        for (std::vector<MagicML::RandomFern* >::const_iterator fernItr = mRandomFerns.begin(); fernItr != mRandomFerns.end(); fernItr++)
        {
            std::vector<bool> features;
            FeatureGeneration(img, finalTheta, features);
            std::vector<double> deltaTheta = (*fernItr)->Predict(features);
            for (int thetaId = 0; thetaId < finalTheta.size(); thetaId++)
            {
                finalTheta.at(thetaId) += deltaTheta.at(thetaId);
            }
        }

        return MAGIC_NO_ERROR;
    }

    void CascadedPoseRegression::FeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, std::vector<bool>& features) const
    {

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
        mFeatureSize = 0;
    }
}
