// CSWSCombatRound (0x004D28B0-0x004D6100) and the classes its state,
// action and lifecycle helpers touch. Shared classes come from
// item_property_handler.h.
#ifndef A_SWS_COMBAT_ROUND_H
#define A_SWS_COMBAT_ROUND_H

#include "a/item_property_handler.h"

#define OBJECT_INVALID 0x7f000000

extern "C" void* __cdecl memset(void* pDest, int nValue, unsigned int nCount);
#pragma intrinsic(memset)

class CSWSCreature;
class CSWSDoor;

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);  // 0x004126C0
    int WriteFieldSHORT(CResStruct* pStruct, short nValue, char* szLabel);          // 0x00412710
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                            // 0x00411A60
    unsigned short ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned short nDefault);                          // 0x00411B40
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);                           // 0x00411C20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411C90
    short ReadFieldSHORT(CResStruct* pStruct, char* szLabel, int& bSuccess, short nDefault); // 0x00411BB0
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault); // 0x00411D00
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);     // 0x00411EC0
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x004118C0
    unsigned long GetListCount(CResList* pList);                                    // 0x00411940
    int GetListElement(CResStruct* pElement, CResList* pList, unsigned long nIndex); // 0x00411990
};

class CGameObject;
class CSWSObject;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject* AsSWSObject();          // 0x10
    virtual void v14();
    virtual CSWSDoor* AsSWSDoor();              // 0x18
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();      // 0x30
};

class CSWSObject : public CGameObject {
public:
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetIsInConversation();          // 0x94
};

class CSWSDoor {
public:
    char pad_0x0[0x2cc];
    unsigned char m_bOpen;                      // 0x2cc
};

class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);              // 0x004AE750
    CSWSItem* GetItemByGameObjectID(unsigned long oid);         // 0x004AE760
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
    int GetPauseState(unsigned char nState);                    // 0x004AE980
};

class CSWCCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_idServer;                   // 0x4
};

class CSWCPartyMember {
public:
    CSWCCreature* GetCreature();                // 0x0060FB20
};

class CSWCParty {
public:
    CSWCPartyMember* GetPartyMember(int nIndex);    // 0x006346C0
};

class CClientExoApp {
public:
    CSWCParty* GetParty();                      // 0x005ED8B0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;             // 0x4
    CServerExoApp* m_pServerExoApp;             // 0x8
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};
extern CAppManager* g_pAppManager;

// Base time of a scheduled attack animation (0x008327DC).
extern int g_nCombatAnimationTime;

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

class CSWSCombatRoundAction {
public:
    CSWSCombatRoundAction() throw();            // 0x004D2FF0
    void Clear() throw();                       // 0x004D1D50
    int SaveData(CResGFF* pRes, CResStruct* pStruct);   // 0x004D1DD0
    int LoadData(CResGFF* pRes, CResStruct* pStruct);   // 0x004D1E90

    int m_nActionTimer;                         // 0x0
    unsigned short m_nAnimation;                // 0x4
    int m_nAnimationTime;                       // 0x8
    int m_nNumAttacks;                          // 0xc
    unsigned char m_nActionType;                // 0x10
    unsigned long m_oidTarget;                  // 0x14
    int m_bRetargettable;                       // 0x18
    int m_nInventorySlot;                       // 0x1c
    unsigned long m_oidItem;                    // 0x20
    int m_nSpellId;                             // 0x24
    int m_nSpellClass;                          // 0x28
    int m_nSpellParam2c;                        // 0x2c
    int m_nSpellParam30;                        // 0x30
    int m_nSpellParam34;                        // 0x34
    Vector m_vTargetLocation;                   // 0x38
    unsigned long m_oidSpellTarget;             // 0x44
    int m_nSpellParam48;                        // 0x48
    int m_nSpellParam4c;                        // 0x4c
    unsigned char m_nSpellParam50;              // 0x50
    int m_nParam54;                             // 0x54
    int m_nParam58;                             // 0x58
    int m_nParam5c;                             // 0x5c (feat for type 0xb)
    unsigned char m_nSpellParam60;              // 0x60
    unsigned long m_oidItemSpellTarget;         // 0x64
    int m_nItemSpellParam68;                    // 0x68
    int m_nItemSpellParam6c;                    // 0x6c
    unsigned short m_nParam70;                  // 0x70
    unsigned short m_nParam72;                  // 0x72
    int m_bCutscene;                            // 0x74
    int m_nCutsceneParam78;                     // 0x78
    int m_nCutsceneParam7c;                     // 0x7c
    int m_nCutsceneParam80;                     // 0x80
    int m_nParam84;                             // 0x84
};

