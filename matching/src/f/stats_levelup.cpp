// CSWSCreatureStats level-up and experience (0x005AF480-0x005B2530) and the
// melee / ranged attack bonus (0x005A7770, 0x005A7B60).

#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

class CGameEffect;
class CExoString;
class CSWSCreature;

class CSWSObject {
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
    virtual CSWSCreature *AsSWSCreature();  // +0x30
};

double floor(double);

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

    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t)
                return i;
        }
        return -1;
    }
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
    char pad_0x0[0x2c];
    unsigned char m_nMinAttackBonus;     // +0x2c
    char pad_0x2d;
    unsigned char m_nMinSTR;             // +0x2e
    unsigned char m_nMinDEX;             // +0x2f
    unsigned char m_nMinINT;             // +0x30
    unsigned char m_nMinWIS;             // +0x31
    unsigned char m_nMinSpellLevel;      // +0x32
    unsigned char m_nUsesPerDay;     // +0x33
    char pad_0x34[0x4];
    unsigned short m_nRequiredFeat1;     // +0x38
    unsigned short m_nRequiredFeat2;     // +0x3a
    unsigned short m_lstOrReqFeat[5];    // +0x3c
    unsigned short m_nRequiredSkill;     // +0x46

    unsigned char GetMinSpellLevel() { return m_nMinSpellLevel; }
    unsigned char GetMinAttackBonus() { return m_nMinAttackBonus; }
    unsigned short GetOrReqFeat(unsigned char n)
    {
        if (n < 5)
            return m_lstOrReqFeat[n];
        return 0xffff;
    }
};

class CSWSArea {
public:
    char pad_0x0[0x124];
    unsigned long m_nTalentMask;     // +0x124
};

class CSWClass {
public:
    static int IsForceUserClass(int nClass);
    unsigned char GetSpellsPerDayLevel(unsigned char nLevel, unsigned char nSpellLevel);
    int IsNormalFeat(unsigned short nFeat);
    int IsClassSkill(unsigned short nSkill);
    int IsBonusFeat(unsigned short nFeat);
    char GetFortSaveBonus(unsigned char nLevel);
    unsigned char GetAttackBonus(unsigned char nLevel);
    CExoString GetNameText();
    char GetReflexSaveBonus(unsigned char nLevel);
    char GetWillSaveBonus(unsigned char nLevel);
    char pad_0x0[0x13c];
    unsigned char m_anBonusFeatsGained[20];   // +0x13c
    unsigned char m_anNormalFeatsGained[20];  // +0x150
    char pad_0x164[0x38];
    int m_bSpellCaster;                       // +0x19c (placeholder name)

    unsigned char GetFeatGainLevel(unsigned short nFeat);
    unsigned char GetSpellLevelRequirement(unsigned char nLevel, unsigned char nSpellLevel,
                                           unsigned char nClass, unsigned short nRace,
                                           unsigned char nCHA);
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
    char pad_0x1e;
    unsigned char m_nFavoredClass;   // +0x1f
    char pad_0x20[0x14];
};

class CSWInventory;

class CSWSCreatureStats;
class CSWSStats;
class CSWSItem;

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const CExoString &s);
    ~CExoString();
    char *CStr() const;
    void Format(const char *szFormat, ...);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    char pad_0x0[0x8];
};

class CSWSMessage {
public:
    static void GetLocStringServer(unsigned long nLanguage, const CExoLocString &lsFirst,
                                   const CExoLocString &lsLast, CExoString &sOut,
                                   unsigned char nGender);
};

class CSWSModule {
public:
    char pad_0x0[0x20];
    unsigned long m_oidModule;       // +0x20 (placeholder name)
};

class CScriptEvent {
public:
    CScriptEvent();
    unsigned short m_nType;
    char pad_0x2[0x32];
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidTarget, unsigned long nEventId, void *pEventData);
};

class CSWSClient;
class CSWSMessage;

class CServerInfo {
public:
    char pad_0x0[0x90];
    int m_nMinLevel;                 // +0x90 (placeholder name)
};

class CSWCCMessageData;

