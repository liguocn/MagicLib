#pragma once
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"

namespace MagicDIP
{
    class ImageLoader
    {
    public:
        enum ImageType
        {
            IT_Color,
            IT_Gray
        };

        ImageLoader();
        ~ImageLoader();

        int LoadImages(const std::vector<std::string>& imgFiles, ImageType it);
        void GenerateIntegralImage(void);
        const unsigned char* GetImage(int imgId) const;
        int GetImageCount(void) const;
        int GetImageWidth(int imgId) const;
        int GetImageHeight(int imgId) const;
        unsigned char GetGrayImageValue(int imgId, int hid, int wid) const;
        int GetIntegralValue(int imgId, int hid, int wid) const;
        void ClearImageData(void);
        void ClearIntegralImageData(void);
        void Reset(void);
        

        static void TransferToIntegralImg(const cv::Mat& img, std::vector<unsigned int>& integralImg);

    private:
        int mImageCount;
        std::vector<unsigned char* > mImages;
        std::vector<unsigned int* > mIntegralImage;
        std::vector<int> mImageSize;
    };
}
