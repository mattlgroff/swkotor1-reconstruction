// CSWSCreatureStats::CSWSCreatureStats (round s10, 0x005ACA80). Own copy of the
// stats layout: every member the constructor touches, at its offset.

#include <string.h>

class CSWSCreature;

class CExoString {
public:
    CExoString();
    ~CExoString();
    CExoString &operator=(const char *s);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef();
    CResRef operator=(const char *s);
    char m_resRef[16];
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int nSize);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class CSWLevelStats {
public:
    CSWLevelStats();
    char pad_0x0[0x30];
};

class CCombatInformation {
public:
    CCombatInformation();
    char pad_0x0[0x48];
};

class CSWSSpecialAbility {
public:
    unsigned long m_nSpell;
    int m_bReady;
    unsigned char m_nCasterLevel;
};

class CSWSClassInfo {
public:
    CSWSClassInfo();
    ~CSWSClassInfo();
    char pad_0x0[0x1b];
    unsigned char m_nClass;                // +0x1b
    unsigned char m_nLevel;                // +0x1c
    unsigned char m_nNegativeLevels;       // +0x1d
    char pad_0x1e[0xa];
};

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;      // +0xab
};

extern CSWRules *g_pRules;
extern const unsigned short RACIAL_TYPE_HUMAN;

class CSWSCreatureStats {
public:
    CSWSCreatureStats(CSWSCreature *pCreature);
    void SetClassLevel(unsigned char nMultiClass, unsigned char nLevel)
    {
        if (nMultiClass < m_nNumMultiClasses)
            m_ClassInfo[nMultiClass].m_nLevel = nLevel;
    }
    void SetClass(unsigned char nMultiClass, unsigned char nClass)
    {
        if (nMultiClass < m_nNumMultiClasses)
            m_ClassInfo[nMultiClass].m_nClass = nClass;
    }

