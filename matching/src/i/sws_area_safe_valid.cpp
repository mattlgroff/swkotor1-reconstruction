// CSWSArea safe-position validation: walk the rooms across a small box
// around the position, then check walls and creatures.
#include "i/sws_area.h"

// FUNCTION: SWKOTOR 0x004BE5E0
int CSWSArea::ValidateSafePosition(const Vector& vPosition, CPathfindInformation* pInfo)
{
    m_pPathfindInformation = pInfo;
    float fRadius = pInfo->m_fRadius;
    int bDone = 0;
    Vector vStart;
    vStart.x = vPosition.x - 0.01f;
    vStart.y = vPosition.y - 0.01f;
    vStart.z = vPosition.z - 0.1f;
    Vector vEnd;
    vEnd.x = vPosition.x + 0.01f;
    vEnd.y = vPosition.y + 0.01f;
    vEnd.z = vPosition.z + 0.1f;
    float fZLow = vPosition.z - 0.1f;
    float fZHigh = pInfo->m_fHeight + fZLow + 0.1f;
    CSWSRoom* pRoom = GetRoom(vStart, NULL);
    if (pRoom == NULL) {
        return 0;
    }
    do {
        Vector vClipStart(0.0f, 0.0f, 0.0f);
        Vector vClipEnd(0.0f, 0.0f, 0.0f);
        int nEdge;
        pRoom->m_pWalkMesh->ClipSegmentToRoom(vStart, vEnd, &vClipStart, &vClipEnd, &nEdge);
        if (!pRoom->NoNonWalkPolysOnRoom(&vClipStart, &vClipEnd, &fZLow, &fZHigh, fRadius, NULL)) {
            return 0;
        }
        if (vClipEnd.x >= vEnd.x - 0.0001f && vClipEnd.x <= vEnd.x + 0.0001f &&
            vClipEnd.y >= vEnd.y - 0.0001f && vClipEnd.y <= vEnd.y + 0.0001f) {
            break;
        }
        int nRoom = pRoom->GetAdjacentRoom(nEdge);
        if (nRoom == -1) {
            return 0;
        }
        pRoom = &m_pRooms[nRoom];
        if (pRoom->m_bBlocked) {
            return 0;
        }
        if (vClipStart == vClipEnd) {
            bDone = 1;
        }
        vStart = vClipEnd;
    } while (!bDone);
    if (!NoNonWalkPolys(vStart, vEnd, fZLow, fZHigh, fRadius, pInfo->m_oidIgnore, NULL, 0)) {
        return 0;
    }
    return NoCreaturesOnLine(vStart, vEnd, pInfo, 1, 0, NULL, 0, NULL) != 0;
}
