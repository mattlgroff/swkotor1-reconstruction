// CSWSCreatureStats small getters / setters (0x005A4CF0-0x005AFAE0).

#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

class CSWSObject;
class CGameEffect;

double floor(double);

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                n++;
        return n;
    }
    void Allocate(int nSize);
    void Remove(T t);
    void AddUnique(T t);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void DelIndex(int i)
    {
        num = num - 1;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    void SetSize0()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
};

class CSWFeat {
public:
    char pad_0x0[0x33];
    unsigned char m_nUsesPerDay;     // +0x33
};

class CSWSArea {
public:
    char pad_0x0[0x124];
    unsigned long m_nTalentMask;     // +0x124
};

class CSWClass {
public:
    static int IsForceUserClass(int nClass);
    int IsNormalFeat(unsigned short nFeat);
    int IsClassSkill(unsigned short nSkill);
    int IsBonusFeat(unsigned short nFeat);
    char GetFortSaveBonus(unsigned char nLevel);
    char GetReflexSaveBonus(unsigned char nLevel);
    char GetWillSaveBonus(unsigned char nLevel);
    char pad_0x0[0x13c];
    unsigned char m_anBonusFeatsGained[20];   // +0x13c
    unsigned char m_anNormalFeatsGained[20];  // +0x150
    char pad_0x164[0x3c];
};

class CSWSkill {
public:
    char pad_0x0[0x1c];
    unsigned char m_nFlags;          // +0x1c
    char pad_0x1d[0x3];
};

class CSWRace {
public:
    char pad_0x0[0x18];
    char m_nSTRAdjust;               // +0x18
    char m_nDEXAdjust;               // +0x19
    char m_nINTAdjust;               // +0x1a
    char m_nCHAAdjust;               // +0x1b
    char m_nWISAdjust;               // +0x1c
    char m_nCONAdjust;               // +0x1d
    char pad_0x1e[0x16];
};

class CSWInventory;

class CSWSCreatureStats;

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
    void SetCurrentHitPoints(int nHP);
    int GetTotalEffectBonus(unsigned char nType, CSWSObject *pVersus, int *pnOut, int a, int b,
                             int c, int d, unsigned char nSkill, unsigned char nAbility, int e);
    int GetTotalAbilityBonus(unsigned char nAbility)
    {
        int n = 0;
        return GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, nAbility, 0);
    }
    int GetIsDroid();
    int IsDebilitated();
    CSWSArea *GetArea();
    void UpdateClassInfo();

    char pad_0x4[0x4f4];
    int m_nCreatureSize;             // +0x4f8
    char pad_0x4fc[0xa2c - 0x4fc];
    CSWInventory *m_pInventory;      // +0xa2c
    char pad_0xa30[0xa74 - 0xa30];
    CSWSCreatureStats *m_pStats;     // +0xa74
    char pad_0xa78[0x10];
    int m_bForceAdjust;              // +0xa88
};

class CSWBaseItem {
public:
    char pad_0x0[0x8];
    unsigned char m_nWeaponWield;    // +0x8
    char pad_0x9[0xa4];
    unsigned char m_nMaxDEXBonus;    // +0xad
};

class CSWSItem {
public:
    int ComputeArmorClass();
    CSWBaseItem *GetBaseItemData();
    int GetIsFinesseWeapon();
};

class CSWSpell {
public:
    int GetAdjustedForcePoints(unsigned long nLevel, int bAdjust);
};

class CSWSpellArray {
public:
    CSWSpell *GetSpell(int nSpell);
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CFeatUseListEntry {
public:
    unsigned short m_nFeat;
    unsigned char m_nUsedToday;
};

class CSWSMemorizedSpell {
public:
    unsigned long m_nSpell;
    int m_bReady;
    unsigned char m_nClass;
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

