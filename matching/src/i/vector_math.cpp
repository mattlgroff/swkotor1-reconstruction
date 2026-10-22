// vector / quaternion leaf helpers (0x004A9680-0x004AA9C0).
#include "i/vector_math.h"
#include <math.h>

inline Vector operator*(const Vector& v, float f)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// FUNCTION: SWKOTOR 0x004A9680
VertexCache::VertexCache(int nSize)
{
    m_nSize = nSize;
    m_pEntries = new int[nSize];
    for (int i = 0; i < m_nSize; i++) {
        m_pEntries[i] = -1;
    }
}

// FUNCTION: SWKOTOR 0x004A96C0
VertexCache::~VertexCache()
{
    delete[] m_pEntries;
}

// FUNCTION: SWKOTOR 0x004A96D0
float FUN_004a96d0(float f)
{
    return f * 57.29578f;
}

// FUNCTION: SWKOTOR 0x004A96E0
float Round(float fValue, float fStep)
{
    return (float)(floor(fValue / fStep + 0.5f) * fStep);
}

// FUNCTION: SWKOTOR 0x004A9710
float FUN_004a9710(const Vector& v, float f, const Plane& p)
{
    return v.z * p.n.z + v.y * p.n.y + v.x * p.n.x + p.d - f;
}

// FUNCTION: SWKOTOR 0x004A9740
Vector& Vector::operator+=(const Vector& v)
{
    x = v.x + x;
    y = v.y + y;
    z = v.z + z;
    return *this;
}

