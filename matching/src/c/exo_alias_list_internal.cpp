// CExoAliasListInternal / CExoAlias constructors (b005e4_exo_pimpl_ctors).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const char* s) const;            // 0x005E5290
    int GetLength() const;                          // 0x005E5790
    int ComparePrefixNoCase(const CExoString& s, int nSize) const;   // 0x005E5900
};

class CExoAlias {
public:
    CExoString m_sAlias;                            // 0x00
    CExoString m_sPath;                             // 0x08
    CExoAlias* m_pNext;                             // 0x10

    CExoAlias();
};

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
