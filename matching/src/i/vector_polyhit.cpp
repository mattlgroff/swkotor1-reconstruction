// polyhit: segment / convex polygon intersection.
#include "i/math3d.h"

// FUNCTION: SWKOTOR 0x004AB750 SYMBOL: ?polyhit@@YAHPAVVector@@HABV1@ABM110@Z
int polyhit(Vector* pPoints, int nPoints, const Vector& vNormal, const float& fDist,
            const Vector& vStart, const Vector& vEnd, Vector* pHit)
{
    float fStart = vNormal * vStart + fDist;
    float fEnd = vNormal * vEnd + fDist;
    if (fStart < 0.0f || fEnd > 0.0f || fStart == fEnd) {
        return 0;
    }
    static Vector s_vHit;
    static Vector s_vCross;
    float fT = fStart / (fStart - fEnd);
    Vector vHit = vStart * (1.0f - fT) + vEnd * fT;
    s_vHit = vHit;
    int bInside = 1;
    for (int i = 0; bInside && i < nPoints; i++) {
        int nNext = (i + 1) % nPoints;
        s_vCross = FUN_004ab210(pPoints[nNext] - pPoints[i], vHit - pPoints[i]);
        bInside = s_vCross * vNormal >= -1e-5f;
    }
    if (bInside && pHit) {
        *pHit = vHit;
    }
    return bInside;
}

float FUN_004ab100(const Vector* v);                // 0x004AB100 (magnitude)

// FUNCTION: SWKOTOR 0x004AB9D0 SYMBOL: ?polyhit@@YAHPAVVector@@HABV1@100@Z
int polyhit(Vector* pPoints, int nPoints, const Vector& vStart, const Vector& vEnd,
            Vector* pHit, Vector* pNormal)
{
    Vector vSum;
    for (int i = 0; i < nPoints - 2; i++) {
        vSum = vSum + FUN_004ab210(pPoints[(i + 1) % nPoints] - pPoints[i],
                                   pPoints[(i + 2) % nPoints] - pPoints[(i + 1) % nPoints]);
    }
    float fMagnitude = FUN_004ab100(&vSum);
    if (fMagnitude < 1e-8f) {
        return 0;
    }
    float fInverse = 1.0f / fMagnitude;
    Vector vNormal;
    vNormal.x = vSum.x * fInverse;
    vNormal.y = vSum.y * fInverse;
    vNormal.z = vSum.z * fInverse;
    float fPlane = -(vNormal * pPoints[0]);
    float fStart = vNormal * vStart + fPlane;
    if (fStart < 0.0f) {
        return 0;
    }
    float fEnd = vNormal * vEnd + fPlane;
    if (fEnd > 0.0f || fStart == fEnd) {
        return 0;
    }
    static Vector s_vHit;
    float fT = fStart / (fStart - fEnd);
    Vector vHit = vStart * (1.0f - fT) + vEnd * fT;
    s_vHit = vHit;
    int bInside = 1;
    for (int j = 0; bInside && j < nPoints; j++) {
        Vector vPoint = pPoints[j];
        Vector vNext = pPoints[(j + 1) % nPoints];
        Vector vCross = FUN_004ab210(vNext - vPoint, vHit - vPoint);
        bInside = vCross * vNormal >= 0.0;
    }
    if (bInside) {
        if (pNormal) {
            *pNormal = vNormal;
        }
        if (pHit) {
            *pHit = s_vHit;
        }
    }
    return bInside;
}
