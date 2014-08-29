#include "RealTimeFaceDetection.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Tool/Profiler.h"
#include <map>
#include <algorithm>
#include <time.h>
#include <stdio.h>

namespace
{
    class ValueIndex
    {
    public:
        ValueIndex();
        ValueIndex(double value, int index);
        ~ValueIndex();
        bool operator < (const ValueIndex &vi) const;

    public:
        double mValue;
        int mIndex;
    };

    ValueIndex::ValueIndex() :
        mValue(0),
        mIndex(0)
    {
    }

    ValueIndex::ValueIndex(double value, int index) :
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

    static int CalCulateLineSegmentOverlap(int sa, int la, int sb, int lb)
    {
        int sMin = sa < sb ? sa : sb;
        int ea = sa + la;
        int eb = sb + lb;
        int eMax = ea > eb ? ea : eb;
        int interLen = la + lb - (eMax - sMin);
        if (interLen > 0)
        {
            return interLen;
        }
        else
        {
            return 0;
        }
    }

    static int ImgBoxValue(const MagicDIP::ImageLoader& imgLoader, int dataId, int sRow, int sCol, int eRow, int eCol)
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

    static double CalFeatureValue(const MagicDIP::ImageLoader& imgLoader, int dataId, int boxSize, int avgImgGray, 
        const MagicDIP::HaarFeature& feature)
    {
        double localAvg = ImgBoxValue(imgLoader, dataId, 0, 0, boxSize - 1, boxSize - 1) / 
            (double(boxSize * boxSize));
        double avgScale = avgImgGray / (localAvg + 0.1);
       // DebugLog << "localAvg: " << localAvg << " avgScale" << avgScale << std::endl;
        if (feature.type == 0)
        {
            int posValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol, 
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol / 2 - 1);
            int negValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol + feature.lCol / 2,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol - 1);
            double difRes = double(posValue - negValue) * avgScale / (feature.lRow * feature.lCol);
            return difRes;
        }
        else if (feature.type == 1)
        {
            int negValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol, 
                feature.sRow + feature.lRow / 2 - 1, feature.sCol + feature.lCol - 1);
            int posValue = ImgBoxValue(imgLoader, dataId, feature.sRow + feature.lRow / 2, feature.sCol,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol - 1);
            double difRes = double(posValue - negValue) * avgScale / (feature.lRow * feature.lCol);
            return difRes;
        }
        else if (feature.type == 2)
        {
            int posLeftValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol / 3 - 1);
            int posRightValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol + feature.lCol * 2 / 3,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol - 1);
            int negValue = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol + feature.lCol / 3,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol * 2 / 3 - 1);
            double difRes = double(posLeftValue + posRightValue - negValue * 2) * avgScale / (feature.lRow * feature.lCol);
            return difRes;
        }
        else if (feature.type == 3)
        {
            int posTopLeft = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol, 
                feature.sRow + feature.lRow / 2 - 1, feature.sCol + feature.lCol / 2 - 1);
            int posRightDown = ImgBoxValue(imgLoader, dataId, feature.sRow + feature.lRow / 2, feature.sCol + feature.lCol / 2,
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol - 1);
            int negRightTop = ImgBoxValue(imgLoader, dataId, feature.sRow, feature.sCol + feature.lCol / 2,
                feature.sRow + feature.lRow / 2 - 1, feature.sCol + feature.lCol - 1);
            int negLeftDown = ImgBoxValue(imgLoader, dataId, feature.sRow + feature.lRow / 2, feature.sCol, 
                feature.sRow + feature.lRow - 1, feature.sCol + feature.lCol / 2 - 1);
            double difRes = double(posTopLeft + posRightDown - negRightTop - negLeftDown) * avgScale / (feature.lRow * feature.lCol);
            return difRes;
        }
        else
        {
            DebugLog << "Error: wrong type: " << feature.type << std::endl;
            return 0;
        }
    }

    static unsigned int GetIntegralValue(const std::vector<unsigned int>& integralImg, int imgW, int hid, int wid)
    {
        int imgH = integralImg.size() / imgW;
        hid = hid >= imgH ? (imgH - 1) : hid;
        wid = wid >= imgW ? (imgW - 1) : wid;

        return integralImg.at(hid * imgW + wid);
    }

    static void CheckIntegralImage(const std::vector<unsigned int>& integralImg, const cv::Mat& img)
    {
        //DebugLog << "CheckIntegralImage" << std::endl;
        int imgH = img.rows;
        int imgW = img.cols;
        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                unsigned char gray = img.ptr(hid, wid)[0];
                int integralValue = 0;
                if (hid > 0 && wid > 0)
                {
                    integralValue = GetIntegralValue(integralImg, imgW, hid, wid) + GetIntegralValue(integralImg, imgW, hid - 1, wid - 1) -
                        GetIntegralValue(integralImg, imgW, hid - 1, wid) - GetIntegralValue(integralImg, imgW, hid, wid - 1);
                }
                else if (hid > 0 && wid == 0)
                {
                    integralValue = GetIntegralValue(integralImg, imgW, hid, wid) - GetIntegralValue(integralImg, imgW, hid - 1, wid);
                }
                else if (hid == 0 && wid > 0)
                {
                    integralValue = GetIntegralValue(integralImg, imgW, hid, wid) - GetIntegralValue(integralImg, imgW, hid, wid - 1);
                }
                else if (hid == 0 && wid == 0)
                {
                    integralValue = GetIntegralValue(integralImg, imgW, hid, wid);
                }
                else 
                {
                    DebugLog << "index error: " << hid << " " << wid << std::endl; 
                }
                int difV = gray - integralValue;
                if (difV != 0)
                {
                    DebugLog << "error: " << difV << " gray: " << int(gray) << " integral: " << integralValue << std::endl;
                }
                /*else
                {
                    DebugLog << "image" << imgId << ": dif: " << difV << " gray: " << int(gray) << " integral: " << integralValue <<
                        " " << hid << " " << wid << std::endl;
                }*/
            }
        }
    }

    static int ImgBoxValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, int eRow, int eCol)
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

    static double CalFeatureValue(const std::vector<unsigned int>& integralImg, int imgW, 
        int sRow, int sCol, int boxSize, float scale, int avgImgGray, const MagicDIP::HaarFeature& feature)
    {
        int lRow = floor(feature.lRow * scale + 0.5);
        int lCol = floor(feature.lCol * scale + 0.5);
        int sRowAbs = floor(feature.sRow * scale + 0.5) + sRow;
        int sColAbs = floor(feature.sCol * scale + 0.5) + sCol;
        double localAvg = ImgBoxValue(integralImg, imgW, sRow, sCol, sRow + boxSize - 1, sCol + boxSize - 1) / 
            (double(boxSize * boxSize));
        double avgScale = avgImgGray / (localAvg + 0.1);
        /*if (MagicTool::gLogOut)
        {
            DebugLog << "     Cal HaarFeature: " << imgW << " lrow: " << lRow << " lcol: " << lCol << " " << boxSize << " " << scale
                << " " << avgImgGray << " flrow: " << feature.lRow << " flcol: " << feature.lCol << std::endl;
        }*/
        if (feature.type == 0)
        {
            int posValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow - 1, sColAbs + lCol / 2 - 1);
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 2,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            double difRes = double(posValue - negValue) * avgScale / (lRow * lCol);
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "     type0: " << difRes << " "  << posValue / scale / scale << " " << negValue / scale / scale << " " << avgScale << std::endl;
            }*/
            return difRes;
        }
        else if (feature.type == 1)
        {
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow / 2 - 1, sColAbs + lCol - 1);
            int posValue = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            double difRes = double(posValue - negValue) * avgScale / (lRow * lCol);
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "     type1: " << difRes << " " << posValue / scale / scale << " " << negValue / scale / scale << " " << avgScale << std::endl;
            }*/
            return difRes;
        }
        else if (feature.type == 2)
        {
            int posLeftValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs,
                sRowAbs + lRow - 1, sColAbs + lCol / 3 - 1);
            int posRightValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol * 2 / 3,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 3,
                sRowAbs + lRow - 1, sColAbs + lCol * 2 / 3 - 1);
            double difRes = double(posLeftValue + posRightValue - negValue * 2) * avgScale / (lRow * lCol);
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "     type2: " << difRes << " " << posLeftValue / scale / scale << " " << posRightValue / scale / scale
                    << " " << negValue / scale / scale << " " << avgScale << std::endl;
            }*/
            return difRes;
        }
        else if (feature.type == 3)
        {
            int posTopLeft = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow / 2 - 1, sColAbs + lCol / 2 - 1);
            int posRightDown = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs + lCol / 2,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            int negRightTop = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 2,
                sRowAbs + lRow / 2 - 1, sColAbs + lCol - 1);
            int negLeftDown = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs, 
                sRowAbs + lRow - 1, sColAbs + lCol / 2 - 1);
            double difRes = double(posTopLeft + posRightDown - negRightTop - negLeftDown) * avgScale / (lRow * lCol);
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "     type3: " << difRes << " " << posTopLeft / scale / scale << " " << posRightDown / scale / scale << " "
                    << negRightTop / scale / scale << " " << negLeftDown / scale / scale << " " << avgScale << std::endl;
            }*/
            return difRes;
        }
        else
        {
            DebugLog << "Error: wrong type: " << feature.type << std::endl;
            return 0;
        }
    }

    class FaceFeatureCache
    {
    public:
        FaceFeatureCache();
        ~FaceFeatureCache();

        /*bool CalFeatureValues(const MagicDIP::ImageLoader& imgLoader, 
            const std::vector<MagicDIP::HaarClassifier*>& classifiers);*/
        bool CalFeatureValues(const MagicDIP::ImageLoader& imgLoader, const std::vector<int>& imgIndex, 
            const std::vector<MagicDIP::HaarClassifier*>& classifiers, int boxSize, int avgImgGray);
        float GetFeatureValue(int classifierId, int imgId);

    private:
        void Reset(void);

    private:
        float* mpFeatureValues;  //in order to save space, we use float here
        int mImageCount;
        //std::map<int, int> mIndexMap;
        std::vector<int> mIndexMap;
    };

    FaceFeatureCache::FaceFeatureCache() :
        mpFeatureValues(NULL),
        mImageCount(0),
        mIndexMap()
    {
    }

    FaceFeatureCache::~FaceFeatureCache()
    {
        Reset();
    }

    /*bool FaceFeatureCache::CalFeatureValues(const MagicDIP::ImageLoader& imgLoader, 
            const std::vector<MagicDIP::HaarClassifier*>& classifiers)
    {
        Reset();
        mImageCount = imgLoader.GetImageCount();
        long long classifierCount = classifiers.size();
        try
        {
            mpFeatureValues = new int[long long(mImageCount) * classifierCount];
        }
        catch(const std::bad_alloc& e)
        {
            if (mpFeatureValues != NULL)
            {
                delete mpFeatureValues;
                mpFeatureValues = NULL;
            }
            return false;
        }
        for (long long classifierId = 0; classifierId < classifierCount; classifierId++)
        {
            MagicDIP::HaarFeature feature = classifiers.at(classifierId)->GetFeature();
            long long baseIndex = classifierId * mImageCount;
            for (long long imgId = 0; imgId < mImageCount; imgId++)
            {
                mpFeatureValues[baseIndex + imgId] = CalFeatureValue(imgLoader, imgId, feature);
            }
        }
        return true;
    }*/

    bool FaceFeatureCache::CalFeatureValues(const MagicDIP::ImageLoader& imgLoader, const std::vector<int>& imgIndex, 
            const std::vector<MagicDIP::HaarClassifier*>& classifiers, int boxSize, int avgImgGray)
    {
        Reset();
        mImageCount = imgIndex.size();
        if (mImageCount < 10)
        {
            return false;
        }
        long long classifierCount = classifiers.size();
        try
        {
            mpFeatureValues = new float[static_cast<long long>(mImageCount) * classifierCount];
        }
        catch(const std::bad_alloc& e)
        {
            if (mpFeatureValues != NULL)
            {
                delete mpFeatureValues;
                mpFeatureValues = NULL;
            }
            return false;
        }
        //Construct index map
        mIndexMap = std::vector<int>(imgLoader.GetImageCount(), -1);
        for (int imgId = 0; imgId < mImageCount; imgId++)
        {
            mIndexMap[imgIndex.at(imgId)] = imgId;
        }
        for (long long classifierId = 0; classifierId < classifierCount; classifierId++)
        {
            MagicDIP::HaarFeature feature = classifiers.at(classifierId)->GetFeature();
            long long baseIndex = classifierId * mImageCount;
            for (long long imgId = 0; imgId < mImageCount; imgId++)
            {
                mpFeatureValues[baseIndex + imgId] = CalFeatureValue(imgLoader, imgIndex.at(imgId), boxSize, avgImgGray, feature);
            }
        }
        return true;
    }

    float FaceFeatureCache::GetFeatureValue(int classifierId, int imgId)
    {
        if (mIndexMap.size() == 0)
        {
            return mpFeatureValues[static_cast<long long>(classifierId) * mImageCount + static_cast<long long>(imgId)];
        }
        else
        {
            return mpFeatureValues[ static_cast<long long>(classifierId) * mImageCount + static_cast<long long>(mIndexMap[imgId]) ];
        }
    }

    void FaceFeatureCache::Reset(void)
    {
        if (mpFeatureValues != NULL)
        {
            delete mpFeatureValues;
            mpFeatureValues = NULL;
        }
        mImageCount = 0;
        mIndexMap.clear();
    }

    static FaceFeatureCache* gFaceFeatureCache = NULL;
    static FaceFeatureCache* gNonFaceFeatureCache = NULL;
}

