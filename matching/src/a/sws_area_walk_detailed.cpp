// CSWSArea::NoNonWalkPolysDetailed (0x00506650): tests the segment against
// the static object boxes, asking each overlapping object whether its
// non-walkable faces block the path; doors are handed to the virtual
// HandleTransparentDoors when requested.
#include "a/sws_area.h"

class CWalkHitInfo {
public:
    char pad_0x0[0x40];
    unsigned long m_oidHit;                         // 0x40 (placeholder name)
};

int operator==(const Vector& a, const Vector& b);   // 0x004AA980

// FUNCTION: SWKOTOR 0x00506650
int CSWSArea::NoNonWalkPolysDetailed(const Vector& vStart, const Vector& vEnd, float fZMin,
                                     float fZMax, float fWidth, unsigned long oidIgnore,
                                     unsigned long& nHit, int bArg8,
                                     CExoArrayList<CGameObject*>* pExclude, int bHandleDoors,
                                     CWalkHitInfo* pHitInfo, int bDoorsOnly)
{
    int bResult = 1;
    float fStartMaxX, fStartMinX, fStartMaxY, fStartMinY;
    float fMinX, fMinY;
    vEnd == vStart;
    if (m_nDoorBoxes > 0) {
        if (vStart.x < vEnd.x) {
            fStartMinX = vStart.x;
            fStartMaxX = vEnd.x;
        } else {
            fStartMinX = vEnd.x;
            fStartMaxX = vStart.x;
        }
        if (vStart.y < vEnd.y) {
            fStartMinY = vStart.y;
            fStartMaxY = vEnd.y;
        } else {
            fStartMinY = vEnd.y;
            fStartMaxY = vStart.y;
        }
        fMinX = fStartMinX - fWidth;
        fMinY = fStartMinY - fWidth;
        fStartMaxX = fStartMaxX + fWidth;
        fStartMaxY = fStartMaxY + fWidth;
        for (int i = 0; i < m_nDoorBoxes; i++) {
            if (bDoorsOnly) {
                CGameObject* pObject = GetServerExoApp()->GetGameObject(m_pDoorBoxIds[i]);
                if (pObject) {
                    CSWSDoor* pDoor = pObject->AsSWSDoor();
                    if (pDoor == NULL || pDoor->m_bOpen) {
                        continue;
                    }
                }
            }
            Vector vMax = m_pDoorBoxes[i].vMax;
            Vector vMin = m_pDoorBoxes[i].vMin;
            if (fMinX < vMin.x && fStartMaxX < vMin.x) {
                continue;
            }
            if (fStartMaxX > vMax.x && fMinX > vMax.x) {
                continue;
            }
            if (fMinY < vMin.y && fStartMaxY < vMin.y) {
                continue;
            }
            if (fStartMaxY > vMax.y && fMinY > vMax.y) {
                continue;
            }
            if (fZMin < vMin.z && fZMax < vMin.z) {
                continue;
            }
            if (fZMax > vMax.z && fZMin > vMax.z) {
                continue;
            }
            if (ListContainsObject(m_pDoorBoxIds[i], pExclude)) {
                continue;
            }
            if (NoNoneWalkPolysInStaticObject(m_pDoorBoxIds[i], vStart, vEnd, fZMin, fZMax, fWidth,
                                              oidIgnore, bArg8, pHitInfo)) {
                continue;
            }
            nHit = m_pDoorBoxIds[i];
            if (pHitInfo) {
                pHitInfo->m_oidHit = nHit;
            }
            if (bHandleDoors == 1) {
                bResult &= HandleTransparentDoors(vStart, vEnd, fZMin, fZMax, fWidth, oidIgnore, nHit,
                                                  bArg8, pExclude, 1);
            } else {
                bResult = 0;
            }
        }
    }
    return bResult;
}
