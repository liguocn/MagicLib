#include "DecisionTree.h"
#include "../Tool/ErrorCodes.h"

namespace MagicML
{
    DecisionNode::DecisionNode() :
        mpLeftNode(NULL),
        mpRightNode(NULL),
        mClassId(-1),
        mFeatureId(-1)
    {
        
    }
    
    DecisionNode::~DecisionNode()
    {
        if (mpLeftNode != NULL)
        {
            delete mpLeftNode;
            mpLeftNode = NULL;
        }
        if (mpRightNode != NULL)
        {
            delete mpRightNode;
            mpRightNode = NULL;
        }
    }
    
    int DecisionNode::Classify(const std::vector<bool>& dataX) const
    {
        if (dataX.at(mFeatureId))
        {
            if (mpLeftNode != NULL)
            {
                return mpLeftNode->Classify(dataX);
            }
            else
            {
                return mClassId;
            }
        }
        else
        {
            if (mpRightNode != NULL)
            {
                return mpRightNode->Classify(dataX);
            }
            else
            {
                return mClassId;
            }
        }
    }

    
    DecisionTree::DecisionTree() :
        mpRootNode(NULL)
    {
        
    }
    
    DecisionTree::~DecisionTree()
    {
        if (mpRootNode != NULL)
        {
            delete mpRootNode;
            mpRootNode = NULL;
        }
    }
    
    int DecisionTree::Learn(const std::vector<bool>& dataX, const std::vector<int>& dataY)
    {
        return MAGIC_NO_ERROR;
    }
    
    int DecisionTree::Predict(const std::vector<bool>& dataX) const
    {
        return 0;
    }
}