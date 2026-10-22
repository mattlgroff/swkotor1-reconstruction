// normalisers (0x004A9D60, 0x004AB130, 0x004AB180). Separate TU so
// the magnitude / quaternion-scale helpers stay out-of-line calls.
#include "i/vector_math.h"
#include <math.h>

// FUNCTION: SWKOTOR 0x004A9D60
Quaternion normalize(Quaternion q)
{
    float fLen = (float)sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    if (fLen == 0.0f) {
        q.w = 1.0f;
        q.x = 0.0f;
        q.y = 0.0f;
        q.z = 0.0f;
        return q;
    }
    return FUN_004a9b80(q, g_fOne / fLen);
}

// FUNCTION: SWKOTOR 0x004AB130
void Vector::Normalize()
{
    float fLen = FUN_004ab100(this);
    if (fLen < 0.0001) {
        x = 1.0f;
        y = 0.0f;
        z = 0.0f;
        return;
    }
    fLen = g_fOne / fLen;
    x = fLen * x;
    y = fLen * y;
    z = fLen * z;
}

// FUNCTION: SWKOTOR 0x004AB180
Vector NormalizeVector(const Vector& v)
{
    float fLen = FUN_004ab100(&v);
    if (fLen < 0.0001) {
        return Vector(1.0f, 0.0f, 0.0f);
    }
    float fInv = g_fOne / fLen;
    return Vector(fInv * v.x, fInv * v.y, fInv * v.z);
}
