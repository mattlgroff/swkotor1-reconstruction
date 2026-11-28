// CExoLocStringInternal constructor, ClearLocString, destructor
// (b005e0_exo_foundation_leaves, b005e4_exo_pimpl_ctors).

#include "shared/exo_string.h"

#include "shared/exo_loc_string_st.h"

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"




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
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
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
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
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
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
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
