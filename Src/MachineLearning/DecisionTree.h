#pragma once
#include <vector>

namespace MagicML
{
    class DecisionNode
    {
    public:
        DecisionNode();
        ~DecisionNode();
        
        int Classify(const std::vector<bool>& dataX) const;
        void SetFeatureId(int featureId);
        void SetClassId(int classId);
        void SetLeftNode(DecisionNode* pNode);
        void SetRightNode(DecisionNode* pNode);
        
    private:
        DecisionNode* mpLeftNode;
        DecisionNode* mpRightNode;
        int mClassId;
        int mFeatureId;
    };
    
    class DecisionTree
    {
    public:
        DecisionTree();
        ~DecisionTree();
        
        int Learn(const std::vector<bool>& dataX, const std::vector<int>& dataY);
        int Predict(const std::vector<bool>& dataX) const;

    private:
        void Reset(void);
        DecisionNode* ConstructTree(const std::vector<bool>& dataX, const std::vector<int>& dataY, int catCount, 
            const std::vector<bool>& featureValidFlag);
        int DominantClassId(const std::vector<int>& dataY, int catCount) const;
        double GiniIndex(const std::vector<int>& typeCount) const;
        
    private:
        DecisionNode* mpRootNode;
    };
}
