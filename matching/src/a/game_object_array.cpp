// CGameObjectArray (0x004D7D80-0x004D82A2; behavioural reference:
// src/batches/b004d7_game_object_array.cpp).
#include "a/game_object_array.h"

// FUNCTION: SWKOTOR 0x004D7D80
CGameObjectArray::CGameObjectArray(int bClientMode)
{
    unsigned long i;
    m_bClientMode = bClientMode;
    if (bClientMode) {
        m_pArray = new CGameObjectArrayNode*[0x2000];
        for (i = 0; i < 0x2000; i++) {
            m_pArray[i] = NULL;
        }
    }
    else {
        m_pArray = new CGameObjectArrayNode*[0x1000];
        for (i = 0; i < 0x1000; i++) {
            m_pArray[i] = NULL;
        }
    }
    for (int n = 0; n < 2; n++) {
        m_nNextObjectArrayID[n] = 0;
        m_nNextCharArrayID[n] = 0x7fffffff;
    }
}

// FUNCTION: SWKOTOR 0x004D7E10
CGameObjectArray::~CGameObjectArray()
{
    int nSize = m_bClientMode ? 0x2000 : 0x1000;
    for (int i = 0; i < nSize; i++) {
        while (m_pArray[i] != NULL) {
            CGameObjectArrayNode* pNode = m_pArray[i];
            if (pNode->m_objObject) {
                delete pNode->m_objObject;
            }
            if (m_pArray[i] == pNode) {
                m_pArray[i] = pNode->m_pNext;
                delete pNode;
            }
        }
    }
    delete[] m_pArray;
    m_pArray = NULL;
}

// FUNCTION: SWKOTOR 0x004D7E90
unsigned char CGameObjectArray::AddObjectAtPos(OBJECT_ID nObjectID, CGameObject* pObject)
{
    if (pObject == NULL) {
        return 4;
    }
    unsigned short bExternal = (unsigned short)((unsigned short)(nObjectID >> 31) & 1);
    OBJECT_ID nID = nObjectID & 0x7fffffff;
    int nBucket = (bExternal << 12) + (nObjectID & 0xfff);
    OBJECT_ID nNextID;
    if (nID & 0x7f000000) {
        if (nID < 0x7f000001 || nID > 0x7fffffff) {
            return 1;
        }
        if (m_nNextCharArrayID[bExternal] >= nID) {
            m_nNextCharArrayID[bExternal] = nID - 1;
        }
        nNextID = m_nNextCharArrayID[bExternal];
    }
    else {
        if (nID > 0x7effffff) {
            return 1;
        }
        if (m_nNextObjectArrayID[bExternal] <= nID) {
            m_nNextObjectArrayID[bExternal] = nID + 1;
        }
        nNextID = m_nNextObjectArrayID[bExternal];
    }

    CGameObjectArrayNode* pNode = new CGameObjectArrayNode;
    pNode->m_nObjectID = nID;
    pNode->m_objObject = pObject;
    pNode->m_pNext = NULL;

    CGameObjectArrayNode* pCur = m_pArray[nBucket];
    if (pCur == NULL) {
        m_pArray[nBucket] = pNode;
        return 0;
    }
    CGameObjectArrayNode* pPrev = NULL;
    while (pCur != NULL) {
        if (pCur->m_nObjectID <= nID) {
            break;
        }
        pPrev = pCur;
        pCur = pCur->m_pNext;
    }
    if (pCur == NULL) {
        pPrev->m_pNext = pNode;
        return 0;
    }
    if (pCur->m_nObjectID == nID) {
        CGameObject* pOld = pCur->m_objObject;
        delete pNode;
        if (bExternal) {
            nNextID |= 0x80000000;
        }
        return AddObjectAtPos(nNextID, pOld);
    }
    if (pPrev == NULL) {
        pNode->m_pNext = m_pArray[nBucket];
        m_pArray[nBucket] = pNode;
        return 0;
    }
    pNode->m_pNext = pPrev->m_pNext;
    pPrev->m_pNext = pNode;
    return 0;
}

