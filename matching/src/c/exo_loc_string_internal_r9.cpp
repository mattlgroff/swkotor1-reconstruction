// CExoLocStringInternal::AddString / GetString(index) / operator=
// (b005e9 foundations leftovers, r9).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
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

class CExoLocStringInternal {
public:
    CExoLinkedList<ExoLocString_st> m_lstLocStrings;   // 0x00
    unsigned long m_nCount;                         // 0x04
    int m_bModified;                                // 0x08

    void ClearLocString();                          // 0x005ECB10
    int AddString(int nId, const CExoString& s);
    int GetString(unsigned long nIndex, int* pId, CExoString& s);
    int operator=(const CExoLocStringInternal& o);
};

// FUNCTION: SWKOTOR 0x005EC700
int CExoLocStringInternal::AddString(int nId, const CExoString& s)
{
    void* pos = m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    while (p) {
        if (p->m_nLanguage == nId) {
            p->m_sString = s;
            return 1;
        }
        p = m_lstLocStrings.GetNext(pos);
    }
    ExoLocString_st* pNew = new ExoLocString_st;
    if (!pNew) {
        return 0;
    }
    pNew->m_nLanguage = nId;
    pNew->m_sString = s;
    m_lstLocStrings.AddTail(pNew);
    m_nCount++;
    return 1;
}

// FUNCTION: SWKOTOR 0x005EC950
int CExoLocStringInternal::GetString(unsigned long nIndex, int* pId, CExoString& s)
{
    if (nIndex >= m_nCount) {
        return 0;
    }
    void* pos = m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    unsigned long i = 0;
    while (p) {
        if (i == nIndex) {
            *pId = p->m_nLanguage;
            s = p->m_sString;
            return 1;
        }
        p = m_lstLocStrings.GetNext(pos);
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005ECBB0
int CExoLocStringInternal::operator=(const CExoLocStringInternal& o)
{
    ClearLocString();
    void* pos = o.m_lstLocStrings.GetHeadPos();
    ExoLocString_st* p = o.m_lstLocStrings.GetHead();
    while (p) {
        ExoLocString_st* pNew = new ExoLocString_st;
        if (!pNew) {
            return 0;
        }
        pNew->m_nLanguage = p->m_nLanguage;
        pNew->m_sString = p->m_sString;
        m_nCount++;
        m_lstLocStrings.AddTail(pNew);
        p = o.m_lstLocStrings.GetNext(pos);
    }
    m_bModified = o.m_bModified;
    return 1;
}
