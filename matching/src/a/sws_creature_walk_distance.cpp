#include <math.h>
struct Vector {
    Vector() {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    Vector operator-(const Vector& v) const { return Vector(x-v.x,y-v.y,z-v.z); }
    float x, y, z;
};
extern float VectorMagnitude(const Vector& v); // 0x004AB100
struct CWalkPathData {
    char pad_0[0x8c];
    int m_nPointCount;
    float* m_pPoints;
    char pad_94[8];
    int m_nFirstPoint;
};
class CSWSCreature {
public:
    float FUN_005111c0(); // Established symbol; ComputeTotalWalkDistance hint.
    float ComputeSpeedFactor(float fDistance, int nMilliseconds);
    char pad_0[0x90];
    Vector m_vPosition;
    char pad_9c[0xb8-0x9c];
    float m_fSpeedFactor;
    char pad_bc[0x340-0xbc];
    CWalkPathData* m_pPathData;
};
// NONMATCH: 5 diff: loop rotation and induction scheduling.
// FUNCTION: SWKOTOR 0x005111C0
float CSWSCreature::FUN_005111c0()
{
    int nFirst = m_pPathData->m_nFirstPoint;
    int nCount = m_pPathData->m_nPointCount;
    float fTotal = 0.0f;
    for (int i = nFirst; i < nCount; i += 2) {
        float* pPoints = m_pPathData->m_pPoints;
        Vector vPoint(pPoints[i], pPoints[i+1], 0.0f);
        if (i == nFirst) {
            fTotal += VectorMagnitude(vPoint - m_vPosition);

        } else {
            Vector vPrevious(pPoints[i-2], pPoints[i-1], 0.0f);
            fTotal += VectorMagnitude(vPoint - vPrevious);

        }
    }
    return fTotal;
}
extern const float g_fSpeedThousand; // 0x0073D6FC
extern const float g_fSpeedOne; // 0x0073D764
extern const float g_fSpeedHundredth; // 0x0073DFEC
extern const float g_fSpeedTenth; // 0x0073E798
extern const float g_fSpeedMilli; // 0x0073D708
extern const float g_fSpeedScale; // 0x00747C40
// NONMATCH: 12 diff: multiplication scheduling and cap stack-slot reuse.
// FUNCTION: SWKOTOR 0x00512F10
float CSWSCreature::ComputeSpeedFactor(float fDistance, int nMilliseconds)
{
    float fTotal = FUN_005111c0();
    fDistance *= g_fSpeedThousand;
    fDistance *= g_fSpeedScale;
    float fSpeed = g_fSpeedOne;
    if (fTotal <= fDistance) {
        float fRatio = fTotal / fDistance;
        if (fRatio >= g_fSpeedHundredth) fRatio = sqrt(fRatio);
        else fRatio = g_fSpeedTenth;
        fSpeed = fRatio - nMilliseconds * g_fSpeedMilli;
    }
    if (m_fSpeedFactor <= g_fSpeedOne) {
        float fCap = nMilliseconds * g_fSpeedMilli + m_fSpeedFactor;
        if (fSpeed >= fCap) fSpeed = fCap;
    }
    if (fSpeed <= g_fSpeedTenth) fSpeed = g_fSpeedTenth;
    return fSpeed;
}