    CExoArrayList<unsigned short> m_lFeats;            // +0x0
    CExoArrayList<void *> m_lFeatUses;                 // +0xc
    CExoArrayList<unsigned short> m_lBonusFeats;       // +0x18
    CSWSCreature *m_pBaseCreature;                     // +0x24
    CExoArrayList<CSWLevelStats *> m_lLevelStats;      // +0x28
    CExoLocString m_lsFirstName;     // +0x34
    CExoLocString m_lsLastName;      // +0x3c
    CResRef m_cConversation;         // +0x44
    int m_bInterruptable;            // +0x54
    CExoLocString m_lsDescription;   // +0x58
    int m_nAge;                      // +0x60
    unsigned char m_nGender;         // +0x64
    char pad_0x65[0x3];
    unsigned long m_nExperience;     // +0x68
    int m_bIsPC;                     // +0x6c
    int m_n70;                       // +0x70 (placeholder name)
    int m_n74;                       // +0x74 (placeholder name)
    int m_nFactionId;                // +0x78
    char pad_0x7c[0x8];
    float m_fChallengeRating;        // +0x84
    unsigned char m_nStartingPackage;  // +0x88
    unsigned char m_nNumMultiClasses;  // +0x89
    char pad_0x8a[0x2];
    CSWSClassInfo m_ClassInfo[2];    // +0x8c
    unsigned short m_nRace;          // +0xdc
    char pad_0xde[0x2];
    CExoString m_sSubrace;           // +0xe0
    unsigned char m_nSubraceIndex;   // +0xe8
    unsigned char m_nSTRBase;        // +0xe9
    char m_nSTRMod;                  // +0xea
    unsigned char m_nDEXBase;        // +0xeb
    char m_nDEXMod;                  // +0xec
    unsigned char m_nCONBase;        // +0xed
    char m_nCONMod;                  // +0xee
    unsigned char m_nINTBase;        // +0xef
    char m_nINTMod;                  // +0xf0
    unsigned char m_nWISBase;        // +0xf1
    char m_nWISMod;                  // +0xf2
    unsigned char m_nCHABase;        // +0xf3
    char m_nCHAMod;                  // +0xf4
    unsigned char m_nACNaturalBase;  // +0xf5
    unsigned char m_nF6;             // +0xf6 (placeholder names through +0x103)
    unsigned char m_nF7;             // +0xf7
    unsigned char m_nF8;             // +0xf8
    unsigned char m_nF9;             // +0xf9
    unsigned char m_nFA;             // +0xfa
    unsigned char m_nFB;             // +0xfb
    unsigned char m_nFC;             // +0xfc
    unsigned char m_nFD;             // +0xfd
    unsigned char m_nFE;             // +0xfe
    unsigned char m_nFF;             // +0xff
    unsigned char m_n100;            // +0x100
    unsigned char m_n101;            // +0x101
    unsigned char m_nBaseAttackBonusOverride;  // +0x102 (placeholder name)
    unsigned char m_n103;            // +0x103
    char pad_0x104[0x4];
    int m_n108;                      // +0x108 (placeholder name)
    int m_n10C;                      // +0x10c (placeholder name)
    int m_n110;                      // +0x110 (placeholder name)
    unsigned char m_n114;            // +0x114 (placeholder name)
    char pad_0x115[0x3];
    CCombatInformation *m_pCombatInformation;  // +0x118
    unsigned char m_n11C;            // +0x11c (placeholder name)
    unsigned char m_n11D;            // +0x11d (placeholder name)
    unsigned char m_n11E;            // +0x11e (placeholder name)
    unsigned char m_nForceResistance;         // +0x11f
    unsigned char m_nForceResistancePenalty;  // +0x120
    char pad_0x121;
    short m_nForcePoints;            // +0x122
    short m_nForcePointsBonus;       // +0x124
    short m_n126;                    // +0x126 (placeholder name)
    CExoArrayList<CSWSSpecialAbility> *m_pSpecialAbilities;  // +0x128
    unsigned short m_n12C;           // +0x12c (placeholder names through +0x15e)
    unsigned short m_n12E;           // +0x12e
    unsigned short m_n130;           // +0x130
    unsigned short m_n132;           // +0x132
    unsigned short m_n134;           // +0x134
    unsigned short m_n136;           // +0x136
    unsigned short m_n138;           // +0x138
    unsigned short m_nAIState;       // +0x13a
    unsigned short m_n13C;           // +0x13c
    unsigned short m_n13E;           // +0x13e
    unsigned short m_n140;           // +0x140
    unsigned short m_n142;           // +0x142
    unsigned short m_n144;           // +0x144
    unsigned short m_n146;           // +0x146
    unsigned short m_n148;           // +0x148
    unsigned short m_n14A;           // +0x14a
    unsigned short m_n14C;           // +0x14c
    unsigned short m_n14E;           // +0x14e
    unsigned short m_n150;           // +0x150
    unsigned short m_n152;           // +0x152
    unsigned short m_n154;           // +0x154
    unsigned short m_n156;           // +0x156
    unsigned short m_n158;           // +0x158
    unsigned short m_n15A;           // +0x15a
    unsigned short m_n15C;           // +0x15c
    unsigned short m_n15E;           // +0x15e
    char pad_0x160[0x4];
    unsigned short m_nSkillPoints;   // +0x164
    char pad_0x166[0x2];
    char *m_pSkillRanks;             // +0x168
    unsigned char m_n16C;            // +0x16c (placeholder name)
    unsigned char m_n16D;            // +0x16d (placeholder name)
    CResRef m_cResRef16E;            // +0x16e (placeholder name)
    short m_nGoodEvil;               // +0x17e
    char pad_0x180[0x2];
    unsigned char m_nColorSkin;      // +0x182
    unsigned char m_nColorHair;      // +0x183
    unsigned char m_nColorTattoo1;   // +0x184
    unsigned char m_nColorTattoo2;   // +0x185
    unsigned short m_nAppearanceType;  // +0x186
    unsigned char m_nPhenotype;      // +0x188
    unsigned char m_nAppearanceHead; // +0x189
    unsigned char m_nDuplicatingHead;  // +0x18a
    char pad_0x18b;
    int m_bUseBackupHead;            // +0x18c
    unsigned char m_nTail;           // +0x190
    unsigned char m_nWings;          // +0x191
    char pad_0x192[0x2];
    int m_nMovementRate;             // +0x194
    int m_n198;                      // +0x198 (placeholder name)
    int m_n19C;                      // +0x19c (placeholder name)
    char m_nFortSaveBonus;           // +0x1a0
    char m_nWillSaveBonus;           // +0x1a1
    char m_nReflexSaveBonus;         // +0x1a2
    char pad_0x1a3;
    CExoString m_sDeity;             // +0x1a4
    float m_fHPAccumulator;          // +0x1ac
    float m_fFPAccumulator;          // +0x1b0
};

