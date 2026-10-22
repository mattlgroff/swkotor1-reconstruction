// CSWSCreature party broadcasts and small leaves
// (0x004EAD70-0x004ECE73). Self-contained declarations; only the members
// these functions use are placed.
#ifndef A_SWS_CREATURE_BROADCAST_H
#define A_SWS_CREATURE_BROADCAST_H

#include <stddef.h>

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }

    float x, y, z;
};

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

// size 0x34 (matching/include/a/nw_message.h has the full class)
class CSWCCMessageData {
public:
    ~CSWCCMessageData();                            // 0x004D6A40
    char pad_0x0[0x34];
};

class CGameObject {
public:
    virtual ~CGameObject() {}
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x8c - 0x8];
    unsigned long m_oidArea;                        // 0x8c
    Vector m_vPosition;                             // 0x90
};

class CSWSPlayer {
public:
    CGameObject* GetGameObject();                   // 0x00561080

    char pad_0x0[0x4];
    unsigned long m_nPlayerID;                      // 0x04
};

class CSWSMessage {
public:
    int SendServerToPlayerCCMessage(unsigned long nPlayerID, unsigned char nMinor,
                                    CSWCCMessageData* pData, void* pAttackData);   // 0x00568570
    int SendServerToPlayerBattleCry(CSWSPlayer* pPlayer, unsigned long oidSpeaker,
                                    int nBattleCry);                               // 0x00569EC0 (placeholder name)
    int SendServerToPlayerGuiTimingEvent(CSWSPlayer* pPlayer, int bStart, int nEvent,
                                         int nDuration);                           // 0x0056B380 (placeholder name)
};

class CSWSCreature;
class CWorldTimer;
class CSWPartyTable;

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x004AE770
    CSWSPlayer* GetClientObjectByObjectId(unsigned long oid);       // 0x004AE910
    unsigned long GetPlayerCharacterID();                           // 0x004AEA40 (placeholder name)
    CSWSMessage* GetSWSMessage();                                   // 0x004AEDB0
    CWorldTimer* GetWorldTimer();                                   // 0x004AEDE0
    CSWPartyTable* GetPartyTable();                                 // 0x004AEE70
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CSWSFaction {
public:
    int GetPlayerList(unsigned long** ppList);      // 0x005BE780 (placeholder name)

    CExoArrayList<unsigned long> m_aMembers;        // 0x00
};

class CSWSArea;

class CSWBaseItem {
public:
    char pad_0x0[0x1a];
    unsigned char m_nRangedWeapon;                  // 0x1a (placeholder name)
    unsigned char m_nWeaponSize;                    // 0x1b (placeholder name)
    char pad_0x1c[0xac - 0x1c];
    unsigned char m_nForceItemType;                 // 0xac (placeholder name)

    int GetIsRangedWeapon() { return m_nRangedWeapon > 0; }
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWSItem {
public:
    CSWBaseItem* GetBaseItem();                     // 0x005B4790
    char pad_0x0[0xc];
    int m_nBaseItem;                                // 0x0c
};

class CItemRepository {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);   // 0x005A4C20
};

class CSWPartyTable {
public:
    class CSWSJournal* GetJournal();                // 0x005633C0 (placeholder name)
    char pad_0x0[0xfc];
    int m_nGold;                                    // 0xfc
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
};

class CGameObjectClient {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual class CSWCCreature* AsSWCCreature();    // 0x2c
};

class CSWSCombatRound {
public:
    int GetSpellActionQueued();                     // 0x004D35C0 (placeholder name)
    void RemoveSpellAction();                       // 0x004D3BA0 (placeholder name)
};

