// CSWSObject / CSWSCreature / CSWSCreatureStats (only what worker B uses).
#ifndef B_SWS_CREATURE_H
#define B_SWS_CREATURE_H

#include "b/exo_string.h"
#include "b/vector.h"
#include "b/exo_loc_string.h"
#include "b/exo_array_list.h"
#include "b/exo_linked_list.h"

class CSWSArea;
class CSWCCreature;

// Queued action: parameters start at +0x38.
class CSWSObjectActionNode {
public:
    unsigned long m_nActionId;                              // 0x0
    char pad_0x4[0x38 - 0x4];
    void* m_pParameter[13];                                 // 0x38
    unsigned short m_n6C;                                   // 0x6c
    unsigned short m_nNumParams;                            // 0x6e
};

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bArg);                                  // 0x00503E40
    void SetInteger(int nIndex, int nValue);                // 0x005036A0

    unsigned __int64 m_nID;                                 // 0x0
    unsigned short m_nType;                                 // 0x8
    unsigned short m_nSubType;                              // 0xa (low 3 bits: duration type)

    void SetDurationType(unsigned short nType) { m_nSubType = (m_nSubType & 0xfff8) | nType; }
    char pad_0xc[0x8c - 0xc];
};
#pragma pack(pop)

// CGameObject base of CSWSArea (at +0x11c, after CResHelperArea).
class CSWSAreaGameObject {
public:
    void* m_pVtable;                                        // 0x0
    unsigned long m_idSelf;                                 // 0x4

    unsigned long GetId() { return m_idSelf; }
};

class CSWVarTable {
public:
    void SetLocalBoolean(int nIndex, int bValue);           // 0x0059B040 (name guessed)
    void SetLocalNumber(int nIndex, char nValue);           // 0x0059B0D0 (name guessed)

    char pad_0x0[0x14];
};

struct CSWSAreaProperties {
    char pad_0x0[0x10];
    int m_n10;                                              // 0x10
};

class CResHelperArea {
public:
    char pad_0x0[0x11c];
};

class CSWSArea : public CResHelperArea, public CSWSAreaGameObject {
public:
    CSWSAreaGameObject& AsGameObject() { return *this; }
    void* GetRoom(const Vector& vPosition, int bArg);       // 0x004BB600
    void PlaySoundAtLocation(int nSound, class CScriptLocation* pLocation); // 0x00507E20 (name guessed)
    int ComputeSafeLocation(const Vector& vPosition, float fRadius, int nArg,
                            int bArg, Vector* pResult);     // 0x004BE860
    void IncrementPlayersInArea();                          // 0x00508C20
    void RemoveObjectFromArea(unsigned long nId);           // 0x0050DC40
    void AddObjectToArea(unsigned long nId, int bArg);      // 0x0050DFD0
    void DecrementPlayersInArea();                          // 0x00508C40

    char pad_0x124[0x158 - 0x124];
    CExoString m_sTag;                                      // 0x158
    char pad_0x160[0x1dc - 0x160];
    int m_n1DC;                                             // 0x1dc (name guessed)
    char pad_0x1e0[0x1f4 - 0x1e0];
    CSWVarTable m_VarTable;                                 // 0x1f4
    CSWSAreaProperties* m_pProperties;                      // 0x208 (name guessed)
};

class CSWCCreature {
public:
    void SetPartyMember(int bMember);                       // 0x0060EF50
    void SetFadeState(int nState);                          // 0x0063E220
    void SetDesiredFadeState(int a1, int a2, int a3, int a4); // 0x0063CE60

    char pad_0x0[0xe4];
    int m_bFadeDestroy;                                     // 0xe4
};

// 0x24-byte appearance block (creature +0xa50, CLastUpdateObject +0x0).
struct CSWSCreatureAppearanceInfo {
    unsigned long m_aParts[4];                              // 0x0
    unsigned short m_nAppearance;                           // 0x10
    unsigned char m_n12;                                    // 0x12
    unsigned char m_n13;                                    // 0x13
    unsigned char m_n14;                                    // 0x14
    unsigned char m_n15;                                    // 0x15
    unsigned char m_n16;                                    // 0x16
    unsigned char m_n17;                                    // 0x17
    unsigned char m_n18;                                    // 0x18
    unsigned char m_n19;                                    // 0x19
    unsigned char m_n1A;                                    // 0x1a
    char pad_0x1b[0x1c - 0x1b];
    unsigned long m_n1C;                                    // 0x1c
    unsigned char m_n20;                                    // 0x20
    char pad_0x21[0x24 - 0x21];
};

