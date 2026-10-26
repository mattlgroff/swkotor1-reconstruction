// CSWCCreatureStats / CSWCLevelUpStats constructors and destructors
// (nwccreaturestats.o). The full polymorphic layout is isolated in this
// translation unit; the shared m/k copies keep the vtable inside padding.

#include <string.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef operator=(const char* s);               // 0x00406290
    char m_resRef[16];
};

class CSWPortrait {
public:
    CSWPortrait();                                  // 0x00678EC0
    ~CSWPortrait();                                 // 0x004015A0
    CResRef m_cBaseResRef;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = NULL;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = NULL;
    }
};

class CSWCombatInformation {
public:
    CSWCombatInformation();                         // 0x00552280
    ~CSWCombatInformation();                        // 0x00550E00
    char pad_0x0[0x48];
};

class CSWCFeatUses;

class CSWCCreatureStats_ClassInfo {
public:
    CSWCCreatureStats_ClassInfo();                  // 0x0064A050
    ~CSWCCreatureStats_ClassInfo();                 // 0x006495F0
    char pad_0x0[0x1c];
    unsigned char m_nClass;                         // 0x1c
    unsigned char m_nLevel;                         // 0x1d
    char pad_0x1e[0x2];
};

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;                     // 0xab
    char pad_0xac[0x4];
    struct CSWRace* m_lstRaces;                     // 0xb0
};

struct CSWRace {
    char pad_0x0[0x18];
    unsigned char m_nSTRAdjust;                     // 0x18
    unsigned char m_nDEXAdjust;                     // 0x19
    unsigned char m_nINTAdjust;                     // 0x1a
    unsigned char m_nCHAAdjust;                     // 0x1b
    unsigned char m_nWISAdjust;                     // 0x1c
    unsigned char m_nCONAdjust;                     // 0x1d
    char pad_0x1e[0x16];
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern const unsigned short RACIAL_TYPE_HUMAN;      // 0x0074BF14
extern const unsigned char CLASS_TYPE_SCOUT;        // 0x0074B319

class CSWCCreatureStats {
public:
    CSWCCreatureStats();
    virtual ~CSWCCreatureStats();
    void ClearFeats();                              // 0x00648320
    void SetClass(unsigned char nIndex, unsigned char nClass)
    {
        if (nIndex < 2) {
            m_ClassInfo[nIndex].m_nClass = nClass;
        }
    }
    void SetClassLevel(unsigned char nIndex, unsigned char nLevel)
    {
        if (nIndex < 2) {
            m_ClassInfo[nIndex].m_nLevel = nLevel;
        }
    }
    void InitializeCombatInformation();             // 0x00649D60

