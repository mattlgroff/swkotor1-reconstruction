// CSWCModule::GetModuleName, CSWCPlayer portrait setters and the 0x128-byte
// client object created at CClientExoAppInternal+0xe0 (worker C r9).

#include "c/exo_base.h"

class CResRef {
public:
    char m_resRef[16];

    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const CResRef& r);            // 0x00406120
};

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
};

class CTwoDimArrays {
public:
    char pad_0x0[0x38];
    C2DA* m_pPortraits;                             // 0x38

    C2DA* GetPortraits() { return m_pPortraits; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCModule {
public:
    char pad_0x0[0x10];
    CExoString m_sModuleName;                       // 0x10

    CExoString GetModuleName();
};

// FUNCTION: SWKOTOR 0x005EE3E0
CExoString CSWCModule::GetModuleName()
{
    return m_sModuleName;
}

class CClientUnknownE0 {
public:
    char pad_0x0[0x104];
    int m_n104;                                     // 0x104
    int m_n108;                                     // 0x108
    char pad_0x10c[0x4];
    CExoString m_s110;                              // 0x110
    CExoString m_s118;                              // 0x118
    CExoString m_s120;                              // 0x120

    CClientUnknownE0();
};

// FUNCTION: SWKOTOR 0x005EE400
CClientUnknownE0::CClientUnknownE0()
{
    m_n108 = 0;
    m_n104 = 0;
}

class CSWCPlayer {
public:
    char pad_0x0[0x14];
    CResRef m_cPortrait;                            // 0x14
    unsigned short m_nPortraitId;                   // 0x24

    void SetPortrait(CResRef cPortrait);
    void SetPortraitId(unsigned short nPortraitId);
};

// FUNCTION: SWKOTOR 0x005EE480
void CSWCPlayer::SetPortrait(CResRef cPortrait)
{
    m_cPortrait = cPortrait;
}

// FUNCTION: SWKOTOR 0x005EE5C0
void CSWCPlayer::SetPortraitId(unsigned short nPortraitId)
{
    CExoString sPrefix;
    CExoString sResRef;
    if (nPortraitId != 0xffff) {
        sPrefix = "po_";
        g_pRules->Get2DArrays()->GetPortraits()->GetCExoStringEntry(nPortraitId, "BaseResRef", &sResRef);
        sResRef = sPrefix + sResRef;
        m_cPortrait = sResRef.CStr();
    }
    m_nPortraitId = nPortraitId;
}
