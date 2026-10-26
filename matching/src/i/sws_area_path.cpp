// CSWSArea room lookup, DFS transposition tables, height and
// area-transition helpers (0x004BB600-0x004BC57F).
#include <math.h>

#include "i/sws_area.h"

// FUNCTION: SWKOTOR 0x004BB600
CSWSRoom* CSWSArea::GetRoom(const Vector& vPosition, int* pFace)
{
    Vector vTop(vPosition.x, vPosition.y, vPosition.z + 1000.0f);
    Vector vBottom(vPosition.x, vPosition.y, vPosition.z - 1000.0f);
    CSWCollisionResult cResult(0xffffffff);
    CSWSRoom* pRoom = NULL;
    if (pFace) {
        *pFace = -1;
    }
    for (int i = 0; i < m_nRooms; i++) {
        if (m_pRooms[i].CheckAABBWalkable(vTop, vBottom, &cResult)) {
            pRoom = &m_pRooms[i];
            if (pFace) {
                *pFace = cResult.m_nFace;
            }
            break;
        }
    }
    return pRoom;
}

// FUNCTION: SWKOTOR 0x004BB700
CSWSRoom* CSWSArea::GetRoomByIndex(int nIndex)
{
    if (nIndex >= 0 && nIndex < m_nRooms) {
        return &m_pRooms[nIndex];
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004BB730
int CSWSArea::GetRoomIndex(const Vector& vPosition, int bAll)
{
    int nRoom = -1;
    Vector vTop(vPosition.x, vPosition.y, vPosition.z + 1000.0f);
    Vector vBottom(vPosition.x, vPosition.y, vPosition.z - 1000.0f);
    CSWCollisionResult cResult(0xffffffff);
    for (int i = 0; i < m_nRooms; i++) {
        int bHit;
        if (bAll) {
            bHit = m_pRooms[i].CheckAABBAll(vTop, vBottom, &cResult);
        }
        else {
            bHit = m_pRooms[i].CheckAABBWalkable(vTop, vBottom, &cResult);
        }
        if (bHit) {
            return i;
        }
    }
    return nRoom;
}

// FUNCTION: SWKOTOR 0x004BB860
float CSWSArea::ComputePathLength(int nCount, float* pPoints)
{
    float fLength = 0.0f;
    for (int i = 0; i < nCount - 2; i += 2) {
        float fY = pPoints[i + 3] - pPoints[i + 1];
        float fX = pPoints[i + 2] - pPoints[i];
        fLength += (float)sqrt(fX * fX + fY * fY);
    }
    return fLength;
}

// FUNCTION: SWKOTOR 0x004BB8B0
int CSWSArea::CountVisibleToPlayers(const Vector& vPosition, int nMax)
{
    int nCount = 0;
    unsigned long oid;
    Vector vEye;
    if (GetFirstObjectIdInArea(&oid) == 1) {
        vEye = vPosition;
        do {
            if (nCount >= nMax) {
                break;
            }
            CGameObject* pObject;
            if (GetServerExoApp()->GetObjectArray()->GetGameObject(oid, &pObject) == g_nGameObjectFound &&
                pObject->AsSWSCreature() &&
                GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
                Vector vCreature = pObject->AsSWSCreature()->m_vPosition;
                Vector vDiff = vEye - vCreature;
                if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z <= 400.0f) {
                    vEye.z += 1.5f;
                    vCreature.z += 1.5f;
                    Vector vHit(0.0f, 0.0f, 0.0f);
                    int nOut;
                    if (ClearLineOfSight(&vEye, &vCreature, &vHit, &nOut, OBJECT_INVALID,
                                         OBJECT_INVALID, 0, 0) == 1) {
                        nCount++;
                    }
                }
            }
        } while (GetNextObjectIdInArea(&oid) == 1);
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x004BBC50
void CSWSArea::GridDFSTransTableInitialize()
{
    if (m_pGridDFSTransTable == NULL) {
        m_pGridDFSTransTable = (CGridDFSTransEntry*)operator new(0x2000);
    }
    for (int i = 0; i < 0x200; i++) {
        m_pGridDFSTransTable[i].nX = 0;
        m_pGridDFSTransTable[i].nY = 0;
        m_pGridDFSTransTable[i].nCost = 0x80000000;
        m_pGridDFSTransTable[i].nDepth = 0;
    }
}

// FUNCTION: SWKOTOR 0x004BBCC0
int CSWSArea::GridDFSTransTableGet(int nX, int nY, int* pCost, int* pDepth)
{
    int nSlot = (nX * 17 + nY * 23 + 6000) % 256;
    if (m_pGridDFSTransTable[nSlot].nX == nX && m_pGridDFSTransTable[nSlot].nY == nY) {
        *pCost = m_pGridDFSTransTable[nSlot].nCost;
        *pDepth = m_pGridDFSTransTable[nSlot].nDepth;
        return 1;
    }
    nSlot += 256;
    if (m_pGridDFSTransTable[nSlot].nX == nX && m_pGridDFSTransTable[nSlot].nY == nY) {
        *pCost = m_pGridDFSTransTable[nSlot].nCost;
        *pDepth = m_pGridDFSTransTable[nSlot].nDepth;
        return 1;
    }
    *pCost = 0;
    *pDepth = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x004BBD80
void CSWSArea::GridDFSTransTablePut(int nX, int nY, int nCost, int nDepth)
{
    int nSlot = (nX * 17 + nY * 23 + 6000) % 256;
    CGridDFSTransEntry* pEntry = &m_pGridDFSTransTable[nSlot];
    if ((pEntry->nX != nX || pEntry->nY != nY) &&
        (pEntry->nCost <= nCost && (pEntry->nCost != nCost || pEntry->nDepth <= nDepth))) {
        nSlot += 256;
    }
    m_pGridDFSTransTable[nSlot].nX = nX;
    m_pGridDFSTransTable[nSlot].nY = nY;
    m_pGridDFSTransTable[nSlot].nCost = nCost;
    m_pGridDFSTransTable[nSlot].nDepth = nDepth;
}

// FUNCTION: SWKOTOR 0x004BBE20
int CSWSArea::NoNoneWalkPolysInStaticObject(unsigned long oid, float fX1, float fY1, float fX2,
                                            float fY2, float fZ, int a7, int a8, int nArg)
{
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oid);
    if (pObject == NULL) {
        return 0;
    }
    switch (pObject->m_nObjectType) {
    case 10: {
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        return pDoor->NoNonWalkPolys(fX1, fY1, &fX2, &fY2, &fZ, nArg);
    }
    case 9:
        return pObject->AsSWSPlaceable()->NoNonWalkPolys(fX1, fY1, &fX2, &fY2, &fZ, nArg);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004BBEB0
void CSWSArea::PathPointDFSTransTableInitialize()
{
    if (m_pPathPointDFSTransTable == NULL) {
        m_pPathPointDFSTransTable = (CPathPointDFSTransEntry*)operator new(0x2000);
    }
    for (int i = 0; i < 0x200; i++) {
        m_pPathPointDFSTransTable[i].fX = 0.0f;
        m_pPathPointDFSTransTable[i].fY = 0.0f;
        m_pPathPointDFSTransTable[i].fCost = 3.4028235e38f;
        m_pPathPointDFSTransTable[i].fDepth = 0.0f;
    }
}

// FUNCTION: SWKOTOR 0x004BBF20
int CSWSArea::PathPointDFSTransTableGet(float fX, float fY, float* pCost, float* pDepth)
{
    int nSlot = ((int)fX * 17 + (int)fY * 23 + 6000) % 256;
    if (m_pPathPointDFSTransTable[nSlot].fX == fX && m_pPathPointDFSTransTable[nSlot].fY == fY) {
        *pCost = m_pPathPointDFSTransTable[nSlot].fCost;
        *pDepth = m_pPathPointDFSTransTable[nSlot].fDepth;
        return 1;
    }
    nSlot += 256;
    if (m_pPathPointDFSTransTable[nSlot].fX == fX && m_pPathPointDFSTransTable[nSlot].fY == fY) {
        *pCost = m_pPathPointDFSTransTable[nSlot].fCost;
        *pDepth = m_pPathPointDFSTransTable[nSlot].fDepth;
        return 1;
    }
    *pCost = 0;
    *pDepth = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x004BC010
void CSWSArea::PathPointDFSTransTablePut(float fX, float fY, float fCost, float fDepth)
{
    int nSlot = ((int)fX * 17 + (int)fY * 23 + 6000) % 256;
    if (m_pPathPointDFSTransTable[nSlot].fX != fX || m_pPathPointDFSTransTable[nSlot].fY != fY) {
        if (!(m_pPathPointDFSTransTable[nSlot].fCost > fCost)) {
            if (m_pPathPointDFSTransTable[nSlot].fCost != fCost || !(m_pPathPointDFSTransTable[nSlot].fDepth > fDepth)) {
                nSlot += 256;
            }
        }
    }
    m_pPathPointDFSTransTable[nSlot].fX = fX;
    m_pPathPointDFSTransTable[nSlot].fY = fY;
    m_pPathPointDFSTransTable[nSlot].fCost = fCost;
    m_pPathPointDFSTransTable[nSlot].fDepth = fDepth;
}

// FUNCTION: SWKOTOR 0x004BC380
float CSWSArea::ComputeHeight(const Vector& vPosition, int bUseFace, CSWSRoom** ppRoom)
{
    float fHeight = 0.0f;
    int nFace;
    CSWSRoom* pRoom = GetRoom(vPosition, &nFace);
    if (ppRoom) {
        *ppRoom = pRoom;
    }
    if (pRoom) {
        if (bUseFace) {
            fHeight = pRoom->ComputeHeightSecondary(nFace, vPosition);
        }
        else {
            fHeight = pRoom->ComputeHeight(vPosition, 0);
        }
    }
    return fHeight;
}

// FUNCTION: SWKOTOR 0x004BC400
int CSWSArea::CountAreaTransitionTriggers(unsigned long* pList, int nExcluded,
                                          unsigned long* pExcluded)
{
    int nCount = 0;
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    int nObjects = m_aGameObjects.num;
    CGameObject* pObject;
    CGameObject* pTarget;
    for (int i = 0; i < nObjects; i++) {
        unsigned long oid = m_aGameObjects.element[i];
        if (pArray->GetGameObject(oid, &pObject) == g_nGameObjectFound) {
            if (pObject->m_nObjectType == g_nObjectTypeTrigger) {
                CSWSTrigger* pTrigger = pObject->AsSWSTrigger();
                if (pTrigger->m_bAreaTransition == 1) {
                            if (pArray->GetGameObject(pTrigger->GetTargetArea(), &pTarget) == g_nGameObjectFound) {
                        CSWSArea* pArea = pTarget->AsSWSArea();
                        if (pArea && !pArea->m_bTransitionCounted) {
                            int bListed = 0;
                            for (int j = 0; j < nExcluded; j++) {
                                if (pExcluded[j] == oid) {
                                    bListed = 1;
                                }
                            }
                            if (!bListed) {
                                if (pList) {
                                    pList[nCount] = oid;
                                    pArea->m_bTransitionCounted = 1;
                                }
                                nCount++;
                            }
                        }
                    }
                }
            }
            else if (pObject->m_nObjectType == 10) {
                CSWSDoor* pDoor = pObject->AsSWSDoor();
                if (pDoor->GetIsLinkedToDoorOrTrigger()) {
                    CSWSObject* pLinked = pDoor->GetLinkedObject();
                    if (pLinked) {
                        CSWSArea* pArea = pLinked->GetArea();
                        if (pArea && !pArea->m_bTransitionCounted) {
                            int bListed = 0;
                            for (int j = 0; j < nExcluded; j++) {
                                if (pExcluded[j] == oid) {
                                    bListed = 1;
                                }
                            }
                            if (!bListed) {
                                if (pList) {
                                    pList[nCount] = oid;
                                    pArea->m_bTransitionCounted = 1;
                                }
                                nCount++;
                            }
                        }
                    }
                }
            }
        }
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x004BC0E0
int CSWSArea::SetHitInfoForCreature(const Vector& vCenter, float fRadius, const Vector& vFrom, const Vector& vTo,
                                    CWalkHitInfo* pHit)
{
    if (pHit == NULL) {
        return 1;
    }
    const Vector* pFrom = &vFrom;
    const Vector* pTo = &vTo;
    const Vector* pCenter = &vCenter;
    Vector vHit1(0.0f, 0.0f, 0.0f);
    Vector vHit2(0.0f, 0.0f, 0.0f);
    int nHits = IntersectCircle2d(*pCenter, fRadius, *pFrom, *pTo, &vHit1, &vHit2, 1);
    if (nHits == 2) {
        float fDist1 = (vHit1 - *pFrom).MagnitudeSquared();
        if ((vHit2 - *pFrom).MagnitudeSquared() < fDist1) {
            Vector vDir = NormalizeVector(vHit2 - *pCenter);
            Vector vPerp(-vDir.y, vDir.x, vDir.z);
            Vector vLeft = vHit2 + vPerp;
            vHit1 = vHit2 - vPerp;
            pHit->Initialize(1, vHit2, vDir, vHit1, vLeft, 1, vDir);
        }
        else {
            Vector vDir = NormalizeVector(vHit1 - *pCenter);
            Vector vPerp(-vDir.y, vDir.x, vDir.z);
            Vector vLeft = vHit1 + vPerp;
            vHit2 = vHit1 - vPerp;
            pHit->Initialize(1, vHit1, vDir, vHit2, vLeft, 1, vDir);
        }
    }
    else if (nHits == 1) {
        Vector vDir = NormalizeVector(vHit1 - *pCenter);
        Vector vPerp(-vDir.y, vDir.x, vDir.z);
        Vector vLeft = vHit1 + vPerp;
        vHit2 = vHit1 - vPerp;
        pHit->Initialize(1, vHit1, vDir, vHit2, vLeft, 1, vDir);
    }
    return nHits;
}

// FUNCTION: SWKOTOR 0x004BBA40
int CSWSArea::EvaluateOverlappingTargets(CPathfindInformation* pInfo, Vector* pA,
                                         unsigned long oidTarget, Vector* pB, float fRange,
                                         int a6, int bCheck, unsigned long* pOut)
{
    if (g_bDebugOverlap) {
        CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
        CSWSCreature* pSelf = GetServerExoApp()->GetCreatureByGameObjectID(pInfo->m_oidCreature);
        Vector vTarget = pTarget->m_vPosition;
        Vector vSelf = pSelf->m_vPosition;
        Vector vDirection = NormalizeVector(vTarget - vSelf);
        vDirection = NormalizeVector(vSelf - vTarget);
        pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    }
    float fDistSq = (*pA - *pB).MagnitudeSquared();
    if (fDistSq <= fRange * fRange) {
        return 0;
    }
    if (pOut) {
        *pOut = oidTarget;
    }
    if (bCheck == 1 && pInfo) {
        CSWSCreature* pOther = GetServerExoApp()->GetCreatureByGameObjectID(pInfo->m_oidTarget);
        CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
        if (pOther && pTarget) {
            Vector vDiff = VectorSubtract(pOther->m_vPosition, pTarget->m_vPosition);
            float fSum = pTarget->m_pPathfindInformation->m_fPersonalSpace +
                         pOther->m_pPathfindInformation->m_fPersonalSpace;
            if (vDiff.x * vDiff.x + vDiff.y * vDiff.y < fSum * fSum) {
                return 1;
            }
        }
    }
    return 0;
}
