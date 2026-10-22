// shared declarations for CSWSItemPropertyHandler and the classes
// its apply/remove handlers touch (0x004E5000-0x004EAFFF). Only the members
// the handlers use are placed; everything else is padding.
#ifndef A_ITEM_PROPERTY_HANDLER_H
#define A_ITEM_PROPERTY_HANDLER_H

// .rdata constants (extern const ints in the original, not folded).
extern int EFFECT_TYPE_DAMAGE_RESISTANCE;   // 0x00746684 (2)
extern int EFFECT_TYPE_REGENERATE;          // 0x00746698 (7)
extern int EFFECT_TYPE_DAMAGE_REDUCTION;    // 0x007466AC (0xc)
extern int EFFECT_TYPE_DAMAGE_INCREASE;     // 0x007466B0
extern int EFFECT_TYPE_DAMAGE_VULNERABILITY;// 0x007466C0 (0x11)
extern int EFFECT_TYPE_TRUESEEING;          // 0x0074679C (0x48)
extern int EFFECT_TYPE_BONUS_FEAT;          // 0x007467C8 (0x53)
extern int EFFECT_TYPE_ABILITY_INCREASE;    // 0x0074670C (36)
extern int EFFECT_TYPE_ABILITY_DECREASE;    // 0x00746710 (37)
extern int EFFECT_TYPE_AC_INCREASE;         // 0x0074673C (48)
extern int EFFECT_TYPE_AC_DECREASE;         // 0x00746740 (49)
extern int EFFECT_TYPE_SKILL_DECREASE;      // 0x0074675C (56)
extern int EFFECT_TYPE_SAVING_THROW_INCREASE;   // 0x007466E4 (26)
extern int EFFECT_TYPE_SAVING_THROW_DECREASE;   // 0x007466E8 (27)
extern int EFFECT_TYPE_FORCE_RESISTANCE_INCREASE; // 0x00746700 (33)
extern int EFFECT_TYPE_ATTACK_DECREASE;     // 0x007466A8 (11)
extern int EFFECT_TYPE_DAMAGE_DECREASE;     // 0x007466B4 (14)
extern unsigned short SUBTYPE_EQUIPPED;     // 0x007473BC (3)
extern int ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON;  // 0x007468C8 (1)
extern int ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON;   // 0x007468CC (2)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW1;         // 0x007468D0 (3)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW2;         // 0x007468D4 (4)
extern int ITEM_SLOT_ATTACK_TYPE_CLAW3;         // 0x007468D8 (5)
extern int ITEM_SLOT_ATTACK_TYPE_ONHAND;        // 0x007468DC (7)
extern int ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON;  // 0x007468E4 (1)
extern int ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON;   // 0x007468E8 (2)
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW1;         // 0x007468EC
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW2;         // 0x007468F0
extern int ITEM_SLOT_DAMAGE_TYPE_CLAW3;         // 0x007468F4
extern int ITEM_SLOT_DAMAGE_TYPE_ONHAND;        // 0x007468F8
extern int EFFECT_TYPE_ATTACK_INCREASE;     // 0x007466A4 (10)
extern int EFFECT_TYPE_SPECIALWALK;         // 0x00746764 (0x3a)
extern int EFFECT_TYPE_SPECIALWALK_END;     // 0x00746768 (0x3b)
extern int EFFECT_TYPE_IMMUNITY;            // 0x007466D4 (0x16)
extern int EFFECT_TYPE_DAMAGE_IMMUNITY;     // 0x007466BC (0x10)
extern int IMMUNITY_TYPE_PARALYSIS;         // 0x00746914 (6)
extern int IMMUNITY_TYPE_ENTANGLE;          // 0x00746920 (9)
extern int IMMUNITY_TYPE_SLOW;              // 0x00746924 (0xa)
extern int IMMUNITY_TYPE_MOVEMENT_SPEED_DECREASE; // 0x00746954 (0x18)
// ApplyImmunity table (.rdata, named by value).
extern int IMMUNITY_TYPE_01;                // 0x00746900
extern int IMMUNITY_TYPE_02;                // 0x00746904
extern int IMMUNITY_TYPE_03;                // 0x00746908
extern int IMMUNITY_TYPE_04;                // 0x0074690C
extern int IMMUNITY_TYPE_13;                // 0x00746940
extern int IMMUNITY_TYPE_1C;                // 0x00746964
extern int IMMUNITY_TYPE_1D;                // 0x00746968
extern int IMMUNITY_TYPE_1E;                // 0x0074696C
extern int IMMUNITY_TYPE_1F;                // 0x00746970
extern int IMMUNITY_TYPE_20;                // 0x00746974
extern int EFFECT_TYPE_PROPERTY29;          // 0x00746754 (item property 29 effect)
extern int EFFECT_TYPE_PROPERTY36;          // 0x00746758
extern int EFFECT_TYPE_PROPERTY55;          // 0x007467E8
extern int EFFECT_TYPE_PROPERTY56;          // 0x007467EC
extern int EFFECT_TYPE_PROPERTY59;          // 0x00746774
// Damage-type flags 0x00746850..0x00746884 (RemoveDamageImmunity switch).
extern int DAMAGE_FLAG_00;              // 0x00746850
extern int DAMAGE_FLAG_01;              // 0x00746854
extern int DAMAGE_FLAG_02;              // 0x00746858
extern int DAMAGE_FLAG_03;              // 0x0074685C
extern int DAMAGE_FLAG_04;              // 0x00746860
extern int DAMAGE_FLAG_05;              // 0x00746864
extern int DAMAGE_FLAG_06;              // 0x00746868
extern int DAMAGE_FLAG_07;              // 0x0074686C
extern int DAMAGE_FLAG_08;              // 0x00746870
extern int DAMAGE_FLAG_09;              // 0x00746874
extern int DAMAGE_FLAG_10;              // 0x00746878
extern int DAMAGE_FLAG_11;              // 0x0074687C
extern int DAMAGE_FLAG_12;              // 0x00746880
extern int DAMAGE_FLAG_13;              // 0x00746884
extern int ABILITY_STRENGTH;                // 0x00746838
extern int ABILITY_DEXTERITY;               // 0x0074683C
extern int ABILITY_CONSTITUTION;            // 0x00746840
extern int ABILITY_INTELLIGENCE;            // 0x00746844
extern int ABILITY_WISDOM;                  // 0x00746848
extern int ABILITY_CHARISMA;                // 0x0074684C
extern int AC_DEFLECTION_BONUS_VS_DAMAGE;   // 0x0074688C (16391): AC modifier damage type