    CResRef m_cAppearanceResRef;                    // 0x04
    CExoString m_sFirstName;                        // 0x14
    CExoString m_sLastName;                         // 0x1c
    unsigned short m_nRace;                         // 0x24
    char pad_0x26[0x2];
    CExoString m_sSubRace;                          // 0x28
    unsigned char m_n30;                            // 0x30
    unsigned char m_n31;                            // 0x31
    unsigned char m_nNumMultiClasses;               // 0x32
    unsigned char m_n33;                            // 0x33
    unsigned char m_nSTR;                           // 0x34
    unsigned char m_nDEX;                           // 0x35
    unsigned char m_nCON;                           // 0x36
    unsigned char m_nINT;                           // 0x37
    unsigned char m_nWIS;                           // 0x38
    unsigned char m_nCHA;                           // 0x39
    unsigned char m_nSTRBase;                       // 0x3a
    unsigned char m_nDEXBase;                       // 0x3b
    unsigned char m_nCONBase;                       // 0x3c
    unsigned char m_nINTBase;                       // 0x3d
    unsigned char m_nWISBase;                       // 0x3e
    unsigned char m_nCHABase;                       // 0x3f
    unsigned char m_nArmorDexBonus;                 // 0x40
    unsigned char m_n41;                            // 0x41
    unsigned char m_n42;                            // 0x42
    unsigned char m_n43;                            // 0x43
    unsigned char m_n44;                            // 0x44
    unsigned char m_n45;                            // 0x45
    unsigned char m_n46;                            // 0x46
    char pad_0x47[0x1];
    short m_nBaseHitPoints;                         // 0x48
    short m_n4a;                                    // 0x4a
    short m_nCurrentHitPoints;                      // 0x4c
    short m_nMaxHitPoints;                          // 0x4e
    short m_nACNaturalBase;                         // 0x50
    unsigned char m_n52;                            // 0x52
    char pad_0x53[0x1];
    float m_fMovementRate;                          // 0x54
    int m_bIsPC;                                    // 0x58
    char m_nFortSave;                               // 0x5c
    char m_nWillSave;                               // 0x5d
    char m_nReflexSave;                             // 0x5e
    char pad_0x5f[0x1];
    int m_n60;                                      // 0x60
    unsigned char m_n64;                            // 0x64
    char pad_0x65[0x3];
    CSWCombatInformation* m_pCombatInformation;     // 0x68
    int m_bCombatInformationSet;                    // 0x6c
    CSWPortrait m_cPortrait;                        // 0x70
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0x2];
    char* m_pModifiedSkillRanks;                    // 0x84
    char* m_pSkillRanks;                            // 0x88
    unsigned short m_nAppearanceRace;               // 0x8c
    unsigned char m_n8e;                            // 0x8e
    unsigned char m_n8f;                            // 0x8f
    unsigned char m_n90;                            // 0x90
    unsigned char m_n91;                            // 0x91
    unsigned char m_n92;                            // 0x92
    unsigned char m_n93;                            // 0x93
    unsigned char m_n94;                            // 0x94
    char pad_0x95[0x3];
    int m_n98;                                      // 0x98
    unsigned char m_n9c;                            // 0x9c
    unsigned char m_n9d;                            // 0x9d
    char pad_0x9e[0x2];
    CExoString m_sDeity;                            // 0xa0
    int m_na8;                                      // 0xa8
    unsigned char m_nAbilityPoints;                 // 0xac
    char pad_0xad[0x1];
    unsigned short m_nSkillPoints;                  // 0xae
    CExoArrayList<unsigned short> m_lstFeats;       // 0xb0
    CExoArrayList<unsigned short> m_lstBonusFeats;  // 0xbc
    CExoArrayList<CSWCFeatUses*> m_lstFeatUses;     // 0xc8
    CSWCCreatureStats_ClassInfo m_ClassInfo[2];     // 0xd4
    CExoString m_s114;                              // 0x114
    unsigned short m_nPortraitId;                   // 0x11c
    unsigned short m_n11e;                          // 0x11e
    unsigned short m_n120;                          // 0x120
    unsigned short m_n122;                          // 0x122
    unsigned short m_n124;                          // 0x124
    char pad_0x126[0x2];
};

class CSWCLevelUpStats : public CSWCCreatureStats {
public:
    CSWCLevelUpStats();
    void SetSTRBase(unsigned char n)
    {
        m_nSTRBase = n;
        m_nSTR = g_pRules->m_lstRaces[m_nRace].m_nSTRAdjust + n;
        InitializeCombatInformation();
    }
    void SetINTBase(unsigned char n)
    {
        m_nINTBase = n;
        m_nINT = g_pRules->m_lstRaces[m_nRace].m_nINTAdjust + n;
    }
    void SetWISBase(unsigned char n)
    {
        m_nWISBase = n;
        m_nWIS = g_pRules->m_lstRaces[m_nRace].m_nWISAdjust + n;
        SetWillSavingThrow();
    }
    void SetCHABase(unsigned char n)
    {
        m_nCHABase = n;
        m_nCHA = g_pRules->m_lstRaces[m_nRace].m_nCHAAdjust + n;
    }
    void SetDEXBase(unsigned char nDEX, int bApply);    // 0x0064A100
    void SetCONBase(unsigned char nCON);            // 0x006496D0
    void SetWillSavingThrow();                      // 0x00648BE0

    CExoArrayList<unsigned short> m_lstAddedFeats;  // 0x128
};

