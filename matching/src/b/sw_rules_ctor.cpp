// CSWRules::CSWRules: loads the rule tables (ranges, 2DA set, spells,
// feats, base items, experience table, skills, classes, races,
// difficulty). Self-contained layout, as in sw_rules_destructor.cpp.
#include "b/res_gff.h"

extern "C" void* memset(void* pDest, int nValue, unsigned int nCount);
#pragma intrinsic(memset)

class CTwoDimArrays {
public:
    CTwoDimArrays();                                        // 0x005BFB60
    int Load2DArrays();                                     // 0x005C4C20
    char pad_0x0[0x12c];
};

class CSWSpellArray {
public:
    CSWSpellArray();                                        // folded at 0x005B3190
    int Load();                                             // 0x0059BA20
    int m_nNumSpells;                                       // 0x0
    void* m_pSpells;                                        // 0x4
};

class CSWBaseItemArray {
public:
    CSWBaseItemArray();                                     // folded at 0x005B3190
    int Load();                                             // 0x005B31D0
    int m_nNumBaseItems;                                    // 0x0
    void* m_pBaseItems;                                     // 0x4
};

// vtable 0x00748BB4
class CSWRules {
public:
    CSWRules();
    virtual ~CSWRules();                                    // 0x00551440
    void LoadFeatInfo();                                    // 0x005515E0
    void LoadSkillInfo();                                   // 0x00550590
    void LoadClassInfo();                                   // 0x0054F2C0
    void LoadRaceInfo();                                    // 0x0054FEC0
    void LoadDifficultyInfo();                              // 0x00550C50

    float m_fPrimaryRange[6];                               // 0x04
    float m_fSecondaryRange[6];                             // 0x1c
    CSWBaseItemArray* m_pBaseItemArray;                     // 0x34
    int m_nExperienceTable[21];                             // 0x38
    CSWSpellArray* m_pSpellArray;                           // 0x8c
    void* m_pFeats;                                         // 0x90
    void* m_p94;                                            // 0x94
    void* m_p98;                                            // 0x98
    void* m_p9C;                                            // 0x9c
    void* m_pA0;                                            // 0xa0
    unsigned short m_nNumFeats;                             // 0xa4
    char pad_0xa6[0xa9 - 0xa6];
    unsigned char m_nNumClasses;                            // 0xa9
    unsigned char m_nNumRaces;                              // 0xaa
    unsigned char m_nNumSkills;                             // 0xab
    void* m_pClasses;                                       // 0xac
    void* m_pRaces;                                         // 0xb0
    void* m_pSkills;                                        // 0xb4
    CTwoDimArrays* m_p2DArrays;                             // 0xb8
    char pad_0xbc[0x120 - 0xbc];
};

// FUNCTION: SWKOTOR 0x00552C50
CSWRules::CSWRules()
{
    m_pSpellArray = NULL;
    m_nNumFeats = 0;
    m_pFeats = NULL;
    m_pA0 = NULL;
    m_pBaseItemArray = NULL;
    memset(m_nExperienceTable, 0, sizeof(m_nExperienceTable));
    m_nNumClasses = 0;
    m_pClasses = NULL;
    m_nNumRaces = 0;
    m_pRaces = NULL;
    m_nNumSkills = 0;
    m_pSkills = NULL;
    m_p94 = NULL;
    m_p98 = NULL;
    m_p9C = NULL;

    float fValue;
    C2DA* pRanges = new C2DA("Ranges", 0);
    pRanges->Load2DArray();
    for (int i = 0; i < 6; i++) {
        if (i != 5) {
            m_fPrimaryRange[i] = 0.0f;
            if (pRanges->GetFLOATEntry(i, "PrimaryRange", &fValue)) {
                m_fPrimaryRange[i] = fValue;
            }
            m_fSecondaryRange[i] = 0.0f;
            if (pRanges->GetFLOATEntry(i, "SecondaryRange", &fValue)) {
                m_fSecondaryRange[i] = fValue;
            }
        } else {
            m_fPrimaryRange[5] = 0.0f;
            if (pRanges->GetFLOATEntry(0x13, "PrimaryRange", &fValue)) {
                m_fPrimaryRange[5] = fValue;
            }
            m_fSecondaryRange[5] = 0.0f;
            if (pRanges->GetFLOATEntry(0x13, "SecondaryRange", &fValue)) {
                m_fSecondaryRange[5] = fValue;
            }
        }
    }
    if (pRanges) {
        delete pRanges;
    }

    m_p2DArrays = new CTwoDimArrays;
    m_p2DArrays->Load2DArrays();
    m_pSpellArray = new CSWSpellArray;
    m_pSpellArray->Load();
    LoadFeatInfo();
    m_pBaseItemArray = new CSWBaseItemArray;
    m_pBaseItemArray->Load();

    int nXP;
    C2DA* pExpTable = new C2DA("EXPTABLE", 0);
    pExpTable->Load2DArray();
    for (int nLevel = 0; nLevel <= 20; nLevel++) {
        pExpTable->GetINTEntry(nLevel, "XP", &nXP);
        m_nExperienceTable[nLevel] = nXP;
    }
    if (pExpTable) {
        delete pExpTable;
    }
    LoadSkillInfo();
    LoadClassInfo();
    LoadRaceInfo();
    LoadDifficultyInfo();
}