    void AddKnownSpell(unsigned char nSpellLevel, unsigned long nSpell);
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

class CSWSCreatureStats {
public:
    char GetTotalSTRBonus();
    char GetTotalDEXBonus();
    char GetTotalCONBonus();
    char GetTotalINTBonus();
    char GetTotalWISBonus();
    char GetTotalCHABonus();
    unsigned char GetClass(unsigned char nMultiClass);
    unsigned char GetClassLevel(unsigned char nMultiClass, int bUseNegativeLevel);
    unsigned char GetClassNegativeLevels(unsigned char nMultiClass);
    void SetClass(unsigned char nMultiClass, unsigned char nClass);
    void SetClassNegativeLevels(unsigned char nMultiClass, unsigned char nLevels);
    char CalcStatModifier(unsigned char nStat);
    unsigned long GetExpNeededForNextLevel();
    void SetSkillRank(unsigned char nSkill, char nRank);
    unsigned char GetSpellsPerDayLeft(unsigned char nMultiClass, unsigned char nSpellLevel);
    int GetForceResistance();
    unsigned char GetACNaturalBase();
    short AdjustHPAccumulatorBy(float f);
    short AdjustFPAccumulatorBy(float f);
    void AddClass(unsigned char nClass, unsigned char nLevel);
    int GetSTRStat();
    int GetDEXStat();
    int GetCONStat();
    int GetINTStat();
    int GetWISStat();
    int GetCHAStat();
    void SetSTRBase(unsigned char n);
    void SetDEXBase(unsigned char n);
    void SetINTBase(unsigned char n);
    void SetWISBase(unsigned char n);
    void SetCHABase(unsigned char n);
    void SetCONBase(unsigned char n, int bAdjustHP);
    int HasFeat(unsigned short nFeat);
    unsigned short GetFeat(unsigned short n);
    unsigned short GetBonusFeat(unsigned short n);
    unsigned long GetKnownSpell(unsigned char nMultiClass, unsigned char nSpellLevel,
                                unsigned char n);
    unsigned short GetNumberKnownSpells(unsigned char nMultiClass, unsigned char nSpellLevel);
    void AdjustSpellUsesPerDay();
    void UpdateForceResistance();
    void SetForceResistance(unsigned char n);
    void SetForceResistancePenalty(unsigned char n);
    int GetUseMonkAttackTables(int bRanged);
    unsigned char ReturnTrue();
    unsigned char GetLevel(int bUseNegativeLevel);
    unsigned long GetExpNeededForLevelUp();
    int GetIsJedi();
    CGameEffect *GetEffect(unsigned char n);
    void DecrementFeatRemainingUses(unsigned short nFeat);
    void SetMemorizedSpellReady(unsigned long nSpell, unsigned char bReady, unsigned char nClass);
    int HasReadySpell(unsigned long nSpell);
    unsigned char GetSimpleAlignmentGoodEvil();
    int HasSpell(unsigned char nSpellLevel, unsigned long nSpell, int bCheckForcePoints);
    int GetCanUseSkill(unsigned char nSkill);
    CSWSCreature *GetBaseCreature();
    unsigned char GetMemorizedSpellClass(unsigned long nSpell);
    void SetNormalBonusFlags(unsigned short nFeat, int *pbNormal, int *pbBonus);
    int HasFeatInList(unsigned short nFeat, CExoArrayList<unsigned short> *pList);
    unsigned char ComputeNumberKnownSpellsLeft(unsigned char nMultiClass);
    void GetFeatGainsForLevelUp(int nUnused, unsigned char *pnNormal, unsigned char *pnBonus);
    unsigned char GetFeatRemainingUses(unsigned short nFeat);
    void SetGoodEvil(short n)
    {
        if (n > 100)
            n = 100;
        else if (n < 0)
            n = 0;
        m_nGoodEvil = n;
    }
    void ModifyAlignment(short nDelta);
    unsigned char GetTotalACFeatMod();
    int CheckTalentMask(unsigned long nMask, unsigned long nWanted);
    int GetCreatureHasTalent(int nType, int nId);
    int CheckPowerSuitability(int nPower, CSWSArea *pArea, unsigned long nMask, int a4, int a5,
                              int a6, int a7, int nExclude);
    int GetCreatureTalentRandomPowerForClass(int a1, int a2, int a3, int a4, int a5,
                                             unsigned char nMultiClass, int a6, int a7);
    int GetCreatureTalentRandomPower(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
    void AddKnownSpell(unsigned char nMultiClass, unsigned long nSpell);
    void AddFeat(unsigned short nFeat);
    void RemoveFeat(unsigned short nFeat);
    void ClearFeats();
    char GetDEXMod(int bArmorDexCap);
    int GetWeaponFinesse(CSWSItem *pWeapon);
    unsigned char GetSpellUsesLeft(unsigned long nSpell, unsigned char nMultiClass,
                                   unsigned char nSpellLevel, unsigned char nMetaType);
    int UnReadySpell(unsigned long nSpell);
    unsigned char GetBaseFortSavingThrow();
    unsigned char GetBaseWillSavingThrow();
    unsigned char GetBaseReflexSavingThrow();
    char GetFortSavingThrow(int bExcludeEffects);
    char GetWillSavingThrow(int bExcludeEffects);
    char GetReflexSavingThrow(int bExcludeEffects);
    unsigned char GetUnarmedDamageDice();
    unsigned char GetUnarmedDamageDie();

    CExoArrayList<unsigned short> m_lFeats;            // +0x0
    CExoArrayList<CFeatUseListEntry *> m_lFeatUses;    // +0xc
    CExoArrayList<unsigned short> m_lBonusFeats;       // +0x18
    CSWSCreature *m_pBaseCreature;                     // +0x24
    CExoArrayList<CGameEffect *> m_lEffects;           // +0x28
    char pad_0x34[0x34];
    unsigned long m_nExperience;     // +0x68
    int m_bEffectsValid;             // +0x6c
    char pad_0x70[0x19];
    unsigned char m_nNumMultiClasses;  // +0x89
    char pad_0x8a[0x2];
    CSWSClassInfo m_ClassInfo[2];    // +0x8c
    unsigned short m_nRace;          // +0xdc
    char pad_0xde[0xb];
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
    char pad_0xf6[0x29];
    unsigned char m_nForceResistance;         // +0x11f
    unsigned char m_nForceResistancePenalty;  // +0x120
    char pad_0x121[0x3];
    short m_nForcePointsBonus;       // +0x124
    short m_nForcePoints;            // +0x126
    CExoArrayList<CSWSMemorizedSpell> *m_pMemorizedSpells;  // +0x128
    char pad_0x12c[0x3c];
    char *m_pSkillRanks;             // +0x168
    char pad_0x16c[0x12];
    short m_nGoodEvil;               // +0x17e
    char pad_0x180[0x20];
    char m_nFortSaveBonus;           // +0x1a0
    char m_nWillSaveBonus;           // +0x1a1
    char m_nReflexSaveBonus;         // +0x1a2
    char pad_0x1a3[0x9];
    float m_fHPAccumulator;          // +0x1ac
    float m_fFPAccumulator;          // +0x1b0
};

class CSWRules {
public:
    char pad_0x0[0x38];
    unsigned long m_anExperienceTable[0x15];  // +0x38
    CSWSpellArray *m_pSpellArray;    // +0x8c
    char pad_0x90[0x1b];
    unsigned char m_nNumSkills;      // +0xab
    CSWClass *m_pClasses;            // +0xac
    CSWFeat *GetFeat(unsigned short nFeat);
    CSWRace *m_pRaces;               // +0xb0
    CSWSkill *m_pSkills;             // +0xb4
};

extern CSWRules *g_pRules;

extern const unsigned char CLASS_TYPE_JEDI_GUARDIAN;
extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;
extern const unsigned char CLASS_TYPE_JEDI_SENTINEL;

// FUNCTION: SWKOTOR 0x005A58B0
char CSWSCreatureStats::GetTotalSTRBonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 0, 0);
}

// FUNCTION: SWKOTOR 0x005A58E0
char CSWSCreatureStats::GetTotalDEXBonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 1, 0);
}

