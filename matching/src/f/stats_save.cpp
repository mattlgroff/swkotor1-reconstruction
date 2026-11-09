// CSWSCreatureStats::SaveStats / SaveClassInfo (round s10, 0x005B1B90, 0x005AEC90).
// Own copy of the stats layout (names follow stats_levelup.cpp).

class CSWSCreature;
class CSWSCreatureStats;
class CSWSItem;

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *CStr() const;
    void Format(const char *szFormat, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    int GetString(int nLanguage, CExoString *psOut, unsigned char nGender);
    char pad_0x0[0x8];
};

class CResRef {
public:
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pElement, CResList *pList, unsigned long nType);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldCHAR(CResStruct *pStruct, char nValue, char *szLabel);
    int WriteFieldWORD(CResStruct *pStruct, unsigned short nValue, char *szLabel);
    int WriteFieldSHORT(CResStruct *pStruct, short nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
};

class CSWSObject;

class CSWSCreature {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual short GetMaxHitPoints(int bIncludeToughness);     // +0x98
    virtual short GetCurrentHitPoints(int bExcludeTemporary); // +0x9c
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual CResRef GetPortrait();                 // +0xc8
    virtual void vcc();
    virtual unsigned short GetPortraitId();        // +0xd0

    int GetGold();
    int GetTotalEffectBonus(unsigned char nType, CSWSObject *pVersus, int *pnOut, int a, int b,
                             int c, int d, unsigned char nSkill, unsigned char nAbility, int e);
    short GetArmorClass();
    short GetMaxForcePoints();
    unsigned short GetSoundSet() { return m_nSoundSet; }
    int GetPlot() { return m_bPlot; }
    int GetDisarmable() { return m_bDisarmable; }
    int GetMin1HP() { return m_bMin1HP; }
    int GetPartyInteract() { return m_bPartyInteract; }
    int GetReorienting() { return m_bReorienting; }
    unsigned char GetBodyBag() { return m_nBodyBag; }
    int GetHitPoints() { return m_nHitPoints; }
    int GetForcePoints();
    short GetForcePointsS();
    short GetHitPointsS() { return m_nHitPoints; }
    unsigned short GetHitPointsU() { return m_nHitPoints; }
    CSWSCreatureStats *GetStats() { return m_pStats; }
    unsigned short GetHitPointsDelta() { return GetHitPointsS() - GetMaxHitPoints(1); }
    int GetSavedCurrentHitPoints() { return GetHitPointsDelta() + GetCurrentHitPoints(1); }
    int GetSavedCurrentForce();

    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0xc0];
    short m_nHitPoints;              // +0xe0
    char pad_0xe2[0x16];
    int m_bPlot;                     // +0xf8
    char pad_0xfc[0x104];
    int m_bMin1HP;                   // +0x200
    char pad_0x204[0x14];
    int m_bPartyInteract;            // +0x218
    int m_bReorienting;              // +0x21c
    char pad_0x220[0x2d4];
    int m_bDisarmable;               // +0x4f4
    char pad_0x4f8[0x4e0];
    unsigned short m_nSoundSet;      // +0x9d8
    unsigned char m_nBodyBag;        // +0x9da
    char pad_0x9db[0x99];
    CSWSCreatureStats *m_pStats;     // +0xa74
};

class CSWBaseItem {
public:
    char pad_0x0[0x1c];
    unsigned char GetDamageDice() { return m_nDamageDice; }
    unsigned char GetDamageDie() { return m_nDamageDie; }
    unsigned char m_nDamageDice;     // +0x1c (placeholder name)
    unsigned char m_nDamageDie;      // +0x1d (placeholder name)
};

class CSWSItem {
public:
    char pad_0x0[0xc];
    int m_nBaseItem;                 // +0xc
    int GetBaseItemType() { return m_nBaseItem; }
    CExoLocString *GetName() { return &m_sName; }
    char pad_0x10[0x270];
    CExoLocString m_sName;           // +0x280
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int nBaseItem);
};

