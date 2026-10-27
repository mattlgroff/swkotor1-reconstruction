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

#include "b/game_effect.h"

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
    int GetBoolean(int nIndex);                             // 0x0059B000
    unsigned char GetNumber(int nIndex);                    // 0x0059B0B0

    char pad_0x0[0x14];
};

// Area music / ambient sound state (vtable 0x0074C268, no destructor slot).
class CSWSAmbientSound {
public:
    virtual void PlayMusic(int bPlay);                      // 0x00 0x005C9900
    virtual void SetMusicDelay(int nDelay);                 // 0x04 0x005C99A0
    virtual void SetMusicDayTrack(int nTrack);              // 0x08 0x005C9A40
    virtual void SetMusicNightTrack(int nTrack);            // 0x0c 0x005C9AE0
    virtual void PlayBattleMusic(int bPlay);                // 0x10 0x005C9B90
    virtual void SetBattleMusicTrack(int nTrack);           // 0x14 0x005C9C30
    virtual void PlayAmbientSound(int bPlay);               // 0x18 0x005C9CD0
    virtual void SetAmbientDayTrack(int nTrack);            // 0x1c 0x005C9D70
    virtual void SetAmbientNightTrack(int nTrack);          // 0x20 0x005C9E20
    void SetAmbientDayVolume(int nVolume);                  // 0x005C9ED0
    void SetAmbientNightVolume(int nVolume);                // 0x005C9F80

    int m_bMusicPlaying;                                    // 0x04
    int m_nMusicDelay;                                      // 0x08
    int m_nMusicDay;                                        // 0x0c
    int m_nMusicNight;                                      // 0x10
    char pad_0x14[0x18 - 0x14];
    int m_nMusicBattle;                                     // 0x18 (name guessed)
};

class CResHelperArea {
public:
    char pad_0x0[0x11c];
};

class CSWSArea : public CResHelperArea, public CSWSAreaGameObject {
public:
    CSWSAreaGameObject& AsGameObject() { return *this; }
    void* GetRoom(const Vector& vPosition, int bArg);       // 0x004BB600
    float ComputeHeight(const Vector& vPosition, int bArg, class CSWSRoom** ppRoom); // 0x004BC380
    void PlaySoundAtLocation(int nSound, class CScriptLocation* pLocation); // 0x00507E20 (name guessed)
    void SetPlayerRestrictMode(int bRestrict);              // 0x00506B90
    void SetStealthXPMax(unsigned long nMax);               // 0x00506A80
    void SetSunFogColor(const Vector& vColor);              // 0x00506AE0 (name guessed)
    class CSWCArea* GetClientArea();                        // 0x00506A10 (name guessed)
    void SetStealthXPCurrent(unsigned long nCurrent);       // 0x00506AA0
    void AwardStealthXP(unsigned long oidCreature);         // 0x00508770 (name guessed)
    void ApplyEffect(class CGameEffect* pEffect, const Vector& vPosition, const Vector& vOrientation); // 0x0050C6B0
    Vector ComputeAwayVector(const Vector& vPosition, const Vector& vAwayFrom, float fRange,
                             int nArg);                     // 0x004BDF80 (name guessed)
    int ComputeSafeLocation(const Vector& vPosition, float fRadius, int nArg,
                            int bArg, Vector* pResult);     // 0x004BE860
    void IncrementPlayersInArea();                          // 0x00508C20
    void RemoveObjectFromArea(unsigned long nId);           // 0x0050DC40
    void AddObjectToArea(unsigned long nId, int bArg);      // 0x0050DFD0
    void DecrementPlayersInArea();                          // 0x00508C40
    int GetFirstObjectIdInArea(unsigned long* pOid);        // 0x005071B0
    int GetNextObjectIdInArea(unsigned long* pOid);         // 0x005071E0

    char pad_0x124[0x12c - 0x124];
    CExoArrayList<unsigned long> m_aTrapList;               // 0x12c (name guessed)
    char pad_0x138[0x158 - 0x138];
    CExoString m_sTag;                                      // 0x158
    char pad_0x160[0x1d4 - 0x160];
    unsigned long m_oidEnteringObject;                      // 0x1d4 (name guessed)
    unsigned long m_oidExitingObject;                       // 0x1d8 (name guessed)
    int m_n1DC;                                             // 0x1dc (name guessed)
    char pad_0x1e0[0x1f4 - 0x1e0];
    CSWVarTable m_VarTable;                                 // 0x1f4
    CSWSAmbientSound* m_pAmbientSound;                      // 0x208
    char pad_0x20c[0x2ac - 0x20c];
    int m_bUnescapable;                                     // 0x2ac (name guessed)
    char pad_0x2b0[0x2b4 - 0x2b0];
    unsigned long m_nStealthXPMax;                          // 0x2b4
    unsigned long m_nStealthXPCurrent;                      // 0x2b8
    unsigned long m_nStealthXPLoss;                         // 0x2bc
    int m_bStealthXPEnabled;                                // 0x2c0
};