// 0x1c-byte action queue entry (creature +0x3a8, player last update +0x40).
struct CSWSActionQueueEntry {
    unsigned short m_n0;                                    // 0x0
    unsigned short m_n2;                                    // 0x2
    unsigned short m_n4;                                    // 0x4
    char pad_0x6[0x8 - 0x6];
    unsigned long m_n8;                                     // 0x8
    Vector m_vC;                                            // 0xc
    unsigned long m_n18;                                    // 0x18
};

class CSWSItem;
class CSWInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);           // 0x005A4C20
};

class CSWSFaction {
public:
    void AddMember(unsigned long oidMember, int bArg);      // 0x005BFA70
};

// Spell-like ability entry of the creature stats (0xc bytes).
struct CSWSStats_SpellLikeAbility {
    CSWSStats_SpellLikeAbility();                           // 0x006475C0

    int m_nSpellId;                                         // 0x0
    int m_bReadied;                                         // 0x4
    unsigned char m_nCasterLevel;                           // 0x8
};

struct CSWSStats_FeatUses {
    unsigned short m_nFeat;                                 // 0x0
    unsigned char m_nUsedToday;                             // 0x2
};

// Perception entry of a creature's visibility list.
struct CSWSCreatureVisibility {
    unsigned long m_oidTarget;                              // 0x0
    unsigned char m_bSeen : 1;                              // 0x4 bit 0
    unsigned char m_bHeard : 1;                             // 0x4 bit 1
    unsigned char m_b2 : 1;                                 // 0x4 bit 2
    unsigned char m_b3 : 1;                                 // 0x4 bit 3
    unsigned char m_bVisible : 1;                           // 0x4 bit 4
};

class CSWSCreatureStats {
public:
    CExoArrayList<unsigned short> m_lstFeats;               // 0x0
    CExoArrayList<CSWSStats_FeatUses*> m_lstFeatUses;       // 0xc
    CExoArrayList<unsigned short> m_lstBonusFeats;          // 0x18
    char pad_0x24[0x34 - 0x24];
    CExoLocString m_lsFirstName;                            // 0x34
    CExoLocString m_lsLastName;                             // 0x3c
    char pad_0x44[0x68 - 0x44];
    int m_nExperience;                                      // 0x68
    int m_bIsPC;                                            // 0x6c
    char pad_0x70[0x89 - 0x70];
    unsigned char m_nNumMultiClasses;                       // 0x89
    char pad_0x8a[0x128 - 0x8a];
    CExoArrayList<CSWSStats_SpellLikeAbility>* m_pSpellLikeAbilities; // 0x128
    char pad_0x12c[0x194 - 0x12c];
    int m_n194;                                             // 0x194 (read by VM command 0x0053C010)

    unsigned short GetNumFeats() { return (unsigned short)m_lstFeats.num; }
    unsigned short GetNumBonusFeats() { return (unsigned short)m_lstBonusFeats.num; }
    unsigned short GetFeat(unsigned short nIndex);          // 0x005A6920
    unsigned short GetBonusFeat(unsigned short nIndex);     // 0x005A6940

    unsigned char GetClass(int nMultiClass);                // 0x005A4E90
    unsigned char GetSpellUsesLeft(int nMultiClass, int nLevel); // 0x005A5650
    int GetCanUseSkill(unsigned char nSkill);               // 0x005AF880
    unsigned char GetLevel(int bArg);                       // 0x005A5FD0
    void SetMovementRate(int nRate);                        // 0x005A5680
    void UpdateCombatInformation();                         // 0x005ADDC0
    int ReadStatsFromGff(CResGFF* pRes, CResStruct* pStruct, void* pArg); // 0x005AFCE0
    void ReadSpellsFromGff(CResGFF* pRes, CResStruct* pStruct); // 0x005AEB30
    int CanLevelUp();                                       // 0x005A6810
    void AutoLevelUp(int bArg);                             // 0x005B27E0
};

