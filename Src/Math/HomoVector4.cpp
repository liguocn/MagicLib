#include "HomoVector4.h"
#include <math.h>

namespace MagicMath
{
    HomoVector4::HomoVector4()
    {
    }

    HomoVector4::HomoVector4(double x, double y, double z, double h) :
        mX(x),
        mY(y),
        mZ(z),
        mH(h)
    {
    }

    double HomoVector4::Distance(const HomoVector4 vec4) const
    {
        return (*this - vec4).Length();
    }

    double HomoVector4::DistanceSquare(const HomoVector4 vec4) const
    {
        return (*this - vec4).LengthSquare();
    }

    bool HomoVector4::NormalisePoint()
    {
        if (fabs(mH) > 1.0e-15)
        {
            mX /= mH;
            mY /= mH;
            mZ /= mH;
            mH = 1.0;

            return true;
        }
        else
        {
            return false;
        }
    }

    double HomoVector4::Length() const
    {
        return sqrt(mX * mX + mY * mY + mZ * mZ);
    }

    double HomoVector4::LengthSquare() const
    {
        return (mX * mX + mY * mY + mZ * mZ);
    }

    double HomoVector4::NormaliseVector()
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

    HomoVector4 HomoVector4::CrossProduct(const HomoVector4& vec4) const
    {
        return HomoVector4(mY * vec4[2] - mZ * vec4[1], mZ * vec4[0] - mX * vec4[2], mX * vec4[1] - mY * vec4[0], 0);
    }

    double HomoVector4::operator [] (int index) const
    {
        return *(&mX + index);
    }

    double& HomoVector4::operator [] (int index)
    {
        return *(&mX + index);
    }

    bool HomoVector4::operator == (const HomoVector4& vec4) const
    {
        if (fabs(mH) < 1.0e-15 && fabs(vec4[3]) < 1.0e-15)
        {
            return ( (vec4 - *this).Length() < 1.0e-15 );
        }
        else if (fabs(mH) > 1.0e-15 && fabs(vec4[3]) > 1.0e-15)
        {
            HomoVector4 normPoint(mX / mH, mY / mH, mZ / mH, 1.0);
            HomoVector4 normVec4(vec4[0] / vec4[3], vec4[1] / vec4[3], vec4[2] / vec4[3], 1.0);
            return ( (normPoint - normVec4).Length() < 1.0e-15 );
        }
        else
        {
            return false;
        }
    }

    HomoVector4  HomoVector4::operator + (const HomoVector4& vec4) const
    {
        return HomoVector4(mX + vec4[0], mY + vec4[1], mZ + vec4[2], mH + vec4[3]);
    }

    HomoVector4  HomoVector4::operator - (const HomoVector4& vec4) const
    {
        return HomoVector4(mX - vec4[0], mY - vec4[1], mZ - vec4[2], mH - vec4[3]);
    }

    HomoVector4  HomoVector4::operator * (double s) const
    {
        return HomoVector4(mX * s, mY * s, mZ * s, mH * s);
    }

    double HomoVector4::operator * (const HomoVector4& vec4) const
    {
        return (mX * vec4[0] + mY * vec4[1] + mZ * vec4[2] + mH * vec4[3]);
    }

    HomoVector4  HomoVector4::operator / (double s) const
    {
        return HomoVector4(mX / s, mY / s, mZ / s, mH / s);
    }

    HomoVector4  HomoVector4::operator - (void) const
    {
        return HomoVector4(-mX, -mY, -mZ, -mH);
    }

    HomoVector4& HomoVector4::operator += (const HomoVector4& vec4)
    {
        mX += vec4[0];
        mY += vec4[1];
        mZ += vec4[2];
        mH += vec4[3];
        return *this;
    }

    HomoVector4& HomoVector4::operator -= (const HomoVector4& vec4)
    {
        mX -= vec4[0];
        mY -= vec4[1];
        mZ -= vec4[2];
        mH -= vec4[3];
        return *this;
    }

    HomoVector4& HomoVector4::operator *= (double s)
    {
        mX *= s;
        mY *= s;
        mZ *= s;
        mH *= s;
        return *this;
    }

    HomoVector4& HomoVector4::operator /= (double s)
    {
        mX /= s;
        mY /= s;
        mZ /= s;
        mH /= s;
        return *this;
    }

    HomoVector4::~HomoVector4()
    {
    }


}