class CSWCCreature {
public:
    void SetPartyMember(int bMember);                       // 0x0060EF50
    void SetFadeState(int nState);                          // 0x0063E220
    void SetDesiredFadeState(int a1, int a2, int a3, int a4); // 0x0063CE60

    void ClearAllActions();                                 // 0x0063D470 (CSWCObject)

    char pad_0x0[0x4];
    unsigned long m_idSelf;                                 // 0x04
    char pad_0x8[0xe4 - 0x8];
    int m_bFadeDestroy;                                     // 0xe4
    char pad_0xe8[0x3a4 - 0xe8];
    int m_bPartyMember;                                     // 0x3a4 (name guessed)
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
    int GetItemInInventory(class CSWSItem* pItem);          // 0x005A4950
    CSWSItem* GetItemInSlot(unsigned long nSlot);           // 0x005A4C20
};

class CSWSFaction {
public:
    unsigned long GetWeakestMember(unsigned long oidSource, int bMustBeVisible);   // 0x005BEA80
    unsigned long GetStrongestMember(unsigned long oidSource, int bMustBeVisible); // 0x005BEBC0
    unsigned long GetMostDamagedMember(unsigned long oidSource, int bMustBeVisible); // 0x005BED00
    unsigned long GetLeastDamagedMember(unsigned long oidSource, int bMustBeVisible); // 0x005BEE70
    int GetGold();                                          // 0x005BEFE0
    int GetAverageReputation(unsigned long oidSource);      // 0x005BF020
    int GetAverageGoodEvilAlignment();                      // 0x005BF090
    int GetAverageLevel();                                  // 0x005BF140
    int GetAverageXP();                                     // 0x005BF1A0
    int GetMostFrequentClass();                             // 0x005BF1F0
    unsigned long GetWorstAC(unsigned long oidSource, int bMustBeVisible); // 0x005BF340
    unsigned long GetBestAC(unsigned long oidSource, int bMustBeVisible);  // 0x005BF480
    unsigned long GetLeader();                              // 0x005BF5C0
    unsigned long GetFactionMember(int nIndex, unsigned long bPCOnly); // 0x005BF6E0
    void AddMember(unsigned long oidMember, int bArg);      // 0x005BFA70
    void RemoveMember(unsigned long oidMember);             // 0x005BF920

    CExoArrayList<unsigned long> m_aMembers;                // 0x00 (name guessed)
    char pad_0xc[0x14 - 0xc];
    int m_nCurrentMember;                                   // 0x14 (GetNextFactionMember cursor)
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
    char pad_0x70[0x78 - 0x70];
    int m_nFactionId;                                       // 0x78
    char pad_0x7c[0x84 - 0x7c];
    float m_fChallengeRating;                               // 0x84
    char pad_0x88[0x89 - 0x88];
    unsigned char m_nNumMultiClasses;                       // 0x89
    char pad_0x8a[0xdc - 0x8a];
    unsigned short m_nRace;                                 // 0xdc
    char pad_0xde[0xe8 - 0xde];
    unsigned char m_nSubRace;                               // 0xe8
    char pad_0xe9[0xea - 0xe9];
    char m_nStrengthModifier;                               // 0xea (name guessed)
    char pad_0xeb[0xee - 0xeb];
    char m_nConstitutionModifier;                           // 0xee (name guessed)
    char pad_0xef[0xf0 - 0xef];
    char m_nIntelligenceModifier;                           // 0xf0 (name guessed)
    char pad_0xf1[0xf2 - 0xf1];
    char m_nWisdomModifier;                                 // 0xf2 (name guessed)
    char pad_0xf3[0xf4 - 0xf3];
    char m_nCharismaModifier;                               // 0xf4 (name guessed)
    char pad_0xf5[0x124 - 0xf5];
    short m_nForcePointsBonus;                              // 0x124 (name guessed)
    short m_nCurrentForcePoints;                            // 0x126 (name guessed)
    CExoArrayList<CSWSStats_SpellLikeAbility>* m_pSpellLikeAbilities; // 0x128
    char pad_0x12c[0x13a - 0x12c];
    unsigned short m_nAIStyle;                              // 0x13a
    char pad_0x13c[0x17e - 0x13c];
    short m_nGoodEvil;                                      // 0x17e
    char pad_0x180[0x186 - 0x180];
    unsigned short m_nAppearanceType;                       // 0x186 (name guessed)
    char pad_0x188[0x194 - 0x188];
    int m_n194;                                             // 0x194 (read by VM command 0x0053C010)