// FUNCTION: SWKOTOR 0x005A5910
char CSWSCreatureStats::GetTotalCONBonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 2, 0);
}

// FUNCTION: SWKOTOR 0x005A5940
char CSWSCreatureStats::GetTotalINTBonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 3, 0);
}

// FUNCTION: SWKOTOR 0x005A5970
char CSWSCreatureStats::GetTotalWISBonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 4, 0);
}

// FUNCTION: SWKOTOR 0x005A59A0
char CSWSCreatureStats::GetTotalCHABonus()
{
    int n = 0;
    return m_pBaseCreature->GetTotalEffectBonus(4, 0, &n, 0, 0, 0, 0, 0xff, 5, 0);
}

// FUNCTION: SWKOTOR 0x005A4E90
unsigned char CSWSCreatureStats::GetClass(unsigned char nMultiClass)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0xff;
    return m_ClassInfo[nMultiClass].m_nClass;
}

// FUNCTION: SWKOTOR 0x005A5090
unsigned char CSWSCreatureStats::GetClassLevel(unsigned char nMultiClass, int bUseNegativeLevel)
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

// FUNCTION: SWKOTOR 0x005A50E0
unsigned char CSWSCreatureStats::GetClassNegativeLevels(unsigned char nMultiClass)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0;
    return m_ClassInfo[nMultiClass].m_nNegativeLevels;
}

// FUNCTION: SWKOTOR 0x005A5420
void CSWSCreatureStats::SetClass(unsigned char nMultiClass, unsigned char nClass)
{
    if (nMultiClass < m_nNumMultiClasses)
        m_ClassInfo[nMultiClass].m_nClass = nClass;
}

// FUNCTION: SWKOTOR 0x005A5440
void CSWSCreatureStats::SetClassNegativeLevels(unsigned char nMultiClass, unsigned char nLevels)
{
    if (nMultiClass < m_nNumMultiClasses)
        m_ClassInfo[nMultiClass].m_nNegativeLevels = nLevels;
}

// FUNCTION: SWKOTOR 0x005A5460
char CSWSCreatureStats::CalcStatModifier(unsigned char nStat)
{
    if (nStat < 10)
        return (nStat - 11) / 2;
    return (nStat - 10) / 2;
}

