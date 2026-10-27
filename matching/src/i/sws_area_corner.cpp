// CSWSArea::ComputeBestCorner: bisect the widest walkable cut across a
// path corner.
#include <math.h>
#include "i/sws_area.h"

// FUNCTION: SWKOTOR 0x004BE3A0
int CSWSArea::ComputeBestCorner(float fX0, float fY0, float fX1, float fY1, float fX2, float fY2,
                                float fRadius, float fHeight, float* pX1, float* pY1,
                                float* pX2, float* pY2)
{
    float fDX2 = fX2 - fX1;
    float fDY2 = fY2 - fY1;
    float fLength2 = (float)sqrt(fDX2 * fDX2 + fDY2 * fDY2);
    float fDX1 = fX1 - fX0;
    float fDY1 = fY1 - fY0;
    float fLength1 = (float)sqrt(fDX1 * fDX1 + fDY1 * fDY1);
    if (fLength1 < 0.0001f || fLength2 < 0.0001f) {
        return 0;
    }
    if ((fDX1 * fDX2 + fDY1 * fDY2) / (fLength1 * fLength2) >= 0.9f) {
        return 0;
    }
    float fShortest = fLength1;
    if (fLength2 < fLength1) {
        fShortest = fLength2;
    }
    float fT = 0.5f;
    float fStep = 0.25f;
    int bFound = 0;
    do {
        float fDistance = fT * fShortest;
        Vector vFrom;
        vFrom.x = (fX0 - fX1) * (1.0f / fLength1) * fDistance + fX1;
        vFrom.y = (fY0 - fY1) * (1.0f / fLength1) * fDistance + fY1;
        vFrom.z = 0.0f;
        Vector vTo;
        vTo.x = (1.0f / fLength2) * fDistance * fDX2 + fX1;
        vTo.y = (1.0f / fLength2) * fDistance * fDY2 + fY1;
        vTo.z = 0.0f;
        if (TestDirectLine(vFrom, vTo, fRadius, fHeight, 1, NULL) == 1) {
            fT += fStep;
            *pX1 = vFrom.x;
            *pX2 = vTo.x;
            *pY1 = vFrom.y;
            *pY2 = vTo.y;
            bFound = 1;
        }
        else {
            fT -= fStep;
        }
        fStep *= 0.5f;
    } while (fStep * fShortest > 0.1f);
    return bFound;
}
