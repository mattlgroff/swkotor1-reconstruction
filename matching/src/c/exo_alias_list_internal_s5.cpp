// CExoAliasListInternal::ResolveFileName (s5).
#include "c/exo_base.h"

class CExoAliasList {
public:
    CExoString* GetAliasPath(const CExoString& sAlias, int nLength);    // 0x005E6890
};

class CExoBase {
public:
    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    void* m_pcExoDebug;                             // 0x08
    CExoAliasList* m_pcExoAliasList;                // 0x0c

    CExoString* GetResourceExtension(unsigned short nType);             // 0x005E6660
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoAlias {
public:
    CExoString m_sAlias;                            // 0x00
    CExoString m_sPath;                             // 0x08
    CExoAlias* m_pNext;                             // 0x10
    CExoAlias* m_pPrev;                             // 0x14

    CExoAlias();                                    // 0x005EB600
};

class CExoAliasListInternal {
public:
    CExoString m_sEmpty;                            // 0x00
    CExoAlias* m_pAliases;                          // 0x08

    CExoString ResolveFileName(const CExoString& sName, unsigned short nType) const;
    void Add(const CExoString& sAlias, const CExoString& sPath);
};

// FUNCTION: SWKOTOR 0x005EB6B0
CExoString CExoAliasListInternal::ResolveFileName(const CExoString& sName, unsigned short nType) const
{
    CExoString sResult;
    int nColon = sName.Find(':', 0);
    if (nColon <= 0) {
        return "";
    }
    if (g_pExoBase->m_pcExoAliasList->GetAliasPath(sName, nColon)->GetLength() == 0) {
        return "";
    }
    int nAliasLength = nColon;
    if (sName.CStr()[nColon + 1] == '\\') {
        nColon++;
    }
    if (g_pExoBase->GetResourceExtension(nType)->GetLength() != 0) {
        sResult.Format("%s%s.%s", g_pExoBase->m_pcExoAliasList->GetAliasPath(sName, nAliasLength)->CStr(),
                       sName.CStr() + nColon + 1, g_pExoBase->GetResourceExtension(nType)->CStr());
    } else {
        sResult.Format("%s%s", g_pExoBase->m_pcExoAliasList->GetAliasPath(sName, nAliasLength)->CStr(),
                       sName.CStr() + nColon + 1);
    }
    return sResult;
}

// NONMATCH (21 diff insns): the original's insertion loop is not rotated
// (top test, jmp back) and keeps `this` in edi with no zero register; ours
// rotates it and caches 0 in edi. while/for(;;)/break forms are identical.
// FUNCTION: SWKOTOR 0x005EBF30
void CExoAliasListInternal::Add(const CExoString& sAlias, const CExoString& sPath)
{
    CExoString sNewAlias(sAlias);
    CExoString sNewPath(sPath);
    if (sNewPath == "") {
        return;
    }
    sNewAlias = sNewAlias.UpperCase();
    if (sNewPath.CStr()[sNewPath.GetLength() - 1] != '\\') {
        sNewPath = sNewPath + "\\";
    }
    CExoAlias* pPrev = 0;
    CExoAlias* pNext = m_pAliases;
    while (pNext && !(pNext->m_sAlias > sNewAlias)) {
        pPrev = pNext;
        pNext = pNext->m_pNext;
    }
    CExoAlias* pAlias = new CExoAlias;
    pAlias->m_sAlias = sNewAlias;
    pAlias->m_sPath = sNewPath;
    if (m_pAliases == pNext) {
        m_pAliases = pAlias;
    }
    pAlias->m_pPrev = pPrev;
    pAlias->m_pNext = pNext;
    if (pPrev) {
        pPrev->m_pNext = pAlias;
    }
    if (pNext) {
        pNext->m_pPrev = pAlias;
    }
}