// FUNCTION: SWKOTOR 0x005A5490
unsigned long CSWSCreatureStats::GetExpNeededForNextLevel()
{
    for (int i = 0; i <= 20; i++) {
        if (m_nExperience < g_pRules->m_anExperienceTable[(unsigned char)i])
            return g_pRules->m_anExperienceTable[(unsigned char)i];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A54C0
void CSWSCreatureStats::SetSkillRank(unsigned char nSkill, char nRank)
{
    if (nSkill < g_pRules->m_nNumSkills)
        m_pSkillRanks[nSkill] = nRank;
}

// FUNCTION: SWKOTOR 0x005A5650
unsigned char CSWSCreatureStats::GetSpellsPerDayLeft(unsigned char nMultiClass,
                                                     unsigned char nSpellLevel)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0;
    return m_ClassInfo[nMultiClass].GetSpellsPerDayLeft(nSpellLevel);
}

// FUNCTION: SWKOTOR 0x005A5A60
int CSWSCreatureStats::GetForceResistance()
{
    char n = m_nForceResistance - m_nForceResistancePenalty;
    if (n < 0)
        return 0;
    return n;
}

// FUNCTION: SWKOTOR 0x005A5A80
unsigned char CSWSCreatureStats::GetACNaturalBase()
{
    m_pBaseCreature->GetIsDroid();
    return m_nACNaturalBase;
}

// FUNCTION: SWKOTOR 0x005A5D10
void CSWSCreatureStats::AddClass(unsigned char nClass, unsigned char nLevel)
{
    if (m_nNumMultiClasses < 2)
        m_nNumMultiClasses++;
    SetClass(m_nNumMultiClasses - 1, nClass);
    if ((unsigned char)(m_nNumMultiClasses - 1) < m_nNumMultiClasses)
        m_ClassInfo[(unsigned char)(m_nNumMultiClasses - 1)].m_nLevel = nLevel;
    if (m_pBaseCreature)
        m_pBaseCreature->UpdateClassInfo();
}

// The returned sum goes through the creature-side inline GetTotalAbilityBonus (object
// loaded before the pushes, int result cast to char); same shape for the five others.
// FUNCTION: SWKOTOR 0x005A6190
int CSWSCreatureStats::GetSTRStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nSTRAdjust + GetTotalSTRBonus() + m_nSTRBase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nSTRAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(0) + m_nSTRBase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005A9FE0
void CSWSCreatureStats::SetSTRBase(unsigned char n)
{
    m_nSTRBase = n;
    m_nSTRMod = CalcStatModifier(GetSTRStat());
}

// FUNCTION: SWKOTOR 0x005A6550
int CSWSCreatureStats::GetDEXStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nDEXAdjust + GetTotalDEXBonus() + m_nDEXBase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nDEXAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(1) + m_nDEXBase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005AA020
void CSWSCreatureStats::SetDEXBase(unsigned char n)
{
    m_nDEXBase = n;
    m_nDEXMod = CalcStatModifier(GetDEXStat());
}

// FUNCTION: SWKOTOR 0x005A6250
int CSWSCreatureStats::GetCONStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nCONAdjust + GetTotalCONBonus() + m_nCONBase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nCONAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(2) + m_nCONBase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005A6310
int CSWSCreatureStats::GetINTStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nINTAdjust + GetTotalINTBonus() + m_nINTBase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nINTAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(3) + m_nINTBase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005AA0F0
void CSWSCreatureStats::SetINTBase(unsigned char n)
{
    m_nINTBase = n;
    m_nINTMod = CalcStatModifier(GetINTStat());
    AdjustSpellUsesPerDay();
}

// FUNCTION: SWKOTOR 0x005A63D0
int CSWSCreatureStats::GetWISStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nWISAdjust + GetTotalWISBonus() + m_nWISBase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nWISAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(4) + m_nWISBase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005AA130
void CSWSCreatureStats::SetWISBase(unsigned char n)
{
    m_nWISBase = n;
    m_nWISMod = CalcStatModifier(GetWISStat());
    AdjustSpellUsesPerDay();
}

// FUNCTION: SWKOTOR 0x005A6490
int CSWSCreatureStats::GetCHAStat()
{
    if (g_pRules->m_pRaces[m_nRace].m_nCHAAdjust + GetTotalCHABonus() + m_nCHABase > 3)
        return g_pRules->m_pRaces[m_nRace].m_nCHAAdjust + (char)m_pBaseCreature->GetTotalAbilityBonus(5) + m_nCHABase;
    return 3;
}

// FUNCTION: SWKOTOR 0x005AA170
void CSWSCreatureStats::SetCHABase(unsigned char n)
{
    m_nCHABase = n;
    m_nCHAMod = CalcStatModifier(GetCHAStat());
    AdjustSpellUsesPerDay();
}

// FUNCTION: SWKOTOR 0x005A6920
unsigned short CSWSCreatureStats::GetFeat(unsigned short n)
{
    if (n >= m_lFeats.num)
        return 0xffff;
    return m_lFeats.element[n];
}

// FUNCTION: SWKOTOR 0x005A6940
unsigned short CSWSCreatureStats::GetBonusFeat(unsigned short n)
{
    if (n >= m_lBonusFeats.num)
        return 0xffff;
    return m_lBonusFeats.element[n];
}

// FUNCTION: SWKOTOR 0x005A6630
int CSWSCreatureStats::HasFeat(unsigned short nFeat)
{
    int bResult = 0;
    if (m_lFeats.Contains(nFeat) || m_lBonusFeats.Contains(nFeat))
        bResult = 1;
    return bResult;
}

// FUNCTION: SWKOTOR 0x005AFAC0
void CSWSCreatureStats::SetForceResistance(unsigned char n)
{
    m_nForceResistance = n;
    UpdateForceResistance();
}

// FUNCTION: SWKOTOR 0x005AFAE0
void CSWSCreatureStats::SetForceResistancePenalty(unsigned char n)
{
    m_nForceResistancePenalty = n;
    UpdateForceResistance();
}

// FUNCTION: SWKOTOR 0x005A5C70
short CSWSCreatureStats::AdjustHPAccumulatorBy(float f)
{
    m_fHPAccumulator += f;
    short n = (short)floor(m_fHPAccumulator);
    m_fHPAccumulator -= n;
    return n;
}

// FUNCTION: SWKOTOR 0x005A5CC0
short CSWSCreatureStats::AdjustFPAccumulatorBy(float f)
{
    m_fFPAccumulator += f;
    short n = (short)floor(m_fFPAccumulator);
    m_fFPAccumulator -= n;
    return n;
}

// FUNCTION: SWKOTOR 0x005A4CF0
unsigned char CSWSCreatureStats::ReturnTrue()
{
    return 1;
}

// NONMATCH (4): original has a dead `mov ecx,[esp+8]` (bRanged) between the
// GetItemInSlot test and the branch.
// FUNCTION: SWKOTOR 0x005A4D00
int CSWSCreatureStats::GetUseMonkAttackTables(int bRanged)
{
    if (m_pBaseCreature->GetIsDroid() && !m_pBaseCreature->m_pInventory->GetItemInSlot(0x10))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005A4D40
unsigned char CSWSCreatureStats::GetUnarmedDamageDice()
{
    volatile int nSize = m_pBaseCreature->m_nCreatureSize;
    m_pBaseCreature->GetIsDroid();
    return 1;
}

// FUNCTION: SWKOTOR 0x005A4D60
unsigned char CSWSCreatureStats::GetUnarmedDamageDie()
{
    int nSize = m_pBaseCreature->m_nCreatureSize;
    if (m_pBaseCreature->GetIsDroid() == 1)
        return (nSize > 2) * 2 + 4;
    return (nSize <= 2) + 1;
}

// FUNCTION: SWKOTOR 0x005A5110
unsigned char CSWSCreatureStats::GetSimpleAlignmentGoodEvil()
{
    if (m_nGoodEvil <= 40)
        return 3;
    if (m_nGoodEvil >= 60)
        return 2;
    return 1;
}

// FUNCTION: SWKOTOR 0x005A5FD0
unsigned char CSWSCreatureStats::GetLevel(int bUseNegativeLevel)
{
    int nLevel = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWSCreatureStats *pStats = m_pBaseCreature->m_pStats;
        unsigned char nClassLevel;
        if ((unsigned char)i >= pStats->m_nNumMultiClasses) {
            nClassLevel = 0;
        } else {
            int nRaw = pStats->m_ClassInfo[(unsigned char)i].m_nLevel;
            int nNeg = 0;
            if (bUseNegativeLevel)
                nNeg = pStats->m_ClassInfo[(unsigned char)i].m_nNegativeLevels;
            if (nRaw - nNeg < 0)
                nClassLevel = 0;
            else
                nClassLevel = nRaw - (unsigned char)nNeg;
        }
        nLevel += nClassLevel;
    }
    return nLevel;
}

// FUNCTION: SWKOTOR 0x005A6060
int CSWSCreatureStats::GetIsJedi()
{
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        int nClass = GetClass(i);
        if (nClass == CLASS_TYPE_JEDI_GUARDIAN || nClass == CLASS_TYPE_JEDI_CONSULAR ||
            nClass == CLASS_TYPE_JEDI_SENTINEL)
            return 1;
    }
    return 0;
}

