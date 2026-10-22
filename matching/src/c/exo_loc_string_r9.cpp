// CExoLocString members that fall back to the dialog.tlk table
// (b005e9 foundations leftovers, r9). Callers are defined before their
// callees so the out-of-line calls are kept.

#include "c/exo_base.h"

class CResRef {
public:
    char m_resRef[16];

    CResRef();                                      // 0x00405ED0
};

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender);  // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CExoLocStringInternal {
public:
    void* m_lstLocStrings;                          // 0x00 CExoLinkedList<ExoLocString_st>
    unsigned long m_nCount;                         // 0x04
    int m_bModified;                                // 0x08

    int GetString(int nId, CExoString& s);          // 0x005EC9C0
    int AddString(int nId, const CExoString& s);    // 0x005EC700
    void RemoveString(int nId);                     // 0x005ECA80
};

class CExoLocString {
public:
    CExoLocStringInternal* m_pcExoLocStringInternal;   // 0x00
    unsigned long m_dwStringRef;                    // 0x04

    static int MakeId(int nLanguage, unsigned char nGender)
    {
        nGender = nLanguage ? nGender : 0;
        return nLanguage * 2 + nGender;
    }
    static int MakeIdRef(int nLanguage, unsigned char& nGender)
    {
        nGender = nLanguage ? nGender : 0;
        return nLanguage * 2 + nGender;
    }

    void RemoveString(int nLanguage, unsigned char nGender)
    {
        m_pcExoLocStringInternal->RemoveString(MakeId(nLanguage, nGender));
    }

    int AddString(int nLanguage, const CExoString& s, unsigned char nGender);
    float GetStringReadTime(int nLanguage, unsigned char nGender);
    int GetStringAndCache(int nLanguage, CExoString& s, unsigned char nGender);
    int GetString(int nLanguage, CExoString& s, unsigned char nGender);
};

// FUNCTION: SWKOTOR 0x005EA340
int CExoLocString::AddString(int nLanguage, const CExoString& sNew, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    RemoveString(nLanguage, nGender);
    CExoString s;
    GetStringAndCache(nLanguage, s, nGender);
    if (s == sNew) {
        return 1;
    }
    int bResult = m_pcExoLocStringInternal->AddString(nId, sNew);
    return bResult;
}

// FUNCTION: SWKOTOR 0x005EA270
float CExoLocString::GetStringReadTime(int nLanguage, unsigned char nGender)
{
    CExoString s;
    int nId = MakeId(nLanguage, nGender);
    GetString(nId, s, 0);
    if (s.GetLength()) {
        return (float)s.GetLength() * 0.11f + 1.0f;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x005EA130
int CExoLocString::GetStringAndCache(int nLanguage, CExoString& s, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    if (m_pcExoLocStringInternal->GetString(nId, s)) {
        return 1;
    }
    STR_RES strRes;
    bool bFound;
    if (g_pTlkTable && g_pTlkTable->Fetch(m_dwStringRef, strRes, nGender)) {
        s = strRes.sString;
        if (s != "") {
            m_pcExoLocStringInternal->AddString(nId, s);
            m_pcExoLocStringInternal->m_bModified = 1;
        }
        bFound = true;
    } else {
        bFound = false;
    }
    return bFound;
}

// FUNCTION: SWKOTOR 0x005EA050
int CExoLocString::GetString(int nLanguage, CExoString& s, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    if (m_pcExoLocStringInternal->GetString(nId, s)) {
        return 1;
    }
    STR_RES strRes;
    bool bFound;
    if (g_pTlkTable && g_pTlkTable->Fetch(m_dwStringRef, strRes, nGender)) {
        s = strRes.sString;
        bFound = true;
    } else {
        bFound = false;
    }
    return bFound;
}