namespace MagicDIP
{

    HaarClassifier::HaarClassifier() :
        mFeature(),
        mThreshold(0),
        mIsLess(true)
    {
    }

    HaarClassifier::HaarClassifier(int id, const HaarFeature& feature) :
        mId(id),
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
        const std::vector<int>& faceIndex, const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, 
        const std::vector<int>& nonFaceIndex, double* trainError, int boxSize, int avgImgGray)
    {
        int faceDataCount = faceDataWeights.size();
        std::vector<ValueIndex> faceFeatures(faceDataCount);
        double faceSum = 0;
        for (int faceId = 0; faceId < faceDataCount; faceId++)
        {
            faceFeatures.at(faceId).mValue = CalFeature(faceImgLoader, 1, faceIndex.at(faceId), boxSize, avgImgGray);
            faceFeatures.at(faceId).mIndex = faceId;
            faceSum += faceDataWeights.at(faceId);
        }
        std::sort(faceFeatures.begin(), faceFeatures.end());

        int nonFaceDataCount = nonFaceDataWeights.size();
        std::vector<ValueIndex> nonFaceFeatures(nonFaceDataCount);
        double nonFaceSum = 0;
        for (int nonFaceId = 0; nonFaceId < nonFaceDataCount; nonFaceId++)
        {
            nonFaceFeatures.at(nonFaceId).mValue = CalFeature(nonFaceImgLoader, 0, nonFaceIndex.at(nonFaceId), boxSize, avgImgGray);
            nonFaceFeatures.at(nonFaceId).mIndex = nonFaceId;
            nonFaceSum += nonFaceDataWeights.at(nonFaceId);
        }
        std::sort(nonFaceFeatures.begin(), nonFaceFeatures.end());

        double minError = DBL_MAX; //1.7976931348623158e+308
        int faceId = 0;
        double faceAccumulate = 0;
        int nonFaceId = 0;
        double nonFaceAccumulate = 0;
        double epsilon = 1.0e-5;
        while (faceId < faceDataCount || nonFaceId < nonFaceDataCount)
        {
            double threshold;
            if (faceId == faceDataCount)
            {
                threshold = nonFaceFeatures.at(nonFaceId).mValue + epsilon;
                while (nonFaceFeatures.at(nonFaceId).mValue < threshold)
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
                threshold = faceFeatures.at(faceId).mValue + epsilon;
                while (faceFeatures.at(faceId).mValue < threshold)
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
                    threshold = faceFeatures.at(faceId).mValue + epsilon;     
                    while (faceFeatures.at(faceId).mValue < threshold)
                    {
                        faceAccumulate += faceDataWeights.at(faceFeatures.at(faceId).mIndex);
                        faceId++;
                        if (faceId == faceDataCount)
                        {
                            break;
                        }
                    }
                    while (nonFaceFeatures.at(nonFaceId).mValue < threshold)
                    {
                        nonFaceAccumulate += nonFaceDataWeights.at(nonFaceFeatures.at(nonFaceId).mIndex);
                        nonFaceId++;
                        if (nonFaceId == nonFaceDataCount)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    threshold = nonFaceFeatures.at(nonFaceId).mValue + epsilon;
                    while (faceFeatures.at(faceId).mValue < threshold)
                    {
                        faceAccumulate += faceDataWeights.at(faceFeatures.at(faceId).mIndex);
                        faceId++;
                        if (faceId == faceDataCount)
                        {
                            break;
                        }
                    }
                    while (nonFaceFeatures.at(nonFaceId).mValue < threshold)
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
                mThreshold = threshold;
                mIsLess = true;
            }
            double largeError = faceAccumulate + nonFaceSum - nonFaceAccumulate;
            if (largeError < minError)
            {
                minError = largeError;
                mThreshold = threshold;
                mIsLess = false;
            }
        } 
        *trainError = minError;
        //double totalTime = MagicTool::Profiler::GetTime() - startTime;
        //DebugLog << "    weak learn total time: " << totalTime << " sortTime: " << sortTime / totalTime 
        //    << " featureTime: " << featureTime / totalTime << std::endl;
        
        return MAGIC_NO_ERROR;
    }
     
    int HaarClassifier::Predict(const std::vector<unsigned int>& integralImg, int imgW,
        int sRow, int sCol, int boxSize, float scale, int avgImgGray) const
    {
        double featureValue = CalFeatureValue(integralImg, imgW, sRow, sCol, boxSize, scale, avgImgGray, mFeature);    
        /*if (MagicTool::gLogOut)
        {
            DebugLog << "   Haar predict: feature value " << featureValue << " isLess: " << mIsLess << " mThreahold: " 
                << mThreshold << std::endl;
        }*/
        if (mIsLess)
        {
            return featureValue < mThreshold;
        }
        else
        {
            return featureValue >= mThreshold;
        }
    }

    int HaarClassifier::Predict(const ImageLoader& imgLoader, int dataId, int boxSize, int avgImgGray) const
    {
        double featureValue = CalFeatureValue(imgLoader, dataId, boxSize, avgImgGray, mFeature);
        if (mIsLess)
        {
            return featureValue < mThreshold;
        }
        else
        {
            return featureValue >= mThreshold;
        }
    }

    int HaarClassifier::CalFeature(const ImageLoader& imgLoader, int imgType, int dataId, int boxSize, int avgImgGray) const
    {
        if (imgType == 1) //face
        {
            if (gFaceFeatureCache != NULL)
            {
                return gFaceFeatureCache->GetFeatureValue(mId, dataId);
            }
            else
            {
                return CalFeatureValue(imgLoader, dataId, boxSize, avgImgGray, mFeature);
            }
        }
        else if (imgType == 0) //non face
        {
            if (gNonFaceFeatureCache != NULL)
            {
                return gNonFaceFeatureCache->GetFeatureValue(mId, dataId);
            }
            else
            {
                return CalFeatureValue(imgLoader, dataId, boxSize, avgImgGray, mFeature);
            }
        }
        else
        {
            DebugLog << "error: false imgType: " << imgType;
            return 0;
        }
    }

    /*int HaarClassifier::CalFeatureValue(const ImageLoader& imgLoader, int dataId) const
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
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol + mFeature.lCol / 2 - 1);
            int posRightDown = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2, mFeature.sCol + mFeature.lCol / 2,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            int negRightTop = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol / 2,
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol + mFeature.lCol - 1);
            int negLeftDown = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2, mFeature.sCol, 
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
    }*/

    /*int HaarClassifier::CalFeatureValue(const std::vector<unsigned int>& integralImg, int imgW, int sRow, int sCol, float scale) const
    {
        int lRow = floor(mFeature.lRow * scale + 0.5);
        int lCol = floor(mFeature.lCol * scale + 0.5);
        int sRowAbs = floor(mFeature.sRow * scale + 0.5) + sRow;
        int sColAbs = floor(mFeature.sCol * scale + 0.5) + sCol;
        if (mFeature.type == 0)
        {
            int posValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow - 1, sColAbs + lCol / 2 - 1);
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 2,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            return (posValue - negValue) / (lRow * lCol / 2);
        }
        else if (mFeature.type == 1)
        {
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow / 2 - 1, sColAbs + lCol - 1);
            int posValue = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            return (posValue - negValue) / (lRow * lCol / 2);
        }
        else if (mFeature.type == 2)
        {
            int posLeftValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs,
                sRowAbs + lRow - 1, sColAbs + lCol / 3 - 1);
            int posRightValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol * 2 / 3,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            int negValue = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 3,
                sRowAbs + lRow - 1, sColAbs + lCol * 2 / 3 - 1);
            return (posLeftValue + posRightValue - negValue) / (lRow * lCol / 3);
        }
        else if (mFeature.type == 3)
        {
            int posTopLeft = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs, 
                sRowAbs + lRow / 2 - 1, sColAbs + lCol / 2 - 1);
            int posRightDown = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs + lCol / 2,
                sRowAbs + lRow - 1, sColAbs + lCol - 1);
            int negRightTop = ImgBoxValue(integralImg, imgW, sRowAbs, sColAbs + lCol / 2,
                sRowAbs + lRow / 2 - 1, sColAbs + lCol - 1);
            int negLeftDown = ImgBoxValue(integralImg, imgW, sRowAbs + lRow / 2, sColAbs, 
                sRowAbs + lRow - 1, sColAbs + lCol / 2 - 1);
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
    }*/

    /*unsigned int HaarClassifier::GetIntegralValue(const std::vector<unsigned int>& integralImg, int imgW, int hid, int wid) const
    {
        int imgH = integralImg.size() / imgW;
        hid = hid >= imgH ? (imgH - 1) : hid;
        wid = wid >= imgW ? (imgW - 1) : wid;

        return integralImg.at(hid * imgW + wid);
    }*/

    void HaarClassifier::Save(std::ofstream& fout) const
    {
        fout << mFeature.sRow << " " << mFeature.sCol << " " << mFeature.lRow << " " << mFeature.lCol << " " << mFeature.type 
            << " " << mThreshold << " " << mIsLess << std::endl;
    }
        
    void HaarClassifier::Load(std::ifstream& fin)
    {
        fin >> mFeature.sRow >> mFeature.sCol >> mFeature.lRow >> mFeature.lCol >> mFeature.type >> mThreshold >> mIsLess;
    }

    double HaarClassifier::CalFeatureSimilarity(const HaarFeature& hf) const
    {
        if (mFeature.type != hf.type)
        {
            return 0;
        }
        int wOverlap = CalCulateLineSegmentOverlap(mFeature.sCol, mFeature.lCol, hf.sCol, hf.lCol);
        int hOverlap = CalCulateLineSegmentOverlap(mFeature.sRow, mFeature.lRow, hf.sRow, hf.lRow);
        float overlapArea = float(wOverlap * hOverlap);
        float areaA = float(mFeature.lCol * mFeature.lRow);
        float areaB = float(hf.lCol * hf.lRow);
        float simA = overlapArea / areaA;
        float simB = overlapArea / areaB;
        return (simA < simB ? simA : simB);
    }

    void HaarClassifier::SaveFeatureAsImage(const std::string& imgName, int baseSize) const
    {
        cv::Mat featureImg(baseSize, baseSize, CV_8UC1, cv::Scalar(128));
        if (mFeature.type == 0)
        {
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol / 2; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol + mFeature.lCol / 2; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 0;
                }
            }
        }
        else if (mFeature.type == 1)
        {
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow / 2; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 0;
                }
            }
            for (int hid = mFeature.sRow + mFeature.lRow / 2; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
        }
        else if (mFeature.type == 2)
        {
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol / 3; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol + mFeature.lCol * 2 / 3; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol + mFeature.lCol / 3; wid < mFeature.sCol + mFeature.lCol * 2 / 3; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 0;
                }
            }
        }
        else if (mFeature.type == 3)
        {
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow / 2; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol / 2; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
            for (int hid = mFeature.sRow + mFeature.lRow / 2; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol + mFeature.lCol / 2; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 255;
                }
            }
            for (int hid = mFeature.sRow; hid < mFeature.sRow + mFeature.lRow / 2; hid++)
            {
                for (int wid = mFeature.sCol + mFeature.lCol / 2; wid < mFeature.sCol + mFeature.lCol; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 0;
                }
            }
            for (int hid = mFeature.sRow + mFeature.lRow / 2; hid < mFeature.sRow + mFeature.lRow; hid++)
            {
                for (int wid = mFeature.sCol; wid < mFeature.sCol + mFeature.lCol / 2; wid++)
                {
                    featureImg.ptr(hid, wid)[0] = 0;
                }
            }
        }
        else
        {
            DebugLog << "error: mFeature.type == " << mFeature.type << std::endl;
        }

        cv::imwrite(imgName, featureImg);
        featureImg.release();
    }

    AdaBoostFaceDetection::AdaBoostFaceDetection() :
        mDetectionRate(0.999),
        mClassifiers(),
        mClassifierWeights(),
        mThreshold(0),
        mClassifierCandidates()
    {
    }

    AdaBoostFaceDetection::AdaBoostFaceDetection(double detectionRate) :
        mDetectionRate(detectionRate),
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

    int AdaBoostFaceDetection::Learn(const ImageLoader& faceImgLoader, const std::vector<bool>& faceValidFlag,
        const ImageLoader& nonFaceImgLoader, const std::vector<bool>& nonFaceValidFlag, int levelCount, 
        int boxSize, int avgImgGray)
    {
        Reset();
        mClassifiers.reserve(levelCount);
        mClassifierWeights.reserve(levelCount);
        
        //int faceCount = faceImgLoader.GetImageCount();
        //std::vector<double> faceWeights(faceCount, 0.5 / faceCount);
        std::vector<int> faceIndex;
        for (int faceId = 0; faceId < faceValidFlag.size(); faceId++)
        {
            if (faceValidFlag.at(faceId))
            {
                faceIndex.push_back(faceId);
            }
        }
        int faceCount = faceIndex.size();
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

        //Generate Feature Value Cache
        long long nonFaceCacheSize = static_cast<long long>(nonFaceCount) * mClassifierCandidates.size() * 4;
        DebugLog << "nonFaceCacheSize: " << nonFaceCacheSize << std::endl;
        GenerateFeatureValueCache(&faceImgLoader, faceIndex, &nonFaceImgLoader, nonFaceIndex, boxSize, avgImgGray);

        std::vector<int> faceResFlag(faceCount);
        std::vector<int> nonFaceResFlag(nonFaceCount);
        double epsilon = 1.0e-10;
        //mThreshold = 0.0;
        bool isValidCandidateExist = true;
        for (int levelId = 0; levelId < levelCount; levelId++)
        {
            if (!isValidCandidateExist)
            {
                break;
            }
            double levelTime = MagicTool::Profiler::GetTime();
            DebugLog << " AdaBoost level: " << levelId << std::endl;
            int weakClassifierId = TrainWeakClassifier(faceImgLoader, faceWeights, faceIndex, nonFaceImgLoader, nonFaceWeights,
                nonFaceIndex, boxSize, avgImgGray);
            HaarClassifier* pWeakClassifier = NULL;
            if (weakClassifierId != -1)
            {
                pWeakClassifier = mClassifierCandidates.at(weakClassifierId);
                HaarFeature hf = pWeakClassifier->GetFeature();
                DebugLog << "  Haar feature: " << hf.sRow << " " << hf.sCol << " " << hf.lRow << " " << hf.lCol << " " << hf.type << std::endl;
                mClassifiers.push_back(pWeakClassifier);
                mClassifierCandidates.at(weakClassifierId) = NULL;
                int validCount = RemoveSimilarClassifierCandidates(hf);
                if (validCount == 0)
                {
                    isValidCandidateExist = false;
                    DebugLog << "There is no valid classifier candidate now" << std::endl;
                }
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
                faceResFlag.at(faceId) = abs(1 - pWeakClassifier->Predict(faceImgLoader, faceIndex.at(faceId), boxSize, avgImgGray));
                trainingError += faceWeights.at(faceId) * faceResFlag.at(faceId);
                if (faceResFlag.at(faceId) == 0)
                {
                    detectedFaceNum++;
                }
            }
            DebugLog << "  Detect Face Right: " << detectedFaceNum << " faceCount: " << faceCount 
                << " pro: " << double(detectedFaceNum) / double(faceCount) << std::endl;
            int detectedNonFaceNum = 0;
            for (int nonFaceId = 0; nonFaceId < nonFaceCount; nonFaceId++)
            {
                nonFaceResFlag.at(nonFaceId) = pWeakClassifier->Predict(nonFaceImgLoader, nonFaceIndex.at(nonFaceId), boxSize, avgImgGray);
                trainingError += nonFaceWeights.at(nonFaceId) * nonFaceResFlag.at(nonFaceId);
                if (nonFaceResFlag.at(nonFaceId) == 0)
                {
                    detectedNonFaceNum++;
                }
            }
            DebugLog << "  Detect Non-Face Right: " << detectedNonFaceNum << " nonFaceCount: " << nonFaceCount 
                << " pro: " << double(detectedNonFaceNum) / double(nonFaceCount) << std::endl;
            DebugLog << "  training error: " << trainingError << std::endl;
            /*if (trainingError < 0.5)
            {
                DebugLog << "  Weak Classifier " << levelId << ": " << trainingError << std::endl;
            }*/

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
            //DebugLog << "  weight: " << weight << " beta: " << beta << std::endl;
            mClassifierWeights.push_back(weight);
            //mThreshold += weight;

            //Update dataWeights
            double weightSum = 0.0;
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                faceWeights.at(faceId) *= pow(beta, 1.0 - faceResFlag.at(faceId));
                weightSum += faceWeights.at(faceId);
            }
            for (int nonFaceId = 0; nonFaceId < nonFaceCount; nonFaceId++)
            {
                nonFaceWeights.at(nonFaceId) *= pow(beta, 1.0 - nonFaceResFlag.at(nonFaceId));
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
            DebugLog << "  time: " << MagicTool::Profiler::GetTime() - levelTime << std::endl;
        }
        //mThreshold *= 0.5;
        //Calculate mThreshold
        std::vector<double> faceDetectValues(faceCount);
        int thresholdIndex = floor(faceCount * (1.0 - mDetectionRate) + 0.5);
        for (int faceId = 0; faceId < faceCount; faceId++)
        {
            double res = 0.0;
            for (int cid = 0; cid < mClassifiers.size(); cid++)
            {
                res += mClassifiers.at(cid)->Predict(faceImgLoader, faceIndex.at(faceId), boxSize, avgImgGray) * mClassifierWeights.at(cid);
            }
            faceDetectValues.at(faceId) = res;
        }
        std::nth_element(faceDetectValues.begin(), faceDetectValues.begin() + thresholdIndex, faceDetectValues.end());
        mThreshold = faceDetectValues.at(thresholdIndex);
        mThreshold -= 1.0e-15;
        ClearClassifierCadidates();
        DebugLog << "  mThreshold: " << mThreshold << " index: " << thresholdIndex << " mDetectionRate: " << mDetectionRate << std::endl;
        
        //Free Feature Cache
        ClearFeatureValueCache();     
        
        return MAGIC_NO_ERROR;
    }
    
    int AdaBoostFaceDetection::Predict(const std::vector<unsigned int>& integralImg, int imgW, 
        int sRow, int sCol, int boxSize, double scale, int avgImgGray) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            int predictRes = mClassifiers.at(cid)->Predict(integralImg, imgW, sRow, sCol, boxSize, scale, avgImgGray);
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "  weak: " << cid << " predictRes: " << predictRes << " w: " << mClassifierWeights.at(cid) << std::endl;
            }*/
            res += predictRes * mClassifierWeights.at(cid);           
        }
        /*if (MagicTool::gLogOut)
        {
            DebugLog << " boost res: " << res << " mThreshold: " << mThreshold << std::endl;
        }*/
        if (res > mThreshold)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    int AdaBoostFaceDetection::Predict(const ImageLoader& imgLoader, int dataId, int boxSize, int avgImgGray) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            int predictRes = mClassifiers.at(cid)->Predict(imgLoader, dataId, boxSize, avgImgGray);
            res += predictRes * mClassifierWeights.at(cid);
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

    void AdaBoostFaceDetection::SaveFeatureAsImage(const std::string& filePath, int baseSize, int detectorId) const
    {
        for (int classifierId = 0; classifierId < mClassifiers.size(); classifierId++)
        {
            std::stringstream ss;
            ss << filePath << "feature_" << detectorId << "_" << classifierId << ".jpg";
            std::string imgName;
            ss >> imgName;
            ss.clear();
            mClassifiers.at(classifierId)->SaveFeatureAsImage(imgName, baseSize);
        }
    }

    std::vector<int> AdaBoostFaceDetection::SampleHaarFeatures(const std::vector<HaarFeature> features, 
        double sampleRate) const
    {
        //double timeStart = MagicCore::ToolKit::GetTime();

        int featureCount = features.size();
        int sampleCount = int(featureCount * sampleRate);
        std::vector<bool> sampleFlag(featureCount, 0);
        std::vector<int> sampleIndex(sampleCount);
        int startIndex = rand() % featureCount;
        sampleFlag.at(startIndex) = true;
        sampleIndex.at(0) = startIndex;
        std::vector<int> minDist(featureCount, 1.0e10);
        int curIndex = startIndex;

        for (int sid = 1; sid < sampleCount; ++sid)
        {
           // MagicMath::Vector3 curPos = pPS->GetPoint(curIndex)->GetPosition();
            HaarFeature curFeature = features.at(curIndex);
            int maxDist = -1;
            int pos = -1;
            for (int vid = 0; vid < featureCount; ++vid)
            {
                if (sampleFlag.at(vid) == 1)
                {
                    continue;
                }
                //int dist = (pPS->GetPoint(vid)->GetPosition() - curPos).LengthSquared();
                HaarFeature vFeature = features.at(vid);
                int dist = (vFeature.sRow - curFeature.sRow) * (vFeature.sRow - curFeature.sRow) + 
                    (vFeature.sCol - curFeature.sCol) * (vFeature.sCol - curFeature.sCol) + 
                    (vFeature.lRow - curFeature.lRow) * (vFeature.lRow - curFeature.lRow) + 
                    (vFeature.lCol - curFeature.lCol) * (vFeature.lCol - curFeature.lCol);
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
        //DebugLog << "SampleHaarFeatures time: " << MagicCore::ToolKit::GetTime() - timeStart << std::endl;
        return sampleIndex;
    }

    void AdaBoostFaceDetection::GenerateClassifierCadidates(int baseImgSize)
    {
        std::vector<std::vector<HaarFeature> > features(4);
        int minLen = 5;  //modify_flag
        for (int sRow = 0; sRow < baseImgSize; sRow += 1)
        {
            for (int sCol = 0; sCol < baseImgSize; sCol += 1)
            {
                int colMaxLen = baseImgSize - sCol;
                int rowMaxLen = baseImgSize - sRow;

                for (int lRow = minLen; lRow <= rowMaxLen; lRow += 1)
                {
                    for (int lCol = minLen * 2; lCol <= colMaxLen; lCol += 2)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 0};
                        features.at(0).push_back(feature);
                    }
                }

                for (int lRow = minLen * 2; lRow <= rowMaxLen; lRow += 2)
                {
                    for (int lCol = minLen; lCol <= colMaxLen; lCol += 1)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 1};
                        features.at(1).push_back(feature);
                    }
                }

                for (int lRow = minLen; lRow <= rowMaxLen; lRow += 1)
                {
                    for (int lCol = minLen * 3; lCol <= colMaxLen; lCol += 3)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 2};
                        features.at(2).push_back(feature);
                    }
                }

                for (int lRow = minLen * 2; lRow <= rowMaxLen; lRow += 2)
                {
                    for (int lCol = minLen * 2; lCol <= colMaxLen; lCol += 2)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 3};
                        features.at(3).push_back(feature);
                    }
                }
            }
        }
        double sampleRate = 0.05;
        //int imgId = 0;
        int classifierId = 0;
        for (int typeId = 0; typeId < 4; typeId++)
        {
            std::vector<int> samples = SampleHaarFeatures(features.at(typeId), sampleRate);
            for (std::vector<int>::iterator itr = samples.begin(); itr != samples.end(); itr++)
            {
                HaarClassifier* pClassifier = new HaarClassifier(classifierId, features.at(typeId).at(*itr));
                classifierId++;
                mClassifierCandidates.push_back(pClassifier);
                
                /*std::stringstream ss;
                ss << "./FeatureCandidates/feature_" << imgId << ".jpg";
                std::string imgName;
                ss >> imgName;
                ss.clear();
                pClassifier->SaveFeatureAsImage(imgName, baseImgSize);
                imgId++;*/
            }
        }
        
        DebugLog << "GenerateClassifierCadidates: " << mClassifierCandidates.size() << std::endl;
    }

    void AdaBoostFaceDetection::GenerateFeatureValueCache(const ImageLoader* pFaceImgLoader, const std::vector<int>& faceIndex,
        const ImageLoader* pNonFaceImgLoader, const std::vector<int>& nonFaceIndex, int boxSize, int avgImgGray) const
    {
        DebugLog << "GenerateFeatureValueCache: boxSize: " << boxSize << " avgImgGray: " << avgImgGray << std::endl; 
        if (pFaceImgLoader != NULL)
        {
            if (gFaceFeatureCache != NULL)
            {
                delete gFaceFeatureCache;
                gFaceFeatureCache = NULL;
            }
            gFaceFeatureCache = new FaceFeatureCache;
            bool res = gFaceFeatureCache->CalFeatureValues(*pFaceImgLoader, faceIndex, mClassifierCandidates, boxSize, avgImgGray);
            if (res)
            {
                DebugLog << "face feature cache is success" << std::endl;
            }
            else
            {
                delete gFaceFeatureCache;
                gFaceFeatureCache = NULL;
                DebugLog << "face feature cache failed" << std::endl;
            }
        }
        
        if (pNonFaceImgLoader != NULL)
        {
            if (gNonFaceFeatureCache != NULL)
            {
                delete gNonFaceFeatureCache;
                gNonFaceFeatureCache = NULL;
            }
            gNonFaceFeatureCache = new FaceFeatureCache;
            bool res = gNonFaceFeatureCache->CalFeatureValues(*pNonFaceImgLoader, nonFaceIndex, mClassifierCandidates, boxSize, avgImgGray);
            if (res)
            {
                DebugLog << "nonface feature cache is success" << std::endl;
            }
            else
            {
                delete gNonFaceFeatureCache;
                gNonFaceFeatureCache = NULL;
                DebugLog << "nonface feature cache failed" << std::endl;
            }
        }
    }
        
    void AdaBoostFaceDetection::ClearFeatureValueCache(void) const
    {
        if (gFaceFeatureCache != NULL)
        {
            delete gFaceFeatureCache;
            gFaceFeatureCache = NULL;
        }
        if (gNonFaceFeatureCache != NULL)
        {
            delete gNonFaceFeatureCache;
            gNonFaceFeatureCache = NULL;
        }
    }

    //void AdaBoostFaceDetection::GenerateClassifierCadidates(int baseImgSize)
    //{
    //    //Enhancement: use uniform sampling strategy.
    //    for (int sRow = 0; sRow < baseImgSize; sRow += 2)
    //    {
    //        for (int sCol = 0; sCol < baseImgSize; sCol += 2)
    //        {
    //            int colMaxLen = baseImgSize - sCol;
    //            int rowMaxLen = baseImgSize - sRow;

    //            for (int lRow = 2; lRow <= rowMaxLen; lRow += 2)
    //            {
    //                for (int lCol = 4; lCol <= colMaxLen; lCol += 4)
    //                {
    //                    HaarFeature feature = {sRow, sCol, lRow, lCol, 0};
    //                    HaarClassifier* pClassifier = new HaarClassifier(feature);
    //                    mClassifierCandidates.push_back(pClassifier);
    //                }
    //            }

    //            for (int lRow = 4; lRow <= rowMaxLen; lRow += 4)
    //            {
    //                for (int lCol = 2; lCol <= colMaxLen; lCol += 2)
    //                {
    //                    HaarFeature feature = {sRow, sCol, lRow, lCol, 1};
    //                    HaarClassifier* pClassifier = new HaarClassifier(feature);
    //                    mClassifierCandidates.push_back(pClassifier);
    //                }
    //            }

    //            for (int lRow = 2; lRow <= rowMaxLen; lRow += 2)
    //            {
    //                for (int lCol = 6; lCol <= colMaxLen; lCol += 6)
    //                {
    //                    HaarFeature feature = {sRow, sCol, lRow, lCol, 2};
    //                    HaarClassifier* pClassifier = new HaarClassifier(feature);
    //                    mClassifierCandidates.push_back(pClassifier);
    //                }
    //            }

    //            for (int lRow = 4; lRow <= rowMaxLen; lRow += 4)
    //            {
    //                for (int lCol = 4; lCol <= colMaxLen; lCol += 4)
    //                {
    //                    HaarFeature feature = {sRow, sCol, lRow, lCol, 3};
    //                    HaarClassifier* pClassifier = new HaarClassifier(feature);
    //                    mClassifierCandidates.push_back(pClassifier);
    //                }
    //            }
    //        }
    //    }
    //    DebugLog << "GenerateClassifierCadidates: " << mClassifierCandidates.size() << std::endl;
    //}

    int AdaBoostFaceDetection::RemoveSimilarClassifierCandidates(const HaarFeature& hf)
    {
        double similarThreshold = 0.7;
        int validCandCount = 0;
        for (std::vector<HaarClassifier*>::iterator itr = mClassifierCandidates.begin(); itr != mClassifierCandidates.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                if ((*itr)->CalFeatureSimilarity(hf) > similarThreshold)
                {
                    delete (*itr);
                    (*itr) = NULL;
                }
                else
                {
                    validCandCount++;
                }
            }
        }
        DebugLog << "  valid candidate count: " << validCandCount << std::endl;
        return validCandCount;
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

    int AdaBoostFaceDetection::TrainWeakClassifier(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
        const std::vector<int>& faceIndex, const ImageLoader& nonFaceImgLoader, 
        const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex,
        int boxSize, int avgImgGray)
    {
        double minTrainError = 1.0e308;
        int classifierId = -1;
        for (int cid = 0; cid < mClassifierCandidates.size(); cid++)
        {
            HaarClassifier* pClassifier = mClassifierCandidates.at(cid);
            if (pClassifier != NULL)
            {
                double trainError;
                int res = pClassifier->Learn(faceImgLoader, faceDataWeights, faceIndex, nonFaceImgLoader, 
                    nonFaceDataWeights, nonFaceIndex, &trainError, boxSize, avgImgGray);
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
        DebugLog << "  Min train error: " << minTrainError << std::endl;
        return classifierId;
    }

    void AdaBoostFaceDetection::Reset(void)
    {
        //mDetectionRate = 0.999;
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
        mAvgImgGray(128),
        mCascadedDetectors()
    {
    }

    RealTimeFaceDetection::~RealTimeFaceDetection()
    {
        Reset();
    }

    int RealTimeFaceDetection::Learn(const std::vector<std::string>& faceImages, const std::vector<std::string>& nonFaceImages)
    {
        if (faceImages.size() == 0 || nonFaceImages.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }

        Reset();
        //int stageCount = layerCounts.size();
        int stageCount = 50; //modify_flag
        mCascadedDetectors.reserve(stageCount);

        ImageLoader faceImgLoader;
        faceImgLoader.LoadImages(faceImages, ImageLoader::IT_Gray);
        faceImgLoader.GenerateIntegralImage();
        faceImgLoader.TestIntegralImage();
        faceImgLoader.ClearImageData();
        DebugLog << "face loaded" << std::endl;

        mBaseImgSize = faceImgLoader.GetImageWidth(0);
        
        int originalNonFaceCount = nonFaceImages.size();
        ImageLoader nonFaceImgLoader;
        nonFaceImgLoader.LoadImages(nonFaceImages, ImageLoader::IT_Gray);
        nonFaceImgLoader.GenerateIntegralImage();
        nonFaceImgLoader.TestIntegralImage();
        nonFaceImgLoader.ClearImageData();
        DebugLog << "non face loaded" << std::endl;
        
        std::vector<bool> faceValidFlag(faceImages.size(), 1);
        std::vector<bool> nonFaceValidFlag(nonFaceImages.size(), 1);
        
        DebugLog << "Learn Cascaded detectors..." << std::endl;
        
        srand(time(NULL)); //sample feature

        int curStageLevelCount = 8;
        int levelCountDelta = 11;  //modify_flag
        int maxStageLevelCount = 200;
        int restartLevelCount = 50;        
        int maxTryNum = 1;  //modify_flag
        int maxPassNum = 3; //modify_flag
        int nonFaceBreakCount = originalNonFaceCount * 0.001;  //modify_flag
        int nonFaceExportCount = originalNonFaceCount * 0.2;  //modify_flag
        for (int stageId = 0; stageId < stageCount; stageId++)
        {
            //Save temp
            std::string tempFileName("./temp.abfd");
            Save(tempFileName);
            //
            if (curStageLevelCount == maxStageLevelCount || stageId >= 5) //modify_flag
            {
                curStageLevelCount = restartLevelCount + rand() % (maxStageLevelCount - restartLevelCount);
                DebugLog << "Stage " << stageId << " random level count: " << curStageLevelCount << std::endl;
            }
            double acceptNonFaceDetectRate = 0.15;  //modify_flag
            int tryNum = maxTryNum;
            int passNum = maxPassNum;
            bool isEmptyInput = false;
            double avgNonFaceDetectRate = 0.0;
            int detectRateTryNum = 0;
            double stageTime = MagicTool::Profiler::GetTime();
            while (true)
            {
                DebugLog << "Stage " << stageId << " level count: " << curStageLevelCount << std::endl;
                AdaBoostFaceDetection* pDetector = new AdaBoostFaceDetection(0.999); //modify_flag
                int res = pDetector->Learn(faceImgLoader, faceValidFlag, nonFaceImgLoader, nonFaceValidFlag, 
                    curStageLevelCount, mBaseImgSize, mAvgImgGray);
                if (res != MAGIC_NO_ERROR)
                {
                    if (res == MAGIC_EMPTY_INPUT)
                    {
                        DebugLog << "Stage: " << stageId << " is empty input, break" << std::endl;
                        delete pDetector;
                        isEmptyInput = true;
                        break;
                    }
                    Reset();
                    DebugLog << "stage " << stageId << " learn failed" << std::endl;
                    return MAGIC_INVALID_RESULT;
                }
            
                //filter non faces
                //int nonFaceDetectCount = 0;

                std::vector<int> nonFaceDetectIndex;
                int validNonFaceCount = 0;
                for (int nonFaceId = 0; nonFaceId < nonFaceValidFlag.size(); nonFaceId++)
                {
                    if (nonFaceValidFlag.at(nonFaceId))
                    {
                        validNonFaceCount++;
                        if (pDetector->Predict(nonFaceImgLoader, nonFaceId, mBaseImgSize, mAvgImgGray) == 0)
                        {
                            nonFaceDetectIndex.push_back(nonFaceId);
                        }
                    }
                }
                //output non-detect non-face to local file
                if (validNonFaceCount < nonFaceExportCount)
                {
                    for (int nonFaceId = 0; nonFaceId < nonFaceValidFlag.size(); nonFaceId++)
                    {
                        cv::Mat falseNonFaceImg = cv::imread(nonFaceImages.at(nonFaceId));
                        std::stringstream ss;
                        if (!nonFaceValidFlag.at(nonFaceId))
                        {
                            ss << "./ValidNonFace/validNonFace_" << nonFaceId << ".jpg";
                        }
                        else
                        {
                            ss << "./FalseNonFace/falseNonFace_" << nonFaceId << ".jpg";
                        }      
                        std::string outFalseNonFaceName;
                        ss >> outFalseNonFaceName;
                        ss.clear();
                        cv::imwrite(outFalseNonFaceName, falseNonFaceImg);
                        falseNonFaceImg.release();        
                    }
                    nonFaceExportCount = 0;
                }
                //
                if (validNonFaceCount < nonFaceBreakCount)
                {
                    isEmptyInput = true;
                    delete pDetector;
                    pDetector = NULL;
                    break;
                }
                double nonFaceDetectRate = double(nonFaceDetectIndex.size()) / (validNonFaceCount + 0.1);
                avgNonFaceDetectRate += nonFaceDetectRate;
                detectRateTryNum++;
                DebugLog << "Stage: " << stageId << "  nonFaceDetectRate: " << nonFaceDetectRate << std::endl;
                if (nonFaceDetectRate > acceptNonFaceDetectRate)
                {
                    mCascadedDetectors.push_back(pDetector);
                    pDetector->SaveFeatureAsImage("./Features/", mBaseImgSize, stageId);
                    for (int nonFaceDetectId = 0; nonFaceDetectId < nonFaceDetectIndex.size(); nonFaceDetectId++)
                    {
                        nonFaceValidFlag.at(nonFaceDetectIndex.at(nonFaceDetectId)) = 0;
                    }
                    //update face
                    std::vector<int> faceDetectFalseIndex;
                    for (int faceId = 0; faceId < faceValidFlag.size(); faceId++)
                    {
                        if (faceValidFlag.at(faceId))
                        {
                            if (pDetector->Predict(faceImgLoader, faceId, mBaseImgSize, mAvgImgGray) == 0)
                            {
                                faceDetectFalseIndex.push_back(faceId);
                            }
                        }
                    }
                    for (int faceFalseId = 0; faceFalseId < faceDetectFalseIndex.size(); faceFalseId++)
                    {
                        faceValidFlag.at(faceDetectFalseIndex.at(faceFalseId)) = 0;
                    }
                    //output false face to local file
                    for (int faceFalseId = 0; faceFalseId < faceDetectFalseIndex.size(); faceFalseId++)
                    {
                        cv::Mat falseFaceImg = cv::imread(faceImages.at(faceDetectFalseIndex.at(faceFalseId)));
                        std::stringstream ss;
                        ss << "./FalseFace/falseFace_" << stageId << "_" << faceFalseId << ".jpg";
                        std::string outFalseFaceName;
                        ss >> outFalseFaceName;
                        ss.clear();
                        cv::imwrite(outFalseFaceName, falseFaceImg);
                        falseFaceImg.release();
                    }
                    //
                    break;
                }
                else
                {
                    DebugLog << "Stage: " << stageId << " few detect non-face: " << nonFaceDetectIndex.size()
                        << " " << validNonFaceCount << " rate: " << nonFaceDetectRate 
                        << " avgRate: " << avgNonFaceDetectRate / detectRateTryNum << std::endl;             
                    if (tryNum > 0)
                    {
                        delete pDetector;
                        pDetector = NULL;
                        tryNum--;
                        DebugLog << "    continue to try: " << tryNum << std::endl;
                        continue;
                    }
                    else
                    {
                        if (passNum > 0)
                        {
                            delete pDetector;
                            pDetector = NULL;
                            passNum--;
                            tryNum = maxTryNum;
                            curStageLevelCount += levelCountDelta;
                            curStageLevelCount = curStageLevelCount > maxStageLevelCount ? maxStageLevelCount : curStageLevelCount;
                            DebugLog << "    add level count: " << curStageLevelCount << ", continue to try: " << tryNum << std::endl;
                            continue;
                        }
                        else
                        {
                            delete pDetector;
                            pDetector = NULL;
                            acceptNonFaceDetectRate = avgNonFaceDetectRate / detectRateTryNum;
                            avgNonFaceDetectRate = 0;
                            detectRateTryNum = 0;
                            passNum = 0;
                            tryNum = maxTryNum * maxPassNum;
                            DebugLog << "Stage: " << stageId << " finally decrease detect rate: " << acceptNonFaceDetectRate << std::endl;
                            continue;
                           /* for (int nonFaceDetectId = 0; nonFaceDetectId < nonFaceDetectIndex.size(); nonFaceDetectId++)
                            {
                                nonFaceValidFlag.at(nonFaceDetectIndex.at(nonFaceDetectId)) = 0;
                            }
                            for (int faceFalseId = 0; faceFalseId < faceDetectFalseIndex.size(); faceFalseId++)
                            {
                                faceValidFlag.at(faceDetectFalseIndex.at(faceFalseId)) = 0;
                            }
                            mCascadedDetectors.push_back(pDetector);*/
                            //break;
                        }
                    }
                }
            }
            if (isEmptyInput)
            {
                break;
            }
            DebugLog << "time: " << MagicTool::Profiler::GetTime() - stageTime << std::endl;
        }

        if (mCascadedDetectors.size() == 0)
        {
            return MAGIC_INVALID_RESULT;
        }

        //export valid face file
        int finalValidId = 0;
        for (int faceId = 0; faceId < faceValidFlag.size(); faceId++)
        {
            if (faceValidFlag.at(faceId))
            {
                cv::Mat finalValidFace = cv::imread(faceImages.at(faceId));
                std::stringstream ss;
                ss << "./ValidFace/validFace_" << finalValidId << ".jpg";
                finalValidId++;
                std::string validFaceName;
                ss >> validFaceName;
                ss.clear();
                cv::imwrite(validFaceName, finalValidFace);
                finalValidFace.release();
            }
        }
        //

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
        //CheckIntegralImage(integralImg, img);
        int maxSubH = imgH;
        int maxSubW = imgW;
        while (curSubImgSize <= maxSubH && curSubImgSize <= maxSubW)
        {
            int maxRow = imgH - curSubImgSize;
            int maxCol = imgW - curSubImgSize;
            for (int rid = 0; rid <= maxRow; rid += curStep)
            {
                for (int cid = 0; cid <= maxCol; cid += curStep)
                {
                    if (DetectOneFace(integralImg, imgW, rid, cid, curSubImgSize, curScale, mAvgImgGray))
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
            //break;
        }
        //DebugLog << "  Post-Process faces: " << faces.size() / 4 << std::endl;
        //PostProcessFaces(faces);
        return (faces.size() / 4);
    }

    int RealTimeFaceDetection::DetectSpecialLocation(const cv::Mat& img, int rid, int cid, int subSize, double scale, std::vector<int>& faces) const
    {
        faces.clear();
        int imgW = img.cols;
        std::vector<unsigned int> integralImg;
        ImageLoader::TransferToIntegralImg(img, integralImg);
        int res = DetectOneFace(integralImg, imgW, rid, cid, subSize, scale, mAvgImgGray);
        return res;
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

    void RealTimeFaceDetection::SaveFeatureAsImage(const std::string& filePath) const
    {
        for (int detectorId = 0; detectorId < mCascadedDetectors.size(); detectorId++)
        {
            mCascadedDetectors.at(detectorId)->SaveFeatureAsImage(filePath, mBaseImgSize, detectorId);
        }
    }

    int RealTimeFaceDetection::DetectOneFace(const std::vector<unsigned int>& integralImg, int imgW, 
        int sRow, int sCol, int boxSize, double scale, int avgImgGray) const
    {
        /*if (MagicTool::gLogOut)
        {
            DebugLog << "DetectOneFace: " << imgW << " " << sRow << " " << sCol << " " << boxSize << " " << scale << " " << avgImgGray << std::endl;
        }*/
        if (mCascadedDetectors.size() == 0)
        {
            return 0;
        }
        for (std::vector<AdaBoostFaceDetection*>::const_iterator itr = mCascadedDetectors.begin(); itr != mCascadedDetectors.end(); itr++)
        {
            /*if (MagicTool::gLogOut)
            {
                DebugLog << "AdaBoost level: " << itr - mCascadedDetectors.begin() << std::endl;
            }*/
            if ( (*itr)->Predict(integralImg, imgW, sRow, sCol, boxSize, scale, avgImgGray) == 0 )
            {
                return 0;
            }
        }
        return 1;
    }

    void RealTimeFaceDetection::PostProcessFaces(std::vector<int>& faces) const
    {
        if (faces.size() > 4)
        {
            int faceCount = faces.size() / 4;
            std::vector<int> faceLabels(faceCount, -1);
            int newLabel = 0;
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                int curLabel = faceLabels.at(faceId);
                if (curLabel == -1)
                {
                    curLabel = newLabel;
                    newLabel++;
                    faceLabels.at(faceId) = curLabel;
                }
                for (int compareId = faceId + 1; compareId < faceCount; compareId++)
                {
                    if (faceLabels.at(faceId) == faceLabels.at(compareId))
                    {
                        continue;
                    }
                    else if (faceLabels.at(compareId) == -1)
                    {
                        int faceBaseId = faceId * 4;
                        int compareBaseId = compareId * 4;
                        if ( IsTheSameFace(faces.at(faceBaseId), faces.at(faceBaseId + 1), faces.at(faceBaseId + 2), faces.at(faceBaseId + 3),
                            faces.at(compareBaseId), faces.at(compareBaseId + 1), faces.at(compareBaseId + 2), faces.at(compareBaseId + 3)) )
                        {
                            faceLabels.at(compareId) = curLabel;
                        }
                    }
                    else
                    {
                        int faceBaseId = faceId * 4;
                        int compareBaseId = compareId * 4;
                        if ( IsTheSameFace(faces.at(faceBaseId), faces.at(faceBaseId + 1), faces.at(faceBaseId + 2), faces.at(faceBaseId + 3),
                            faces.at(compareBaseId), faces.at(compareBaseId + 1), faces.at(compareBaseId + 2), faces.at(compareBaseId + 3)) )
                        {
                            int preChangeLabel, postChangeLabel; 
                            if (faceLabels.at(compareId) < faceLabels.at(faceId))
                            {
                                preChangeLabel = faceLabels.at(faceId);
                                postChangeLabel = faceLabels.at(compareId);
                                curLabel = postChangeLabel;
                            }
                            else
                            {
                                preChangeLabel = faceLabels.at(compareId);
                                postChangeLabel = faceLabels.at(faceId);
                            }
                            for (int changeId = 0; changeId < faceCount; changeId++)
                            {
                                if (faceLabels.at(changeId) == preChangeLabel)
                                {
                                    faceLabels.at(changeId) = postChangeLabel;
                                }
                            }
                        }
                    } // end of else
                } // end of compareId for
            } // end of faceId for

            std::vector<int> facesCopy = faces;
            faces.clear();
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                if (faceLabels.at(faceId) == -1)
                {
                    continue;
                }
                std::vector<int> clusters;
                int curLabel = faceLabels.at(faceId);
                for (int compareId = faceId; compareId < faceCount; compareId++)
                {
                    if (faceLabels.at(compareId) == curLabel)
                    {
                        clusters.push_back(compareId);
                        faceLabels.at(compareId) = -1;
                    }
                }
                //Process clusters
                //simple average
                int sRow = 0;
                int sCol = 0;
                int lRow = 0;
                int lCol = 0;
                for (std::vector<int>::iterator itr = clusters.begin(); itr != clusters.end(); itr++)
                {
                    int baseIndex = (*itr) * 4;
                    sRow += facesCopy.at(baseIndex);
                    sCol += facesCopy.at(baseIndex + 1);
                    lRow += facesCopy.at(baseIndex + 2);
                    lCol += facesCopy.at(baseIndex + 3);
                }
                float clusterCount = clusters.size();
                faces.push_back(sRow / clusterCount);
                faces.push_back(sCol / clusterCount);
                faces.push_back(lRow / clusterCount);
                faces.push_back(lCol / clusterCount);
            }
        }
    }

    bool RealTimeFaceDetection::IsTheSameFace(int sRowA, int sColA, int lRowA, int lColA, int sRowB, int sColB, int lRowB, int lColB) const
    {
        int wOverlap = CalCulateLineSegmentOverlap(sColA, lColA, sColB, lColB);
        int hOverlap = CalCulateLineSegmentOverlap(sRowA, lRowA, sRowB, lRowB);
        float overlapArea = float(wOverlap * hOverlap);
        float areaA = float(lRowA * lColA);
        float areaB = float(lRowB * lColB);
        float simA = overlapArea / areaA;
        float simB = overlapArea / areaB;
        return (simA > 0.5f && simB > 0.5f);
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
