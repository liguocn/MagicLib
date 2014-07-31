#include "RealTimeFaceDetection.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
//#include "../Common/ToolKit.h"
#include <algorithm>

namespace
{
    class ValueIndex
    {
    public:
        ValueIndex();
        ValueIndex(int value, int index);
        ~ValueIndex();
        bool operator < (const ValueIndex &vi) const;

    public:
        int mValue;
        int mIndex;
    };

    ValueIndex::ValueIndex() :
        mValue(0),
        mIndex(0)
    {
    }

    ValueIndex::ValueIndex(int value, int index) :
        mValue(value),
        mIndex(index)
    {
    }

    ValueIndex::~ValueIndex()
    {
    }

    bool ValueIndex::operator < (const ValueIndex &vi) const
    {
        return mValue < vi.mValue;
    }
}

namespace MagicDIP
{

    HaarClassifier::HaarClassifier() :
        mFeature(),
        mThreshold(0),
        mIsLess(true)
    {
    }

    HaarClassifier::HaarClassifier(const HaarFeature& feature) :
        mFeature(feature),
        mThreshold(0),
        mIsLess(true)
    {

    }

    HaarClassifier::~HaarClassifier()
    {
    }

    void HaarClassifier::SetFeature(const HaarFeature& feature)
    {
        mFeature = feature;
    }

    HaarFeature HaarClassifier::GetFeature(void) const
    {
        return mFeature;
    }

    double HaarClassifier::GetThreshold(void) const
    {
        return mThreshold;
    }

    bool HaarClassifier::IsLess(void) const
    {
        return mIsLess;
    }

