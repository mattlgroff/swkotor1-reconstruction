// Pilot 4 arm B: 0x004A5950 global wind sample without a point (x87,
// quaternion rotation of the wind direction by two noise angles).
// NONMATCH (45 diff insns, same size): the inlined rotation's four spilled
// products (xz, xw, yz, yw) land in different reused stack slots than the
// original's (orig xz->sinX slot, xw->cosY, yz->cosX, yw->angleY), which also
// reorders the off-diagonal terms; first noise lookup is [eax+edx] vs
// [edx+eax]. Everything before the rotation matches.
#include <stdlib.h>

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

inline Vector operator*(const Vector &v, float f)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

class Quaternion {
public:
    Quaternion() { x = y = z = 0.0f; w = 1.0f; }
    float w, x, y, z;
};

__forceinline Vector operator*(const Quaternion &q, const Vector &v)
{
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    float xy = q.y * q.x;
    float yw, yz, xw, xz;
    xz = q.z * q.x;
    xw = q.x * q.w;
    yz = q.z * q.y;
    yw = q.y * q.w;
    float zw = q.z * q.w;
    float m00 = 1.0f - (yy + zz) * 2.0f;
    float m01 = (xy - zw) * 2.0f;
    float m02 = (xz + yw) * 2.0f;
    float m10 = (xy + zw) * 2.0f;
    float m11 = 1.0f - (xx + zz) * 2.0f;
    float m12 = (yz - xw) * 2.0f;
    float m20 = (xz - yw) * 2.0f;
    float m21 = (yz + xw) * 2.0f;
    float m22 = 1.0f - (xx + yy) * 2.0f;
    return Vector(m00 * v.x + m02 * v.z + m01 * v.y,
                  m10 * v.x + m11 * v.y + m12 * v.z,
                  m20 * v.x + m21 * v.y + m22 * v.z);
}

float FUN_004aadf0(float f);                    // 0x004AADF0 (table cosine)
float FUN_004aae30(float f);                    // 0x004AAE30 (table sine)

class CAurWind {
public:
    Vector GetGlobalWindNoPoint(const Vector &vPos, float fScale);
    char pad_0x0[0x18];
    Vector m_vDirection;                        // 0x18
    char pad_0x24[0x830 - 0x24];
    float *m_pNoiseA;                           // 0x830
    float *m_pNoiseB;                           // 0x834
    float m_fAmplitudeX;                        // 0x838
    float m_fAmplitudeY;                        // 0x83c
    float m_fBlend;                             // 0x840
};

// FUNCTION: SWKOTOR 0x004A5950
Vector CAurWind::GetGlobalWindNoPoint(const Vector &vPos, float fScale)
{
    const Vector &vDir = m_vDirection;
    if (vDir.x == 0.0f && vDir.y == 0.0f && vDir.z == 0.0f) {
        return vDir;
    }
    int nX = abs((int)vPos.x * 4) % 16;
    int nY = abs((int)vPos.y * 4) % 16;
    float fAngleX = 0.0f;
    float fAngleY = 0.0f;
    static Quaternion q;
    float fInvBlend = 1.0f - m_fBlend;
    float (*pNoiseA)[16] = (float (*)[16])m_pNoiseA;
    float (*pNoiseB)[16] = (float (*)[16])m_pNoiseB;
    float fNoise = fInvBlend * pNoiseA[nX][nY] + pNoiseB[nX][nY] * m_fBlend;
    Vector v = vDir * (fScale * fNoise);
    if (m_fAmplitudeX != 0.0f) {
        fAngleX = (fNoise * m_fAmplitudeX * 2.0f - m_fAmplitudeX) * 0.5f;
    }
    if (m_fAmplitudeY != 0.0f) {
        float fNoiseY = fInvBlend * pNoiseA[nY][nX] + pNoiseB[nY][nX] * m_fBlend;
        fAngleY = (fNoiseY * m_fAmplitudeY * 2.0f - m_fAmplitudeY) * 0.5f;
    }
    float fSinY, fCosY, fSinX, fCosX;
    fCosX = FUN_004aadf0(fAngleX);
    fSinX = FUN_004aae30(fAngleX);
    fCosY = FUN_004aadf0(fAngleY);
    fSinY = FUN_004aae30(fAngleY);
    q.w = fCosY * fCosX;
    q.x = fCosX * fSinY;
    q.y = fSinY * fSinX;
    q.z = fCosY * fSinX;
    return q * v;
}
