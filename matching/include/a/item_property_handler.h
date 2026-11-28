// shared declarations for CSWSItemPropertyHandler and the classes
// its apply/remove handlers touch (0x004E5000-0x004EAFFF). Only the members
// the handlers use are placed; everything else is padding.
#ifndef A_ITEM_PROPERTY_HANDLER_H
#define A_ITEM_PROPERTY_HANDLER_H

// .rdata constants (extern const ints in the original, not folded).
#include "shared/globals.h"
// ApplyImmunity table (.rdata, named by decimal value, as f/combat_resolve.cpp).
// Damage-type flags 0x00746850..0x00746884 (RemoveDamageImmunity switch).

#include "shared/exo_string.h"

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);
};

class CTwoDimArrays {
public:
    char pad_0x0[0x30];
    C2DA* m_pIPRPEnhancementTable;          // 0x30: attack/enhancement bonus "Value"
    C2DA* GetIPRPCostTable(unsigned char nTable);
};

class CSWBaseItem {
public:
    char pad_0x0[0x8];
    unsigned char m_nWeaponWield;           // 0x8 (3: double-bladed)
    unsigned char m_bOffHandable;           // 0x9
    char pad_0xa[0x1a - 0xa];
    unsigned char m_bRangedWeapon;          // 0x1a
    unsigned char m_nMinimumSize;           // 0x1b (vs creature size 0x4f8)
    char pad_0x1c[0x8b - 0x1c];
    unsigned char m_nACEnchantmentType;     // 0x8b
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nIndex);
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;     // 0x34
    char pad_0x38[0xaa - 0x38];
    unsigned char m_nDifficultyRacialType;  // 0xaa
    char pad_0xab[0xb8 - 0xab];
    CTwoDimArrays* m_p2DArrays;             // 0xb8
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
    CSWBaseItem* GetBaseItem(int nBaseItem) { return m_pBaseItemArray->GetBaseItem(nBaseItem); }
};

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreate);
    ~CGameEffect();                 // 0x00503590
    void SetCreator(unsigned long oidCreator);
    void CopyEffect(CGameEffect* pSource, int bIgnoreArrays);
    void SetInteger(int nIndex, int nValue);
    int GetInteger(int nIndex);
    void SetEffectType(unsigned short nType) { m_nType = nType; }
    void SetSubType(unsigned short nSubType) { m_nSubType = (m_nSubType & 0xfff8) | nSubType; }
    unsigned short GetSubType() { return m_nSubType & 7; }
    void SetSubTypeCategory(unsigned short n) { m_nSubType = (m_nSubType & 0xffe7) | n; }

    unsigned __int64 m_nID;         // 0x0
    unsigned short m_nType;         // 0x8
    unsigned short m_nSubType;      // 0xa: low 3 bits duration subtype
    char pad_0xc[0xc];
    unsigned long m_oidCreator;     // 0x18
    char pad_0x1c[0x8c - 0x1c];
};
#pragma pack(pop)

struct CSWItemProperty {
    unsigned short m_nPropertyName;    // 0x0
    unsigned short m_nSubType;         // 0x2
    unsigned char m_nCostTable;        // 0x4
    char pad_0x5[0x1];
    unsigned short m_nCostTableValue;  // 0x6
    char pad_0x8[0x1];
    unsigned char m_nParam1Value;      // 0x9
};

class CSWSItem {
public:
    char pad_0x0[0xc];
    unsigned long m_nBaseItem;  // 0xc
    char pad_0x10[0x4];
    unsigned long m_idSelf;     // 0x14
    int GetDamageFlags();
    CSWBaseItem* GetBaseItem();                                 // 0x005B4790
    int GetPropertyByTypeExists(int nProperty, int nSubType);   // 0x00555C90
};

class CSWSCreatureStats {
public:
    char pad_0x0[0x12e];
    unsigned short m_nACEffectStart;                // 0x12e
    unsigned short m_nAttackEffectStart;            // 0x130
    unsigned short m_nDamageReductionEffectStart;   // 0x132
    unsigned short m_nDamageResistanceEffectStart;  // 0x134
    unsigned short m_nDamageBonusEffectStart;       // 0x136
    char pad_0x138[0x142 - 0x138];
    unsigned short m_nForceResistanceEffectStart;   // 0x142
    unsigned short m_nAbilityEffectStart;           // 0x144
    char pad_0x146[0x150 - 0x146];
    unsigned short m_nSpecialWalkEffectStart;       // 0x150
    char pad_0x152[0x158 - 0x152];
    unsigned short m_nSkillEffectStart;             // 0x158
    char pad_0x15a[0x15e - 0x15a];
    unsigned short m_nImmunityEffectStart;          // 0x15e
    unsigned char GetNumberOfAttacks();             // 0x005A4CF0 (folded)
};

