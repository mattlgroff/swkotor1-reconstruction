// CSWSArea::ClearLineOfSight (0x0050C330): static objects first (ignoring
// the source and target objects), then the nearest room walkmesh hit.
#include "a/sws_area.h"

class CWalkHitInfo {
public:
    CWalkHitInfo()
        : m_vHit(0.0f, 0.0f, 0.0f), m_v10(0.0f, 0.0f, 0.0f), m_v1c(0.0f, 0.0f, 0.0f),
          m_v28(0.0f, 0.0f, 0.0f), m_v34(0.0f, 0.0f, 0.0f)
    {
        m_nType = 0;
        m_n44 = 0;
        m_oidHit = 0x7f000000;
    }

    int m_nType;                                    // 0x00
    Vector m_vHit;                                  // 0x04
    Vector m_v10;                                   // 0x10
    Vector m_v1c;                                   // 0x1c
    Vector m_v28;                                   // 0x28
    Vector m_v34;                                   // 0x34
    unsigned long m_oidHit;                         // 0x40
    int m_n44;                                      // 0x44
};

class CSWSRoom {
public:
    virtual ~CSWSRoom();                            // 0x005794B0
    int ClearLineOfSight(Vector vFrom, Vector vTo, Vector* pHit,
                         unsigned long* pMask);     // 0x005797A0

    char pad_0x4[0x4c - 0x4];
};

// FUNCTION: SWKOTOR 0x0050C330
int CSWSArea::ClearLineOfSight(Vector* pFrom, Vector* pTo, Vector* pHit, unsigned long* pBlocker,
                               unsigned long oidSource, unsigned long oidTarget, int bArg7,
                               int bArg8)
{
    Vector vRoomHit(0.0f, 0.0f, 0.0f);
    unsigned long nMask = 0x7f000000;
    float fBest = 3.4028235e38f;
    int bResult = 1;
    float fZMin, fZMax;
    if (pFrom->z < pTo->z) {
        fZMin = pFrom->z;
        fZMax = pTo->z;
    } else {
        fZMin = pTo->z;
        fZMax = pFrom->z;
    }
    CExoArrayList<CGameObject*> aExclude;
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oidSource);
    if (pObject && pObject->AsSWSObject()) {
        aExclude.AddInline(pObject);
    }
    pObject = GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSObject()) {
        aExclude.AddInline(pObject);
    }
    CWalkHitInfo cHitInfo;
    int bOpen = NoNonWalkPolysDetailed(*pFrom, *pTo, fZMin, fZMax, 0.001f, 0x7f000000, *pBlocker, 1,
                                       &aExclude, bArg7, &cHitInfo, 0);
    if (!bOpen) {
        *pHit = cHitInfo.m_vHit;
        bResult = 0;
    }
    if (bOpen || bArg8) {
        for (int i = 0; i < m_nRooms; i++) {
            if (m_pRooms[i].ClearLineOfSight(*pFrom, *pTo, &vRoomHit, &nMask)) {
                continue;
            }
            float fDistance = (vRoomHit - *pFrom).MagnitudeSquared();
            if (fDistance < fBest) {
                fBest = fDistance;
                *pHit = vRoomHit;
                bResult = 0;
            }
        }
    }
    return bResult;
}
