#include "Deformation.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Tool/LogSystem.h"

namespace MagicDIP
{
    Deformation::Deformation()
    {
    }

    Deformation::~Deformation()
    {
    }

    cv::Mat Deformation::DeformByMovingLeastSquares(const cv::Mat& inputImg, 
            const std::vector<int>& originIndex, const std::vector<int>& targetIndex)
    {
        int imgW = inputImg.cols;
        int imgH = inputImg.rows;
        cv::Size imgSize(imgW, imgH);
        cv::Mat resImg(imgSize, CV_8UC3);
        int markNum = originIndex.size() / 2;
        std::vector<double> wList(markNum);
        std::vector<MagicMath::Vector2> pHatList(markNum);
        std::vector<MagicMath::Vector2> qHatList(markNum);
        MagicMath::Vector2 pStar, qStar;
        std::vector<MagicMath::Vector2> pList(markNum);
        for (int mid = 0; mid < markNum; mid++)
        {
            pList.at(mid) = MagicMath::Vector2(originIndex.at(mid * 2), originIndex.at(mid * 2 + 1));
        }
        std::vector<MagicMath::Vector2> qList(markNum);
        for (int mid = 0; mid < markNum; mid++)
        {
            qList.at(mid) = MagicMath::Vector2(targetIndex.at(mid * 2), targetIndex.at(mid * 2 + 1));
        }
        std::vector<std::vector<double> > aMatList(markNum);
        std::vector<bool> visitFlag(imgW * imgH, 0);

        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                MagicMath::Vector2 pos(wid, hid);
                //calculate w
                bool isMarkVertex = false;
                int markedIndex = -1;
                double wSum = 0;
                for (int mid = 0; mid < markNum; mid++)
                {
                    //double dTemp = (pos - pList.at(mid)).LengthSquared(); //variable
                    double dTemp = (pos - pList.at(mid)).Length();
                    //dTemp = pow(dTemp, 1.25);
                    if (dTemp < 1.0e-15)
                    {
                        isMarkVertex = true;
                        markedIndex = mid;
                        break;
                    }
                    dTemp = pow(dTemp, 1.25);
                    wList.at(mid) = 1.0 / dTemp;
                    wSum += wList.at(mid);
                }
                //
                if (isMarkVertex)
                {
                    const unsigned char* pPixel = inputImg.ptr(hid, wid);
                    int targetH = targetIndex.at(2 * markedIndex + 1);
                    int targetW = targetIndex.at(2 * markedIndex);
                    unsigned char* pResPixel = resImg.ptr(targetH, targetW);
                    pResPixel[0] = pPixel[0];
                    pResPixel[1] = pPixel[1];
                    pResPixel[2] = pPixel[2];
                    visitFlag.at(targetH * imgW + targetW) = 1;
                }
                else
                {
                    //Calculate pStar qStar
                    pStar = MagicMath::Vector2(0.0, 0.0);
                    qStar = MagicMath::Vector2(0.0, 0.0);
                    for (int mid = 0; mid < markNum; mid++)
                    {
                        pStar += (pList.at(mid) * wList.at(mid));
                        qStar += (qList.at(mid) * wList.at(mid));
                    }
                    pStar /= wSum;
                    qStar /= wSum;

                    //Calculate pHat qHat
                    for (int mid = 0; mid < markNum; mid++)
                    {
                        pHatList.at(mid) = pList.at(mid) - pStar;
                        qHatList.at(mid) = qList.at(mid) - qStar;
                    }
                    
                    //Calculate A
                    MagicMath::Vector2 col0 = pos - pStar;
                    MagicMath::Vector2 col1(col0[1], -col0[0]);
                    for (int mid = 0; mid < markNum; mid++)
                    {
                        std::vector<double> aMat(4);
                        MagicMath::Vector2 row1(pHatList.at(mid)[1], -pHatList.at(mid)[0]);
                        aMat.at(0) = pHatList.at(mid) * col0 * wList.at(mid);
                        aMat.at(1) = pHatList.at(mid) * col1 * wList.at(mid);
                        aMat.at(2) = row1 * col0 * wList.at(mid);
                        aMat.at(3) = row1 * col1 * wList.at(mid);
                        aMatList.at(mid) = aMat;
                    }

                    //Calculate fr(v)
                    MagicMath::Vector2 fVec(0, 0);
                    for (int mid = 0; mid < markNum; mid++)
                    {
                        fVec[0] += (qHatList.at(mid)[0] * aMatList.at(mid).at(0) + qHatList.at(mid)[1] * aMatList.at(mid).at(2));
                        fVec[1] += (qHatList.at(mid)[0] * aMatList.at(mid).at(1) + qHatList.at(mid)[1] * aMatList.at(mid).at(3));
                    }

                    //Calculate target position
                    fVec.Normalise();
                    MagicMath::Vector2 targetPos = fVec * ((pos - pStar).Length()) + qStar;
                    int targetW = targetPos[0];
                    int targetH = targetPos[1];
                    if (targetH >= 0 && targetH < imgH && targetW >= 0 && targetW < imgW)
                    {
                        const unsigned char* pPixel = inputImg.ptr(hid, wid);
                        unsigned char* pResPixel = resImg.ptr(targetH, targetW);
                        pResPixel[0] = pPixel[0];
                        pResPixel[1] = pPixel[1];
                        pResPixel[2] = pPixel[2];
                        visitFlag.at(targetH * imgW + targetW) = 1;
                    }
                }
            }
        }

        std::vector<int> unVisitVecH;
        std::vector<int> unVisitVecW;
        for (int hid = 0; hid < imgH; hid++)
        {
            int baseIndex = hid * imgW;
            for (int wid = 0; wid < imgW; wid++)
            {
                if (!visitFlag.at(baseIndex + wid))
                {
                    unVisitVecH.push_back(hid);
                    unVisitVecW.push_back(wid);
                }
            }
        }
        int minAcceptSize = 4;
        int fillTime = 1;
        while (unVisitVecH.size() > 0)
        {
            DebugLog << "unVisit number: " << unVisitVecH.size() << std::endl;
            std::vector<int> unVisitVecHCopy = unVisitVecH;
            std::vector<int> unVisitVecWCopy = unVisitVecW;
            unVisitVecH.clear();
            unVisitVecW.clear();
            int unVisitSize = unVisitVecHCopy.size();
            for (int uid = 0; uid < unVisitSize; uid++)
            {
                MagicMath::Vector3 avgColor(0, 0, 0);
                int hid = unVisitVecHCopy.at(uid);
                int wid = unVisitVecWCopy.at(uid);
                int avgSize = 0;
                if ((hid - 1) >= 0 && visitFlag.at((hid - 1) * imgW + wid))
                {
                    unsigned char* pPixel = resImg.ptr(hid - 1, wid);
                    avgColor[0] += pPixel[0];
                    avgColor[1] += pPixel[1];
                    avgColor[2] += pPixel[2];
                    avgSize++;
                }
                if ((hid + 1) < imgH && visitFlag.at((hid + 1) * imgW + wid))
                {
                    unsigned char* pPixel = resImg.ptr(hid + 1, wid);
                    avgColor[0] += pPixel[0];
                    avgColor[1] += pPixel[1];
                    avgColor[2] += pPixel[2];
                    avgSize++;
                }
                if ((wid - 1) >= 0 && visitFlag.at(hid * imgW + wid - 1))
                {
                    unsigned char* pPixel = resImg.ptr(hid, wid - 1);
                    avgColor[0] += pPixel[0];
                    avgColor[1] += pPixel[1];
                    avgColor[2] += pPixel[2];
                    avgSize++;
                }
                if ((wid + 1) < imgW && visitFlag.at(hid * imgW + wid + 1))
                {
                    unsigned char* pPixel = resImg.ptr(hid, wid + 1);
                    avgColor[0] += pPixel[0];
                    avgColor[1] += pPixel[1];
                    avgColor[2] += pPixel[2];
                    avgSize++;
                }
                if (avgSize >= minAcceptSize)
                {
                    visitFlag.at(hid * imgW + wid) = 1;
                    avgColor /= avgSize;
                    unsigned char* pFillPixel = resImg.ptr(hid, wid);
                    pFillPixel[0] = avgColor[0];
                    pFillPixel[1] = avgColor[1];
                    pFillPixel[2] = avgColor[2];
                }
                else
                {
                    unVisitVecH.push_back(hid);
                    unVisitVecW.push_back(wid);
                }
            }
            if (fillTime == 4)
            {
                minAcceptSize--;
            }
            else if (fillTime == 6)
            {
                minAcceptSize--;
            }
            else if (fillTime == 8)
            {
                minAcceptSize--;
            }
            fillTime++;
        }
        //fill hole
        /*for (int hid = 0; hid < imgH; hid++)
        {
            int baseIndex = hid * imgW;
            for (int wid = 0; wid < imgW; wid++)
            {
                if (!visitFlag.at(baseIndex + wid))
                {
                    double wSum = 0;
                    MagicMath::Vector3 avgColor(0, 0, 0);
                    for (int wRight = wid + 1; wRight < imgW; wRight++)
                    {
                        if (visitFlag.at(baseIndex + wRight))
                        {
                            double wTemp = 1.0 / (wRight - wid);
                            wSum += wTemp;
                            unsigned char* pPixel = resImg.ptr(hid, wRight);
                            avgColor[0] += wTemp * pPixel[0];
                            avgColor[1] += wTemp * pPixel[1];
                            avgColor[2] += wTemp * pPixel[2];
                            break;
                        }
                    }
                    for (int wLeft = wid - 1; wLeft >= 0; wLeft--)
                    {
                        if (visitFlag.at(baseIndex + wLeft))
                        {
                            double wTemp = 1.0 / (wid - wLeft);
                            wSum += wTemp;
                            unsigned char* pPixel = resImg.ptr(hid, wLeft);
                            avgColor[0] += wTemp * pPixel[0];
                            avgColor[1] += wTemp * pPixel[1];
                            avgColor[2] += wTemp * pPixel[2];
                            break;
                        }
                    }
                    for (int hUp = hid - 1; hUp >= 0; hUp--)
                    {
                        if (visitFlag.at(hUp * imgW + wid))
                        {
                            double wTemp = 1.0 / (hid - hUp);
                            unsigned char* pPixel = resImg.ptr(hUp, wid);
                            wSum += wTemp;
                            avgColor[0] += wTemp * pPixel[0];
                            avgColor[1] += wTemp * pPixel[1];
                            avgColor[2] += wTemp * pPixel[2];
                            break;
                        }
                    }
                    for (int hDown = hid + 1; hDown < imgH; hDown++)
                    {
                        if (visitFlag.at(hDown * imgW + wid))
                        {
                            double wTemp = 1.0 / (hDown - hid);
                            unsigned char* pPixel = resImg.ptr(hDown, wid);
                            wSum += wTemp;
                            avgColor[0] += wTemp * pPixel[0];
                            avgColor[1] += wTemp * pPixel[1];
                            avgColor[2] += wTemp * pPixel[2];
                            break;
                        }
                    }
                    if (wSum > 1.0e-15)
                    {
                        avgColor /= wSum;
                    }
                    unsigned char* pFillPixel = resImg.ptr(hid, wid);
                    pFillPixel[0] = avgColor[0];
                    pFillPixel[1] = avgColor[1];
                    pFillPixel[2] = avgColor[2];
                }
            }
        }*/
        return resImg;
    }
}
