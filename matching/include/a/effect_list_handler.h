// CSWSEffectListHandler destructor and the apply/remove handlers in
// 0x004D82B0-0x004D99AF. Self-contained declarations; only the members the
// handlers use are placed.
#ifndef A_EFFECT_LIST_HANDLER_H
#define A_EFFECT_LIST_HANDLER_H

#include <stddef.h>

class CSWSCreature;
class CSWSCreatureStats;

template <class T> class CExoArrayList {
public:
    void Remove(T t);

    T* element;
    int num;
    int array_size;
};

// 0x004E3B20 (CExoArrayList<unsigned short>, called by OnRemoveBonusFeat)
template <class T> void CExoArrayList<T>::Remove(T t)
{
    int i;
    for (i = 0; i < num; i++) {
        if (element[i] == t) {
            break;
        }
    }
    num--;
    for (; i < num; i++) {
        element[i] = element[i + 1];
    }
}

class CExoString {
public:
    CExoString(const char* s);                  // 0x005E5A90
    ~CExoString();                              // 0x005E5C20

    char* m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660

    char pad_0x0[0x24];
    int m_nNumRows;                             // 0x24
};

class C2DAs {
public:
    char pad_0x0[0x124];
    C2DA* m_pIgnoreOnDeath;                     // 0x124 (placeholder name)
};

struct CSWRulesEntry34 {
    char pad_0x0[0x34];
};

class CSWRules {
public:
    CSWRulesEntry34* GetEntryB0(unsigned short n) { return &m_pEntriesB0[n]; }

    char pad_0x0[0xaa];
    unsigned char m_nAA;                        // 0xaa (placeholder name)
    char pad_0xab[0xb0 - 0xab];
    CSWRulesEntry34* m_pEntriesB0;              // 0xb0 (placeholder name)
    char pad_0xb4[0xb8 - 0xb4];
    C2DAs* m_p2DArrays;                         // 0xb8
};
extern CSWRules* g_pRules;                      // 0x007A3A28

int IsIgnoredVisualEffect(int nVisual);         // 0x004CA960 (placeholder name)

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);              // 0x00503E40
    CGameEffect(CGameEffect* pCopy);            // 0x00503F70
    void UpdateLinked();                        // 0x00504240 (placeholder name)
    void ClearLinked(int a, int b);             // 0x00504810 (placeholder name)
    int GetInteger(int nIndex);                 // 0x00503690
    void SetInteger(int nIndex, int nValue);    // 0x005036A0
    unsigned long GetObjectID(int nIndex);      // 0x005036E0
    void SetObjectID(int nIndex, unsigned long oid);  // 0x005036F0
    float GetFloat(int nIndex);                 // 0x005036C0
    void SetFloat(int nIndex, float fValue);    // 0x005036D0
    void SetCreator(unsigned long oid);         // 0x00503A00
    int GetSaveType();                          // 0x00503A70 (placeholder name)
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    unsigned short GetDurationType() { return m_nSubType & 7; }

    unsigned __int64 m_nID;                     // 0x00
    unsigned short m_nType;                     // 0x08
    unsigned short m_nSubType;                  // 0x0a
    float m_fDuration;                          // 0x0c
    char pad_0x10[0x18 - 0x10];
    unsigned long m_oidCreator;                 // 0x18
    char pad_0x1c[0x28 - 0x1c];
    CGameEffect* m_pLinkLeft;                   // 0x28
    CGameEffect* m_pLinkRight;                  // 0x2c
    char pad_0x30[0x8c - 0x30];
};
#pragma pack(pop)

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }

    float x, y, z;
};

class CSWSCombatRound {
public:
    void SetRoundPaused(int bPaused, unsigned long oidPausedBy);  // 0x004D28F0
    void SetPauseTimer(int nTime, int bInfinite);                 // 0x004D2920

    char pad_0x0[0x958];
    int m_bRoundPaused;                         // 0x958
    char pad_0x95c[0x9a4 - 0x95c];
    int m_nAdditionalAttacks;                   // 0x9a4
};

// size 0x34 (matching/include/a/nw_message.h has the full class)
class CSWCCMessageData {
public:
    CSWCCMessageData();                         // 0x004D69F0
    ~CSWCCMessageData();
    void CopyTo(CSWCCMessageData* pOther);      // 0x004D6EE0 (placeholder name)
    void SetObjectID(int nIndex, unsigned long oid);  // 0x004D7780

