// math3d.o x87 helpers (0x004A99B0-0x004ACCA0).
#include "i/vector_math.h"
#include <math.h>

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Matrix {
public:
    Matrix() {}
    Matrix(Vector a, Vector b, Vector c);                   // 0x00437BA0
    float m[9];
};

// FUNCTION: SWKOTOR 0x004A99B0
Matrix Transpose(const Matrix& m)
{
    return Matrix(Vector(m.m[0], m.m[3], m.m[6]), Vector(m.m[1], m.m[4], m.m[7]),
                  Vector(m.m[2], m.m[5], m.m[8]));
}

static inline float Twice(float f)
{
    return f + f;
}

static inline float Row(float a, float b, float c, float x, float y, float z)
{
    return a * x + b * y + c * z;
}

extern int g_bQuaternionIdentityTest;                    // 0x007A1980
extern float g_fRadToDeg;                               // 0x00742EF8

// NONMATCH: cl factors the 2 out of the last two terms of each row (x87
// reassociation) and assigns different stack slots to the products.
// FUNCTION: SWKOTOR 0x004A9BB0 SYMBOL: ??D@YA?AVVector@@ABVQuaternion@@ABV0@@Z
Vector operator*(const Quaternion& q, const Vector& v)
{
    if (g_bQuaternionIdentityTest
        && (*(const int*)&q.w == 0x3f800000 || *(const int*)&q.w == 0xbf800000)) {
        return v;
    }
    float xx, zy, xw, zz, yy, yx, zw, yw, zx;
    xx = q.x * q.x;
    yy = q.y * q.y;
    zz = q.z * q.z;
    yx = q.y * q.x;
    zx = q.z * q.x;
    xw = q.x * q.w;
    zy = q.z * q.y;
    yw = q.y * q.w;
    zw = q.z * q.w;
    return Vector(Row(g_fOne - Twice(zz + yy), Twice(yx - zw), Twice(yw + zx), v.x, v.y, v.z),
                  Row(g_fOne - Twice(zz + xx), Twice(zw + yx), Twice(zy - xw), v.y, v.x, v.z),
                  Row(g_fOne - Twice(yy + xx), Twice(zx - yw), Twice(zy + xw), v.z, v.x, v.y));
}

// FUNCTION: SWKOTOR 0x004A9E00
Quaternion Slerp(Quaternion from, const Quaternion& to, float t)
{
    float fDot = FUN_004a9d30(from, to);
    if (fDot < g_fZero) {
        from.x = -from.x;
        from.y = -from.y;
        from.z = -from.z;
        from.w = -from.w;
    }
    float fCos = fDot < g_fZero ? -fDot : fDot;
    if (fCos >= 1.0) {
        return from;
    }
    float fAngle = (float)acos(fCos);
    if (fAngle == g_fZero) {
        return from;
    }
    float fSin = (float)sin(fAngle);
    float fAt = fAngle * t;
    Quaternion r = FUN_004a9d00(FUN_004a9b80(from, (float)(sin(fAngle - fAt) / fSin)),
                                FUN_004a9b80(to, (float)(sin(fAt) / fSin)));
    return r;
}

static __forceinline Vector YAxisOf(const Quaternion& q)
{
    Vector v;
    v.x = (q.y * q.x - q.z * q.w) * 2.0f;
    v.y = g_fOne - (q.z * q.z + q.x * q.x) * 2.0f;
    v.z = (q.y * q.z + q.w * q.x) * 2.0f;
    return v;
}

// NONMATCH: only the static z store is scheduled before the first
// compare's fld (original: after it).
// FUNCTION: SWKOTOR 0x004A9F40
float GetPitch(const Quaternion& q)
{
    static Vector vDir(0.0f, 0.0f, 0.0f);
    vDir = YAxisOf(q);
    if (vDir.y != 0.0 || vDir.x != 0.0) {
        return (float)(atan2(-vDir.x, vDir.y) * g_fRadToDeg);
    }
    return g_fZero;
}

static inline float RollOf(Quaternion q)
{
    return (float)(atan2(-((q.z * q.x - q.y * q.w) * 2.0f),
                         g_fOne - (q.y * q.y + q.z * q.z) * 2.0f) * g_fRadToDeg);
}

