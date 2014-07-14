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
        mPredictions()
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

    std::vector<double> TreeNode::Predict(const std::vector<bool>& dataX) const
    {
        if (mFeatureId == -1)
        {
            return mPredictions;
        }
        else if (dataX.at(mFeatureId))
        {
            if (mpRightNode != NULL)
            {
                return mpRightNode->Predict(dataX);
            }
            else
            {
                return mPredictions;
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
                return mPredictions;
            }
        }
    }

    void TreeNode::SetFeatureId(int featureId)
    {
        mFeatureId = featureId;
    }

    void TreeNode::SetPredictions(const std::vector<double>& prediction)
    {
        mPredictions = prediction;
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

    int RandomTree::Learn(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim, int depth)
    {
        if (xDim < 1 || yDim < 1)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (dataX.size() / xDim != dataY.size() / yDim)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (dataY.size() / yDim == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }
        Reset();
        int featureDim = dataX.size() / dataY.size();
        std::vector<bool> validFeatureFlag(featureDim);
        mpRootNode = ConstructTree(dataX, xDim, dataY, yDim, validFeatureFlag, depth);
        if (mpRootNode != NULL)
        {
            return MAGIC_NO_ERROR;
        }
        else
        {
            return MAGIC_INVALID_RESULT;
        }
    }
     
    std::vector<double> RandomTree::Predict(const std::vector<bool>& dataX) const
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

    TreeNode* RandomTree::ConstructTree(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim,
            std::vector<bool>& validFeatureFlag, int depthLeft)
    {
        int dataSize = dataY.size() / yDim;
        if (depthLeft == 0)
        {
            std::vector<double> predictions(yDim, 0);
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                int baseIndex = dataId * yDim;
                for (int dimID = 0; dimID < yDim; dimID++)
                {
                    predictions.at(dimID) += dataY.at(baseIndex + dimID);
                }
            }
            for (int dimId = 0; dimId < yDim; dimId++)
            {
                predictions.at(dimId) /= dataSize;
            }
            TreeNode* pNode = new TreeNode;
            pNode->SetPredictions(predictions);
            return pNode;
        }
        //dataY.size() must > 0
        //random chose a feature id, and then process it.
        //if no valid feature id, make this node as leaf
        std::vector<bool> localValidFeatureFlag = validFeatureFlag;
        while (true)
        {
            int validId = RandomChoseValidId(localValidFeatureFlag);
            if (validId == -1)
            {
                std::vector<double> predictions(yDim, 0);
                for (int dataId = 0; dataId < dataSize; dataId++)
                {
                    int baseIndex = dataId * yDim;
                    for (int dimID = 0; dimID < yDim; dimID++)
                    {
                        predictions.at(dimID) += dataY.at(baseIndex + dimID);
                    }
                }
                for (int dimId = 0; dimId < yDim; dimId++)
                {
                    predictions.at(dimId) /= dataSize;
                }
                TreeNode* pNode = new TreeNode;
                pNode->SetPredictions(predictions);
                return pNode;
            }
            //
            std::vector<bool> subGroupFlag(dataSize, 0); //0: left, 1: right
            int leftSubCount = 0;
            int rightSubCount = 0;
            for (int dataId = 0; dataId < dataSize; dataId++)
            {
                if (dataX.at(dataId * xDim + validId))
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
                std::vector<bool> leftDataX(leftSubCount * xDim);
                std::vector<double> leftDataY(leftSubCount * yDim);
                std::vector<bool> rightDataX(rightSubCount * xDim);
                std::vector<double> rightDataY(rightSubCount * yDim);
                int leftId = 0;
                int rightId = 0;
                for (int dataId = 0; dataId < dataSize; dataId++)
                {
                    if (subGroupFlag.at(dataId))
                    {
                        for (int yId = 0; yId < yDim; yId++)
                        {
                            rightDataY.at(rightId * yDim + yId) = dataY.at(dataId * yDim + yId);
                        }
                        for (int featureId = 0; featureId < xDim; featureId++)
                        {
                            rightDataX.at(rightId * xDim + featureId) = dataX.at(dataId * xDim + featureId);
                        }
                        rightId++;
                    }
                    else
                    {
                        for (int yId = 0; yId < yDim; yId++)
                        {
                            leftDataY.at(leftId * yDim + yId) = dataY.at(dataId * yDim + yId);
                        }
                        for (int featureId = 0; featureId < xDim; featureId++)
                        {
                            leftDataX.at(leftId * xDim + featureId) = dataX.at(dataId * xDim + featureId);
                        }
                        leftId++;
                    }
                }
                TreeNode* pNode = new TreeNode;
                pNode->SetFeatureId(validId);
                pNode->SetLeftNode(ConstructTree(leftDataX, xDim, leftDataY, yDim, localValidFeatureFlag, depthLeft - 1));
                pNode->SetRightNode(ConstructTree(rightDataX, xDim, rightDataY, yDim, localValidFeatureFlag, depthLeft - 1));
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
        mPredictionDim(0),
        mPredictions(),
        mFeatureIds(),
        mFeatureBases()
    {
    }

    RandomFern::~RandomFern()
    {
    }

    int RandomFern::Learn(const std::vector<bool>& dataX, int xDim, const std::vector<double>& dataY, int yDim, int fernSize)
    {
        if (xDim < 1 || yDim < 1)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (dataX.size() / xDim != dataY.size() / yDim)
        {
            return MAGIC_INVALID_INPUT;
        }
        if (dataY.size() / yDim == 0)
        {
            return MAGIC_EMPTY_INPUT;
        }

        Reset();
        mPredictionDim = yDim;
        int predSize = pow(2, fernSize);
        int dataSize = dataY.size() / yDim;
        mFeatureIds = GenerateRandomFeatureIds(xDim, fernSize);
        mFeatureBases.resize(fernSize);
        mFeatureBases.at(0) = 1;
        for (int fernId = 1; fernId < fernSize; fernId++)
        {
            mFeatureBases.at(fernId) = mFeatureBases.at(fernId - 1) * 2;
        }
        mPredictions = std::vector<double>(predSize * mPredictionDim, 0.0);
        std::vector<int> predDataNum(predSize, 0);
        for (int dataId = 0; dataId < dataSize; dataId++)
        {
            int predId = PredictionId(dataX, xDim, dataId);
            int predBase = predId * mPredictionDim;
            int dataBase = dataId * mPredictionDim;
            for (int dimId = 0; dimId < mPredictionDim; dimId++)
            {
                mPredictions.at(predBase + dimId) += dataY.at(dataBase + dimId);
            }
            predDataNum.at(predId)++;
        }
        for (int predId = 0; predId < predSize; predId++)
        {
            if (predDataNum.at(predId) != 0)
            {
                int predBase = predId * mPredictionDim;
                for (int dimId = 0; dimId < mPredictionDim; dimId++)
                {
                    mPredictions.at(predBase + dimId) /= predDataNum.at(predId);
                }
            }
            /*else
            {
                DebugLog << "Empty fern block: " << predId << std::endl;
            }*/
        }

        return MAGIC_NO_ERROR;
    }
        
    std::vector<double> RandomFern::Predict(const std::vector<bool>& dataX) const
    {
        std::vector<double> predictions(mPredictionDim);
        int predictionId = PredictionId(dataX);
        int baseIndex = predictionId * mPredictionDim;
        for (int dimId = 0; dimId < mPredictionDim; dimId++)
        {
            predictions.at(dimId) = mPredictions.at(baseIndex + dimId);
        }
        return predictions;
    }

    std::vector<double> RandomFern::PredictWithValidFeature(const std::vector<bool>& dataX) const
    {
        int fernSize = dataX.size();
        int predictionId = 0;
        for (int fernId = 0; fernId < fernSize; fernId++)
        {
            predictionId += dataX.at(fernId) * mFeatureBases.at(fernId);
        }
        std::vector<double> predictions(mPredictionDim);
        int baseIndex = predictionId * mPredictionDim;
        for (int dimId = 0; dimId < mPredictionDim; dimId++)
        {
            predictions.at(dimId) = mPredictions.at(baseIndex + dimId);
        }
        return predictions;
    }

    const std::vector<int>& RandomFern::GetFeatureIds(void) const
    {
        return mFeatureIds;
    }

    void RandomFern::Save(int& predictionDim, std::vector<double>& predictions, std::vector<int>& featureIds, 
        std::vector<int>& featureBases) const
    {
        predictionDim = mPredictionDim;
        predictions = mPredictions;
        featureIds = mFeatureIds;
        featureBases = mFeatureBases;
    }
        
    void RandomFern::Load(int predictionDim, const std::vector<double> predictions, const std::vector<int>& featureIds, 
        const std::vector<int>& featureBases)
    {
        Reset();
        mPredictionDim = predictionDim;
        mPredictions = predictions;
        mFeatureIds = featureIds;
        mFeatureBases = featureBases;
    }

    void RandomFern::Reset(void)
    {
        mPredictionDim = 0;
        mPredictions.clear();
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
