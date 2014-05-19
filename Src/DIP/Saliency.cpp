#include "Saliency.h"
#include "../Math/Vector3.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    SaliencyDetection::SaliencyDetection()
    {
    }

    SaliencyDetection::~SaliencyDetection()
    {
    }

    cv::Mat SaliencyDetection::DoGBandSaliency(const cv::Mat& inputImg)
    {
        cv::Mat cvtImg;
        cv::cvtColor(inputImg, cvtImg, CV_BGR2Lab);
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        std::vector<std::vector<int> > salientValue(inputH);
        for (int hid = 0; hid < inputH; hid++)
        {
            salientValue.at(hid) = std::vector<int>(inputW, 0);
        }
        float meanPixel[3] = {0.f, 0.f, 0.f};
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                unsigned char* pixel = cvtImg.ptr(hid, wid);
                meanPixel[0] += pixel[0];
                meanPixel[1] += pixel[1];
                meanPixel[2] += pixel[2];
            }
        }
        meanPixel[0] /= (inputH * inputW);
        meanPixel[1] /= (inputH * inputW);
        meanPixel[2] /= (inputH * inputW);
        int maxSalientValue = 0;
        for (int hid = 2; hid < inputH - 2; hid++)
        {
            for (int wid = 2; wid < inputW - 2; wid++)
            {
                float gaussianPixel[3] = {0.f, 0.f, 0.f};
                for (int i = -2; i <= 2; i++)
                {
                    for (int j = -2; j <= 2; j++)
                    {
                        if (i == 0 && j == 0)
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 6;
                            gaussianPixel[1] += pixel[1] * 6;
                            gaussianPixel[2] += pixel[2] * 6;
                        }
                        if ((abs(i) == 1 && abs(j) <= 1) || (abs(i) <= 1 && abs(j) == 1))
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 4;
                            gaussianPixel[1] += pixel[1] * 4;
                            gaussianPixel[2] += pixel[2] * 4;
                        }
                        else
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0];
                            gaussianPixel[1] += pixel[1];
                            gaussianPixel[2] += pixel[2];
                        }
                    }
                }
                gaussianPixel[0] /= 54.f;
                gaussianPixel[1] /= 54.f;
                gaussianPixel[2] /= 54.f;
                float fTemp = (meanPixel[0] - gaussianPixel[0]) * (meanPixel[0] - gaussianPixel[0]) + 
                    (meanPixel[1] - gaussianPixel[1]) * (meanPixel[1] - gaussianPixel[1]) + 
                    (meanPixel[2] - gaussianPixel[2]) * (meanPixel[2] - gaussianPixel[2]);
                fTemp = sqrt(fTemp);
                salientValue.at(hid).at(wid) = int(fTemp);
                if (salientValue.at(hid).at(wid) > maxSalientValue)
                {
                    maxSalientValue = salientValue.at(hid).at(wid);
                }
            }
        }
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                unsigned char* pixel = cvtImg.ptr(hid, wid);
                pixel[0] = salientValue.at(hid).at(wid) * 255 / maxSalientValue;
                pixel[1] = salientValue.at(hid).at(wid) * 255 / maxSalientValue;
                pixel[2] = salientValue.at(hid).at(wid) * 255 / maxSalientValue;
                //pixel[0] = salientValue.at(hid).at(wid);
                //pixel[1] = salientValue.at(hid).at(wid);
                //pixel[2] = salientValue.at(hid).at(wid);
            }
        }
        return cvtImg;
    }

    int SaliencyDetection::MaxNumber(int n0, int n1, int n2)
    {
        int maxNum = n0;
        if (n1 > maxNum)
        {
            maxNum = n1;
        }
        if (n2 > maxNum)
        {
            maxNum = n2;
        }
        return maxNum;
    }

    cv::Mat SaliencyDetection::GradientSaliency(const cv::Mat& inputImg)
    {
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        std::vector<std::vector<int> > gradMat(inputH);
        int maxGrad = 0;
        for (int hid = 0; hid < inputH; hid++)
        {
            std::vector<int> gradList(inputW);
            for (int wid = 0; wid < inputW; wid++)
            {
                const unsigned char* pixel = inputImg.ptr(hid, wid);
                const unsigned char* pixelWNext = NULL;
                const unsigned char* pixelWPre = NULL;
                if (wid == 0)
                {
                    pixelWNext = inputImg.ptr(hid, wid + 1);
                    pixelWPre = inputImg.ptr(hid, wid);
                }
                else if (wid == inputW - 1)
                {
                    pixelWNext = inputImg.ptr(hid, wid);
                    pixelWPre = inputImg.ptr(hid, wid - 1);
                }
                else
                {
                    pixelWNext = inputImg.ptr(hid, wid + 1);
                    pixelWPre = inputImg.ptr(hid, wid - 1);
                }
                const unsigned char* pixelHNext = NULL;
                const unsigned char* pixelHPre = NULL;
                if (hid == 0)
                {
                    pixelHNext = inputImg.ptr(hid + 1, wid);
                    pixelHPre = inputImg.ptr(hid, wid);
                }
                else if (hid == inputH - 1)
                {
                    pixelHNext = inputImg.ptr(hid, wid);
                    pixelHPre = inputImg.ptr(hid - 1, wid);
                }
                else
                {
                    pixelHNext = inputImg.ptr(hid + 1, wid);
                    pixelHPre = inputImg.ptr(hid - 1, wid);
                }
                gradList.at(wid) = MaxNumber(abs(pixelWPre[0] - pixelWNext[0]), abs(pixelWPre[1] - pixelWNext[1]), abs(pixelWPre[2] - pixelWNext[2])) +
                    MaxNumber(abs(pixelHPre[0] - pixelHNext[0]), abs(pixelHPre[1] - pixelHNext[1]), abs(pixelHPre[2] - pixelHNext[2]));
                if (gradList.at(wid) > maxGrad)
                {
                    maxGrad = gradList.at(wid);
                }
            }
            gradMat.at(hid) = gradList;
        }
        cv::Size imgSize(inputW, inputH);
        cv::Mat saliencyImg(imgSize, CV_8UC3);
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                unsigned char* pixel = saliencyImg.ptr(hid, wid);
                pixel[0] = gradMat.at(hid).at(wid) * 255 / maxGrad;
                pixel[1] = gradMat.at(hid).at(wid) * 255 / maxGrad;
                pixel[2] = gradMat.at(hid).at(wid) * 255 / maxGrad;
            }
        }

        return saliencyImg;
    }

    cv::Mat SaliencyDetection::DoGAndGradientSaliency(const cv::Mat& inputImg)
    {
        cv::Mat cvtImg;
        cv::cvtColor(inputImg, cvtImg, CV_BGR2Lab);
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        std::vector<std::vector<int> > salientValue(inputH);
        for (int hid = 0; hid < inputH; hid++)
        {
            salientValue.at(hid) = std::vector<int>(inputW, 0);
        }
        float meanPixel[3] = {0.f, 0.f, 0.f};
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                unsigned char* pixel = cvtImg.ptr(hid, wid);
                meanPixel[0] += pixel[0];
                meanPixel[1] += pixel[1];
                meanPixel[2] += pixel[2];
            }
        }
        meanPixel[0] /= (inputH * inputW);
        meanPixel[1] /= (inputH * inputW);
        meanPixel[2] /= (inputH * inputW);
        int maxSalientValue = 0;
        for (int hid = 2; hid < inputH - 2; hid++)
        {
            for (int wid = 2; wid < inputW - 2; wid++)
            {
                float gaussianPixel[3] = {0.f, 0.f, 0.f};
                for (int i = -2; i <= 2; i++)
                {
                    for (int j = -2; j <= 2; j++)
                    {
                        if (i == 0 && j == 0)
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 6;
                            gaussianPixel[1] += pixel[1] * 6;
                            gaussianPixel[2] += pixel[2] * 6;
                        }
                        if ((abs(i) == 1 && abs(j) <= 1) || (abs(i) <= 1 && abs(j) == 1))
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 4;
                            gaussianPixel[1] += pixel[1] * 4;
                            gaussianPixel[2] += pixel[2] * 4;
                        }
                        else
                        {
                            unsigned char* pixel = cvtImg.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0];
                            gaussianPixel[1] += pixel[1];
                            gaussianPixel[2] += pixel[2];
                        }
                    }
                }
                gaussianPixel[0] /= 54.f;
                gaussianPixel[1] /= 54.f;
                gaussianPixel[2] /= 54.f;
                float fTemp = (meanPixel[0] - gaussianPixel[0]) * (meanPixel[0] - gaussianPixel[0]) + 
                    (meanPixel[1] - gaussianPixel[1]) * (meanPixel[1] - gaussianPixel[1]) + 
                    (meanPixel[2] - gaussianPixel[2]) * (meanPixel[2] - gaussianPixel[2]);
                fTemp = sqrt(fTemp);
                salientValue.at(hid).at(wid) = int(fTemp);
                if (salientValue.at(hid).at(wid) > maxSalientValue)
                {
                    maxSalientValue = salientValue.at(hid).at(wid);
                }
            }
        }
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                salientValue.at(hid).at(wid) = salientValue.at(hid).at(wid) * 255 / maxSalientValue;
            }
        }
        //
        std::vector<std::vector<int> > gradMat(inputH);
        int maxGrad = 0;
        for (int hid = 0; hid < inputH; hid++)
        {
            std::vector<int> gradList(inputW);
            for (int wid = 0; wid < inputW; wid++)
            {
                const unsigned char* pixel = inputImg.ptr(hid, wid);
                const unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = inputImg.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = inputImg.ptr(hid, wid - 1);
                }
                const unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = inputImg.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = inputImg.ptr(hid - 1, wid);
                }
                gradList.at(wid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
                if (gradList.at(wid) > maxGrad)
                {
                    maxGrad = gradList.at(wid);
                }
            }
            gradMat.at(hid) = gradList;
        }
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                gradMat.at(hid).at(wid) = gradMat.at(hid).at(wid) * 255 / maxGrad;
            }
        }
        //
        cv::Size imgSize(inputW, inputH);
        cv::Mat saliencyImg(imgSize, CV_8UC3);
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                unsigned char* pixel = saliencyImg.ptr(hid, wid);
                if (gradMat.at(hid).at(wid) > salientValue.at(hid).at(wid))
                {
                    pixel[0] = gradMat.at(hid).at(wid);
                    pixel[1] = gradMat.at(hid).at(wid);
                    pixel[2] = gradMat.at(hid).at(wid);
                }
                else
                {
                    pixel[0] = salientValue.at(hid).at(wid);
                    pixel[1] = salientValue.at(hid).at(wid);
                    pixel[2] = salientValue.at(hid).at(wid);
                }
            }
        }

        return saliencyImg;
    }

    class ImgSubRegion
    {
    public:
        ImgSubRegion();
        ImgSubRegion(int leftTopX, int leftTopY, int width, int height);
        ImgSubRegion(int leftTopX, int leftTopY, int width, int height, MagicMath::Vector3& avgPixel);
        ~ImgSubRegion();
        bool MergeRegion(const ImgSubRegion& imgNeighbor, ImgSubRegion& mergedRegion);
        void CalculateAvgPixel(const cv::Mat& img);

    public:
        int mLeftTopX, mLeftTopY;
        int mWidth, mHeight;
        MagicMath::Vector3 mAvgPixel;
    };

    ImgSubRegion::ImgSubRegion() :
        mLeftTopX(-1),
        mLeftTopY(-1),
        mWidth(-1),
        mHeight(-1),
        mAvgPixel()
    {
    }

    ImgSubRegion::ImgSubRegion(int leftTopX, int leftTopY, int width, int height) :
        mLeftTopX(leftTopX),
        mLeftTopY(leftTopY),
        mWidth(width),
        mHeight(height),
        mAvgPixel()
    {
    }

    ImgSubRegion::ImgSubRegion(int leftTopX, int leftTopY, int width, int height, MagicMath::Vector3& avgPixel) :
        mLeftTopX(leftTopX),
        mLeftTopY(leftTopY),
        mWidth(width),
        mHeight(height),
        mAvgPixel(avgPixel)
    {
    }

    ImgSubRegion::~ImgSubRegion()
    {
    }

    bool ImgSubRegion::MergeRegion(const ImgSubRegion& imgNeighbor, ImgSubRegion& mergedRegion)
    {
        if (mLeftTopY == imgNeighbor.mLeftTopY)
        {
            if (mLeftTopX + mWidth == imgNeighbor.mLeftTopX)
            {
                mergedRegion.mLeftTopX = mLeftTopX;
                mergedRegion.mLeftTopY = mLeftTopY;
                mergedRegion.mWidth = mWidth + imgNeighbor.mWidth;
                mergedRegion.mHeight = mHeight;
                mergedRegion.mAvgPixel = (mAvgPixel + imgNeighbor.mAvgPixel) / 2.0;
                return true;
            }
            else if (imgNeighbor.mLeftTopX + imgNeighbor.mWidth == mLeftTopX)
            {
                mergedRegion.mLeftTopX = imgNeighbor.mLeftTopX;
                mergedRegion.mLeftTopY = mLeftTopY;
                mergedRegion.mWidth = mWidth + imgNeighbor.mWidth;
                mergedRegion.mHeight = mHeight;
                mergedRegion.mAvgPixel = (mAvgPixel + imgNeighbor.mAvgPixel) / 2.0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (mLeftTopX == imgNeighbor.mLeftTopX)
        {
            if (mLeftTopY + mHeight == imgNeighbor.mLeftTopY)
            {
                mergedRegion.mLeftTopX = mLeftTopX;
                mergedRegion.mLeftTopY = mLeftTopY;
                mergedRegion.mWidth = mWidth;
                mergedRegion.mHeight = mHeight + imgNeighbor.mHeight;
                mergedRegion.mAvgPixel = (mAvgPixel + imgNeighbor.mAvgPixel) / 2.0;
                return true;
            }
            else if (imgNeighbor.mLeftTopY + imgNeighbor.mHeight == mLeftTopY)
            {
                mergedRegion.mLeftTopX = mLeftTopX;
                mergedRegion.mLeftTopY = imgNeighbor.mLeftTopY;
                mergedRegion.mWidth = mWidth;
                mergedRegion.mHeight = mHeight + imgNeighbor.mHeight;
                mergedRegion.mAvgPixel = (mAvgPixel + imgNeighbor.mAvgPixel) / 2.0;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    void ImgSubRegion::CalculateAvgPixel(const cv::Mat& img)
    {
        mAvgPixel = MagicMath::Vector3(0, 0, 0);
        for (int hid = mLeftTopY; hid < mLeftTopY + mHeight; hid++)
        {
            for (int wid = mLeftTopX; wid < mLeftTopX + mWidth; wid++)
            {
                const unsigned char* pixel = img.ptr(hid, wid);
                mAvgPixel[0] += pixel[0];
                mAvgPixel[1] += pixel[1];
                mAvgPixel[2] += pixel[2];
            }
        }
        mAvgPixel /= (mHeight * mWidth);
    }

    cv::Mat SaliencyDetection::MultiScaleDoGBandSaliency(const cv::Mat& inputImg, int wNum, int hNum)
    {
        cv::Mat cvtImg;
        cv::cvtColor(inputImg, cvtImg, CV_BGR2Lab);
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;

        //calculate pixel average value
        std::vector<std::vector<MagicMath::Vector3> > avgImg(inputH);
        for (int hid = 0; hid < inputH; hid++)
        {
            avgImg.at(hid) = std::vector<MagicMath::Vector3>(inputW);
        }
        for (int wid = 0; wid < inputW; wid++)
        {
            for (int hid = 1; hid < inputH - 1; hid++)
            {
                MagicMath::Vector3 avgV(0, 0, 0);
                unsigned char* pixel = cvtImg.ptr(hid, wid);
                unsigned char* pixelTop  = cvtImg.ptr(hid + 1, wid);
                unsigned char* pixelDown = cvtImg.ptr(hid - 1, wid);
                for (int i = 0; i < 3; i++)
                {
                    avgV[i] += pixel[i] + pixelTop[i] + pixelDown[i];
                }
                avgImg.at(hid).at(wid) = avgV;
            }
        }
        std::vector<MagicMath::Vector3> avgRowTemp(inputW);
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 1; wid < inputW - 1; wid++)
            {
                avgRowTemp.at(wid) = (avgImg.at(hid).at(wid - 1) + avgImg.at(hid).at(wid) + avgImg.at(hid).at(wid + 1)) / 9.0;
            }
            avgImg.at(hid) = avgRowTemp;
        }
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                if (hid == 0 || hid == inputH - 1 || wid == 0 || wid == inputW - 1)
                {
                    unsigned char* pixel = cvtImg.ptr(hid, wid);
                    avgImg.at(hid).at(wid)[0] = pixel[0];
                    avgImg.at(hid).at(wid)[1] = pixel[1];
                    avgImg.at(hid).at(wid)[2] = pixel[2];
                }
            }
        }

        //construct a sub-region matrix
        int wDelta = inputW / wNum;
        int hDelta = inputH / hNum;
        std::vector<std::vector<ImgSubRegion> > subRgnMat(hNum);
        for (int hid = 0; hid < hNum; hid++)
        {
            std::vector<ImgSubRegion> subRgnRow(wNum);
            for (int wid = 0; wid < wNum; wid++)
            {
                int leftTopX = wid * wDelta;
                int leftTopY = hid * hDelta;
                int width = wDelta;
                int height = hDelta;
                if (wid == wNum - 1)
                {
                    width = inputW - wDelta * (wNum - 1);
                }
                if (hid == hNum - 1)
                {
                    height = inputH - hDelta * (hNum - 1);
                }
                ImgSubRegion subRgn(leftTopX, leftTopY, width, height);
                subRgn.CalculateAvgPixel(cvtImg);
                subRgnRow.at(wid) = subRgn;
            }
            subRgnMat.at(hid) = subRgnRow;
        }
        
        int subNumH = subRgnMat.size();
        int subNumW = subRgnMat.at(0).size();
        std::vector<std::vector<int> > saliencyMat(inputH);
        for (int hid = 0; hid < inputH; hid++)
        {
            saliencyMat.at(hid) = std::vector<int>(inputW, 0);
        }
        while (true)
        {
            //calculate sub-region saliency
            for (int subHid = 0; subHid < subNumH; subHid++)
            {
                for (int subWid = 0; subWid < subNumW; subWid++)
                {
                    ImgSubRegion& subRgn = subRgnMat.at(subHid).at(subWid);
                    for (int hid = subRgn.mLeftTopY; hid < subRgn.mLeftTopY + subRgn.mHeight; hid++)
                    {
                        for (int wid = subRgn.mLeftTopX; wid < subRgn.mLeftTopX + subRgn.mWidth; wid++)
                        {
                            saliencyMat.at(hid).at(wid) += (subRgn.mAvgPixel - avgImg.at(hid).at(wid)).Length();
                        }
                    }
                }
            }

            //combine neighbor sub-region
            if (subNumH == 1 && subNumW == 1)
            {
                break;
            }

            int subNumHNext = subNumH / 2 + subNumH % 2;
            int subNumWNext = subNumW / 2 + subNumW % 2;
            std::vector<std::vector<ImgSubRegion> > subRgnMatNext(subNumW);
            int combinedColNum = subNumH - subNumH % 2;
            for (int subWid = 0; subWid < subNumW; subWid++)
            {
                std::vector<ImgSubRegion> subRgnColNext;
                for (int subHid = 0; subHid < combinedColNum; subHid += 2)
                {
                    ImgSubRegion mergedRgn;
                    if ( subRgnMat.at(subHid).at(subWid).MergeRegion(subRgnMat.at(subHid + 1).at(subWid), mergedRgn) )
                    {
                        subRgnColNext.push_back(mergedRgn);
                    }
                    else
                    {
                        DebugLog << "Merge Col Region Error" << std::endl;
                    }
                }
                if (subNumH % 2)
                {
                    subRgnColNext.push_back(subRgnMat.at(subNumH - 1).at(subWid));
                }
                subRgnMatNext.at(subWid) = subRgnColNext;
            }
            int combinedRowNum = subNumW - subNumW % 2;
            subRgnMat.clear();
            subRgnMat = std::vector<std::vector<ImgSubRegion> >(subNumHNext);
            for (int subHid = 0; subHid < subNumHNext; subHid++)
            {
                std::vector<ImgSubRegion> subRgnRowNext;
                for (int subWid = 0; subWid < combinedRowNum; subWid += 2)
                {
                    ImgSubRegion mergedRgn;
                    if ( subRgnMatNext.at(subWid).at(subHid).MergeRegion(subRgnMatNext.at(subWid + 1).at(subHid), mergedRgn) )
                    {
                        subRgnRowNext.push_back(mergedRgn);
                    }
                    else
                    {
                        DebugLog << "Merge Row Region Error" << std::endl;
                    }
                }
                if (subNumW % 2)
                {
                    subRgnRowNext.push_back(subRgnMatNext.at(subNumW - 1).at(subHid));
                }
                subRgnMat.at(subHid) = subRgnRowNext;
            }
            subNumH = subNumHNext;
            subNumW = subNumWNext;
        }
        
        //Update Saliency Image
        int maxSaliency = 0;
        for (int hid = 0; hid < inputH; hid++)
        {
            for (int wid = 0; wid < inputW; wid++)
            {
                if (saliencyMat.at(hid).at(wid) > maxSaliency)
                {
                    maxSaliency = saliencyMat.at(hid).at(wid);
                }
            }
        }
        cv::Size imgSize(inputW, inputH);
        cv::Mat saliencyImg(imgSize, CV_8UC3);
        if (maxSaliency == 0)
        {
            DebugLog << "No Saliency Detected" << std::endl;
            for (int hid = 0; hid < inputH; hid++)
            {
                for (int wid = 0; wid < inputW; wid++)
                {
                    unsigned char* pixel = saliencyImg.ptr(hid, wid);
                    pixel[0] = 0;
                    pixel[1] = 0;
                    pixel[2] = 0;
                }
            }
        }
        else
        {
            for (int hid = 0; hid < inputH; hid++)
            {
                for (int wid = 0; wid < inputW; wid++)
                {
                    unsigned char* pixel = saliencyImg.ptr(hid, wid);
                    pixel[0] = saliencyMat.at(hid).at(wid) * 255 / maxSaliency;
                    pixel[1] = saliencyMat.at(hid).at(wid) * 255 / maxSaliency;
                    pixel[2] = saliencyMat.at(hid).at(wid) * 255 / maxSaliency;
                }
            }
        }

        return saliencyImg;
    }
}
