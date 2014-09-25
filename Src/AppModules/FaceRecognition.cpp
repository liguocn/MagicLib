#include "FaceRecognition.h"
#include "../Math/HomoMatrix3.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include "../Tool/CommonTools.h"
#include "../DIP/HighDimensionalFeature.h"
#include "../MachineLearning/Clustering.h"
#include "../MachineLearning/PrincipalComponentAnalysis.h"

namespace MagicApp
{
    FaceRecognition::FaceRecognition() : 
        mpHdFeature(NULL)
    {
    }
    
    FaceRecognition::~FaceRecognition()
    {
        Reset();
    }

    void FaceRecognition::Reset()
    {
        if (mpHdFeature != NULL)
        {
            delete mpHdFeature;
            mpHdFeature = NULL;
        }
    }
    
    int FaceRecognition::AlignFace(const std::string& faceImgFiles, const std::string& alignedFilePath)
    {
        DebugLog << "FaceRecognition::AlignFace..." << std::endl;
        std::string imgPath = MagicTool::CommonTools::GetPath(faceImgFiles);
        imgPath += "/";
        
        //read image and mark data
        std::ifstream fin(faceImgFiles);
        int dataSize;
        fin >> dataSize;
        std::vector<int> faceIdList(dataSize);
        std::vector<std::string> imgFileNames(dataSize);
        std::vector<double> markList;
        int markCount = 0;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int faceId;
            std::string imgName;
            std::string markFileName;
            fin >> faceId >> imgName >> markFileName;

            faceIdList.at(dataId) = faceId;
            imgName = imgPath + imgName;
            imgFileNames.at(dataId) = imgName;         

            markFileName = imgPath + markFileName;
            std::ifstream markFin(markFileName);
            markFin >> markCount;
            if (dataId == 0)
            {
                markList.reserve(markCount * dataSize * 2);
            }
            for (int markId = 0; markId < markCount; markId++)
            {
                double row, col;
                markFin >> col >> row;
                markList.push_back(col);
                markList.push_back(row);
            }
            markFin.close();
        }
        fin.close();
        
        //Calculate mean marks
        DebugLog << "Calculate mean marks" << std::endl;
        int iterCount = 2;
        std::vector<cv::Point2f> cvMeanMarks(markCount);
        std::vector<cv::Point2f> cvSumMarks(markCount);
        std::vector<cv::Point2f> cvCurMarks(markCount);
        for (int iterIndex = 0; iterIndex < iterCount; iterIndex++)
        {
            //DebugLog << "iterIndex: " << iterIndex << std::endl;
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
                //DebugLog << " transMat, dataId: " << dataId << std::endl;
                cv::Mat transMat = cv::estimateRigidTransform(cvCurMarks, cvMeanMarks, false);
                //DebugLog << "     finished" << std::endl;
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
        
        DebugLog << "Align each face to mean face and extract it" << std::endl;
        std::string fileListName = alignedFilePath + "/fileList.txt";
        std::ofstream fileListOut(fileListName);
        //Align each face to mean face and extract it
        int validAlignedId = 0;
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
            fileListOut << faceIdList.at(dataId) << " t_" << validAlignedId << ".jpg" << " t_" << validAlignedId << ".land" << std::endl;

            cv::Mat img = cv::imread(imgFileNames.at(dataId));
            cv::Mat alignedImg(img.rows, img.cols, img.type());
            cv::warpAffine(img, alignedImg, transMat, alignedImg.size());
            img.release();
            std::stringstream ss;
            ss << alignedFilePath << "/t_" << validAlignedId << ".jpg";
            std::string alignedImgName;
            ss >> alignedImgName;
            cv::imwrite(alignedImgName, alignedImg);
            //alignedImg.release();
            
            MagicMath::HomoMatrix3 homoMat;
            homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
            ss.clear();
            ss << alignedFilePath << "/t_" << validAlignedId << ".land";
            std::string alignedMarkName;
            ss >> alignedMarkName;
            ss.clear();
            ss << alignedFilePath << "/t_mark_" << validAlignedId << ".jpg";
            std::string markedAlignedImgName;
            ss >> markedAlignedImgName;
            std::ofstream markFout(alignedMarkName);
            markFout << markCount << std::endl;
            for (int markId = 0; markId < markCount; markId++)
            {
                double xRes, yRes;
                homoMat.TransformPoint(cvCurMarks.at(markId).x, cvCurMarks.at(markId).y, xRes, yRes);
                markFout << xRes << " " << yRes << std::endl;
                alignedImg.ptr(int(yRes), int(xRes))[0] = 255;
                alignedImg.ptr(int(yRes), int(xRes))[1] = 255;
                alignedImg.ptr(int(yRes), int(xRes))[2] = 255;
            }
            markFout.close();
            
            cv::imwrite(markedAlignedImgName, alignedImg);
            alignedImg.release();
            validAlignedId++;
        }
        fileListOut.close();
        DebugLog << "Finished" << std::endl;
        
