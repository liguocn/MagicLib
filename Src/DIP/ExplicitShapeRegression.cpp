#include "ExplicitShapeRegression.h"
#include "../MachineLearning/RandomMethod.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Tool/Profiler.h"
#include <stdio.h>
#include <time.h>

namespace MagicDIP
{
    ExplicitShapeRegression::ExplicitShapeRegression() :
        mInnerFernCount(0),
        mValidFeatureSize(0),
        mImgPatchSize(11),
        mValidFeaturePosPairs(),
        mRandomFerns(),
        mImageLoader(),
        mFeaturePosPairs()
    {
    }

    ExplicitShapeRegression::ExplicitShapeRegression(int patchSize) :
        mInnerFernCount(0),
        mValidFeatureSize(0),
        mImgPatchSize(patchSize),
        mValidFeaturePosPairs(),
        mRandomFerns(),
        mImageLoader(),
        mFeaturePosPairs()
    {

    }

    ExplicitShapeRegression::~ExplicitShapeRegression()
    {
        Reset();
    }

    int ExplicitShapeRegression::LearnRegression(const std::vector<std::string>& imgFiles, const std::vector<double>& initTheta, 
            const std::vector<double>& finalTheta, int dataPerImgCount, int keyPointCount, int outerCount, int innerCount, 
            int fernSize, int featureSizePerKey)
    {
        if (imgFiles.size() == 0 || initTheta.size() == 0 || finalTheta.size() == 0)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (imgFiles.size() * dataPerImgCount != finalTheta.size() / (keyPointCount * 2))
        {
            return MAGIC_INVALID_INPUT;
        }
        Reset();
        mInnerFernCount = innerCount;

        //ImageLoader imageLoader;
        DebugLog << "Load Image......";
        mImageLoader.LoadImages(imgFiles, ImageLoader::IT_Gray);
        DebugLog << "done" << std::endl;

        srand(time(NULL));
        int thetaDim = keyPointCount * 2;
        int fernCount = outerCount * innerCount;
        mRandomFerns.reserve(fernCount);
        int dataCount = initTheta.size() / thetaDim;
        std::vector<bool> dataX(featureSizePerKey * keyPointCount * dataCount);
        std::vector<double> theta(thetaDim); //one theta
        std::vector<double> curTheta = initTheta;
        std::vector<double> interTheta;
        std::vector<double> deltaTheta(curTheta.size());
        for (int outerId = 0; outerId < outerCount; outerId++)
        {
            DebugLog << "Out stage: " << outerId << std::endl;
            interTheta = curTheta;
            for (int thetaId = 0; thetaId < interTheta.size(); thetaId++)
            {
                deltaTheta.at(thetaId) = finalTheta.at(thetaId) - interTheta.at(thetaId);
            }
            FeaturePatternGeneration(interTheta, deltaTheta, dataPerImgCount, dataCount, featureSizePerKey, keyPointCount, dataX);
            int fernBaseId = outerId * mInnerFernCount;
            for (int innerId = 0; innerId < mInnerFernCount; innerId++)
            {
                DebugLog << "  Inner stage: " << innerId << std::endl;
                double timeStart = MagicTool::Profiler::GetTime();
                DebugLog << "    fernId: " << fernBaseId + innerId << std::endl;

                int fernId = fernBaseId + innerId;
                double avgDelta = 0;
                for (int thetaId = 0; thetaId < curTheta.size(); thetaId++)
                {
                    deltaTheta.at(thetaId) = finalTheta.at(thetaId) - curTheta.at(thetaId);
                    avgDelta += fabs(deltaTheta.at(thetaId));
                }
                avgDelta /= curTheta.size();
                DebugLog << "    AvgDelta: " << avgDelta << std::endl;
                MagicML::RandomFern* pFern = new MagicML::RandomFern;
                pFern->Learn(dataX, featureSizePerKey * keyPointCount, deltaTheta, thetaDim, fernSize);
                mRandomFerns.push_back(pFern);
                //Update Valid Features
                std::vector<int> validFeatureIds = pFern->GetFeatureIds();
                /*DebugLog << "  ValidFeatureIds: ";
                for (std::vector<int>::iterator itr = validFeatureIds.begin(); itr != validFeatureIds.end(); itr++)
                {
                    DebugLog << *itr << " ";
                }
                DebugLog << std::endl;*/
                UpdateValidFeaturePosPair(validFeatureIds);
                //Update curTheta
                for (int dataId = 0; dataId < dataCount; dataId++)
                {
                    //Generate feature from theta
                    int baseIndex = dataId * thetaDim;
                    for (int thetaId = 0; thetaId < thetaDim; thetaId++)
                    {
                        //theta.at(thetaId) = curTheta.at(baseIndex + thetaId);
                        theta.at(thetaId) = interTheta.at(baseIndex + thetaId);
                    }
                    std::vector<bool> features;
                    int imgId = dataId / dataPerImgCount;
                    ValidFeatureGenerationByImageLoader(imgId, theta, fernId, features);
                    //Predict delta theta
                    std::vector<double> predictDelta = pFern->PredictWithValidFeature(features);
                    //Update curTheta
                    baseIndex = dataId * thetaDim;
                    for (int thetaId = 0; thetaId < thetaDim; thetaId++)
                    {
                        curTheta.at(baseIndex + thetaId) += predictDelta.at(thetaId);
                    }
                }

                DebugLog << "    time: " << MagicTool::Profiler::GetTime() - timeStart << std::endl;
            }
        }

        return MAGIC_NO_ERROR;
    }
     