// NONMATCH (4): original lays the !m_bEffectsValid `return 0` inline and jumps
// back to it from the bounds check; every if/ternary shape tried puts it last.
// FUNCTION: SWKOTOR 0x005A6170
CGameEffect *CSWSCreatureStats::GetEffect(unsigned char n)
{
    if (m_bEffectsValid) {
        if (n >= m_lEffects.num)
            return 0;
        return m_lEffects.element[n];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005A6610
unsigned long CSWSCreatureStats::GetExpNeededForLevelUp()
{
    return g_pRules->m_anExperienceTable[GetLevel(0)];
}

// FUNCTION: SWKOTOR 0x005A6720
void CSWSCreatureStats::DecrementFeatRemainingUses(unsigned short nFeat)
{
    for (unsigned short i = 0; i < m_lFeatUses.num; i++) {
        if (m_lFeatUses.element[i]->m_nFeat == nFeat)
            m_lFeatUses.element[i]->m_nUsedToday++;
    }
}

// FUNCTION: SWKOTOR 0x005A6750
unsigned long CSWSCreatureStats::GetKnownSpell(unsigned char nMultiClass,
                                               unsigned char nSpellLevel, unsigned char n)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0xffffffff;
    return m_ClassInfo[nMultiClass].GetKnownSpell(nSpellLevel, n);
}

// FUNCTION: SWKOTOR 0x005A67A0
unsigned short CSWSCreatureStats::GetNumberKnownSpells(unsigned char nMultiClass,
                                                       unsigned char nSpellLevel)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0;
    return m_ClassInfo[nMultiClass].m_pKnownSpellList[nSpellLevel].num;
}

