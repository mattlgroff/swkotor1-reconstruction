// WindManager::PointSourceWindVector.
#include <math.h>
#include "i/wind_manager.h"

// Typed view of the 0x18-byte PointSourceWind record.
struct PointSourceWindData {
    Vector vPosition;                               // +0x00
    float fRadius;                                  // +0x0c
    float f10;                                      // +0x10
    float fStrength;                                // +0x14
};

#define WIND_ABS(f) ((f) < g_fZero ? -(f) : (f))

static inline Vector operator-(const Vector& a, const Vector& b)
{
    Vector r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static inline Vector operator*(float f, const Vector& v)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

// FUNCTION: SWKOTOR 0x004A5C80
Vector* WindManager::PointSourceWindVector(Vector* out, Vector point)
{
    Vector vSum(0.0f, 0.0f, 0.0f);
    Vector vDelta(0.0f, 0.0f, 0.0f);
    float fDistance;
    for (int i = 0; i < m_lPointSourceWinds.num; i++) {
        PointSourceWindData* pWind = (PointSourceWindData*)&m_lPointSourceWinds.element[i];
        vDelta = point - pWind->vPosition;
        fDistance = WIND_ABS(vDelta.x);
        if (WIND_ABS(vDelta.y) > fDistance) {
            fDistance = WIND_ABS(vDelta.y);
        }
        if (fabs(vDelta.z) > fDistance) {
            fDistance = WIND_ABS(vDelta.z);
        }
        if (fDistance < pWind->fRadius) {
            vSum += ((pWind->fRadius - fDistance) / pWind->fRadius * pWind->fStrength) *
                    NormalizeVector(vDelta);
        }
    }
    *out = vSum;
    return out;
}