class CCombatInformation {
public:
    int SaveData(CResGFF *pGFF, CResStruct *pStruct);
    unsigned long GetRightEquipped() { return m_oidRightEquipped; }
    unsigned long GetLeftEquipped() { return m_oidLeftEquipped; }
    CExoString *GetLeftString() { return &m_sLeftString; }
    char pad_0x0[0x2c];
    unsigned long m_oidRightEquipped;   // +0x2c
    unsigned long m_oidLeftEquipped;    // +0x30
    CExoString m_sRightString;       // +0x34
    CExoString m_sLeftString;        // +0x3c
    unsigned char m_nDamageDice;     // +0x44
    unsigned char m_nDamageDie;      // +0x45
};

class CSWPartyTable {
public:
    char pad_0x0[0xf0];
    unsigned long m_n0F0;            // +0xf0 (placeholder name)
};

class CServerExoApp {
public:
    CSWSItem *GetItemByGameObjectID(unsigned long oid);
    CSWPartyTable *GetPartyTable();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWClass {
public:
    static int IsForceUserClass(int nClass);
};

class CSWLevelStats {
public:
    unsigned char GetSkillRank(unsigned short nSkill);
    unsigned char GetAbilityGain() { return m_nAbilityGain; }
    unsigned short GetSkillPoints() { return m_nSkillPoints; }
    unsigned char GetNumKnown() { return m_lKnown.num; }
    unsigned long GetKnown(unsigned char n) { return m_lKnown.element[n]; }
    unsigned char GetNumKnownRemove() { return m_lKnownRemove.num; }
    unsigned long GetKnownRemove(unsigned char n) { return m_lKnownRemove.element[n]; }
    unsigned short GetNumFeats() { return m_lFeats.num; }
    unsigned short GetFeat(unsigned short n) { return m_lFeats.element[n]; }
    unsigned char GetHitDie() { return m_nHitDie; }
    unsigned char GetForce() { return m_nForce; }
    unsigned char GetClass() { return m_nClass; }
    CExoArrayList<unsigned long> m_lKnown;        // +0x0
    CExoArrayList<unsigned long> m_lKnownRemove;  // +0xc
    CExoArrayList<unsigned short> m_lFeats;       // +0x18
    char pad_0x24[0x4];
    unsigned short m_nSkillPoints;   // +0x28
    unsigned char m_nAbilityGain;    // +0x2a
    unsigned char m_nHitDie;         // +0x2b
    unsigned char m_nClass;          // +0x2c
    unsigned char m_nForce;          // +0x2d
};

class CSWSSpecialAbility {
public:
    unsigned long m_nSpell;          // +0x0
    int m_bReady;                    // +0x4
    unsigned char m_nCasterLevel;    // +0x8
};

class CSWSClassInfo {
public:
    CExoArrayList<unsigned long> m_pKnownSpellList[1];  // +0x0
    char pad_0xc[0xd];
    unsigned char m_nSpellsPerDayLeft[1];  // +0x19
    unsigned char m_nSpellsPerDay[1];      // +0x1a
    unsigned char m_nClass;                // +0x1b
    unsigned char m_nLevel;                // +0x1c
    unsigned char m_nNegativeLevels;       // +0x1d
    char pad_0x1e[0xa];

