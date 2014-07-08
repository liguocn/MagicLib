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

    void DecisionNode::SetFeatureId(int featureId)
    {
        mFeatureId = featureId;
    }

    void DecisionNode::SetClassId(int classId)
    {
        mClassId = classId;
    }

    void DecisionNode::SetLeftNode(DecisionNode* pNode)
    {
        if (mpLeftNode != NULL)
        {
            delete mpLeftNode;
        }
        mpLeftNode = pNode;
    }

    void DecisionNode::SetRightNode(DecisionNode* pNode)
    {
        if (mpRightNode != NULL)
        {
            delete mpRightNode;
        }
        mpRightNode = pNode;
    }
    
    int DecisionNode::Classify(const std::vector<bool>& dataX) const
    {
        if (mFeatureId == -1)
        {
            return mClassId;
        }
        if (dataX.at(mFeatureId))
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
        else
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
    }

    
    DecisionTree::DecisionTree() :
        mpRootNode(NULL)
    {
        
    }
    
    DecisionTree::~DecisionTree()
    {
        Reset();
    }
    
    int DecisionTree::Learn(const std::vector<bool>& dataX, const std::vector<int>& dataY)
    {
        if (dataY.size() == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        if (dataX.size() / dataY.size() < 1)
        {
            return MAGIC_INVALID_INPUT;
        }

        Reset();
        int featureDim = dataX.size() / dataY.size();
        std::vector<bool> featureValidFlag(featureDim, 1);
        int catCount = 0;
        for (std::vector<int>::const_iterator itr = dataY.begin(); itr != dataY.end(); ++itr)
        {
            if (*itr > catCount)
            {
                catCount = *itr;
            }
        }
        catCount++;

        mpRootNode = ConstructTree(dataX, dataY, catCount, featureValidFlag);

        if (mpRootNode != NULL)
        {
            return MAGIC_NO_ERROR;
        }
        else
        {
            return MAGIC_INVALID_RESULT;
        }
    }
    
    int DecisionTree::Predict(const std::vector<bool>& dataX) const
    {
        return mpRootNode->Classify(dataX);
    }

    void DecisionTree::Reset(void)
    {
        if (mpRootNode != NULL)
        {
            delete mpRootNode;
            mpRootNode = NULL;
        }
    }

    DecisionNode* DecisionTree::ConstructTree(const std::vector<bool>& dataX, const std::vector<int>& dataY, int catCount, 
            const std::vector<bool>& featureValidFlag)
    {
        //dataY only has one type
        //dataY.size() must > 0
        bool isOneType = true;
        int firstType = dataY.at(0);
        for (std::vector<int>::const_iterator itr = dataY.begin(); itr != dataY.end(); ++itr)
        {
            if (*itr != firstType)
            {
                isOneType = false;
                break;
            }
        }
        if (isOneType)
        {
            DecisionNode* pNode = new DecisionNode;
            pNode->SetClassId(firstType);
            return pNode;
        }

        //featureValidFlag has no valid feature
        bool noValidFeature = true;
        for (std::vector<bool>::const_iterator itr = featureValidFlag.begin(); itr != featureValidFlag.end(); ++itr)
        {
            if (*itr)
            {
                noValidFeature = false;
                break;
            }
        }
        if (noValidFeature)
        {
            DecisionNode* pNode = new DecisionNode;
            int dominantType = DominantClassId(dataY, catCount);
            pNode->SetClassId(dominantType);
            return pNode;
        }

        //chose feature id
        int dataSize = dataY.size();
        int featureDim = dataX.size() / dataY.size();
        std::vector<int> leftTypeCount(catCount, 0);
        std::vector<int> rightTypeCount(catCount, 0);
        double minGiniIndex = 2.0; //Gini index <= 1.0
        int cutFeatureId = -1;
        for (int featureId = 0; featureId < featureValidFlag.size(); featureId++)
        {
            if (!featureValidFlag.at(featureId))
            {
                continue;
            }
            //reset type count
            for (int typeId = 0; typeId < catCount; typeId++)
            {
                leftTypeCount.at(typeId) = 0;
                rightTypeCount.at(typeId) = 0;
            }
            int leftDataCount = 0;
            int rightDataCount = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                if (dataX.at(dataId * featureDim + featureId))
                {
                    rightTypeCount.at(dataY.at(dataId))++;
                    rightDataCount++;
                }
                else
                {
                    leftTypeCount.at(dataY.at(dataId))++;
                    leftDataCount++;
                }
            }
            double giniIndex = (leftDataCount * GiniIndex(leftTypeCount) + rightDataCount * GiniIndex(rightTypeCount)) / double(dataSize);
            if (giniIndex < minGiniIndex)
            {
                minGiniIndex = giniIndex;
                cutFeatureId = featureId;
            }
        }
        std::vector<bool> subGroupFlag(dataSize, 0); //0: left, 1: right
        int leftSubCount = 0;
        int rightSubCount = 0;
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            if (dataX.at(dataId * featureDim + cutFeatureId))
            {
                subGroupFlag.at(dataId) = 1;
                rightSubCount++;
            }
            else
            {
                subGroupFlag.at(dataId) = 0;
                leftSubCount++;
            }
        }
        if (leftSubCount == 0 || rightSubCount == 0)
        {
            DecisionNode* pNode = new DecisionNode;
            int dominantType = DominantClassId(dataY, catCount);
            pNode->SetClassId(dominantType);
            return pNode;
        }
        else
        {
            std::vector<bool> subFeatureValidFlag = featureValidFlag;
            subFeatureValidFlag.at(cutFeatureId) = 0;
            std::vector<bool> leftDataX(leftSubCount * featureDim);
            std::vector<int> leftDataY(leftSubCount);
            std::vector<bool> rightDataX(rightSubCount * featureDim);
            std::vector<int> rightDataY(rightSubCount);
            int leftId = 0;
            int rightId = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                if (subGroupFlag.at(dataId))
                {
                    rightDataY.at(rightId) = dataY.at(dataId);
                    for (int featureId = 0; featureId < featureDim; featureId++)
                    {
                        rightDataX.at(rightId * featureDim + featureId) = dataX.at(dataId * featureDim + featureId);
                    }
                    rightId++;
                }
                else
                {
                    leftDataY.at(leftId) = dataY.at(dataId);
                    for (int featureId = 0; featureId < featureDim; featureId++)
                    {
                        leftDataX.at(leftId * featureDim + featureId) = dataX.at(dataId * featureDim + featureId);
                    }
                    leftId++;
                }
            }
            DecisionNode* pNode = new DecisionNode;
            pNode->SetFeatureId(cutFeatureId);
            pNode->SetLeftNode(ConstructTree(leftDataX, leftDataY, catCount, subFeatureValidFlag));
            pNode->SetRightNode(ConstructTree(rightDataX, rightDataY, catCount, subFeatureValidFlag));
            return pNode;
        }
    }

    int DecisionTree::DominantClassId(const std::vector<int>& dataY, int catCount) const
    {
        std::vector<int> typeCount(catCount, 0);
        for (std::vector<int>::const_iterator itr = dataY.begin(); itr != dataY.end(); ++itr)
        {
            typeCount.at(*itr)++;
        }
        int dominantType = 0;
        int dominantCount = typeCount.at(0);
        for (int typeId = 1; typeId < typeCount.size(); typeId++)
        {
            if (typeCount.at(typeId) > dominantCount)
            {
                dominantType = typeId;
                dominantCount = typeCount.at(typeId);
            }
        }
        return dominantType;
    }

    double DecisionTree::GiniIndex(const std::vector<int>& typeCount) const
    {
        double res = 1.0;
        int catSize = typeCount.size();
        int dataCount = 0;
        for (std::vector<int>::const_iterator itr = typeCount.begin(); itr != typeCount.end(); itr++)
        {
            dataCount += *itr;
        }
        for (std::vector<int>::const_iterator itr = typeCount.begin(); itr != typeCount.end(); itr++)
        {
            double pro = *itr / (double)dataCount;
            res -= (pro * pro);
        }
        return res;
    }
}