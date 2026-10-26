// CSWCCreatureStats display strings (nwccreaturestats.o): class/level
// summary and alignment name.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(int n);                              // 0x005E5BC0
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern unsigned char g_nGender;                     // 0x0073ECD0

class CSWClass {
public:
    CExoString GetNameText();                       // 0x005BE0E0
    char pad_0x0[0x1a0];
};

class CSWRules {
public:
    char pad_0x0[0xac];
    CSWClass* m_lstClasses;                         // 0xac
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCCreatureStats_ClassInfo {
public:
    char pad_0x0[0x1c];
    unsigned char m_nClass;                         // 0x1c
    unsigned char m_nLevel;                         // 0x1d
    char pad_0x1e[0x2];
};

class CSWCCreatureStats {
public:
    CExoString GetClassDescription(char cSeparator);
    CExoString GetAlignmentName(int nUnused);

    unsigned char GetClass(unsigned char n)
    {
        if (n >= 2) {
            return 0xff;
        }
        return m_ClassInfo[n].m_nClass;
    }
    unsigned char GetClassLevel(unsigned char n)
    {
        if (n >= 2) {
            return 0;
        }
        return m_ClassInfo[n].m_nLevel;
    }
    CSWClass* GetClassObject(unsigned char n)
    {
        return &g_pRules->m_lstClasses[GetClass(n)];
    }
    unsigned char GetSimpleAlignment()
    {
        int nAlign;
        short nGoodEvil = m_nGoodEvil;
        if (nGoodEvil <= 40) {
            nAlign = 3;
        } else if (nGoodEvil >= 60) {
            nAlign = 2;
        } else {
            nAlign = 1;
        }
        return nAlign;
    }

    char pad_0x0[0x32];
    unsigned char m_nNumMultiClasses;               // 0x32
    char pad_0x33[0x80 - 0x33];
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0xd4 - 0x82];
    CSWCCreatureStats_ClassInfo m_ClassInfo[2];     // 0xd4
};

// Descriptive name: "Class (level)" for each class, separated by cSeparator.
// FUNCTION: SWKOTOR 0x00648470
CExoString CSWCCreatureStats::GetClassDescription(char cSeparator)
{
    CExoString sResult;
    char sSeparator[2];
    sSeparator[0] = 0;
    sSeparator[1] = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass) {
            sResult = sResult + sSeparator + pClass->GetNameText() + " (" + GetClassLevel(i) + ")";
            sSeparator[0] = cSeparator;
        }
    }
    return sResult;
}

// Descriptive name. Every alignment branch fetches string reference 0.
// FUNCTION: SWKOTOR 0x00648710
CExoString CSWCCreatureStats::GetAlignmentName(int nUnused)
{
    STR_RES strRes;
    unsigned long nStrRef = 0;
    switch (GetSimpleAlignment()) {
    case 2:
        nStrRef = 0;
        break;
    case 3:
        nStrRef = 0;
        break;
    }
    g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
    return strRes.sString;
}
