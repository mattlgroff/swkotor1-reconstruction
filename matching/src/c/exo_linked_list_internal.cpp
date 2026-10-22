// CExoLinkedListInternal position leaves (b005e5_exo_list_positions).

class CExoDebug {
public:
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005E9400
};

class CExoBase {
public:
    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    CExoDebug* m_pcExoDebug;                        // 0x08
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08

    CExoLinkedListNode()
    {
        pObject = 0;
        pNext = 0;
        pPrev = 0;
    }
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();
    CExoLinkedListPosition AddBefore(void* pObject, CExoLinkedListPosition pos);
    void* GetAtPos(CExoLinkedListPosition pos);
    void* GetNext(CExoLinkedListPosition& pos);
    void* Remove(CExoLinkedListPosition pos);
    void* RemoveHead();
    void* RemoveTail();
    CExoLinkedListPosition AddHead(void* pObject);
    CExoLinkedListPosition AddTail(void* pObject);
};

// FUNCTION: SWKOTOR 0x005E9AD0
CExoLinkedListPosition CExoLinkedListInternal::AddBefore(void* pObject, CExoLinkedListPosition pos)
{
    if (pos == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0xbf, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::AddBefore() Position parameter is NULL");
        return 0;
    }
    CExoLinkedListNode* pNode = new CExoLinkedListNode;
    CExoLinkedListNode* pAt = (CExoLinkedListNode*)pos;
    pNode->pNext = pAt;
    pNode->pPrev = pAt->pPrev;
    if (pAt->pPrev) {
        pAt->pPrev->pNext = pNode;
    }
    pAt->pPrev = pNode;
    pNode->pObject = pObject;
    if (pTail == 0) {
        pTail = pNode;
    }
    if (pHead == pAt) {
        pHead = pNode;
    }
    nCount++;
    return pNode;
}

// FUNCTION: SWKOTOR 0x005E9B60
void* CExoLinkedListInternal::GetAtPos(CExoLinkedListPosition pos)
{
    if (pos == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0xec, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::GetAtPos() Position paramter is NULL");
        return 0;
    }
    return ((CExoLinkedListNode*)pos)->pObject;
}

// FUNCTION: SWKOTOR 0x005E9B90
void* CExoLinkedListInternal::GetNext(CExoLinkedListPosition& pos)
{
    if (pos == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0xff, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::GetNext() Position paramter is NULL");
        return 0;
    }
    CExoLinkedListNode* pNext = ((CExoLinkedListNode*)pos)->pNext;
    if (pNext == 0) {
        pos = pNext;
        return 0;
    }
    pos = pNext;
    return pNext->pObject;
}

// FUNCTION: SWKOTOR 0x005E9C10
void* CExoLinkedListInternal::Remove(CExoLinkedListPosition pos)
{
    if (pos == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0x133, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::Remove() Position paramter is NULL");
        return 0;
    }
    CExoLinkedListNode* pNode = (CExoLinkedListNode*)pos;
    CExoLinkedListNode* pPrev = pNode->pPrev;
    CExoLinkedListNode* pNext = pNode->pNext;
    void* pObject = pNode->pObject;
    if (pPrev) {
        pPrev->pNext = pNext;
    }
    if (pNext) {
        pNext->pPrev = pPrev;
    }
    if (pTail == pNode) {
        pTail = pNode->pPrev;
    }
    if (pHead == pNode) {
        pHead = pNode->pNext;
    }
    delete pNode;
    nCount--;
    return pObject;
}

// FUNCTION: SWKOTOR 0x005E9C80
void* CExoLinkedListInternal::RemoveHead()
{
    CExoLinkedListNode* pNode = pHead;
    if (pNode == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0x165, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::RemoveHead() Removing from empty List");
        return 0;
    }
    CExoLinkedListNode* pNext = pNode->pNext;
    void* pObject = pNode->pObject;
    pHead = pNext;
    if (pNext == 0) {
        pTail = 0;
    } else {
        pNext->pPrev = 0;
    }
    delete pNode;
    nCount--;
    return pObject;
}

// FUNCTION: SWKOTOR 0x005E9CE0
void* CExoLinkedListInternal::RemoveTail()
{
    CExoLinkedListNode* pNode = pTail;
    if (pNode == 0) {
        g_pExoBase->m_pcExoDebug->Warning(0x18b, "exolinkedlistinternal.cpp",
            "CExoLinkedListInternal::RemoveTail() Removing from empty List");
        return 0;
    }
    CExoLinkedListNode* pPrev = pNode->pPrev;
    void* pObject = pNode->pObject;
    pTail = pPrev;
    if (pPrev == 0) {
        pHead = 0;
    } else {
        pPrev->pNext = 0;
    }
    delete pNode;
    nCount--;
    return pObject;
}

// FUNCTION: SWKOTOR 0x005E9D70
CExoLinkedListPosition CExoLinkedListInternal::AddHead(void* pObject)
{
    CExoLinkedListNode* pNode = new CExoLinkedListNode;
    pNode->pNext = pHead;
    pNode->pPrev = 0;
    if (pHead) {
        pHead->pPrev = pNode;
    }
    pHead = pNode;
    pNode->pObject = pObject;
    if (pTail == 0) {
        pTail = pNode;
    }
    nCount++;
    return pNode;
}

// FUNCTION: SWKOTOR 0x005E9DC0
CExoLinkedListPosition CExoLinkedListInternal::AddTail(void* pObject)
{
    CExoLinkedListNode* pNode = new CExoLinkedListNode;
    pNode->pPrev = pTail;
    pNode->pNext = 0;
    if (pTail) {
        pTail->pNext = pNode;
    }
    pTail = pNode;
    pNode->pObject = pObject;
    if (pHead == 0) {
        pHead = pNode;
    }
    nCount++;
    return pNode;
}

// FUNCTION: SWKOTOR 0x005E9AB0
CExoLinkedListInternal::~CExoLinkedListInternal()
{
    CExoLinkedListNode* pNode = pHead;
    while (pNode) {
        CExoLinkedListNode* pDel = pNode;
        pNode = pNode->pNext;
        delete pDel;
    }
}
