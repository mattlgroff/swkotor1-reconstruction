// CSWSArea::LineSegmentsIntersect (in the CSWArea module, between the CSWArea
// destructor and the fog / night / wind setters).

#include <math.h>

class CSWSArea {
public:
    int LineSegmentsIntersect(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3,
                              float fX4, float fY4, float *pX, float *pY);
};

static inline float Length2D(float a, float b)
{
    return (float)sqrt(a * a + b * b);
}

// The deltas are written out (no named locals): cl CSEs them, keeps the first
// pair on the FPU stack and copies the second into the helper's parameters.
// FUNCTION: SWKOTOR 0x0058F840
int CSWSArea::LineSegmentsIntersect(float fX1, float fY1, float fX2, float fY2, float fX3,
                                    float fY3, float fX4, float fY4, float *pX, float *pY)
{
    float fCos = ((fX2 - fX1) * (fX4 - fX3) + (fY2 - fY1) * (fY4 - fY3)) / Length2D(fY2 - fY1, fX2 - fX1);
    fCos = fCos / Length2D(fY4 - fY3, fX4 - fX3);
    if (fCos == 1.0)
        return 0;

    int bIntersect = 0;
    if (fX1 == fX2) {
        float fM2 = (fY4 - fY3) / (fX4 - fX3);
        *pX = fX1;
        float fB2 = fY3 - fM2 * fX3;
        float fY = fM2 * fX1 + fB2;
        *pY = fY;
        if (fY4 >= fY3 && fY >= fY3 && fY <= fY4)
            bIntersect = 1;
        if (fY3 >= fY4 && fY >= fY4 && fY <= fY3)
            bIntersect = 1;
        if (bIntersect != 1)
            return bIntersect;
        if (fX4 >= fX3 && !(*pX >= fX3 && *pX <= fX4))
            bIntersect = 0;
        if (fX3 >= fX4 && !(*pX >= fX4 && *pX <= fX3))
            bIntersect = 0;
        if (bIntersect != 1)
            return bIntersect;
        if (fY2 >= fY1 && !(fY >= fY1 && fY <= fY2))
            bIntersect = 0;
        if (fY1 >= fY2 && !(fY >= fY2 && fY <= fY1))
            bIntersect = 0;
        return bIntersect;
    }

    if (fX3 == fX4) {
        float fM1 = (fY2 - fY1) / (fX2 - fX1);
        *pX = fX3;
        float fB1 = fY1 - fM1 * fX1;
        float fY = fM1 * fX3 + fB1;
        *pY = fY;
        if (fY2 >= fY1 && fY >= fY1 && fY <= fY2)
            bIntersect = 1;
        if (fY1 >= fY2 && fY >= fY2 && fY <= fY1)
            bIntersect = 1;
        if (bIntersect != 1)
            return bIntersect;
        if (fX2 >= fX1 && !(*pX >= fX1 && *pX <= fX2))
            bIntersect = 0;
        if (fX1 >= fX2 && !(*pX >= fX2 && *pX <= fX1))
            bIntersect = 0;
        if (bIntersect != 1)
            return bIntersect;
        if (fY4 >= fY3 && !(fY >= fY3 && fY <= fY4))
            bIntersect = 0;
        if (fY3 >= fY4 && !(fY >= fY4 && fY <= fY3))
            bIntersect = 0;
        return bIntersect;
    }

    float fM1 = (fY2 - fY1) / (fX2 - fX1);
    float fM2 = (fY4 - fY3) / (fX4 - fX3);
    if (fM2 - fM1 == 0.0)
        return 0;
    float fB2 = fY3 - fM2 * fX3;
    float fB1 = fY1 - fM1 * fX1;
    float fX = (fB2 - fB1) / (fM1 - fM2);
    *pX = fX;
    *pY = fM2 * fX + fB2;
    if (fX4 >= fX3 && *pX >= fX3 && *pX <= fX4)
        bIntersect = 1;
    if (fX3 >= fX4 && *pX >= fX4 && *pX <= fX3)
        bIntersect = 1;
    if (bIntersect != 1)
        return bIntersect;
    if (fX2 >= fX1 && !(*pX >= fX1 && *pX <= fX2))
        bIntersect = 0;
    if (fX1 >= fX2 && !(*pX >= fX2 && *pX <= fX1))
        bIntersect = 0;
    return bIntersect;
}
