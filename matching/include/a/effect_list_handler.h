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
    void Insert(T t, int nPos);                 // folded instantiation, e.g. 0x00616940
    void Allocate(int nSize);                   // folded instantiation, e.g. 0x004D2FA0
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }

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
    CExoString();                               // 0x005B3190
    CExoString(const char* s);                  // 0x005E5A90
    ~CExoString();                              // 0x005E5C20
    CExoString& operator=(const CExoString& s); // 0x005E5C50
    int operator!=(const char* s) const;        // 0x005E5390
    int operator==(const char* s) const;        // 0x005E5290

    char* m_sString;
    unsigned long m_nBufferLength;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270

    char pad_0x0[0x24];
    int m_nNumRows;                             // 0x24
};

class C2DAs {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                   // 0x0c (appearance.2da)
    C2DA* GetAppearanceTable() { return m_pAppearanceTable; }
    char pad_0x10[0x90 - 0x10];
    C2DA* m_pPoisonTable;                       // 0x90 (poison.2da)
    C2DA* m_pDiseaseTable;                      // 0x94 (disease.2da)
    char pad_0x98[0x120 - 0x98];
    C2DA* m_pEffectIconTable;                   // 0x120 (effecticon.2da)
    C2DA* m_pIgnoreOnDeath;                     // 0x124 (placeholder name)
};

struct CSWRulesEntry34 {
    char pad_0x0[0x34];
};

class CSWRules {
public:
    unsigned short RollDice(unsigned char nDice, unsigned char nSides);  // 0x00550B60
    CSWRulesEntry34* GetEntryB0(unsigned short n) { return &m_pEntriesB0[n]; }

    char pad_0x0[0xaa];
    unsigned char m_nAA;                        // 0xaa (placeholder name)
    char pad_0xab[0xb0 - 0xab];
    CSWRulesEntry34* m_pEntriesB0;              // 0xb0 (placeholder name)
    char pad_0xb4[0xb8 - 0xb4];
    C2DAs* m_p2DArrays;                         // 0xb8
    C2DAs* Get2DArrays() { return m_p2DArrays; }
};
extern CSWRules* g_pRules;                      // 0x007A3A28

int IsIgnoredVisualEffect(int nVisual);         // 0x004CA960 (placeholder name)

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);              // 0x00503E40
    CGameEffect(CGameEffect* pCopy);            // 0x00503F70
    ~CGameEffect();                             // 0x00503590
    void UpdateLinked();                        // 0x00504240 (placeholder name)
    void ClearLinked(int a, int b);             // 0x00504810 (placeholder name)
    int GetInteger(int nIndex);                 // 0x00503690
    void SetInteger(int nIndex, int nValue);    // 0x005036A0
    unsigned long GetObjectID(int nIndex);      // 0x005036E0
    void SetObjectID(int nIndex, unsigned long oid);  // 0x005036F0
    float GetFloat(int nIndex);                 // 0x005036C0
    CExoString GetString(int nIndex);           // 0x00503700
    void SetFloat(int nIndex, float fValue);    // 0x005036D0
    void SetCreator(unsigned long oid);         // 0x00503A00
    void CopyEffect(CGameEffect* pFrom, int bArg);  // 0x00504090
    void SetExpiryTime(unsigned long nDay, unsigned long nTime);  // 0x00503750
    void GetExpiryTime(unsigned long* pDay, unsigned long* pTime);  // 0x00503770
    int GetSaveType();                          // 0x00503A70 (placeholder name)
    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    unsigned short GetDurationType() { return m_nSubType & 7; }
    unsigned short GetType() { return m_nType; }
    unsigned __int64 GetID() { return m_nID; }
    float GetDuration() { return m_fDuration; }
    unsigned long GetCreator() { return m_oidCreator; }
    unsigned long GetSpellId() { return m_nSpellId; }

    unsigned __int64 m_nID;                     // 0x00
    unsigned short m_nType;                     // 0x08
    unsigned short m_nSubType;                  // 0x0a
    float m_fDuration;                          // 0x0c
    char pad_0x10[0x18 - 0x10];
    unsigned long m_oidCreator;                 // 0x18
    unsigned long m_nSpellId;                   // 0x1c
    int m_bExpose;                              // 0x20
    char pad_0x24[0x28 - 0x24];
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
    void RemoveAllActions();                                      // 0x004D3770
    struct CSWSAttackData* GetAttack(int nAttack);                // 0x004D2B50

    char pad_0x0[0x958];
    int m_bRoundPaused;                         // 0x958
    char pad_0x95c[0x96c - 0x95c];
    unsigned char m_nCurrentAttack;             // 0x96c
    char pad_0x96d[0x9a4 - 0x96d];
    int m_nAdditionalAttacks;                   // 0x9a4
};

