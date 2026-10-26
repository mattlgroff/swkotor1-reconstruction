// Worker M copy of the client creature stats / level-up stats layouts
// (copied from k/sw_c_creature_stats.h; extended for nwccreaturestats.o).
#ifndef M_SW_C_CREATURE_STATS_H
#define M_SW_C_CREATURE_STATS_H

#include <math.h>

class CResRef {
public:
    char m_resRef[16];
    CResRef operator=(const CResRef& r);            // 0x00406120
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList();
    ~CExoArrayList();
    void Allocate(int n);
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
    int AddUnique(T t);                             // ushort: 0x005A7670
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                return i;
            }
        }
        return -1;
    }
    T GetAt(int i)
    {
        if (i >= num) {
            return (T)-1;
        }
        return element[i];
    }
};

class C2DA {
public:
    int GetINTEntry(int nRow, int nColumn, int* pValue);    // 0x00413510
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pRacialTypes;                           // 0x0c
};

class CSWSkill {
public:
    char pad_0x0[0x18];
    int m_nKeyAbility;                              // 0x18
    unsigned long m_nFlags;                         // 0x1c
};

class CSWFeat {
public:
    char pad_0x0[0x2c];
    unsigned char m_nMinAttackBonus;                // 0x2c
    unsigned char m_nMinLevel;                      // 0x2d
    unsigned char m_nMinSTR;                        // 0x2e
    unsigned char m_nMinDEX;                        // 0x2f
    unsigned char m_nMinINT;                        // 0x30
    unsigned char m_nMinWIS;                        // 0x31
    unsigned char m_nMinSpellLevel;                 // 0x32
    char pad_0x33[0x5];
    unsigned short m_nRequiredFeat1;                // 0x38
    unsigned short m_nRequiredFeat2;                // 0x3a
    unsigned short m_lstOrReqFeat[5];               // 0x3c
    unsigned short m_nRequiredSkill;                // 0x46
};

class CSWSpell {
public:
    unsigned char GetSpellLevel(unsigned char nClass);  // 0x0059B650
    char pad_0x0[0x140];
    unsigned char m_nUserType;                      // 0x140
    char pad_0x141[0x1f];
    int m_lstPrereqSpells2[5];                      // 0x160
    int m_lstPrereqSpells[5];                       // 0x174
};

class CSWSpellArray {
public:
    int m_nNumSpells;                               // 0x00
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

class CSWClass {
public:
    int IsClassSkill(unsigned short nSkill);        // 0x005BE010
    int IsSkillUseable(unsigned short nSkill);      // 0x005BE070
    unsigned char GetAttackBonus(unsigned char nLevel);     // 0x005BCCB0
    unsigned char GetFortSaveBonus(unsigned char nLevel);   // 0x005BCCD0
    unsigned char GetRefSaveBonus(unsigned char nLevel);    // 0x005BCCF0
    unsigned char GetWillSaveBonus(unsigned char nLevel);   // 0x005BCD10
    unsigned char GetSpellsKnownPerLevel(unsigned char nLevel, unsigned char nSpellLevel);  // 0x005BCD30
    unsigned char GetSpellLevelRequirement(unsigned char nClassLevel, unsigned char nSpellLevel,
                                           unsigned char nClass, unsigned short nRace,
                                           unsigned char nAbility);         // 0x005BCD60
    unsigned char GetFeatGainLevel(unsigned short nFeat);   // 0x005BE220
    int IsBonusFeat(unsigned short nFeat);          // 0x005BE270
    int IsNormalFeat(unsigned short nFeat);         // 0x005BE2C0
    int IsGrantedFeat(unsigned short nFeat, unsigned char& nLevel);  // 0x005BE310
    unsigned short GetPackageFeat(int n);           // 0x005BE3C0
    int GetPackageSkill(int n);                     // 0x005BE430
    unsigned short GetPackageSpell(int nNumSpells, int n);  // 0x005BE6D0