// Result blocks shared by CSWSCombatAttackData (+0xc0) and
// CSWSCombatSubAttack (+0x50). Copied block by block (separate rep movsd
// runs), so they are separate POD member structs; the byte arrays are
// cleared element by element (one byte store each).
struct CSWSAttackResultA {                      // 0x20
    void Clear()
    {
        m_nBytes0[0] = 0; m_nBytes0[1] = 0; m_nBytes0[2] = 0; m_nBytes0[3] = 0;
        m_nBytes0[4] = 0; m_nBytes0[5] = 0; m_nBytes0[6] = 0; m_nBytes0[7] = 0;
        m_nField08 = 0;
        m_nBytes0c[0] = 0; m_nBytes0c[1] = 0; m_nBytes0c[2] = 0; m_nBytes0c[3] = 0;
        m_nBytes0c[4] = 0; m_nBytes0c[5] = 0; m_nBytes0c[6] = 0; m_nBytes0c[7] = 0;
        m_nBytes0c[8] = 0; m_nBytes0c[9] = 0;
        m_nField18 = 0;
        m_nField1c = 0;
    }
    unsigned char m_nBytes0[8];                 // 0x0
    int m_nField08;                             // 0x8
    unsigned char m_nBytes0c[10];               // 0xc
    int m_nField18;                             // 0x18
    int m_nField1c;                             // 0x1c
};

struct CSWSAttackResultB {                      // 0x14
    void Clear()
    {
        m_nByte00 = 0;
        m_nField04 = 0;
        m_nByte08 = 0;
        m_nField0c = 0;
        m_nByte10 = 0;
        m_nByte11 = 0;
    }
    unsigned char m_nByte00;                    // 0x0
    int m_nField04;                             // 0x4
    unsigned char m_nByte08;                    // 0x8
    int m_nField0c;                             // 0xc
    unsigned char m_nByte10;                    // 0x10
    unsigned char m_nByte11;                    // 0x11
};

struct CSWSAttackResultC {                      // 0x9
    void Clear()
    {
        m_nBytes[0] = 0; m_nBytes[1] = 0; m_nBytes[2] = 0; m_nBytes[3] = 0;
        m_nBytes[4] = 0; m_nBytes[5] = 0; m_nBytes[6] = 0; m_nBytes[7] = 0;
        m_nBytes[8] = 0;
    }
    unsigned char m_nBytes[9];
};

struct CSWSAttackResultD {                      // 0x2a
    void Clear()
    {
        m_nBytes[0] = 0; m_nBytes[1] = 0; m_nBytes[2] = 0; m_nBytes[3] = 0;
        m_nBytes[4] = 0; m_nBytes[5] = 0; m_nBytes[6] = 0; m_nBytes[7] = 0;
        m_nBytes[8] = 0; m_nBytes[9] = 0; m_nBytes[10] = 0; m_nBytes[11] = 0;
        for (int i = 0; i < 15; i++) {
            m_nDamage[i] = -1;
        }
    }
    unsigned char m_nBytes[12];                 // 0x0
    short m_nDamage[15];                        // 0xc
};