    int ExplicitShapeRegression::ShapeRegression(const cv::Mat& img, const std::vector<double>& initTheta, 
        std::vector<double>&finalTheta) const
    {
        if (mRandomFerns.size() == 0)
        {
            return MAGIC_NON_INITIAL;
        }
        finalTheta = initTheta;
        std::vector<double> interTheta;
        int outerCount = mRandomFerns.size() / mInnerFernCount;
        for (int outerId = 0; outerId < outerCount; outerId++)
        {
            interTheta = finalTheta;
            int fernBaseId = outerId * mInnerFernCount;
            for (int innerId = 0; innerId < mInnerFernCount; innerId++)
            {
                int fernId = fernBaseId + innerId;
                std::vector<bool> features;
                ValidFeatureGeneration(img, interTheta, fernId, features);
                std::vector<double> deltaTheta = mRandomFerns.at(fernId)->PredictWithValidFeature(features);
                for (int thetaId = 0; thetaId < finalTheta.size(); thetaId++)
                {
                    finalTheta.at(thetaId) += deltaTheta.at(thetaId);
                }
            }
        }

        return MAGIC_NO_ERROR;
    }

    void ExplicitShapeRegression::FeaturePatternGeneration(const std::vector<double>& theta, const std::vector<double>& dataY, 
        int dataPerImgCount, int dataCount, int featureSizePerKey, int keyPointCount, std::vector<bool>& features)
    {
        //srand(time(NULL));
        int maxIndex = mImgPatchSize * mImgPatchSize * keyPointCount;
        mFeaturePosPairs.clear();
        int featureSize = featureSizePerKey * keyPointCount;
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
        int thetaDim = keyPointCount * 2;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int imgId = dataId / dataPerImgCount;
            int imgH = mImageLoader.GetImageHeight(imgId);
            int imgW = mImageLoader.GetImageWidth(imgId);
            int featureBase = featureSize * dataId;
            int thetaBaseId = dataId * thetaDim;
            for (int featureId = 0; featureId < featureSize; featureId++)
            {    
                int keyIdX, imgRowX, imgColX;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2), keyIdX, imgRowX, imgColX);
                int patchCenRowX = theta.at(thetaBaseId + keyIdX * 2);
                int patchCenColX = theta.at(thetaBaseId + keyIdX * 2 + 1);
                imgRowX += patchCenRowX;
                imgColX += patchCenColX;
                imgRowX = imgRowX < 0 ? 0 : (imgRowX > imgH - 1 ? imgH - 1 : imgRowX);
                imgColX = imgColX < 0 ? 0 : (imgColX > imgW - 1 ? imgW - 1 : imgColX);
                int keyIdY, imgRowY, imgColY;
                ScaleToPatchCoord(mFeaturePosPairs.at(featureId * 2 + 1), keyIdY, imgRowY, imgColY);
                int patchCenRowY = theta.at(thetaBaseId + keyIdY * 2);
                int patchCenColY = theta.at(thetaBaseId + keyIdY * 2 + 1);
                imgRowY += patchCenRowY;
                imgColY += patchCenColY;
                imgRowY = imgRowY < 0 ? 0 : (imgRowY > imgH - 1 ? imgH - 1 : imgRowY);
                imgColY = imgColY < 0 ? 0 : (imgColY > imgW - 1 ? imgW - 1 : imgColY);
                if (mImageLoader.GetGrayImageValue(imgId, imgRowX, imgColX) > 
                    mImageLoader.GetGrayImageValue(imgId, imgRowY, imgColY))
                {
                    features.at(featureBase + featureId) = 1;
                }
                else
                {
                    features.at(featureBase + featureId) = 0;
                }
            }
        }

    }

    void ExplicitShapeRegression::ValidFeatureGeneration(const cv::Mat& img, const std::vector<double>& theta, 
        int fernId, std::vector<bool>& features) const
    {
        features.clear();
        features.resize(mValidFeatureSize);
        int baseIndex = fernId * 2 * mValidFeatureSize;
        for (int featureId = 0; featureId < mValidFeatureSize; featureId++)
        {
            //DebugLog << "    " << featureId << " ";
            int keyIdX, imgRowX, imgColX;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId), keyIdX, imgRowX, imgColX);
            imgRowX += theta.at(keyIdX * 2);
            imgColX += theta.at(keyIdX * 2 + 1);
            imgRowX = imgRowX < 0 ? 0 : (imgRowX > img.rows - 1 ? img.rows - 1 : imgRowX);
            imgColX = imgColX < 0 ? 0 : (imgColX > img.cols - 1 ? img.cols - 1 : imgColX);
            int keyIdY, imgRowY, imgColY;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId + 1), keyIdY, imgRowY, imgColY);
            imgRowY += theta.at(keyIdY * 2);
            imgColY += theta.at(keyIdY * 2 + 1);
            imgRowY = imgRowY < 0 ? 0 : (imgRowY > img.rows - 1 ? img.rows - 1 : imgRowY);
            imgColY = imgColY < 0 ? 0 : (imgColY > img.cols - 1 ? img.cols - 1 : imgColY);
            if (img.ptr(imgRowX, imgColX)[0] > img.ptr(imgRowY, imgColY)[0])
            {
                features.at(featureId) = 1;
            }
            else
            {
                features.at(featureId) = 0;
            }
        }
    }

    void ExplicitShapeRegression::ValidFeatureGenerationByImageLoader(int imgId, const std::vector<double>& theta, int fernId, 
        std::vector<bool>& features) const
    {
        features.clear();
        features.resize(mValidFeatureSize);
        int baseIndex = fernId * 2 * mValidFeatureSize;
        int imgH = mImageLoader.GetImageHeight(imgId);
        int imgW = mImageLoader.GetImageWidth(imgId);
        for (int featureId = 0; featureId < mValidFeatureSize; featureId++)
        {
            int keyIdX, imgRowX, imgColX;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId), keyIdX, imgRowX, imgColX);
            imgRowX += theta.at(keyIdX * 2);
            imgColX += theta.at(keyIdX * 2 + 1);
            imgRowX = imgRowX < 0 ? 0 : (imgRowX > imgH - 1 ? imgH - 1 : imgRowX);
            imgColX = imgColX < 0 ? 0 : (imgColX > imgW - 1 ? imgW - 1 : imgColX);
            int keyIdY, imgRowY, imgColY;
            ScaleToPatchCoord(mValidFeaturePosPairs.at(baseIndex + 2 * featureId + 1), keyIdY, imgRowY, imgColY);
            imgRowY += theta.at(keyIdY * 2);
            imgColY += theta.at(keyIdY * 2 + 1);
            imgRowY = imgRowY < 0 ? 0 : (imgRowY > imgH - 1 ? imgH - 1 : imgRowY);
            imgColY = imgColY < 0 ? 0 : (imgColY > imgW - 1 ? imgW - 1 : imgColY);
            if (mImageLoader.GetGrayImageValue(imgId, imgRowX, imgColX) > 
                mImageLoader.GetGrayImageValue(imgId, imgRowY, imgColY))
            {
                features.at(featureId) = 1;
            }
            else
            {
                features.at(featureId) = 0;
            }
        }
    }

    void ExplicitShapeRegression::UpdateValidFeaturePosPair(const std::vector<int>& validFeatureIds)
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

    void ExplicitShapeRegression::ScaleToPatchCoord(int pos, int& keyId, int& imgRow, int& imgCol) const
    {
        int keySize = mImgPatchSize * mImgPatchSize;
        keyId = pos / keySize;
        int localId = pos % keySize;
        imgRow = localId / mImgPatchSize;
        imgCol = localId % mImgPatchSize;
        int halfPatchSize = mImgPatchSize / 2;
        imgRow = imgRow - halfPatchSize;
        imgCol = imgCol - halfPatchSize;
    }
        
    void ExplicitShapeRegression::Save(const std::string& fileName) const
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
        fout << mInnerFernCount << " " << mValidFeatureSize << " " << mImgPatchSize << std::endl;
        for (std::vector<int>::const_iterator itr = mValidFeaturePosPairs.begin(); itr != mValidFeaturePosPairs.end(); itr++)
        {
            fout << *itr << " ";
        }

        fout.close();
    }
        
    void ExplicitShapeRegression::Load(const std::string& fileName)
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
        
        fin >> mInnerFernCount >> mValidFeatureSize >> mImgPatchSize;
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

    void ExplicitShapeRegression::Reset(void)
    {
        mInnerFernCount = 0;
        mValidFeatureSize = 0;
        mValidFeaturePosPairs.clear();
        for (std::vector<MagicML::RandomFern* >::iterator itr = mRandomFerns.begin(); itr != mRandomFerns.end(); itr++)
        {
            if (*itr != NULL)
            {
                delete (*itr);
                *itr = NULL;
            }
        }
        mRandomFerns.clear();
        mImageLoader.Reset();
        mFeaturePosPairs.clear();
    }
}