    char pad_0x0[0x54];
    unsigned char m_nHitDie;                        // 0x54
    char pad_0x55[0x1];
    unsigned char m_nSkillPointBase;                // 0x56
    char pad_0x57[0xe5];
    unsigned char m_lstBonusFeatsPerLevel[20];      // 0x13c
    unsigned char m_lstNormalFeatsPerLevel[20];     // 0x150
    char pad_0x164[0x38];
    int m_bSpellCaster;                             // 0x19c

    int GetNormalFeatsPerLevel(unsigned char nLevel)
    {
        if (nLevel - 1 < 20) {
            return m_lstNormalFeatsPerLevel[nLevel - 1];
        }
        return 0;
    }
    int GetBonusFeatsPerLevel(unsigned char nLevel)
    {
        if (nLevel - 1 < 20) {
            return m_lstBonusFeatsPerLevel[nLevel - 1];
        }
        return 0;
    }
};

class CSWRace {
public:
    int IsFirstLevelGrantedFeat(unsigned short nFeat);  // 0x005C7CC0
    char pad_0x0[0x18];
    unsigned char m_nSTRAdjust;                     // 0x18
    unsigned char m_nDEXAdjust;                     // 0x19
    unsigned char m_nINTAdjust;                     // 0x1a
    unsigned char m_nCHAAdjust;                     // 0x1b
    unsigned char m_nWISAdjust;                     // 0x1c
    unsigned char m_nCONAdjust;                     // 0x1d
    char pad_0x1e[0x12];
    unsigned short m_nRacialTypeRow;                // 0x30
    char pad_0x32[0x2];
};

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
    CSWSkill* GetSkill(unsigned short nSkill) { return &m_lstSkills[nSkill]; }

    char pad_0x0[0x8c];
    CSWSpellArray* m_pSpellArray;                   // 0x8c
    char pad_0x90[0x14];
    unsigned short m_nNumFeats;                     // 0xa4
    char pad_0xa6[0x5];
    unsigned char m_nNumSkills;                     // 0xab
    CSWClass* m_lstClasses;                         // 0xac
    CSWRace* m_lstRaces;                            // 0xb0
    CSWSkill* m_lstSkills;                          // 0xb4
    C2DAs* m_p2DArrays;                             // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nRacialTypeColumn;                     // 0x007A2124

extern const unsigned char CLASS_TYPE_JEDI_GUARDIAN;    // 0x0074B31C
extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;    // 0x0074B31D
extern const unsigned char CLASS_TYPE_JEDI_SENTINEL;    // 0x0074B31E
extern const unsigned short RACIAL_TYPE_DROID;      // 0x0074BF10

class CSWCCombatInformation {
public:
    void SetOnHandAttackMod(int n) { m_nOnHandAttackMod = n; }

    unsigned char m_nNumAttacks;                    // 0x00
    char m_nOnHandAttackMod;                        // 0x01
    char m_nOnHandDamageMod;                        // 0x02
    char pad_0x3[0x5];
    unsigned char m_n08;                            // 0x08
    unsigned char m_n09;                            // 0x09
    unsigned char m_n0a;                            // 0x0a
};

class CSWCFeatUses {
public:
    unsigned short m_nFeat;                         // 0x00
    unsigned char m_nUsesLeft;                      // 0x02
};

extern "C" void __cdecl free(void* p);

class CSWCCreatureStats_ClassInfo {
public:
    CSWCCreatureStats_ClassInfo();
    ~CSWCCreatureStats_ClassInfo();
    void RemoveKnownSpell(unsigned long nSpell);    // 0x0066DCC0
    void ClearKnownSpells() throw();
    unsigned char GetSpellsPerDayLeft(unsigned char nLevel)
    {
        if (nLevel >= 1) {
            return 0;
        }
        return m_nSpellsPerDayLeft[nLevel];
    }
    unsigned short GetNumberKnownSpells(unsigned char nLevel)
    {
        return m_pKnownSpellList[nLevel].num;
    }
    void AddKnownSpell(unsigned char nLevel, unsigned long nSpell);
    unsigned long GetKnownSpell(unsigned char nLevel, unsigned char nIndex)
    {
        if (nLevel < 1) {
            return m_pKnownSpellList[nLevel].GetAt(nIndex);
        }
        return 0xffffffff;
    }

