// CExoLocString facade leaves (b005e0_exo_foundation_leaves,
// b005e4_exo_pimpl_ctors).

#include "c/exo_base.h"

class CExoLocStringInternal {
public:
    void* m_lstLocStrings;                          // 0x00 CExoLinkedList<ExoLocString_st>
    unsigned long m_nCount;                         // 0x04
    unsigned long m_nUnknown8;                      // 0x08

    CExoLocStringInternal();                        // 0x005ECAF0
    ~CExoLocStringInternal();                       // 0x005ECB50
    int GetString(int nIndex, int* pLanguage, CExoString& s);   // 0x005EC950
    CExoLocStringInternal& operator=(const CExoLocStringInternal& o);   // 0x005ECBB0
};

class CExoLocString {
public:
    CExoLocStringInternal* m_pcExoLocStringInternal;   // 0x00
    unsigned long m_dwStringRef;                    // 0x04

    CExoLocString();
    CExoLocString(const CExoLocString& o);
    ~CExoLocString();
    int GetString(int nIndex, int* pLanguage, CExoString& s, unsigned char* pGender);
    CExoLocString& operator=(const CExoLocString& o);
    unsigned long GetStringCount();
};

// FUNCTION: SWKOTOR 0x005E9E70
CExoLocString& CExoLocString::operator=(const CExoLocString& o)
{
    if (this != &o) {
        *m_pcExoLocStringInternal = *o.m_pcExoLocStringInternal;
        m_dwStringRef = o.m_dwStringRef;
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x005E9EF0
unsigned long CExoLocString::GetStringCount()
{
    return m_pcExoLocStringInternal->m_nCount;
}

// FUNCTION: SWKOTOR 0x005EA000
CExoLocString::~CExoLocString()
{
    if (m_pcExoLocStringInternal) {
        delete m_pcExoLocStringInternal;
    }
}

// FUNCTION: SWKOTOR 0x005E9E10 SYMBOL: ??0CExoLocString@@QAE@XZ
CExoLocString::CExoLocString()
{
    m_pcExoLocStringInternal = new CExoLocStringInternal();
    m_dwStringRef = 0xffffffff;
}

// FUNCTION: SWKOTOR 0x005E9EB0
int CExoLocString::GetString(int nIndex, int* pLanguage, CExoString& s, unsigned char* pGender)
{
    int bResult = m_pcExoLocStringInternal->GetString(nIndex, pLanguage, s);
    *pGender = (unsigned char)(*pLanguage % 2);
    *pLanguage = *pLanguage / 2;
    return bResult;
}

// FUNCTION: SWKOTOR 0x005E9F80 SYMBOL: ??0CExoLocString@@QAE@ABV0@@Z
CExoLocString::CExoLocString(const CExoLocString& o)
{
    m_pcExoLocStringInternal = new CExoLocStringInternal();
    if (this != &o) {
        *m_pcExoLocStringInternal = *o.m_pcExoLocStringInternal;
        m_dwStringRef = o.m_dwStringRef;
    }
}
