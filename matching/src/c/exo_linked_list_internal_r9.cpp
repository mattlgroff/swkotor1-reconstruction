// CExoLinkedListInternal constructor and IsInList (b005e9, r9).

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    CExoLinkedListInternal();
    int IsInList(void* pObject);
};

// FUNCTION: SWKOTOR 0x005E9AA0 SYMBOL: ??0CExoLinkedListInternal@@QAE@XZ
CExoLinkedListInternal::CExoLinkedListInternal()
{
    pTail = 0;
    pHead = 0;
    nCount = 0;
}

// FUNCTION: SWKOTOR 0x005E9D40
int CExoLinkedListInternal::IsInList(void* pObject)
{
    for (CExoLinkedListNode* pNode = pHead; pNode; pNode = pNode->pNext) {
        if (pNode->pObject == pObject) {
            return 1;
        }
    }
    return 0;
}
