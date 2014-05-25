#include "Face2DObj.h"
#include "../DIP/Deformation.h"
#include "../Math/HomoMatrix3.h"

namespace MagicApp
{
    Face2DPCA::Face2DPCA() :
        mpFeaturePca(NULL)
    {
    }

    Face2DPCA::~Face2DPCA()
    {
        if (mpFeaturePca != NULL)
        {
            delete mpFeaturePca;
            mpFeaturePca = NULL;
        }
    }

    void Face2DPCA::DoFeaturePca(const std::string& path, const std::vector<int>& imgIndex)
    {
        std::vector<Face2DFeaturePoints*> ffpList;
        std::vector<cv::Point2f> cvMeanFps;
        CalMeanFeature(path, imgIndex, &ffpList, &cvMeanFps);
        int fpsSize = cvMeanFps.size();

        //align to mean features and collect pca data
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int dataDim = fpsSize * 2;
        int imgCount = imgIndex.size();
        std::vector<double> pcaData(dataDim * imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            std::vector<double> curFps;
            ffpList.at(imgId)->GetFps(&curFps);
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, cvMeanFps, false);
            MagicMath::HomoMatrix3 homoMat;
            homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
            int baseIndex = imgId * dataDim;
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                double xRes, yRes;
                homoMat.TransformPoint(cvCurFps.at(fpsId).x, cvCurFps.at(fpsId).y, xRes, yRes);
                pcaData.at(baseIndex + fpsId * 2) = yRes;
                pcaData.at(baseIndex + fpsId * 2 + 1) = xRes;
            }
        }

        //Do Pca
        int pcaDim = imgCount - 1;
        if (mpFeaturePca == NULL)
        {
            mpFeaturePca = new MagicML::PrincipalComponentAnalysis;
        }
        mpFeaturePca->Analyse(pcaData, dataDim, pcaDim);

        //Save Pca
        std::string pcaFileName = path + ".pca";
        mpFeaturePca->Save(pcaFileName);

        //free memory
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            delete ffpList.at(imgId);
            ffpList.at(imgId) = NULL;
        }
    }

    MagicML::PrincipalComponentAnalysis* Face2DPCA::GetFeaturePca(void)
    {
        return NULL;
    }

    void Face2DPCA::DeformFeatureToMeanFace(const std::string& path, std::vector<int>& imgIndex)
    {
        //Calculate mean feature
        std::vector<Face2DFeaturePoints*> ffpList;
        std::vector<cv::Point2f> cvMeanFps;
        CalMeanFeature(path, imgIndex, &ffpList, &cvMeanFps);

        //Calculate meanDps
        int fpsSize = cvMeanFps.size();
        std::vector<double> meanFps(fpsSize * 2);
        for (int fpsId = 0; fpsId < fpsSize; fpsId++)
        {
            meanFps.at(fpsId * 2) = floor(cvMeanFps.at(fpsId).y + 0.5);
            meanFps.at(fpsId * 2 + 1) = floor(cvMeanFps.at(fpsId).x + 0.5);
        }
        Face2DFeaturePoints meanFfp;
        meanFfp.Set(2, meanFps, ffpList.at(0));
        std::vector<double> meanDps;
        meanFfp.GetDefaultDps(&meanDps);
        int dpsSize = meanDps.size() / 2;
        for (int dpsId = 0; dpsId < dpsSize; dpsId++)
        {
            int temp = meanDps.at(dpsId * 2);
            meanDps.at(dpsId * 2) = meanDps.at(dpsId * 2 + 1);
            meanDps.at(dpsId * 2 + 1) = temp;
        }

        //Deform to mean
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int imgCount = imgIndex.size();
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            //Calculate transformation
            std::vector<double> curFps;
            ffpList.at(imgId)->GetFps(&curFps);
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
            }
            cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, cvMeanFps, false);
            MagicMath::HomoMatrix3 homoTransMat;
            homoTransMat.SetValue(0, 0, transMat.at<double>(0, 0));
            homoTransMat.SetValue(0, 1, transMat.at<double>(0, 1));
            homoTransMat.SetValue(0, 2, transMat.at<double>(0, 2));
            homoTransMat.SetValue(1, 0, transMat.at<double>(1, 0));
            homoTransMat.SetValue(1, 1, transMat.at<double>(1, 1));
            homoTransMat.SetValue(1, 2, transMat.at<double>(1, 2));

            //Load image
            std::stringstream ss;
            ss << path << imgIndex.at(imgId) << ".jpg";
            std::string imgName;
            ss >> imgName;
            ss.clear();
            cv::Mat img = cv::imread(imgName);
            cv::Mat uniformImg(img.rows, img.cols, img.type());
            cv::warpAffine(img, uniformImg, transMat, uniformImg.size());

            //Deform
            std::vector<double> curDps;
            ffpList.at(imgId)->GetDefaultDps(&curDps);
            for (int dpsId = 0; dpsId < dpsSize; dpsId++)
            {
                double xRes, yRes;
                homoTransMat.TransformPoint(curDps.at(dpsId * 2 + 1), curDps.at(dpsId * 2), xRes, yRes);
                curDps.at(dpsId * 2) = floor(xRes + 0.5);
                curDps.at(dpsId * 2 + 1) = floor(yRes + 0.5);
            }

            std::vector<int> curDpsInt(curDps.size());
            std::vector<int> meanDpsInt(meanDps.size());
            for (int dpsId = 0; dpsId < curDps.size(); dpsId++)
            {
                curDpsInt.at(dpsId) = floor(curDps.at(dpsId) + 0.5);
                meanDpsInt.at(dpsId) = floor(meanDps.at(dpsId) + 0.5);
            }
            cv::Mat deformImg = MagicDIP::Deformation::DeformByMovingLeastSquares(uniformImg, curDpsInt, meanDpsInt);

            //Write image
            ss << path << "ToMean" << imgIndex.at(imgId) << ".jpg";
            std::string transImgName;
            ss >> transImgName;
            ss.clear();
            cv::imwrite(transImgName, deformImg);
        }
    }

    void Face2DPCA::CalMeanFeature(const std::string& path, const std::vector<int>& imgIndex, 
            std::vector<Face2DFeaturePoints*>* ffpList, std::vector<cv::Point2f>* cvMeanFps)
    {
        //Load feature data
        int imgCount = imgIndex.size();
        ffpList->clear();
        ffpList->resize(imgCount);
        for (int imgId = 0; imgId < imgCount; imgId++)
        {
            std::stringstream ss;
            ss << path << imgIndex.at(imgId) << ".fp";
            std::string fileName;
            ss >> fileName;
            ffpList->at(imgId) = new Face2DFeaturePoints;
            ffpList->at(imgId)->Load(fileName);
        }

        //Calculate mean face
        std::vector<double> firstFps;
        ffpList->at(0)->GetFps(&firstFps);
        int fpsSize = firstFps.size() / 2;
        cvMeanFps->clear();
        cvMeanFps->resize(fpsSize);
        std::vector<cv::Point2f> cvSumFps(fpsSize);
        std::vector<cv::Point2f> cvCurFps(fpsSize);
        int iterCount = 3;
        for (int iterIndex = 0; iterIndex < iterCount; iterIndex++)
        {
            if (iterIndex == 0)
            {
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    cvMeanFps->at(fpsId).x = firstFps.at(fpsId * 2 + 1);
                    cvMeanFps->at(fpsId).y = firstFps.at(fpsId * 2);
                }
            }
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvSumFps.at(fpsId).x = 0;
                cvSumFps.at(fpsId).y = 0;
            }
            for (int imgId = 0; imgId < imgCount; imgId++)
            {
                std::vector<double> curFps;
                ffpList->at(imgId)->GetFps(&curFps);
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    cvCurFps.at(fpsId).x = curFps.at(fpsId * 2 + 1);
                    cvCurFps.at(fpsId).y = curFps.at(fpsId * 2);
                }
                cv::Mat transMat = cv::estimateRigidTransform(cvCurFps, *cvMeanFps, false);
                MagicMath::HomoMatrix3 homoMat;
                homoMat.SetValue(0, 0, transMat.at<double>(0, 0));
                homoMat.SetValue(0, 1, transMat.at<double>(0, 1));
                homoMat.SetValue(0, 2, transMat.at<double>(0, 2));
                homoMat.SetValue(1, 0, transMat.at<double>(1, 0));
                homoMat.SetValue(1, 1, transMat.at<double>(1, 1));
                homoMat.SetValue(1, 2, transMat.at<double>(1, 2));
                for (int fpsId = 0; fpsId < fpsSize; fpsId++)
                {
                    double xRes, yRes;
                    homoMat.TransformPoint(cvCurFps.at(fpsId).x, cvCurFps.at(fpsId).y, xRes, yRes);
                    cvSumFps.at(fpsId).x += xRes;
                    cvSumFps.at(fpsId).y += yRes;
                }
            }
            for (int fpsId = 0; fpsId < fpsSize; fpsId++)
            {
                cvMeanFps->at(fpsId).x = cvSumFps.at(fpsId).x / imgCount;
                cvMeanFps->at(fpsId).y = cvSumFps.at(fpsId).y / imgCount;
            }
        }
    }

    void Face2DPCA::CalMeanFace(const std::string& path, std::vector<int>& imgIndex)
    {

    }

    Face2DObj::Face2DObj() :
        mpImage(NULL),
        mpFfp(NULL)
    {
    }

    Face2DObj::~Face2DObj()
    {
        if (mpImage != NULL)
        {
            mpImage->release();
            delete mpImage;
            mpImage = NULL;
        }
    }

    bool Face2DObj::LoadFaceImage(const std::string& fileName)
    {
        if (mpImage == NULL)
        {
            mpImage = new cv::Mat;
        }
        mpImage->release();
        *mpImage = cv::imread(fileName);
        if (mpImage->data != NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    cv::Mat Face2DObj::GetFaceImage(void)
    {
        return *mpImage;
    }

    bool Face2DObj::LoadFfp(const std::string& fileName)
    {
        if (mpFfp == NULL)
        {
            mpFfp = new Face2DFeaturePoints;
        }
        return mpFfp->Load(fileName);
    }

    Face2DFeaturePoints* Face2DObj::GetFfp(void)
    {
        return mpFfp;
    }

    void Face2DObj::SaveFfp(const std::string& fileName)
    {
        mpFfp->Save(fileName);
    }

    void Face2DObj::AutoAlignFfp(const std::vector<double>& markPos, const cv::Mat& featureImg,
        const MagicML::PrincipalComponentAnalysis& featurePca)
    {

    }

    void Face2DObj::DeformToFeaturePcaSpace(const MagicML::PrincipalComponentAnalysis& featurePca)
    {

    }

    void Face2DObj::DeformToFeature(const Face2DFeaturePoints& refFfp)
    {

    }

    void Face2DObj::ResizeImage(int width, int height, bool keepRatio)
    {
        if (keepRatio)
        {
            int imgW = mpImage->cols;
            int imgH = mpImage->rows;
            bool resized = false;
            if (imgW > width)
            {
                imgH = int(imgH * float(width) / imgW);
                imgW = width;
                resized = true;
            }
            if (imgH > height)
            {
                imgW = int(imgW * float(height) / imgH);
                imgH = height;
                resized = true;
            }
            if (resized)
            {
                cv::Size vcSize(imgW, imgH);
                cv::Mat resizedImg(vcSize, CV_8UC3);
                cv::resize(*mpImage, resizedImg, vcSize);
                mpImage->release();
                *mpImage = resizedImg.clone();
            }
        }
        else
        {
            cv::Size vcSize(width, height);
            cv::Mat resizedImg(vcSize, CV_8UC3);
            cv::resize(*mpImage, resizedImg, vcSize);
            mpImage->release();
            *mpImage = resizedImg.clone();
        }
    }
}
