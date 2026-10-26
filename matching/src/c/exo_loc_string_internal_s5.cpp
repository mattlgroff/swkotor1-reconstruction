// CExoLocStringInternal::Compare (s5).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int operator!=(const CExoString& s) const;      // 0x005E5310
};

struct ExoLocString_st {
    int m_nLanguage;                                // 0x00
    CExoString m_sString;                           // 0x04
};

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

    void* GetNext(void*& pos);                      // 0x005E9B90
    void* AddTail(void* p);                         // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    void* GetHeadPos() const { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead() const
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(void*& pos) const { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    void* AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
};

class CExoLocString {
public:
    void* m_pcExoLocStringInternal;                 // 0x00

    unsigned long GetStringCount() const;           // 0x005E9EF0
    int GetString(int nLanguage, CExoString& s, unsigned char nGender) const;   // 0x005EA050
};

class CExoLocStringInternal {
public:
    CExoLinkedList<ExoLocString_st> m_lstLocStrings;   // 0x00
    unsigned long m_nCount;                         // 0x04
    int m_bModified;                                // 0x08

    int Compare(const CExoLocString& other);
};

// NONMATCH (16 diff insns): loop-entry register choice (original reads the
// language into esi and reuses eax for the string address; ours keeps the
// node in esi). A language local swaps esi/edi for the two scratch strings.
// FUNCTION: SWKOTOR 0x005EC7F0
int CExoLocStringInternal::Compare(const CExoLocString& other)
{
    unsigned long nCount = m_nCount;
    if (nCount != other.GetStringCount()) {
        return 0;
    }
    CExoString* pMine = new CExoString;
    CExoString* pTheirs = new CExoString;
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    void* pos = m_lstLocStrings.GetHeadPos();
    while (p) {
        *pMine = p->m_sString;
        other.GetString(p->m_nLanguage, *pTheirs, 0);
        if (*pMine != *pTheirs) {
            delete pMine;
            delete pTheirs;
            return 0;
        }
        p = m_lstLocStrings.GetNext(pos);
    }
    delete pMine;
    delete pTheirs;
    return 1;
}
