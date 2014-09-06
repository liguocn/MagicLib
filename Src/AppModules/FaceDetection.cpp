#include "FaceDetection.h"
#include "../DIP/RealTimeFaceDetection.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Math/HomoMatrix3.h"
#include <fstream>

namespace
{
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

    static double CalculateOverlapRate(int rowA, int colA, int lenA, int rowB, int colB, int lenB)
    {
        int overlapRow = CalCulateLineSegmentOverlap(rowA, lenA, rowB, lenB);
        int overLapCol = CalCulateLineSegmentOverlap(colA, lenA, colB, lenB);
        double areaA = lenA * lenA;
        double areaB = lenB * lenB;
        double areaCommon = overlapRow * overLapCol;
        double rateA = areaCommon / areaA;
        double rateB = areaCommon / areaB;
        return (rateA < rateB ? rateA : rateB);
    }
}

namespace MagicApp
{
    FaceDetection::FaceDetection() :
        mDm(DM_Default),
        mpRealTimeDetector(NULL)
    {
    }

    FaceDetection::~FaceDetection()
    {
        if (mpRealTimeDetector != NULL)
        {
            delete mpRealTimeDetector;
            mpRealTimeDetector = NULL;
        }
    }

    int FaceDetection::LearnDetector(const std::string& faceFile, const std::string& nonFaceFile, DetectionMethod dm)
    {
        switch (dm)
        {
        case DM_Default:
            return LearnRealTimeDetector(faceFile, nonFaceFile);
        default:
            return MAGIC_INVALID_INPUT;
        }
    }
     
    int FaceDetection::DetectFace(const cv::Mat& img, std::vector<int>& faces) const
    {
        switch (mDm)
        {
        case MagicApp::FaceDetection::DM_Default:
            if (mpRealTimeDetector == NULL)
            {
                return 0;
            }
            else
            {
                return mpRealTimeDetector->Detect(img, faces);
            }
        default:
            return 0;
        }
    }
     