template <class T> class CExoArrayList {
public:
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { delete[] element; element = 0; }
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
    void DelIndex(int nIndex);
    T& operator[](int n) { return element[n]; }
    void SetSize(int nSize)
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        }
        else {
            Allocate(nSize);
        }
        num = nSize;
    }
    T* element;
    int num;
    int array_size;
};


#include "shared/exo_linked_list.h"

// List node: pPrev, pNext, pObject.
#include "shared/exo_linked_list.h"



class CSWInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);              // 0x005A4C20
};

class CSWSCombatRound;
struct CSWSObjectActionNode {
    int m_nActionId;                                // 0x0
};

class CSWSCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x4
    char pad_0x8[0x18 - 0x8];
    CExoString m_sTag;                              // 0x18
    char pad_0x20[0xcc - 0x20];
    int m_nUpdateDelta;                             // 0xcc (placeholder name)
    char pad_0xd0[0xfc - 0xd0];
    CExoLinkedList<CSWSObjectActionNode> m_lQueuedActions; // 0xfc
    char pad_0x100[0x124 - 0x100];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    char pad_0x130[0x4f8 - 0x130];
    int m_nCreatureSize;                            // 0x4f8
    char pad_0x4fc[0x50c - 0x4fc];
    unsigned long m_oidAttackTarget;                // 0x50c
    unsigned long m_oidAttemptedAttackTarget;       // 0x510
    char pad_0x514[0x524 - 0x514];
    unsigned long m_oidAttemptedSpellTarget;        // 0x524
    char pad_0x528[0x540 - 0x528];
    int m_nCombatState;                             // 0x540
    char pad_0x544[0x548 - 0x544];
    int m_nCombatStateParam;                        // 0x548
    char pad_0x54c[0x9c8 - 0x54c];
    CSWSCombatRound* m_pcCombatRound;               // 0x9c8
    char pad_0x9cc[0xa2c - 0x9cc];
    CSWInventory* m_pInventory;                     // 0xa2c
    char pad_0xa30[0xa74 - 0xa30];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    char pad_0xa78[0xa88 - 0xa78];
    int m_bPlayerCharacter;                         // 0xa88
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bIgnoreState);
    void RemoveEffectById(unsigned __int64 nID);
    int IsDebilitated();                                        // 0x005B4880
    unsigned char CanUnequipWeapon(CSWSItem* pItem);            // 0x00513E30
    void SetCombatTarget(unsigned long oidTarget, int bFlag);   // 0x004F34A0
    void UpdateCombatRoundTimer();                              // 0x004FAF20
    int ResetCombatAnimations(int nTime);                       // 0x004F1250
    void SignalDamage(int nDamage, unsigned long oidTarget);    // 0x005B8050
    int GetDeadTemp();                                          // 0x004EF890
};

class CSWSItemPropertyHandler;
typedef int (CSWSItemPropertyHandler::*ItemPropertyApplyFn)(CSWSItem* pItem,
        CSWItemProperty* pProperty, CSWSCreature* pCreature, unsigned long nSlot,
        int bLoadingGame);
typedef int (CSWSItemPropertyHandler::*ItemPropertyRemoveFn)(CSWSItem* pItem,
        CSWItemProperty* pProperty, CSWSCreature* pCreature, unsigned long nSlot);

// vtable 0x00744264
class CItemPropertyApplierRemover {
public:
    virtual ~CItemPropertyApplierRemover() {}
};

// vtable 0x00744284
class CSWSItemPropertyHandler : public CItemPropertyApplierRemover {
public:
    virtual ~CSWSItemPropertyHandler();                  // 0x004E53A0

    ItemPropertyApplyFn* m_pApplyFunctions;     // 0x4: 60 entries
    ItemPropertyRemoveFn* m_pRemoveFunctions;   // 0x8: 60 entries

