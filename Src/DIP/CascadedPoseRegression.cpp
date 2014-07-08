#include "CascadedPoseRegression.h"
#include "../Tool/ErrorCodes.h"

namespace MagicDIP
{
    CascadedPoseRegression::CascadedPoseRegression() :
        mRandomFerns()
    {
    }

    CascadedPoseRegression::~CascadedPoseRegression()
    {
        Reset();
    }

    int CascadedPoseRegression::LearnRegression(const std::string& landFileName)
    {
        Reset();
        return MAGIC_NO_ERROR;
    }
        
    void CascadedPoseRegression::PoseRegression(const cv::Mat& img, const std::vector<double>& dataX, std::vector<double>&dataY) const
    {
        //Estimate initial Theta from dataX
 
        //Cascaded Pose Regression

    }

    void CascadedPoseRegression::Reset(void)
    {
        for (std::vector<MagicML::RandomFern* >::iterator itr = mRandomFerns.begin(); itr != mRandomFerns.end(); itr++)
        {
            if (*itr != NULL)
            {
                delete (*itr);
                *itr = NULL;
            }
        }
        mRandomFerns.clear();
    }
}
