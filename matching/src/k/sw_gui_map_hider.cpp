// CSWGuiMapHider (map control, 0x248 bytes, vtable 0x00754738): map note
// list and mouse hit test. Names from the Mac debug map where present,
// otherwise descriptive.

typedef void* CExoLinkedListPosition;

class CExoLinkedListNode {
public:
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    T* GetHead()
    {
        void* p = m_pList->m_pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    T* RemoveHead() { return (T*)m_pList->RemoveHead(); }
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    T& operator[](int i) { return element[i]; }
};

class CSWGuiControl {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual int HitCheckMouse(int x, int y);        // 0x44
};

// One map note entry; the object id comes first (descriptive).
struct CSWGuiMapNote {
    unsigned long m_oidNote;
};

class CSWGuiMapHider {
public:
    void ClearMapNotes();
    void SetMapNote(unsigned long oidNote);
    CSWGuiControl* HitCheckMouse(int x, int y);

    void* m_pVTable;                                // 0x00 (vtable 0x00754738)
    int m_nLeft;                                    // 0x04
    int m_nTop;                                     // 0x08
    int m_nWidth;                                   // 0x0c
    int m_nHeight;                                  // 0x10
    char pad_0x14[0x68 - 0x14];
    CSWGuiControl m_cMarker;                        // 0x68 (descriptive)
    char pad_0x6c[0x22c - 0x6c];
    CExoArrayList<CSWGuiControl*> m_aControls;      // 0x22c
    CExoLinkedList<CSWGuiMapNote> m_lMapNotes;      // 0x238
    CExoLinkedListPosition m_posSelectedNote;       // 0x23c
};

// NONMATCH (4): register tie only; the original loads the list pointer into
// EAX in the body and ECX at the loop test (ours ECX / EDX). IsEmpty,
// GetHead and GetHeadPos loop tests give the same code.
// FUNCTION: SWKOTOR 0x00692E30
void CSWGuiMapHider::ClearMapNotes()
{
    while (m_lMapNotes.GetHeadPos() != 0) {
        delete m_lMapNotes.GetHead();
        m_lMapNotes.RemoveHead();
    }
    m_posSelectedNote = 0;
}

// FUNCTION: SWKOTOR 0x006932A0
void CSWGuiMapHider::SetMapNote(unsigned long oidNote)
{
    CExoLinkedListPosition pos = m_lMapNotes.GetHeadPos();
    while (pos != 0) {
        if (oidNote == m_lMapNotes.GetAtPos(pos)->m_oidNote) {
            m_posSelectedNote = pos;
            return;
        }
        m_lMapNotes.GetNext(pos);
    }
}

// NONMATCH (4): the original copies m_nLeft / m_nTop into EBX / EBP (the
// later nX / nY registers) before adding the extent; local-copy, operand
// order and in-place parameter shapes do not reproduce it.
// FUNCTION: SWKOTOR 0x00693300
CSWGuiControl* CSWGuiMapHider::HitCheckMouse(int x, int y)
{
    if (x < m_nLeft || x > m_nLeft + m_nWidth || y < m_nTop || y > m_nTop + m_nHeight) {
        return 0;
    }
    int nX = x - m_nLeft;
    int nY = y - m_nTop;
    CSWGuiControl* pMarker = &m_cMarker;
    if (pMarker->HitCheckMouse(nX, nY)) {
        return pMarker;
    }
    for (int i = 0; i < m_aControls.num; i++) {
        if (m_aControls[i]->HitCheckMouse(nX, nY)) {
            return m_aControls[i];
        }
    }
    return 0;
}