// size 0x34 (matching/include/a/nw_message.h has the full class)
class CSWCCMessageData {
public:
    CSWCCMessageData();                         // 0x004D69F0
    ~CSWCCMessageData();
    void CopyTo(CSWCCMessageData* pOther);      // 0x004D6EE0 (placeholder name)
    void SetObjectID(int nIndex, unsigned long oid);  // 0x004D7780
    void SetInteger(int nIndex, int nValue);    // 0x004D76B0

    char pad_0x0[0x34];
};

class CGameObject {
public:
    virtual ~CGameObject();                     // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual class CSWSObject* AsSWSObject();    // 0x10
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();      // 0x30

    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                     // 0x04
};

class CSWSObject : public CGameObject {
public:
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44();
    virtual class CSWSPlaceable* AsSWSPlaceable();  // 0x48
    virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SendFeedbackMessage(int nStrRef);  // 0x7c (placeholder name)
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetIsInConversation();          // 0x94
    virtual short GetMaxHitPoints(int bArg);    // 0x98
    virtual short GetCurrentHitPoints(int bArg);  // 0x9c
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual char GetDamageImmunityB0(int nType);  // 0xb0 (placeholder name)
    virtual char GetDamageImmunity(int nType);  // 0xb4

    void SetDamageImmunity(int nType, int nValue);  // 0x004CAF20
    void SetDamageImmunity(unsigned short nType, char nValue);  // 0x004CAF20 (real signature)
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bFlag);  // 0x004D14F0
    void RemoveEffect(CGameEffect* pEffect);    // 0x004D05C0
    int RemoveEffectById(unsigned __int64 nID); // 0x004D06C0

    void SetAIValue(int nValue);                // 0x004CD240 (placeholder name)

    unsigned char m_nObjectType;                // 0x08
    char pad_0x9[0x8c - 0x9];
    unsigned long m_oidArea;                    // 0x8c
    Vector m_vPosition;                         // 0x90
    Vector m_vOrientation;                      // 0x9c
    char pad_0xa8[0xe4 - 0xa8];
    int m_nTemporaryHitPoints;                  // 0xe4 (read as a short)
    int m_nE8;                                  // 0xe8
    int m_nEC;                                  // 0xec
    int m_bF0;                                  // 0xf0 (placeholder name)
    char pad_0xf4[0xf8 - 0xf4];
    int m_bF8;                                  // 0xf8
    char pad_0xfc[0x124 - 0xfc];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
};

class CSWSCreatureStats {
public:
    unsigned char GetLevel(int bArg);           // 0x005A5FD0
    unsigned char GetClassLevel(unsigned char nMultiClass, int bArg);  // 0x005A5090
    unsigned char GetClassNegativeLevels(unsigned char nClass);            // 0x005A50E0
    void SetClassNegativeLevels(unsigned char nClass, unsigned char nLevels);  // 0x005A5440
    char GetForceResistance();                  // 0x005A5A60
    void SetForceResistance(int nValue);        // 0x005AFAC0
    void SetForceResistance(unsigned char nValue);  // 0x005AFAC0 (real signature)

