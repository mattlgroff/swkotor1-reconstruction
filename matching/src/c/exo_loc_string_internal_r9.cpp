// CExoLocStringInternal::AddString / GetString(index) / operator=
// (b005e9 foundations leftovers, r9).

#include "shared/exo_string.h"

#include "shared/exo_loc_string_st.h"

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



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
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
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
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
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
    CExoLinkedListPosition pos = o.m_lstLocStrings.GetHeadPos();
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
