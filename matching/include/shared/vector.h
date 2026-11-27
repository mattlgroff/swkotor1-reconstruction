// vector.h -- Vector, the engine's three-float vector (math3d)
//
// One declaration for every unit (CODE_RULES.md, R1). The compound
// assignment operators and Normalize are out-of-line functions of math3d.o
// in the Mac symbol tables, at the addresses given. The arithmetic operators
// are free functions (the Mac tables name operator/(Vector const&, float));
// shared/vector_ops.h has their inline forms.
#ifndef SHARED_VECTOR_H
#define SHARED_VECTOR_H

class Vector
{
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }

    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    operator const float*() const { return &x; }

    Vector& operator+=(const Vector& v);                    // 0x004A9740
    Vector& operator-=(const Vector& v);                    // 0x004A9770
    Vector& operator*=(float f);                            // 0x004A97A0
    Vector& operator/=(float f);                            // 0x004A97D0
    int IsZero();                                           // 0x0046A620
    void Normalize();                                       // 0x004AB130

    float x;
    float y;
    float z;
};

#endif
