#include "RealTimeFaceDetection.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

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

    int HaarClassifier::Learn(const ImageLoader& faceImgLoader, const std::vector<double>& faceDataWeights, 
        const ImageLoader& nonFaceImgLoader, const std::vector<double>& nonFaceDataWeights, const std::vector<int>& nonFaceIndex,
        double* trainError)
    {
        return MAGIC_NO_ERROR;
    }
     
    int HaarClassifier::Predict(const std::vector<unsigned int>& integralImg, int sRow, int sCol, float scale) const
    {
        return 0;
    }

    int HaarClassifier::Predict(const ImageLoader& imgLoader, int dataId) const
    {
        return 0;
    }

    int HaarClassifier::CalFeatureValue(const ImageLoader& imgLoader, int dataId) const
    {
        if (mFeature.type == 0)
        {
            int posValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol / 2 - 1);
            int negValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol + mFeature.lCol / 2 - 1,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            return posValue - negValue;
        }
        else if (mFeature.type == 1)
        {
            int negValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow, mFeature.sCol, 
                mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol + mFeature.lCol - 1);
            int posValue = ImgBoxValue(imgLoader, dataId, mFeature.sRow + mFeature.lRow / 2 - 1, mFeature.sCol,
                mFeature.sRow + mFeature.lRow - 1, mFeature.sCol + mFeature.lCol - 1);
            return posValue - negValue;
        }
        else if (mFeature.type == 2)
        {
            return 0;
        }
        else if (mFeature.type == 3)
        {
            return 0;
        }
        else
        {
            DebugLog << "Error: wrong type: " << mFeature.type << std::endl;
            return 0;
        }
    }

    int HaarClassifier::ImgBoxValue(const ImageLoader& imgLoader, int dataId, int sRow, int sCol, int eRow, int eCol) const
    {
        return 0;
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
        if (nonFaceCount == 0)
        {
            DebugLog << "There is no non-face data now" << std::endl;
            return MAGIC_INVALID_INPUT;
        }
        std::vector<double> nonFaceWeights(nonFaceCount, 0.5 / nonFaceCount);

        GenerateClassifierCadidates(faceImgLoader.GetImageWidth(0));
        return MAGIC_NO_ERROR;
        std::vector<int> faceResFlag(faceCount);
        std::vector<int> nonFaceResFlag(nonFaceCount);
        double epsilon = 1.0e-5;
        mThreshold = 0.0;
        for (int levelId = 0; levelId < levelCount; levelId++)
        {
            int weakClassifierId = TrainWeakClassifier(faceImgLoader, faceWeights, nonFaceImgLoader, nonFaceWeights,
                nonFaceIndex);
            HaarClassifier* pWeakClassifier = NULL;
            if (weakClassifierId != -1)
            {
                pWeakClassifier = mClassifierCandidates.at(weakClassifierId);
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
            for (int faceId = 0; faceId < faceCount; faceId++)
            {
                faceResFlag.at(faceId) = abs(1 - pWeakClassifier->Predict(faceImgLoader, faceId));
                trainingError += faceWeights.at(faceId) * faceResFlag.at(faceId);
            }
            for (int nonFaceId = 0; nonFaceId < nonFaceCount; nonFaceId++)
            {
                nonFaceResFlag.at(nonFaceId) = pWeakClassifier->Predict(nonFaceImgLoader, nonFaceIndex.at(nonFaceId));
                trainingError += nonFaceWeights.at(nonFaceId) * nonFaceResFlag.at(nonFaceId);
            }
            if (trainingError < 0.5)
            {
                DebugLog << "Weak Classifier " << levelId << ": " << trainingError << std::endl;
            }

            if (fabs(trainingError - 1.0) < epsilon)
            {
                trainingError = 1.0 - epsilon;
                DebugLog << "Weak Classifier " << levelId << ": is too weak!" << std::endl;
            }
            else if (fabs(trainingError) < epsilon)
            {
                trainingError = epsilon;
                DebugLog << "Weak Classifier " << levelId << ": is too good!" << std::endl;
            }

            double beta = trainingError / (1.0 - trainingError);
            double weight = log(1.0 / beta);
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

        return MAGIC_NO_ERROR;
    }
    
    int AdaBoostFaceDetection::Predict(const std::vector<unsigned int>& integralImg, int sRow, int sCol, double scale) const
    {
        double res = 0.0;
        int classifierCount = mClassifierWeights.size();
        for (int cid = 0; cid < classifierCount; cid++)
        {
            res += mClassifiers.at(cid)->Predict(integralImg, sRow, sCol, scale) * mClassifierWeights.at(cid);
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
        for (int sRow = 0; sRow < baseImgSize; sRow++)
        {
            for (int sCol = 0; sCol < baseImgSize; sCol++)
            {
                int colMaxLen = baseImgSize - sCol;
                int rowMaxLen = baseImgSize - sRow;
                for (int lRow = 1; lRow <= rowMaxLen; lRow++)
                {
                    for (int lCol = 2; lCol <= colMaxLen; lCol += 2)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 0};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 2; lRow <= rowMaxLen; lRow += 2)
                {
                    for (int lCol = 1; lCol <= colMaxLen; lCol++)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 1};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 1; lRow <= rowMaxLen; lRow++)
                {
                    for (int lCol = 2; lCol <= colMaxLen; lCol += 3)
                    {
                        HaarFeature feature = {sRow, sCol, lRow, lCol, 2};
                        HaarClassifier* pClassifier = new HaarClassifier(feature);
                        mClassifierCandidates.push_back(pClassifier);
                    }
                }
                for (int lRow = 1; lRow <= rowMaxLen; lRow += 2)
                {
                    for (int lCol = 2; lCol <= colMaxLen; lCol += 2)
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
                int res = pClassifier->Learn(faceImgLoader, faceDataWeights, nonFaceImgLoader, nonFaceDataWeights, nonFaceIndex, &trainError);
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
        mBaseImgSize = faceImgLoader.GetImageWidth(0);
        ImageLoader nonFaceImgLoader;
        nonFaceImgLoader.LoadImages(nonFaceImages, ImageLoader::IT_Gray);
        nonFaceImgLoader.GenerateIntegralImage();
        nonFaceImgLoader.ClearImageData();
        std::vector<bool> nonFaceValidFlag(nonFaceImages.size(), 1);
        DebugLog << "Learn Cascaded detectors..." << std::endl;
        for (int stageId = 0; stageId < stageCount; stageId++)
        {
            AdaBoostFaceDetection* pDetector = new AdaBoostFaceDetection;
            int res = pDetector->Learn(faceImgLoader, nonFaceImgLoader, nonFaceValidFlag, layerCounts.at(stageId));
            if (res != MAGIC_NO_ERROR)
            {
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
        }

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
        while (curSubImgSize < imgH && curSubImgSize < imgW)
        {
            int maxRow = imgH - curSubImgSize;
            int maxCol = imgW - curSubImgSize;
            for (int rid = 0; rid < maxRow; rid += curStep)
            {
                for (int cid = 0; cid < maxCol; cid += curStep)
                {
                    if (DetectOneFace(integralImg, rid, cid, curScale))
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

    int RealTimeFaceDetection::DetectOneFace(const std::vector<unsigned int>& integralImg, int sRow, int sCol, double scale) const
    {
        if (mCascadedDetectors.size() == 0)
        {
            return 0;
        }
        for (std::vector<AdaBoostFaceDetection*>::const_iterator itr = mCascadedDetectors.begin(); itr != mCascadedDetectors.end(); itr++)
        {
            if ( (*itr)->Predict(integralImg, sRow, sCol, scale) == 0 )
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