    void FaceDetection::Save(const std::string& fileName, const std::string& detectFileName) const
    {
        std::ofstream fout(fileName);
        fout << mDm << std::endl;
        fout << detectFileName << std::endl;
        std::string filePath = fileName;
        std::string::size_type pos = filePath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = filePath.rfind("\\");
        }
        filePath.erase(pos);
        filePath += "/";
        std::string detectFileFullName = filePath + detectFileName;
        switch (mDm)
        {
        case DM_Default:
            if (mpRealTimeDetector != NULL)
            {
                mpRealTimeDetector->Save(detectFileFullName);
            }
            else
            {
                DebugLog << "Error: AdaBoostDetector is NULL" << std::endl;
            }
            break;
        default:
            break;
        }
        fout.close();
    }

    void FaceDetection::SaveByEnhanceThreshold(const std::string& fileName, const std::string& detectFileName, double percentage) const
    {
        std::ofstream fout(fileName);
        fout << mDm << std::endl;
        fout << detectFileName << std::endl;
        std::string filePath = fileName;
        std::string::size_type pos = filePath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = filePath.rfind("\\");
        }
        filePath.erase(pos);
        filePath += "/";
        std::string detectFileFullName = filePath + detectFileName;
        switch (mDm)
        {
        case DM_Default:
            if (mpRealTimeDetector != NULL)
            {
                //mpRealTimeDetector->Save(detectFileFullName);
                mpRealTimeDetector->SaveByEnhanceThreshold(percentage, detectFileFullName);
            }
            else
            {
                DebugLog << "Error: AdaBoostDetector is NULL" << std::endl;
            }
            break;
        default:
            break;
        }
        fout.close();
    }
     
    void FaceDetection::Load(const std::string& fileName)
    {
        std::ifstream fin(fileName);
        int dm;
        fin >> dm;
        mDm = DetectionMethod(dm);
        std::string detectorFile;
        fin >> detectorFile;
        fin.close();
        //
        std::string filePath = fileName;
        std::string::size_type pos = filePath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = filePath.rfind("\\");
        }
        filePath.erase(pos);
        filePath += "/";
        detectorFile = filePath + detectorFile;
        //
        switch (mDm)
        {
        case DM_Default:
            if (mpRealTimeDetector == NULL)
            {
                mpRealTimeDetector = new MagicDIP::RealTimeFaceDetection;
            }
            mpRealTimeDetector->Load(detectorFile);
            break;
        default:
            break;
        }
    }

    void FaceDetection::SaveFeatureAsImage(const std::string& filePath) const
    {
        mpRealTimeDetector->SaveFeatureAsImage(filePath);
    }

    int FaceDetection::LearnRealTimeDetector(const std::string& faceFile, const std::string& nonFaceFile)
    {
        std::vector<std::string> faceImgNames, nonFaceImgNames;
        int res = GetImageNames(faceFile, faceImgNames);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }
        res = GetImageNames(nonFaceFile, nonFaceImgNames);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }
        if (mpRealTimeDetector == NULL)
        {
            mpRealTimeDetector = new MagicDIP::RealTimeFaceDetection;
        }
        /*std::vector<int> layerCount(4, 5);
        for (int layerId = 0; layerId < 1000; layerId++)
        {
            layerCount.push_back(10);
        }*/
        /*int iterCount = 100;
        int levelCount = 8;
        int maxLevelCount = 512;
        int duplicateCount = 2;
        int increaseNum = 2;
        int maxDuplicateCount = 32;
        std::vector<int> layerCount;
        for (int iterId = 0; iterId < iterCount; iterId++)
        {
            for (int duplicateId = 0; duplicateId < duplicateCount; duplicateId++)
            {
                layerCount.push_back(levelCount);
            }
            levelCount *= 2;
            levelCount = (levelCount > maxLevelCount ? maxLevelCount : levelCount);
            if (iterId % increaseNum == 0)
            {
                duplicateCount *= 2;
                duplicateCount = (duplicateCount > maxDuplicateCount ? maxDuplicateCount : duplicateCount);
            }
        }*/
        
        return mpRealTimeDetector->Learn(faceImgNames, nonFaceImgNames);
    }

    int FaceDetection::GetImageNames(const std::string& imgfile, std::vector<std::string>& imgNames) const
    {
        std::string imgPath = imgfile;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
            if (pos == std::string::npos)
            {
                return MAGIC_INVALID_INPUT;
            }
        }
        imgPath.erase(pos);
        imgPath += "/";
        std::ifstream fin(imgfile);
        int dataSize;
        fin >> dataSize;
        imgNames.clear();
        imgNames.resize(dataSize);
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            if (!imgName.empty() && imgName[imgName.size() - 1] == '\r')
            {
                imgName.erase(imgName.size() - 1);
            }
            imgNames.at(dataId) = imgPath + imgName;
        }
        fin.close();
        return MAGIC_NO_ERROR;
    }

    void FaceDetection::TuneGrayValues(const std::string& fileName)
    {
        std::string imgPath = fileName;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        imgPath += "/";
        std::ifstream fin(fileName);
        int dataSize;
        fin >> dataSize;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        double avgGray = 105;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgName = imgPath + imgName;
            std::string tuneImgName = imgName;
            //std::string::size_type pos = tuneImgName.rfind(".");
            //tuneImgName.replace(pos, 4, "_tune.jpg");
            cv::Mat grayImg = cv::imread(imgName);
            int imgH = grayImg.rows;
            int imgW = grayImg.cols;
            double avgImgGray = 0;
            for (int hid = 0; hid < imgH; hid++)
            {
                for (int wid = 0; wid < imgW; wid++)
                {
                    avgImgGray += grayImg.ptr(hid, wid)[0];
                }
            }
            avgImgGray /= (imgH * imgW);
            double grayScale = avgGray / avgImgGray;
            cv::Size cvSize(imgW, imgH);
            cv::Mat tuneImg(cvSize, CV_8UC1);
            for (int hid = 0; hid < imgH; hid++)
            {
                for (int wid = 0; wid < imgW; wid++)
                {
                    double newGray = grayImg.ptr(hid, wid)[0] * grayScale;
                    tuneImg.ptr(hid, wid)[0] = newGray > 255 ? 255 : newGray;
                }
            }
            grayImg.release();
            cv::imwrite(tuneImgName, tuneImg);
            tuneImg.release();
        }
        fin.close();
    }

    void FaceDetection::GenerateTrainingFacesFromLandFile(const std::string& fileName, const std::string& outputPath) 
    {
        DebugLog << "GenerateTrainingFacesFromLandFile..." << std::endl;
        std::string imgPath = fileName;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        std::ifstream fin(fileName);
        int dataSize;
        fin >> dataSize;
        std::vector<std::string> imgFileNames(dataSize);
        std::vector<double> markList;
        int markCount = 37;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            fin.getline(pLine, maxSize);
            std::string landName(pLine);
            std::string imgName = imgPath + landName;
            //std::string posName = imgPath + landName;
            std::string::size_type pos = imgName.rfind(".");
            imgName.replace(pos, 5, ".jpg");
            imgFileNames.at(dataId) = imgName;
            //pos = posName.rfind(".");
            //posName.replace(pos, 5, ".pos");
            //posFileNames.at(dataId) = posName;

            cv::Mat grayImg = cv::imread(imgName);
            int imgH = grayImg.rows;
            int imgW = grayImg.cols;

            landName = imgPath + landName;
            std::ifstream landFin(landName);
            int nTemp;
            landFin >> nTemp;
            for (int markId = 0; markId < markCount * 2; markId++)
            {
                double row, col;
                landFin >> col >> row;
                row = imgH - row;
                if (markId % 2 == 0)
                {
                    markList.push_back(col);
                    markList.push_back(row);
                }
            }
            landFin.close();
        }
        fin.close();

        DebugLog << "Calculate mean marks" << std::endl;
        //Calculate mean marks
        int iterCount = 2;
        std::vector<cv::Point2f> cvMeanMarks(markCount);
        std::vector<cv::Point2f> cvSumMarks(markCount);
        std::vector<cv::Point2f> cvCurMarks(markCount);
        for (int iterIndex = 0; iterIndex < iterCount; iterIndex++)
        {
            if (iterIndex == 0)
            {
                for (int markId = 0; markId < markCount; markId++)
                {
                    cvMeanMarks.at(markId).x = markList.at(markId * 2);
                    cvMeanMarks.at(markId).y = markList.at(markId * 2 + 1);
                }
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                cvSumMarks.at(markId).x = 0;
                cvSumMarks.at(markId).y = 0;
            }
            int validSumNum = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                int markBaseIndex = dataId * markCount * 2;
                for (int markId = 0; markId < markCount; markId++)
                {
                    cvCurMarks.at(markId).x = markList.at(markBaseIndex + markId * 2);
                    cvCurMarks.at(markId).y = markList.at(markBaseIndex + markId * 2 + 1);
                }
                cv::Mat transMat = cv::estimateRigidTransform(cvCurMarks, cvMeanMarks, false);
                if (transMat.data == NULL)
                {
                    DebugLog << "    transMat.data == NULL: " << dataId << " " << iterIndex << std::endl;
                    continue;
                }
                MagicMath::HomoMatrix3 homoMat;
                homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
                homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
                homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
                homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
                homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
                homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
                for (int markId = 0; markId < markCount; markId++)
                {
                    double xRes, yRes;
                    homoMat.TransformPoint(cvCurMarks.at(markId).x, cvCurMarks.at(markId).y, xRes, yRes);
                    cvSumMarks.at(markId).x += xRes;
                    cvSumMarks.at(markId).y += yRes;
                }
                validSumNum++;
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                cvMeanMarks.at(markId).x = cvSumMarks.at(markId).x / validSumNum;
                cvMeanMarks.at(markId).y = cvSumMarks.at(markId).y / validSumNum;
            }
        }

        DebugLog << "Calculate mean face rectangle position" << std::endl; 
        //Calculate mean face rectangle position
        int left = 100000;
        int right = -100000; 
        int top = 100000;
        int down = -100000;
        for (int markId = 0; markId < markCount; markId++)
        {
            if (cvMeanMarks.at(markId).x > right)
            {
                right = cvMeanMarks.at(markId).x;
            }
            if (cvMeanMarks.at(markId).x < left)
            {
                left = cvMeanMarks.at(markId).x;
            }
            if (cvMeanMarks.at(markId).y > down)
            {
                down = cvMeanMarks.at(markId).y;
            }
            if (cvMeanMarks.at(markId).y < top)
            {
                top = cvMeanMarks.at(markId).y;
            }
        }
        int xLen = right - left;
        int yLen = down - top;
        int len = xLen > yLen ? xLen : yLen;
        len += 12; //margin size
        int cenX = (left + right) / 2;
        int cenY = (top + down) / 2;
        left = cenX - len / 2;
        top = cenY - len / 2;

        DebugLog << "Align each face to mean face and extract it" << std::endl;
        //Align each face to mean face and extract it
        int outputSize = 128;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int markBaseIndex = dataId * markCount * 2;
            for (int markId = 0; markId < markCount; markId++)
            {
                cvCurMarks.at(markId).x = markList.at(markBaseIndex + markId * 2);
                cvCurMarks.at(markId).y = markList.at(markBaseIndex + markId * 2 + 1);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurMarks, cvMeanMarks, false);
            if (transMat.data == NULL)
            {
                DebugLog << "miss face: " << dataId << " : " << imgFileNames.at(dataId) << std::endl;
                continue;
            }
            cv::Mat img = cv::imread(imgFileNames.at(dataId));
            cv::Mat uniformImg(img.rows, img.cols, img.type());
            cv::warpAffine(img, uniformImg, transMat, uniformImg.size());
            img.release();

            cv::Mat cropImg(len, len, CV_8UC1);
            for (int hid = 0; hid < len; hid++)
            {
                for (int wid = 0; wid < len; wid++)
                {
                    cropImg.ptr(hid, wid)[0] = uniformImg.ptr(top + hid, left + wid)[0];
                }
            }

            cv::Size cvOutputSize(outputSize, outputSize);
            cv::Mat outputImg(cvOutputSize, CV_8UC1);
            cv::resize(cropImg, outputImg, cvOutputSize);
            cropImg.release();

            std::stringstream ss;
            ss << outputPath << dataId << ".jpg";
            std::string outputName;
            ss >> outputName;
            ss.clear();

            cv::imwrite(outputName, outputImg);
            //cv::imwrite(outputName, uniformImg);
            outputImg.release();
            uniformImg.release();
        }
        DebugLog << "Finished" << std::endl;
    }

    void FaceDetection::GenerateFalsePositivesFromLandFile(const std::string& fileName, const std::string& outputPath) const
    {
        std::string imgPath = fileName;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        std::ifstream fin(fileName);
        int dataSize;
        fin >> dataSize;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        double maxOverlapRate = 0.3;
        //std::string outputPath = "./NonFacefw4/nonFace4_fw_";
        int outputSize = 32;
        int outputId = 0;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            DebugLog << "dataId: " << dataId << " dataSize: " << dataSize << std::endl;
            fin.getline(pLine, maxSize);
            std::string landName(pLine);
            landName = imgPath + landName;
            std::string imgName = landName;
            std::string posName = landName;
            std::string::size_type pos = imgName.rfind(".");
            imgName.replace(pos, 5, ".jpg");
            pos = posName.rfind(".");
            posName.replace(pos, 5, ".pos");
            
            std::ifstream posFin(posName);
            int faceRow, faceCol, faceLen;
            posFin >> faceRow >> faceCol >> faceLen;
            posFin.close();

            /*cv::Mat img = cv::imread(imgName);
            cv::Size cvHalfSize(img.cols / 2, img.rows / 2);
            cv::Mat halfImg(cvHalfSize, CV_8UC1);
            cv::resize(img, halfImg, cvHalfSize);*/

            cv::Mat halfImg = cv::imread(imgName);

            std::vector<int> faces;
            int detectNum = DetectFace(halfImg, faces);
            for (int detectId = 0; detectId < detectNum; detectId++) //modify_flag
            {
                int detectBase = detectId * 4;
                int detectRow = faces.at(detectBase);
                int detectCol = faces.at(detectBase + 1);
                int detectLen = faces.at(detectBase + 2);

                if (CalculateOverlapRate(faceRow, faceCol, faceLen, detectRow, detectCol, detectLen) < maxOverlapRate)
                {
                    cv::Mat detectImg(detectLen, detectLen, CV_8UC1);
                    for (int hid = 0; hid < detectLen; hid++)
                    {
                        for (int wid = 0; wid < detectLen; wid++)
                        {
                            detectImg.ptr(hid, wid)[0] = halfImg.ptr(detectRow + hid, detectCol + wid)[0];
                        }
                    }

                    //std::stringstream ss;
                    //ss << outputPath << outputId << ".jpg";
                    //std::string outputImgName;
                    //ss >> outputImgName;
                    //outputId++;
                    //cv::imwrite(outputImgName, detectImg);
                    ////detectImg.release();

                    //std::vector<int> subFaces;
                    ////cv::Mat newDetectImg = cv::imread(outputImgName);
                    //cv::Mat newDetectImg = detectImg.clone();
                    //detectImg.release();
                    //cv::Size cvOutputSize(outputSize, outputSize);
                    //cv::Mat outputImg(cvOutputSize, CV_8UC1);
                    //cv::resize(newDetectImg, outputImg, cvOutputSize);
                    //newDetectImg.release();
                    //int subRes = DetectFace(outputImg, subFaces);
                    //if (subRes == 0)
                    //{
                    //    MagicTool::gLogOut = true;
                    //    DebugLog << "compare: " << std::endl;
                    //    DebugLog << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                    //    subRes = mpRealTimeDetector->DetectSpecialLocation(halfImg, detectRow, detectCol, detectLen, 1.5625, subFaces);
                    //    DebugLog << "subRes: " << subRes << std::endl;
                    //    DebugLog << "--------------------------------------------" << std::endl;
                    //    subRes = DetectFace(outputImg, subFaces);
                    //    DebugLog << "subRes: " << subRes << std::endl;
                    //    MagicTool::gLogOut = false;
                    //}
                    //outputImg.release();

                    cv::Size cvOutputSize(outputSize, outputSize);
                    cv::Mat outputImg(cvOutputSize, CV_8UC1);
                    cv::resize(detectImg, outputImg, cvOutputSize);
                    detectImg.release();
                    std::stringstream ss;
                    ss << outputPath << outputId << ".jpg";
                    std::string outputImgName;
                    ss >> outputImgName;
                    outputId++;
                    cv::imwrite(outputImgName, outputImg);
                    outputImg.release();
                }
            }
            halfImg.release();
        }
        fin.close();
    }

    void FaceDetection::GenerateFalsePositivesFromNonFace(const std::string& fileName, const std::string& outputPath) const
    {
        std::string imgPath = fileName;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        imgPath += "/";
        std::ifstream fin(fileName);
        int dataSize;
        fin >> dataSize;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        //std::string outputPath = "./NonFacefw1_1/nonFace1_fw_1_";
        int outputId = 0;
        int outputSize = 32;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            DebugLog << "dataId: " << dataId << " dataSize: " << dataSize << std::endl;
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgName = imgPath + imgName;
            cv::Mat colorImg = cv::imread(imgName);
            cv::Mat grayImg;
            cv::cvtColor(colorImg, grayImg, CV_BGR2GRAY); 
            colorImg.release();
            std::vector<int> faces;
            int detectNum = DetectFace(grayImg, faces);
            for (int detectId = 0; detectId < detectNum; detectId++)
            {
                int detectBase = detectId * 4;
                int detectRow = faces.at(detectBase);
                int detectCol = faces.at(detectBase + 1);
                int detectLen = faces.at(detectBase + 2);
                cv::Mat detectImg(detectLen, detectLen, CV_8UC1);
                for (int hid = 0; hid < detectLen; hid++)
                {
                    for (int wid = 0; wid < detectLen; wid++)
                    {
                        detectImg.ptr(hid, wid)[0] = grayImg.ptr(detectRow + hid, detectCol + wid)[0];
                    }
                }
                cv::Size cvOutputSize(outputSize, outputSize);
                cv::Mat outputImg(cvOutputSize, CV_8UC1);
                cv::resize(detectImg, outputImg, cvOutputSize);
                detectImg.release();
                std::stringstream ss;
                ss << outputPath << outputId << ".jpg";
                std::string outputImgName;
                ss >> outputImgName;
                outputId++;
                cv::imwrite(outputImgName, outputImg);
                outputImg.release();
            }
            grayImg.release();
        }
        fin.close();
    }

    void FaceDetection::TestFaceDetectionInFile(const std::string& fileName, const std::string& outputPath) const
    {
        std::string imgPath = fileName;
        std::string::size_type pos = imgPath.rfind("/");
        if (pos == std::string::npos)
        {
            pos = imgPath.rfind("\\");
        }
        imgPath.erase(pos);
        imgPath += "/";
        std::ifstream fin(fileName);
        int imgCount;
        fin >> imgCount;
        const int maxSize = 512;
        char pLine[maxSize];
        fin.getline(pLine, maxSize);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            fin.getline(pLine, maxSize);
            std::string imgName(pLine);
            imgName = imgPath + imgName;
            cv::Mat imgOrigin = cv::imread(imgName);
            if (imgOrigin.data == NULL)
            {
                continue;
            }
            cv::Mat grayImg;
            cv::cvtColor(imgOrigin, grayImg, CV_BGR2GRAY);
            std::vector<int> faces;
            DebugLog << "Detect face id: " << imgId << std::endl;
            int detectNum = DetectFace(grayImg, faces);
            DebugLog << "    detect number: " << detectNum << std::endl;
            grayImg.release();
            if (detectNum > 0)
            {
                std::vector<double> marks;
                for (int detectId = 0; detectId < detectNum; detectId++)
                {
                    int baseId = detectId * 4;

                    int topRow = faces.at(baseId);
                    int downRow = topRow + faces.at(baseId + 3);
                    int leftCol = faces.at(baseId + 1);
                    int rightCol = leftCol + faces.at(baseId + 2);
                    for (int colId = leftCol; colId <= rightCol; colId++)
                    {
                        marks.push_back(topRow);
                        marks.push_back(colId);
                        marks.push_back(downRow);
                        marks.push_back(colId);
                    }
                    for (int rowId = topRow; rowId <= downRow; rowId++)
                    {
                        marks.push_back(rowId);
                        marks.push_back(leftCol);
                        marks.push_back(rowId);
                        marks.push_back(rightCol);
                    }
                }
                //MarkPointsToImage(imgOrigin, &marks, 0, 255, 255, 1);
                int imgW = imgOrigin.cols;
                int imgH = imgOrigin.rows;
                int markNum = marks.size() / 2;
                int markWidth = 2;
                for (int mid = 0; mid < markNum; mid++)
                {
                    int wPos = floor(marks.at(2 * mid + 1) + 0.5);
                    int hPos = floor(marks.at(2 * mid) + 0.5);
                    int hBottom = hPos - markWidth;
                    hBottom = hBottom > 0 ? hBottom : 0;
                    int hUp = hPos + markWidth;
                    hUp = hUp >= imgH ? imgH - 1 : hUp;
                    int wLeft = wPos - markWidth;
                    wLeft = wLeft > 0 ? wLeft : 0;
                    int wRight = wPos + markWidth;
                    wRight = wRight >= imgW ? imgW - 1 : wRight;
                    for (int hid = hBottom; hid <= hUp; hid++)
                    {
                        for (int wid = wLeft; wid <= wRight; wid++)
                        {
                            unsigned char* pixel = imgOrigin.ptr(hid, wid);
                            pixel[0] = 0;
                            pixel[1] = 255;
                            pixel[2] = 255;
                        }
                    }
                }
            }
            std::stringstream ss;
            ss << outputPath << imgId << ".jpg" << std::endl;
            std::string resName;
            ss >> resName;
            ss.clear();
            cv::imwrite(resName, imgOrigin);
            imgOrigin.release();
        }
        fin.close();
    }
}