// NONMATCH: the second product lands in its own temporary and q is not
// overwritten; the fchs of the atan2 numerator is issued late.
// FUNCTION: SWKOTOR 0x004ACB90
float GetEulerRoll(Quaternion q)
{
    q = FUN_004a9ac0(Quaternion(g_vZAxis, GetPitch(q) * -0.017453292f), q);
    q = FUN_004a9ac0(Quaternion(g_vXAxis, FUN_004aa020(q) * -0.017453292f), q);
    return RollOf(q);
}

// FUNCTION: SWKOTOR 0x004ABDD0
int SphereIntersect(Vector* pStart, Vector* pEnd, Vector* pCenter, float fRadius)
{
    Vector vDir = *pEnd - *pStart;
    float fLenSq = vDir * vDir;
    if (fLenSq < 1e-9) {
        return 0;
    }
    float t = ((*pCenter - *pStart) * vDir) / fLenSq;
    if (!(g_fZero > t)) {
        if (g_fOne < t) {
            t = g_fOne;
        }
    } else {
        t = g_fZero;
    }
    Vector vDelta = vDir * t + *pStart - *pCenter;
    if (FUN_004ab100(&vDelta) < fRadius) {
        return 1;
    }
    return 0;
}

// NONMATCH: min/max box pointers get ebp/ebx swapped (register tie), and
// the last face computes t * end.y before u * start.y.
// FUNCTION: SWKOTOR 0x004AA610
int BoxIntersect(Vector* pStart, Vector* pEnd, Vector* pMin, Vector* pMax)
{
    unsigned char nStart;
    unsigned char nEnd;
    Outcode(pStart, pMin, pMax, &nStart);
    if (nStart == 0) {
        return -1;
    }
    Outcode(pEnd, pMin, pMax, &nEnd);
    if (nStart & nEnd) {
        return 0;
    }
    if (nStart & 0x20) {
        float t = (pMin->x - pStart->x) / (pEnd->x - pStart->x);
        float u = g_fOne - t;
        float y = u * pStart->y + t * pEnd->y;
        float z = u * pStart->z + t * pEnd->z;
        if (y >= pMin->y && y <= pMax->y && z >= pMin->z && z <= pMax->z) {
            return 1;
        }
    } else if (nStart & 0x10) {
        float t = (pMax->x - pStart->x) / (pEnd->x - pStart->x);
        float u = g_fOne - t;
        float y = u * pStart->y + t * pEnd->y;
        float z = u * pStart->z + t * pEnd->z;
        if (y >= pMin->y && y <= pMax->y && z >= pMin->z && z <= pMax->z) {
            return 2;
        }
    }
    if (nStart & 8) {
        float t = (pMin->y - pStart->y) / (pEnd->y - pStart->y);
        float u = g_fOne - t;
        float x = u * pStart->x + t * pEnd->x;
        float z = u * pStart->z + t * pEnd->z;
        if (x >= pMin->x && x <= pMax->x && z >= pMin->z && z <= pMax->z) {
            return 3;
        }
    } else if (nStart & 4) {
        float t = (pMax->y - pStart->y) / (pEnd->y - pStart->y);
        float u = g_fOne - t;
        float x = u * pStart->x + t * pEnd->x;
        float z = u * pStart->z + t * pEnd->z;
        if (x >= pMin->x && x <= pMax->x && z >= pMin->z && z <= pMax->z) {
            return 4;
        }
    }
    if (nStart & 2) {
        float t = (pMin->z - pStart->z) / (pEnd->z - pStart->z);
        float u = g_fOne - t;
        float x = u * pStart->x + t * pEnd->x;
        float y = u * pStart->y + t * pEnd->y;
        if (y >= pMin->y && y <= pMax->y && x >= pMin->x && x <= pMax->x) {
            return 5;
        }
    } else if (nStart & 1) {
        float t = (pMax->z - pStart->z) / (pEnd->z - pStart->z);
        float u = g_fOne - t;
        float x = u * pStart->x + t * pEnd->x;
        float y = t * pEnd->y + u * pStart->y;
        if (y >= pMin->y && y <= pMax->y && x >= pMin->x && x <= pMax->x) {
            return 6;
        }
    }
    return 0;
}