class CServerExoApp {
public:
    CSWSClient *GetClientObjectByObjectID(unsigned long oid);
    CSWSMessage *GetSWSMessage();
    CServerInfo *GetServerInfo();
    CSWSModule *GetModule();
    CServerAIMaster *GetServerAIMaster();
    int GetDifficultyOption(int nOption);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
};

extern unsigned long EVENT_SIGNAL_EVENT;

extern CAppManager *g_pAppManager;

class CSWSCombatAttackData {
public:
    char pad_0x0[0x64];
    unsigned short m_nAttackType;    // +0x64
};

class CSWSCombatRound {
public:
    CSWSCombatAttackData *GetAttack(int nAttack);
    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;  // +0x96c
};

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
    void SendFeedbackMessage(unsigned short nMessage, CSWCCMessageData *pData);
    int IsDebilitated();
    int GetRelativeWeaponSize(CSWSItem *pWeapon);
    CSWSArea *GetArea();
    void UpdateClassInfo();

    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x4d8];
    int m_nCreatureSize;             // +0x4f8
    char pad_0x4fc[0x9c8 - 0x4fc];
    CSWSCombatRound *m_pcCombatRound;  // +0x9c8
    char pad_0x9cc[0x8];
    int m_b9d4;                      // +0x9d4
    char pad_0x9d8[0xa2c - 0x9d8];
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
    unsigned char m_nWeaponType;     // +0x9 (placeholder name)
    char pad_0xa[0x1a - 0xa];
    unsigned char m_nRangedWeapon;   // +0x1a
    unsigned char m_nWeaponSize;     // +0x1b
    char pad_0x1c[0x1e - 0x1c];
    unsigned char m_nCritThreat;     // +0x1e
    unsigned char m_nCritMultiplier; // +0x1f
    char pad_0x20[0xad - 0x20];
    unsigned char m_nMaxDEXBonus;    // +0xad
    char pad_0xae[0xb0 - 0xae];
    unsigned short m_nSpecializationFeat;  // +0xb0 (placeholder name)
    unsigned short m_nFocusFeat;           // +0xb2 (placeholder name)
};

struct CSWItemProperty {
    char pad_0x0[0x6];
    unsigned short m_nCostValue;     // +0x6
};

class CSWSItem {
public:
    CSWBaseItem *GetBaseItem();
    int GetPropertyByType(CSWItemProperty **ppProperty, unsigned short nType, unsigned short nSubType);
    int ComputeArmorClass();
    CSWBaseItem *GetBaseItemData();
    int GetIsFinesseWeapon();
    int FindProperty(CSWItemProperty **ppProperty, unsigned short nName, unsigned short nSubType);
    char pad_0x0[0xc];
    int m_nBaseItem;                 // +0xc
    int GetBaseItemType() { return m_nBaseItem; }
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int nBaseItem);
};

