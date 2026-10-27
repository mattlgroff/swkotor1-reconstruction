// PointTo: the rotation that turns +Z towards a direction.
#include <math.h>
#include "i/vector_math.h"

static inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// FUNCTION: SWKOTOR 0x004ACCA0
Quaternion PointTo(const Vector& vDirection)
{
    static Vector s_vDirection(0.0f, 0.0f, 0.0f);
    static Vector s_vAxis(0.0f, 0.0f, 0.0f);
    s_vDirection = NormalizeVector(vDirection);
    s_vAxis = FUN_004ab210(Vector(0.0f, 0.0f, 1.0f), s_vDirection);
    float fAngle = FUN_004ab100(&s_vAxis);
    if (fAngle > 1.0f) {
        fAngle = 1.0f;
    }
    else if (fAngle < -1.0f) {
        fAngle = -1.0f;
    }
    else if (fAngle == 0.0f) {
        fAngle = s_vDirection.z > 0.0f ? 0.0f : 3.1415927f;
        return Quaternion(g_vXAxis, fAngle);
    }
    fAngle = (float)asin(fAngle);
    if (s_vDirection * Vector(0.0f, 0.0f, 1.0f) < 0.0f) {
        fAngle = 3.1415927f - fAngle;
    }
    return Quaternion(NormalizeVector(s_vAxis), fAngle);
}
