#include "HomoMatrix3.h"
#include <math.h>
#include "../Tool/LogSystem.h"

namespace MagicMath
{
    HomoMatrix3::HomoMatrix3()
    {
        Unit();
    }

    HomoMatrix3::~HomoMatrix3()
    {
    }

    void HomoMatrix3::Print() const
    {
        DebugLog << "Matrix: " << std::endl; 
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                DebugLog << mValues[3 * i + j] << " ";
            }
            DebugLog << std::endl;
        }
    }

    void HomoMatrix3::TransformPoint(double x, double y, double& resX, double& resY) const
    {
        resX = mValues[0] * x + mValues[1] * y + mValues[2];
        resY = mValues[3] * x + mValues[4] * y + mValues[5];
    }

    void HomoMatrix3::GenerateTranslation(double deltaX, double deltaY)
    {
        mValues[0] = 1;
        mValues[1] = 0;
        mValues[2] = deltaX;
        mValues[3] = 0;
        mValues[4] = 1;
        mValues[5] = deltaY;
        mValues[6] = 0;
        mValues[7] = 0;
        mValues[8] = 1;
    }

    void HomoMatrix3::GenerateRotation(double theta)
    {
        mValues[0] = cos(theta);
        mValues[1] = sin(theta);
        mValues[2] = 0;
        mValues[3] = -mValues[1];
        mValues[4] = mValues[0];
        mValues[5] = 0;
        mValues[6] = 0;
        mValues[7] = 0;
        mValues[8] = 1;
    }

    void HomoMatrix3::GenerateScaling(double scaleX, double scaleY)
    {
        mValues[0] = scaleX;
        mValues[1] = 0;
        mValues[2] = 0;
        mValues[3] = 0;
        mValues[4] = scaleY;
        mValues[5] = 0;
        mValues[6] = 0;
        mValues[7] = 0;
        mValues[8] = 1;
    }

    void HomoMatrix3::Unit(void)
    {
        for (int i = 0; i < 9; i++)
        {
            mValues[i] = 0;
        }
        mValues[0] = 1;
        mValues[4] = 1;
        mValues[8] = 1;
    }

    HomoMatrix3 HomoMatrix3::ReverseRigidTransform(void)
    {
        HomoMatrix3 revMat;
        double s = sqrt(mValues[0] * mValues[0] + mValues[1] * mValues[1]);
        double newS = 1 / s;
        double r00 = mValues[0] / s;
        double r01 = mValues[1] / s;
        double r10 = mValues[3] / s;
        double r11 = mValues[4] / s;
        
        revMat.SetValue(0, 0, r00 * newS);
        revMat.SetValue(0, 1, r10 * newS);
        revMat.SetValue(1, 0, r01 * newS);
        revMat.SetValue(1, 1, r11 * newS);
        revMat.SetValue(0, 2, -newS * (r00 * mValues[2] + r10 * mValues[5]));
        revMat.SetValue(1, 2, -newS * (r01 * mValues[2] + r11 * mValues[5]));

        return revMat;
    }

    HomoMatrix3 HomoMatrix3::operator * (const HomoMatrix3& mat) const
    {
        HomoMatrix3 matRes;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                double vTemp = 0;
                for (int k = 0; k < 3; k++)
                {
                    vTemp += GetValue(i, k) * mat.GetValue(k, j);
                }
                matRes.SetValue(i, j, vTemp);
            }
        }

        return matRes;
    }

    HomoMatrix3 HomoMatrix3::operator *=(const HomoMatrix3& mat)
    {
        HomoMatrix3 matCopy = *this;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                double vTemp = 0;
                for (int k = 0; k < 3; k++)
                {
                    vTemp += matCopy.GetValue(i, k) * mat.GetValue(k, j);
                }
                SetValue(i, j, vTemp);
            }
        }
        return *this;
    }

    double HomoMatrix3::GetValue(int row, int col) const
    {
        return mValues[row * 3 + col];
    }

    void HomoMatrix3::SetValue(int row, int col, double v)
    {
        mValues[row * 3 + col] = v;
    }

}
