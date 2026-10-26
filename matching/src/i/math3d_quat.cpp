// math3d.o: axis-angle quaternion and triangle normal (0x004AC960-
// 0x004ACAC0). Names from the Mac debug map except YawPitchRoll
// (descriptive).
#include "i/vector_math.h"
#include <math.h>

// NONMATCH: the original keeps `this` in edx across the NormalizeVector
// call (cross-function register knowledge); ours saves it in esi.
// FUNCTION: SWKOTOR 0x004AC960
Quaternion::Quaternion(Vector vAxis, float fAngle)
{
    if (fAngle == 0.0f) {
        z = 0.0f;
        y = 0.0f;
        x = 0.0f;
        w = 1.0f;
        return;
    }
    vAxis = NormalizeVector(vAxis);
    float fHalf = fAngle * 0.5f;
    w = (float)cos(fHalf);
    float fSin = (float)sin(fHalf);
    x = vAxis.x * fSin;
    y = vAxis.y * fSin;
    z = vAxis.z * fSin;
}

// FUNCTION: SWKOTOR 0x004ACA00
Vector NormalOf(const Vector& a, const Vector& b, const Vector& c)
{
    Vector e1(c.x - b.x, c.y - b.y, c.z - b.z);
    Vector e2(b.x - a.x, b.y - a.y, b.z - a.z);
    Vector n = FUN_004ab210(e2, e1);
    float fLen = FUN_004ab100(&n);
    if (fLen == 0.0f) {
        return Vector(1.0f, 0.0f, 0.0f);
    }
    fLen = 1.0f / fLen;
    return Vector(n.x * fLen, n.y * fLen, n.z * fLen);
}

// NONMATCH: the original returns the hidden pointer from edx kept live
// across the 0x004A9AC0 call (cross-function register use).
// FUNCTION: SWKOTOR 0x004ACAC0
Quaternion YawPitchRoll(float fYaw, float fPitch, float fRoll)
{
    return FUN_004a9ac0(FUN_004a9ac0(Quaternion(g_vZAxis, fYaw * 0.017453292f),
                                     Quaternion(g_vXAxis, fPitch * 0.017453292f)),
                        Quaternion(g_vYAxis, fRoll * 0.017453292f));
}
