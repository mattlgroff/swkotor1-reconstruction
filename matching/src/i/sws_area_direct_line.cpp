// CSWSArea::TestDirectLine: walks a straight segment room by room.
#include "i/sws_area.h"

// FUNCTION: SWKOTOR 0x004BCB70
int CSWSArea::TestDirectLine(const Vector& vFrom, const Vector& vTo, const float& fRadius,
                             float fHeight, int nUnused, CWalkHitInfo* pHit)
{
    int nResult = 1;
    int bDone = 0;
    Vector vCurrent = vFrom;
    Vector vClipStart(0.0f, 0.0f, 0.0f);
    Vector vClipEnd(0.0f, 0.0f, 0.0f);
    CSWSRoom* pRoom = GetRoom(vCurrent, NULL);
    if (g_bDebugDirectLine) {
        g_bDebugDirectLine = 0;
    }
    float fZLow = 0.0f;
    float fZHigh = 0.0f;
    int nEdge;
    unsigned long oidBlocker;
    for (;;) {
        if (bDone) {
            break;
        }
        if (pRoom == NULL) {
            return 0;
        }
        if (pRoom->ClipSegmentWalkable(&vCurrent, vTo, &vClipStart, &vClipEnd, &fZLow, &fZHigh,
                                       &nEdge) != 1) {
            nResult = -1;
            break;
        }
        fZLow = fZLow - 0.1f;
        fZHigh = fZHigh + fHeight + 0.1f;
        if (!pRoom->NoNonWalkPolysOnRoom(&vClipStart, &vClipEnd, &fZLow, &fZHigh, fRadius, pHit)) {
            nResult = -1;
            break;
        }
        float fLow = fZLow - 1000.0f;
        float fHigh = fZHigh + 1000.0f;
        if (!NoNonWalkPolysDetailed(vClipStart, vClipEnd, fLow, fHigh, fRadius,
                                    m_pPathfindInformation->m_oidIgnore, &oidBlocker, 0, 0, 0,
                                    pHit, 0)) {
            nResult = -2;
            break;
        }
        CPathfindInformation* pInfo = m_pPathfindInformation;
        if (pInfo == NULL) {
            nResult = -3;
            break;
        }
        if (!pInfo->m_bIgnoreCreatures) {
            if (!(fRadius > 0.0f)) {
                nResult = -3;
                break;
            }
            if (!NoCreaturesOnLine(vClipStart, vClipEnd, pInfo, 1, 0, NULL, 1, pHit)) {
                nResult = -3;
                break;
            }
        }
        if (vTo.x - 0.0001f <= vClipEnd.x && 0.0001f + vTo.x >= vClipEnd.x &&
            vTo.y - 0.0001f <= vClipEnd.y && 0.0001f + vTo.y >= vClipEnd.y) {
            bDone = 1;
            continue;
        }
        int nNext = pRoom->GetAdjacentRoom(nEdge);
        if (nNext == -1 || m_pRooms[nNext].m_bBlocked) {
            nResult = 0;
            break;
        }
        VectorEqual(vClipStart, vClipEnd);
        pRoom = &m_pRooms[nNext];
        vCurrent = vClipEnd;
    }
    if (g_bDebugDirectLineHeights) {
        Vector vStart = vFrom;
        Vector vEnd = vTo;
        int nFace;
        CSWSRoom* pStartRoom = GetRoom(vStart, &nFace);
        if (pStartRoom) {
            pStartRoom->ComputeHeight(vStart, 0);
        }
        CSWSRoom* pEndRoom = GetRoom(vEnd, &nFace);
        if (pEndRoom) {
            pEndRoom->ComputeHeight(vEnd, 0);
        }
    }
    return nResult;
}