    CExoArrayList<unsigned long> m_pKnownSpellList[1];  // 0x00
    CExoArrayList<unsigned long> m_pAddedSpellList[1];  // 0x0c
    unsigned char m_nSpellsPerDayLeft[1];           // 0x18
    unsigned char m_n19;                            // 0x19
    unsigned char m_n1a;                            // 0x1a
    unsigned char m_n1b;                            // 0x1b
    unsigned char m_nClass;                         // 0x1c
    unsigned char m_nLevel;                         // 0x1d
    char pad_0x1e[0x2];
};

class CSWCCreatureStats {
public:
    int GetCanUseSkill(unsigned short nSkill);
    void SetBaseSkillRank(unsigned short nSkill, unsigned char nRank);
    char GetAbilityMod(unsigned char nScore);
    void SetCombatInformation(CSWCCombatInformation* pInfo);
    int CalculateNumAttacks();
    char GetSTRMod();
    char GetDEXMod();
    char GetCONMod();
    char GetINTMod();
    char GetWISMod();
    char GetCHAMod();
    char GetINTBaseMod();
    unsigned char GetTotalLevel();
    void ClearFeats();
    char GetSkillModifier(unsigned short nSkill);
    void UpdateAbilityModifiedSkillRanks();
    void ClearSpells();
    void InitializeCombatInformation();
    void RemoveKnownSpell(unsigned char nClassIndex, unsigned long nSpell);
    void AddKnownSpell(unsigned char nClassIndex, unsigned long nSpell);
    int HasSpell(unsigned char nClassIndex, unsigned long nSpell);
    int HasFeat(unsigned short nFeat);
    unsigned short GetFeat(unsigned short nIndex);
    unsigned short GetNumberKnownSpells(unsigned char nClassIndex, unsigned char nLevel);
    unsigned char GetSpellsPerDayLeft(unsigned char nClassIndex, unsigned char nLevel);
    int HasNormalFeat(unsigned short nFeat);
    void SetFeatRemainingUses(unsigned short nFeat, unsigned char nUses);
    void ClearKnownSpells(unsigned char nClassIndex);
    unsigned long GetKnownSpell(unsigned char nClassIndex, unsigned char nLevel, unsigned char nIndex);

    int GetSTRModInt()
    {
        char nMod = (char)floor(m_nSTR * 0.5) - 5;
        return nMod;
    }
    unsigned short GetNumberKnownSpellsInline(unsigned char nClassIndex, unsigned char nLevel)
    {
        if (nClassIndex >= 2) {
            return 0;
        }
        return m_ClassInfo[nClassIndex].GetNumberKnownSpells(nLevel);
    }
    unsigned long GetKnownSpellInline(unsigned char nClassIndex, unsigned char nLevel, unsigned char nIndex)
    {
        if (nClassIndex < 2) {
            return m_ClassInfo[nClassIndex].GetKnownSpell(nLevel, nIndex);
        }
        return 0xffffffff;
    }
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
    char GetSkillRank(unsigned short nSkill)
    {
        if (nSkill >= g_pRules->m_nNumSkills) {
            return 0;
        }
        return m_pSkillRanks[nSkill];
    }
    void SetModifiedSkillRank(unsigned short nSkill, char nRank)
    {
        if (nSkill < g_pRules->m_nNumSkills) {
            m_pModifiedSkillRanks[nSkill] = nRank;
        }
    }

