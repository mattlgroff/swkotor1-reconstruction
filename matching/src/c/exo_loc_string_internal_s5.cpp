// CExoLocStringInternal::Compare (s5).

#include "shared/exo_string.h"

#include "shared/exo_loc_string_st.h"

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



#include "shared/exo_loc_string.h"

class CExoLocStringInternal {
public:
    CExoLinkedList<ExoLocString_st> m_lstLocStrings;   // 0x00
    unsigned long m_nCount;                         // 0x04
    int m_bModified;                                // 0x08

    int Compare(CExoLocString& other);
};

// NONMATCH (16 diff insns): loop-entry register choice (original reads the
// language into esi and reuses eax for the string address; ours keeps the
// node in esi). A language local swaps esi/edi for the two scratch strings.
// FUNCTION: SWKOTOR 0x005EC7F0
int CExoLocStringInternal::Compare(CExoLocString& other)
{
    unsigned long nCount = m_nCount;
    if (nCount != other.GetStringCount()) {
        return 0;
    }
    CExoString* pMine = new CExoString;
    CExoString* pTheirs = new CExoString;
    ExoLocString_st* p = m_lstLocStrings.GetHead();
    CExoLinkedListPosition pos = m_lstLocStrings.GetHeadPos();
    while (p) {
        *pMine = p->m_sString;
        other.GetString(p->m_nLanguage, pTheirs, 0);
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
