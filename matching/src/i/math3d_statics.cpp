// math3d.o helpers that keep their scratch in function statics
// (0x004AB250-0x004AB630). Names from the Mac debug map; segmentproject
// is descriptive.
#include "i/math3d.h"
#include <math.h>

// FUNCTION: SWKOTOR 0x004AB250
Vector planelineintersection(const Vector& vNormal, float fDist, const Vector& vStart, const Vector& vEnd)
{
    static Vector vDir;
    vDir = vEnd - vStart;
    float fDenom = vDir.x * vNormal.x + vDir.y * vNormal.y + vDir.z * vNormal.z;
    float fT = 0.0f;
    if (fDenom != 0.0f) {
        fT = -(vStart.x * vNormal.x + vStart.y * vNormal.y + vStart.z * vNormal.z + fDist) / fDenom;
    }
    return vDir * fT + vStart;
}

// FUNCTION: SWKOTOR 0x004AB360
Vector segmentproject(const Vector& a, const Vector& b, const Vector& p)
{
    if (a == b) {
        return a;
    }
    static Vector vDir;
    vDir = b - a;
    float fT = vDir.x * vDir.x + vDir.y * vDir.y + vDir.z * vDir.z;
    if (fT != 0.0f) {
        fT = ((p - a) * vDir) / fT;
    }
    if (1.0f < fT) {
        fT = 1.0f;
    } else if (0.0f > fT) {
        fT = 0.0f;
    }
    return vDir * fT + a;
}

// NONMATCH: register allocation; the original keeps vNormal.x in ebx for
// the final copy into n.
// FUNCTION: SWKOTOR 0x004AB4C0
void Plane::Transform(const Vector& vTranslation, const Quaternion& qRotation)
{
    static Vector vNormal;
    static Vector vPoint;
    vNormal = -qRotation * n;
    vPoint = -qRotation * (-n * d - vTranslation);
    n.x = vNormal.x;
    n.y = vNormal.y;
    n.z = vNormal.z;
    d = -(vPoint * vNormal);
}

// FUNCTION: SWKOTOR 0x004AB630
Quaternion RotationArc(Vector vFrom, Vector vTo)
{
    static Quaternion q;
    vFrom.Normalize();
    vTo.Normalize();
    Vector c = FUN_004ab210(vFrom, vTo);
    float d = vFrom * vTo;
    if (d <= -1.0f) {
        return Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
    }
    float s = (float)sqrt((d + 1.0f) * 2.0f);
    q.x = c.x * (1.0f / s);
    q.y = c.y * (1.0f / s);
    q.z = c.z * (1.0f / s);
    q.w = s * 0.5f;
    return q;
}