struct CSWSAttackResultE {                      // 0x18
    void Clear()
    {
        m_oidTarget = OBJECT_INVALID;
        m_nBytes[0] = 0; m_nBytes[1] = 0; m_nBytes[2] = 0; m_nBytes[3] = 0;
        m_nBytes[4] = 0; m_nBytes[5] = 0; m_nBytes[6] = 0; m_nBytes[7] = 0;
        m_nByte0c = 0xff;
        m_nField10 = 0;
        m_nField14 = 0;
    }
    unsigned long m_oidTarget;                  // 0x0
    unsigned char m_nBytes[8];                  // 0x4
    unsigned char m_nByte0c;                    // 0xc
    int m_nField10;                             // 0x10
    int m_nField14;                             // 0x14
};

class CSWSCombatSubAttack {
public:
    CSWSCombatSubAttack();                      // 0x004D32F0
    ~CSWSCombatSubAttack() {}

    int m_nAttackType;                          // 0x0
    unsigned char m_nAttackGroup;               // 0x4
    unsigned short m_nSpecialAttack;            // 0x6
    unsigned char m_nAttackResult;              // 0x8
    unsigned long m_nTime;                      // 0xc
    int m_nAnimationLength;                     // 0x10
    int m_nParam14;                             // 0x14
    int m_bRanged;                              // 0x18
    int m_nWeaponAttackType;                    // 0x1c
    Vector m_vRangedTarget;                     // 0x20
    int m_nParam2c;                             // 0x2c
    short m_nDamage[15];                        // 0x30
    CSWSAttackResultA m_resultA;                // 0x50
    CSWSAttackResultB m_resultB;                // 0x70
    CSWSAttackResultC m_resultC;                // 0x84
    CSWSAttackResultD m_resultD;                // 0x8e
    CSWSAttackResultE m_resultE;                // 0xb8
};

class CSWSCombatAttackData {
public:
    CSWSCombatAttackData();                     // 0x004D57F0
    ~CSWSCombatAttackData();                    // 0x004D4480
    void ClearAttackData();                     // 0x004D3010
    void RemoveTopSubAttack();                  // 0x004D32D0
    CSWSCombatSubAttack GetTopSubAttack();      // 0x004D4560
    void ResolveSubAttackToAttack(const CSWSCombatSubAttack& sub); // 0x004D27E0
    int GetDamage(int nFlags);                  // 0x004D1F80
    void AddBaseDamage(int nDamage);            // 0x004D2030
    int GetBaseDamage();                        // 0x004D2060
    void SetDamage(unsigned short nFlags, int nDamage);                 // 0x004D2070
    void AddDamage(unsigned short nFlags, int nDamage, int bAddToBase); // 0x004D20D0
    int GetTotalDamage(int nUnused);            // 0x004D2180 (placeholder name)
    int SaveData(CResGFF* pRes, CResStruct* pStruct);   // 0x004D2210
    int LoadData(CResGFF* pRes, CResStruct* pStruct);   // 0x004D2450
    void ResolveTopSubAttackToAttack();         // 0x004D45B0
    int IsSubAttackReady(unsigned long nTime);  // 0x004D45E0
    void ResolveAttackToMeleeSubAttack(int nAttackType, unsigned long nTime);
    void ResolveAttackToRangedSubAttack(int nAttackType, unsigned long nTime,
                                        int nAnimationLength, int nParam14,
                                        int nParam2c);