    char pad_0x0[0x18];
    CExoArrayList<unsigned short> m_lstBonusFeats;  // 0x18
    char pad_0x24[0x6c - 0x24];
    int m_b6C;                                  // 0x6c
    char pad_0x70[0x89 - 0x70];
    unsigned char m_nNumMultiClasses;           // 0x89
    char pad_0x8a[0xf8 - 0x8a];
    unsigned char m_nACBonus[10];               // 0xf8..0x101 (AC increase/decrease by kind, placeholder name)
    char pad_0x102;
    unsigned char m_nEnemyAttackBonus;          // 0x103
    char pad_0x104[0x11c - 0x104];
    unsigned char m_n11C;                       // 0x11c
    char pad_0x11d[0x120 - 0x11d];
    char m_nForceResistanceDecrease;            // 0x120 (placeholder name)
    char pad_0x121[0x124 - 0x121];
    short m_nHitPoints124;                      // 0x124 (placeholder name)
    short m_nHitPoints126;                      // 0x126 (placeholder name)
    char pad_0x128[0x12e - 0x128];
    unsigned short m_nACStart;                  // 0x12e (placeholder name)
    char pad_0x130[0x138 - 0x130];
    unsigned short m_nAIStateStart;             // 0x138 (placeholder name)
    char pad_0x13a[0x13c - 0x13a];
    unsigned short m_nEffectIconStart;          // 0x13c (placeholder name)
    char pad_0x13e[0x140 - 0x13e];
    unsigned short m_nInvisibilityStart;        // 0x140 (placeholder name)
    char pad_0x142[0x146 - 0x142];
    unsigned short m_nFlag8ECBit0Start;         // 0x146 (placeholder name)
    unsigned short m_nTrueSeeingStart;          // 0x148 (placeholder name)
    char pad_0x14a[0x14e - 0x14a];
    unsigned short m_nFlags8ECStart;            // 0x14e (placeholder name)

    short GetCurrentHitPoints() { return (short)(m_nHitPoints126 + m_nHitPoints124); }
    unsigned char GetClass(unsigned char nMultiClass);  // 0x005A4E90

    void Set11C(char nValue);                   // 0x0060E760 (placeholder name)
    void SetForceResistanceDecrease(int nValue);  // 0x005AFAE0 (placeholder name)
    int GetEffectImmunity(unsigned char nType, CSWSCreature* pVersus);  // 0x005A6960
};

// effect icon entry (placeholder name; only the leading id is used)
struct CEffectIcon {
    short m_nIcon;
};

class CSWSCreature : public CSWSObject {
public:
    void UnsummonMyself();                    // 0x005149B0
    void UpdateAmbientAnimationState();     // 0x004F28D0
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
    int UnDisguise(CGameEffect* pEffect);       // 0x004EE150
    short GetMaxHitPoints();                    // 0x004FD490 (placeholder name)
    int Disguise(int nAppearance, CGameEffect* pEffect, int bLoadingGame);  // 0x004EE010 (placeholder name)
    void SetMovementRate(float fRate);          // 0x004EC3B0 (placeholder name)
    CSWSCreatureStats* GetStats() { return m_pStats; }
    int GetF8() { return m_bF8; }

    char pad_0x130[0x19c - 0x130];
    int m_b19C;                                 // 0x19c (placeholder name)
    char pad_0x1a0[0x270 - 0x1a0];
    CExoString m_sScriptOnSpawn;                // 0x270 (placeholder name)
    char pad_0x278[0x310 - 0x278];
    unsigned long m_oid310;                     // 0x310 (placeholder name)
    Vector m_v314;                              // 0x314 (placeholder name)
    int m_n320;                                 // 0x320
    char pad_0x324[0x340 - 0x324];
    class CSWSCreatureFollowInfo* m_pFollowInfo;  // 0x340 (pathfind information)
    int m_bUpdateCombatInformation;             // 0x344
    char pad_0x348[0x34c - 0x348];
    int m_bSpawned34C;                          // 0x34c (placeholder name)
    int m_n350;                                 // 0x350 (placeholder name)
    int m_n354;                                 // 0x354 (placeholder name)
    char pad_0x358[0x8c8 - 0x358];
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
    unsigned char m_n8ED;                       // 0x8ed (placeholder name)
    char pad_0x8ee[0x8f4 - 0x8ee];
    CExoArrayList<CEffectIcon*> m_lstEffectIcons;  // 0x8f4 (placeholder name)
    char pad_0x900[0x9a4 - 0x900];
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
    char pad_0xa11[0xa60 - 0xa11];
    unsigned short m_nAppearanceType;           // 0xa60
    char pad_0xa62[0xa74 - 0xa62];
    CSWSCreatureStats* m_pStats;                // 0xa74
    char pad_0xa78[0xa8c - 0xa78];
    int m_nAIState;                             // 0xa8c
    char pad_0xa90[0xa9c - 0xa90];
    int m_bA9C;                                 // 0xa9c
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);  // 0x004ADE40
    unsigned long GetCalendarDayFromSeconds(float fSeconds);       // 0x004AE1B0
    unsigned long GetTimeOfDayFromSeconds(float fSeconds);         // 0x004AE200
    int AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDay, unsigned long* pTime);  // 0x004ADF50
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void* pData);  // 0x004B08D0
};