class CExoString {
public:
    CExoString();                           // 0x005B3190
    CExoString(const char* source);
    ~CExoString();
    CExoString& operator=(const char* s);   // 0x005E5140
    CExoString& operator=(const CExoString& s); // 0x005E5C50
    char* CStr();                          // 0x005E5670
    void Format(const char* format, ...);   // 0x005E5680 (cdecl)
    char* m_sString;
    unsigned long m_nBufferLength;
};

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
extern CSWRules* g_pRules;

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
    T* element;
    int num;
    int array_size;
};

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    CExoLinkedListPosition pHead;   // 0x0
    CExoLinkedListPosition pTail;   // 0x4
    unsigned long m_nCount;         // 0x8
    CExoLinkedListInternal();                                   // 0x005E9AA0
    ~CExoLinkedListInternal();                                  // 0x005E9AB0
    void* GetAtPos(CExoLinkedListPosition pos);                 // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);                 // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);                   // 0x005E9C10
    void* RemoveHead();                                         // 0x005E9C80
    void* RemoveTail();                                         // 0x005E9CE0
    CExoLinkedListPosition AddBefore(void* pObject, CExoLinkedListPosition pos); // 0x005E9AD0
    CExoLinkedListPosition AddTail(void* pObject);             // 0x005E9DC0
};

// List node: pPrev, pNext, pObject.
struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;      // 0x0
    CExoLinkedListNode* pNext;      // 0x4
    void* pObject;                  // 0x8
};

template <class T> class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;
    CExoLinkedList();
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead() {
        CExoLinkedListNode* pNode = (CExoLinkedListNode*)m_pcExoLinkedListInternal->pHead;
        return pNode ? (T*)pNode->pObject : 0;
    }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
    unsigned long Count() { return m_pcExoLinkedListInternal->m_nCount; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    T* RemoveTail() { return (T*)m_pcExoLinkedListInternal->RemoveTail(); }
    CExoLinkedListPosition AddBefore(T* pObject, CExoLinkedListPosition pos) {
        return m_pcExoLinkedListInternal->AddBefore(pObject, pos);
    }
    CExoLinkedListPosition AddTail(T* pObject) { return m_pcExoLinkedListInternal->AddTail(pObject); }
    CExoLinkedListPosition AddHead(T* pObject) {
        CExoLinkedListInternal* pList = m_pcExoLinkedListInternal;
        if (pList->pHead) {
            return pList->AddBefore(pObject, pList->pHead);
        }
        return pList->AddTail(pObject);
    }
};

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
    char pad_0x20[0xfc - 0x20];
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