    unsigned char m_nAttackGroup;               // 0x0
    int m_nAnimationLength;                     // 0x4
    unsigned short m_nTime;                     // 0x8
    char pad_0xa[0x2];
    unsigned long m_oidReactObject;             // 0xc
    unsigned short m_nWeaponAttackType;         // 0x10
    unsigned short m_nField12;                  // 0x12
    unsigned short m_nField14;                  // 0x14
    char pad_0x16[0x2];
    int m_nField18;                             // 0x18
    short m_nDamage[15];                        // 0x1c
    unsigned char m_nAttackResult;              // 0x3a
    unsigned char m_nField3b;                   // 0x3b
    unsigned char m_nField3c;                   // 0x3c
    char pad_0x3d[0x3];
    int m_nField40;                             // 0x40
    int m_nField44;                             // 0x44
    int m_nField48;                             // 0x48
    int m_nField4c;                             // 0x4c
    int m_nField50;                             // 0x50
    unsigned char m_nField54;                   // 0x54
    char pad_0x55[0x3];
    int m_nField58;                             // 0x58
    unsigned char m_nAttackGroup5c;             // 0x5c
    char pad_0x5d[0x3];
    int m_nField60;                             // 0x60
    unsigned short m_nSpecialAttack;            // 0x64
    unsigned short m_nField66;                  // 0x66
    Vector m_vRangedTarget;                     // 0x68
    unsigned long m_oidAmmunition;              // 0x74
    CExoString m_sAttackDebugText;              // 0x78
    CExoString m_sDamageDebugText;              // 0x80
    CExoArrayList<CGameEffect*> m_alstOnHitEffects;         // 0x88
    CExoArrayList<unsigned long> m_alstList94;              // 0x94
    CExoArrayList<unsigned long> m_alstListA0;              // 0xa0
    CExoArrayList<CSWSCombatSubAttack> m_alstSubAttacks;    // 0xac
    int m_nFieldB8;                             // 0xb8
    int m_nFieldBC;                             // 0xbc
    CSWSAttackResultA m_resultA;                // 0xc0
    CSWSAttackResultB m_resultB;                // 0xe0
    CSWSAttackResultC m_resultC;                // 0xf4
    CSWSAttackResultD m_resultD;                // 0xfe
    CSWSAttackResultE m_resultE;                // 0x128
    CSWSAttackResultC m_resultC2;               // 0x140
};

class CSWSCombatRound {
public:
    CSWSCombatRound(CSWSCreature* pCreature);
    ~CSWSCombatRound();

#ifndef SWS_COMBAT_ROUND_OUTLINE_GETATTACK
    CSWSCombatAttackData* GetAttack(int nAttack) {
        if (nAttack < 0) {
            nAttack = 0;
        }
        return &m_pcAttackList[nAttack];
    }
#else
    // Callers outside nwscombatround.cpp reach the out-of-line copy.
    CSWSCombatAttackData* GetAttack(int nAttack);   // 0x004D2B50
#endif
    int GetTotalAttacks() { return m_nOffHandAttacks + m_nOnHandAttacks; }

    void SetRoundSpell(int nSpellParam, int nSpellId);
    void SetRoundPaused(int bPaused, unsigned long oidPausedBy);
    void SetPauseTimer(int nTime, int bInfinite);
    int GetActionValid(unsigned long oidActor, int nTime, int nDuration);
    void SetOffHandAttack(int bOffHand);
    int GetOffHandAttack();
    int CalculateOnHandAttacks(unsigned short nAction);
    int CalculateOffHandAttacks();
    unsigned char GetCurrentAction();
    void SetCurrentAction(unsigned char nAction);
    int ResolveChoreographedState(unsigned long oidTarget);
    int ResolveEngagedState(unsigned long oidTarget);
    int ResolveMasterState(unsigned long oidTarget, int* pbMaster);
    int HasCreatureWeapons();
    void DecrementRoundLength(int nTime, int bFlag);
    int GetActionPending();
    int GetSpellActionPending();
    CSWSCombatRoundAction* GetAction();
    void AddAction(CSWSCombatRoundAction* pAction, int nIndex);
    void RemoveAllActions();
    int RemoveLastAction();
    void ClearAllAttacks();
    void AddCutsceneAttackAction(unsigned long oidTarget, int nParam78, int nParam7c,
                                 int nParam80);
    void AddAttackAction(unsigned long oidTarget, unsigned short nFeat, int nParam84,
                         int nUnused, int nNumAttacks);
    void AddSWSpellAction(int nSpellId, int nSpellClass, int nParam2c, int nParam30,
                          int nParam34, Vector vTarget, unsigned long oidTarget,
                          int nParam48, int nParam4c, unsigned char nParam50, int nParam54,
                          int nParam5c, unsigned char nParam60, int nParam84);
    void AddSWItemSpellAction(unsigned long oidItemTarget, int nParam68, int nParam6c,
                              Vector vTarget, unsigned long oidTarget, int nParam48,
                              int nParam84);
    void AddCutsceneMoveActions(unsigned long oidTarget, Vector vTarget, int nParam54);
    void RemoveSpellAction();
    void AddEquipAction(unsigned long oidItem, int nInventorySlot, int nParam54,
                        int nParam84);
    void AddUnequipAction(unsigned long oidItem, unsigned long oidRepository, int nParam54);
    int GetWeaponAttackType();
    void UpdateAttackTargetForAllAction(unsigned long oidTarget);
    void DEBUGDumpCombatRoundToScreen(int nUnused, CExoString& sMessage);
    void EndCombatRound(int bFlag);                             // 0x004D4620
    int SaveCombatRound(CResGFF* pRes, CResStruct* pStruct);    // 0x004D3EC0
    int LoadCombatRound(CResGFF* pRes, CResStruct* pStruct);    // 0x004D5120
    void IncrementTimer(int nTime);
    void DecrementPauseTimer(int nTime);
    void ClearAllSpecialAttacks();
    CSWSItem* GetCurrentAttackWeapon(int nType);
    void StartCombatRound(unsigned long oidTarget, int bSynchronized, int bMaster,
                          unsigned short nAction, int nParam940);

