// CExoLinkedList<T>: doubly linked list wrapper around CExoLinkedListInternal.
#ifndef B_EXO_LINKED_LIST_H
#define B_EXO_LINKED_LIST_H

#include <stddef.h>

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                                   // 0x005E9AA0
    ~CExoLinkedListInternal();                                  // 0x005E9AB0
    void* GetAtPos(void* pPos);                                 // 0x005E9B60
    void* GetNext(void*& pPos);                                 // 0x005E9B90
    void* RemoveHead();                                         // 0x005E9C80
    void* AddTail(void* pObject);                               // 0x005E9DC0
    void* GetHead() { return m_pHead == NULL ? NULL : m_pHead->pObject; }

    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList() { m_pList = new CExoLinkedListInternal(); }
    ~CExoLinkedList() { delete m_pList; }
    void* GetHeadPos() { return m_pList->m_pHead; }
    T* GetHead() { return (T*)m_pList->GetHead(); }
    T* GetAtPos(void* pPos) { return (T*)m_pList->GetAtPos(pPos); }
    T* GetNext(void*& pPos) { return (T*)m_pList->GetNext(pPos); }
    T* RemoveHead() { return (T*)m_pList->RemoveHead(); }

    CExoLinkedListInternal* m_pList;
};

#endif