class CResGFF;
class CResRef;
class CSWSCreature;
class CSWSScriptVarTable {
public:
    int GetInt(CExoString& sName);                          // 0x0059A530
    void SetInt(CExoString& sName, int nValue, int bArg);   // 0x0059A6F0
    void SetFloat(CExoString& sName, float fValue);         // 0x0059A8C0
    void SetString(CExoString& sName, CExoString& sValue);  // 0x0059A8E0
    void SetObject(CExoString& sName, unsigned long oid);   // 0x0059A900
    void DestroyFloat(CExoString& sName);                   // 0x0059AA50
    void DestroyString(CExoString& sName);                  // 0x0059AA60
    void DestroyObject(CExoString& sName);                  // 0x0059AA70

    char pad_0x0[0x24];
};

class CSWSCombatRound {
public:
    void ClearAllSpecialAttacks();                          // 0x004D4F60
};
class CExoLocString;
struct CResStruct;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual class CSWSObject* AsSWSObject();                // 0x10
    virtual void v05();
    virtual class CSWSDoor* AsSWSDoor();                    // 0x18
    virtual void v07();
    virtual class CSWSModule* AsSWSModule();                // 0x20
    virtual void v09();
    virtual CSWSArea* AsSWSArea();                          // 0x28
    virtual void v11();
    virtual CSWSCreature* AsSWSCreature();                  // 0x30
    virtual void v13();
    virtual class CSWSItem* AsSWSItem();                    // 0x38
    virtual void v15();
    virtual class CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject(); // 0x40
    virtual void v17();
    virtual class CSWSPlaceable* AsSWSPlaceable();          // 0x48
    virtual void v19();
    virtual class CSWSTrigger* AsSWSTrigger();              // 0x50
    virtual void v21(); virtual void v22();
    virtual void* AsSWS5C();                                // 0x5c (unknown object kind)
    virtual void v24();
    virtual class CSWSStore* AsSWSStore();                  // 0x64
    virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34();
    virtual CExoLocString* GetFirstName();                  // 0x8c
    virtual CExoLocString* GetLastName();                   // 0x90
    virtual int GetIsInConversation();                      // 0x94
    virtual short GetCreatureSize(int bArg);                // 0x98
    virtual short GetCurrentHitPoints(int bArg);            // 0x9c
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetDialogOwner(unsigned long oid);         // 0xc0 (name guessed)

    unsigned long m_idSelf;                                 // 0x4
    unsigned char m_nObjectType;                            // 0x8
    char pad_0x9[0x18 - 0x9];
    CExoString m_sTag;                                      // 0x18
    char pad_0x20[0x50 - 0x20];
    int m_bDialogPaused;                                    // 0x50 (name guessed)
    unsigned long m_oid54;                                  // 0x54 (name guessed)
    char pad_0x58[0x90 - 0x58];
    Vector m_vPosition;                                     // 0x90
    Vector m_vOrientation;                                  // 0x9c
    char pad_0xa8[0xd4 - 0xa8];
    int m_nD4;                                              // 0xd4 (items: 10001)
    char pad_0xd8[0xe8 - 0xd8];
    int m_bCommandable;                                     // 0xe8
    char pad_0xec[0x4];
    int m_bF0;                                              // 0xf0
};

class CGameObjectClient {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void* AsClientItem();                           // 0x34
};

class CResHelperModule {
public:
    char pad_0x0[0x1c];
};

class CSWSModule : public CResHelperModule, public CSWSAreaGameObject {
public:
    CSWSArea* GetArea();                                    // 0x004C30F0
    void RemoveObjectFromLookupTable(const CExoString& sTag, unsigned long nId); // 0x004C5EF0
    void AddObjectToLookupTable(const CExoString& sTag, unsigned long nId); // 0x004C7DE0
    void PackModuleIntoMessage(unsigned long nPlayerId);    // 0x004C3170 (V0, name guessed)

