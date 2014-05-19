#include "Segmentation.h"
#include "graph.h"
#include "../MachineLearning/Clustering.h"
#include "../MachineLearning/GaussianMixtureModel.h"

namespace MagicDIP
{
    Segmentation::Segmentation()
    {
    }

    Segmentation::~Segmentation()
    {
    }

    cv::Mat Segmentation::SegmentByGraphCut(const cv::Mat& inputImg, const cv::Mat& markImg)
    {
        //Construct GMM
        std::vector<double> forgroundData;
        std::vector<double> backgroundData;
        int imgW = inputImg.cols;
        int imgH = inputImg.rows;
        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                const unsigned char* pPixel = markImg.ptr(hid, wid);
                if (pPixel[0] == 255) //background
                {
                    const unsigned char* fPixel = inputImg.ptr(hid, wid);
                    forgroundData.push_back(fPixel[0]);
                    forgroundData.push_back(fPixel[1]);
                    forgroundData.push_back(fPixel[2]);
                }
                else if (pPixel[2] == 255) //forground
                {
                    const unsigned char* bPixel = inputImg.ptr(hid, wid);
                    backgroundData.push_back(bPixel[0]);
                    backgroundData.push_back(bPixel[1]);
                    backgroundData.push_back(bPixel[2]);
                }
            }
        }
        int dim = 3;
        int k = 5;
        std::vector<int> fClusterRes;
        std::vector<int> bClusterRes;
        MagicML::Clustering::OrchardBoumanClustering(forgroundData, dim, k, fClusterRes);
        MagicML::Clustering::OrchardBoumanClustering(backgroundData, dim, k, bClusterRes);
        MagicML::GaussianMixtureModel fGmm;
        fGmm.CalParameter(forgroundData, dim, k, fClusterRes);
        MagicML::GaussianMixtureModel bGmm;
        bGmm.CalParameter(backgroundData, dim, k, bClusterRes);
        
        //Construct Graph and apply graph cut
        //Calculate some constant value
        double lambda = 10;
        double maxL = 8 * lambda + 1;
        double beta = 0;
        for (int hid = 1; hid < imgH - 1; hid++)
        {
            for (int wid = 1; wid < imgW - 1; wid++)
            {
                beta += PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid - 1));
                beta += PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid));
                beta += PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid + 1));
                beta += PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid, wid + 1));
            }
        }
        beta = 0.5 / beta / (imgH - 1) / (imgW - 1);
        typedef Graph<double, double, double> GraphType;
        GraphType* graph = new GraphType(imgW * imgH, 4 * imgW * imgH);
        //Add Node
        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                graph->add_node();
            }
        }
        //Construct T-Links
        std::vector<double> pixelColor(3);
        int pixelIndex = 0;
        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                double fW, bW;
                const unsigned char* pPixel = markImg.ptr(hid, wid);
                if (pPixel[0] == 255) //background
                {
                    fW = 0;
                    bW = maxL;
                }
                else if (pPixel[2] == 255) //forground
                {
                    fW = maxL;
                    bW = 0;
                }
                else //unknown
                {
                    const unsigned char* pPixel = inputImg.ptr(hid, wid);
                    pixelColor.at(0) = pPixel[0];
                    pixelColor.at(1) = pPixel[1];
                    pixelColor.at(2) = pPixel[2];
                    fW = -log(fGmm.Pro(pixelColor));
                    bW = -log(bGmm.Pro(pixelColor));
                }
                graph->add_tweights(pixelIndex, fW, bW);
                pixelIndex++;
            }
        }
        //Construct N-Links
        for (int hid = 1; hid < imgH - 1; hid++)
        {
            for (int wid = 1; wid < imgW - 1; wid++)
            {
                int centerIndex = hid * imgW + wid;
                double nW = lambda * exp(-beta * PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid - 1))) / 1.414;
                graph->add_edge(centerIndex, (hid - 1) * imgW + wid - 1, nW, nW);
                nW = lambda * exp(-beta * PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid)));
                graph->add_edge(centerIndex, (hid - 1) * imgW + wid, nW, nW);
                nW = lambda * exp(-beta * PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid - 1, wid + 1))) / 1.414;
                graph->add_edge(centerIndex, (hid - 1) * imgW + wid + 1, nW, nW);
                nW = lambda * exp(-beta * PixelDistance2(inputImg.ptr(hid, wid), inputImg.ptr(hid, wid + 1)));
                graph->add_edge(centerIndex, hid * imgW + wid + 1, nW, nW);
            }
        }
        //Graph cut
        graph->maxflow();
        //Copy result
        cv::Size imgSize(imgW, imgH);
        cv::Mat segImg(imgSize, CV_8UC3);
        pixelIndex = 0;
        for (int hid = 0; hid < imgH; hid++)
        {
            for (int wid = 0; wid < imgW; wid++)
            {
                unsigned char* pPixel = segImg.ptr(hid, wid);
                if (graph->what_segment(pixelIndex) == GraphType::SOURCE)
                {
                    pPixel[0] = 0;
                    pPixel[1] = 0;
                    pPixel[2] = 0;
                }
                else
                {
                    pPixel[0] = 255;
                    pPixel[1] = 0;
                    pPixel[2] = 0;
                }
                pixelIndex++;
            }
        }

        //Clear
        delete graph;

        return segImg;
    }

    double Segmentation::PixelDistance2(const unsigned char* pixel1, const unsigned char* pixel2)
    {
        double x0 = pixel1[0] - pixel2[0];
        double x1 = pixel1[1] - pixel2[1];
        double x2 = pixel1[2] - pixel2[2];
        return (x0 * x0 + x1 * x1 + x2 * x2);
    }
}