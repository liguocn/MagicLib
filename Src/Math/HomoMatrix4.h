#pragma once
#include "Vector3.h"
#include "HomoVector4.h"

namespace MagicMath
{
    class HomoMatrix4
    {
    public:
        HomoMatrix4();
        HomoMatrix4(const HomoMatrix4& mat);
        HomoMatrix4(const double* pValues);
        ~HomoMatrix4();

        void Print() const;
        Vector3 TransformPoint(const Vector3& point) const;
        HomoVector4 TransformPoint(const HomoVector4& point) const;
        Vector3 RotateVector(const Vector3& vec) const;
        HomoVector4 RotateVector(const HomoVector4& vec) const;
        HomoMatrix4 operator * (const HomoMatrix4& mat) const;
        HomoMatrix4 operator *= (const HomoMatrix4& mat);
        double GetValue(int row, int col) const;
        void SetValue(int row, int col, double v);
        void Unit();
        HomoMatrix4 Inverse();
        //make sure source and target are unit vector (direction)
        //source * target != -1
        //Since if source * target == -1, the rotation is not unique
        void GenerateVectorToVectorRotation(const Vector3& source, const Vector3& target);
        void GenerateTranslation(const Vector3& translate);

    private:
        double mValues[16];
    };

}