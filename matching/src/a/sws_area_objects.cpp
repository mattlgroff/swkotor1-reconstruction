// CSWSArea room arrays and the x-sorted game-object array
// (0x00506910, 0x00506F20-0x005071AF).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00506910
void CSWSArea::AllocateRoomArrays(int nRooms)
{
    if (m_pRoomNames != NULL) {
        delete[] m_pRoomNames;
        m_pRoomNames = NULL;
    }
    if (m_pRoomData260 != NULL) {
        delete[] m_pRoomData260;
        m_pRoomData260 = NULL;
    }
    if (m_pRoomData264 != NULL) {
        delete[] m_pRoomData264;
        m_pRoomData264 = NULL;
    }
    if (nRooms != 0) {
        m_pRoomNames = new CExoString[nRooms];
        m_pRoomData260 = new unsigned long[nRooms];
        m_pRoomData264 = new unsigned long[nRooms];
    }
}

// FUNCTION: SWKOTOR 0x00506F20
int CSWSArea::GetFirstObjectIndiceByX(int* pIndex, float fX)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    int nLast = m_aGameObjects.num - 1;
    int nLow = 0;
    int nHigh = nLast;
    while (nLow < nHigh || nLow == nLast) {
        int nMid = (nLow + nHigh) >> 1;
        CGameObject* pObject;
        if (pArray->GetGameObject(m_aGameObjects.element[nMid], &pObject) != g_nGameObjectFound) {
            return 0;
        }
        if (((CSWSObject*)pObject)->m_vPosition.x >= fX) {
            if (nMid == nHigh) {
                *pIndex = nMid;
                return 1;
            }
            nHigh = nMid;
        }
        else {
            nLow = nMid + 1;
        }
    }
    if (nLow == nHigh) {
        *pIndex = nLow;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00506FE0
int CSWSArea::UpdatePositionInObjectsArray(CGameObject* pObject)
{
    int nIndex = m_aGameObjects.IndexOf(pObject->m_idSelf);
    int nNum = m_aGameObjects.num;
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    float fX = ((CSWSObject*)pObject)->m_vPosition.x;
    CGameObject* pOther = pObject;
    unsigned long nTemp;

    if (nIndex + 1 != nNum &&
        (pArray->GetGameObject((nIndex + 1)[m_aGameObjects.element], &pOther) != g_nGameObjectFound ||
         !(((CSWSObject*)pOther)->m_vPosition.x >= fX))) {
        while (nIndex + 1 != nNum) {
            if (pArray->GetGameObject((nIndex + 1)[m_aGameObjects.element], &pOther) != g_nGameObjectFound ||
                !(((CSWSObject*)pOther)->m_vPosition.x < fX)) {
                return 1;
            }
            nTemp = (nIndex + 1)[m_aGameObjects.element];
            (nIndex + 1)[m_aGameObjects.element] = m_aGameObjects.element[nIndex];
            m_aGameObjects.element[nIndex] = nTemp;
            nIndex++;
        }
    }
    else if (nIndex != 0 &&
             (pArray->GetGameObject(m_aGameObjects.element[nIndex - 1], &pOther) != g_nGameObjectFound ||
              !(((CSWSObject*)pOther)->m_vPosition.x <= fX))) {
        while (nIndex != 0) {
            if (pArray->GetGameObject(m_aGameObjects.element[nIndex - 1], &pOther) != g_nGameObjectFound ||
                !(((CSWSObject*)pOther)->m_vPosition.x > fX)) {
                return 1;
            }
            nTemp = m_aGameObjects.element[nIndex - 1];
            m_aGameObjects.element[nIndex - 1] = m_aGameObjects.element[nIndex];
            m_aGameObjects.element[nIndex] = nTemp;
            nIndex--;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005086D0
int CSWSArea::NoNonWalkPolys(const Vector& vStart, const Vector& vEnd, float fRadius, float fHeight,
                             float fZ, unsigned long oidIgnore, CWalkHitInfo* pHitInfo, int bArg8)
{
    unsigned long nHit;
    return NoNonWalkPolysDetailed(vStart, vEnd, fRadius, fHeight, fZ, oidIgnore, nHit, 0, NULL, 0,
                                  pHitInfo, bArg8);
}

// CExoArrayList<T>::Add is the header inline in this translation unit; the
// shared declaration stays out of line for the folded call sites elsewhere.
template <>
inline void CExoArrayList<unsigned long>::Add(unsigned long t)
{
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num++] = t;
}

// FUNCTION: SWKOTOR 0x0050DFD0
int CSWSArea::AddObjectToArea(unsigned long oidObject, int bRunScripts)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CGameObject* pObject;
    if (pArray->GetGameObject(oidObject, &pObject) != g_nGameObjectFound) {
        return 0;
    }
    if (m_aGameObjects.Contains(oidObject)) {
        RemoveObjectFromArea(oidObject);
    }
    float fX = ((CSWSObject*)pObject)->m_vPosition.x;
    int nIndex;
    if (GetFirstObjectIndiceByX(&nIndex, fX)) {
        m_aGameObjects.Insert(oidObject, nIndex);
    }
    else {
        m_aGameObjects.Add(oidObject);
    }
    CGameObject* pCreature;
    if (pArray->GetGameObject(oidObject, &pCreature) == g_nGameObjectFound && pCreature != NULL &&
        pCreature->AsSWSCreature()) {
        if (GetServerExoApp()->GetClientObjectByObjectID(oidObject)) {
            IncrementPlayersInArea();
        }
        if (bRunScripts == 1) {
            CScriptEvent* pEvent = new CScriptEvent;
            pEvent->m_nType = 0xc;
            pEvent->SetInteger(0, GetServerExoApp()->GetLoadFromSaveGame());
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidObject, m_idSelf,
                                                                      EVENT_0x7441DC, pEvent);
        }
    }
    return 1;
}
