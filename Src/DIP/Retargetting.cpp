#include "Retargetting.h"
#include <vector>
#include <math.h>

namespace MagicDIP
{
    Retargetting::Retargetting()
    {
    }

    Retargetting::~Retargetting()
    {
    }

    cv::Mat Retargetting::SeamCarvingResizing(const cv::Mat& inputImg, int targetW, int targetH)
    {
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        int horizontalTime = inputW - targetW;
        if (horizontalTime > 0)
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                float wScale = float(targetW) / inputW;
                float hScale = float(targetH) / inputH;
                if (wScale > hScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW;
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * hScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
            else
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = int(inputW * float(targetH) / inputH);
                int tempH = targetH;
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                SeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                cv::Mat imgRes = tempImg.colRange(0, targetW);
                return imgRes;
            }
        }
        else
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = targetW; 
                int tempH = int(inputH * float(targetW) / inputW);
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                SeamCarvingVertical(tempImg, tempW, tempH, targetH);
                cv::Mat imgRes = tempImg.rowRange(0, targetH);
                return imgRes;
            }
            else
            {
                float wScale = float(targetW) / inputW;
                float HScale = float(targetH) / inputH;
                if (wScale > HScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW; 
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * HScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
        }
    }

    void Retargetting::SeamCarvingHorizontal(cv::Mat& img, int originW, int originH, int targetW)
    {
        int cutTimes = originW - targetW;
        std::vector<std::vector<int> > gradMat(originH);
        for (int hid = 0; hid < originH; hid++)
        {
            std::vector<int> gradList(originW);
            for (int wid = 0; wid < originW; wid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
                }
                gradList.at(wid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            }
            gradMat.at(hid) = gradList;
        }
        std::vector<std::vector<int> > traceIndex(originH);
        std::vector<int> curM(originW);
        std::vector<int> lastM(originW, 0);
        std::vector<int> traceLastIndex(originW);
        for (int cutIndex = 0; cutIndex < cutTimes; cutIndex++)
        {
            int curW = originW - cutIndex;
            for (int hid = 0; hid < originH; hid++)
            {
                std::vector<int>& gradList = gradMat.at(hid);
                //wid == 0
                {
                    if (lastM.at(1) < lastM.at(0))
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(1);
                        traceLastIndex.at(0) = 1;
                    }
                    else
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(0);
                        traceLastIndex.at(0) = 0;
                    }
                }
                //0 < wid < curW - 1
                for (int wid = 1; wid < curW - 1; wid++)
                {
                    int traceTemp = wid;
                    if (lastM.at(wid - 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid - 1;
                    }
                    if (lastM.at(wid + 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid + 1;
                    }
                    curM.at(wid) = gradList.at(wid) + lastM.at(traceTemp);
                    traceLastIndex.at(wid) = traceTemp;
                }
                //wid == curW - 1
                {
                    if (lastM.at(curW - 2) < lastM.at(curW - 1))
                    {
                        curM.at(curW - 1) = gradList.at(curW - 1) + lastM.at(curW - 2);
                        traceLastIndex.at(curW - 1) = curW - 2;
                    }
                    else
                    {
                        curM.at(curW - 1) = gradList.at(curW - 1) + lastM.at(curW - 1);
                        traceLastIndex.at(curW - 1) = curW - 1;
                    }
                }
                //
                traceIndex.at(hid) = traceLastIndex;
                lastM = curM;
            }
            //chose the minimal cut index
            int minimalIndex = 0;
            for (int wid = 0; wid < curW; wid++)
            {
                if (curM.at(wid) < curM.at(minimalIndex))
                {
                    minimalIndex = wid;
                }
            }
            //move pixel and gradMat
            int cutPos = minimalIndex;
            for (int hid = originH - 1; hid >= 0; hid--)
            {
                for (int mid = cutPos; mid < curW - 1; mid++)
                {
                    unsigned char* pixel = img.ptr(hid, mid);
                    unsigned char* pixelNext = img.ptr(hid, mid + 1);
                    pixel[0] = pixelNext[0];
                    pixel[1] = pixelNext[1];
                    pixel[2] = pixelNext[2];
                    gradMat.at(hid).at(mid) = gradMat.at(hid).at(mid + 1);
                }
                cutPos = traceIndex.at(hid).at(cutPos);
            }
            //re-calculate cut grad
            cutPos = minimalIndex;
            for (int hid = originH - 1; hid >= 0; hid--)
            {
                if (cutPos < curW - 1)
                {
                    unsigned char* pixel = img.ptr(hid, cutPos);
                    unsigned char* pixelWNext = NULL;
                    if (cutPos == 0)
                    {
                        pixelWNext = img.ptr(hid, cutPos + 1);
                    }
                    else
                    {
                        pixelWNext = img.ptr(hid, cutPos - 1);
                    }
                    unsigned char* pixelHNext = NULL;
                    if (hid == 0)
                    {
                        pixelHNext = img.ptr(hid + 1, cutPos);
                    }
                    else
                    {
                        pixelHNext = img.ptr(hid - 1, cutPos);
                    }
                    gradMat.at(hid).at(cutPos) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                        abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
                }
                cutPos = traceIndex.at(hid).at(cutPos);
            }
        }
    }

    void Retargetting::SeamCarvingVertical(cv::Mat& img, int originW, int originH, int targetH)
    {
        int cutTimes = originH - targetH;
        std::vector<std::vector<int> > gradMat(originW);
        for (int wid = 0; wid < originW; wid++)
        {
            std::vector<int> gradList(originH);
            for (int hid = 0; hid < originH; hid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
                }
                gradList.at(hid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            }
            gradMat.at(wid) = gradList;
        }
        std::vector<std::vector<int> > traceIndex(originW);
        std::vector<int> curM(originH);
        std::vector<int> lastM(originH, 0);
        std::vector<int> traceLastIndex(originH);
        for (int cutIndex = 0; cutIndex < cutTimes; cutIndex++)
        {
            int curH = originH - cutIndex;
            for (int wid = 0; wid < originW; wid++)
            {
                std::vector<int>& gradList = gradMat.at(wid);
                //wid == 0
                {
                    if (lastM.at(1) < lastM.at(0))
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(1);
                        traceLastIndex.at(0) = 1;
                    }
                    else
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(0);
                        traceLastIndex.at(0) = 0;
                    }
                }
                //0 < wid < curW - 1
                for (int hid = 1; hid < curH - 1; hid++)
                {
                    int traceTemp = hid;
                    if (lastM.at(hid - 1) < lastM.at(traceTemp))
                    {
                        traceTemp = hid - 1;
                    }
                    if (lastM.at(hid + 1) < lastM.at(traceTemp))
                    {
                        traceTemp = hid + 1;
                    }
                    curM.at(hid) = gradList.at(hid) + lastM.at(traceTemp);
                    traceLastIndex.at(hid) = traceTemp;
                }
                //wid == curW - 1
                {
                    if (lastM.at(curH - 2) < lastM.at(curH - 1))
                    {
                        curM.at(curH - 1) = gradList.at(curH - 1) + lastM.at(curH - 2);
                        traceLastIndex.at(curH - 1) = curH - 2;
                    }
                    else
                    {
                        curM.at(curH - 1) = gradList.at(curH - 1) + lastM.at(curH - 1);
                        traceLastIndex.at(curH - 1) = curH - 1;
                    }
                }
                //
                traceIndex.at(wid) = traceLastIndex;
                lastM = curM;
            }
            //chose the minimal cut index
            int minimalIndex = 0;
            for (int hid = 0; hid < curH; hid++)
            {
                if (curM.at(hid) < curM.at(minimalIndex))
                {
                    minimalIndex = hid;
                }
            }
            //move pixel and gradMat
            int cutPos = minimalIndex;
            //for (int wid = 0; wid < originW; wid++)
            for (int wid = originW - 1; wid >= 0; wid--)
            {
                for (int mid = cutPos; mid < curH - 1; mid++)
                {
                    unsigned char* pixel = img.ptr(mid, wid);
                    unsigned char* pixelNext = img.ptr(mid + 1, wid);
                    pixel[0] = pixelNext[0];
                    pixel[1] = pixelNext[1];
                    pixel[2] = pixelNext[2];
                    gradMat.at(wid).at(mid) = gradMat.at(wid).at(mid + 1);
                }
                cutPos = traceIndex.at(wid).at(cutPos);
            }
            //re-calculate cut grad
            cutPos = minimalIndex;
            //for (int wid = 0; wid < originW; wid++)
            for (int wid = originW - 1; wid >= 0; wid--)
            {
                if (cutPos < curH - 1)
                {
                    unsigned char* pixel = img.ptr(cutPos, wid);
                    unsigned char* pixelHNext = NULL;
                    if (cutPos == 0)
                    {
                        pixelHNext = img.ptr(cutPos + 1, wid);
                    }
                    else
                    {
                        pixelHNext = img.ptr(cutPos - 1, wid);
                    }
                    unsigned char* pixelWNext = NULL;
                    if (wid == 0)
                    {
                        pixelWNext = img.ptr(cutPos, wid + 1);
                    }
                    else
                    {
                        pixelWNext = img.ptr(cutPos, wid - 1);
                    }
                    gradMat.at(wid).at(cutPos) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                        abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
                }
                cutPos = traceIndex.at(wid).at(cutPos);
            }
        }
    }

    cv::Mat Retargetting::FastSeamCarvingResizing(const cv::Mat& inputImg, int targetW, int targetH)
    {
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        int horizontalTime = inputW - targetW;
        if (horizontalTime > 0)
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                float wScale = float(targetW) / inputW;
                float hScale = float(targetH) / inputH;
                if (wScale > hScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW;
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    FastSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * hScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    FastSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
            else
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = int(inputW * float(targetH) / inputH);
                int tempH = targetH;
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                FastSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                cv::Mat imgRes = tempImg.colRange(0, targetW);
                return imgRes;
            }
        }
        else
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = targetW; 
                int tempH = int(inputH * float(targetW) / inputW);
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                FastSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                cv::Mat imgRes = tempImg.rowRange(0, targetH);
                return imgRes;
            }
            else
            {
                float wScale = float(targetW) / inputW;
                float HScale = float(targetH) / inputH;
                if (wScale > HScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW; 
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    FastSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * HScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    FastSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
        }
    }

    void Retargetting::FastSeamCarvingHorizontal(cv::Mat& img, int originW, int originH, int targetW)
    {
        std::vector<std::vector<int> > gradMat(originH);
        for (int hid = 0; hid < originH; hid++)
        {
            std::vector<int> gradList(originW);
            for (int wid = 0; wid < originW; wid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
                }
                gradList.at(wid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            }
            gradMat.at(hid) = gradList;
        }

        //calculate M 
        std::vector<std::vector<int> > mMat(originH);
        std::vector<int> mRow(originW);
        mMat.at(originH - 1) = gradMat.at(originH - 1);
        for (int hid = originH - 2; hid >= 0; hid--)
        {
            std::vector<int>& lastMRow = mMat.at(hid + 1);
            std::vector<int>& gradList = gradMat.at(hid);
            //wid == 0
            {
                if (lastMRow.at(1) < lastMRow.at(0))
                {
                    mRow.at(0) = gradList.at(0) + lastMRow.at(1);
                }
                else
                {
                    mRow.at(0) = gradList.at(0) + lastMRow.at(0);
                }
            }
            for (int wid = 1; wid < originW - 1; wid++)
            {
                int minIndex = wid;
                if (lastMRow.at(wid - 1) < lastMRow.at(minIndex))
                {
                    minIndex = wid - 1;
                }
                if (lastMRow.at(wid + 1) < lastMRow.at(minIndex))
                {
                    minIndex = wid + 1;
                }
                mRow.at(wid) = gradList.at(wid) + lastMRow.at(minIndex);
            }
            //wid == originW - 1
            {
                if (lastMRow.at(originW - 2) < lastMRow.at(originW - 1))
                {
                    mRow.at(originW - 1) = gradList.at(originW - 1) + lastMRow.at(originW - 2);
                }
                else
                {
                    mRow.at(originW - 1) = gradList.at(originW - 1) + lastMRow.at(originW - 1);
                }
            }
            mMat.at(hid) = mRow;
        }

        //calculate matching relations
        std::vector<std::vector<int> > matchingRelations(originH);
        std::vector<int> matchingList(originW);
        std::vector<int> aList = gradMat.at(0);
        for (int hid = 0; hid < originH - 1; hid++)
        {
            int wid = originW - 1;
            std::vector<int>& mList = mMat.at(hid + 1);
            while (wid >= 1)
            {
                int w0 = aList.at(wid) * mList.at(wid);
                int w1 = aList.at(wid) * mList.at(wid - 1);
                if (w1 > w0)
                {
                    matchingList.at(wid) = wid - 1;
                    matchingList.at(wid - 1) = wid;
                    wid -= 2;
                }
                else
                {
                    matchingList.at(wid) = wid;
                    wid--;
                }
            }
            if (wid == 0)
            {
                matchingList.at(wid) = wid;
            }
            matchingRelations.at(hid) = matchingList;
            //update aList
            std::vector<int> aListCopy = aList;
            std::vector<int>& gradList = gradMat.at(hid + 1);
            for (int wid = 0; wid < originW; wid++)
            {
                aList.at(matchingList.at(wid)) = aListCopy.at(wid) + gradList.at(matchingList.at(wid));
            }
        }
        matchingRelations.at(originH - 1) = std::vector<int>(originW, 1);

        //Calculate seam scores.
        std::vector<int> seamScores(originW);
        for (int wid = 0; wid < originW; wid++)
        {
            int score = 0;
            int curW = wid;
            for (int hid = 0; hid < originH; hid++)
            {
                score += gradMat.at(hid).at(curW);
                curW = matchingRelations.at(hid).at(curW);
            }
            seamScores.at(wid) = score;
        }

        //Find the minimal k seams.
        int cutTimes = originW - targetW;
        std::vector<int> seams;
        FindMinimalK(seamScores, cutTimes, seams);

        //mark seam pixel
        for (int sid = 0; sid < cutTimes; sid++)
        {
            int curSeamIndex = seams.at(sid);
            for (int hid = 0; hid < originH - 1; hid++)
            {
                int nextSeamIndex = matchingRelations.at(hid).at(curSeamIndex);
                matchingRelations.at(hid).at(curSeamIndex) = -1;
                curSeamIndex = nextSeamIndex;
            }
        }

        //copy valid pixels
        for (int hid = 0; hid < originH; hid++)
        {
            int validIndex = 0;
            std::vector<int>& markList = matchingRelations.at(hid);
            for (int wid = 0; wid < targetW; wid++)
            {
                while (markList.at(validIndex) == -1)
                {
                    validIndex++;
                }
                if (validIndex == wid)
                {
                    validIndex++;
                    continue;
                }
                else
                {
                    unsigned char* pixel = img.ptr(hid, wid);
                    unsigned char* pixelValid = img.ptr(hid, validIndex);
                    pixel[0] = pixelValid[0];
                    pixel[1] = pixelValid[1];
                    pixel[2] = pixelValid[2];
                    validIndex++;
                }
            }
        }
    }

    void Retargetting::FastSeamCarvingVertical(cv::Mat& img, int originW, int originH, int targetH)
    {
        std::vector<std::vector<int> > gradMat(originW);
        for (int wid = 0; wid < originW; wid++)
        {
            std::vector<int> gradList(originH);
            for (int hid = 0; hid < originH; hid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
                }
                gradList.at(hid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            }
            gradMat.at(wid) = gradList;
        }

        //calculate M 
        std::vector<std::vector<int> > mMat(originW);
        std::vector<int> mRow(originH);
        mMat.at(originW - 1) = gradMat.at(originW - 1);
        for (int wid = originW - 2; wid >= 0; wid--)
        {
            std::vector<int>& lastMRow = mMat.at(wid + 1);
            std::vector<int>& gradList = gradMat.at(wid);
            //wid == 0
            {
                if (lastMRow.at(1) < lastMRow.at(0))
                {
                    mRow.at(0) = gradList.at(0) + lastMRow.at(1);
                }
                else
                {
                    mRow.at(0) = gradList.at(0) + lastMRow.at(0);
                }
            }
            for (int hid = 1; hid < originH - 1; hid++)
            {
                int minIndex = hid;
                if (lastMRow.at(hid - 1) < lastMRow.at(minIndex))
                {
                    minIndex = hid - 1;
                }
                if (lastMRow.at(hid + 1) < lastMRow.at(minIndex))
                {
                    minIndex = hid + 1;
                }
                mRow.at(hid) = gradList.at(hid) + lastMRow.at(minIndex);
            }
            //wid == originW - 1
            {
                if (lastMRow.at(originH - 2) < lastMRow.at(originH - 1))
                {
                    mRow.at(originH - 1) = gradList.at(originH - 1) + lastMRow.at(originH - 2);
                }
                else
                {
                    mRow.at(originH - 1) = gradList.at(originH - 1) + lastMRow.at(originH - 1);
                }
            }
            mMat.at(wid) = mRow;
        }

        //calculate matching relations
        std::vector<std::vector<int> > matchingRelations(originW);
        std::vector<int> matchingList(originH);
        std::vector<int> aList = gradMat.at(0);
        for (int wid = 0; wid < originW - 1; wid++)
        {
            int hid = originH - 1;
            std::vector<int>& mList = mMat.at(wid + 1);
            while (hid >= 1)
            {
                int w0 = aList.at(hid) * mList.at(hid);
                int w1 = aList.at(hid) * mList.at(hid - 1);
                if (w1 > w0)
                {
                    matchingList.at(hid) = hid - 1;
                    matchingList.at(hid - 1) = hid;
                    hid -= 2;
                }
                else
                {
                    matchingList.at(hid) = hid;
                    hid--;
                }
            }
            if (hid == 0)
            {
                matchingList.at(hid) = hid;
            }
            matchingRelations.at(wid) = matchingList;
            //update aList
            std::vector<int> aListCopy = aList;
            std::vector<int>& gradList = gradMat.at(wid + 1);
            for (int hid = 0; hid < originH; hid++)
            {
                aList.at(matchingList.at(hid)) = aListCopy.at(hid) + gradList.at(matchingList.at(hid));
            }
        }
        matchingRelations.at(originW - 1) = std::vector<int>(originH, 1);

        //Calculate seam scores.
        std::vector<int> seamScores(originH);
        for (int hid = 0; hid < originH; hid++)
        {
            int score = 0;
            int curW = hid;
            for (int wid = 0; wid < originW; wid++)
            {
                score += gradMat.at(wid).at(curW);
                curW = matchingRelations.at(wid).at(curW);
            }
            seamScores.at(hid) = score;
        }

        //Find the minimal k seams.
        int cutTimes = originH - targetH;
        std::vector<int> seams;
        FindMinimalK(seamScores, cutTimes, seams);

        //mark seam pixel
        for (int sid = 0; sid < cutTimes; sid++)
        {
            int curSeamIndex = seams.at(sid);
            for (int wid = 0; wid < originW - 1; wid++)
            {
                int nextSeamIndex = matchingRelations.at(wid).at(curSeamIndex);
                matchingRelations.at(wid).at(curSeamIndex) = -1;
                curSeamIndex = nextSeamIndex;
            }
        }

        //copy valid pixels
        for (int wid = 0; wid < originW; wid++)
        {
            int validIndex = 0;
            std::vector<int>& markList = matchingRelations.at(wid);
            for (int hid = 0; hid < targetH; hid++)
            {
                while (markList.at(validIndex) == -1)
                {
                    validIndex++;
                }
                if (validIndex == hid)
                {
                    validIndex++;
                    continue;
                }
                else
                {
                    unsigned char* pixel = img.ptr(hid, wid);
                    unsigned char* pixelValid = img.ptr(validIndex, wid);
                    pixel[0] = pixelValid[0];
                    pixel[1] = pixelValid[1];
                    pixel[2] = pixelValid[2];
                    validIndex++;
                }
            }
        }
    }

    void Retargetting::FindMinimalK(const std::vector<int>& scores, int k, std::vector<int>& minimalK)
    {
        int vNum = scores.size();
        minimalK.clear();
        minimalK.resize(k);
        std::vector<int> indexList(vNum);
        for (int i = 0; i < vNum; i++)
        {
            indexList.at(i) = i;
        }
        for (int kid = 0; kid < k; kid++)
        {
            int minIndex = kid;
            for (int i = kid; i < vNum; i++)
            {
                if (scores.at(indexList.at(i)) < scores.at(indexList.at(minIndex)))
                {
                    minIndex = i;
                }
            }
            minimalK.at(kid) = indexList.at(minIndex);
            int temp = indexList.at(minIndex);
            indexList.at(minIndex) = indexList.at(kid);
            indexList.at(kid) = temp;
        }
    }

    cv::Mat Retargetting::SaliencyBasedSeamCarvingResizing(const cv::Mat& inputImg, int targetW, int targetH)
    {
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        int horizontalTime = inputW - targetW;
        if (horizontalTime > 0)
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                float wScale = float(targetW) / inputW;
                float hScale = float(targetH) / inputH;
                if (wScale > hScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW;
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SaliencyBasedSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * hScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SaliencyBasedSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
            else
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = int(inputW * float(targetH) / inputH);
                int tempH = targetH;
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                SaliencyBasedSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                cv::Mat imgRes = tempImg.colRange(0, targetW);
                return imgRes;
            }
        }
        else
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = targetW; 
                int tempH = int(inputH * float(targetW) / inputW);
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                SaliencyBasedSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                cv::Mat imgRes = tempImg.rowRange(0, targetH);
                return imgRes;
            }
            else
            {
                float wScale = float(targetW) / inputW;
                float HScale = float(targetH) / inputH;
                if (wScale > HScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW; 
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SaliencyBasedSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * HScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    SaliencyBasedSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
        }
    }

    void Retargetting::SaliencyBasedSeamCarvingHorizontal(cv::Mat& img, int originW, int originH, int targetW)
    {
        //salient value
        std::vector<std::vector<int> > salientValue(originH);
        for (int hid = 0; hid < originH; hid++)
        {
            salientValue.at(hid) = std::vector<int>(originW, 0);
        }
        float meanPixel[3] = {0.f, 0.f, 0.f};
        for (int hid = 0; hid < originH; hid++)
        {
            for (int wid = 0; wid < originW; wid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                meanPixel[0] += pixel[0];
                meanPixel[1] += pixel[1];
                meanPixel[2] += pixel[2];
            }
        }
        meanPixel[0] /= (originH * originW);
        meanPixel[1] /= (originH * originW);
        meanPixel[2] /= (originH * originW);
        int maxSalientValue = 0;
        for (int hid = 2; hid < originH - 2; hid++)
        {
            for (int wid = 2; wid < originW - 2; wid++)
            {
                float gaussianPixel[3] = {0.f, 0.f, 0.f};
                for (int i = -2; i <= 2; i++)
                {
                    for (int j = -2; j <= 2; j++)
                    {
                        if (i == 0 && j == 0)
                        {
                            unsigned char* pixel = img.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 6;
                            gaussianPixel[1] += pixel[1] * 6;
                            gaussianPixel[2] += pixel[2] * 6;
                        }
                        if ((abs(i) == 1 && abs(j) <= 1) || (abs(i) <= 1 && abs(j) == 1))
                        {
                            unsigned char* pixel = img.ptr(hid + i, wid + j);
                            gaussianPixel[0] += pixel[0] * 4;
                            gaussianPixel[1] += pixel[1] * 4;
                            gaussianPixel[2] += pixel[2] * 4;
                        }
                        else
                        {
                            unsigned char* pixel = img.ptr(hid + i, wid + j);
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
                //DebugLog << "Saliency: " << int(fTemp) << std::endl;
            }
        }
        for (int hid = 0; hid < originH; hid++)
        {
            for (int wid = 0; wid < originW; wid++)
            {
                salientValue.at(hid).at(wid) = salientValue.at(hid).at(wid) * 255 / maxSalientValue;
            }
        }
        std::vector<std::vector<int> > gradMat(originH);
        int maxGrad = 0;
        for (int hid = 0; hid < originH; hid++)
        {
            std::vector<int> gradList(originW);
            for (int wid = 0; wid < originW; wid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
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
        for (int hid = 0; hid < originH; hid++)
        {
            for (int wid = 0; wid < originW; wid++)
            {
                gradMat.at(hid).at(wid) = gradMat.at(hid).at(wid) * 255 / maxGrad;
                if (gradMat.at(hid).at(wid) > salientValue.at(hid).at(wid))
                {
                    salientValue.at(hid).at(wid) = gradMat.at(hid).at(wid);
                }
            }
        }
        //

        int cutTimes = originW - targetW;
        std::vector<std::vector<int> > traceIndex(originH);
        std::vector<int> curM(originW);
        std::vector<int> lastM(originW, 0);
        std::vector<int> traceLastIndex(originW);
        for (int cutIndex = 0; cutIndex < cutTimes; cutIndex++)
        {
            int curW = originW - cutIndex;
            for (int hid = 0; hid < originH; hid++)
            {
                std::vector<int>& salientList = salientValue.at(hid);
                //wid == 0
                {
                    if (lastM.at(1) < lastM.at(0))
                    {
                        curM.at(0) = salientList.at(0) + lastM.at(1);
                        traceLastIndex.at(0) = 1;
                    }
                    else
                    {
                        curM.at(0) = salientList.at(0) + lastM.at(0);
                        traceLastIndex.at(0) = 0;
                    }
                }
                //0 < wid < curW - 1
                for (int wid = 1; wid < curW - 1; wid++)
                {
                    int traceTemp = wid;
                    if (lastM.at(wid - 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid - 1;
                    }
                    if (lastM.at(wid + 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid + 1;
                    }
                    curM.at(wid) = salientList.at(wid) + lastM.at(traceTemp);
                    traceLastIndex.at(wid) = traceTemp;
                }
                //wid == curW - 1
                {
                    if (lastM.at(curW - 2) < lastM.at(curW - 1))
                    {
                        curM.at(curW - 1) = salientList.at(curW - 1) + lastM.at(curW - 2);
                        traceLastIndex.at(curW - 1) = curW - 2;
                    }
                    else
                    {
                        curM.at(curW - 1) = salientList.at(curW - 1) + lastM.at(curW - 1);
                        traceLastIndex.at(curW - 1) = curW - 1;
                    }
                }
                //
                traceIndex.at(hid) = traceLastIndex;
                lastM = curM;
            }
            //chose the minimal cut index
            int minimalIndex = 0;
            for (int wid = 0; wid < curW; wid++)
            {
                if (curM.at(wid) < curM.at(minimalIndex))
                {
                    minimalIndex = wid;
                }
            }
            //move pixel and gradMat
            int cutPos = minimalIndex;
            int scale = 1;
            for (int hid = originH - 1; hid >= 0; hid--)
            {
                if (cutPos > 0)
                {
                    salientValue.at(hid).at(cutPos - 1) += salientValue.at(hid).at(cutPos) * scale;
                }
                if (cutPos < curW -1)
                {
                    salientValue.at(hid).at(cutPos + 1) += salientValue.at(hid).at(cutPos) * scale;
                }
                for (int mid = cutPos; mid < curW - 1; mid++)
                {
                    unsigned char* pixel = img.ptr(hid, mid);
                    unsigned char* pixelNext = img.ptr(hid, mid + 1);
                    pixel[0] = pixelNext[0];
                    pixel[1] = pixelNext[1];
                    pixel[2] = pixelNext[2];
                    salientValue.at(hid).at(mid) = salientValue.at(hid).at(mid + 1);
                }
                cutPos = traceIndex.at(hid).at(cutPos);
            }
        }
    }

    void Retargetting::SaliencyBasedSeamCarvingVertical(cv::Mat& img, int originW, int originH, int targetH)
    {

    }

    cv::Mat Retargetting::ImportanceDiffusionSeamCarvingResizing(const cv::Mat& inputImg, int targetW, int targetH)
    {
        int inputW = inputImg.cols;
        int inputH = inputImg.rows;
        int horizontalTime = inputW - targetW;
        if (horizontalTime > 0)
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                float wScale = float(targetW) / inputW;
                float hScale = float(targetH) / inputH;
                if (wScale > hScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW;
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    ImportanceDiffusionSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * hScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    ImportanceDiffusionSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
            else
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = int(inputW * float(targetH) / inputH);
                int tempH = targetH;
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                ImportanceDiffusionSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                cv::Mat imgRes = tempImg.colRange(0, targetW);
                return imgRes;
            }
        }
        else
        {
            int verticalTime = inputH - targetH;
            if (verticalTime > 0)
            {
                cv::Mat imgPro = inputImg.clone();
                int tempW = targetW; 
                int tempH = int(inputH * float(targetW) / inputW);
                cv::Size tempSize(tempW, tempH);
                cv::Mat tempImg(tempSize, CV_8UC3);
                cv::resize(imgPro, tempImg, tempSize);
                ImportanceDiffusionSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                cv::Mat imgRes = tempImg.rowRange(0, targetH);
                return imgRes;
            }
            else
            {
                float wScale = float(targetW) / inputW;
                float HScale = float(targetH) / inputH;
                if (wScale > HScale)
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = targetW; 
                    int tempH = int(inputH * wScale);
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    ImportanceDiffusionSeamCarvingVertical(tempImg, tempW, tempH, targetH);
                    cv::Mat imgRes = tempImg.rowRange(0, targetH);
                    return imgRes;
                }
                else
                {
                    cv::Mat imgPro = inputImg.clone();
                    int tempW = int(inputW * HScale);
                    int tempH = targetH;
                    cv::Size tempSize(tempW, tempH);
                    cv::Mat tempImg(tempSize, CV_8UC3);
                    cv::resize(imgPro, tempImg, tempSize);
                    ImportanceDiffusionSeamCarvingHorizontal(tempImg, tempW, tempH, targetW);
                    cv::Mat imgRes = tempImg.colRange(0, targetW);
                    return imgRes;
                }
            }
        }
    }

    void Retargetting::ImportanceDiffusionSeamCarvingHorizontal(cv::Mat& img, int originW, int originH, int targetW)
    {
        int cutTimes = originW - targetW;
        std::vector<std::vector<int> > gradMat(originH);
        for (int hid = 0; hid < originH; hid++)
        {
            std::vector<int> gradList(originW);
            for (int wid = 0; wid < originW; wid++)
            {
                unsigned char* pixel = img.ptr(hid, wid);
                unsigned char* pixelWNext = NULL;
                if (wid == 0)
                {
                    pixelWNext = img.ptr(hid, wid + 1);
                }
                else
                {
                    pixelWNext = img.ptr(hid, wid - 1);
                }
                unsigned char* pixelHNext = NULL;
                if (hid == 0)
                {
                    pixelHNext = img.ptr(hid + 1, wid);
                }
                else
                {
                    pixelHNext = img.ptr(hid - 1, wid);
                }
                gradList.at(wid) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
                    abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            }
            gradMat.at(hid) = gradList;
        }
        std::vector<std::vector<int> > traceIndex(originH);
        std::vector<int> curM(originW);
        std::vector<int> lastM(originW, 0);
        std::vector<int> traceLastIndex(originW);
        for (int cutIndex = 0; cutIndex < cutTimes; cutIndex++)
        {
            int curW = originW - cutIndex;
            for (int hid = 0; hid < originH; hid++)
            {
                std::vector<int>& gradList = gradMat.at(hid);
                //wid == 0
                {
                    if (lastM.at(1) < lastM.at(0))
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(1);
                        traceLastIndex.at(0) = 1;
                    }
                    else
                    {
                        curM.at(0) = gradList.at(0) + lastM.at(0);
                        traceLastIndex.at(0) = 0;
                    }
                }
                //0 < wid < curW - 1
                for (int wid = 1; wid < curW - 1; wid++)
                {
                    int traceTemp = wid;
                    if (lastM.at(wid - 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid - 1;
                    }
                    if (lastM.at(wid + 1) < lastM.at(traceTemp))
                    {
                        traceTemp = wid + 1;
                    }
                    curM.at(wid) = gradList.at(wid) + lastM.at(traceTemp);
                    traceLastIndex.at(wid) = traceTemp;
                }
                //wid == curW - 1
                {
                    if (lastM.at(curW - 2) < lastM.at(curW - 1))
                    {
                        curM.at(curW - 1) = gradList.at(curW - 1) + lastM.at(curW - 2);
                        traceLastIndex.at(curW - 1) = curW - 2;
                    }
                    else
                    {
                        curM.at(curW - 1) = gradList.at(curW - 1) + lastM.at(curW - 1);
                        traceLastIndex.at(curW - 1) = curW - 1;
                    }
                }
                //
                traceIndex.at(hid) = traceLastIndex;
                lastM = curM;
            }
            //chose the minimal cut index
            int minimalIndex = 0;
            for (int wid = 0; wid < curW; wid++)
            {
                if (curM.at(wid) < curM.at(minimalIndex))
                {
                    minimalIndex = wid;
                }
            }
            //move pixel and gradMat
            int cutPos = minimalIndex;
            int scale = 1;
            for (int hid = originH - 1; hid >= 0; hid--)
            {
                if (cutPos > 0)
                {
                    gradMat.at(hid).at(cutPos - 1) += gradMat.at(hid).at(cutPos) * scale;
                }
                if (cutPos < curW -1)
                {
                    gradMat.at(hid).at(cutPos + 1) += gradMat.at(hid).at(cutPos) * scale;
                }
                for (int mid = cutPos; mid < curW - 1; mid++)
                {
                    unsigned char* pixel = img.ptr(hid, mid);
                    unsigned char* pixelNext = img.ptr(hid, mid + 1);
                    pixel[0] = pixelNext[0];
                    pixel[1] = pixelNext[1];
                    pixel[2] = pixelNext[2];
                    gradMat.at(hid).at(mid) = gradMat.at(hid).at(mid + 1);
                }
                cutPos = traceIndex.at(hid).at(cutPos);
            }
            ////re-calculate cut grad
            //cutPos = minimalIndex;
            //for (int hid = originH - 1; hid >= 0; hid--)
            //{
            //    if (cutPos < curW - 1)
            //    {
            //        unsigned char* pixel = img.ptr(hid, cutPos);
            //        unsigned char* pixelWNext = NULL;
            //        if (cutPos == 0)
            //        {
            //            pixelWNext = img.ptr(hid, cutPos + 1);
            //        }
            //        else
            //        {
            //            pixelWNext = img.ptr(hid, cutPos - 1);
            //        }
            //        unsigned char* pixelHNext = NULL;
            //        if (hid == 0)
            //        {
            //            pixelHNext = img.ptr(hid + 1, cutPos);
            //        }
            //        else
            //        {
            //            pixelHNext = img.ptr(hid - 1, cutPos);
            //        }
            //        gradMat.at(hid).at(cutPos) = abs(pixel[0] - pixelWNext[0]) + abs(pixel[1] - pixelWNext[1]) + abs(pixel[2] - pixelWNext[2]) +
            //            abs(pixel[0] - pixelHNext[0]) + abs(pixel[1] - pixelHNext[1]) + abs(pixel[2] - pixelHNext[2]);
            //    }
            //    cutPos = traceIndex.at(hid).at(cutPos);
            //}
        }
    }

    void Retargetting::ImportanceDiffusionSeamCarvingVertical(cv::Mat& img, int originW, int originH, int targetH)
    {

    }
}