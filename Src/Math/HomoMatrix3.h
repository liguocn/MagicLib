#pragma once

namespace MagicMath
{
    class HomoMatrix3
    {
    public:
        HomoMatrix3();
        ~HomoMatrix3();

        void Print() const;
        void TransformPoint(double x, double y, double& resX, double& resY) const;
        void GenerateTranslation(double deltaX, double deltaY);
        void GenerateRotation(double theta); //counterclockwise
        void GenerateScaling(double scaleX, double scaleY);
        void Unit(void);
        HomoMatrix3 ReverseRigidTransform(void);
        HomoMatrix3 operator * (const HomoMatrix3& mat) const;
        HomoMatrix3 operator *= (const HomoMatrix3& mat);
        double GetValue(int row, int col) const;
        void SetValue(int row, int col, double v);

    private:
        double mValues[9];
    };

}
