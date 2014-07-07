#pragma once
#include <vector>
#include <string>

namespace MagicApp
{
    class FaceFeaturePoints
    {
    public:
        FaceFeaturePoints();
        FaceFeaturePoints(int dim);
        virtual ~FaceFeaturePoints();

        bool Load(const std::string& fileName);
        bool LoadLandFile(const std::string& fileName, int imgH);
        void Set(int dim, const std::vector<double>& fpsList, const FaceFeaturePoints* pRefFfp);
        void Save(const std::string& fileName) const;
        void GetDefaultDps(std::vector<double>* dpsList) const;
        void GetDps(const std::vector<int>& addSizeList, std::vector<double>* dpsList) const; // h, w
        void GetFps(std::vector<double>* fpsList) const;
        void GetParameter(std::vector<int>* startIndex, std::vector<bool>* isClose) const;
        virtual void GetFPsNormal(std::vector<double>* norList) const = 0;
        virtual void GetSalientFeatures(std::vector<double>* salientFps) const = 0;

    protected:
        int mSelectIndex;
        int mDim;
        std::vector<double> mFps;
        std::vector<bool> mIsClose;
        std::vector<int> mStartIndex; //the last member is the total size
    };

    class Face2DFeaturePoints : public FaceFeaturePoints
    {
    public:
        Face2DFeaturePoints();
        ~Face2DFeaturePoints();

        virtual void GetFPsNormal(std::vector<double>* norList) const;
        virtual void GetSalientFeatures(std::vector<double>* salientFps) const;

        bool Select(double hPos, double wPos);
        void MoveTo(double hPos, double wPos);
        void MoveDelta(double hDelta, double wDelta);
    };
}
