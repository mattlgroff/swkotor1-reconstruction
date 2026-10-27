// DistanceBetweenLines: shortest distance between two lines (and the
// closest points), with the common normal kept in a function static.
#include "i/math3d.h"

Vector NormalizeVector(const Vector& v);            // 0x004AB180

static inline float Abs(float f)
{
    return f < 0.0f ? -f : f;
}

// FUNCTION: SWKOTOR 0x004ABF00
float DistanceBetweenLines(const Vector& vPoint1, const Vector& vDir1, const Vector& vPoint2,
                           const Vector& vDir2, Vector* pClosest1, Vector* pClosest2)
{
    static Vector s_vAxis;
    s_vAxis = FUN_004ab210(vDir1, vDir2);
    s_vAxis.Normalize();
    float fDistance = Abs(-(s_vAxis * vPoint1) - -(s_vAxis * vPoint2));
    if (pClosest1) {
        Vector vNormal = NormalizeVector(FUN_004ab210(vDir2, s_vAxis));
        float fPlane = -(vNormal * vPoint2);
        *pClosest1 = planelineintersection(vNormal, fPlane, vPoint1, vPoint1 + vDir1);
    }
    if (pClosest2) {
        Vector vNormal = NormalizeVector(FUN_004ab210(vDir1, s_vAxis));
        float fPlane = -(vNormal * vPoint1);
        *pClosest2 = planelineintersection(vNormal, fPlane, vPoint2, vPoint2 + vDir2);
    }
    return fDistance;
}
