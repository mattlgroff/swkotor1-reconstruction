// exo_linked_list.h -- CExoLinkedList<T>, the engine's doubly linked list
//
// One declaration for every unit (CODE_RULES.md, R1). CExoLinkedListInternal
// does the work out of line (exolinkedlistinternal.o, 0x005E9AA0-0x005E9E10,
// as the Mac debug map lists it); CExoLinkedList<T> is the typed inline
// wrapper every unit expands. A position is the node itself.
#ifndef SHARED_EXO_LINKED_LIST_H
#define SHARED_EXO_LINKED_LIST_H

class CExoLinkedListNode {
public:
    CExoLinkedListNode()
    {
        pObject = 0;
        pNext = 0;
        pPrev = 0;
    }

    CExoLinkedListNode* pPrev;                          // +0x00
    CExoLinkedListNode* pNext;                          // +0x04
    void* pObject;                                      // +0x08
};

typedef CExoLinkedListNode* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                           // 0x005E9AA0
    ~CExoLinkedListInternal();                          // 0x005E9AB0

    CExoLinkedListPosition AddBefore(void* pObject, CExoLinkedListPosition pos); // 0x005E9AD0
    void* GetAtPos(CExoLinkedListPosition pos) const;  // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos) const;  // 0x005E9B90
    void* GetPrev(CExoLinkedListPosition& pos) const;  // 0x005E9BD0
    void* Remove(CExoLinkedListPosition pos);           // 0x005E9C10
    void* RemoveHead();                                 // 0x005E9C80
    void* RemoveTail();                                 // 0x005E9CE0
    int Contains(void* pObject) const;                  // 0x005E9D40
    CExoLinkedListPosition AddHead(void* pObject);      // 0x005E9D70
    CExoLinkedListPosition AddTail(void* pObject);      // 0x005E9DC0

    void* GetHead() const
    {
        CExoLinkedListNode* pNode = pHead;
        if (!pNode) {
            return 0;
        }
        return pNode->pObject;
    }
    // A second head accessor: the original reads the head object in two
    // shapes, this conditional assignment where a loop fetches the object
    // before its position (CSWCObject's visual-effect loops) and GetHead's
    // early return elsewhere. GetHeadInline is a placeholder name.
    void* GetHeadInline() const
    {
        void* p = pHead;
        if (pHead) {
            p = pHead->pObject;
        }
        return p;
    }
    void* GetTail() const
    {
        void* p = pTail;
        if (pTail) {
            p = pTail->pObject;
        }
        return p;
    }

    CExoLinkedListNode* pHead;                          // +0x00
    CExoLinkedListNode* pTail;                          // +0x04
    unsigned long nCount;                               // +0x08
};

template <class T> class CExoLinkedList {
public:
    CExoLinkedList() { m_pcExoLinkedListInternal = new CExoLinkedListInternal(); }
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }

    CExoLinkedListPosition GetHeadPos() const { return m_pcExoLinkedListInternal->pHead; }
    CExoLinkedListPosition GetTailPos() const { return m_pcExoLinkedListInternal->pTail; }
    T* GetNext(CExoLinkedListPosition& pos) const { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* GetPrev(CExoLinkedListPosition& pos) const { return (T*)m_pcExoLinkedListInternal->GetPrev(pos); }
    T* GetAtPos(CExoLinkedListPosition pos) const { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetHead() const
    {
        CExoLinkedListInternal* pInternal = m_pcExoLinkedListInternal;
        return (T*)pInternal->GetHead();
    }
    T* GetHeadInline() const { return (T*)m_pcExoLinkedListInternal->GetHeadInline(); }
    T* GetTail() const { return (T*)m_pcExoLinkedListInternal->GetTail(); }
    unsigned long GetCount() const { return m_pcExoLinkedListInternal->nCount; }
    int IsEmpty() const { return m_pcExoLinkedListInternal->pHead == 0; }
    int Contains(T* pObject) const { return m_pcExoLinkedListInternal->Contains(pObject); }

    CExoLinkedListPosition AddHead(T* pObject) { return m_pcExoLinkedListInternal->AddHead(pObject); }
    CExoLinkedListPosition AddTail(T* pObject) { return m_pcExoLinkedListInternal->AddTail(pObject); }
    CExoLinkedListPosition AddBefore(T* pObject, CExoLinkedListPosition pos)
    {
        return m_pcExoLinkedListInternal->AddBefore(pObject, pos);
    }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    T* RemoveTail() { return (T*)m_pcExoLinkedListInternal->RemoveTail(); }

    CExoLinkedListInternal* m_pcExoLinkedListInternal;  // +0x00
};

#endif
