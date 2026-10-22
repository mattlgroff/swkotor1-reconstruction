// CExoLocStringInternal constructor, ClearLocString, destructor
// (b005e0_exo_foundation_leaves, b005e4_exo_pimpl_ctors).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int GetLength() const;                          // 0x005E5790
};

struct ExoLocString_st {
    int m_nLanguage;                                // 0x00
    CExoString m_sString;                           // 0x04

    ~ExoLocString_st();                             // 0x0059EA20
};

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
    void* GetNext(void*& pos);                      // 0x005E9B90
    void* Remove(void* pos);                        // 0x005E9C10
};

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedList();                               // 0x005EC690 for ExoLocString_st
    ~CExoLinkedList()
    {
        if (m_pcExoLinkedListInternal) {
            delete m_pcExoLinkedListInternal;
        }
    }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    void* GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(void*& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* Remove(void* pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
};

class CExoLocStringInternal {
public:
    CExoLinkedList<ExoLocString_st> m_lstLocStrings;   // 0x00
    unsigned long m_nCount;                         // 0x04
    unsigned long m_nUnknown8;                      // 0x08

    CExoLocStringInternal();
    ~CExoLocStringInternal();
    void ClearLocString();
    int GetString(int nLanguage, CExoString& sOut);
    int GetStringLength(unsigned long nIndex);
    void RemoveString(int nLanguage);
};

// FUNCTION: SWKOTOR 0x005ECAF0
CExoLocStringInternal::CExoLocStringInternal()
{
    m_nCount = 0;
    m_nUnknown8 = 0;
}

// FUNCTION: SWKOTOR 0x005ECB10
void CExoLocStringInternal::ClearLocString()
{
    while (!m_lstLocStrings.IsEmpty()) {
        ExoLocString_st* p = m_lstLocStrings.RemoveHead();
        if (p) {
            delete p;
        }
    }
    m_nCount = 0;
}

// FUNCTION: SWKOTOR 0x005ECB50
CExoLocStringInternal::~CExoLocStringInternal()
{
    ClearLocString();
}

// FUNCTION: SWKOTOR 0x005EC9C0
int CExoLocStringInternal::GetString(int nLanguage, CExoString& sOut)
{
    // NONMATCH (0 diff insns, one jump target): the original's empty-list
    // `je` lands on the loop's own null test instead of being threaded to
    // the exit; for-loop and GetHead(pos) variants did not change it.
    void* pos = m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    while (p) {
        if (p->m_nLanguage == nLanguage) {
            sOut = p->m_sString;
            return 1;
        }
        p = m_lstLocStrings.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005ECA20
int CExoLocStringInternal::GetStringLength(unsigned long nIndex)
{
    if (nIndex >= m_nCount) {
        return 0;
    }
    void* pos = m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    unsigned long i = 0;
    while (p) {
        if (i == nIndex) {
            return p->m_sString.GetLength();
        }
        p = m_lstLocStrings.GetNext(pos);
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005ECA80
void CExoLocStringInternal::RemoveString(int nLanguage)
{
    // NONMATCH (6 diff insns): same unthreaded empty-list `je` as GetString,
    // plus the original keeps the head in eax and copies to esi.
    void* pos = m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    while (p) {
        if (p->m_nLanguage == nLanguage) {
            m_lstLocStrings.Remove(pos);
            delete p;
            m_nCount--;
            return;
        }
        p = m_lstLocStrings.GetNext(pos);
    }
}