    int HaarClassifier::Learn(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
        const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex,
        double* trainError)
    {
        int faceDataCount = faceDataWeights.size();
        std::vector<ValueIndex> faceFeatures(faceDataCount);
        double faceSum = 0;
        for (int faceId = 0; faceId < faceDataCount; faceId++)
        {
            faceFeatures.at(faceId).mValue = CalFeatureValue(faceImgLoader, faceId);
            faceFeatures.at(faceId).mIndex = faceId;
            faceSum += faceDataWeights.at(faceId);
        }
        std::sort(faceFeatures.begin(), faceFeatures.end());

        int nonFaceDataCount = nonFaceDataWeights.size();
        std::vector<ValueIndex> nonFaceFeatures(nonFaceDataCount);
        double nonFaceSum = 0;
        for (int nonFaceId = 0; nonFaceId < nonFaceDataCount; nonFaceId++)
        {
            nonFaceFeatures.at(nonFaceId).mValue = CalFeatureValue(nonFaceImgLoader, nonFaceIndex.at(nonFaceId));
            nonFaceFeatures.at(nonFaceId).mIndex = nonFaceId;
            nonFaceSum += nonFaceDataWeights.at(nonFaceId);
        }
        std::sort(nonFaceFeatures.begin(), nonFaceFeatures.end());

        double minError = DBL_MAX; //1.7976931348623158e+308
        int faceId = 0;
        double faceAccumulate = 0;
        int nonFaceId = 0;
        double nonFaceAccumulate = 0;
        while (faceId < faceDataCount || nonFaceId < nonFaceDataCount)
        {
            int threshold;
            if (faceId == faceDataCount)
            {
                threshold = nonFaceFeatures.at(nonFaceId).mValue;
                while (nonFaceFeatures.at(nonFaceId).mValue == threshold)
                {
                    nonFaceAccumulate += nonFaceDataWeights.at(nonFaceFeatures.at(nonFaceId).mIndex);
                    nonFaceId++;
                    if (nonFaceId == nonFaceDataCount)
                    {
                        break;
                    }
                }
            }
            else if (nonFaceId == nonFaceDataCount)
            {
                threshold = faceFeatures.at(faceId).mValue;
                while (threshold == faceFeatures.at(faceId).mValue)
                {
                    faceAccumulate += faceDataWeights.at(faceFeatures.at(faceId).mIndex);
                    faceId++;
                    if (faceId == faceDataCount)
                    {
                        break;
                    }
                }
            }
            else
            {
                if (faceFeatures.at(faceId).mValue < nonFaceFeatures.at(nonFaceId).mValue)
                {
                    threshold = faceFeatures.at(faceId).mValue;
                    while (threshold == faceFeatures.at(faceId).mValue)
                    {
                        faceAccumulate += faceDataWeights.at(faceFeatures.at(faceId).mIndex);
                        faceId++;
                        if (faceId == faceDataCount)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    threshold = nonFaceFeatures.at(nonFaceId).mValue;
                    while (nonFaceFeatures.at(nonFaceId).mValue == threshold)
                    {
                        nonFaceAccumulate += nonFaceDataWeights.at(nonFaceFeatures.at(nonFaceId).mIndex);
                        nonFaceId++;
                        if (nonFaceId == nonFaceDataCount)
                        {
                            break;
                        }
                    }
                }
            }
            //Calculate Error
            double lessError = faceSum - faceAccumulate + nonFaceAccumulate; 
            if (lessError < minError)
            {
                minError = lessError;
                mThreshold = threshold + 0.5;
                mIsLess = true;
            }
            double largeError = faceAccumulate + nonFaceSum - nonFaceAccumulate;
            if (largeError < minError)
            {
                minError = largeError;
                mThreshold = threshold + 0.5;
                mIsLess = false;
            }
        } 
        *trainError = minError;
        
        return MAGIC_NO_ERROR;
    }
     
    int HaarClassifier::Predict(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, float scale) const
    {
        int featureValue = CalFeatureValue(integralImg, imgW, sRow, sCol, scale);
        if (mIsLess)
        {
            return featureValue < mThreshold;
        }
        else
        {
            return featureValue > mThreshold;
        }
    }

    int HaarClassifier::Predict(const ImageLoader& imgLoader, int dataId) const
    {
        int featureValue = CalFeatureValue(imgLoader, dataId);
        if (mIsLess)
        {
            return featureValue < mThreshold;
        }
        else
        {
            return featureValue > mThreshold;
        }
    }

    int HaarClassifier::CalFeatureValue(const ImageLoader& imgLoader, int dataId) const
    {
        if (mFeature.type == 0)
        {
            int posValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol / 2 - 1);
            int negValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol / 2,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            return (posValue - negValue) / (mFeature.lRow * mFeature.lCol / 2);
        }
        else if (mFeature.type == 1)
        {
            int negValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol + mFeature.lCol - 1);
            int posValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2, mFeature.sCol,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            return (posValue - negValue) / (mFeature.lRow * mFeature.lCol / 2);
        }
        else if (mFeature.type == 2)
        {
            int posLeftValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol / 3 - 1);
            int posRightValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol * 2 / 3,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            int negValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol / 3,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol * 2 / 3 - 1);
            return (posLeftValue + posRightValue - negValue) / (mFeature.lRow * mFeature.lCol / 3);
        }
        else if (mFeature.type == 3)
        {
            int posTopLeft = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.lCol + mFeature.lCol / 2 - 1);
            int posRightDown = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2, mFeature.sCol + mFeature.lCol / 2,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            int negRightTop = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol / 2,
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol + mFeature.lCol - 1);
            int negLeftDown = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2, mFeature.lCol, 
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol / 2 - 1);
            return (posTopLeft + posRightDown - negRightTop - negLeftDown) / (mFeature.lRow * mFeature.lCol / 4);
        }
        else
        {
            DebugLog << "Error: wrong type: " << mFeature.type << std::endl;
            return 0;
        }
    }

    int HaarClassifier::ImgBoxValue(const ImageLoader& imgLoader, int dataId, int sRow, int sCol, int eRow, int eCol) const
    {
        if (sRow > 0 && sCol > 0)
        {
            return imgLoader.GetIntegralValue(dataId, sRow - 1, sCol - 1) + imgLoader.GetIntegralValue(dataId, eRow, eCol) -
                imgLoader.GetIntegralValue(dataId, sRow - 1, eCol) - imgLoader.GetIntegralValue(dataId, eRow, sCol - 1);
        }
        else if (sRow > 0 && sCol == 0)
        {
            return imgLoader.GetIntegralValue(dataId, eRow, eCol) - imgLoader.GetIntegralValue(dataId, sRow - 1, eCol);
        }
        else if (sRow == 0 && sCol > 0)
        {
            return imgLoader.GetIntegralValue(dataId, eRow, eCol) - imgLoader.GetIntegralValue(dataId, eRow, sCol - 1);
        }
        else if (sRow == 0 && sCol == 0)
        {
            return imgLoader.GetIntegralValue(dataId, eRow, eCol);
        }
        else
        {
            DebugLog << "Error: ImgBoxValue: " << sRow << " " << sCol << " " << eRow << " " << eCol << std::endl;
            return 0;
        }
    }

    int HaarClassifier::CalFeatureValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, float scale) const
    {
        int lRow = floor(mFeature.lRow * scale + 0.5);
        int lCol = floor(mFeature.lCol * scale + 0.5);
        if (mFeature.type == 0)
        {
            int posValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol / 2 - 1);
            int negValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol + lCol / 2,
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol - 1);
            return (posValue - negValue) / (lRow * lCol / 2);
        }
        else if (mFeature.type == 1)
        {
            int negValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + lRow / 2 - 1, mFeature.sCol + lCol - 1);
            int posValue = ImgBoxValue(integralImg, imgW, mFeature.sRow + lRow / 2, mFeature.sCol,
                mFeature.sRow + lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            return (posValue - negValue) / (lRow * lCol / 2);
        }
        else if (mFeature.type == 2)
        {
            int posLeftValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol,
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol / 3 - 1);
            int posRightValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol + lCol * 2 / 3,
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol - 1);
            int negValue = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol + lCol / 3,
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol * 2 / 3 - 1);
            return (posLeftValue + posRightValue - negValue) / (lRow * lCol / 3);
        }
        else if (mFeature.type == 3)
        {
            int posTopLeft = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + lRow / 2 - 1, mFeature.lCol + lCol / 2 - 1);
            int posRightDown = ImgBoxValue(integralImg, imgW, mFeature.sRow + lRow / 2, mFeature.sCol + lCol / 2,
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol - 1);
            int negRightTop = ImgBoxValue(integralImg, imgW, mFeature.sRow, mFeature.sCol + lCol / 2,
                mFeature.sRow + lRow / 2 - 1, mFeature.sCol + lCol - 1);
            int negLeftDown = ImgBoxValue(integralImg, imgW, mFeature.sRow + lRow / 2, mFeature.lCol, 
                mFeature.sRow + lRow - 1, mFeature.sCol + lCol / 2 - 1);
            return (posTopLeft + posRightDown - negRightTop - negLeftDown) / (lRow * lCol / 4);
        }
        else
        {
            DebugLog << "Error: wrong type: " << mFeature.type << std::endl;
            return 0;
        }
    }

    int HaarClassifier::ImgBoxValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, int eRow, int eCol) const
    {
        if (sRow > 0 && sCol > 0)
        {
            return GetIntegralValue(integralImg, imgW, sRow - 1, sCol - 1) + GetIntegralValue(integralImg, imgW, eRow, eCol) -
                GetIntegralValue(integralImg, imgW, sRow - 1, eCol) - GetIntegralValue(integralImg, imgW, eRow, sCol - 1);
        }
        else if (sRow > 0 && sCol == 0)
        {
            return GetIntegralValue(integralImg, imgW, eRow, eCol) - GetIntegralValue(integralImg, imgW, sRow - 1, eCol);
        }
        else if (sRow == 0 && sCol > 0)
        {
            return GetIntegralValue(integralImg, imgW, eRow, eCol) - GetIntegralValue(integralImg, imgW, eRow, sCol - 1);
        }
        else if (sRow == 0 && sCol == 0)
        {
            return GetIntegralValue(integralImg, imgW, eRow, eCol);
        }
        else
        {
            DebugLog << "Error: ImgBoxValue: " << sRow << " " << sCol << " " << eRow << " " << eCol << std::endl;
            return 0;
        }
    }

    unsigned int HaarClassifier::GetIntegralValue(const std::vector<unsigned int>& integralImg, int imgW, int hid, int wid) const
    {
        return integralImg.at(hid * imgW + wid);
    }

    void HaarClassifier::Save(std::ofstream& fout) const
    {
        fout << mFeature.sRow << " " << mFeature.sCol << " " << mFeature.lRow << " " << mFeature.lCol << " " << mFeature.type 
            << " " << mThreshold << " " << mIsLess << std::endl;
    }
        
    void HaarClassifier::Load(std::ifstream& fin)
    {
        fin >> mFeature.sRow >> mFeature.sCol >> mFeature.lRow >> mFeature.lCol >> mFeature.type >> mThreshold >> mIsLess;
    }

    AdaBoostFaceDetection::AdaBoostFaceDetection() :
        mClassifiers(),
        mClassifierWeights(),
        mThreshold(0),
        mClassifierCandidates()
    {
    }

    AdaBoostFaceDetection::~AdaBoostFaceDetection()
    {
        Reset();
    }

    int AdaBoostFaceDetection::Learn(const ImageLoader& faceImgLoader, const ImageLoader& nonFaceImgLoader,
        const std::vector<bool>& nonFaceValidFlag, int levelCount)
    {
        Reset();
        mClassifiers.reserve(levelCount);
        mClassifierWeights.reserve(levelCount);
        
        int faceCount = faceImgLoader.GetImageCount();
        std::vector<double> faceWeights(faceCount, 0.5 / faceCount);
        
        std::vector<int> nonFaceIndex;
        for (int nonFaceId = 0; nonFaceId < nonFaceValidFlag.size(); nonFaceId++)
        {
            if (nonFaceValidFlag.at(nonFaceId))
            {
                nonFaceIndex.push_back(nonFaceId);
            }
        }
        int nonFaceCount = nonFaceIndex.size();
        DebugLog << "nonFaceCount: " << nonFaceCount << std::endl;
        if (nonFaceCount == 0)
        {
            DebugLog << "There is no non-face data now" << std::endl;
            return MAGIC_EMPTY_INPUT;
        }
        std::vector<double> nonFaceWeights(nonFaceCount, 0.5 / nonFaceCount);

        GenerateClassifierCadidates(faceImgLoader.GetImageWidth(0));

        std::vector<int> faceResFlag(faceCount);
        std::vector<int> nonFaceResFlag(nonFaceCount);
        double epsilon = 1.0e-10;
        mThreshold = 0.0;
        for (int levelId = 0; levelId < levelCount; levelId++)
        {
            DebugLog << " AdaBoost level: " << levelId << std::endl;
            int weakClassifierId = TrainWeakClassifier(faceImgLoader, faceWeights, nonFaceImgLoader, nonFaceWeights,
                nonFaceIndex);
            HaarClassifier* pWeakClassifier = NULL;
            if (weakClassifierId != -1)
            {
                pWeakClassifier = mClassifierCandidates.at(weakClassifierId);
                HaarFeature hf = pWeakClassifier->GetFeature();
                DebugLog << "  Haar feature: " << hf.type << " " << hf.sRow << " " << hf.sCol << " " << hf.lRow << " " << hf.lCol << std::endl;
                mClassifiers.push_back(pWeakClassifier);
                mClassifierCandidates.at(weakClassifierId) = NULL;
            }
            else
            {
                Reset();
                DebugLog << "Error: NULL Weak Classifier" << std::endl;
                return MAGIC_INVALID_RESULT;
            } 
            double trainingError = 0.0;
            int detectedFaceNum = 0;
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                faceResFlag.at(faceId) = abs(1 - pWeakClassifier->Predict(faceImgLoader, faceId));
                trainingError += faceWeights.at(faceId) * faceResFlag.at(faceId);
                if (faceResFlag.at(faceId) == 0)
                {
                    detectedFaceNum++;
                }
            }
            DebugLog << "  Detect Face Right: " << detectedFaceNum << " faceCount: " << faceCount 
                << " pro: " << float(detectedFaceNum) / float(faceCount) << std::endl;
            int detectedNonFaceNum = 0;
            for (int nonFaceId = 0; nonFaceId < nonFaceCount; nonFaceId++)
            {
                nonFaceResFlag.at(nonFaceId) = pWeakClassifier->Predict(nonFaceImgLoader, nonFaceIndex.at(nonFaceId));
                trainingError += nonFaceWeights.at(nonFaceId) * nonFaceResFlag.at(nonFaceId);
                if (nonFaceResFlag.at(nonFaceId) == 0)
                {
                    detectedNonFaceNum++;
                }
            }
            DebugLog << "  Detect Non-Face Right: " << detectedNonFaceNum << " nonFaceCount: " << nonFaceCount 
                << " pro: " << float(detectedNonFaceNum) / float(nonFaceCount) << std::endl;
            DebugLog << "  training error: " << trainingError << std::endl;
            if (trainingError < 0.5)
            {
                DebugLog << "  Weak Classifier " << levelId << ": " << trainingError << std::endl;
            }

            if (fabs(trainingError - 1.0) < epsilon)
            {
                trainingError = 1.0 - epsilon;
                DebugLog << "  Weak Classifier " << levelId << ": is too weak!" << std::endl;
            }
            else if (fabs(trainingError) < epsilon)
            {
                trainingError = epsilon;
                DebugLog << "  Weak Classifier " << levelId << ": is too good!" << std::endl;
            }

            double beta = trainingError / (1.0 - trainingError);
            double weight = log(1.0 / beta);
            DebugLog << "  weight: " << weight << std::endl;
            mClassifierWeights.push_back(weight);
            mThreshold += weight;

            //Update dataWeights
            double weightSum = 0.0;
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                faceWeights.at(faceId) *= pow(beta, faceResFlag.at(faceId));
                weightSum += faceWeights.at(faceId);
            }
            for (int nonFaceId = 0; nonFaceId < nonFaceCount; nonFaceId++)
            {
                nonFaceWeights.at(nonFaceId) *= pow(beta, nonFaceResFlag.at(nonFaceId));
                weightSum += nonFaceWeights.at(nonFaceId);
            }
            for (std::vector<double>::iterator itr = faceWeights.begin(); itr != faceWeights.end(); itr++)
            {
                *itr /= weightSum;
            }
            for (std::vector<double>::iterator itr = nonFaceWeights.begin(); itr != nonFaceWeights.end(); itr++)
            {
                *itr /= weightSum;
            }
        }
        mThreshold *= 0.5;

        ClearClassifierCadidates();

        return MAGIC_NO_ERROR;
    }
    
    int AdaBoostFaceDetection::Predict(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, double scale) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            res += mClassifiers.at(cid)->Predict(integralImg, imgW, sRow, sCol, scale) * mClassifierWeights.at(cid);
        }
        if (res > mThreshold)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    int AdaBoostFaceDetection::Predict(const ImageLoader& imgLoader, int dataId) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            res += mClassifiers.at(cid)->Predict(imgLoader, dataId) * mClassifierWeights.at(cid);
        }
        if (res > mThreshold)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void AdaBoostFaceDetection::Save(std::ofstream& fout) const
    {
        fout << mThreshold << " " << mClassifiers.size() << std::endl;
        for (std::vector<HaarClassifier*>::const_iterator itr = mClassifiers.begin(); itr != mClassifiers.end(); itr++)
        {
            (*itr)->Save(fout);
        }
        for (std::vector<double>::const_iterator itr = mClassifierWeights.begin(); itr != mClassifierWeights.end(); itr++)
        {
            fout << (*itr) << " ";
        }
        fout << std::endl;
    }
        
    void AdaBoostFaceDetection::Load(std::ifstream& fin)
    {
        Reset();
        int classifierCount;
        fin >> mThreshold >> classifierCount;
        mClassifiers.reserve(classifierCount);
        mClassifierWeights.reserve(classifierCount);
        for (int cid = 0; cid < classifierCount; cid++)
        {
            HaarClassifier* pWeakClassifier = new HaarClassifier;
            pWeakClassifier->Load(fin);
            mClassifiers.push_back(pWeakClassifier);
        }
        for (int cid = 0; cid < classifierCount; cid++)
        {
            double dTemp;
            fin >> dTemp;
            mClassifierWeights.push_back(dTemp);
        }
    }

    void AdaBoostFaceDetection::GenerateClassifierCadidates(int baseImgSize)
    {
        //Enhancement: use uniform sampling strategy.
        for (int sRow = 0; sRow < baseImgSize; sRow += 4)
        {
            for (int sCol = 0; sCol < baseImgSize; sCol += 4)
            {
                int halfImgSize = baseImgSize / 2;
                int colMaxLen = baseImgSize - sCol;
                colMaxLen = colMaxLen > halfImgSize ? halfImgSize : colMaxLen;
                int rowMaxLen = baseImgSize - sRow;
                rowMaxLen = rowMaxLen > halfImgSize ? halfImgSize : rowMaxLen;
                for (int lRow = 4; lRow <= rowMaxLen; lRow += 4)
                {
                    for (int lCol = 8; lCol <= colMaxLen; lCol += 8)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 0};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 8; lRow <= rowMaxLen; lRow += 8)
                {
                    for (int lCol = 4; lCol <= colMaxLen; lCol += 4)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 1};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 4; lRow <= rowMaxLen; lRow += 4)
                {
                    for (int lCol = 12; lCol <= colMaxLen; lCol += 12)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 2};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 8; lRow <= rowMaxLen; lRow += 8)
                {
                    for (int lCol = 8; lCol <= colMaxLen; lCol += 8)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 3};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
            }
        }
        DebugLog << "GenerateClassifierCadidates: " << mClassifierCandidates.size() << std::endl;
    }

    void AdaBoostFaceDetection::ClearClassifierCadidates(void)
    {
        for (std::vector<HaarClassifier*>::iterator itr = mClassifierCandidates.begin(); itr != mClassifierCandidates.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mClassifierCandidates.clear();
    }

    int AdaBoostFaceDetection::TrainWeakClassifier(const ImageLoader& faceImgLoader, 
        const std::vector<double>& faceDataWeights, const ImageLoader& nonFaceImgLoader, 
        const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex)
    {
        double minTrainError = 1.0e308;
        int classifierId = -1;
        for (int cid = 0; cid < mClassifierCandidates.size(); cid++)
        {
            HaarClassifier* pClassifier = mClassifierCandidates.at(cid);
            if (pClassifier != NULL)
            {
                double trainError;
                //double weakLearTime = MagicCore::ToolKit::GetTime();
                int res = pClassifier->Learn(faceImgLoader, faceDataWeights, nonFaceImgLoader, nonFaceDataWeights, nonFaceIndex, &trainError);
                HaarFeature hf = pClassifier->GetFeature();
                /*DebugLog << "   weak candidate: " << cid << " trainError: " << trainError <<
                    " feature: " << hf.type << " " << hf.sRow << " " << hf.sCol << " " << hf.lRow << " " << hf.lCol <<
                    " threshold: " << pClassifier->GetThreshold() << " IsLess: " << pClassifier->IsLess() << std::endl;*/
                    //<< " time: " << MagicCore::ToolKit::GetTime() - weakLearTime << std::endl;
                if (res != MAGIC_NO_ERROR)
                {
                    DebugLog << "HaarClassifier training failed" << std::endl;
                    return -1;
                }
                if (trainError < minTrainError)
                {
                    minTrainError = trainError;
                    classifierId = cid;
                }
            }
        }
        return classifierId;
    }

    void AdaBoostFaceDetection::Reset(void)
    {
        mThreshold = 0;
        for (std::vector<HaarClassifier*>::iterator itr = mClassifiers.begin(); itr != mClassifiers.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mClassifiers.clear();
        mClassifierWeights.clear();
        for (std::vector<HaarClassifier*>::iterator itr = mClassifierCandidates.begin(); itr != mClassifierCandidates.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mClassifierCandidates.clear();
    }

    RealTimeFaceDetection::RealTimeFaceDetection() : 
        mBaseImgSize(0),
        mCascadedDetectors()
    {
    }

    RealTimeFaceDetection::~RealTimeFaceDetection()
    {
        Reset();
    }

    int RealTimeFaceDetection::Learn(const std::vector<std::string>& faceImages, const std::vector<std::string>& nonFaceImages, 
        const std::vector<int>& layerCounts)
    {
        if (faceImages.size() == 0 || nonFaceImages.size() == 0 || layerCounts.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        for (std::vector<int>::const_iterator itr = layerCounts.begin(); itr != layerCounts.end(); itr++)
        {
            if ((*itr) <= 0)
            {
                return MAGIC_INVALID_INPUT;
            }
        }
        Reset();
        int stageCount = layerCounts.size();
        mCascadedDetectors.reserve(stageCount);
        ImageLoader faceImgLoader;
        faceImgLoader.LoadImages(faceImages, ImageLoader::IT_Gray);
        faceImgLoader.GenerateIntegralImage();
        faceImgLoader.ClearImageData();
        DebugLog << "face loaded" << std::endl;
        mBaseImgSize = faceImgLoader.GetImageWidth(0);
        ImageLoader nonFaceImgLoader;
        nonFaceImgLoader.LoadImages(nonFaceImages, ImageLoader::IT_Gray);
        nonFaceImgLoader.GenerateIntegralImage();
        nonFaceImgLoader.ClearImageData();
        DebugLog << "non face loaded" << std::endl;
        std::vector<bool> nonFaceValidFlag(nonFaceImages.size(), 1);
        DebugLog << "Learn Cascaded detectors..." << std::endl;
        for (int stageId = 0; stageId < stageCount; stageId++)
        {
            DebugLog << "Stage " << stageId << std::endl;
            //double stageTime = MagicCore::ToolKit::GetTime();
            AdaBoostFaceDetection* pDetector = new AdaBoostFaceDetection;
            int res = pDetector->Learn(faceImgLoader, nonFaceImgLoader, nonFaceValidFlag, layerCounts.at(stageId));
            if (res != MAGIC_NO_ERROR)
            {
                if (res == MAGIC_EMPTY_INPUT)
                {
                    DebugLog << "Stage: " << stageId << " is empty input, break" << std::endl;
                    delete pDetector;
                    break;
                }
                Reset();
                DebugLog << "stage " << stageId << " learn failed" << std::endl;
                return MAGIC_INVALID_RESULT;
            }
            mCascadedDetectors.push_back(pDetector);
            
            //filter non faces
            for (int nonFaceId = 0; nonFaceId < nonFaceValidFlag.size(); nonFaceId++)
            {
                if (pDetector->Predict(nonFaceImgLoader, nonFaceId) == 0)
                {
                    nonFaceValidFlag.at(nonFaceId) = 0;
                }
            }
            //DebugLog << "time: " << MagicCore::ToolKit::GetTime() - stageTime << std::endl;
        }

        if (mCascadedDetectors.size() == 0)
        {
            return MAGIC_INVALID_RESULT;
        }

        //test
        int detectedFaceNum = 0;
        for (int faceDataId = 0; faceDataId < faceImages.size(); faceDataId++)
        {
            cv::Mat testImg = cv::imread(faceImages.at(faceDataId));
            std::vector<int> faces;
            int res = Detect(testImg, faces);
            testImg.release();
            if (res == 1)
            {
                detectedFaceNum++;
                //DebugLog << "face image name: " << faceImages.at(faceDataId) << std::endl;
            }
            else if (res > 1)
            {
                DebugLog << "Test error: " << res << std::endl;
            }
        }
        DebugLog << "Test: Detected face: " << detectedFaceNum << " face count: " << faceImages.size()
            << " pro: " << float(detectedFaceNum) / float(faceImages.size()) << std::endl;

        int detectedNonFaceNum = 0;
        for (int nonFaceDataId = 0; nonFaceDataId < nonFaceImages.size(); nonFaceDataId++)
        {
            cv::Mat testImg = cv::imread(nonFaceImages.at(nonFaceDataId));
            std::vector<int> faces;
            int res = Detect(testImg, faces);
            testImg.release();
            if (res == 0)
            {
                detectedNonFaceNum++;
            }
        }
        DebugLog << "Test: Detected non-face: " << detectedNonFaceNum << " face count: " << nonFaceImages.size()
            << " pro: " << float(detectedNonFaceNum) / float(nonFaceImages.size()) << std::endl;

        return MAGIC_NO_ERROR;
    }

    int RealTimeFaceDetection::Detect(const cv::Mat& img, std::vector<int>& faces) const
    {
        faces.clear();
        double stepSize = 2;
        double deltaScale = 1.25;
        double curScale = 1.0;
        int curStep = stepSize * curScale;
        int curSubImgSize = curScale * mBaseImgSize;
        int imgH = img.rows;
        int imgW = img.cols;
        std::vector<unsigned int> integralImg;
        ImageLoader::TransferToIntegralImg(img, integralImg);
        while (curSubImgSize <= imgH && curSubImgSize <= imgW)
        {
            int maxRow = imgH - curSubImgSize;
            int maxCol = imgW - curSubImgSize;
            for (int rid = 0; rid <= maxRow; rid += curStep)
            {
                for (int cid = 0; cid <= maxCol; cid += curStep)
                {
                    if (DetectOneFace(integralImg, imgW, rid, cid, curScale))
                    {
                        faces.push_back(rid);
                        faces.push_back(cid);
                        faces.push_back(curSubImgSize);
                        faces.push_back(curSubImgSize);
                    }
                }
            }
            //update parameter
            curScale *= deltaScale;
            curSubImgSize = curScale * mBaseImgSize;
            curStep = stepSize * curScale;
        }
        return (faces.size() / 4);
    }

    void RealTimeFaceDetection::Save(const std::string& fileName) const
    {
        std::ofstream fout(fileName);
        fout << mBaseImgSize << " " << mCascadedDetectors.size() << std::endl;
        for (std::vector<AdaBoostFaceDetection* >::const_iterator itr = mCascadedDetectors.begin(); itr != mCascadedDetectors.end(); itr++)
        {
            (*itr)->Save(fout);
        }
        fout.close();
    }
        
    void RealTimeFaceDetection::Load(const std::string& fileName)
    {
        Reset();
        int cascadeCount;
        std::ifstream fin(fileName);
        fin >> mBaseImgSize >> cascadeCount;
        mCascadedDetectors.reserve(cascadeCount);
        for (int cascadeId = 0; cascadeId < cascadeCount; cascadeId++)
        {
            AdaBoostFaceDetection* pDetector = new AdaBoostFaceDetection;
            pDetector->Load(fin);
            mCascadedDetectors.push_back(pDetector);
        }
        fin.close();
    }

    int RealTimeFaceDetection::DetectOneFace(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, double scale) const
    {
        if (mCascadedDetectors.size() == 0)
        {
            return 0;
        }
        for (std::vector<AdaBoostFaceDetection*>::const_iterator itr = mCascadedDetectors.begin(); itr != mCascadedDetectors.end(); itr++)
        {
            if ( (*itr)->Predict(integralImg, imgW, sRow, sCol, scale) == 0 )
            {
                return 0;
            }
        }
        return 1;
    }

    void RealTimeFaceDetection::Reset(void)
    {
        mBaseImgSize = 0;
        for (std::vector<AdaBoostFaceDetection* >::iterator itr = mCascadedDetectors.begin(); itr != mCascadedDetectors.end(); itr++)
        {
            delete (*itr);
            (*itr) = NULL;
        }
        mCascadedDetectors.clear();
    }
}
