#pragma once
#include <vector>

namespace MagicML
{
    class TreeNode
    {
    public:
        TreeNode();
        ~TreeNode();

        std::vector<double> Predict(const std::vector<bool>& dataX) const;
        void SetFeatureId(int featureId);
        void SetPredictions(const std::vector<double>& predictions);
        void SetLeftNode(TreeNode* pNode);
        void SetRightNode(TreeNode* pNode);

    private:
        int mFeatureId;
        TreeNode* mpLeftNode;
        TreeNode* mpRightNode;
        std::vector<double> mPredictions;
    };

    class RandomTree
    {
    public:
        RandomTree();
        ~RandomTree();

        int Learn(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim, int depth);
        std::vector<double> Predict(const std::vector<bool>& dataX) const;
    
    private:
        void Reset(void);
        TreeNode* ConstructTree(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim,
            std::vector<bool>& validFeatureFlag, int depthLeft);
        int RandomChoseValidId(const std::vector<bool>& validFeatureFlag) const; //return -1 if no valid 

    private:
        TreeNode* mpRootNode;

    };

    class RandomFern
    {
    public:
        RandomFern();
        ~RandomFern();

        int Learn(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim, int fernSize);
        std::vector<double> Predict(const std::vector<bool>& dataX) const;
        std::vector<double> PredictWithValidFeature(const std::vector<bool>& dataX) const;
        const std::vector<int>& GetFeatureIds(void) const;
        void Save(int& predictionDim, std::vector<double>& predictions, std::vector<int>& featureIds, std::vector<int>& featureBases) const;
        void Load(int predictionDim, const std::vector<double> predictions, const std::vector<int>& featureIds, const std::vector<int>& featureBases);

    private:
        void Reset(void);
        int PredictionId(const std::vector<bool>& dataX) const;
        int PredictionId(const std::vector<bool>& dataX, int dataDim, int dataId) const;
        std::vector<int> GenerateRandomFeatureIds(int dataDim, int fernSize) const;

    private:
        int mPredictionDim;
        std::vector<double> mPredictions;
        std::vector<int> mFeatureIds;
        //cache
        std::vector<int> mFeatureBases;
    };

    
}