// FUNCTION: SWKOTOR 0x004D7FF0
unsigned char CGameObjectArray::AddExternalObject(OBJECT_ID* pObjectID, CGameObject* pObject, int bUnused)
{
    if (pObject == NULL) {
        return 4;
    }
    OBJECT_ID nID = *pObjectID & 0x7fffffff;
    int nBucket = (nID & 0xfff) + 0x1000;
    if (nID & 0x7f000000) {
        if (nID < 0x7f000001) {
            return 1;
        }
        if (nID > 0x7fffffff) {
            return 1;
        }
        if (m_nNextCharArrayID[1] >= nID) {
            m_nNextCharArrayID[1] = nID - 1;
        }
    }
    else {
        if (nID > 0x7effffff) {
            return 1;
        }
        if (m_nNextObjectArrayID[1] <= nID) {
            m_nNextObjectArrayID[1] = nID + 1;
        }
    }

    CGameObjectArrayNode* pNode = new CGameObjectArrayNode;
    pNode->m_pNext = NULL;
    pNode->m_nObjectID = nID;
    pNode->m_objObject = pObject;

    if (m_pArray[nBucket] == NULL) {
        m_pArray[nBucket] = pNode;
    }
    else {
        CGameObjectArrayNode* pCur = m_pArray[nBucket];
        CGameObjectArrayNode* pPrev = NULL;
        while (pCur != NULL) {
            if (pCur->m_nObjectID <= nID) {
                break;
            }
            pPrev = pCur;
            pCur = pCur->m_pNext;
        }
        if (pCur == NULL) {
            pPrev->m_pNext = pNode;
        }
        else if (pPrev == NULL) {
            pNode->m_pNext = m_pArray[nBucket];
            m_pArray[nBucket] = pNode;
        }
        else {
            pNode->m_pNext = pPrev->m_pNext;
            pPrev->m_pNext = pNode;
        }
    }
    *pObjectID |= 0x80000000;
    return 0;
}

// FUNCTION: SWKOTOR 0x004D80E0
unsigned char CGameObjectArray::AddInternalObject(OBJECT_ID* pObjectID, CGameObject* pObject, int bCharacter)
{
    if (pObject == NULL) {
        return 4;
    }
    if (bCharacter) {
        *pObjectID = m_nNextCharArrayID[0]--;
    }
    else {
        *pObjectID = m_nNextObjectArrayID[0]++;
    }
    int nBucket = *pObjectID & 0xfff;

    CGameObjectArrayNode* pNode = new CGameObjectArrayNode;
    pNode->m_nObjectID = *pObjectID;
    pNode->m_objObject = pObject;
    pNode->m_pNext = NULL;

    CGameObjectArrayNode* pHead = m_pArray[nBucket];
    if (pHead == NULL) {
        m_pArray[nBucket] = pNode;
        return 0;
    }
    CGameObjectArrayNode* pCur = pHead;
    CGameObjectArrayNode* pPrev = NULL;
    while (pCur != NULL) {
        if (pCur->m_nObjectID <= pNode->m_nObjectID) {
            if (pPrev == NULL) {
                pNode->m_pNext = pHead;
                m_pArray[nBucket] = pNode;
                return 0;
            }
            pNode->m_pNext = pPrev->m_pNext;
            pPrev->m_pNext = pNode;
            return 0;
        }
        pPrev = pCur;
        pCur = pCur->m_pNext;
    }
    pPrev->m_pNext = pNode;
    return 0;
}

// FUNCTION: SWKOTOR 0x004D81A0 SYMBOL: ?Delete@CGameObjectArray@@QAEEKPAPAVCGameObject@@@Z
unsigned char CGameObjectArray::Delete(OBJECT_ID nObjectID, CGameObject** ppObject)
{
    OBJECT_ID nID = nObjectID & 0x7fffffff;
    int nBucket = (((unsigned short)(nObjectID >> 31) & 1) << 12) + (nObjectID & 0xfff);
    CGameObjectArrayNode* pCur = m_pArray[nBucket];
    if (pCur == NULL) {
        return 1;
    }
    CGameObjectArrayNode* pPrev = NULL;
    while (pCur != NULL) {
        if (pCur->m_nObjectID == nID) {
            *ppObject = pCur->m_objObject;
            if (pPrev == NULL) {
                m_pArray[nBucket] = pCur->m_pNext;
            }
            else {
                pPrev->m_pNext = pCur->m_pNext;
            }
            delete pCur;
            return 0;
        }
        pPrev = pCur;
        pCur = pCur->m_pNext;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8230
unsigned char CGameObjectArray::GetGameObject(OBJECT_ID nObjectID, CGameObject** ppObject)
{
    OBJECT_ID nID = nObjectID & 0x7fffffff;
    int nBucket = (((unsigned short)(nObjectID >> 31) & 1) << 12) + (nObjectID & 0xfff);
    CGameObjectArrayNode* pCur = m_pArray[nBucket];
    while (pCur != NULL) {
        if (pCur->m_nObjectID == nID) {
            break;
        }
        pCur = pCur->m_pNext;
    }
    if (pCur == NULL) {
        *ppObject = NULL;
        return 1;
    }
    *ppObject = pCur->m_objObject;
    return 0;
}

// FUNCTION: SWKOTOR 0x004D8290 SYMBOL: ?Delete@CGameObjectArray@@QAEEK@Z
unsigned char CGameObjectArray::Delete(OBJECT_ID nObjectID)
{
    CGameObject* pObject;
    return Delete(nObjectID, &pObject);
}