    char pad_0x0[0x24];
    unsigned short m_nRace;                         // 0x24
    char pad_0x26[0xc];
    unsigned char m_nNumMultiClasses;               // 0x32
    char pad_0x33[0x1];
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
    char pad_0x41[0x7];
    short m_nBaseHitPoints;                         // 0x48
    char pad_0x4a[0x2];
    short m_nCurrentHitPoints;                      // 0x4c
    short m_nMaxHitPoints;                          // 0x4e
    short m_nACNaturalBase;                         // 0x50
    char pad_0x52[0x6];
    int m_bIsPC;                                    // 0x58
    char m_nFortSave;                               // 0x5c
    char m_nWillSave;                               // 0x5d
    char m_nReflexSave;                             // 0x5e
    char pad_0x5f[0x9];
    CSWCCombatInformation* m_pCombatInformation;    // 0x68
    int m_bCombatInformationSet;                    // 0x6c
    char pad_0x70[0x10];
    short m_nGoodEvil;                              // 0x80
    char pad_0x82[0x2];
    char* m_pModifiedSkillRanks;                    // 0x84
    char* m_pSkillRanks;                            // 0x88
    char pad_0x8c[0x22];
    unsigned short m_nSkillPoints;                  // 0xae
    CExoArrayList<unsigned short> m_lstFeats;       // 0xb0
    CExoArrayList<unsigned short> m_lstBonusFeats;  // 0xbc
    CExoArrayList<CSWCFeatUses*> m_lstFeatUses;             // 0xc8
    CSWCCreatureStats_ClassInfo m_ClassInfo[2];     // 0xd4
    char pad_0x114[0x14];
    CExoArrayList<CResRef> m_lstResRefs;            // 0x128
};

class CSWCLevelUpFeatList;

class CSWCLevelUpStats : public CSWCCreatureStats {
public:
    int GetSkillPointCostForSkillRank(unsigned short nSkill, int bIncrease);
    int GetBaseAttackBonus();
    void SetSkillRankBase(unsigned short nSkill, char nRank);
    void AddSkillPointsPerLevel();
    void SetFortSavingThrow();
    void SetWillSavingThrow();
    void SetReflexSavingThrow();
    int IncreaseSkillRank(unsigned short nSkill);
    void DecreaseSkillRank(unsigned short nSkill, unsigned char nMinRank);
    void GetPackageFeatsLeft(unsigned char* pnNormal, unsigned char* pnBonus);   // 0x00648FE0
    unsigned char ComputeNumberKnownSpellsLeft();
    void SetNormalBonusFlags(unsigned short nFeat, int* pbNormal, int* pbBonus);
    void AddDefaultPackageSkills(unsigned char nClass, int bAll);
    int HasSpellPrereq(CSWSpell* pSpell, CExoArrayList<unsigned long>* pList);
    int HasFeatPrereq(CSWFeat* pFeat, CExoArrayList<unsigned short>* pList);
    void AddGrantedFeats(CSWCLevelUpFeatList* pList);
    void ClearPackage();
    int CanChooseSpell(unsigned long nSpell, unsigned char nKnownLeft, CExoArrayList<unsigned long>* pList);
    unsigned char AddDefaultPackageSpells(CExoArrayList<unsigned long>* pList);
    void SetDEXBase(unsigned char nDEX, int bApply);
    void SetClass(unsigned char nIndex, unsigned char nClass);
    void SetClassLevel(unsigned char nIndex, unsigned char nLevel);
    void SetSTRBase(unsigned char nSTR);
    void SetCONBase(unsigned char nCON);
    void SetWISBase(unsigned char nWIS);
    void SetINTBase(unsigned char nINT);
    void SetCHABase(unsigned char nCHA);
    int HasFeat(unsigned short nFeat, CExoArrayList<unsigned short>* pList);
    unsigned char GetSimpleAlignment()
    {
        unsigned char nAlign;
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
    int FeatRequirementsMet(unsigned short nFeat, CExoArrayList<unsigned short>* pList);
    int CanChooseFeat(unsigned short nFeat, unsigned char nNormal, unsigned char nBonus, CExoArrayList<unsigned short>* pList);  // 0x0064A4C0
    unsigned char AddDefaultPackageFeats(unsigned char nClass, CExoArrayList<unsigned short>* pList,
                                         int bNormal, int bBonus);
};

class CSWCLevelUpFeatList {
public:
    void AddFeat(unsigned short nFeat);             // 0x00671C00
};

#endif