// FUNCTION: SWKOTOR 0x005ACA80
CSWSCreatureStats::CSWSCreatureStats(CSWSCreature *pCreature)
{
    m_lLevelStats.Add(new CSWLevelStats);
    m_nNumMultiClasses = 1;
    m_nStartingPackage = 0;
    m_cConversation = "";
    m_bInterruptable = 1;
    m_bIsPC = 1;
    m_n70 = 0;
    m_n74 = 0;
    m_nGender = 0;
    m_nAge = 0;
    m_nRace = RACIAL_TYPE_HUMAN;
    m_sSubrace = "";
    m_nSubraceIndex = 0;
    SetClassLevel(0, 1);
    SetClass(0, 0);
    m_nNumMultiClasses = 2;
    for (unsigned char i = 1; i < 2; i++) {
        SetClassLevel(i, 0);
        SetClass(i, 0xff);
    }
    m_nNumMultiClasses = 1;
    m_nSkillPoints = 0;
    m_n16C = 0;
    m_n16D = 0;
    m_n114 = 0;
    m_nExperience = 0;
    m_nGoodEvil = 0;
    m_pBaseCreature = pCreature;
    m_n108 = 0;
    m_n10C = 20;
    m_n110 = 0;
    m_nF8 = 0;
    m_nF9 = 0;
    m_nFC = 0;
    m_nFD = 0;
    m_n100 = 0;
    m_n101 = 0;
    m_nFA = 0;
    m_nFB = 0;
    m_nFE = 0;
    m_nFF = 0;
    m_nACNaturalBase = 0;
    m_nF6 = 0;
    m_nF7 = 0;
    m_n103 = 0;
    m_n12E = 0;
    m_n130 = 0;
    m_n132 = 0;
    m_n134 = 0;
    m_n136 = 0;
    m_n138 = 0;
    m_n12C = 0;
    m_n13C = 0;
    m_n13E = 0;
    m_n140 = 0;
    m_n144 = 0;
    m_n142 = 0;
    m_n146 = 0;
    m_n148 = 0;
    m_n14A = 0;
    m_n14C = 0;
    m_n14E = 0;
    m_n150 = 0;
    m_n152 = 0;
    m_n154 = 0;
    m_n156 = 0;
    m_n158 = 0;
    m_n15A = 0;
    m_n15C = 0;
    m_n15E = 0;
    m_nBaseAttackBonusOverride = 0;
    m_pCombatInformation = new CCombatInformation;
    m_nAIState = 0;
    m_nMovementRate = 0;
    m_n19C = 0;
    m_n198 = 0;
    m_pSkillRanks = new char[g_pRules->m_nNumSkills];
    memset(m_pSkillRanks, 0, g_pRules->m_nNumSkills);
    m_n11C = 0;
    m_n11E = 0;
    m_n11D = 0;
    m_nForceResistance = 0;
    m_nForceResistancePenalty = 0;
    m_nSTRBase = 0;
    m_nSTRMod = 0;
    m_nDEXBase = 0;
    m_nDEXMod = 0;
    m_nCONBase = 0;
    m_nCONMod = 0;
    m_nINTBase = 0;
    m_nINTMod = 0;
    m_nWISBase = 0;
    m_nWISMod = 0;
    m_nCHABase = 0;
    m_nCHAMod = 0;
    m_nFortSaveBonus = 0;
    m_nWillSaveBonus = 0;
    m_nReflexSaveBonus = 0;
    m_cResRef16E = "";
    m_nColorSkin = 0;
    m_nColorHair = 0;
    m_nColorTattoo1 = 0;
    m_nColorTattoo2 = 0;
    m_nAppearanceType = 0;
    m_nPhenotype = 0;
    m_nAppearanceHead = 0;
    m_bUseBackupHead = 0;
    m_nDuplicatingHead = 0xff;
    m_nTail = 0;
    m_nWings = 0;
    m_nFactionId = -1;
    m_fChallengeRating = 0.0f;
    m_pSpecialAbilities = new CExoArrayList<CSWSSpecialAbility>;
    m_nForcePoints = 0;
    m_nForcePointsBonus = 0;
    m_n126 = 0;
    m_fHPAccumulator = 0.0f;
    m_fFPAccumulator = 0.0f;
}