    void InitializeItemProperties();
    // vtable slots 0x8 / 0xc (no functions.csv rows); declared non-virtual like InitializeItemProperties.
    int ApplyItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot, int bLoadingGame);                      // 0x004E5410
    int RemoveItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                           unsigned long nSlot);                                       // 0x004E5450
    // Handlers without a functions.csv row yet (named by table index / behaviour).
    int ApplyBonusFeat(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);   // 0x004E6D50
    int ApplyProperty29(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);  // 0x004E62D0
    int ApplyProperty36(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);  // 0x004E7B10
    int ApplyProperty55(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);  // 0x004E7CE0
    int ApplyProperty56(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);  // 0x004E7DB0
    int ApplyProperty59(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long, int);  // 0x004E7E80
    int RemoveDamageBonus(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);     // 0x004E86F0 (pilot1)
    int RemoveDamageImmunity(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);  // 0x004E97A0
    int RemoveImmunity(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);        // 0x004EA3F0
    int RemoveNothing(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);         // 0x00401590 (folded)
    int RemoveRegeneration(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);    // 0x004EA650
    int RemoveProperty36(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);      // 0x004EA580
    int RemoveTrueSeeing(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);      // 0x004E95A0
    int RemoveFreedomOfMovement(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long); // 0x004E9620
    int RemoveProperty55(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);      // 0x004EA6E0
    int RemoveProperty56(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);      // 0x004EA770
    int RemoveProperty59(CSWSItem*, CSWItemProperty*, CSWSCreature*, unsigned long);      // 0x004EA800

    int ApplyTrueSeeing(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                        unsigned long nSlot, int bLoadingGame);
    int ApplyRegeneration(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot, int bLoadingGame);
    int ApplyDamageReduction(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                             unsigned long nSlot, int bLoadingGame);
    int ApplyDamageResistance(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                              unsigned long nSlot, int bLoadingGame);
    int ApplyDamageVulnerability(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                 unsigned long nSlot, int bLoadingGame);
    int ApplyImprovedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                 unsigned long nSlot, int bLoadingGame);
    int ApplyReducedSavingThrows(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                 unsigned long nSlot, int bLoadingGame);
    int ApplyChangedSavingThrow(CSWSItem* pItem, int nValue, CSWItemProperty* pProperty,
                                CSWSCreature* pCreature, int bLoadingGame);

    int ApplyAbilityBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot, int bLoadingGame);
    int ApplyACBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                     unsigned long nSlot, int bLoadingGame);
    int ApplyDecreasedAbilityScore(CSWSItem* pItem, CSWItemProperty* pProperty,
                                   CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame);
    int ApplyDecreasedAC(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                         unsigned long nSlot, int bLoadingGame);
    int ApplyDecreasedSkillModifier(CSWSItem* pItem, CSWItemProperty* pProperty,
                                    CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame);
    int RemoveAbilityBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                           unsigned long nSlot);
    int RemoveACBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                      unsigned long nSlot);
    int RemoveDecreasedAbilityScore(CSWSItem* pItem, CSWItemProperty* pProperty,
                                    CSWSCreature* pCreature, unsigned long nSlot);
    int RemoveDecreasedAC(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot);
    int RemoveDecreasedSkillModifier(CSWSItem* pItem, CSWItemProperty* pProperty,
                                     CSWSCreature* pCreature, unsigned long nSlot);
    int ApplyAttackPenalty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                           unsigned long nSlot, int bLoadingGame);
    int ApplyDamagePenalty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                           unsigned long nSlot, int bLoadingGame);
    int ApplyImprovedForceResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
                                     CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame);
    int RemoveImprovedForceResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
                                      CSWSCreature* pCreature, unsigned long nSlot);
    int RemoveBonusFeat(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                        unsigned long nSlot);
    int RemoveDamageReduction(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                              unsigned long nSlot);
    int RemoveDamageResistance(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                               unsigned long nSlot);
    int RemoveDamageVulnerability(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                  unsigned long nSlot);
    int RemoveImprovedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                  unsigned long nSlot);
    int RemoveReducedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                                 unsigned long nSlot);
    int RemoveEnhancementPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                 CSWSCreature* pCreature, unsigned long nSlot);
    int RemoveAttackPenalty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                            unsigned long nSlot);
    int RemoveDamagePenalty(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                            unsigned long nSlot);
    int RemoveAttackBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot);
    int RemoveEnhancementBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot);
    int ApplyAttackBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                         unsigned long nSlot, int bLoadingGame);
    int ApplyDamageBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                         unsigned long nSlot, int bLoadingGame);
    int ApplyEnhancementBonus(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                              unsigned long nSlot, int bLoadingGame);
    int ApplyEnhancementPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame);
    int ApplySpecialWalk(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                         unsigned long nSlot, int bLoadingGame);
    int ApplyFreedomOfMovement(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame);
    int ApplyDamageImmunity(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                            unsigned long nSlot, int bLoadingGame);
    int ApplyImmunity(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                      unsigned long nSlot, int bLoadingGame);
    int RemoveSpecialWalk(CSWSItem* pItem, CSWItemProperty* pProperty, CSWSCreature* pCreature,
                          unsigned long nSlot);
    int RemoveChangedSavingThrow(int nValue, CSWSItem* pItem, CSWItemProperty* pProperty,
                                 CSWSCreature* pCreature);
};

#endif