    int GetNumberKnownSpells(unsigned char nSpellLevel)
    {
        if (nSpellLevel >= 1)
            return 0;
        return m_pKnownSpellList[nSpellLevel].num;
    }
    unsigned long GetKnownSpell(unsigned char nSpellLevel, unsigned char n)
    {
        if (nSpellLevel >= 1)
            return 0xffffffff;
        if (n >= m_pKnownSpellList[nSpellLevel].num)
            return 0xffffffff;
        return m_pKnownSpellList[nSpellLevel].element[n];
    }
    unsigned char GetSpellsPerDayLeft(unsigned char nSpellLevel)
    {
        if (nSpellLevel >= 1)
            return 0;
        return m_nSpellsPerDayLeft[nSpellLevel];
    }
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray;  // +0x34
    char pad_0x38[0x73];
    unsigned char m_nNumSkills;      // +0xab
    unsigned char GetNumSkills() { return m_nNumSkills; }
};

extern CSWRules *g_pRules;

class CSWSCreatureStats {
public:
    void SaveStats(CResGFF *pGFF, CResStruct *pStruct);
    void SaveClassInfo(CResGFF *pGFF, CResStruct *pStruct);
    unsigned char GetLevel(int bUseNegativeLevel);
    unsigned char GetBaseFortSavingThrow();
    unsigned char GetBaseWillSavingThrow();
    char GetReflexSavingThrow(int bExcludeEffects);
    unsigned char GetNumMultiClasses() { return m_nNumMultiClasses; }
    unsigned short GetSkillPoints() { return m_nSkillPoints; }
    unsigned char GetClass(unsigned char nMultiClass)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0xff;
        return m_ClassInfo[nMultiClass].m_nClass;
    }
    unsigned char GetClassLevel(unsigned char nMultiClass, int bUseNegativeLevel)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0;
        int nLevel = m_ClassInfo[nMultiClass].m_nLevel;
        int nNeg = 0;
        if (bUseNegativeLevel)
            nNeg = m_ClassInfo[nMultiClass].m_nNegativeLevels;
        nLevel -= nNeg;
        if (nLevel < 0)
            return 0;
        return nLevel;
    }
    unsigned char GetSpellsPerDayLeft(unsigned char nMultiClass, unsigned char nSpellLevel)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0;
        return m_ClassInfo[nMultiClass].GetSpellsPerDayLeft(nSpellLevel);
    }
    short GetNumberKnownSpells(unsigned char nMultiClass, unsigned char nSpellLevel)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0;
        return m_ClassInfo[nMultiClass].GetNumberKnownSpells(nSpellLevel);
    }
    unsigned long GetKnownSpell(unsigned char nMultiClass, unsigned char nSpellLevel,
                                unsigned char n)
    {
        if (nMultiClass >= m_nNumMultiClasses)
            return 0xffffffff;
        return m_ClassInfo[nMultiClass].GetKnownSpell(nSpellLevel, n);
    }
    CSWLevelStats *GetLevelStats(unsigned char n)
    {
        if (!m_bIsPC)
            return 0;
        if (n >= m_lLevelStats.num)
            return 0;
        return m_lLevelStats.element[n];
    }
    unsigned short GetFeat(unsigned short n)
    {
        if (n >= m_lFeats.num)
            return 0xffff;
        return m_lFeats.element[n];
    }
    char GetSkillRank(unsigned char nSkill);
    CSWSCreature *GetBaseCreature() { return m_pBaseCreature; }
    CCombatInformation *GetCombatInformation() { return m_pCombatInformation; }
    int GetForcePoints() { return m_nForcePoints; }
    short GetForcePointsShort() { return m_nForcePoints; }
    unsigned short GetForcePointsTotal() { return m_nForcePointsBonus + GetForcePointsShort(); }
    unsigned short GetForcePointsTotal2() { return GetForcePointsBonusS() + GetForcePointsShort(); }
    unsigned short GetForcePointsTotal3() { return m_nForcePointsBonus + this->GetForcePointsShort(); }
    short GetForcePointsBonusS() { return m_nForcePointsBonus; }
    unsigned short GetForcePointsBonusU() { return m_nForcePointsBonus; }
    char GetFortSavingThrow(int bExcludeEffects);
    char GetWillSavingThrow(int bExcludeEffects);
    int GetIsPC() { return m_bIsPC; }
    int GetInterruptable() { return m_bInterruptable; }
    int GetAge() { return m_nAge; }
    unsigned char GetGender() { return m_nGender; }
    unsigned short GetRace() { return m_nRace; }
    unsigned char GetSubraceIndex() { return m_nSubraceIndex; }
    unsigned char GetStartingPackage() { return m_nStartingPackage; }
    char GetWillSaveBonus() { return m_nWillSaveBonus; }
    char GetFortSaveBonus() { return m_nFortSaveBonus; }
    char GetReflexSaveBonus() { return m_nReflexSaveBonus; }
    unsigned char GetSTRBase() { return m_nSTRBase; }
    unsigned char GetDEXBase() { return m_nDEXBase; }
    unsigned char GetINTBase() { return m_nINTBase; }
    unsigned char GetWISBase() { return m_nWISBase; }
    unsigned char GetCONBase() { return m_nCONBase; }
    unsigned char GetCHABase() { return m_nCHABase; }
    unsigned char GetACNaturalBase() { return m_nACNaturalBase; }
    unsigned long GetExperience() { return m_nExperience; }
    int GetMovementRate() { return m_nMovementRate; }
    short GetGoodEvil() { return m_nGoodEvil; }
    unsigned char GetColorSkin() { return m_nColorSkin; }
    unsigned char GetColorHair() { return m_nColorHair; }
    unsigned char GetColorTattoo1() { return m_nColorTattoo1; }
    unsigned char GetColorTattoo2() { return m_nColorTattoo2; }
    unsigned char GetPhenotype() { return m_nPhenotype; }
    unsigned short GetAppearanceType() { return m_nAppearanceType; }
    unsigned char GetAppearanceHead() { return m_nAppearanceHead; }
    unsigned char GetDuplicatingHead() { return m_nDuplicatingHead; }
    int GetUseBackupHead() { return m_bUseBackupHead; }
    unsigned char GetTail() { return m_nTail; }
    unsigned char GetWings() { return m_nWings; }
    int GetFactionId() { return m_nFactionId; }
    float GetChallengeRating() { return m_fChallengeRating; }
    unsigned short GetAIState() { return m_nAIState; }

    CExoArrayList<unsigned short> m_lFeats;            // +0x0
    char pad_0xc[0x18];
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
    char pad_0x70[0x8];
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
    char pad_0xf6[0x22];
    CCombatInformation *m_pCombatInformation;  // +0x118
    char pad_0x11c[0x6];
    short m_nForcePoints;            // +0x122
    short m_nForcePointsBonus;       // +0x124
    char pad_0x126[0x2];
    CExoArrayList<CSWSSpecialAbility> *m_pSpecialAbilities;  // +0x128
    char pad_0x12c[0xe];
    unsigned short m_nAIState;       // +0x13a
    char pad_0x13c[0x28];
    unsigned short m_nSkillPoints;   // +0x164
    char pad_0x166[0x2];
    char *m_pSkillRanks;             // +0x168
    char pad_0x16c[0x12];
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
    char pad_0x198[0x8];
    char m_nFortSaveBonus;           // +0x1a0
    char m_nWillSaveBonus;           // +0x1a1
    char m_nReflexSaveBonus;         // +0x1a2
    char pad_0x1a3;
    CExoString m_sDeity;             // +0x1a4
};

