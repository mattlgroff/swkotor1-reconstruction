// CSWSArea::HandleTransparentDoors (0x0050C1E0, vtable slot 0x18): a door
// that blocked the walk test but does not block sight is added to the
// exclusion list and the detailed test is repeated.
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x0050C1E0
int CSWSArea::HandleTransparentDoors(const Vector& vStart, const Vector& vEnd, float fRadius,
                                     float fHeight, float fZ, unsigned long oidIgnore,
                                     unsigned long& nHit, int bArg8,
                                     CExoArrayList<CGameObject*>* pExclude, int bArg10)
{
    if (nHit != 0x7f000000) {
        CSWSDoor* pDoor = GetServerExoApp()->GetDoorByGameObjectID(nHit);
        if (pDoor) {
            int bBlockSight;
            if (pDoor->m_nAppearanceType == 0) {
                CExoString sColumn("BlockSight");
                g_pRules->Get2DArrays()->GetGenericDoors()->GetINTEntry(
                    pDoor->GetGenericType(), sColumn, &bBlockSight);
            } else {
                CExoString sColumn("BlockSight");
                g_pRules->Get2DArrays()->GetDoorTypes()->GetINTEntry(
                    pDoor->GetAppearanceType(), sColumn, &bBlockSight);
            }
            if (!bBlockSight && pExclude) {
                pExclude->Add((CGameObject*)pDoor->AsSWSObject());
                return NoNonWalkPolysDetailed(vStart, vEnd, fRadius, fHeight, fZ, oidIgnore,
                                              nHit, bArg8, pExclude, 0, NULL, 0);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0050CA10
int CSWSArea::NoNonWalkPolysInDoors(const Vector& vStart, const Vector& vEnd, float fZMin,
                                    float fZMax, float fWidth, unsigned long oidIgnore,
                                    unsigned long& nHit, CExoArrayList<CGameObject*>* pExclude,
                                    int bArg9, int bHandleDoors)
{
    float fStartMaxX, fStartMinX, fStartMaxY, fStartMinY;
    float fMinX, fMinY;
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
        int bAllocated = 0;
        for (int i = 0; i < m_nDoorBoxes; i++) {
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
            unsigned long oidObject = m_pDoorBoxIds[i];
            CGameObject* pObject = GetServerExoApp()->GetGameObject(oidObject);
            if (pObject == NULL || pObject->AsSWSDoor() == NULL) {
                continue;
            }
            if (ListContainsObject(oidObject, pExclude)) {
                continue;
            }
            if (NoNoneWalkPolysInStaticObject(m_pDoorBoxIds[i], vStart, vEnd, fZMin, fZMax, fWidth,
                                              oidIgnore, bArg9, NULL)) {
                continue;
            }
            nHit = m_pDoorBoxIds[i];
            if (bHandleDoors != 1) {
                return 0;
            }
            if (pExclude == NULL) {
                pExclude = new CExoArrayList<CGameObject*>;
                bAllocated = 1;
            }
            int bResult = HandleTransparentDoors(vStart, vEnd, fZMin, fZMax, fWidth, oidIgnore, nHit,
                                                 bArg9, pExclude, 1);
            if (pExclude && bAllocated) {
                delete pExclude;
            }
            return bResult;
        }
    }
    return 1;
}