    char pad_0x0[0x34];
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();      // 0x30
};

class CSWSObject : public CGameObject {
public:
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SendFeedbackMessage(int nStrRef);  // 0x7c (placeholder name)
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetIsInConversation();          // 0x94
    virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0();
    virtual char GetDamageImmunity(int nType);  // 0xb4

    void SetDamageImmunity(int nType, int nValue);  // 0x004CAF20
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bFlag);  // 0x004D14F0

    void SetAIValue(int nValue);                // 0x004CD240 (placeholder name)

    unsigned long m_idSelf;                     // 0x04
    unsigned char m_nObjectType;                // 0x08
    char pad_0x9[0x90 - 0x9];
    Vector m_vPosition;                         // 0x90
    char pad_0x9c[0xe4 - 0x9c];
    int m_nTemporaryHitPoints;                  // 0xe4 (read as a short)
    int m_nE8;                                  // 0xe8
    int m_nEC;                                  // 0xec
    char pad_0xf0[0xf8 - 0xf0];
    int m_bF8;                                  // 0xf8
};

class CSWSCreatureStats {
public:
    unsigned char GetClassNegativeLevels(unsigned char nClass);            // 0x005A50E0
    void SetClassNegativeLevels(unsigned char nClass, unsigned char nLevels);  // 0x005A5440
    char GetForceResistance();                  // 0x005A5A60
    void SetForceResistance(int nValue);        // 0x005AFAC0

    char pad_0x0[0x18];
    CExoArrayList<unsigned short> m_lstBonusFeats;  // 0x18
    char pad_0x24[0x6c - 0x24];
    int m_b6C;                                  // 0x6c
    char pad_0x70[0x103 - 0x70];
    unsigned char m_nEnemyAttackBonus;          // 0x103
    char pad_0x104[0x11c - 0x104];
    unsigned char m_n11C;                       // 0x11c
    char pad_0x11d[0x120 - 0x11d];
    char m_nForceResistanceDecrease;            // 0x120 (placeholder name)
    char pad_0x121[0x124 - 0x121];
    short m_nHitPoints124;                      // 0x124 (placeholder name)
    short m_nHitPoints126;                      // 0x126 (placeholder name)
    char pad_0x128[0x138 - 0x128];
    unsigned short m_nAIStateStart;             // 0x138 (placeholder name)
    char pad_0x13a[0x146 - 0x13a];
    unsigned short m_nFlag8ECBit0Start;         // 0x146 (placeholder name)
    char pad_0x148[0x14e - 0x148];
    unsigned short m_nFlags8ECStart;            // 0x14e (placeholder name)

    short GetCurrentHitPoints() { return (short)(m_nHitPoints126 + m_nHitPoints124); }

    void Set11C(char nValue);                   // 0x0060E760 (placeholder name)
    void SetForceResistanceDecrease(int nValue);  // 0x005AFAE0 (placeholder name)
    int GetEffectImmunity(unsigned char nType, CSWSCreature* pVersus);  // 0x005A6960
};

class CSWSCreature : public CSWSObject {
public:
    int GetIsPossessed();                       // 0x0063E7F0 (placeholder name)
    int GetDeadTemp();                          // 0x004EF890
    void ClearVisibleList();                    // 0x005178F0
    void UpdateVisibleList(int bFlag);          // 0x0051B050
    float GetMovementRate();                    // 0x004EC370 (placeholder name)
    void Fn4F34A0(unsigned long nValue, int bFlag);  // 0x004F34A0 (placeholder name)
    void SetCommandable(int bFlag);
    void Fn4FD960();                            // 0x004FD960 (placeholder name)
    void SendFeedbackData(int nMessage, CSWCCMessageData* pData);  // 0x004EDE10 (placeholder name)             // 0x004CCD80 (placeholder name)
    void ClearActions4ECFF0();                  // 0x004ECFF0 (placeholder name)
    void ClearActions4ED060(int bFlag);         // 0x004ED060 (placeholder name)
    void AddEffect4EE150(CGameEffect* pEffect); // 0x004EE150 (placeholder name)
    short GetMaxHitPoints();                    // 0x004FD490 (placeholder name)
    void SetMovementRate(float fRate);          // 0x004EC3B0 (placeholder name)

