#include "CascadedPoseRegression.h"
#include "../MachineLearning/RandomMethod.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
//#include "../Common/ToolKit.h"
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
        ////Construct mImageList
        //mImageList.reserve(dataCount);
        //for (int dataId = 0; dataId < 1000; dataId++)
        //{
        //    DebugLog << "   image: " << dataId << std::endl;
        //    cv::Mat* pImage = new cv::Mat;
        //    *pImage = cv::imread(imgFiles.at(dataId));
        //    mImageList.push_back(pImage);
        //}
        ////
        std::vector<bool> dataX(featureSize * dataCount);
        std::vector<double> theta(thetaDim); //one theta
        std::vector<double> curTheta = initTheta;
        std::vector<double> deltaTheta(curTheta.size());
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
            //double timeStart = MagicCore::ToolKit::GetTime();
            DebugLog << "fernId: " << fernId << std::endl; 
            //Calculate deltaTheta
            for (int dataId = 0; dataId < dataCount; dataId++)
            {
                if (dataId % 500 != 0)
                {
                    continue;
                }
                DebugLog << "  Cur" << dataId << ": " << curTheta.at(dataId * 2) << " " << curTheta.at(dataId * 2 + 1) << std::endl;
                DebugLog << "  Tar" << dataId << ": " << finalTheta.at(dataId * 2) << " " << finalTheta.at(dataId * 2 + 1) << std::endl;  
            }
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
                cv::Mat img = cv::imread(imgFiles.at(dataId));
                //DebugLog << "    imgOrigin susscess : " << imgFiles.at(dataId) << std::endl;
                if (img.data == NULL)
                {
                    DebugLog << "    error: img.data == NULL" << std::endl;
                }
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
            //DebugLog << "  time: " << MagicCore::ToolKit::GetTime() - timeStart << std::endl;
        }

        //free mImageList
        /*for (int dataId = 0; dataId < dataCount; dataId++)
        {
            if (mImageList.at(dataId) != NULL)
            {
                mImageList.at(dataId)->release();
                delete mImageList.at(dataId);
                mImageList.at(dataId) = NULL;
            }
        }
        mImageList.clear();*/

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

    void SimpleCascadedPoseRegression::Save(const std::string& fileName) const
    {
        std::ofstream fout(fileName);
        //Save random ferns
        int fernCount = mRandomFerns.size();
        fout << fernCount << std::endl;
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
            int predictionDim;
            std::vector<double> predictions;
            std::vector<int> featureIds;
            std::vector<int> featureBases;
            mRandomFerns.at(fernId)->Save(predictionDim, predictions, featureIds, featureBases);
            fout << predictionDim << std::endl;
            int fernSize = featureIds.size();
            fout << fernSize << std::endl;
            for (std::vector<double>::iterator itr = predictions.begin(); itr != predictions.end(); itr++)
            {
                fout << *itr << " ";
            }
            for (std::vector<int>::iterator itr = featureIds.begin(); itr != featureIds.end(); itr++)
            {
                fout << *itr << " ";
            }
            for (std::vector<int>::iterator itr = featureBases.begin(); itr != featureBases.end(); itr++)
            {
                fout << *itr << " ";
            }
            fout << std::endl;
        }

        //Save subclass content
        fout << mValidFeatureSize << " " << mImgPatchSize << std::endl;
        for (std::vector<int>::const_iterator itr = mValidFeaturePosPairs.begin(); itr != mValidFeaturePosPairs.end(); itr++)
        {
            fout << *itr << " ";
        }

        fout.close();
    }

    void SimpleCascadedPoseRegression::Load(const std::string& fileName)
    {
        Reset();
        std::ifstream fin(fileName);
        int fernCount;
        fin >> fernCount;
        mRandomFerns.reserve(fernCount);
        for (int fernId = 0; fernId < fernCount; fernId++)
        {
            int predictionDim;
            fin >> predictionDim;
            int fernSize;
            fin >> fernSize;
            int predVecCount = predictionDim * pow(2, fernSize);
            std::vector<double> predictions(predVecCount);
            double dTemp;
            for (int predId = 0; predId < predVecCount; predId++)
            {
                fin >> dTemp;
                predictions.at(predId) = dTemp;
            }
            std::vector<int> featureIds(fernSize);
            int nTemp;
            for (int fernId = 0; fernId < fernSize; fernId++)
            {
                fin >> nTemp;
                featureIds.at(fernId) = nTemp;
            }
            std::vector<int> featureBases(fernSize);
            for (int fernId = 0; fernId < fernSize; fernId++)
            {
                fin >> nTemp;
                featureBases.at(fernId) = nTemp;
            }
            MagicML::RandomFern* pFern = new MagicML::RandomFern;
            pFern->Load(predictionDim, predictions, featureIds, featureBases);
            mRandomFerns.push_back(pFern);
        }
        
        fin >> mValidFeatureSize >> mImgPatchSize;
        int validPairSize = mValidFeatureSize * fernCount * 2;
        mValidFeaturePosPairs.reserve(validPairSize);
        int nTemp;
        for (int validId = 0; validId < validPairSize; validId++)
        {
            fin >> nTemp;
            mValidFeaturePosPairs.push_back(nTemp);
        }

        fin.close();
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
            cv::Mat img = cv::imread(imgFiles.at(dataId));
            int featureBase = featureSize * dataId;
            for (int featureId = 0; featureId < featureSize; featureId++)
            {
                int patchCenRow = theta.at(dataId * 2);
                int patchCenCol = theta.at(dataId * 2 + 1);
                int imgRowX, imgColX;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2), imgRowX, imgColX);
                imgRowX += patchCenRow;
                imgColX += patchCenCol;
                imgRowX = imgRowX < 0 ? 0 : (imgRowX > img.rows - 1 ? img.rows - 1 : imgRowX);
                imgColX = imgColX < 0 ? 0 : (imgColX > img.cols - 1 ? img.cols - 1 : imgColX);
                int imgRowY, imgColY;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2 + 1), imgRowY, imgColY);
                imgRowY += patchCenRow;
                imgColY += patchCenCol;
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
