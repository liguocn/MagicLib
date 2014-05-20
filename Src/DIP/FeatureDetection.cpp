#include "FeatureDetection.h"

namespace MagicDIP
{
    FeatureDetection::FeatureDetection()
    {
    }

    FeatureDetection::~FeatureDetection()
    {
    }

    cv::Mat FeatureDetection::CannyEdgeDetection(const cv::Mat& img)
    {
        int imgW = img.cols;
        int imgH = img.rows;
        cv::Mat grayImg(imgH, imgW, CV_8UC1);
        cv::cvtColor(img, grayImg, CV_BGR2GRAY);
        cv::Mat edgeImg(imgH, imgW, CV_8U);
        cv::Mat blurImg(imgH, imgW, CV_8U);
        int blurCount = 10;
        std::vector<int> resList(imgW * imgH, 0);
        for (int blurIndex = 0; blurIndex < blurCount; blurIndex++)
        {
            cv::GaussianBlur(grayImg, blurImg, cv::Size(5, 5), 0);
            grayImg.release();
            grayImg = blurImg.clone();
            cv::Canny(grayImg, edgeImg, 15, 45);
            for (int hid = 0; hid < imgH; hid++)
            {
                for (int wid = 0; wid < imgW; wid++)
                {
                    unsigned char* pixel = edgeImg.ptr(hid, wid);
                    if (pixel[0] > 0)
                    {
                        resList.at(hid * imgW + wid)++;
                    }
                }
            }
        }
        cv::Mat resImg(imgH, imgW, CV_8UC3);
        for (int hid = 0; hid < imgH; hid++)
        {
            int baseIndex = hid * imgW;
            for (int wid = 0; wid < imgW; wid++)
            {
                unsigned char* pixel = resImg.ptr(hid, wid);
                unsigned char pValue = resList.at(baseIndex + wid) * 255 / blurCount;
                pixel[0] = pValue;
                pixel[1] = pValue;
                pixel[2] = pValue;
            }
        }
        return resImg;
    }
}