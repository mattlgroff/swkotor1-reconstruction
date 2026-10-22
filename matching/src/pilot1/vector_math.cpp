// Pilot group 1: vector math. BEST ATTEMPT (not matching): identical except
// pMin/pMax get ebx/ebp swapped (forces disp8 on [ebp], +2 bytes per block)
// and the z-high block schedules fxch differently.

struct Vector {
    float x;
    float y;
    float z;
};

void Outcode(Vector* pPoint, Vector* pMin, Vector* pMax, unsigned char* pCode);

// FUNCTION: SWKOTOR 0x004AA1F0
int BoxIntersec(Vector* pStart, Vector* pEnd, Vector* pMin, Vector* pMax, Vector* pOut)
{
    unsigned char nCode0;
    unsigned char nCode1;

    Outcode(pStart, pMin, pMax, &nCode0);
    if (!nCode0) {
        return -1;
    }
    Outcode(pEnd, pMin, pMax, &nCode1);
    if (nCode0 & nCode1) {
        return 0;
    }

    if (nCode0 & 0x20) {
        float t = (pMin->x - pStart->x) / (pEnd->x - pStart->x);
        float s = 1.0f - t;
        float a = s * pStart->y + t * pEnd->y;
        float b = s * pStart->z + t * pEnd->z;
        if (a >= pMin->y && a <= pMax->y && b >= pMin->z && b <= pMax->z) {
            pOut->x = pMin->x;
            pOut->y = a;
            pOut->z = b;
            return 1;
        }
    }
    else if (nCode0 & 0x10) {
        float t = (pMax->x - pStart->x) / (pEnd->x - pStart->x);
        float s = 1.0f - t;
        float a = s * pStart->y + t * pEnd->y;
        float b = s * pStart->z + t * pEnd->z;
        if (a >= pMin->y && a <= pMax->y && b >= pMin->z && b <= pMax->z) {
            pOut->x = pMax->x;
            pOut->y = a;
            pOut->z = b;
            return 2;
        }
    }

    if (nCode0 & 0x08) {
        float t = (pMin->y - pStart->y) / (pEnd->y - pStart->y);
        float s = 1.0f - t;
        float a = s * pStart->x + t * pEnd->x;
        float b = s * pStart->z + t * pEnd->z;
        if (a >= pMin->x && a <= pMax->x && b >= pMin->z && b <= pMax->z) {
            pOut->x = a;
            pOut->y = pMin->y;
            pOut->z = b;
            return 3;
        }
    }
    else if (nCode0 & 0x04) {
        float t = (pMax->y - pStart->y) / (pEnd->y - pStart->y);
        float s = 1.0f - t;
        float a = s * pStart->x + t * pEnd->x;
        float b = s * pStart->z + t * pEnd->z;
        if (a >= pMin->x && a <= pMax->x && b >= pMin->z && b <= pMax->z) {
            pOut->x = a;
            pOut->y = pMax->y;
            pOut->z = b;
            return 4;
        }
    }

    if (nCode0 & 0x02) {
        float t = (pMin->z - pStart->z) / (pEnd->z - pStart->z);
        float s = 1.0f - t;
        float a = s * pStart->x + t * pEnd->x;
        float b = s * pStart->y + t * pEnd->y;
        if (b >= pMin->y && b <= pMax->y && a >= pMin->x && a <= pMax->x) {
            pOut->x = a;
            pOut->y = b;
            pOut->z = pMin->z;
            return 5;
        }
    }
    else if (nCode0 & 0x01) {
        float t = (pMax->z - pStart->z) / (pEnd->z - pStart->z);
        float s = 1.0f - t;
        float a = s * pStart->x + t * pEnd->x;
        float b = t * pEnd->y + s * pStart->y;
        if (b >= pMin->y && b <= pMax->y && a >= pMin->x && a <= pMax->x) {
            pOut->x = a;
            pOut->y = b;
            pOut->z = pMax->z;
            return 6;
        }
    }
    return 0;
}