// a CExoArrayList<unsigned long> (placeholder class name)
class CVisibilityList {
public:
    void AddInvisible(unsigned long oid);       // 0x006449D0 (placeholder name)
    void Remove(unsigned long oid);             // 0x0066DCC0
    int Contains(unsigned long oid)
    {
        int n = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == oid) {
                n++;
            }
        }
        return n;
    }

    unsigned long* element;
    int num;
    int array_size;
};

class CServerExoAppInternal2 {
public:
    char pad_0x0[0x13c];
    CVisibilityList m_lInvisible;               // 0x13c (placeholder name)
};

class CSWSClient {
public:
    char pad_0x0[0x4];
    unsigned long m_nPlayerId;                  // 0x04 (placeholder name)
};

class CSWSMessage {
public:
    int SendServerToPlayerCCMessage(unsigned long nPlayerId, unsigned char nMinor,
                                    CSWCCMessageData* pData, void* pArg);  // 0x00568570
};

class CServerExoApp {
public:
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);  // 0x004AE910
    CSWSMessage* GetNWSMessage();               // 0x004AEDB0
    CServerExoAppInternal2* GetInternal6B0();   // 0x004AE6B0 (placeholder name)
    CGameObjectArray* GetObjectArray();         // 0x004AED70
    CServerAIMaster* GetServerAIMaster();       // 0x004AED80
    CWorldTimer* GetWorldTimer();               // 0x004AEDE0
    CGameObject* GetAreaByGameObjectID(unsigned long oid);  // 0x004AE780
    void Fn4AE960(int bFlag);                   // 0x004AE960 (placeholder name)
    void TogglePauseState(unsigned char nState);  // 0x004AE960
    unsigned char GetActivePauseState();        // 0x004AE990
    void AddToExclusionList(unsigned long oid, unsigned char nState);  // 0x004AE9B0
    CWorldTimer* GetActiveTimer(unsigned long oid);  // 0x004AE830
    void Fn4AE9C0(unsigned long oid, int bFlag);  // 0x004AE9C0 (placeholder name)
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
};

