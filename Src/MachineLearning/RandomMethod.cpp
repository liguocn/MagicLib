#include "RandomMethod.h"
#include "../Tool/ErrorCodes.h"
#include "../Tool/LogSystem.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <set>

namespace MagicML
{
    TreeNode::TreeNode() : 
        mFeatureId(-1),
        mpLeftNode(NULL),
        mpRightNode(NULL),
        mPrediction(0)
    {
    }

    TreeNode::~TreeNode()
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

    double TreeNode::Predict(const std::vector<bool>& dataX) const
    {
        if (mFeatureId == -1)
        {
            return mPrediction;
        }
        else if (dataX.at(mFeatureId))
        {
            if (mpRightNode != NULL)
            {
                return mpRightNode->Predict(dataX);
            }
            else
            {
                return mPrediction;
            }
        }
        else
        {
            if (mpLeftNode != NULL)
            {
                return mpLeftNode->Predict(dataX);
            }
            else
            {
                return mPrediction;
            }
        }
    }

    void TreeNode::SetFeatureId(int featureId)
    {
        mFeatureId = featureId;
    }

    void TreeNode::SetPrediction(double prediction)
    {
        mPrediction = prediction;
    }
        
    void TreeNode::SetLeftNode(TreeNode* pNode)
    {
        if (mpLeftNode != NULL)
        {
            delete mpLeftNode;
        }
        mpLeftNode = pNode;
    }
        
    void TreeNode::SetRightNode(TreeNode* pNode)
    {
        if (mpRightNode != NULL)
        {
            delete mpRightNode;
        }
        mpRightNode = pNode;
    }

    RandomTree::RandomTree() : 
        mpRootNode(NULL)
    {
    }

    RandomTree::~RandomTree()
    {
        Reset();
    }