inline int CSWSCreature::GetSavedCurrentForce()
{
    return m_pStats->GetForcePointsTotal() - GetMaxForcePoints();
}

inline int CSWSCreature::GetForcePoints()
{
    return m_pStats->m_nForcePoints;
}

inline short CSWSCreature::GetForcePointsS()
{
    return m_pStats->m_nForcePoints;
}

inline char CSWSCreatureStats::GetSkillRank(unsigned char nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills)
        return 0;
    int n = m_pSkillRanks[nSkill];
    if (n < 127 && n > -127)
        return n;
    if (n >= 127)
        return 127;
    return -127;
}

// FUNCTION: SWKOTOR 0x005AEC90
void CSWSCreatureStats::SaveClassInfo(CResGFF *pGFF, CResStruct *pStruct)
{
    CResStruct cLevel;
    CResStruct cKnown;
    CResStruct cClass;
    CResStruct cFeat;
    CResStruct cSpec;
    CResStruct cSkill;
    CExoString sKey;
    CResList cKnownList;
    CResList cSkillList;
    CResList cFeatList;
    CResList cSpellsList;
    CResList cLevelList;
    CResList cClassList;
    CResList cSpecList;
    unsigned char i;
    unsigned char j;

    pGFF->AddList(&cClassList, pStruct, "ClassList");
    for (i = 0; i < m_nNumMultiClasses; i++) {
        unsigned char nClass = GetClass(i);
        pGFF->AddListElement(&cClass, &cClassList, 2);
        pGFF->WriteFieldINT(&cClass, nClass, "Class");
        pGFF->WriteFieldSHORT(&cClass, GetClassLevel(i, 0), "ClassLevel");
        if (CSWClass::IsForceUserClass(GetClass(i))) {
            CResStruct cSpells;
            pGFF->AddList(&cSpellsList, &cClass, "SpellsPerDayList");
            pGFF->AddListElement(&cSpells, &cSpellsList, 0x4567);
            pGFF->WriteFieldBYTE(&cSpells, GetSpellsPerDayLeft(i, 0), "NumSpellsLeft");
        }
        for (unsigned char nSpellLevel = 0; nSpellLevel < 1; nSpellLevel++) {
            short nKnown = GetNumberKnownSpells(i, nSpellLevel);
            if (nKnown > 0) {
                sKey.Format("%s%d", "KnownList", nSpellLevel);
                pGFF->AddList(&cKnownList, &cClass, sKey.CStr());
                for (j = 0; j < nKnown; j++) {
                    pGFF->AddListElement(&cKnown, &cKnownList, 3);
                    pGFF->WriteFieldWORD(&cKnown, GetKnownSpell(i, nSpellLevel, j), "Spell");
                }
            }
        }
    }
    if (m_bIsPC && g_pAppManager->m_pServerExoApp->GetPartyTable()->m_n0F0 == 0xffffffff) {
        pGFF->AddList(&cLevelList, pStruct, "LvlStatList");
        for (i = 0; i < GetLevel(0); i++) {
            CSWLevelStats *pLevel = GetLevelStats(i);
            pGFF->AddListElement(&cLevel, &cLevelList, 0);
            if (i > 0 && pLevel->GetAbilityGain() != 6)
                pGFF->WriteFieldBYTE(&cLevel, pLevel->GetAbilityGain(), "LvlStatAbility");
            pGFF->WriteFieldBYTE(&cLevel, pLevel->GetHitDie(), "LvlStatHitDie");
            pGFF->WriteFieldBYTE(&cLevel, pLevel->GetForce(), "LvlStatForce");
            pGFF->WriteFieldBYTE(&cLevel, pLevel->GetClass(), "LvlStatClass");
            unsigned short nKnown = pLevel->GetNumKnown();
            if (nKnown) {
                CExoString sKnown;
                sKnown.Format("%s%d", "KnownList", 0);
                pGFF->AddList(&cKnownList, &cLevel, sKnown.CStr());
                for (j = 0; j < nKnown; j++) {
                    pGFF->AddListElement(&cKnown, &cKnownList, 0);
                    pGFF->WriteFieldWORD(&cKnown, pLevel->GetKnown(j), "Spell");
                }
            }
            unsigned short nRemove = pLevel->GetNumKnownRemove();
            if (nRemove) {
                CExoString sRemove;
                sRemove.Format("%s%d", "KnownRemoveList", 0);
                pGFF->AddList(&cKnownList, &cLevel, sRemove.CStr());
                for (j = 0; j < nRemove; j++) {
                    pGFF->AddListElement(&cKnown, &cKnownList, 0);
                    pGFF->WriteFieldWORD(&cKnown, pLevel->GetKnownRemove(j), "Spell");
                }
            }
            pGFF->WriteFieldWORD(&cLevel, pLevel->GetSkillPoints(), "SkillPoints");
            pGFF->AddList(&cSkillList, &cLevel, "SkillList");
            for (j = 0; j < g_pRules->GetNumSkills(); j++) {
                pGFF->AddListElement(&cSkill, &cSkillList, 0);
                pGFF->WriteFieldBYTE(&cSkill, pLevel->GetSkillRank(j), "Rank");
            }
            pGFF->AddList(&cFeatList, &cLevel, "FeatList");
            for (unsigned short k = 0; k < pLevel->GetNumFeats(); k++) {
                pGFF->AddListElement(&cFeat, &cFeatList, 0);
                pGFF->WriteFieldWORD(&cFeat, pLevel->GetFeat(k), "Feat");
            }
        }
    }
    pGFF->WriteFieldWORD(pStruct, GetSkillPoints(), "SkillPoints");
    pGFF->AddList(&cSkillList, pStruct, "SkillList");
    for (unsigned char nSkill = 0; nSkill < 8; nSkill++) {
        pGFF->AddListElement(&cSkill, &cSkillList, 0);
        pGFF->WriteFieldBYTE(&cSkill, GetSkillRank(nSkill), "Rank");
    }
    if (m_lFeats.num > 0) {
        pGFF->AddList(&cFeatList, pStruct, "FeatList");
        for (unsigned short k = 0; k < m_lFeats.num; k++) {
            pGFF->AddListElement(&cFeat, &cFeatList, 1);
            pGFF->WriteFieldWORD(&cFeat, GetFeat(k), "Feat");
        }
    }
    if (m_pSpecialAbilities->num > 0) {
        pGFF->AddList(&cSpecList, pStruct, "SpecAbilityList");
        for (int n = 0; n < m_pSpecialAbilities->num; n++) {
            pGFF->AddListElement(&cSpec, &cSpecList, 4);
            pGFF->WriteFieldWORD(&cSpec, m_pSpecialAbilities->element[n].m_nSpell, "Spell");
            pGFF->WriteFieldBYTE(&cSpec, m_pSpecialAbilities->element[n].m_bReady, "SpellFlags");
            pGFF->WriteFieldBYTE(&cSpec, m_pSpecialAbilities->element[n].m_nCasterLevel, "SpellCasterLevel");
        }
    }
}

