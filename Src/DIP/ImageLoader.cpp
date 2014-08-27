#include "ImageLoader.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    ImageLoader::ImageLoader() :
        mImageCount(0),
        mImages(),
        mImageSize(),
        mIntegralImage()
    {
    }

    ImageLoader::~ImageLoader()
    {
        Reset();
    }

    int ImageLoader::LoadImages(const std::vector<std::string>& imgFiles, ImageType it)
    {
        if (imgFiles.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        Reset();
        mImageCount = imgFiles.size();
        if (it == IT_Gray)
        {
            mImages.reserve(mImageCount);
            mImageSize.reserve(mImageCount * 2);
            for (int imgId = 0; imgId < mImageCount; imgId++)
            {
                cv::Mat img = cv::imread(imgFiles.at(imgId));
                int imgH = img.rows;
                int imgW = img.cols;
                mImageSize.push_back(imgH);
                mImageSize.push_back(imgW);
                unsigned char* pImagePixels = new unsigned char[imgH * imgW];
                for (int hid = 0; hid < imgH; hid++)
                {
                    int baseIndex = hid * imgW;
                    for (int wid = 0; wid < imgW; wid++)
                    {
                        pImagePixels[baseIndex + wid] = img.ptr(hid, wid)[0];
                    }
                }
                img.release();
                mImages.push_back(pImagePixels);
            }
        }
        else if (it == IT_Color)
        {
            DebugLog << "Error: No implementation" << std::endl;
        }

        return MAGIC_NO_ERROR;
    }

    void ImageLoader::GenerateIntegralImage(void)
    {
        ClearIntegralImageData();
        mIntegralImage.reserve(mImageCount);
        unsigned int* colCumValues = NULL;
        int lastH = 0;
        int lastW = 0;
        for (int imgId = 0; imgId < mImageCount; imgId++)
        {
            unsigned char* pImage = mImages.at(imgId);
            int imgH = mImageSize.at(imgId * 2);
            int imgW = mImageSize.at(imgId * 2 + 1);
            if (lastH != imgH || lastW != imgW)
            {
                if (colCumValues != NULL)
                {
                    delete []colCumValues;
                    colCumValues = NULL;
                }
                colCumValues = new unsigned int[imgH * imgW];
            }
            for (int wid = 0; wid < imgW; wid++)
            {
                colCumValues[wid] = pImage[wid];
            }
            for (int hid = 1; hid < imgH; hid++)
            {
                int baseIndex = hid * imgW;
                int lastBaseIndex = baseIndex - imgW;
                for (int wid = 0; wid < imgW; wid++)
                {
                    colCumValues[baseIndex + wid] = colCumValues[lastBaseIndex + wid] + pImage[baseIndex + wid];
                }
            }
            unsigned int* pIntegralValues = new unsigned int[imgH * imgW];
            for (int hid = 0; hid < imgH; hid++)
            {
                pIntegralValues[hid * imgW] = colCumValues[hid * imgW];
            }
            for (int wid = 1; wid < imgW; wid++)
            {
                for (int hid = 0; hid < imgH; hid++)
                {
                    int baseIndex = hid * imgW + wid;
                    pIntegralValues[baseIndex] = pIntegralValues[baseIndex - 1] + colCumValues[baseIndex];
                }
            }
            mIntegralImage.push_back(pIntegralValues);
            lastH = imgH;
            lastW = imgW;
        }
        if (colCumValues != NULL)
        {
            delete []colCumValues;
            colCumValues = NULL;
        }
    }

    void ImageLoader::TestIntegralImage(void)
    {
        DebugLog << "TestIntegralImage: " << mImageCount << std::endl;
        for (int imgId = 0; imgId < mImageCount; imgId++)
        {
            int imgH = GetImageHeight(imgId);
            int imgW = GetImageWidth(imgId);
            for (int hid = 0; hid < imgH; hid++)
            {
                for (int wid = 0; wid < imgW; wid++)
                {
                    unsigned char gray = GetGrayImageValue(imgId, hid, wid);
                    int integralValue = 0;
                    if (hid > 0 && wid > 0)
                    {
                        integralValue = GetIntegralValue(imgId, hid, wid) + GetIntegralValue(imgId, hid - 1, wid - 1) -
                            GetIntegralValue(imgId, hid - 1, wid) - GetIntegralValue(imgId, hid, wid - 1);
                    }
                    else if (hid > 0 && wid == 0)
                    {
                        integralValue = GetIntegralValue(imgId, hid, wid) - GetIntegralValue(imgId, hid - 1, wid);
                    }
                    else if (hid == 0 && wid > 0)
                    {
                        integralValue = GetIntegralValue(imgId, hid, wid) - GetIntegralValue(imgId, hid, wid - 1);
                    }
                    else if (hid == 0 && wid == 0)
                    {
                        integralValue = GetIntegralValue(imgId, hid, wid);
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
    }

    const unsigned char* ImageLoader::GetImage(int imgId) const
    {
        return mImages.at(imgId);
    }

    int ImageLoader::GetImageWidth(int imgId) const
    {
        return mImageSize.at(imgId * 2 + 1);
    }
     
    int ImageLoader::GetImageHeight(int imgId) const
    {
        return mImageSize.at(imgId * 2);
    }
     
    unsigned char ImageLoader::GetGrayImageValue(int imgId, int hid, int wid) const
    {
        return mImages.at(imgId)[hid * mImageSize.at(imgId * 2 + 1) + wid];
    }

    int ImageLoader::GetIntegralValue(int imgId, int hid, int wid) const
    {
        return mIntegralImage.at(imgId)[hid * mImageSize.at(imgId * 2 + 1) + wid];
    }

    void ImageLoader::ClearImageData(void)
    {
        for (std::vector<unsigned char* >::iterator itr = mImages.begin(); itr != mImages.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mImages.clear();
    }

    void ImageLoader::ClearIntegralImageData(void)
    {
        for (std::vector<unsigned int* >::iterator itr = mIntegralImage.begin(); itr != mIntegralImage.end(); itr++)
        {
            if ((*itr) != NULL)
            {
                delete (*itr);
                (*itr) = NULL;
            }
        }
        mIntegralImage.clear();
    }
            
    void ImageLoader::Reset(void)
    {
        mImageCount = 0;
        ClearImageData();
        ClearIntegralImageData();
        mImageSize.clear();
    }

    int ImageLoader::GetImageCount(void) const
    {
        return mImageCount;
    }

    void ImageLoader::TransferToIntegralImg(const cv::Mat& img, std::vector<unsigned int>& integralImg)
    {
        int imgH = img.rows;
        int imgW = img.cols;
        std::vector<unsigned int> colCumValues(imgH * imgW);
        for (int wid = 0; wid < imgW; wid++)
        {
            colCumValues.at(wid) = img.ptr(0, wid)[0];
        }
        for (int hid = 1; hid < imgH; hid++)
        {
            int baseIndex = hid * imgW;
            int lastBaseIndex = baseIndex - imgW;
            for (int wid = 0; wid < imgW; wid++)
            {
                colCumValues.at(baseIndex + wid) = colCumValues.at(lastBaseIndex + wid) + img.ptr(hid, wid)[0];
            }
        }
        integralImg.clear();
        integralImg = std::vector<unsigned int>(imgW * imgH);
        for (int hid = 0; hid < imgH; hid++)
        {
            integralImg.at(hid * imgW) = colCumValues.at(hid * imgW);
        }
        for (int wid = 1; wid < imgW; wid++)
        {
            for (int hid = 0; hid < imgH; hid++)
            {
                int baseIndex = hid * imgW + wid;
                integralImg.at(baseIndex) = integralImg.at(baseIndex - 1) + colCumValues.at(baseIndex);
            }
        }
    }
}