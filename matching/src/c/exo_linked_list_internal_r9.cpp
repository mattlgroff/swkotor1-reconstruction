// CExoLinkedListInternal constructor and IsInList (b005e9, r9).

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"

// FUNCTION: SWKOTOR 0x005E9AA0 SYMBOL: ??0CExoLinkedListInternal@@QAE@XZ
CExoLinkedListInternal::CExoLinkedListInternal()
{
    pTail = 0;
    pHead = 0;
    nCount = 0;
}

// FUNCTION: SWKOTOR 0x005E9D40
int CExoLinkedListInternal::Contains(void* pObject) const
{
    for (CExoLinkedListNode* pNode = pHead; pNode; pNode = pNode->pNext) {
        if (pNode->pObject == pObject) {
            return 1;
        }
    }
    return 0;
}
