// Worker M: CSWPortrait (nwportrait.o) and the portrait / name members of
// CSWCCreatureStats (nwccreaturestats.o). Self-contained declarations.
#ifndef M_SW_PORTRAIT_H
#define M_SW_PORTRAIT_H

#include <stddef.h>

#include "shared/exo_string.h"

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const char* s);               // 0x00406290
    CResRef operator=(const CExoString& s);         // 0x004061F0
    CResRef operator+=(const CExoString& s);        // 0x00406300
    char* GetResRef() const;                        // 0x00641DB0

    char m_resRef[16];
};

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize); // 0x00408BC0
};

#include "shared/globals.h"

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn,
                           CExoString* pValue);     // 0x00413270
};

class C2DAs {
public:
    C2DA* GetPortraits() { return m_pPortraits; }

    char pad_0x0[0x38];
    C2DA* m_pPortraits;                             // 0x38
};

class CSWRules {
public:
    C2DAs* Get2DArrays() { return m_p2DArrays; }

    char pad_0x0[0x20];
    float m_fCreatureSizeTiny;                      // 0x20 ("T" / "P")
    float m_fCreatureSizeSmall;                     // 0x24
    float m_fCreatureSizeMedium;                    // 0x28
    float m_fCreatureSizeLarge;                     // 0x2c
    float m_fCreatureSizeHuge;                      // 0x30 ("W")
    char pad_0x34[0xb8 - 0x34];
    C2DAs* m_p2DArrays;                             // 0xb8
};


class CClientOptions {
public:
    char pad_0x0[0x72];
    unsigned char m_nGoreLevel;                     // 0x72 (0 selects LowGore)
};

class CClientExoApp {
public:
    CClientOptions* GetClientOptions();             // 0x005ED700
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
};


class CSWPortrait {
public:
    CSWPortrait();
    ~CSWPortrait();                                 // 0x004015A0
    int SetPortraitId(unsigned short nId);
    CResRef GetPortrait(unsigned char nSize);

    CResRef m_cBaseResRef;                          // 0x00
};

class CSWCCreatureStats {
public:
    CExoString GetFullName();
    void SetPortraitId(unsigned short nId);
    static int CheckCustomPortrait(CResRef* pResRef, int bFemale);
    CResRef GetPortrait(int bUseAlignment);
    float GetCreatureSize(CExoString* pSize);

    char pad_0x0[0x14];
    CExoString m_sFirstName;                        // 0x14
    CExoString m_sLastName;                         // 0x1c
    char pad_0x24[0x70 - 0x24];
    CResRef m_cPortrait;                            // 0x70
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0x11c - 0x82];
    unsigned short m_nPortraitId;                   // 0x11c
};

#endif
