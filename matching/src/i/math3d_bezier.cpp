// Cubic Bezier through de Casteljau (math3d.o, 0x004AC110). No Mac name;
// descriptive.
#include "i/math3d.h"

// FUNCTION: SWKOTOR 0x004AC110
Vector InterpolateBezier(const Vector& p0, const Vector& p3, const Vector& p1, const Vector& p2, float t)
{
    float s = g_fOne - t;
    Vector q[3];
    q[0] = p0 * s + p1 * t;
    q[1] = p1 * s + p2 * t;
    q[2] = p2 * s + p3 * t;
    Vector d = q[0] * s + q[1] * t;
    Vector e = q[1] * s + q[2] * t;
    Vector r = d * s + e * t;
    return r;
}
