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

#include "shared/globals.h"

class CExoLocStringInternal {
public:
    void* m_lstLocStrings;                          // 0x00 CExoLinkedList<ExoLocString_st>
    unsigned long m_nCount;                         // 0x04
    int m_bModified;                                // 0x08

    int GetString(int nId, CExoString& s);          // 0x005EC9C0
    int AddString(int nId, const CExoString& s);    // 0x005EC700
    void RemoveString(int nId);                     // 0x005ECA80
};

#include "shared/exo_loc_string.h"

inline void CExoLocString::RemoveString(int nLanguage, unsigned char nGender)
{
    m_pcExoLocStringInternal->RemoveString(MakeId(nLanguage, nGender));
}

// FUNCTION: SWKOTOR 0x005EA340
int CExoLocString::AddString(int nLanguage, const CExoString& sNew, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    RemoveString(nLanguage, nGender);
    CExoString s;
    GetStringAndCache(nLanguage, &s, nGender);
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
    GetString(nId, &s, 0);
    if (s.GetLength()) {
        return (float)s.GetLength() * 0.11f + 1.0f;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x005EA130
int CExoLocString::GetStringAndCache(int nLanguage, CExoString* pString, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    if (m_pcExoLocStringInternal->GetString(nId, *pString)) {
        return 1;
    }
    STR_RES strRes;
    bool bFound;
    if (g_pTlkTable && g_pTlkTable->Fetch(m_dwStringRef, strRes, nGender)) {
        *pString = strRes.sString;
        if (*pString != "") {
            m_pcExoLocStringInternal->AddString(nId, *pString);
            m_pcExoLocStringInternal->m_bModified = 1;
        }
        bFound = true;
    } else {
        bFound = false;
    }
    return bFound;
}

// FUNCTION: SWKOTOR 0x005EA050
int CExoLocString::GetString(int nLanguage, CExoString* pString, unsigned char nGender)
{
    int nId = MakeIdRef(nLanguage, nGender);
    if (m_pcExoLocStringInternal->GetString(nId, *pString)) {
        return 1;
    }
    STR_RES strRes;
    bool bFound;
    if (g_pTlkTable && g_pTlkTable->Fetch(m_dwStringRef, strRes, nGender)) {
        *pString = strRes.sString;
        bFound = true;
    } else {
        bFound = false;
    }
    return bFound;
}
