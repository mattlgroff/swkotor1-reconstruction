// CSWCCreature movement speed scale, worker C s8 (translation-unit-local
// layout).

#include <math.h>

struct Vector {
    float x, y, z;
};

float Magnitude(const Vector& v);                   // 0x004AB100

class CSWCCreature {
public:
    float GetMovementScale(float fRate);

    void* m_pVtable;                                // 0x00
    char pad_0x4[0x44 - 0x4];
    unsigned long m_nDeltaTime;                     // 0x44
    char pad_0x48[0x1bc - 0x48];
    Vector* m_pWayPoints;                           // 0x1bc
    Vector* m_pCurWayPoint;                         // 0x1c0
    Vector m_vDesiredDirection;                     // 0x1c4
    Vector m_vDesiredOrientation;                   // 0x1d0
    Vector m_vDesiredPosition;                      // 0x1dc
    char pad_0x1e8[0x4];
    unsigned short m_nWayPoints;                    // 0x1ec
    char pad_0x1ee[0x1f4 - 0x1ee];
    int m_bWalking;                                 // 0x1f4 (descriptive)
    float m_fMovementScale;                         // 0x1f8 (descriptive)
};

// Descriptive name: speed scale for the remaining waypoint path. Slows the
// creature down when the rest of the path is short (square-root falloff) and
// ramps the previous scale up over 1 s (0.5 s when walking); never below 0.1.
// NONMATCH (4 diff insns): x87 scheduling; the original issues the
// fadd of the path length right after the Magnitude call, we hoist the
// waypoint-count load above it (same after six loop shapes).
// FUNCTION: SWKOTOR 0x0060BF60
float CSWCCreature::GetMovementScale(float fRate)
{
    Vector* pWayPoint = m_pCurWayPoint;
    float fRampTime = 1000.0f;
    if (!pWayPoint) {
        return 0.0f;
    }
    if (m_bWalking == 1) {
        fRampTime = 500.0f;
    }
    float fDistance = 0.0f;
    Vector vPrevious = m_vDesiredPosition;
    do {
        Vector vNext = *pWayPoint;
        Vector vDiff;
        vDiff.x = vNext.x - vPrevious.x;
        vDiff.y = vNext.y - vPrevious.y;
        vDiff.z = 0.0f;
        fDistance = Magnitude(vDiff) + fDistance;
        pWayPoint++;
        vPrevious = vNext;
    } while (pWayPoint < m_pWayPoints + m_nWayPoints);
    float fStopDistance = fRate * 1000.0f;
    fStopDistance *= 0.0000005f;
    float fScale = 1.0f;
    if (fDistance <= fStopDistance) {
        float fRatio = fDistance / fStopDistance;
        if (fRatio >= 0.01f) {
            fRatio = (float)sqrt(fRatio);
        } else {
            fRatio = 0.1f;
        }
        fScale = fRatio - (float)m_nDeltaTime * 0.001f;
    }
    if (m_fMovementScale <= 1.0f) {
        float fRamp = (float)m_nDeltaTime / fRampTime + m_fMovementScale;
        if (fScale >= fRamp) {
            fScale = fRamp;
        }
    }
    if (fScale <= 0.1f) {
        fScale = 0.1f;
    }
    return fScale;
}