// FUNCTION: SWKOTOR 0x005A6880
void CSWSCreatureStats::SetMemorizedSpellReady(unsigned long nSpell, unsigned char bReady,
                                               unsigned char nClass)
{
    for (int i = 0; i < m_pMemorizedSpells->num; i++) {
        if (m_pMemorizedSpells->element[i].m_nSpell == nSpell &&
            m_pMemorizedSpells->element[i].m_bReady != bReady &&
            m_pMemorizedSpells->element[i].m_nClass == nClass) {
            m_pMemorizedSpells->element[i].m_bReady = bReady;
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x005A68E0
int CSWSCreatureStats::HasReadySpell(unsigned long nSpell)
{
    for (int i = 0; i < m_pMemorizedSpells->num; i++) {
        if (m_pMemorizedSpells->element[i].m_nSpell == nSpell &&
            m_pMemorizedSpells->element[i].m_bReady == 1)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005AA060
void CSWSCreatureStats::SetCONBase(unsigned char n, int bAdjustHP)
{
    short nOldMax;
    m_nCONBase = n;
    if (bAdjustHP == 1)
        nOldMax = m_pBaseCreature->GetMaxHitPoints(bAdjustHP);
    m_nCONMod = CalcStatModifier(GetCONStat());
    if (bAdjustHP == 1) {
        CSWSCreature *pCreature = m_pBaseCreature;
        pCreature->SetCurrentHitPoints(pCreature->GetCurrentHitPoints(bAdjustHP) - nOldMax +
                                       pCreature->GetMaxHitPoints(1));
    }
}

// FUNCTION: SWKOTOR 0x005AA1B0
unsigned char CSWSCreatureStats::GetBaseFortSavingThrow()
{
    unsigned char nSave = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass *pClass = &g_pRules->m_pClasses[GetClass(i)];
        if (pClass)
            nSave += pClass->GetFortSaveBonus(GetClassLevel(i, 0));
    }
    if (HasFeat(0x16))
        nSave += 3;
    else if (HasFeat(0x15))
        nSave += 2;
    else if (HasFeat(0xd))
        nSave += 1;
    return nSave;
}

// FUNCTION: SWKOTOR 0x005AA2F0
unsigned char CSWSCreatureStats::GetBaseWillSavingThrow()
{
    unsigned char nSave = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass *pClass = &g_pRules->m_pClasses[GetClass(i)];
        if (pClass)
            nSave += pClass->GetWillSaveBonus(GetClassLevel(i, 0));
    }
    if (HasFeat(0x16))
        nSave += 3;
    else if (HasFeat(0x15))
        nSave += 2;
    else if (HasFeat(0xd))
        nSave += 1;
    return nSave;
}

// FUNCTION: SWKOTOR 0x005AA430
unsigned char CSWSCreatureStats::GetBaseReflexSavingThrow()
{
    unsigned char nSave = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass *pClass = &g_pRules->m_pClasses[GetClass(i)];
        if (pClass)
            nSave += pClass->GetReflexSaveBonus(GetClassLevel(i, 0));
    }
    if (HasFeat(0x16))
        nSave += 3;
    else if (HasFeat(0x15))
        nSave += 2;
    else if (HasFeat(0xd))
        nSave += 1;
    return nSave;
}

// FUNCTION: SWKOTOR 0x005AB810
char CSWSCreatureStats::GetFortSavingThrow(int bExcludeEffects)
{
    int nEffects = 0;
    if (bExcludeEffects == 0) {
        int n = 0;
        nEffects = m_pBaseCreature->GetTotalEffectBonus(3, 0, &n, 0, 0, 1, 0, 0xff, 0xff, 0);
    }
    char nMod = m_nCONMod;
    char nMisc = m_nFortSaveBonus;
    return nMod + GetBaseFortSavingThrow() + nEffects + nMisc;
}

// FUNCTION: SWKOTOR 0x005AB880
char CSWSCreatureStats::GetWillSavingThrow(int bExcludeEffects)
{
    int nEffects = 0;
    if (bExcludeEffects == 0) {
        int n = 0;
        nEffects = m_pBaseCreature->GetTotalEffectBonus(3, 0, &n, 0, 0, 3, 0, 0xff, 0xff, 0);
    }
    char nMod = m_nWISMod;
    char nMisc = m_nWillSaveBonus;
    return nMod + GetBaseWillSavingThrow() + nEffects + nMisc;
}

// NONMATCH (5): original keeps the DEX term as an int (movsx ebx) and the misc
// bonus in the dead arg slot; char nDEX gets the slot right but the DEX term in bl.
// FUNCTION: SWKOTOR 0x005AB8F0
char CSWSCreatureStats::GetReflexSavingThrow(int bExcludeEffects)
{
    int nEffects = 0;
    if (!bExcludeEffects) {
        int n = 0;
        nEffects = m_pBaseCreature->GetTotalEffectBonus(3, 0, &n, 0, 0, 2, 0, 0xff, 0xff, 0);
    }
    char nDEX;
    if (m_pBaseCreature->IsDebilitated())
    {
        int nDEXMod = m_nDEXMod;
        nDEX = nDEXMod < 0 ? nDEXMod : 0;
    }
    else
        nDEX = m_nDEXMod;
    char nMisc = m_nReflexSaveBonus;
    return nMisc + GetBaseReflexSavingThrow() + nEffects + nDEX;
}

// NONMATCH (8): original jumps from the armour `nMaxDEX >= 0` test into the shared
// min(DEX, nMaxDEX) block of the debilitated path (cross-jump); goto / one-if
// shapes are worse (12).
// FUNCTION: SWKOTOR 0x005A5330
char CSWSCreatureStats::GetDEXMod(int bArmorDexCap)
{
    int nMaxDEX = 0;
    if (bArmorDexCap == 1 && !m_pBaseCreature->IsDebilitated()) {
        CSWSItem *pArmor = m_pBaseCreature->m_pInventory->GetItemInSlot(2);
        if (pArmor && pArmor->ComputeArmorClass() > 0) {
            nMaxDEX = pArmor->GetBaseItemData()->m_nMaxDEXBonus;
            if (nMaxDEX >= 0)
            {
                if (m_nDEXMod < nMaxDEX)
                    return m_nDEXMod;
                return nMaxDEX;
            }
        }
    }
    if (m_pBaseCreature->IsDebilitated())
    {
        if (m_nDEXMod < nMaxDEX)
            return m_nDEXMod;
        return nMaxDEX;
    }
    return m_nDEXMod;
}

// FUNCTION: SWKOTOR 0x005A54F0
int CSWSCreatureStats::GetWeaponFinesse(CSWSItem *pWeapon)
{
    if (pWeapon && pWeapon->GetIsFinesseWeapon() == 1) {
        int nDEX;
        if (m_pBaseCreature->IsDebilitated()) {
            int nDEXMod = m_nDEXMod;
            nDEX = nDEXMod < 0 ? nDEXMod : 0;
        } else
            nDEX = m_nDEXMod;
        if ((char)nDEX > m_nSTRMod)
            return 1;
    }
    return 0;
}

// NONMATCH (8): original loads the creature +0xa88 argument first and m_nGoodEvil
// through `mov di, word ptr` + movsx (edi saved); ours loads the short first.
// FUNCTION: SWKOTOR 0x005A5550
unsigned char CSWSCreatureStats::GetSpellUsesLeft(unsigned long nSpell, unsigned char nMultiClass,
                                                  unsigned char nSpellLevel,
                                                  unsigned char nMetaType)
{
    if (nMultiClass >= m_nNumMultiClasses)
        return 0;
    CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    if (!pSpell)
        return 0;
    short nGoodEvil = m_nGoodEvil;
    unsigned long nCost = pSpell->GetAdjustedForcePoints(nGoodEvil, m_pBaseCreature->m_bForceAdjust);
    return (unsigned long)(short)(m_nForcePoints + m_nForcePointsBonus) >= nCost;
}

// NONMATCH (7): same m_nGoodEvil load order as GetSpellUsesLeft, and the original
// compares m_nForcePointsBonus in memory instead of reloading it.
// FUNCTION: SWKOTOR 0x005A55C0
int CSWSCreatureStats::UnReadySpell(unsigned long nSpell)
{
    int bResult = 0;
    CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    if (pSpell) {
        short nGoodEvil = m_nGoodEvil;
        int nCost = pSpell->GetAdjustedForcePoints(nGoodEvil, m_pBaseCreature->m_bForceAdjust);
        int nFP = m_nForcePoints;
        if (nFP >= nCost) {
            m_nForcePoints = nFP - nCost;
            return 1;
        }
        m_nForcePointsBonus += nFP - nCost;
        m_nForcePoints = 0;
        if (m_nForcePointsBonus < 0)
            m_nForcePointsBonus = 0;
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005AA810
void CSWSCreatureStats::AddFeat(unsigned short nFeat)
{
    CSWFeat *pFeat = g_pRules->GetFeat(nFeat);
    if (pFeat) {
        m_lFeats.AddUnique(nFeat);
        int bFound = 0;
        if (pFeat->m_nUsesPerDay > 0 || pFeat->m_nUsesPerDay == -1) {
            for (unsigned short i = 0; !bFound && i < m_lFeatUses.num; i++) {
                if (m_lFeatUses.element[i]->m_nFeat == nFeat)
                    bFound = 1;
            }
            if (!bFound) {
                CFeatUseListEntry *pEntry = new CFeatUseListEntry;
                pEntry->m_nFeat = nFeat;
                pEntry->m_nUsedToday = 0;
                m_lFeatUses.Add(pEntry);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005AA8C0
void CSWSCreatureStats::ClearFeats()
{
    m_lFeats.SetSize0();
    while (m_lFeatUses.num) {
        CFeatUseListEntry *pEntry = m_lFeatUses.element[m_lFeatUses.num - 1];
        m_lFeatUses.DelIndex(m_lFeatUses.num - 1);
        if (pEntry)
            delete pEntry;
    }
}

// FUNCTION: SWKOTOR 0x005AA940
void CSWSCreatureStats::RemoveFeat(unsigned short nFeat)
{
    m_lFeats.Remove(nFeat);
    int nCount = m_lFeatUses.num;
    if (nCount > 0) {
        for (short i = nCount - 1; i >= 0; i--) {
            CFeatUseListEntry *pEntry = m_lFeatUses.element[i];
            if (pEntry->m_nFeat == nFeat) {
                m_lFeatUses.DelIndex(i);
                delete pEntry;
                return;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005A7670 SYMBOL: ?AddUnique@?$CExoArrayList@G@@QAEXG@Z
template <class T>
void CExoArrayList<T>::AddUnique(T t)
{
    if (!Contains(t))
        Add(t);
}

template class CExoArrayList<unsigned short>;

// NONMATCH (9): original evaluates the __max(0, __min(100, n)) macro with the
// duplicated min (three-level clamp) before the inline SetGoodEvil clamp.
// FUNCTION: SWKOTOR 0x005AE610
void CSWSCreatureStats::ModifyAlignment(short nDelta)
{
    int n = nDelta + m_nGoodEvil;
    short nNew = __max(0, __min(100, n));
    SetGoodEvil(nNew);
    if (m_pBaseCreature)
        m_pBaseCreature->UpdateClassInfo();
    UpdateForceResistance();
}

// FUNCTION: SWKOTOR 0x005A9F50
unsigned char CSWSCreatureStats::GetTotalACFeatMod()
{
    unsigned char nMod = 0;
    CSWSItem *pRight = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    CSWSItem *pLeft = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    if (pRight && !pLeft) {
        CSWBaseItem *pBase = pRight->GetBaseItemData();
        if (pBase && (pBase->m_nWeaponWield == 2 || pBase->m_nWeaponWield == 4)) {
            if (HasFeat(0x73))
                nMod = 3;
            else if (HasFeat(0x72))
                nMod = 2;
            else if (HasFeat(0x71))
                nMod = 1;
        }
    }
    return nMod;
}

// FUNCTION: SWKOTOR 0x005A5D70
int CSWSCreatureStats::CheckTalentMask(unsigned long nMask, unsigned long nWanted)
{
    int bResult = 1;
    {
        unsigned long n = nWanted & 0xf000;
        if (n != 0xf000 && n != (nMask & 0xf000))
            bResult = 0;
    }
    {
        unsigned long n = nWanted & 0xf00;
        if (n != 0xf00 && n != (nMask & 0xf00))
            bResult = 0;
    }
    {
        unsigned long n = nWanted & 0xf0;
        if (n != 0xf0 && n != (nMask & 0xf0))
            bResult = 0;
    }
    {
        unsigned long n = nWanted & 0xf;
        if (n != 0xf && n != (nMask & 0xf))
            bResult = 0;
    }
    return bResult;
}

// NONMATCH (3): merged `return 0` block placement (original puts it before
// `return 1`).
// FUNCTION: SWKOTOR 0x005AB980
int CSWSCreatureStats::CheckPowerSuitability(int nPower, CSWSArea *pArea, unsigned long nMask,
                                             int a4, int a5, int a6, int a7, int nExclude)
{
    if (nPower == nExclude || !GetCreatureHasTalent(0, nPower))
        return 0;
    if (!m_pBaseCreature->GetArea() || CheckTalentMask(pArea->m_nTalentMask, nMask))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005AFA30
int CSWSCreatureStats::GetCreatureTalentRandomPower(int a1, int a2, int a3, int a4, int a5, int a6,
                                                    int a7)
{
    int nResult = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        if (CSWClass::IsForceUserClass(GetClass(i))) {
            nResult = GetCreatureTalentRandomPowerForClass(a1, a2, a3, a4, a5, i, a6, a7);
            if (nResult == 1)
                break;
        }
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005AA9B0
void CSWSCreatureStats::AddKnownSpell(unsigned char nMultiClass, unsigned long nSpell)
{
    if (nMultiClass < m_nNumMultiClasses) {
        if (g_pRules->m_pSpellArray->GetSpell(nSpell)) {
            if (m_bEffectsValid && !CSWClass::IsForceUserClass(GetClass(nMultiClass)))
                return;
            m_ClassInfo[nMultiClass].AddKnownSpell(0, nSpell);
        }
    }
}

// FUNCTION: SWKOTOR 0x005A4C50
CSWSCreature *CSWSCreatureStats::GetBaseCreature()
{
    return m_pBaseCreature;
}

// FUNCTION: SWKOTOR 0x005A6E30
unsigned char CSWSCreatureStats::GetMemorizedSpellClass(unsigned long nSpell)
{
    for (int i = 0; i < m_pMemorizedSpells->num; i++) {
        if (m_pMemorizedSpells->element[i].m_nSpell == nSpell)
            return m_pMemorizedSpells->element[i].m_nClass;
    }
    return 0xff;
}

// FUNCTION: SWKOTOR 0x005A6FE0
void CSWSCreatureStats::SetNormalBonusFlags(unsigned short nFeat, int *pbNormal, int *pbBonus)
{
    *pbNormal = 0;
    *pbBonus = 0;
    if (g_pRules->GetFeat(nFeat)) {
        CSWClass *pClass = &g_pRules->m_pClasses[GetClass(m_nNumMultiClasses - 1)];
        if (pClass) {
            if (pClass->IsNormalFeat(nFeat))
                *pbNormal = 1;
            if (pClass->IsBonusFeat(nFeat))
                *pbBonus = 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x005A7080
int CSWSCreatureStats::HasFeatInList(unsigned short nFeat, CExoArrayList<unsigned short> *pList)
{
    if (nFeat == 0xffff)
        return 0;
    if (m_lFeats.Contains(nFeat) || m_lBonusFeats.Contains(nFeat))
        return 1;
    if (pList) {
        for (unsigned short i = 0; i < pList->num; i++) {
            if (nFeat == pList->element[i])
                return 1;
        }
    }
    return 0;
}

// NONMATCH (7): original lays the !pClass `return 0` inline and jumps back to it
// after IsForceUserClass.
// FUNCTION: SWKOTOR 0x005A59D0
unsigned char CSWSCreatureStats::ComputeNumberKnownSpellsLeft(unsigned char nMultiClass)
{
    CSWClass *pClass = &g_pRules->m_pClasses[GetClass(nMultiClass)];
    unsigned char nClass = GetClass(nMultiClass);
    if (!pClass)
        return 0;
    if (!CSWClass::IsForceUserClass(nClass))
        return 0;
    unsigned char nLevel = GetClassLevel(nMultiClass, 0);
    if (nLevel == 1)
        return 2;
    return 1;
}

// FUNCTION: SWKOTOR 0x005A6F40
void CSWSCreatureStats::GetFeatGainsForLevelUp(int nUnused, unsigned char *pnNormal,
                                               unsigned char *pnBonus)
{
    CSWClass *pClass = &g_pRules->m_pClasses[GetClass(m_nNumMultiClasses - 1)];
    int nLevel = (unsigned char)(GetClassLevel(m_nNumMultiClasses - 1, 0) + 1);
    *pnNormal = nLevel - 1 < 20 ? pClass->m_anNormalFeatsGained[nLevel - 1] : 0;
    *pnBonus = nLevel - 1 < 20 ? pClass->m_anBonusFeatsGained[nLevel - 1] : 0;
}

// NONMATCH (9): original compares the remaining uses as int (movzx + cmp 0x64)
// with a separate int `return 0`; ours keeps a byte compare.
// FUNCTION: SWKOTOR 0x005A6680
unsigned char CSWSCreatureStats::GetFeatRemainingUses(unsigned short nFeat)
{
    CSWFeat *pFeat = g_pRules->GetFeat(nFeat);
    if (!pFeat || !HasFeat(nFeat))
        return 0;
    int bFound = 0;
    CFeatUseListEntry *pEntry = 0;
    for (unsigned short i = 0; !bFound && i < m_lFeatUses.num; i++) {
        pEntry = m_lFeatUses.element[i];
        if (pEntry->m_nFeat == nFeat)
            bFound = 1;
    }
    if (bFound && pEntry) {
        unsigned char nRemaining = pFeat->m_nUsesPerDay - pEntry->m_nUsedToday;
        return nRemaining > 0 ? __min((int)nRemaining, 100) : 0;
    }
    return 100;
}

// FUNCTION: SWKOTOR 0x005AF880
int CSWSCreatureStats::GetCanUseSkill(unsigned char nSkill)
{
    unsigned char nRealSkill;
    if (nSkill == 0x64 || nSkill == 0x65 || nSkill == 0x66)
        nRealSkill = 1;
    else
        nRealSkill = nSkill;
    if (nRealSkill >= g_pRules->m_nNumSkills)
        return 0;
    if (g_pRules->m_pSkills[nRealSkill].m_nFlags & 2) {
        for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
            if (g_pRules->m_pClasses[GetClass(i)].IsClassSkill(nSkill))
                return 1;
        }
        return 0;
    }
    int nRank = m_pSkillRanks[nRealSkill];
    char nClamped;
    if (nRank >= 127)
        nClamped = 127;
    else if (nRank <= -127)
        nClamped = -127;
    else
        nClamped = nRank;
    if (nClamped > 0)
        return 1;
    return 0;
}

// NONMATCH (55): original walks &m_ClassInfo[i].m_pKnownSpellList[lvl] with a
// constant +4 index register and re-reads nSpell / the word count every iteration.
// FUNCTION: SWKOTOR 0x005A6E70
int CSWSCreatureStats::HasSpell(unsigned char nSpellLevel, unsigned long nSpell,
                                int bCheckForcePoints)
{
    int bFound = 0;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CExoArrayList<unsigned long> *pList = &m_ClassInfo[i].m_pKnownSpellList[nSpellLevel];
        for (int j = 0; j < (unsigned short)pList->num; j++) {
            if (pList->element[j] == nSpell) {
                bFound = 1;
                break;
            }
        }
    }
    if (bCheckForcePoints) {
        if (!bFound)
            return 0;
        CSWSpell *pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
        short nGoodEvil = m_nGoodEvil;
        if (pSpell->GetAdjustedForcePoints(nGoodEvil, m_pBaseCreature->m_bForceAdjust) >
            (short)(m_nForcePoints + m_nForcePointsBonus))
            return 0;
        return 1;
    }
    return bFound;
}
