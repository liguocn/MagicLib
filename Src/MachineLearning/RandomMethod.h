#pragma once
#include <vector>

namespace MagicML
{
    class TreeNode
    {
    public:
        TreeNode();
        ~TreeNode();

        double Predict(const std::vector<bool>& dataX) const;
        void SetFeatureId(int featureId);
        void SetPrediction(double prediction);
        void SetLeftNode(TreeNode* pNode);
        void SetRightNode(TreeNode* pNode);

    private:
        int mFeatureId;
        TreeNode* mpLeftNode;
        TreeNode* mpRightNode;
        double mPrediction;
    };

    class RandomTree
    {
    public:
        RandomTree();
        ~RandomTree();

        int Learn(const std::vector<bool>& dataX, const std::vector<double>& dataY, int depth);
        double Predict(const std::vector<bool>& dataX) const;
    
    private:
        void Reset(void);
        TreeNode* ConstructTree(const std::vector<bool>& dataX, const std::vector<double>& dataY, 
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

        int Learn(const std::vector<bool>& dataX, const std::vector<double>& dataY, int fernSize);
        double Predict(const std::vector<bool>& dataX) const;

    private:
        void Reset(void);
        int PredictionId(const std::vector<bool>& dataX) const;
        int PredictionId(const std::vector<bool>& dataX, int dataDim, int dataId) const;
        std::vector<int> GenerateRandomFeatureIds(int dataDim, int fernSize) const;

    private:
        std::vector<double> mPreditions;
        std::vector<int> mFeatureIds;
        //cache
        std::vector<int> mFeatureBases;
    };

    
}
