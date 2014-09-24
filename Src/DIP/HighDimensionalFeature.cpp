#include "HighDimensionalFeature.h"
#include "../Math/SparseMatrix.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    HighDimensionalFeature::HighDimensionalFeature() :
        mpProjectMat(NULL),
        mTargetDim(2000),
        mMultiScaleCount(4),
        mMultiScaleValue(0.8),
        mPatchSize(40),
        mCellSize(10),
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
        double* features = NULL;
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
                features = new double[dataCount * oneFaceFeature.size()];
                DebugLog << "Allocate features memory" << std::endl;
            }
            for (int markId = 0; markId < markCountPerImg * 2; markId++)
            {
                features[globalMarkStartId + markId] = oneFaceFeature.at(markId);
            }
        }
        
        //Compress dimension
        
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
        if (mpProjectMat != NULL)
        {
            delete mpProjectMat;
            mpProjectMat = NULL;
        }
    }

    void HighDimensionalFeature::ConstructUniformPatternMap(void)
    {
        mUniformPatternMap.resize(256);
        for (int index = 0; index < 256; index++)
        {

        }
    }

    std::vector<double> HighDimensionalFeature::CalFaceFeature(const cv::Mat& img, const std::vector<int>& marksList) const
    {
        int markCount = marksList.size() / 2;
        int cellCount = mPatchSize * mPatchSize / (mCellSize * mCellSize);
        std::vector<double> features;
        features.reserve(mMultiScaleCount * markCount * cellCount * 256);
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
        for (int cellLineRow = 0; cellLineRow < cellLineCount; cellLineRow++)
        {
            for (int cellLineCol = 0; cellLineCol < cellLineCount; cellLineCol++)
            {
                int cellStartRow = patchStartRow + mCellSize * cellLineRow;
                int cellStartCol = patchStartCol + mCellSize * cellLineCol;
                int cellEndRow = cellStartRow + mCellSize;
                int cellEndCol = cellStartCol + mCellSize;
                std::vector<int> lbpHistory(256, 0);
                for (int pixelRow = cellStartRow; pixelRow < cellEndRow; pixelRow++)
                {
                    for (int pixelCol = cellStartCol; pixelCol < cellEndCol; pixelCol++)
                    {
                        int lbpValue;
                        CalPixelLbpValue(img, pixelRow, pixelCol, lbpValue);
                        lbpHistory.at(lbpValue)++;
                    }
                }
                for (int lbpId = 0; lbpId < 256; lbpId++)
                {
                    features.push_back( double(lbpHistory.at(lbpId)) / cellPixelCount );
                }
            }
        }
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