    char pad_0x24[0x5c - 0x24];
    CExoString m_sModuleName;                               // 0x5c
    CExoLocString m_lsModuleName;                           // 0x64
    char pad_0x6c[0x9c - 0x6c];
    CSWVarTable m_VarTable;                                 // 0x9c
    char pad_0xb0[0x148 - 0xb0];
    int m_n148;                                             // 0x148 (name guessed)
    char pad_0x14c[0x1f8 - 0x14c];
    CExoString m_sTag;                                      // 0x1f8
};

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nObjectType, unsigned long nId, int bArg); // 0x004CFCB0
    ~CSWSObject();                                          // 0x004D0220
    void SetOrientation(const Vector& vOrientation);        // 0x004CD170
    void SetPosition(const Vector& vPosition, int a2, int a3, int a4); // 0x004CD1A0
    void ClearAllEffects(int bArg);                         // 0x004D0940
    void ClearAllActions(int bArg);                         // 0x004CCD80
    void LoadObjectState(CResGFF* pRes, CResStruct* pStruct); // 0x004D1CF0
    void RemoveEffectById(unsigned __int64 nID);            // 0x004D06C0
    void ApplyEffect(class CGameEffect* pEffect, int a2, int a3); // 0x004D14F0
    CSWSArea* GetArea();                                    // 0x004CB120
    void SetArea(CSWSArea* pArea);                          // 0x004CB160
    class CGameObjectClient* GetClientObject();             // 0x004CC2B0
    void SetTag(CExoString& sTag);                          // 0x00553040
    void RemoveEffectByCreator(unsigned long nCreator);     // 0x004D0820
    int GetIsPaused();                                      // 0x0063E7F0 (shared `return 0`)
    int ReplyDialog(unsigned long nPlayerId, unsigned long nReply, int bArg,
                    unsigned long nArg, int bArg2);         // 0x004CB480
    void SetCommandable(int bCommandable) { m_bCommandable = bCommandable; }
    void AddAction(unsigned long nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2 = 0, void* p2 = 0,
                   int nType3 = 0, void* p3 = 0, int nType4 = 0, void* p4 = 0,
                   int nType5 = 0, void* p5 = 0, int nType6 = 0, void* p6 = 0,
                   int nType7 = 0, void* p7 = 0, int nType8 = 0, void* p8 = 0,
                   int nType9 = 0, void* p9 = 0, int nType10 = 0, void* p10 = 0,
                   int nType11 = 0, void* p11 = 0, int nType12 = 0, void* p12 = 0,
                   int nType13 = 0, void* p13 = 0);         // 0x004CEA20
    void AddActionToFront(unsigned long nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2 = 0, void* p2 = 0,
                   int nType3 = 0, void* p3 = 0, int nType4 = 0, void* p4 = 0,
                   int nType5 = 0, void* p5 = 0, int nType6 = 0, void* p6 = 0,
                   int nType7 = 0, void* p7 = 0, int nType8 = 0, void* p8 = 0,
                   int nType9 = 0, void* p9 = 0, int nType10 = 0, void* p10 = 0,
                   int nType11 = 0, void* p11 = 0, int nType12 = 0, void* p12 = 0,
                   int nType13 = 0, void* p13 = 0);         // 0x004CCB00

    int GetIsActionTimed(unsigned long nActionId);          // 0x0057A240 (name guessed)
    unsigned short GetAQActionIDByID(unsigned short nActionId, int bAlternate); // 0x0057A2B0
    void SpeakOneLinerConversation(CExoString& sResRef, unsigned long oidTalkTo); // 0x004CB220 (name guessed)
    void SetListenExpression(CExoString& sPattern, int nNumber); // 0x004CF8F0
    int GetEffectCount(unsigned long nMask);                // 0x004CAFE0 (name guessed)

    // AI actions (0x0057B290-0x0057CF00).
    unsigned long AIActionPauseDialog(class CSWSObjectActionNode* pNode);
    unsigned long AIActionResumeDialog(class CSWSObjectActionNode* pNode);
    unsigned long AIActionSpeakStrRef(class CSWSObjectActionNode* pNode);
    unsigned long AIActionSpeak(class CSWSObjectActionNode* pNode);
    unsigned long AIActionDoCommand(class CSWSObjectActionNode* pNode);
    unsigned long AIActionTakeItem(class CSWSObjectActionNode* pNode);
    unsigned long AIActionCloseDoor(class CSWSObjectActionNode* pNode);
    unsigned long AIActionSetCommandable(class CSWSObjectActionNode* pNode);
    unsigned long AIActionBarkString(class CSWSObjectActionNode* pNode);
    unsigned long AIActionPlaySound(class CSWSObjectActionNode* pNode);
    int AddUseObjectAction(unsigned long oidObject);
    int AddGiveItemActions(unsigned long oidItem, unsigned long oidGiveTo,
                           int nStackSize, int bArg);
    int AddTakeItemActions(unsigned long oidItem, unsigned long oidTakeFrom, int bArg);
    int AddDoCommandAction(void* pScript);
    int GetReputation(unsigned long oidTarget, int bArg);
    unsigned char GetAIStateReputation(unsigned long oidTarget);
};

