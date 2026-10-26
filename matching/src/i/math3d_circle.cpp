// math3d.o: segment / sphere intersection (0x004AAB50). The second root
// goes through the out-of-line Vector operators; the first is spelled out.
#include "i/vector_math.h"
#include <math.h>

Vector operator+(const Vector& a, const Vector& b);         // 0x00423B90
Vector operator*(const Vector& v, float f);                 // 0x00423BF0

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// FUNCTION: SWKOTOR 0x004AAB50
int IntersectCircle2d(const Vector& vCenter, float fRadius, const Vector& vStart, const Vector& vEnd,
                      Vector* pOut1, Vector* pOut2, int bFlag)
{
    const Vector* pCenter = &vCenter;
    const Vector* pStart = &vStart;
    const Vector* pEnd = &vEnd;
    Vector vDir = *pEnd - *pStart;
    Vector vOff = *pStart - *pCenter;
    float a = vDir.z * vDir.z + vDir.y * vDir.y + vDir.x * vDir.x;
    float b = (vOff * vDir) * 2.0f;
    float c = vOff * vOff - fRadius * fRadius;
    float fDisc = b * b - a * c * 4.0f;
    if (fDisc == g_fZero) {
        float t = -b / (a * 2.0f);
        if (t >= g_fZero && t <= g_fOne) {
            Vector vScaled(t * vDir.x, t * vDir.y, t * vDir.z);
            *pOut1 = Vector(vScaled.x + pStart->x, vScaled.y + pStart->y, vScaled.z + pStart->z);
            return 1;
        }
        return 0;
    }
    if (fDisc > g_fZero) {
        float fSqrt = (float)sqrt(fDisc);
        int nRoots = 2;
        float t1 = (fSqrt - b) / (a * 2.0f);
        if (t1 >= g_fZero && t1 <= g_fOne) {
            Vector vScaled(t1 * vDir.x, t1 * vDir.y, t1 * vDir.z);
            *pOut1 = Vector(vScaled.x + pStart->x, vScaled.y + pStart->y, vScaled.z + pStart->z);
        } else {
            nRoots = 1;
        }
        float t2 = (-b - fSqrt) / (a * 2.0f);
        if (t2 >= g_fZero && t2 <= g_fOne) {
            if (nRoots == 2) {
                *pOut2 = *pStart + vDir * t2;
            } else {
                *pOut1 = *pStart + vDir * t2;
            }
        } else {
            nRoots--;
        }
        return nRoots;
    }
    return 0;
}
