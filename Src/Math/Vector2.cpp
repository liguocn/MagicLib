#include "Vector2.h"
#include <math.h>

namespace MagicMath
{
    Vector2::Vector2() : 
        mX(0), 
        mY(0)
    {
    }

    Vector2::Vector2(double x, double y) : 
        mX(x), 
        mY(y)
    {
    }

    double Vector2::Length() const
    {
        return sqrt(mX * mX + mY * mY);
    }

    double Vector2::LengthSquared() const
    {
        return mX * mX + mY * mY;
    }

    double Vector2::Distance(const Vector2& vec2) const
    {
        return (*this - vec2).Length();
    }

    double Vector2::DistanceSquared(const Vector2& vec2) const
    {
        return (*this - vec2).LengthSquared();
    }

    double Vector2::Normalise()
    {
        double length = sqrt(mX * mX + mY * mY);
        if (length > 1.0e-15)
        {
            mX /= length;
            mY /= length;
        }
        return length;
    }

    double Vector2::operator [] (int index) const
    {
        return *(&mX + index);
    }

    double& Vector2::operator [] (int index)
    {
        return *(&mX + index);
    }
    bool Vector2::operator == (const Vector2& vec2) const
    {
        return ((*this - vec2).Length() < 1.0e-15);
    }

    Vector2 Vector2::operator + (const Vector2& vec2) const
    {
        return Vector2(mX + vec2[0], mY + vec2[1]);
    }

    Vector2 Vector2::operator - (const Vector2& vec2) const
    {
        return Vector2(mX - vec2[0], mY - vec2[1]);
    }

    Vector2 Vector2::operator * (double s) const
    {
        return Vector2(mX * s, mY * s);
    }

    double Vector2::operator * (const Vector2& vec2) const
    {
        return (mX * vec2[0] + mY * vec2[1]);
    }

    Vector2 Vector2::operator / (double s) const
    {
        return Vector2(mX / s, mY / s);
    }

    Vector2 Vector2::operator - (void) const
    {
        return Vector2(-mX, -mY);
    }

    Vector2& Vector2::operator += (const Vector2& vec2)
    {
        mX += vec2[0];
        mY += vec2[1];
        return *this;
    }

    Vector2& Vector2::operator -= (const Vector2& vec2)
    {
        mX -= vec2[0];
        mY -= vec2[1];
        return *this;
    }

    Vector2& Vector2::operator *= (double s)
    {
        mX *= s;
        mY *= s;
        return *this;
    }

    Vector2& Vector2::operator /= (double s)
    {
        mX /= s;
        mY /= s;
        return *this;
    }

    Vector2::~Vector2()
    {
    }
}