    char pad_0xfc[0x124 - 0xfc];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    char pad_0x130[0x310 - 0x130];
    unsigned long m_oid310;                     // 0x310 (placeholder name)
    Vector m_v314;                              // 0x314 (placeholder name)
    int m_n320;                                 // 0x320
    char pad_0x324[0x344 - 0x324];
    int m_bUpdateCombatInformation;             // 0x344
    char pad_0x348[0x8c8 - 0x348];
    int m_b8C8;                                 // 0x8c8
    int m_b8CC;                                 // 0x8cc
    int m_b8D0;                                 // 0x8d0
    int m_b8D4;                                 // 0x8d4
    int m_bAssuredDeflection;                   // 0x8d8
    int m_bAssuredDeflectionApplied;            // 0x8dc
    char pad_0x8e0[0x8e4 - 0x8e0];
    int m_b8E4;                                 // 0x8e4
    int m_b8E8;                                 // 0x8e8
    unsigned char m_n8EC;                       // 0x8ec
    char pad_0x8ed[0x9a4 - 0x8ed];
    int m_n9A4;                                 // 0x9a4
    char pad_0x9a8[0x9c8 - 0x9a8];
    CSWSCombatRound* m_pCombatRound;            // 0x9c8
    char pad_0x9cc[0x9d4 - 0x9cc];
    int m_b9D4;                                 // 0x9d4
    char pad_0x9d8[0x9e0 - 0x9d8];
    int m_n9E0;                                 // 0x9e0
    int m_b9E4;                                 // 0x9e4
    unsigned __int64 m_nEffectID9E8;            // 0x9e8
    unsigned short m_nAIStateFlags;             // 0x9f0
    char pad_0x9f2[0xa04 - 0x9f2];
    int m_bA04;                                 // 0xa04
    char pad_0xa08[0xa10 - 0xa08];
    unsigned char m_nA10;                       // 0xa10
    char pad_0xa11[0xa74 - 0xa11];
    CSWSCreatureStats* m_pStats;                // 0xa74
    char pad_0xa78[0xa9c - 0xa78];
    int m_bA9C;                                 // 0xa9c
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);  // 0x004ADE40
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void* pData);  // 0x004B08D0
};

class CVisibilityList {
public:
    void AddInvisible(unsigned long oid);       // 0x006449D0 (placeholder name)
};

class CServerExoAppInternal2 {
public:
    char pad_0x0[0x13c];
    CVisibilityList m_lInvisible;               // 0x13c (placeholder name)
};

class CServerExoApp {
public:
    CServerExoAppInternal2* GetInternal6B0();   // 0x004AE6B0 (placeholder name)
    CGameObjectArray* GetObjectArray();         // 0x004AED70
    CServerAIMaster* GetServerAIMaster();       // 0x004AED80
    CWorldTimer* GetWorldTimer();               // 0x004AEDE0
    CGameObject* GetAreaByGameObjectID(unsigned long oid);  // 0x004AE780
    void Fn4AE960(int bFlag);                   // 0x004AE960 (placeholder name)
    void Fn4AE9C0(unsigned long oid, int bFlag);  // 0x004AE9C0 (placeholder name)
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;             // 0x8
};
extern CAppManager* g_pAppManager;              // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
extern const unsigned char g_nGameObjectFound;   // 0x0074666C
extern const unsigned long g_nEventRemoveAoE;    // 0x007441E0 (0x0b)
extern const unsigned long g_nEvent4;            // 0x007441C4 (4)
extern const unsigned long g_nEvent7441C8;       // 0x007441C8
extern const unsigned short g_nDurationInstant;
extern const unsigned short g_nDurationTemporary;  // 0x007473C0
extern const unsigned char g_nObjectTypeA;       // 0x00746634
extern const unsigned char g_nObjectTypeB;       // 0x00746638
extern const unsigned char g_nObjectTypeC;       // 0x00746639  // 0x007473B8

// vtable 0x00744254
class CGameEffectApplierRemover {
public:
    virtual ~CGameEffectApplierRemover() {}
};