// FUNCTION: SWKOTOR 0x004A9770
Vector& Vector::operator-=(const Vector& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// FUNCTION: SWKOTOR 0x004A97A0
Vector& Vector::operator*=(float f)
{
    x = f * x;
    y = f * y;
    z = f * z;
    return *this;
}

// FUNCTION: SWKOTOR 0x004A97D0
Vector& Vector::operator/=(float f)
{
    if (f != 0.0f) {
        float fInv = g_fOne / f;
        x = fInv * x;
        y = fInv * y;
        z = fInv * z;
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x004A9810
float FUN_004a9810(const Vector& v, float f, const Vector& n)
{
    if (v.z == 0.0f) {
        return g_fZero;
    }
    return -((v.y * n.y + v.x * n.x + f) / v.z);
}

// FUNCTION: SWKOTOR 0x004A9850
Vector FUN_004a9850(const Vector& n, float d, const Vector& v)
{
    float t = n.y * v.y + n.x * v.x + v.z * n.z + d;
    return v - n * t;
}

// FUNCTION: SWKOTOR 0x004A9A60
Vector FUN_004a9a60(const Matrix3& m, const Vector& v)
{
    return Vector(m.m[6] * v.z + m.m[3] * v.y + m.m[0] * v.x,
                  m.m[7] * v.z + m.m[1] * v.x + m.m[4] * v.y,
                  m.m[8] * v.z + m.m[2] * v.x + m.m[5] * v.y);
}

// FUNCTION: SWKOTOR 0x004A9AC0
Quaternion FUN_004a9ac0(const Quaternion& a, const Quaternion& b)
{
    return Quaternion(a.w * b.w - b.x * a.x - a.y * b.y - b.z * a.z,
                      b.z * a.y + b.x * a.w + b.w * a.x - a.z * b.y,
                      b.y * a.w - b.z * a.x + a.y * b.w + b.x * a.z,
                      b.y * a.x + b.z * a.w - b.x * a.y + a.z * b.w);
}

// FUNCTION: SWKOTOR 0x004A9B50
Quaternion FUN_004a9b50(const Quaternion& q)
{
    return Quaternion(q.w * -1.0f, q.x, q.y, q.z);
}

// FUNCTION: SWKOTOR 0x004A9B80
Quaternion FUN_004a9b80(const Quaternion& q, float f)
{
    return Quaternion(f * q.w, f * q.x, f * q.y, f * q.z);
}

// FUNCTION: SWKOTOR 0x004A9D00
Quaternion FUN_004a9d00(const Quaternion& a, const Quaternion& b)
{
    return Quaternion(a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z);
}

// FUNCTION: SWKOTOR 0x004A9D30
float FUN_004a9d30(const Quaternion& a, const Quaternion& b)
{
    return a.z * b.z + a.y * b.y + a.x * b.x + a.w * b.w;
}

// FUNCTION: SWKOTOR 0x004AA980
int operator==(const Vector& a, const Vector& b)
{
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004AA0F0
float FUN_004aa0f0(const Vector& v)
{
    if (v.y == 0.0 && v.x == 0.0) {
        return g_fZero;
    }
    return (float)atan2(-v.x, v.y) * 57.29578f;
}

// FUNCTION: SWKOTOR 0x004AA130
float FUN_004aa130(const Vector& v)
{
    float x = v.x;
    float y = v.y;
    float z = v.z;
    float r = (float)sqrt(y * y + x * x);
    return (float)atan2(z, r) * 57.29578f;
}

// FUNCTION: SWKOTOR 0x004AA160
void Outcode(Vector* pPoint, Vector* pMin, Vector* pMax, unsigned char* pCode)
{
    *pCode = 0;
    if (pPoint->x < pMin->x) {
        *pCode = 0x20;
    } else if (pPoint->x > pMax->x) {
        *pCode = 0x10;
    }
    if (pPoint->y < pMin->y) {
        *pCode |= 8;
    } else if (pPoint->y > pMax->y) {
        *pCode |= 4;
    }
    if (pPoint->z < pMin->z) {
        *pCode |= 2;
    } else if (pPoint->z > pMax->z) {
        *pCode |= 1;
    }
}

// FUNCTION: SWKOTOR 0x004AAAB0
Plane* FUN_004aaab0(const Vector& n, const float& d)
{
    static float s_plane[4];
    s_plane[0] = n.x;
    s_plane[1] = n.y;
    s_plane[2] = n.z;
    s_plane[3] = d;
    return (Plane*)s_plane;
}

// FUNCTION: SWKOTOR 0x004AADF0
float FUN_004aadf0(float f)
{
    int* pIndex = (int*)&f;
    float fScaled = f * 5.0929582f;
    __asm {
        fld fScaled
        mov edx, pIndex
        frndint
        fistp dword ptr [edx]
    }
    return g_afSinTable[*pIndex & 0x1f];
}

// FUNCTION: SWKOTOR 0x004AAE30
float FUN_004aae30(float f)
{
    int* pIndex = (int*)&f;
    float fScaled = f * 5.0929582f;
    __asm {
        fld fScaled
        mov edx, pIndex
        frndint
        fistp dword ptr [edx]
    }
    return g_afCosTable[*pIndex & 0x1f];
}

// FUNCTION: SWKOTOR 0x004AAE70
Vector FUN_004aae70(Vector a, Vector b)
{
    return Vector(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
}

// FUNCTION: SWKOTOR 0x004AAEE0
Vector FUN_004aaee0(Vector a, Vector b)
{
    return Vector(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
}

// FUNCTION: SWKOTOR 0x004AAF50
float FUN_004aaf50(const Vector& a, const Vector& b, const Plane& p)
{
    Vector v;
    v.z = p.n.z > 0.0f ? a.z : b.z;
    v.y = p.n.y > 0.0f ? a.y : b.y;
    v.x = p.n.x > 0.0f ? a.x : b.x;
    return v.y * p.n.y + v.x * p.n.x + v.z * p.n.z + p.d;
}

// FUNCTION: SWKOTOR 0x004AAFC0
Vector FUN_004aafc0(const CExoArrayList<Vector>& l)
{
    Vector* p = l.element;
    Vector v = p[0];
    for (int i = 0; i < l.num; i++) {
        if (!(v.x > p->x)) {
            v.x = p->x;
        }
        if (!(v.y > p->y)) {
            v.y = p->y;
        }
        if (!(v.z > p->z)) {
            v.z = p->z;
        }
        p++;
    }
    return v;
}

// FUNCTION: SWKOTOR 0x004AB060
Vector FUN_004ab060(const CExoArrayList<Vector>& l)
{
    Vector* p = l.element;
    Vector v = p[0];
    for (int i = 0; i < l.num; i++) {
        if (!(v.x < p->x)) {
            v.x = p->x;
        }
        if (!(v.y < p->y)) {
            v.y = p->y;
        }
        if (!(v.z < p->z)) {
            v.z = p->z;
        }
        p++;
    }
    return v;
}

// FUNCTION: SWKOTOR 0x004AB100
float FUN_004ab100(const Vector* v)
{
    return (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

// FUNCTION: SWKOTOR 0x004AB1E0
Vector FUN_004ab1e0(const Vector& a, const Vector& b)
{
    return Vector(a.x * b.x, a.y * b.y, a.z * b.z);
}

// FUNCTION: SWKOTOR 0x004AB210
Vector FUN_004ab210(const Vector& a, const Vector& b)
{
    return Vector(a.y * b.z - a.z * b.y, a.z * b.x - b.z * a.x, a.x * b.y - a.y * b.x);
}

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// FUNCTION: SWKOTOR 0x004A98B0
Vector lineproject(const Vector& a, const Vector& b, const Vector& p)
{
    static Vector s_vDir(0.0f, 0.0f, 0.0f);
    Vector vDir = b - a;
    s_vDir = vDir;
    float t = vDir.y * vDir.y + vDir.z * vDir.z + vDir.x * vDir.x;
    if (t != 0.0f) {
        Vector vRel = p - a;
        t = (vRel.x * vDir.x + vRel.y * vDir.y + vRel.z * vDir.z) / t;
    }
    Vector vOffset(vDir.x * t, vDir.y * t, vDir.z * t);
    return a + vOffset;
}

// FUNCTION: SWKOTOR 0x004AA020
float FUN_004aa020(const Quaternion& q)
{
    static Vector s_vAxis(0.0f, 0.0f, 0.0f);
    Vector v((q.y * q.x - q.z * q.w) * 2.0f, 1.0f - (q.z * q.z + q.x * q.x) * 2.0f,
             (q.y * q.z + q.w * q.x) * 2.0f);
    s_vAxis = v;
    return FUN_004aa130(v);
}

// FUNCTION: SWKOTOR 0x004AAAE0
float FUN_004aaae0(float a, float b, float c, float d, float t)
{
    float u = g_fOne - t;
    float p[3];
    p[0] = a * u + c * t;
    p[1] = u * c + d * t;
    p[2] = u * d + b * t;
    p[0] = p[0] * u + p[1] * t;
    p[1] = p[1] * u + p[2] * t;
    p[0] = p[0] * u + p[1] * t;
    return p[0];
}