    unsigned short GetNumFeats() { return (unsigned short)m_lstFeats.num; }
    unsigned short GetNumBonusFeats() { return (unsigned short)m_lstBonusFeats.num; }
    unsigned short GetFeat(unsigned short nIndex);          // 0x005A6920
    unsigned short GetBonusFeat(unsigned short nIndex);     // 0x005A6940

    unsigned char GetClass(int nMultiClass);                // 0x005A4E90
    unsigned char GetClass(unsigned char nMultiClass);      // 0x005A4E90 (byte argument)
    unsigned char GetClassLevel(unsigned char nMultiClass, int bArg); // 0x005A5090
    unsigned char GetMemorizedSpellClass(unsigned long nSpellId); // 0x005A6E30
    int HasReadySpell(unsigned long nSpellId);              // 0x005A68E0
    int GetCreatureTalent(int nCategory, int nCRMax, int* pType, int* pId, unsigned char* pMultiClass,
                          unsigned long* pItem, int* pItemPropertyIndex, unsigned char* pCasterLevel,
                          int nInclusion, int nExcludeType, int nExcludeId); // 0x005B23A0 (name guessed)
    unsigned char GetSpellUsesLeft(int nMultiClass, int nLevel); // 0x005A5650
    int GetMeleeAttackBonus(int bArg1, int bArg2, int bArg3, int bArg4); // 0x005A7770 (Mac debug map 2/2)
    char GetForceResistance();                              // 0x005A5A60 (Mac debug map 2/2)
    int GetRangedAttackBonus(int bArg1, int bArg2, int bArg3); // 0x005A7B60 (Mac debug map 2/2)
    short GetArmorClassVersus(class CSWSCreature* pAttacker, int bArg); // 0x005AD0E0 (Mac debug map 2/2)
    unsigned char GetSpellUsesLeft(unsigned long nSpell, unsigned char nMultiClass,
                                   unsigned char nSpellLevel, unsigned char nMetaType); // 0x005A5550
    char GetSkillRank(unsigned char nSkill, class CSWSObject* pVersus, int bBaseOnly); // 0x005AA570
    int GetCanUseSkill(unsigned char nSkill);               // 0x005AF880
    int GetEffectImmunity(unsigned char nType, class CSWSCreature* pVersus); // 0x005A6960
    unsigned char GetFeatRemainingUses(unsigned short nFeat); // 0x005A6680
    int HasSpell(unsigned char nList, unsigned long nSpell, int bUpgrade); // 0x005A6E70
    unsigned char GetLevel(int bArg);                       // 0x005A5FD0
    int GetHasTalent(int nType, int nId);                   // 0x005AAA30 (name guessed)
    int GetIsLinkImmune(class CGameEffect* pEffect);        // 0x005A6A90 (name guessed)
    void SetMovementRate(int nRate);                        // 0x005A5680
    void UpdateCombatInformation();                         // 0x005ADDC0
    int ReadStatsFromGff(CResGFF* pRes, CResStruct* pStruct, void* pArg); // 0x005AFCE0
    void ReadSpellsFromGff(CResGFF* pRes, CResStruct* pStruct); // 0x005AEB30
    int CanLevelUp();                                       // 0x005A6810
    void AutoLevelUp(int bArg);                             // 0x005B27E0
    unsigned long GetExpNeededForNextLevel();               // 0x005A5490
    void AddClass(unsigned char nClass, unsigned char nArg); // 0x005A5D10
    unsigned char GetSimpleAlignmentGoodEvil();             // 0x005A5110
    unsigned char GetSTRStat();                             // 0x005A6190 (name guessed)
    unsigned char GetDEXStat();                             // 0x005A6550 (name guessed)
    unsigned char GetCONStat();                             // 0x005A6250 (name guessed)
    unsigned char GetINTStat();                             // 0x005A6310 (name guessed)
    unsigned char GetWISStat();                             // 0x005A63D0 (name guessed)
    unsigned char GetCHAStat();                             // 0x005A6490 (name guessed)
    char GetDEXMod(int bArmorDexCap);                       // 0x005A5330 (name guessed)
    char GetFortSavingThrow(int bExcludeEffects);           // 0x005AB810
    char GetWillSavingThrow(int bExcludeEffects);           // 0x005AB880
    char GetReflexSavingThrow(int bExcludeEffects);         // 0x005AB8F0
    void ModifyAlignment(short nDelta);                     // 0x005AE610
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

class CSWSCombatAttackData;

class CSWSCombatRound {
public:
    void ClearAllSpecialAttacks();                          // 0x004D4F60
    CSWSCombatAttackData* GetAttack(int nAttack);           // 0x004D2B50 (Mac debug map 2/2)