class CSWSEffectListHandler;
typedef int (CSWSEffectListHandler::*EffectApplyFn)(CSWSObject* pObject, CGameEffect* pEffect,
                                                     int bLoadingGame);
typedef int (CSWSEffectListHandler::*EffectRemoveFn)(CSWSObject* pObject, CGameEffect* pEffect);

// vtable 0x00744274
class CSWSEffectListHandler : public CGameEffectApplierRemover {
public:
    virtual ~CSWSEffectListHandler();

    int OnApply8D4(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);       // 0x004D83A0
    int OnRemove8D4(CSWSObject* pObject, CGameEffect* pEffect);                        // 0x004D83F0
    int OnApplyAssuredDeflection(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveAssuredDeflection(CSWSObject* pObject, CGameEffect* pEffect);          // 0x004D8490
    int OnApplyAbilityIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveAbilityIncrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyAbilityDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveAbilityDecrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplySavingThrowIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplySavingThrowDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyAttackIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyAttackDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyFlag344(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004D9200 (placeholder)
    int OnApplyDamageDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveFlag344(CSWSObject* pObject, CGameEffect* pEffect);  // 0x004D92A0 (placeholder)
    int OnApplyTemporaryHitpoints(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDamageImmunityIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDamageImmunityDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveEntangled(CSWSObject* pObject, CGameEffect* pEffect);

    // 0x004DA050-0x004DF510 (effect_list_handler2.cpp)
    int OnApplySetAIState(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyEnemyAttackBonus(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyMovementSpeedIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyMovementSpeedDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveAreaOfEffect(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyForceResistanceIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyRegenerate(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemove9E0(CSWSObject* pObject, CGameEffect* pEffect);       // 0x004DB810 (placeholder)
    int OnRemove9E4(CSWSObject* pObject, CGameEffect* pEffect);       // 0x004DB870 (placeholder)
    int OnRemove8C8(CSWSObject* pObject, CGameEffect* pEffect);       // 0x004DC380 (placeholder)
    int OnRemove8E4(CSWSObject* pObject, CGameEffect* pEffect);       // 0x004DC500 (placeholder)
    int OnApply8E8(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004DC530
    int OnApplySkillIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplySkillDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApply8EC(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004DCD00
    int OnApplyMissChance(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyStats6C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004DD100
    int OnRemoveNegativeLevel(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyVampiricRegeneration(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyForceResisted(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyForceFizzle(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int IsEffectToIgnoreOnDeath(CGameEffect* pEffect);

    // Untabled handlers (effect_list_handler3.cpp; placeholder names)
    int OnApplyStats11C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveAIValue(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyLinkedVisual(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyForceResistanceDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyLink(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemove8CC(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemove8D0(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApply9A4(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemove9A4(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveCreatureOnly(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyA9C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveA9C(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyCreatureOnly(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveA04(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveNothing(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyA10(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemove4EE150(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveServerFlags(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyRulesEntry(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyCommandable(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyCommandable2(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyHitPoints126(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveHitPoints126(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyHitPoints124(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDamage124(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyHeal124(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyVisualCopy(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveApplyVisual17(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyTemporary54(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPair2F49(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPair1C30(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPair3119(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyFloat39(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyArea(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveCombatPause(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyImmunityFeedback(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyTargetEvent(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);

    // 0x004E1A70-0x004E478B (effect_list_handler4.cpp)
    int OnRemoveSetAIState(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveEnemyAttackBonus(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveArcaneSpellFailure(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemove8E8(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveSpecialWalkAnimation(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemove8ECBit0(CSWSObject* pObject, CGameEffect* pEffect);  // 0x004E32A0 (placeholder)
    int OnRemoveBlindness(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyInvisibility(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveBonusFeat(CSWSObject* pObject, CGameEffect* pEffect);

    EffectApplyFn* m_pApplyFunctions;           // 0x04
    EffectRemoveFn* m_pRemoveFunctions;         // 0x08
};

// 0x004DF510: a CExoArrayList of 64-bit ids (placeholder class name).
class CEffectIDList {
public:
    int CountMatches(unsigned __int64 nID);     // 0x004DF510

    unsigned __int64* element;
    int num;
    int array_size;
};

#endif
