// Worker C matching headers: shared Exo foundation types.
#ifndef C_EXO_BASE_H
#define C_EXO_BASE_H

class CResRef;

#include "shared/exo_string.h"

typedef void* CExoLinkedListPosition;

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

    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    void* RemoveHead();                             // 0x005E9C80
    void* RemoveTail();                             // 0x005E9CE0
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
    void* GetHead()
    {
        CExoLinkedListNode* pNode = pHead;
        if (!pNode) {
            return 0;
        }
        return pNode->pObject;
    }
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return pos ? (T*)((CExoLinkedListNode*)pos)->pObject : 0; }
    T* GetHead()
    {
        CExoLinkedListInternal* pInternal = m_pcExoLinkedListInternal;
        return (T*)pInternal->GetHead();
    }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    T* RemoveTail() { return (T*)m_pcExoLinkedListInternal->RemoveTail(); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    T& operator[](int i) { return element[i]; }
    int count(CExoString s);
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void RemoveIndex(int i);                        // out of line (e.g. 0x0048DDE0)
    void Insert(T t, int pos);                      // out of line
    void AddUnique(T t);                            // out of line (CExoString: 0x005F7C10)
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
};

#endif
