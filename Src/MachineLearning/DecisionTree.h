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
        DecisionNode* mpRootNode;
    };
}