class CAppManager {
public:
    char pad_0x0[0x4];
    class CClientExoApp* m_pClientExoApp;       // 0x4
    CServerExoApp* m_pServerExoApp;             // 0x8
};
extern CAppManager* g_pAppManager;              // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
extern const unsigned char g_nGameObjectFound;   // 0x0074666C
extern const unsigned long g_nEventRemoveAoE;    // 0x007441E0 (0x0b)
extern const unsigned long g_nEventRemoveEffect;  // 0x007441EC (placeholder name)
extern const unsigned long g_nEvent4;            // 0x007441C4 (4)
extern const unsigned long g_nEvent7441C8;       // 0x007441C8
extern const unsigned short g_nDurationInstant;
extern const unsigned short g_nDurationTemporary;  // 0x007473C0
extern const unsigned short g_nDurationPermanent;  // 0x007473C4 (placeholder name)
extern const unsigned short g_nDurationEquipped;   // 0x007473C8 (placeholder name)
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
    int OnRemoveSummonCreature(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveSetStateInternal(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyACIncrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyACDecrease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
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

    // effect_list_handler5.cpp
    int OnRemoveEffectIcon(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplySlow(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDispelAllMagic(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyTimestop(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyBlindness(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveACIncrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveACDecrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveDamageImmunityIncrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveDamageImmunityDecrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveSetState(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveMovementSpeedChange(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveForceResistanceIncrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyUltraVision(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveUltraVision(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveInvisibility(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveForceResistanceDecrease(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyHasteOrSlow(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveHasteOrSlow(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyVision(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDisguise(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyTrueSeeing(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveTrueSeeing(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyBonusFeat(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);

    // effect_list_handler6.cpp
    int OnApplySanctuary(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveSanctuary(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyResurrection(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPureGoodPowers(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPureEvilPowers(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyEffectIcon(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);

    // effect_list_handler7.cpp
    int OnApplyAreaOfEffect(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDeaf(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyEntangled(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnRemoveDisappearAppear(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveTemporaryHitpoints(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveWounding(CSWSObject* pObject, CGameEffect* pEffect);
    int OnRemoveHitPointChangeWhenDying(CSWSObject* pObject, CGameEffect* pEffect);
    int OnApplyCurse(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyDisease(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyPoison(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplyNegativeLevel(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    int OnApplySetStateInternal(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    void InitializeEffects();                   // 0x004E4A10

    // handlers not reconstructed yet (registered by InitializeEffects)
    int OnApplySummonCreature(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);    // 0x004D8680
    int OnApplySummonParty(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);       // 0x004D8A40
    int OnApplyKnockdown(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);         // 0x004D99B0
    int OnApplyDisarm(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);            // 0x004D9CE0
    int OnApplyVisualEffect(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);      // 0x004DAA40
    int OnApplyForceJumpDelayed(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004DE0A0
    int OnApplyLightsaberThrow(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);   // 0x004DE600
    int OnApplyForceShield(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);       // 0x004DF540
    int OnApplyDamage(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);            // 0x004DFA40
    int OnApplyHeal(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);              // 0x004E0750
    int OnApplyDeath(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);             // 0x004E0AC0
    int OnApplySetState(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);          // 0x004E1C20
    int OnApplyForcePush(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);         // 0x004E3800
    int OnApplyDispelBestMagic(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);   // 0x004E43C0
    // returns 0; folded with other three-argument zero leaves at 0x0066B360 (placeholder name)
    int OnApplyNothing(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);
    // InitializeEffects slots whose identical bodies the linker folded into one
    // address (placeholder names: base handler + effect type)
    int OnRemoveFlag344Type1B(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveFlag344
    int OnRemoveFlag344Type0A(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveFlag344
    int OnRemoveFlag344Type0B(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveFlag344
    int OnRemoveFlag344Type0D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveFlag344
    int OnRemoveFlag344Type0E(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveFlag344
    int OnApplyNothingType3D(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyNothing
    int OnRemoveNothingType3D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnApplyNothingType0C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyNothing
    int OnRemoveMovementSpeedChangeType1D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveMovementSpeedChange
    int OnRemoveNothingType20(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnApplyNothingType41(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyNothing
    int OnRemoveNothingType41(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnApplyNothingType32(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyNothing
    int OnRemoveNothingType32(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType2D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType4A(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType4B(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnApplyMissChanceType4C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyMissChance
    int OnRemoveNothingType4C(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnApplyNothingType4E(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyNothing
    int OnRemoveNothingType4E(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType16(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType36(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType37(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType38(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType44(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType45(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType4D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveSummonCreatureType59(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveSummonCreature
    int OnApplyCreatureOnlyType2B(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnApplyCreatureOnlyType42(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType42(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnApplyCreatureOnlyType5C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType5C(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnApplyCreatureOnlyType5D(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType5D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnApplyCreatureOnlyType5E(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType5E(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnApplyCreatureOnlyType61(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType61(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnApplyCreatureOnlyType63(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // folded with OnApplyCreatureOnly
    int OnRemoveCreatureOnlyType63(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveCreatureOnly
    int OnRemoveNothingType66(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType67(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType6B(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType6C(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing
    int OnRemoveNothingType6D(CSWSObject* pObject, CGameEffect* pEffect);  // folded with OnRemoveNothing

    EffectApplyFn* m_pApplyFunctions;           // 0x04
    EffectRemoveFn* m_pRemoveFunctions;         // 0x08
};

// 0x004DF510: a CExoArrayList of 64-bit ids (placeholder class name).
class CEffectIDList {
public:
    CEffectIDList() { num = 0; array_size = 0; element = NULL; }
    ~CEffectIDList() { delete[] element; }
    int CountMatches(unsigned __int64 nID);     // 0x004DF510
    void AddUnique(unsigned __int64 nID);       // 0x004E3B70
    void Allocate(int nSize);                   // 0x004C4F50 (folded 8-byte instantiation)
    int Contains(unsigned __int64 nID)
    {
        int nMatches = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == nID) {
                nMatches++;
            }
        }
        return nMatches;
    }
    void Add(unsigned __int64 nID)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = nID;
    }

    unsigned __int64* element;
    int num;
    int array_size;
};

#endif