    char pad_0x0[0x96c];
    unsigned char m_nCurrentAttack;                         // 0x96c
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
    virtual void v29();
    virtual void EventHandler(unsigned long nEventId, unsigned long nCallerObjectId, void* pScript,
                              unsigned long nCalendarDay, unsigned long nTimeOfDay); // 0x78
    virtual void SetDialogDelay7C(int nMilliseconds);       // 0x7c (name guessed)
    virtual void v32();
    virtual void v33(); virtual void v34();
    virtual CExoLocString* GetFirstName();                  // 0x8c
    virtual CExoLocString* GetLastName();                   // 0x90
    virtual int GetObjectState94();                      // 0x94
    virtual short GetCreatureSize(int bArg);                // 0x98
    virtual short GetCurrentHitPoints(int bArg);            // 0x9c
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45();
    virtual unsigned long GetLastSpellId();                 // 0xb8 (placeholder name, as in bucket a)
    virtual void v47();
    virtual void SetDialogOwner(unsigned long oid);         // 0xc0 (name guessed)

    unsigned long GetId() { return m_idSelf; }

    unsigned long m_idSelf;                                 // 0x4
    unsigned char m_nObjectType;                            // 0x8
    char pad_0x9[0x18 - 0x9];
    CExoString m_sTag;                                      // 0x18
    char pad_0x20[0x50 - 0x20];
    int m_bDialogPaused;                                    // 0x50 (name guessed)
    unsigned long m_oid54;                                  // 0x54 (name guessed)
    char pad_0x58[0x78 - 0x58];
    int m_nAILevel;                                         // 0x78 (name guessed)
    char pad_0x7c[0x8c - 0x7c];
    unsigned long m_oidArea;                                // 0x8c
    Vector m_vPosition;                                     // 0x90
    Vector m_vOrientation;                                  // 0x9c
    char pad_0xa8[0xd4 - 0xa8];
    int m_nD4;                                              // 0xd4 (items: 10001)
    char pad_0xd8[0xe8 - 0xd8];
    int m_bCommandable;                                     // 0xe8
    int m_bEC;                                              // 0xec (name guessed)
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
    unsigned long GetWaypoint(const CExoString& sTag);      // 0x004C6E00
    unsigned long GetObjectByTag(const CExoString& sTag, int nNth); // 0x004C5FE0
    void RemoveObjectFromLookupTable(const CExoString& sTag, unsigned long nId); // 0x004C5EF0
    void AddObjectToLookupTable(const CExoString& sTag, unsigned long nId); // 0x004C7DE0
    void PackModuleIntoMessage(unsigned long nPlayerId);    // 0x004C3170 (V0, name guessed)
    void AddWorldJournalEntry(unsigned long nIndex, const CExoString& sEntry, const CExoString& sTitle,
                              unsigned long nCalendarDay, unsigned long nTimeOfDay); // 0x004C7ED0
    void AddWorldJournalEntryStrref(unsigned long nStrref, unsigned long nTitleStrref,
                                    unsigned long nCalendarDay, unsigned long nTimeOfDay); // 0x004C8030 (name guessed)
    void DeleteWorldJournalEntry(unsigned long nIndex);     // 0x004C8170
    void DeleteWorldJournalEntryStrref(unsigned long nStrref); // 0x004C8230
    void DeleteWorldJournalAllEntries();                    // 0x004C82F0
    void UpdateTime(unsigned long nDay, unsigned long nTime, unsigned long nDelta); // 0x004C6210