class CSWSpell {
public:
    int GetAdjustedForcePoints(unsigned long nLevel, int bAdjust);
    unsigned char GetSpellLevel(unsigned char nClass);
    char pad_0x0[0x140];
    unsigned char m_nUserType;           // +0x140
    char pad_0x141[0x1f];
    int m_lstPrereqSpells2[5];           // +0x160
    int m_lstPrereqSpells[5];            // +0x174
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
    unsigned char GetLevel(int bUseNegativeLevel)
    {
        int nLevel = m_nLevel;
        int nNeg = 0;
        if (bUseNegativeLevel)
            nNeg = m_nNegativeLevels;
        nLevel -= nNeg;
        if (nLevel < 0)
            return 0;
        return nLevel;
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

class CSWSCreatureStats {
public:
    char GetTotalSTRBonus();
    char GetTotalDEXBonus();
    char GetTotalCONBonus();
    char GetTotalINTBonus();
    char GetTotalWISBonus();
    char GetTotalCHABonus();
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
    unsigned short GetNumFeats() { return m_lFeats.num; }
    unsigned short GetFeat(unsigned short n)
    {
        if (n >= m_lFeats.num)
            return 0xffff;
        return m_lFeats.element[n];
    }
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
    int GetNumLevelStats() { return m_lLevelStats.num; }
    unsigned long GetExperience() { return m_nExperience; }
    CSWSStats *GetLevelStats(unsigned char n)
    {
        if (n < m_lLevelStats.num)
            return m_lLevelStats.element[n];
        return 0;
    }
    void SetExperience(unsigned long nValue, int bDoLevel);
    void AddExperience(unsigned long nXP);
    int GetMeleeAttackBonus(int bOffHand, int bIncludeBase, int bIgnorePenalties, int bSwapHands);
    int GetRangedAttackBonus(int bIgnorePenalties, int bIncludeBase, int nUnused);
    void LevelDown(CSWSStats *pLevelStats);
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
    unsigned char GetCHABase() { return m_nCHABase; }
    unsigned short GetRace() { return m_nRace; }
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
    CExoString GetFullName();
    unsigned short GetUnarmedDamageRoll(CSWSObject *pTarget);
    int ResolveSpecialAttackAttackBonus(int nUnused);
    int ResolveSpecialAttackDamageBonus(int nUnused);
    int GetCriticalHitMultiplier(int bOffHand);
    int GetCriticalHitRoll(int bOffHand);
    int EffectId2Effect2DAId(int nEffectId, int nSubType);
    CExoString GetClassString(unsigned char nMultiClass);
    int GetBaseAttackBonus();
    int HasSpellPrereq(CSWSpell *pSpell, CExoArrayList<unsigned long> *pList);
    int HasFeatPrereq(CSWFeat *pFeat, CExoArrayList<unsigned short> *pList);
    int CanChooseSpell(unsigned long nSpell, unsigned char nKnownLeft,
                       CExoArrayList<unsigned long> *pList);
    int GetMeleeDamageBonus(int bOffHand);
    unsigned char GetSpellGainWithBonus(unsigned char nMultiClass, unsigned char nSpellLevel);
    int GetRangedDamageBonus();
    int CanLevelUp();
    void LevelUp(CSWSStats *pLevelStats, int bAddStats);
    int ValidateLevelUp(CSWSStats *pLevelStats);
    int FeatRequirementsMet(unsigned short nFeat, CExoArrayList<unsigned short> *pList);
    int CanChooseFeat(unsigned short nFeat, unsigned char nNormalLeft, unsigned char nBonusLeft,
                      CExoArrayList<unsigned short> *pList);

    CExoArrayList<unsigned short> m_lFeats;            // +0x0
    CExoArrayList<CFeatUseListEntry *> m_lFeatUses;    // +0xc
    CExoArrayList<unsigned short> m_lBonusFeats;       // +0x18
    CSWSCreature *m_pBaseCreature;                     // +0x24
    CExoArrayList<CSWSStats *> m_lLevelStats;          // +0x28
    CExoLocString m_lsFirstName;     // +0x34
    CExoLocString m_lsLastName;      // +0x3c
    char pad_0x44[0x68 - 0x44];
    unsigned long m_nExperience;     // +0x68
    int m_bIsPC;                     // +0x6c
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
    char pad_0xf6[0x102 - 0xf6];
    unsigned char m_nBaseAttackBonusOverride;  // +0x102 (placeholder name)
    char pad_0x103[0x11f - 0x103];
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
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray;  // +0x34
    unsigned long m_anExperienceTable[0x15];  // +0x38
    CSWSpellArray *m_pSpellArray;    // +0x8c
    char pad_0x90[0x1b];
    unsigned char m_nNumSkills;      // +0xab
    CSWClass *m_pClasses;            // +0xac
    CSWFeat *GetFeat(unsigned short nFeat);
    unsigned short RollDice(unsigned char nNumDice, unsigned char nDieSize);
    unsigned short GetWeightedDamageAmount(int nRoll, int nDie, int bAttackerPC, int bDefenderPC,
                                           int nPercent);
    CSWRace *m_pRaces;               // +0xb0
    CSWSkill *m_pSkills;             // +0xb4
};

extern CSWRules *g_pRules;

extern const unsigned char CLASS_TYPE_JEDI_GUARDIAN;
extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;
extern const unsigned char CLASS_TYPE_JEDI_SENTINEL;


// FUNCTION: SWKOTOR 0x005AFB00
int CSWSCreatureStats::FeatRequirementsMet(unsigned short nFeat, CExoArrayList<unsigned short> *pList)
{
    CSWFeat *pFeat = g_pRules->GetFeat(nFeat);
    if (!pFeat)
        return 0;
    CSWClass *pClass = &g_pRules->m_pClasses[GetClass(m_nNumMultiClasses - 1)];
    unsigned char nGainLevel = pClass->GetFeatGainLevel(nFeat);
    if (nGainLevel != 0xff && nGainLevel > GetLevel(0))
        return 0;
    if (pFeat->m_nMinSpellLevel) {
        int bFound = 0;
        unsigned char i;
        for (i = 0; !bFound; i++) {
            if (i >= m_nNumMultiClasses)
                break;
            unsigned char nClass = m_ClassInfo[i].m_nClass;
            CSWClass *pSpellClass = &g_pRules->m_pClasses[nClass];
            if (pSpellClass->m_bSpellCaster &&
                pSpellClass->GetSpellLevelRequirement(m_ClassInfo[i].GetLevel(0), pFeat->GetMinSpellLevel(),
                                                      nClass, GetRace(), GetCHABase()))
                bFound = 1;
        }
        if (!bFound)
            return 0;
    }
    if (pFeat->GetMinAttackBonus() > GetBaseAttackBonus())
        return 0;
    if (pFeat->m_nMinSTR > m_nSTRBase)
        return 0;
    if (pFeat->m_nMinDEX > m_nDEXBase)
        return 0;
    if (pFeat->m_nMinINT > m_nINTBase)
        return 0;
    if (pFeat->m_nMinWIS > m_nWISBase)
        return 0;
    if (!HasFeatPrereq(pFeat, pList))
        return 0;
    unsigned short nSkill = pFeat->m_nRequiredSkill;
    if (nSkill != 0xffff && !GetCanUseSkill(nSkill))
        return 0;
    return 1;
}

// NONMATCH (32): register-allocation tie. The original keeps nFeat in esi for the
// known-feat scan (count in edx); cl keeps the count in esi and compares nFeat from
// its argument slot, and the scratch registers rotate (eax/ecx/edx) after that.
// FUNCTION: SWKOTOR 0x005B2530
int CSWSCreatureStats::CanChooseFeat(unsigned short nFeat, unsigned char nNormalLeft,
                                     unsigned char nBonusLeft, CExoArrayList<unsigned short> *pList)
{
    int bNormal = 0;
    int bBonus = 0;
    if (!g_pRules->GetFeat(nFeat))
        return 0;
    if (!nNormalLeft && !nBonusLeft)
        return 0;
    for (unsigned short i = 0; i < GetNumFeats(); i++) {
        if (GetFeat(i) == nFeat)
            return 0;
    }
    for (unsigned short k = 0; k < pList->num; k++) {
        if (nFeat == pList->element[k])
            return 0;
    }
    if (!FeatRequirementsMet(nFeat, pList))
        return 0;
    SetNormalBonusFlags(nFeat, &bNormal, &bBonus);
    if (!bNormal && !bBonus)
        return 0;
    unsigned char nNormal = nNormalLeft;
    unsigned char nBonus = nBonusLeft;
    int bUsed = 0;
    CExoArrayList<int> lUsed;
    int nCount = pList->num;
    if (nCount) {
        lUsed.array_size = nCount;
        lUsed.element = new int[nCount];
    }
    for (int j = 0; j < pList->num; j++)
        lUsed.element[j] = 0;
    lUsed.num = nCount;
    unsigned short n;
    for (n = 0; n < pList->num; n++) {
        SetNormalBonusFlags(pList->element[n], &bNormal, &bBonus);
        if (bNormal) {
            if (!bBonus) {
                if (!nNormal)
                    return 0;
                nNormal--;
                lUsed.element[n] = 1;
            }
        } else if (bBonus && nBonus) {
            nBonus--;
            lUsed.element[n] = 1;
        }
    }
    SetNormalBonusFlags(nFeat, &bNormal, &bBonus);
    if (bNormal) {
        if (!bBonus) {
            if (!nNormal)
                return 0;
            nNormal--;
            bUsed = 1;
        }
    } else if (bBonus) {
        if (!nBonus)
            return 0;
        nBonus--;
        bUsed = 1;
    }
    for (n = 0; n < pList->num; n++) {
        if (!lUsed.element[n]) {
            if (nNormal)
                nNormal--;
            else if (nBonus)
                nBonus--;
            else
                return 0;
        }
    }
    if (!bUsed && !nNormal && !nBonus)
        return 0;
    return 1;
}

// LevelUp's argument type is not known (descriptive class name).
// NONMATCH (22): register swap only - the original keeps this in edi and pModule /
// the creature id in esi; cl picks the opposite pair.
// FUNCTION: SWKOTOR 0x005AF950
int CSWSCreatureStats::ValidateLevelUp(CSWSStats *pLevelStats)
{
    if (!CanLevelUp())
        return 0;
    if (m_bIsPC) {
        CSWSModule *pModule = g_pAppManager->m_pServerExoApp->GetModule();
        if (pModule) {
            CScriptEvent *pEvent = new CScriptEvent;
            if (pEvent) {
                pEvent->m_nType = 0x25;
                unsigned long oidModule = pModule->m_oidModule;
                unsigned long oidCreature = m_pBaseCreature->m_idSelf;
                g_pAppManager->m_pServerExoApp->GetServerAIMaster()->AddEventDeltaTime(
                    0, 0, oidCreature, oidModule, EVENT_SIGNAL_EVENT, pEvent);
            }
        }
    }
    LevelUp(pLevelStats, 1);
    UpdateForceResistance();
    return 1;
}

// Names of the level-down helper and of the CServerInfo minimum are descriptive.
// NONMATCH (61): register allocation. The original keeps nDelta in edi through the
// level-down loop and re-extends nLevel for every use; cl caches the zero-extended
// level in edi and preloads m_nExperience. The two CExoString frame slots are also
// swapped (outer at +0x18 in the original).
// FUNCTION: SWKOTOR 0x005AF480
void CSWSCreatureStats::SetExperience(unsigned long nValue, int bDoLevel)
{
    CSWSClient *pClient =
        g_pAppManager->m_pServerExoApp->GetClientObjectByObjectID(m_pBaseCreature->m_idSelf);
    g_pAppManager->m_pServerExoApp->GetSWSMessage();
    int nDelta = nValue - m_nExperience;
    CExoString sUnused;
    if (nValue < m_nExperience) {
        CExoString sError;
        char *szTag = "<NO TAG>";
        if (m_pBaseCreature && m_pBaseCreature->m_sTag.CStr() != "")
            szTag = m_pBaseCreature->m_sTag.CStr();
        sError.Format("Tried to reduce XP of creature '%s' to '%d'. Cannot reduce XP.", szTag,
                      nValue);
        return;
    }
    m_nExperience = nValue;
    if (!bDoLevel)
        return;
    if (nDelta < 0) {
        unsigned char nLevel = GetLevel(0);
        while (nLevel > g_pAppManager->m_pServerExoApp->GetServerInfo()->m_nMinLevel && nLevel > 0 &&
               g_pRules->m_anExperienceTable[--nLevel] > m_nExperience)
            LevelDown(m_bIsPC ? GetLevelStats(nLevel) : 0);
    } else if (nDelta > 0) {
        while (CanLevelUp()) {
            if (GetNumLevelStats() - 1 < GetLevel(0))
                break;
            CSWSStats *pLevelStats = m_lLevelStats.element[GetLevel(0)];
            if (pLevelStats)
                LevelUp(pLevelStats, 0);
        }
    }
    if (pClient && CanLevelUp() &&
        nValue - nDelta < g_pRules->m_anExperienceTable[GetLevel(0)])
        m_pBaseCreature->SendFeedbackMessage(0xb, 0);
}

// Multiclass experience penalty: each non-favoured class two or more levels below
// the highest non-favoured class costs 20%.
// FUNCTION: SWKOTOR 0x005AF6A0
void CSWSCreatureStats::AddExperience(unsigned long nXP)
{
    if (m_nNumMultiClasses > 1) {
        double fPenalty = 0.0;
        unsigned char nHighest = 0;
        unsigned char nBest = 0;
        unsigned char nFavored = g_pRules->m_pRaces[m_nRace].m_nFavoredClass;
        if (nFavored == 0xff) {
            if (GetClassLevel(nBest, 0) < GetClassLevel(1, 0))
                nBest = 1;
            if (GetClassLevel(nBest, 0) < GetClassLevel(2, 0))
                nBest = 2;
            nFavored = GetClass(nBest);
        }
        unsigned char i;
        for (i = 0; i < m_nNumMultiClasses; i++) {
            if (nFavored != GetClass(i))
                nHighest = __max(nHighest, GetClassLevel(i, 0));
        }
        for (i = 0; i < m_nNumMultiClasses; i++) {
            if (nFavored != GetClass(i) && nHighest - GetClassLevel(i, 0) >= 2)
                fPenalty += 1.0;
        }
        nXP = (unsigned long)(nXP * (1.0 - fPenalty * 0.2));
    }
    SetExperience(m_nExperience + nXP, 1);
}

// Attack bonus names and parameter names are descriptive.
// NONMATCH (30): the DEX + STR sum is formed with lea into ecx (original: add into
// ebx), then only ecx/edx/eax choices in the hand-slot and base-item lookups of
// the penalty and style blocks. `int nSTRBonus;` assigned 0 late is what makes
// cl keep 0 in ebp for the early zero pushes and compares.
// FUNCTION: SWKOTOR 0x005A7770
int CSWSCreatureStats::GetMeleeAttackBonus(int bOffHand, int bIncludeBase, int bIgnorePenalties,
                                           int bSwapHands)
{
    int nSTRBonus;
    int nBonus = 0;
    CSWSItem *pWeapon;
    if (bOffHand && !bSwapHands)
        pWeapon = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    else
        pWeapon = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    if (pWeapon && g_pRules->m_pBaseItemArray->GetBaseItem(pWeapon->GetBaseItemType())->m_nRangedWeapon > 0)
        return 0;
    if (bIncludeBase == 1) {
        int n = 0;
        nBonus = m_pBaseCreature->GetTotalEffectBonus(1, 0, &n, 0, 0, 0, 0, 0xff, 0xff, bOffHand) +
                 GetBaseAttackBonus();
    }
    int nDEXBonus = 0;
    nSTRBonus = 0;
    if (GetWeaponFinesse(pWeapon)) {
        int nDEXMod;
        if (m_pBaseCreature->IsDebilitated()) {
            nDEXMod = m_nDEXMod;
            nDEXMod = __min(nDEXMod, 0);
        } else {
            nDEXMod = m_nDEXMod;
        }
        nDEXBonus = (char)nDEXMod;
    } else {
        nSTRBonus = m_nSTRMod;
    }
    nBonus += nSTRBonus + nDEXBonus;
    int nFocus = 0;
    if (pWeapon && pWeapon->GetBaseItem()) {
        unsigned short nFeat = pWeapon->GetBaseItem()->m_nFocusFeat;
        if (nFeat && HasFeat(nFeat))
            nFocus = 1;
    }
    nBonus += nFocus;
    if (!bIgnorePenalties) {
        int nPenalty = 0;
        CSWSItem *pOther;
        if (!bSwapHands)
            pOther = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
        else
            pOther = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
        if (!bOffHand) {
            if (pOther && g_pRules->m_pBaseItemArray->GetBaseItem(pOther->GetBaseItemType())->m_nWeaponType) {
                nPenalty = -6;
                if (m_pBaseCreature->GetRelativeWeaponSize(pOther) == -1 ||
                    g_pRules->m_pBaseItemArray->GetBaseItem(pOther->GetBaseItemType())->m_nWeaponWield == 3)
                    nPenalty = -4;
                if (HasFeat(0x55))
                    nPenalty += 4;
                else if (HasFeat(9))
                    nPenalty += 2;
            }
        } else if (pOther) {
            nPenalty = -10;
            if (HasFeat(0x55))
                nPenalty = -2;
            else if (HasFeat(9))
                nPenalty = -4;
            else if (HasFeat(3))
                nPenalty = -6;
        }
        nBonus += nPenalty;
    }
    int nStyle = 0;
    CSWSItem *pRight = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    CSWSItem *pLeft = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    CSWBaseItem *pBaseItem;
    if (pRight && !pLeft && (pBaseItem = pRight->GetBaseItem()) != 0) {
        unsigned char nWield = pBaseItem->m_nWeaponWield;
        if (nWield == 2 || nWield == 4) {
            if (HasFeat(0x73))
                nStyle = 3;
            else if (HasFeat(0x72))
                nStyle = 2;
            else if (HasFeat(0x71))
                nStyle = 1;
        }
    }
    return nBonus + nStyle;
}

// FUNCTION: SWKOTOR 0x005A7B60
int CSWSCreatureStats::GetRangedAttackBonus(int bIgnorePenalties, int bIncludeBase, int nUnused)
{
    int nPenalty;
    int nBonus = 0;
    CSWSItem *pWeapon = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    if (pWeapon && !(g_pRules->m_pBaseItemArray->GetBaseItem(pWeapon->GetBaseItemType())->m_nRangedWeapon > 0))
        return 0;
    if (bIncludeBase == 1) {
        int n = 0;
        nBonus = m_pBaseCreature->GetTotalEffectBonus(1, 0, &n, 0, 0, 0, 0, 0xff, 0xff, 0) +
                 GetBaseAttackBonus();
    }
    CSWSItem *pLeft = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    nPenalty = 0;
    if (!bIgnorePenalties) {
        if (pLeft && g_pRules->m_pBaseItemArray->GetBaseItem(pLeft->GetBaseItemType())->m_nWeaponType) {
            nPenalty = -6;
            if (m_pBaseCreature->GetRelativeWeaponSize(pWeapon) <= -1 ||
                g_pRules->m_pBaseItemArray->GetBaseItem(pWeapon->GetBaseItemType())->m_nWeaponWield == 3)
                nPenalty = -4;
            if (HasFeat(0x55))
                nPenalty += 4;
            else if (HasFeat(9))
                nPenalty += 2;
        }
    } else if (pWeapon) {
        nPenalty = -10;
        if (HasFeat(0x55))
            nPenalty = -2;
        else if (HasFeat(9))
            nPenalty = -4;
        else if (HasFeat(3))
            nPenalty = -6;
    }
    nBonus += nPenalty;
    int nDEXMod;
    if (m_pBaseCreature->IsDebilitated()) {
        nDEXMod = m_nDEXMod;
        nDEXMod = __min(nDEXMod, 0);
    } else {
        nDEXMod = m_nDEXMod;
    }
    nBonus += (char)nDEXMod;
    int nFocus = 0;
    if (pWeapon && pWeapon->GetBaseItem()) {
        unsigned short nFeat = pWeapon->GetBaseItem()->m_nFocusFeat;
        if (nFeat && HasFeat(nFeat))
            nFocus = 1;
    }
    nBonus += nFocus;
    int nStyle = 0;
    CSWSItem *pRight = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    pLeft = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    CSWBaseItem *pBaseItem;
    if (pRight && !pLeft && (pBaseItem = pRight->GetBaseItem()) != 0) {
        unsigned char nWield = pBaseItem->m_nWeaponWield;
        if (nWield == 2 || nWield == 4) {
            if (HasFeat(0x73))
                nStyle = 3;
            else if (HasFeat(0x72))
                nStyle = 2;
            else if (HasFeat(0x71))
                nStyle = 1;
        }
    }
    return nBonus + nStyle;
}