class CSWSEvent;

class CSWSCreature : public CSWSObject {
public:
    int RemoveItem(class CSWSItem* pItem, int a2, int a3, int a4); // 0x00510EF0
    void AddExperience(int nXP);                            // 0x004EF930
    void SignalEvent(int nEvent, CSWSEvent* pEvent);        // 0x004EDE10
    void SetInParty(int bInParty, int bArg);                // 0x004FDB20
    void OnJoinParty();                                     // 0x00501C40
    CSWSCreature(unsigned long nId, int bArg);              // 0x004F7A10
    void SetSize(int nSize);                                // 0x004CC280
    void ReadScriptsFromGff(CResGFF* pRes, CResStruct* pStruct); // 0x004EBF20
    void LoadFromGff(CResGFF* pRes, CResStruct* pStruct, int a3, int a4, int a5); // 0x004FFDA0
    void PostLoad();                                        // 0x004F1C40
    void LoadCreatureState(CResGFF* pRes, CResStruct* pStruct); // 0x004F17D0
    int GetGold();                                          // 0x004EDD60
    int GetCreatureValue518B30();                           // 0x00518B30 (name guessed)
    CSWSFaction* GetFaction();                              // 0x00513FC0
    class CItemRepository* GetItemRepository(int bArg);     // 0x004EF770
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);   // 0x00500610
    int LoadFromTemplate(const CResRef& cResRef, int bArg); // 0x005026D0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int a5, int a6); // 0x004FA100
    CSWCCreature* GetClientCreature();                      // 0x004EFB20
    void SetActivity(int nActivity, int bOn, int nArg);     // 0x004F00A0
    void AddGold(int nGold, int bFeedback);                 // 0x004F3DB0
    void AddPazaakCard(class CSWSItem* pCard);              // 0x004EFB40
    int GetDeadTemp();                                      // 0x004EF890
    void SetCombatState(int a, int b);                      // 0x0050EE80 (name guessed)
    void ClearActivityFlag(int nFlag);                      // 0x004F8770 (name guessed)
    void AddDriveAction(int nId, Vector* pPosition, int nAngle, int nA, int nB,
                        int nC);                            // 0x004EB510
    void TransferGold(unsigned long oidTarget, Vector* pPosition, int nAmount); // 0x004FD730 (name guessed)
    int GetCreatureReputation(unsigned long oidTarget, int nFaction); // 0x00513FF0 (name guessed)
    void AddMoveToPointAction(unsigned short nGroupId, const Vector& vNewPos,
                              unsigned long oidArea, unsigned long oidTarget, int bRun,
                              float fRange, int a7, int a8, int a9, int a10, int a11,
                              int a12, int a13, const Vector& vFace, int a15, int a16,
                              int a17);                     // 0x004F8B60 (name guessed)
    int AcquireItem(class CSWSItem** ppItem, unsigned long oidPossessor,
                    unsigned long oidTarget, int bDisplayFeedback); // 0x005158E0
    void SetAIStateFlag(int nFlag, int bOn)
    {
        if (bOn) {
            m_nAIStateFlags |= nFlag;
        } else {
            m_nAIStateFlags &= ~nFlag;
        }
    }
    void ActivityManager(int nActivity);                    // 0x004F87D0 (name guessed)
    void UpdateAppearanceForEquippedItems();                // 0x004ED620
    unsigned char GetMode();                                // 0x004EDAE0 (name guessed)
    int GetWeaponSlotFlag(unsigned long nSlot);             // 0x004EFDD0 (name guessed)
    int CanUseItem(class CSWSItem* pItem, int bArg);        // 0x0051B440 (name guessed)
    class CSWSJournal* GetJournal();                        // 0x004EDB00
    void AddEquipActions(class CSWSItem* pItem, int nSlot, int a3, int a4, int bInstant,
                         int a6, int a7);                   // 0x004F0420 (name guessed)
    void AddObjectAction(unsigned long oidTarget, Vector* pPosition, int nArg); // 0x004F0840 (name guessed)
    unsigned char SavingThrowRoll(unsigned char nSave, unsigned short nDC,
                                  unsigned short nSaveType, unsigned long oidSaveVersus,
                                  int bFeedback, unsigned short nFeat,
                                  int bArg);                // 0x005B92B0 (name guessed)

    char pad_0xf4[0xf8 - 0xf4];
    int m_nF8;                                              // 0xf8
    CExoLinkedList<class CSWSObjectActionNode> m_lQueuedActions; // 0xfc
    CSWSScriptVarTable m_ScriptVars;                        // 0x100
    CGameEffect** m_pAppliedEffects;                        // 0x124
    int m_nAppliedEffects;                                  // 0x128
    char pad_0x12c[0x22c - 0x12c];
    int m_nXPDebt;                                          // 0x22c
    char pad_0x230[0x310 - 0x230];
    unsigned long m_oidLastArea;                            // 0x310
    Vector m_vLastPosition;                                 // 0x314
    int m_n320;                                             // 0x320
    char pad_0x324[0x340 - 0x324];
    int m_n340;                                             // 0x340
    char pad_0x344[0x348 - 0x344];
    int m_nCharSheetViewers;                                // 0x348 (name guessed)
    char pad_0x34c[0x384 - 0x34c];
    unsigned long m_n384;                                   // 0x384
    char pad_0x388[0x3a8 - 0x388];
    CSWSActionQueueEntry m_aActionQueue[10];                // 0x3a8
    char pad_0x4c0[0x4d1 - 0x4c0];
    unsigned char m_bPlayerCharacter;                       // 0x4d1
    unsigned char m_n4D2;                                   // 0x4d2
    char pad_0x4d3[0x4e0 - 0x4d3];
    int m_n4E0;                                             // 0x4e0
    char pad_0x4e4[0x4f8 - 0x4e4];
    int m_nCreatureSize;                                    // 0x4f8
    char pad_0x4fc[0x8e8 - 0x4fc];
    int m_n8E8;                                             // 0x8e8
    char pad_0x8ec[0x920 - 0x8ec];
    CExoArrayList<CSWSCreatureVisibility*> m_aVisibility;  // 0x920
    char pad_0x92c[0x9c8 - 0x92c];
    class CSWSCombatRound* m_pCombatRound;                  // 0x9c8
    char pad_0x9cc[0x9d4 - 0x9cc];
    int m_bIsPC;                                            // 0x9d4
    unsigned short m_n9D8;                                  // 0x9d8
    char pad_0x9da[0x9dc - 0x9da];
    int m_n9DC;                                             // 0x9dc
    char pad_0x9e0[0xa00 - 0x9e0];
    int m_nAIStateFlags;                                    // 0xa00 (bit 2: dialog paused)
    char pad_0xa04[0xa10 - 0xa04];
    unsigned char m_nA10;                                   // 0xa10
    char pad_0xa11[0xa2c - 0xa11];
    CSWInventory* m_pInventory;                             // 0xa2c
    char pad_0xa30[0xa48 - 0xa30];
    int m_bDisguised;                                       // 0xa48
    unsigned short m_nDisguiseAppearance;                   // 0xa4c
    char pad_0xa4e[0x2];
    CSWSCreatureAppearanceInfo m_cAppearance;               // 0xa50
    CSWSCreatureStats* m_pStats;                            // 0xa74
    char pad_0xa78[0xa88 - 0xa78];
    int m_bInParty;                                         // 0xa88
    char pad_0xa8c[0xac8 - 0xa8c];
};

#endif