// Copies of 0x005AB810 / 0x005AB880 (annotated in stats_getters.cpp): the
// original inlines both into SaveStats.
inline char CSWSCreatureStats::GetFortSavingThrow(int bExcludeEffects)
{
    int nEffects = 0;
    if (bExcludeEffects == 0) {
        int n = 0;
        nEffects = GetBaseCreature()->GetTotalEffectBonus(3, 0, &n, 0, 0, 1, 0, 0xff, 0xff, 0);
    }
    char nMod = m_nCONMod;
    char nMisc = m_nFortSaveBonus;
    return nMod + GetBaseFortSavingThrow() + nEffects + nMisc;
}

inline char CSWSCreatureStats::GetWillSavingThrow(int bExcludeEffects)
{
    int nEffects = 0;
    if (bExcludeEffects == 0) {
        int n = 0;
        nEffects = GetBaseCreature()->GetTotalEffectBonus(3, 0, &n, 0, 0, 3, 0, 0xff, 0xff, 0);
    }
    char nMod = m_nWISMod;
    char nMisc = m_nWillSaveBonus;
    return nMod + GetBaseWillSavingThrow() + nEffects + nMisc;
}

// NONMATCH (73 by count, 3 real spots): (1) CurrentHitPoints: the original
// spills the short hit points after `push 1; mov ecx, ebx` (value in ax);
// (2) CurrentForce: the original copies the stats pointer (`mov eax, ebx`)
// before the inlined force-points getter; (3) the left-weapon GetString
// computes `add ecx, 0x3c` before `push 0`. Everything else lines up.
// FUNCTION: SWKOTOR 0x005B1B90
void CSWSCreatureStats::SaveStats(CResGFF *pGFF, CResStruct *pStruct)
{
    pGFF->WriteFieldCExoLocString(pStruct, m_lsFirstName, "FirstName");
    pGFF->WriteFieldCExoLocString(pStruct, m_lsLastName, "LastName");
    pGFF->WriteFieldCExoLocString(pStruct, m_lsDescription, "Description");
    pGFF->WriteFieldBYTE(pStruct, GetIsPC(), "IsPC");
    pGFF->WriteFieldCExoString(pStruct, m_pBaseCreature->m_sTag, "Tag");
    pGFF->WriteFieldCResRef(pStruct, CResRef(m_cConversation), "Conversation");
    pGFF->WriteFieldBYTE(pStruct, GetInterruptable(), "Interruptable");
    pGFF->WriteFieldINT(pStruct, GetAge(), "Age");
    pGFF->WriteFieldBYTE(pStruct, GetGender(), "Gender");
    pGFF->WriteFieldBYTE(pStruct, GetRace(), "Race");
    pGFF->WriteFieldCExoString(pStruct, m_sSubrace, "Subrace");
    pGFF->WriteFieldBYTE(pStruct, GetSubraceIndex(), "SubraceIndex");
    pGFF->WriteFieldBYTE(pStruct, GetStartingPackage(), "StartingPackage");
    pGFF->WriteFieldCExoString(pStruct, m_sDeity, "Deity");
    pGFF->WriteFieldBYTE(pStruct, GetNumMultiClasses() - 1, "MClassLevUpIn");
    pGFF->WriteFieldSHORT(pStruct, GetWillSaveBonus(), "willbonus");
    pGFF->WriteFieldSHORT(pStruct, GetFortSaveBonus(), "fortbonus");
    pGFF->WriteFieldSHORT(pStruct, GetReflexSaveBonus(), "refbonus");
    pGFF->WriteFieldDWORD(pStruct, m_pBaseCreature->GetGold(), "Gold");
    pGFF->WriteFieldCHAR(pStruct, GetReflexSavingThrow(0), "RefSaveThrow");
    pGFF->WriteFieldCHAR(pStruct, GetWillSavingThrow(0), "WillSaveThrow");
    pGFF->WriteFieldCHAR(pStruct, GetFortSavingThrow(0), "FortSaveThrow");
    pGFF->WriteFieldSHORT(pStruct, m_pBaseCreature->GetArmorClass(), "ArmorClass");
    pGFF->WriteFieldBYTE(pStruct, GetSTRBase(), "Str");
    pGFF->WriteFieldBYTE(pStruct, GetDEXBase(), "Dex");
    pGFF->WriteFieldBYTE(pStruct, GetINTBase(), "Int");
    pGFF->WriteFieldBYTE(pStruct, GetWISBase(), "Wis");
    pGFF->WriteFieldBYTE(pStruct, GetCONBase(), "Con");
    pGFF->WriteFieldBYTE(pStruct, GetCHABase(), "Cha");
    pGFF->WriteFieldBYTE(pStruct, m_nACNaturalBase, "NaturalAC");
    pGFF->WriteFieldWORD(pStruct, GetBaseCreature()->GetSoundSet(), "SoundSetFile");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetPlot(), "Plot");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetDisarmable(), "Disarmable");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetMin1HP(), "Min1HP");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetPartyInteract(), "PartyInteract");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetReorienting() == 0, "NotReorienting");
    pGFF->WriteFieldBYTE(pStruct, GetBaseCreature()->GetBodyBag(), "BodyBag");
    pGFF->WriteFieldSHORT(pStruct, GetBaseCreature()->GetHitPoints(), "HitPoints");
    short nCurrentHP = m_pBaseCreature->GetSavedCurrentHitPoints();
    pGFF->WriteFieldSHORT(pStruct, nCurrentHP, "CurrentHitPoints");
    pGFF->WriteFieldSHORT(pStruct, m_pBaseCreature->GetMaxHitPoints(1), "MaxHitPoints");
    pGFF->WriteFieldSHORT(pStruct, m_pBaseCreature->GetCurrentHitPoints(1), "PregameCurrent");
    pGFF->WriteFieldSHORT(pStruct, GetBaseCreature()->GetStats()->GetForcePointsShort(), "ForcePoints");
    short nCurrentFP = m_pBaseCreature->GetSavedCurrentForce();
    pGFF->WriteFieldSHORT(pStruct, nCurrentFP, "CurrentForce");
    pGFF->WriteFieldSHORT(pStruct, m_pBaseCreature->GetMaxForcePoints(), "MaxForcePoints");
    pGFF->WriteFieldDWORD(pStruct, GetExperience(), "Experience");
    pGFF->WriteFieldBYTE(pStruct, GetMovementRate(), "MovementRate");
    if (m_pBaseCreature->GetPortraitId() == 0xffff)
        pGFF->WriteFieldCResRef(pStruct, m_pBaseCreature->GetPortrait(), "Portrait");
    else
        pGFF->WriteFieldWORD(pStruct, m_pBaseCreature->GetPortraitId(), "PortraitId");
    pGFF->WriteFieldBYTE(pStruct, GetGoodEvil(), "GoodEvil");
    pGFF->WriteFieldBYTE(pStruct, GetColorSkin(), "Color_Skin");
    pGFF->WriteFieldBYTE(pStruct, GetColorHair(), "Color_Hair");
    pGFF->WriteFieldBYTE(pStruct, GetColorTattoo1(), "Color_Tattoo1");
    pGFF->WriteFieldBYTE(pStruct, GetColorTattoo2(), "Color_Tattoo2");
    pGFF->WriteFieldINT(pStruct, GetPhenotype(), "Phenotype");
    pGFF->WriteFieldWORD(pStruct, GetAppearanceType(), "Appearance_Type");
    pGFF->WriteFieldBYTE(pStruct, GetAppearanceHead(), "Appearance_Head");
    pGFF->WriteFieldBYTE(pStruct, GetDuplicatingHead(), "DuplicatingHead");
    pGFF->WriteFieldBYTE(pStruct, GetUseBackupHead(), "UseBackupHead");
    pGFF->WriteFieldBYTE(pStruct, GetTail(), "Tail");
    pGFF->WriteFieldBYTE(pStruct, GetWings(), "Wings");
    pGFF->WriteFieldWORD(pStruct, GetFactionId(), "FactionID");
    pGFF->WriteFieldFLOAT(pStruct, GetChallengeRating(), "ChallengeRating");
    pGFF->WriteFieldINT(pStruct, m_nAIState, "AIState");
    SaveClassInfo(pGFF, pStruct);
    CExoString sName;
    CSWSItem *pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(
        GetCombatInformation()->GetLeftEquipped());
    if (pItem)
        pItem->m_sName.GetString(0, GetCombatInformation()->GetLeftString(), 0);
    pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(
        GetCombatInformation()->GetRightEquipped());
    if (pItem) {
        pItem->m_sName.GetString(0, &GetCombatInformation()->m_sRightString, 0);
        CSWBaseItem *pBase = g_pRules->m_pBaseItemArray->GetBaseItem(pItem->GetBaseItemType());
        if (pBase) {
            m_pCombatInformation->m_nDamageDice = pBase->GetDamageDice();
            m_pCombatInformation->m_nDamageDie = pBase->GetDamageDie();
        }
    }
    m_pCombatInformation->SaveData(pGFF, pStruct);
}
