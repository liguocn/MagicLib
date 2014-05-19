#pragma once

namespace MagicMath
{
    class Vector3
    {
    public:
        Vector3();
        Vector3(double x, double y, double z);

        double Length() const;
        double LengthSquared() const;
        double Distance(const Vector3& vec3) const;
        double DistanceSquared(const Vector3& vec3) const;
        double Normalise();
        Vector3 CrossProduct(const Vector3& vec3) const;

        double   operator [] (int index) const;
        double&  operator [] (int index);
        bool     operator == (const Vector3& vec3) const;
        bool     operator < (const Vector3& vec3) const; // just used in std::set
        Vector3  operator + (const Vector3& vec3) const;
        Vector3  operator - (const Vector3& vec3) const;
        Vector3  operator * (double s) const;
        double   operator * (const Vector3& vec3) const;
        Vector3  operator / (double s) const;
        Vector3  operator - (void) const;
        Vector3& operator += (const Vector3& vec3);
        Vector3& operator -= (const Vector3& vec3);
        Vector3& operator *= (double s);
        Vector3& operator /= (double s);

        ~Vector3();

    private:
        double mX, mY, mZ;

    };
}