// FUNCTION: SWKOTOR 0x0064AFC0
CSWCCreatureStats::CSWCCreatureStats()
{
    m_sFirstName = "";
    m_sLastName = "";
    m_nRace = RACIAL_TYPE_HUMAN;
    m_n33 = 0;
    m_na8 = 0;
    m_sSubRace = "";
    m_n30 = 0;
    m_cAppearanceResRef = "";
    m_n31 = 0;
    m_nNumMultiClasses = 3;
    SetClass(0, CLASS_TYPE_SCOUT);
    SetClassLevel(0, 1);
    SetClass(1, 0xff);
    SetClassLevel(1, 0);
    m_nNumMultiClasses = 1;
    m_nSTR = 8;
    m_nDEX = 8;
    m_nINT = 8;
    m_nWIS = 8;
    m_nCHA = 8;
    m_nCON = 8;
    m_nSTRBase = 0;
    m_nDEXBase = 0;
    m_nINTBase = 0;
    m_nWISBase = 0;
    m_nCHABase = 0;
    m_nCONBase = 0;
    m_nArmorDexBonus = 0;
    m_n41 = 0;
    m_n42 = 0;
    m_n43 = 0;
    m_n44 = 0;
    m_n45 = 0;
    m_n46 = 0;
    m_nACNaturalBase = 0;
    m_nBaseHitPoints = 0;
    m_n4a = 0;
    m_nMaxHitPoints = 0;
    m_nCurrentHitPoints = 0;
    m_n52 = 0;
    m_fMovementRate = 6.0f;
    m_bIsPC = 0;
    m_nFortSave = 0;
    m_nWillSave = 0;
    m_nReflexSave = 0;
    m_nGoodEvil = 50;
    m_n60 = 0;
    m_n64 = 0;
    m_nAppearanceRace = m_nRace;
    m_pModifiedSkillRanks = new char[g_pRules->m_nNumSkills];
    m_pSkillRanks = new char[g_pRules->m_nNumSkills];
    memset(m_pModifiedSkillRanks, 0, g_pRules->m_nNumSkills);
    memset(m_pSkillRanks, 0, g_pRules->m_nNumSkills);
    m_pCombatInformation = new CSWCombatInformation;
    m_bCombatInformationSet = 0;
    m_n11e = 0;
    m_n120 = 0;
    m_n122 = 0;
    m_n124 = 0;
    m_n8e = 0;
    m_n8f = 0;
    m_n90 = 0;
    m_n91 = 0;
    m_n92 = 0;
    m_n93 = 0;
    m_n9c = 0;
    m_n9d = 0;
    m_n98 = 0;
    m_nPortraitId = 0xffff;
    m_n94 = 0xff;
}

// FUNCTION: SWKOTOR 0x00649E40
CSWCCreatureStats::~CSWCCreatureStats()
{
    if (m_pModifiedSkillRanks) {
        delete[] m_pModifiedSkillRanks;
        m_pModifiedSkillRanks = NULL;
    }
    if (m_pSkillRanks) {
        delete[] m_pSkillRanks;
        m_pSkillRanks = NULL;
    }
    ClearFeats();
    if (m_pCombatInformation) {
        delete m_pCombatInformation;
        m_pCombatInformation = NULL;
    }
}

// FUNCTION: SWKOTOR 0x0064B2D0
CSWCLevelUpStats::CSWCLevelUpStats()
{
    m_nAbilityPoints = 30;
    m_nSkillPoints = 0;
    SetSTRBase(8);
    SetDEXBase(8, 0);
    SetINTBase(8);
    SetWISBase(8);
    SetCHABase(8);
    SetCONBase(8);
}

// The level-up destructor is compiler generated (no vptr reset); the
// constructor's vtable emits it together with both deleting destructors.
// FUNCTION: SWKOTOR 0x0064B400 SYMBOL: ??1CSWCLevelUpStats@@UAE@XZ
// FUNCTION: SWKOTOR 0x0064B3E0 SYMBOL: ??_GCSWCLevelUpStats@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0064B2B0 SYMBOL: ??_GCSWCCreatureStats@@UAEPAXI@Z
