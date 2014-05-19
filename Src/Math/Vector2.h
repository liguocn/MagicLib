#pragma once

namespace MagicMath
{
    class Vector2
    {
    public:
        Vector2();
        Vector2(double x, double y);

        double Length() const;
        double LengthSquared() const;
        double Distance(const Vector2& vec2) const;
        double DistanceSquared(const Vector2& vec2) const;
        double Normalise();
        //Vector2 CrossProduct(const Vector2& vec2) const;

        double     operator [] (int index) const;
        double&    operator [] (int index);
        bool     operator == (const Vector2& vec2) const;
        Vector2  operator + (const Vector2& vec2) const;
        Vector2  operator - (const Vector2& vec2) const;
        Vector2  operator * (double s) const;
        double     operator * (const Vector2& vec2) const;
        Vector2  operator / (double s) const;
        Vector2  operator - (void) const;
        Vector2& operator += (const Vector2& vec2);
        Vector2& operator -= (const Vector2& vec2);
        Vector2& operator *= (double s);
        Vector2& operator /= (double s);

        ~Vector2();
    private:
        double mX, mY;
    };


}