    char pad_0x24[0x5c - 0x24];
    CExoString m_sModuleName;                               // 0x5c
    CExoLocString m_lsModuleName;                           // 0x64
    char pad_0x6c[0x9c - 0x6c];
    CSWVarTable m_VarTable;                                 // 0x9c
    char pad_0xb0[0x148 - 0xb0];
    int m_n148;                                             // 0x148 (name guessed)
    unsigned long m_oidEnteringObject;                      // 0x14c (name guessed)
    unsigned long m_oidExitingObject;                       // 0x150 (name guessed)
    unsigned long m_oidItemAcquired;                        // 0x154
    unsigned long m_oidItemAcquiredFrom;                    // 0x158
    unsigned long m_oidItemLost;                            // 0x15c
    unsigned long m_oidItemLostBy;                          // 0x160
    char pad_0x164[0x1bc - 0x164];
    unsigned char m_nTimeOfDay;                             // 0x1bc (1 day, 2 night, 3 dawn)
    char pad_0x1bd[0x1f8 - 0x1bd];
    CExoString m_sTag;                                      // 0x1f8
    char pad_0x200[0x218 - 0x200];
    class CSWSAreaMap* m_pAreaMap;                          // 0x218 (name guessed)
};

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nObjectType, unsigned long nId, int bArg); // 0x004CFCB0
    ~CSWSObject();                                          // 0x004D0220
    void SetOrientation(const Vector& vOrientation);        // 0x004CD170
    int DoSpellLevelAbsorption(CSWSObject* pCaster);        // 0x004D13A0 (Mac debug map 2/2)
    int DoSpellImmunity(CSWSObject* pCaster);               // 0x004CCFC0 (name guessed)
    void SetDeleteNoFade(int bNoFade);                      // 0x004CE920
    void SetFadeOutTime(int nMilliseconds);                 // 0x004CE8A0
    void SetPosition(const Vector& vPosition, int a2, int a3, int a4); // 0x004CD1A0
    void ClearAllEffects(int bArg);                         // 0x004D0940
    void SetCurrentHitPoints(int nHitPoints);               // 0x004CC280
    void ClearAllActions(int bArg);                         // 0x004CCD80
    int GetHasSpellEffectApplied(int nSpellId);             // 0x004CDAC0
    int GetHasFeatEffectApplied(unsigned short nFeat);      // 0x004CE680
    void LoadObjectState(CResGFF* pRes, CResStruct* pStruct); // 0x004D1CF0
    void RemoveEffectById(unsigned __int64 nID);            // 0x004D06C0
    void ApplyEffect(class CGameEffect* pEffect, int a2, int a3); // 0x004D14F0
    CSWSArea* GetArea();                                    // 0x004CB120
    void SetArea(CSWSArea* pArea);                          // 0x004CB160
    class CGameObjectClient* GetClientObject();             // 0x004CC2B0
    void SetTag(CExoString& sTag);                          // 0x00553040
    void RemoveEffectByCreator(unsigned long nCreator);     // 0x004D0820
    int GetIsPaused();                                      // 0x0063E7F0 (shared `return 0`)
    int StartDialog(unsigned long oidTarget, const CExoString& sDialog,
                    int bPrivate);                          // 0x004CF490 (name guessed)
    void ClearUpdateFlag(unsigned long nBit);               // 0x004CC250
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

    int GetAcceptableAction(unsigned long nActionId);          // 0x0057A240 (Mac debug map 2/2)
    unsigned short GetAQActionIDByID(unsigned short nActionId, int bAlternate); // 0x0057A2B0
    void SpeakOneLinerConversation(CExoString& sResRef, unsigned long oidTalkTo); // 0x004CB220 (name guessed)
    void SetListenExpression(CExoString& sPattern, int nNumber); // 0x004CF8F0
    int GetEffectCount(unsigned long nMask);                // 0x004CAFE0 (name guessed)
    class CScriptLocation GetScriptLocation();              // 0x004CB7B0

    // AI actions (0x0057B290-0x0057CF00).
    unsigned long AIActionPauseDialog(class CSWSObjectActionNode* pNode);
    unsigned long AIActionResumeDialog(class CSWSObjectActionNode* pNode);
    unsigned long AIActionSpeakStrRef(class CSWSObjectActionNode* pNode);
    unsigned long AIActionSpeak(class CSWSObjectActionNode* pNode);
    unsigned long AIActionDoCommand(class CSWSObjectActionNode* pNode);
    unsigned long AIActionTakeItem(class CSWSObjectActionNode* pNode);
    unsigned long AIActionCloseDoor(class CSWSObjectActionNode* pNode);
    unsigned long AIActionOpenDoor(class CSWSObjectActionNode* pNode);     // 0x0057D490
    unsigned long AIActionLockObject(class CSWSObjectActionNode* pNode);   // 0x0057BEC0
    unsigned long AIActionUnlockObject(class CSWSObjectActionNode* pNode); // 0x0057D9D0
    unsigned long AIActionUseObject(class CSWSObjectActionNode* pNode);    // 0x0057E8C0
    unsigned long AIActionPlayAnimation(class CSWSObjectActionNode* pNode); // 0x0057D080
    unsigned long AIActionGiveItem(class CSWSObjectActionNode* pNode);     // 0x0057B6A0
    unsigned long AIActionSetCommandable(class CSWSObjectActionNode* pNode);
    unsigned long AIActionBarkString(class CSWSObjectActionNode* pNode);
    unsigned long AIActionPlaySound(class CSWSObjectActionNode* pNode);
    int AddUseObjectAction(unsigned long oidObject);
    int AddUnlockObjectAction(unsigned long oidDoor, unsigned long oidThievesTool, int bArg); // 0x004CFB80
    int AddLockObjectAction(unsigned long oidDoor);          // 0x004CFC20
    int AddCloseDoorAction(unsigned long oidDoor, int bArg); // 0x004CFB20
    int AddOpenDoorAction(unsigned long oidDoor, int bArg);  // 0x004CFAC0
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
    CSWSCreatureStats* GetStats() { return m_pStats; }
    int RemoveItem(class CSWSItem* pItem, int a2, int a3, int a4); // 0x00510EF0
    void AddExperience(int nXP);                            // 0x004EF930
    void CancelCombat(int bArg);                            // 0x004FDAA0
    void SetLockOrientationToObject(unsigned long oidTarget, int bArg); // 0x004F34A0
    void SetFormation(unsigned long oidAnchor, int nFormation, int nPosition); // 0x00514700 (name guessed)
    int IsDebilitated();                                    // 0x005B4880
    void SignalEvent(int nEvent, CSWSEvent* pEvent);        // 0x004EDE10
    void SendFeedbackMessage(unsigned short nMessage, class CSWCCMessageData* pData); // 0x004EDE10
    void SetInParty(int bInParty, int bArg);                // 0x004FDB20
    void OnJoinParty();                                     // 0x00501C40
    CSWSCreature(unsigned long nId, int bArg);              // 0x004F7A10
    void SetSize(int nSize);                                // 0x004CC280
    void ReadScriptsFromGff(CResGFF* pRes, CResStruct* pStruct); // 0x004EBF20
    void LoadFromGff(CResGFF* pRes, CResStruct* pStruct, int a3, int a4, int a5); // 0x004FFDA0
    void PostLoad();                                        // 0x004F1C40
    void LoadCreatureState(CResGFF* pRes, CResStruct* pStruct); // 0x004F17D0
    int GetGold();                                          // 0x004EDD60
    int GetIsWeaponEffective(unsigned long oidVersus, unsigned long bOffHand); // 0x004EDB20
    CSWSCreatureVisibility* GetFromList920(unsigned long oid); // 0x00517A20 (bucket a placeholder name)
    int CalculateSpellSaveDC(int nSpell);                   // 0x004EF6E0
    short GetMaxForcePoints();                              // 0x004FD490 (GetMaxHitPoints in bucket a)
    int GetCreatureValue518B30();                           // 0x00518B30 (name guessed)
    CSWSFaction* GetFaction();                              // 0x00513FC0
    unsigned long GetFirstAttacker();                       // 0x005BA400 (name guessed)
    void Surrender(int bRetainBuffs);                       // 0x00518990 (name guessed)
    int AddUnequipActions(class CSWSItem* pItem, unsigned long oidRepository, int bToFront,
                          int bSkipPending, int nArg);      // 0x004F06D0
    unsigned long GetNextAttacker();                        // 0x005B7FE0 (name guessed)
    int AddPickUpItemActions(unsigned long oidItem, unsigned long oidArea, unsigned long nArg); // 0x004EB620
    class CItemRepository* GetItemRepository(int bArg);     // 0x004EF770
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);   // 0x00500610
    int LoadFromTemplate(const CResRef& cResRef, int bArg); // 0x005026D0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int a5, int a6); // 0x004FA100
    CSWCCreature* GetClientCreature();                      // 0x004EFB20
    void SetActivity(int nActivity, int bOn, int nArg);     // 0x004F00A0
    void AddGold(int nGold, int bFeedback);                 // 0x004F3DB0
    void RemoveGold(int nGold, int bFeedback);              // 0x004F3ED0
    void AddPazaakCard(class CSWSItem* pCard);              // 0x004EFB40
    int RemovePazaakCard(class CSWSItem* pCard);            // 0x004EFC30
    int GetDeadTemp();                                      // 0x004EF890
    short GetArmorClass();                                  // 0x004ED1D0 (name guessed)
    void SetLastHostileActor(unsigned long oidActor, int bArg); // 0x004CC0C0
    void SetCombatMode(unsigned char nMode, int bArg);     // 0x0050EE80 (Mac debug map 2/2)
    void SetCombatState(int nState, unsigned char bArg);    // 0x004F2610 (Mac name 1/2)
    void PacifyCreature();                                 // 0x004FD960 (Mac name 1/2)
    void ClearVisibleList();                               // 0x005178F0
    void UpdateVisibleList(unsigned char bArg);           // 0x0051B050
    void ClearActivityFlag(int nFlag);                      // 0x004F8770 (name guessed)
    void AddDriveAction(int nId, Vector* pPosition, int nAngle, int nA, int nB,
                        int nC);                            // 0x004EB510
    void TransferGold(unsigned long oidTarget, Vector* pPosition, int nAmount); // 0x004FD730 (name guessed)
    int GetCreatureReputation(unsigned long oidTarget, int nFaction); // 0x00513FF0 (name guessed)
    int AddMoveToPointAction(unsigned short nGroupId, const Vector& vNewPos,
                             unsigned long oidArea, unsigned long oidTarget, int bRun,
                             float fRange, float fTolerance, int a7, int a8, int a9, int a10,
                             int a11, int a12, const Vector& vFace, unsigned long a14,
                             unsigned long a15, int a16);  // 0x004F8B60
    int GetUseRange(unsigned long oid, Vector& vPosition, float& fRange, int nFlag); // 0x004EE440
    int GetIsInUseRange(unsigned long oidTarget, float fRange, int bArg); // 0x004F6000
    int EvaluateLock(CGameObject* pObject, int bArg);       // 0x004F0B20
    int AddMoveToPointActionToFront(unsigned short nGroupId, const Vector& vNewPos,
                                    unsigned long oidArea, unsigned long oidTarget, int bRun,
                                    float fRange, float fTolerance, int a7, int a8, int a9,
                                    int a10, int a11, int a12, const Vector& vFace,
                                    unsigned long a14, unsigned long a15, int a16); // 0x004F8A50
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
    void EquipMostEffectiveArmor();                         // 0x004F3A60
    void AdjustReputation(int nFaction, int nAdjustment);   // 0x00514070
    void AddAttackActions(unsigned long oidAttackee, int a2, int a3, int bPassive, int a5, int a6,
                          int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14,
                          int a15);                         // 0x004FDE40 (name guessed)
    int ResolveDefensiveEffects(CSWSObject* pTarget, int bHit); // 0x005B8E50
    void AddCastSpellActions(unsigned long nSpell, int nMultiClass, int a3, int a4, int a5, Vector vTarget,
                             unsigned long oidTarget, int a8, int a9, int a10, int a11, unsigned char a12,
                             int a13, int a14, unsigned char a15, int a16); // 0x004F9460 (Mac debug map 2/2)
    void AddCutsceneAttackActions(unsigned long oidTarget, int nAnimation, int nAttackResult,
                                  int nDamage);             // 0x004F9FB0
    void AddCutsceneMoveActions(unsigned long oidTarget, const Vector& vPosition, int bRun); // 0x004FA050
    int UseFeat(unsigned short nFeat, unsigned short nSubFeat, unsigned long oidTarget,
                unsigned long oidArea);                     // 0x004ECEE0
    int UseSkill(unsigned char nSkill, unsigned char nSubSkill, unsigned long oidTarget,
                 const Vector& vTarget, unsigned long oidArea, unsigned long oidItem,
                 int bArg);                                 // 0x004FBE40
    void UseTalentOnObject(int nType, int nId, unsigned long oidTarget, unsigned char nMultiClass,
                           unsigned long oidItem, int nItemPropertyIndex, unsigned char nCasterLevel,
                           unsigned char nMetaType);        // 0x00500CA0 (name guessed)
    void UseTalentAtLocation(int nType, int nId, class CScriptLocation* pLocation,
                             unsigned char nMultiClass, unsigned long oidItem, int nItemPropertyIndex,
                             unsigned char nCasterLevel, unsigned char nMetaType); // 0x004FD070 (name guessed)
    void EquipMostDamagingMeleeWeapon(unsigned long oidVersus, int bOffHand, int bArg); // 0x00500E50
    int EquipMostDamagingRangedWeapon(unsigned long oidVersus); // 0x00501250
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
    char pad_0x12c[0x154 - 0x12c];
    unsigned long m_oid154;                                 // 0x154 (decay event caller; name unknown)
    char pad_0x158[0x164 - 0x158];
    unsigned long m_oidLastDisturbed;                       // 0x164 (name guessed)
    char pad_0x168[0x16c - 0x168];
    unsigned short m_nLastAttackType;                       // 0x16c (name guessed)
    unsigned char m_nLastAttackMode;                        // 0x16e (name guessed)
    char pad_0x16f[0x170 - 0x16f];
    unsigned long m_oidLastWeaponUsed;                      // 0x170 (name guessed)
    char pad_0x174[0x17c - 0x174];
    unsigned long m_oidLastSpeaker;                         // 0x17c (name guessed)
    char pad_0x180[0x22c - 0x180];
    int m_nXPDebt;                                          // 0x22c
    char pad_0x230[0x310 - 0x230];
    unsigned long m_oidLastArea;                            // 0x310
    Vector m_vLastPosition;                                 // 0x314
    int m_n320;                                             // 0x320
    char pad_0x324[0x33c - 0x324];
    unsigned long m_oidBlockingDoor;                        // 0x33c (name guessed)
    int m_n340;                                             // 0x340
    char pad_0x344[0x348 - 0x344];
    int m_nCharSheetViewers;                                // 0x348 (name guessed)
    char pad_0x34c[0x370 - 0x34c];
    unsigned long m_oidLastTrapDetected;                    // 0x370 (name guessed)
    char pad_0x374[0x384 - 0x374];
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
    char pad_0x4fc[0x504 - 0x4fc];
    unsigned long m_oidAttackTarget;                        // 0x504 (name guessed)
    unsigned long m_oidAttemptedMovementTarget;             // 0x508 (name guessed)
    unsigned long m_oidAttemptedAttackTarget;               // 0x50c (name guessed)
    char pad_0x510[0x520 - 0x510];
    unsigned long m_oidGoingToBeAttackedBy;                 // 0x520
    unsigned long m_oidAttemptedSpellTarget;                // 0x524 (name guessed)
    unsigned long m_oidSpellTarget;                         // 0x528 (name guessed)
    char pad_0x52c[0x53c - 0x52c];
    unsigned long m_oidLastHostileTarget;                   // 0x53c (name guessed)
    char pad_0x540[0x544 - 0x540];
    int m_nLastAttackAction;                                // 0x544 (name guessed)
    char pad_0x548[0x54c - 0x548];
    int m_nLastForcePowerUsed;                              // 0x54c (name guessed)
    char pad_0x550[0x554 - 0x550];
    int m_nLastCombatFeatUsed;                              // 0x554 (name guessed)
    char pad_0x558[0x55c - 0x558];
    int m_nLastAttackResult;                                // 0x55c (name guessed)
    char pad_0x560[0x564 - 0x560];
    int m_bLastForcePowerSuccessful;                        // 0x564 (name guessed)
    char pad_0x568[0x8e8 - 0x568];
    int m_n8E8;                                             // 0x8e8
    char pad_0x8ec[0x900 - 0x8ec];
    unsigned long m_oidLastPerceived;                       // 0x900 (name guessed)
    int m_bLastPerceptionHeard;                             // 0x904 (name guessed)
    int m_bLastPerceptionSeen;                              // 0x908 (name guessed)
    int m_bLastPerceptionInaudible;                         // 0x90c (name guessed)
    int m_bLastPerceptionVanished;                          // 0x910 (name guessed)
    char pad_0x914[0x920 - 0x914];
    CExoArrayList<CSWSCreatureVisibility*> m_aVisibility;  // 0x920
    char pad_0x92c[0x964 - 0x92c];
    int m_n964;                                             // 0x964 (name guessed)
    int m_n968;                                             // 0x968 (name guessed)
    char pad_0x96c[0x9c8 - 0x96c];
    class CSWSCombatRound* m_pCombatRound;                  // 0x9c8
    char pad_0x9cc[0x9d4 - 0x9cc];
    int m_bIsPC;                                            // 0x9d4
    unsigned short m_n9D8;                                  // 0x9d8
    char pad_0x9da[0x9dc - 0x9da];
    int m_n9DC;                                             // 0x9dc
    int m_bPoisoned;                                        // 0x9e0
    char pad_0x9e4[0xa00 - 0x9e4];
    int m_nAIStateFlags;                                    // 0xa00 (bit 2: dialog paused)
    char pad_0xa04[0xa10 - 0xa04];
    unsigned char m_nA10;                                   // 0xa10
    char pad_0xa11[0xa24 - 0xa11];
    unsigned long m_oidEncounter;                           // 0xa24
    char pad_0xa28[0xa2c - 0xa28];
    CSWInventory* m_pInventory;                             // 0xa2c
    char pad_0xa30[0xa3c - 0xa30];
    int m_nTotalWeight;                                     // 0xa3c (name guessed)
    char pad_0xa40[0xa48 - 0xa40];
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