    int m_nField000;                            // 0x0
    CSWSCombatAttackData m_pcAttackList[7];     // 0x4
    CExoArrayList<unsigned short> m_alstSpecialAttacks;     // 0x918
    CExoArrayList<unsigned short> m_alstSpecialAttackIds;   // 0x924
    unsigned short m_nAttackID;                 // 0x930
    char pad_0x932[0x2];
    int m_bRoundStarted;                        // 0x934
    int m_bSpellCastRound;                      // 0x938
    int m_nSpellId;                             // 0x93c
    int m_nField940;                            // 0x940
    int m_nTimer;                               // 0x944
    int m_nField948;                            // 0x948
    int m_nRoundLength;                         // 0x94c
    int m_nOverlapAmount;                       // 0x950
    int m_nBleedTimer;                          // 0x954
    int m_bRoundPaused;                         // 0x958
    unsigned long m_oidRoundPausedBy;           // 0x95c
    int m_nPauseTimer;                          // 0x960
    int m_bInfinitePause;                       // 0x964
    int m_nField968;                            // 0x968
    unsigned char m_nCurrentAttack;             // 0x96c
    char pad_0x96d[0x3];
    int m_bOffHandAttack;                       // 0x970
    unsigned char m_nField974;                  // 0x974
    char pad_0x975[0x3];
    int m_nField978;                            // 0x978
    int m_nField97c;                            // 0x97c
    int m_nField980;                            // 0x980
    int m_nField984;                            // 0x984
    int m_nField988;                            // 0x988
    unsigned long m_oidField98c;                // 0x98c
    int m_nOnHandAttacks;                       // 0x990
    int m_nOffHandAttacks;                      // 0x994
    int m_nOffHandAttacksTaken;                 // 0x998
    int m_nExtraAttacksTaken;                   // 0x99c
    int m_nEffectAttacks;                       // 0x9a0
    int m_nAdditionalAttacks;                   // 0x9a4
    unsigned char m_nField9a8;                  // 0x9a8
    char pad_0x9a9[0x3];
    unsigned long m_oidNewAttackTarget;         // 0x9ac
    CExoLinkedList<CSWSCombatRoundAction>* m_pScheduledActions; // 0x9b0
    CSWSCreature* m_pBaseCreature;              // 0x9b4
    int m_bSynchronized;                        // 0x9b8
    int m_bMaster;                              // 0x9bc
    unsigned long m_oidMaster;                  // 0x9c0
    unsigned long m_oidSlave;                   // 0x9c4
    unsigned char* m_pField9c8;                 // 0x9c8
    unsigned long m_oidField9cc;                // 0x9cc
    unsigned char m_nCurrentAction;             // 0x9d0
    char pad_0x9d1[0x3];
    int m_nField9d4;                            // 0x9d4
};

#endif