        return MAGIC_NO_ERROR;
    }

    int FaceRecognition::LearnHighDimensionalFeature(const std::string& fileListName)
    {
        std::string filePath = MagicTool::CommonTools::GetPath(fileListName);
        filePath += "/";

        std::ifstream fileFin(fileListName);
        int dataCount;
        fileFin >> dataCount;
        if (dataCount <= 0)
        {
            return MAGIC_INVALID_INPUT;
        }
        std::vector<std::string> imgFiles(dataCount);
        std::vector<int> faceIds(dataCount);
        std::vector<int> marksList;
        int markCountPerImage;
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int faceId;
            fileFin >> faceId;
            faceIds.at(dataId) = faceId;
            
            std::string faceName;
            fileFin >> faceName;
            faceName = filePath + faceName;
            imgFiles.at(dataId) = faceName;

            std::string markName;
            fileFin >> markName;
            markName = filePath + markName;
            std::ifstream markFin(markName);
            markFin >> markCountPerImage;
            if (dataId == 0)
            {
                marksList.reserve(dataCount * markCountPerImage * 2);
            }
            for (int markId = 0; markId < markCountPerImage; markId++)
            {
                double row, col;
                markFin >> col >> row;
                marksList.push_back(col);
                marksList.push_back(row);
            }
            markFin.close();
        }
        fileFin.close();

        if (mpHdFeature == NULL)
        {
            mpHdFeature = new MagicDIP::HighDimensionalFeature;
        }
        return mpHdFeature->Learn(imgFiles, marksList, markCountPerImage, faceIds);
    }

    int FaceRecognition::GetHighDimensionalFeature(const std::string& fileListName, std::vector<double>& features, int& featureDim, 
            int& clusterCount, std::vector<std::string>& imgFiles)
    {
        std::string filePath = MagicTool::CommonTools::GetPath(fileListName);
        filePath += "/";

        std::ifstream fileFin(fileListName);
        int dataCount;
        fileFin >> dataCount;
        if (dataCount <= 0)
        {
            return MAGIC_INVALID_INPUT;
        }

        features.clear();
        imgFiles.clear();
        imgFiles.resize(dataCount);
        clusterCount = 0;
        int lastClusterId = -1;
        if (mpHdFeature == NULL)
        {
            mpHdFeature = new MagicDIP::HighDimensionalFeature;
        }
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            int faceId;
            fileFin >> faceId;
            if (faceId != lastClusterId)
            {
                clusterCount++;
                lastClusterId = faceId;
            }
            
            std::string faceName;
            fileFin >> faceName;
            faceName = filePath + faceName;
            imgFiles.at(dataId) = faceName;
            cv::Mat img = cv::imread(faceName);

            std::string markName;
            fileFin >> markName;
            markName = filePath + markName;
            std::ifstream markFin(markName);
            int markCount;
            markFin >> markCount;
            std::vector<int> marksList;
            marksList.reserve(markCount * 2);
            for (int markId = 0; markId < markCount; markId++)
            {
                double row, col;
                markFin >> col >> row;
                marksList.push_back(col);
                marksList.push_back(row);
            }
            markFin.close();

            std::vector<double> oneFaceFeature;
            mpHdFeature->GetHighDimensionalFeature(img, marksList, oneFaceFeature);
            if (dataId == 0)
            {
                featureDim = oneFaceFeature.size();
                features.reserve(static_cast<long long>(featureDim) * dataCount);
            }
            for (std::vector<double>::iterator itr = oneFaceFeature.begin(); itr != oneFaceFeature.end(); itr++)
            {
                features.push_back(*itr);
            }

            //DebugLog << "dataId: " << dataId << " clusterCount: " << clusterCount << " featureDim: " << featureDim << std::endl;
        }
        fileFin.close();

        return MAGIC_NO_ERROR;
    }

    int FaceRecognition::KMeansClusteringHighDimensionalFeature(const std::string& fileListName, const std::string& resPath)
    {
        std::vector<double> features;
        int featureDim;
        int clusterCount;
        std::vector<std::string> imgFiles;
        int res = GetHighDimensionalFeature(fileListName, features, featureDim, clusterCount, imgFiles);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }

        DebugLog << "KMeans clustring: featureDim: " << featureDim << " clusterCount: " << clusterCount << std::endl;
        int dataCount = imgFiles.size();
        std::vector<int> clusterRes;
        MagicML::Clustering::KMeans(features, featureDim, clusterCount, clusterRes);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            std::stringstream ss;
            ss << resPath << "/" << clusterRes.at(dataId) << "_" << dataId << ".jpg";
            std::string outputName;
            ss >> outputName;
            cv::Mat img = cv::imread(imgFiles.at(dataId));
            cv::imwrite(outputName, img);
            img.release();
        }

        return MAGIC_NO_ERROR;
    }

    int FaceRecognition::KMeansClusteringPcaCompressedFeature(const std::string& fileListName, const std::string& resPath)
    {
        std::vector<double> features;
        int featureDim;
        int clusterCount;
        std::vector<std::string> imgFiles;
        int res = GetHighDimensionalFeature(fileListName, features, featureDim, clusterCount, imgFiles);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }

        DebugLog << "PCA Compressing: featureDim: " << featureDim << " clusterCount: " << clusterCount << std::endl;
        int dataCount = imgFiles.size();
        int pcaDim;
        MagicML::PrincipalComponentAnalysis pca;
        res = pca.Analyse(features, featureDim, 0.99, pcaDim);
        if (res != MAGIC_NO_ERROR)
        {
            return res;
        }
        std::vector<double> compressedFeatures;
        compressedFeatures.reserve(pcaDim * dataCount);
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
                compressedFeatures.push_back(compressedOneFeature.at(pcaIndex));
            }
        }
        features.clear();

        DebugLog << "KMeans clustring: featureDim: " << pcaDim << " clusterCount: " << clusterCount << std::endl;
        std::vector<int> clusterRes;
        MagicML::Clustering::KMeans(compressedFeatures, pcaDim, clusterCount, clusterRes);
        for (int dataId = 0; dataId < dataCount; dataId++)
        {
            std::stringstream ss;
            ss << resPath << "/" << clusterRes.at(dataId) << "_" << dataId << ".jpg";
            std::string outputName;
            ss >> outputName;
            cv::Mat img = cv::imread(imgFiles.at(dataId));
            cv::imwrite(outputName, img);
            img.release();
        }

        return MAGIC_NO_ERROR;
    }
}