class CSWSCreature : public CGameObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    CGameObjectClient* GetClientObject();           // 0x004CC2B0
    void UpdateObjectId(unsigned long nId, int a, int b, int c, int d);   // 0x0051B370 (placeholder name)
    void OnIdChanged(unsigned long nOldId);         // 0x00517960 (placeholder name)

    void SetId(unsigned long nId);                  // 0x004EB160
    int RangeWeaponEquipped();                      // 0x004EBB30
    int GetRelativeWeaponSize(CSWSItem* pItem);     // 0x004ED190
    unsigned char GetGenderForDialog();             // 0x004EDAE0 (placeholder name)
    class CSWSJournal* GetJournal();                // 0x004EDB00
    int GetGold();                                  // 0x004EDD60
    void SetGold(int nGold);                        // 0x004EDD90
    int GetFlags8ECState();                         // 0x004EE210 (placeholder name)
    void RemoveSpellActionFromRound();              // 0x004EF450
    void StartGuiTimingBar(int nDuration, int nEvent);  // 0x004EF480
    void StopGuiTimingBar();                        // 0x004EF4D0
    void SetInvitedToParty(int bInvited);           // 0x004EF520
    unsigned long GetForceItemMask();               // 0x004EFA80
    class CSWCCreature* GetClientCreature();        // 0x004EFB20
    unsigned long GetAAxValue(int nType);           // 0x004EFDD0 (placeholder name)
    void SetAAxValue(int nType, unsigned long nValue);  // 0x004EFE00 (placeholder name)
    void SetByte4C4(unsigned char nValue);          // 0x004EFEF0 (placeholder name)
    CSWSFaction* GetFaction();                      // 0x00513FC0

    void SetFlag288Bit3(int bValue);                // 0x004EAD70 (placeholder name)
    int GetHasDamageFlag(unsigned char nFlag);      // 0x004EADA0 (placeholder name)
    int GetHasFlags9FC(unsigned long nMask);        // 0x004EB190 (placeholder name)
    float GetMovementRateFactor();                  // 0x004EC370 (placeholder name)
    void SetMovementRateFactor(float fFactor);      // 0x004EC3B0 (placeholder name)
    void BroadcastDamageDataToParty(CSWCCMessageData* pData, int bHeal);   // 0x004EC400
    void BroadcastDeathDataToParty(CSWCCMessageData* pData);               // 0x004EC500
    void BroadcastData0B(CSWCCMessageData* pData);                         // 0x004EC610 (placeholder name)
    void BroadcastSkillData(CSWCCMessageData* pData);                      // 0x004EC700
    void BroadcastSavingThrowData(CSWCCMessageData* pData);                // 0x004EC760
    void BroadcastStealthDetectionData(CSWCCMessageData* pData);           // 0x004EC7A0
    void BroadcastMineDetectionData(CSWCCMessageData* pData);              // 0x004EC840
    void BroadcastBattleCry();                                             // 0x004EC8E0
    void BroadcastAttackSummaryToParty(CSWCCMessageData* pData);           // 0x004EC9E0
    void BroadcastBreakdownMessage(int nType, CSWCCMessageData* pData);    // 0x004ECAE0
    void BroadcastAttackBreakdown(unsigned long oidTarget, CSWCCMessageData* pData);   // 0x004ECBE0
    void BroadcastThreatBreakdown(unsigned long oidTarget, CSWCCMessageData* pData);   // 0x004ECC20
    void BroadcastDefenseBreakdown(unsigned long oidTarget, CSWCCMessageData* pData);  // 0x004ECC60
    void BroadcastDamageBreakdown(unsigned long oidTarget, CSWCCMessageData* pData);   // 0x004ECCA0
    void BroadcastEffectApplicationBreakdown(unsigned long oidTarget, CSWCCMessageData* pData); // 0x004ECCE0
    void BroadcastDeflectBreakdown(unsigned long oidTarget, CSWCCMessageData* pData);  // 0x004ECD20
    void BroadcastPoisoned(CSWCCMessageData* pData);                       // 0x004ECD60
    void UpdateFlag4DC();                           // 0x004ECE60 (placeholder name)

    char pad_0x9c[0xd4 - 0x9c];
    int m_nAppearanceType;                          // 0xd4 (placeholder name)
    char pad_0xd8[0x288 - 0xd8];
    unsigned long m_b288_0 : 1;                     // 0x288 bit 0
    unsigned long m_b288_1 : 1;
    unsigned long m_b288_2 : 1;
    unsigned long m_b288_3 : 1;                     // 0x288 bit 3
    char pad_0x28c[0x294 - 0x28c];
    unsigned long m_nDamageFlags;                   // 0x294 (placeholder name)
    char pad_0x298[0x4c4 - 0x298];
    unsigned char m_n4C4;                           // 0x4c4 (placeholder name)
    char pad_0x4c5[0x4d0 - 0x4c5];
    unsigned char m_nGender;                        // 0x4d0 (placeholder name)
    char pad_0x4d1[0x4dc - 0x4d1];
    int m_b4DC;                                     // 0x4dc (placeholder name)
    char pad_0x4e0[0x4f8 - 0x4e0];
    int m_nCreatureSize;                            // 0x4f8 (placeholder name)
    char pad_0x4fc[0x8ec - 0x4fc];
    unsigned char m_n8EC;                           // 0x8ec (placeholder name)
    char pad_0x8ed[0x930 - 0x8ed];
    int m_bInvitedToParty;                          // 0x930
    unsigned long m_nInvitedDay;                    // 0x934 (placeholder name)
    unsigned long m_nInvitedTime;                   // 0x938 (placeholder name)
    char pad_0x93c[0x9c8 - 0x93c];
    CSWSCombatRound* m_pCombatRound;                // 0x9c8
    char pad_0x9cc[0x9d0 - 0x9cc];
    int m_nGold;                                    // 0x9d0
    int m_bPlayerCharacter;                         // 0x9d4 (placeholder name)
    char pad_0x9d8[0x9fc - 0x9d8];
    unsigned long m_n9FC;                           // 0x9fc (placeholder name)
    char pad_0xa00[0xa08 - 0xa00];
    float m_fMovementRateFactor;                    // 0xa08 (placeholder name)
    char pad_0xa0c[0xa2c - 0xa0c];
    CItemRepository* m_pInventory;                  // 0xa2c (placeholder name)
    char pad_0xa30[0xa88 - 0xa30];
    void* m_pA88;                                   // 0xa88 (placeholder name)
    char pad_0xa8c[0xaa8 - 0xa8c];
    unsigned long m_nAA8;                           // 0xaa8 (placeholder name)
    unsigned long m_nAAC;                           // 0xaac (placeholder name)
};

#endif