    int RandomTree::Learn(const std::vector<bool>& dataX, const std::vector<double>& dataY, int depth)
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
        std::vector<bool> validFeatureFlag(featureDim);
        mpRootNode = ConstructTree(dataX, dataY, validFeatureFlag, depth);
        if (mpRootNode != NULL)
        {
            return MAGIC_NO_ERROR;
        }
        else
        {
            return MAGIC_INVALID_RESULT;
        }
    }
     
    double RandomTree::Predict(const std::vector<bool>& dataX) const
    {
        return mpRootNode->Predict(dataX);
    }

    void RandomTree::Reset(void)
    {
        if (mpRootNode != NULL)
        {
            delete mpRootNode;
            mpRootNode = NULL;
        }
    }

    TreeNode* RandomTree::ConstructTree(const std::vector<bool>& dataX, const std::vector<double>& dataY, 
            std::vector<bool>& validFeatureFlag, int depthLeft)
    {
        if (depthLeft == 0)
        {
            TreeNode* pNode = new TreeNode;
            double prediction = 0.0;
            for (std::vector<double>::const_iterator itr = dataY.begin(); itr != dataY.end(); itr++)
            {
                prediction += *itr;
            }
            prediction /= dataY.size();
            pNode->SetPrediction(prediction);
            return pNode;
        }
        //dataY.size() must > 0
        //random chose a feature id, and then process it.
        //if no valid feature id, make this node as leaf
        int dataSize = dataY.size();
        int featureDim = dataX.size() / dataY.size();
        std::vector<bool> localValidFeatureFlag = validFeatureFlag;
        while (true)
        {
            int validId = RandomChoseValidId(localValidFeatureFlag);
            if (validId == -1)
            {
                TreeNode* pNode = new TreeNode;
                double prediction = 0.0;
                for (std::vector<double>::const_iterator itr = dataY.begin(); itr != dataY.end(); itr++)
                {
                    prediction += *itr;
                }
                prediction /= dataY.size();
                pNode->SetPrediction(prediction);
                return pNode;
            }
            //
            std::vector<bool> subGroupFlag(dataSize, 0); //0: left, 1: right
            int leftSubCount = 0;
            int rightSubCount = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                if (dataX.at(dataId * featureDim + validId))
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
                localValidFeatureFlag.at(validId) = 0;
                continue;
            }
            else
            {
                localValidFeatureFlag.at(validId) = 0;
                std::vector<bool> leftDataX(leftSubCount * featureDim);
                std::vector<double> leftDataY(leftSubCount);
                std::vector<bool> rightDataX(rightSubCount * featureDim);
                std::vector<double> rightDataY(rightSubCount);
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
                TreeNode* pNode = new TreeNode;
                pNode->SetFeatureId(validId);
                pNode->SetLeftNode(ConstructTree(leftDataX, leftDataY, localValidFeatureFlag, depthLeft - 1));
                pNode->SetRightNode(ConstructTree(rightDataX, rightDataY, localValidFeatureFlag, depthLeft - 1));
                return pNode;
            }
        }
        return NULL;
    }

    int RandomTree::RandomChoseValidId(const std::vector<bool>& validFeatureFlag) const
    {
        int validCount = 0;
        for (std::vector<bool>::const_iterator itr = validFeatureFlag.begin(); itr != validFeatureFlag.end(); itr++)
        {
            if (*itr)
            {
                validCount++;
            }
        }
        if (validCount == 0)
        {
            return -1;
        }
        srand(time(NULL));
        int randomId = rand() % validCount;
        for (std::vector<bool>::const_iterator itr = validFeatureFlag.begin(); itr != validFeatureFlag.end(); itr++)
        {
            if (*itr)
            {
                if (randomId > 0)
                {
                    randomId--;
                }
                else
                {
                    return *itr;
                }
            }
        }

        return -1;
    }

    RandomFern::RandomFern() :
        mPreditions(),
        mFeatureIds(),
        mFeatureBases()
    {
    }

    RandomFern::~RandomFern()
    {
    }

    int RandomFern::Learn(const std::vector<bool>& dataX, const std::vector<double>& dataY, int fernSize)
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
        int predSize = pow(2, fernSize);
        int dataDim = dataX.size() / dataY.size();
        int dataSize = dataY.size();
        mFeatureIds = GenerateRandomFeatureIds(dataDim, fernSize);
        mFeatureBases.resize(fernSize);
        mFeatureBases.at(0) = 1;
        for (int fernId = 1; fernId < fernSize; fernId++)
        {
            mFeatureBases.at(fernId) = mFeatureBases.at(fernId - 1) * 2;
        }
        mPreditions = std::vector<double>(predSize, 0.0);
        std::vector<int> predDataNum(predSize, 0);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int predId = PredictionId(dataX, dataDim, dataId);
            mPreditions.at(predId) += dataY.at(dataId);
            predDataNum.at(predId)++;
        }
        for (int predId = 0; predId < predSize; predId++)
        {
            if (predDataNum.at(predId) != 0)
            {
                mPreditions.at(predId) /= predDataNum.at(predId);
            }
            else
            {
                DebugLog << "Empty fern block: " << predId << std::endl;
            }
        }

        return MAGIC_NO_ERROR;
    }
        
    double RandomFern::Predict(const std::vector<bool>& dataX) const
    {
        int predictionId = PredictionId(dataX);
        return mPreditions.at(predictionId);
    }

    void RandomFern::Reset(void)
    {
        mPreditions.clear();
        mFeatureIds.clear();
        mFeatureBases.clear();
    }

    int RandomFern::PredictionId(const std::vector<bool>& dataX) const
    {
        int fernSize = mFeatureIds.size();
        int predId = 0;
        for (int fernId = 0; fernId < fernSize; fernId++)
        {
            predId += dataX.at( mFeatureIds.at(fernId) ) * mFeatureBases.at(fernId);
        }
        return predId;
    }

    int RandomFern::PredictionId(const std::vector<bool>& dataX, int dataDim, int dataId) const
    {
        int fernSize = mFeatureIds.size();
        int predId = 0;
        int baseIndex = dataDim * dataId;
        for (int fernId = 0; fernId < fernSize; fernId++)
        {
            predId += dataX.at( baseIndex + mFeatureIds.at(fernId) ) * mFeatureBases.at(fernId);
        }
        return predId;
    }

    std::vector<int> RandomFern::GenerateRandomFeatureIds(int dataDim, int fernSize) const
    {
        srand(time(NULL));
        std::set<int> randomSet;
        for (int fernId = 0; fernId < fernSize; fernId++)
        {
            int randomId = rand() % dataDim;
            randomSet.insert(randomId);
        }
        while (randomSet.size() < fernSize)
        {
            int randomId = rand() % dataDim;
            randomSet.insert(randomId);
        }
        std::vector<int> featureIds;
        featureIds.reserve(fernSize);
        for (std::set<int>::iterator itr = randomSet.begin(); itr != randomSet.end(); itr++)
        {
            featureIds.push_back(*itr);
        }
        return featureIds;
    }
}
