#include "HighDimensionalFeature.h"
#include "../Math/SparseMatrix.h"
#include "../MachineLearning/PrincipalComponentAnalysis.h"
#include "../MachineLearning/LinearDiscriminantAnalysis.h"
#include "../MachineLearning/RotatedSparseRegression.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    HighDimensionalFeature::HighDimensionalFeature() :
        mpRSRegression(NULL),
        mTargetDim(2000),
        mMultiScaleCount(1),
        mMultiScaleValue(0.75),
        mPatchSize(40),
        mCellSize(40),
        mUniformPatternSize(0),
        mUniformPatternMap()
    {
        ConstructUniformPatternMap();
    }
    
    HighDimensionalFeature::~HighDimensionalFeature()
    {
        Reset();
    }
    
    int HighDimensionalFeature::Learn(const std::vector<std::string>& imgFiles, const std::vector<int>& marksList,
                                      int markCountPerImg, const std::vector<int> faceIds)
    {
        int dataCount = imgFiles.size();
        if (dataCount < 1)
        {
            return MAGIC_EMPTY_INPUT;
        }
        if (markCountPerImg * dataCount != marksList.size())
        {
            return MAGIC_INVALID_INPUT;
        }
        
        //Calculate high dimensional features
        std::vector<double> features;
        int featureDim = 0;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            cv::Mat img = cv::imread(imgFiles.at(dataId));
            std::vector<int> oneFaceMarks;
            oneFaceMarks.reserve(markCountPerImg * 2);
            int globalMarkStartId = dataId * markCountPerImg * 2;
            int globalMarkEndId = globalMarkStartId + markCountPerImg * 2;
            for (int markId = globalMarkStartId; markId < globalMarkEndId; markId++)
            {
                oneFaceMarks.push_back(marksList.at(markId));
            }
            std::vector<double> oneFaceFeature = CalFaceFeature(img, oneFaceMarks);
            if (dataId == 0)
            {
                featureDim = oneFaceFeature.size();
                features.reserve(dataCount * featureDim);
                DebugLog << "Allocate features memory" << std::endl;
            }
            for (std::vector<double>::iterator itr = oneFaceFeature.begin(); itr != oneFaceFeature.end(); itr++)
            {
                features.push_back(*itr);
            }
        }
        if (featureDim == 0)
        {
            DebugLog << "Error: featureDim == 0" << std::endl;
            Reset();
            return MAGIC_INVALID_RESULT;
        }

        //Compress dimension using pca
        MagicML::PrincipalComponentAnalysis pca;
        int pcaDim;
        int pcaRes = pca.Analyse(features, featureDim, 0.99, pcaDim);
        if (pcaRes != MAGIC_NO_ERROR)
        {
            DebugLog << "PCA error code: " << pcaRes << std::endl;
            Reset();
            return pcaRes;
        }
        std::vector<double> pcaCompressedFeatures;
        pcaCompressedFeatures.reserve(pcaDim * dataCount);
        for (long long dataId = 0; dataId < dataCount; dataId++)
        {
            std::vector<double> oneFeature;
            oneFeature.reserve(featureDim);
            long long startFeatureIndex = dataId * featureDim;
            long long endFeatureIndex = (dataId + 1) * featureDim;
            for (long long featureIndex = startFeatureIndex; featureIndex < endFeatureIndex; featureIndex++)
            {
                oneFeature.push_back(features.at(featureIndex));
            }
            std::vector<double> compressedOneFeature = pca.Project(oneFeature);
            for (int pcaIndex = 0; pcaIndex < pcaDim; pcaIndex++)
            {
                pcaCompressedFeatures.push_back(compressedOneFeature.at(pcaIndex));
            }
        }
        pca.Clear();
        //features.clear();

        //Compress feature using lda
        int ldaDim;
        MagicML::LinearDiscriminantAnalysis lda;
        int ldaRes = lda.Analyse(pcaCompressedFeatures, faceIds, 0.99, ldaDim);
        if (ldaRes != MAGIC_NO_ERROR)
        {
            DebugLog << "LDA error code: " << ldaRes << std::endl;
            Reset();
            return ldaRes;
        }
        std::vector<double> ldaCompressedFeatures;
        ldaCompressedFeatures.reserve(ldaDim * dataCount);
        for (long long dataId = 0; dataId < dataCount; dataId++)
        {
            std::vector<double> oneFeature;
            oneFeature.reserve(pcaDim);
            long long startFeatureIndex = dataId * pcaDim;
            long long endFeatureIndex = (dataId + 1) * pcaDim;
            for (long long featureIndex = startFeatureIndex; featureIndex < endFeatureIndex; featureIndex++)
            {
                oneFeature.push_back(pcaCompressedFeatures.at(featureIndex));
            }
            std::vector<double> ldaOneFeature = lda.Project(oneFeature);
            for (int ldaIndex = 0; ldaIndex < ldaDim; ldaIndex++)
            {
                ldaCompressedFeatures.push_back(ldaOneFeature.at(ldaIndex));
            }
        }
        lda.Reset();
        pcaCompressedFeatures.clear();

        //Learn Projection Matrix
        if (mpRSRegression == NULL)
        {
            mpRSRegression = new MagicML::RotatedSparseRegression;
        }
        mpRSRegression->Reset();
        double lamda = 10;
        int regRes = mpRSRegression->Learn(features, ldaCompressedFeatures, dataCount, lamda);
        if (regRes != MAGIC_NO_ERROR)
        {
            DebugLog << "RSRegression error code: " << regRes << std::endl;
            Reset();
            return regRes;
        }

        return MAGIC_NO_ERROR;
    }
    
    int HighDimensionalFeature::GetHighDimensionalFeature(const cv::Mat& img, const std::vector<int>& marksList,
                                                          std::vector<double>& feature) const
    {
        feature = CalFaceFeature(img, marksList);
        return MAGIC_NO_ERROR;
    }
    
    int HighDimensionalFeature::GetCompressedFeature(const cv::Mat& img, const std::vector<int>& marksList,
                                                     std::vector<double>& feature) const
    {
        return MAGIC_NO_ERROR;
    }
    
    void HighDimensionalFeature::Save(const std::string& fileName) const
    {
        
    }
    
    void HighDimensionalFeature::Load(const std::string& fileName)
    {
        
    }
    
    void HighDimensionalFeature::Reset(void)
    {
        if (mpRSRegression != NULL)
        {
            delete mpRSRegression;
            mpRSRegression = NULL;
        }
    }

    void HighDimensionalFeature::ConstructUniformPatternMap(void)
    {
        mUniformPatternMap.resize(256);
        mUniformPatternSize = 1;
        int maxFlapCount = 2;
        for (unsigned int index = 0; index < 256; index++)
        {
            unsigned int lastFlag = index & 1;
            unsigned int indexCopy = index >> 1;
            int flapNum = 0;
            for (int bitId = 1; bitId <= 8; bitId++)
            {
                unsigned int curFlag = indexCopy & 1;
                indexCopy = indexCopy >> 1;
                if (lastFlag != curFlag)
                {
                    flapNum++;
                }
                lastFlag = curFlag;
            }
            if (flapNum > maxFlapCount)
            {
                mUniformPatternMap.at(index) = 0;
            }
            else
            {
                mUniformPatternMap.at(index) = mUniformPatternSize;
                mUniformPatternSize++;
            }
        }
        DebugLog << "Uniform map size: " << mUniformPatternSize << std::endl;
    }

    std::vector<double> HighDimensionalFeature::CalFaceFeature(const cv::Mat& img, const std::vector<int>& marksList) const
    {
        int markCount = marksList.size() / 2;
        int cellCount = mPatchSize * mPatchSize / (mCellSize * mCellSize);
        std::vector<double> features;
        features.reserve(mMultiScaleCount * markCount * cellCount * mUniformPatternSize);
        cv::Mat scaleImg = img.clone();
        std::vector<double> scaleMarksList;
        scaleMarksList.reserve(marksList.size());
        for (std::vector<int>::const_iterator itr = marksList.begin(); itr != marksList.end(); itr++)
        {
            scaleMarksList.push_back(*itr);
        }
        for (int multiScaleId = 0; multiScaleId < mMultiScaleCount; multiScaleId++)
        {
            for (int markId = 0; markId < markCount; markId++)
            {
                int markCol = floor( scaleMarksList.at(markId * 2) + 0.5 );
                int markRow = floor( scaleMarksList.at(markId * 2 + 1) + 0.5 );
                CalLocalMarkFeature(scaleImg, markRow, markCol, features);
            }
            if (multiScaleId == mMultiScaleCount - 1)
            {
                break;
            }
            for (std::vector<double>::iterator itr = scaleMarksList.begin(); itr != scaleMarksList.end(); itr++)
            {
                (*itr) = (*itr) * mMultiScaleValue;
            }
            cv::Size cvSize(scaleImg.rows * mMultiScaleValue, scaleImg.cols * mMultiScaleValue);
            cv::Mat curScaleImg(cvSize, CV_8UC1);
            cv::resize(scaleImg, curScaleImg, cvSize);
            scaleImg.release();
            scaleImg = curScaleImg;
        }

        return features;
    }

    void HighDimensionalFeature::CalLocalMarkFeature(const cv::Mat& img, int markRow, int markCol, std::vector<double>& features) const
    {
        int cellLineCount = mPatchSize / mCellSize;
        int patchStartRow = markRow - mPatchSize / 2;
        int patchStartCol = markCol - mPatchSize / 2;
        int cellPixelCount = mCellSize * mCellSize;
        //double cellNonUniformValue = 0;
        for (int cellLineRow = 0; cellLineRow < cellLineCount; cellLineRow++)
        {
            for (int cellLineCol = 0; cellLineCol < cellLineCount; cellLineCol++)
            {
                int cellStartRow = patchStartRow + mCellSize * cellLineRow;
                int cellStartCol = patchStartCol + mCellSize * cellLineCol;
                int cellEndRow = cellStartRow + mCellSize;
                int cellEndCol = cellStartCol + mCellSize;
                std::vector<int> lbpHistory(mUniformPatternSize, 0);
                for (int pixelRow = cellStartRow; pixelRow < cellEndRow; pixelRow++)
                {
                    for (int pixelCol = cellStartCol; pixelCol < cellEndCol; pixelCol++)
                    {
                        int lbpValue;
                        CalPixelLbpValue(img, pixelRow, pixelCol, lbpValue);
                        lbpHistory.at(mUniformPatternMap.at(lbpValue))++;
                    }
                }
                for (int lbpId = 0; lbpId < mUniformPatternSize; lbpId++)
                {
                    features.push_back( double(lbpHistory.at(lbpId)) / cellPixelCount );
                }
                //cellNonUniformValue += double(lbpHistory.at(0)) / cellPixelCount;
            }
        }
        //DebugLog << "Cell NonUniform Value: " << cellNonUniformValue / cellLineCount / cellLineCount << std::endl;
    }

    void HighDimensionalFeature::CalPixelLbpValue(const cv::Mat& img, int pixelRow, int pixelCol, int& lbpValue) const
    {
        lbpValue = 0;
        if (pixelRow > 0 && pixelRow < img.rows - 1 && pixelCol > 0 && pixelCol < img.cols - 1)
        {
            const unsigned char* pPixelCenter = img.ptr(pixelRow, pixelCol);
            const unsigned char* pPixel0 = img.ptr(pixelRow - 1, pixelCol - 1);
            if (pPixel0[0] > pPixelCenter[0])
            {
                lbpValue += 1;
            }
            const unsigned char* pPixel1 = img.ptr(pixelRow, pixelCol - 1);
            if (pPixel1[0] > pPixelCenter[0])
            {
                lbpValue += 2;
            }
            const unsigned char* pPixel2 = img.ptr(pixelRow + 1, pixelCol - 1);
            if (pPixel2[0] > pPixelCenter[0])
            {
                lbpValue += 4;
            }
            const unsigned char* pPixel3 = img.ptr(pixelRow + 1, pixelCol);
            if (pPixel3[0] > pPixelCenter[0])
            {
                lbpValue += 8;
            }
            const unsigned char* pPixel4 = img.ptr(pixelRow + 1, pixelCol + 1);
            if (pPixel4[0] > pPixelCenter[0])
            {
                lbpValue += 16;
            }
            const unsigned char* pPixel5 = img.ptr(pixelRow, pixelCol + 1);
            if (pPixel5[0] > pPixelCenter[0])
            {
                lbpValue += 32;
            }
            const unsigned char* pPixel6 = img.ptr(pixelRow - 1, pixelCol + 1);
            if (pPixel6[0] > pPixelCenter[0])
            {
                lbpValue += 64;
            }
            const unsigned char* pPixel7 = img.ptr(pixelRow - 1, pixelCol);
            if (pPixel7[0] > pPixelCenter[0])
            {
                lbpValue += 128;
            }
        }
    }
}
