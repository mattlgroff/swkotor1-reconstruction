// CExoAliasListInternal / CExoAlias constructors (b005e4_exo_pimpl_ctors).

#include "shared/exo_string.h"

#include "shared/exo_alias.h"

class CExoAliasListInternal {
public:
    CExoString m_sUnknown0;                         // 0x00
    CExoAlias* m_pAliases;                          // 0x08

    CExoAliasListInternal();
    CExoString* GetAliasPath(const CExoString& sAlias, int nLength);
};

// FUNCTION: SWKOTOR 0x005EB5B0
CExoAliasListInternal::CExoAliasListInternal()
{
    m_pAliases = 0;
    m_sUnknown0 = "";
}

// FUNCTION: SWKOTOR 0x005EB600
CExoAlias::CExoAlias()
{
}

// Returns the empty default string (this) on a miss.
// FUNCTION: SWKOTOR 0x005EB650
CExoString* CExoAliasListInternal::GetAliasPath(const CExoString& sAlias, int nLength)
{
    if (sAlias == "") {
        return &m_sUnknown0;
    }
    if (nLength == 0) {
        nLength = sAlias.GetLength();
    }
    // NONMATCH (14 diff insns): the original's loop is not rotated (top
    // test, jmp back, no loop alignment); for/while/goto/for(;;) forms all
    // come out rotated with -O2.
    CExoAlias* pAlias = m_pAliases;
    while (pAlias && pAlias->m_sAlias.ComparePrefixNoCase(sAlias, nLength) != 1) {
        pAlias = pAlias->m_pNext;
    }
    if (pAlias) {
        return &pAlias->m_sPath;
    }
    return &m_sUnknown0;
}
