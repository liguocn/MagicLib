#pragma once

namespace MagicMath
{
    class HomoVector4
    {
    public:
        HomoVector4();
        HomoVector4(double x, double y, double z, double h);

        double Distance(const HomoVector4 vec4) const;
        double DistanceSquare(const HomoVector4 vec4) const;
        bool NormalisePoint();

        double Length() const;
        double LengthSquare() const;
        double NormaliseVector();
        HomoVector4 CrossProduct(const HomoVector4& vec4) const;

        double operator [] (int index) const;
        double& operator [] (int index);
        bool operator == (const HomoVector4& vec4) const;
        HomoVector4  operator + (const HomoVector4& vec4) const;
        HomoVector4  operator - (const HomoVector4& vec4) const;
        HomoVector4  operator * (double s) const;
        double operator * (const HomoVector4& vec4) const;
        HomoVector4  operator / (double s) const;
        HomoVector4  operator - (void) const;
        HomoVector4& operator += (const HomoVector4& vec4);
        HomoVector4& operator -= (const HomoVector4& vec4);
        HomoVector4& operator *= (double s);
        HomoVector4& operator /= (double s);

        ~HomoVector4();

    private:
        double mX, mY, mZ, mH;
    };
}