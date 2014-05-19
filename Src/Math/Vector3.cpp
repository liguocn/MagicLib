//#include "StdAfx.h"
#include "Vector3.h"
#include <math.h>

namespace MagicMath
{
    Vector3::Vector3() : mX(0), mY(0), mZ(0)
    {
    }

    Vector3::Vector3(double x, double y, double z) : mX(x), mY(y), mZ(z)
    {
    }

    double Vector3::Length() const
    {
        return sqrt(mX * mX + mY * mY + mZ * mZ);
    }

    double Vector3::LengthSquared() const
    {
        return mX * mX + mY * mY + mZ * mZ;
    }

    double Vector3::Distance(const Vector3& vec3) const
    {
        return (*this - vec3).Length();
    }

    double Vector3::DistanceSquared(const Vector3& vec3) const
    {
        return (*this - vec3).LengthSquared();
    }

    double Vector3::Normalise()
    {
        double length = sqrt(mX * mX + mY * mY + mZ * mZ);
        if (length > 1.0e-15)
        {
            mX /= length;
            mY /= length;
            mZ /= length;
        }
        return length;
    }

    Vector3 Vector3::CrossProduct(const Vector3& vec3) const
    {
        return Vector3(mY * vec3[2] - mZ * vec3[1], mZ * vec3[0] - mX * vec3[2], mX * vec3[1] - mY * vec3[0]);
    }

    double Vector3::operator [] (int index) const
    {
        return *(&mX + index);
    }

    double& Vector3::operator [] (int index)
    {
        return *(&mX + index);
    }

    bool Vector3::operator == (const Vector3& vec3) const
    {
        return ( (vec3 - *this).Length() < 1.0e-15 );
    }

    bool Vector3::operator < (const Vector3& vec3) const
    {
        if ((vec3 - *this).Length() < 1.0e-15)
        {
            return false;
        }
        if (mX < vec3[0])
        {
            return true;
        }
        else if (mX == vec3[0])
        {
            if (mY < vec3[1])
            {
                return true;
            }
            else if (mY == vec3[1])
            {
                if (mZ < vec3[2])
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    Vector3 Vector3::operator + (const Vector3& vec3) const
    {
        return Vector3(mX + vec3[0], mY + vec3[1], mZ + vec3[2]);
    }

    Vector3 Vector3::operator - (const Vector3& vec3) const
    {
        return Vector3(mX - vec3[0], mY - vec3[1], mZ - vec3[2]);
    }

    Vector3 Vector3::operator * (double s) const
    {
        return Vector3(mX * s, mY * s, mZ * s);
    }

    double Vector3::operator * (const Vector3& vec3) const
    {
        return (mX * vec3[0] + mY * vec3[1] + mZ * vec3[2]);
    }

    Vector3 Vector3::operator / (double s) const
    {
        return Vector3(mX / s, mY / s, mZ / s);
    }

    Vector3 Vector3::operator - (void) const
    {
        return Vector3(-mX, -mY, -mZ);
    }

    Vector3& Vector3::operator += (const Vector3& vec3)
    {
        mX += vec3[0];
        mY += vec3[1];
        mZ += vec3[2];
        return *this;
    }

    Vector3& Vector3::operator -= (const Vector3& vec3)
    {
        mX -= vec3[0];
        mY -= vec3[1];
        mZ -= vec3[2];
        return *this;
    }

    Vector3& Vector3::operator *= (double s)
    {
        mX *= s;
        mY *= s;
        mZ *= s;
        return *this;
    }

    Vector3& Vector3::operator /= (double s)
    {
        mX /= s;
        mY /= s;
        mZ /= s;
        return *this;
    }

    Vector3::~Vector3()
    {
    }
}