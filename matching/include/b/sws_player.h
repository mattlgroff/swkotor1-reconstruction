// CSWSPlayer: server-side player (client connection + controlled creature).
#ifndef B_SWS_PLAYER_H
#define B_SWS_PLAYER_H

#include "b/exo_string.h"
#include "b/res_gff.h"
#include "b/exo_array_list.h"
#include "b/exo_linked_list.h"
#include "b/sws_creature.h"

// .rdata copy of the invalid object id (0x7F000000) some sites load.
extern const unsigned long OBJECT_INVALID_ID;                   // 0x00746644

class CGameObject;
class CSWSCreature;
class CLastUpdateObject;
class CSWSPlayerLastUpdateObject;

class CSWSPlayerLastUpdateObject {
public:
    CSWSPlayerLastUpdateObject();                               // 0x005CAC80
    ~CSWSPlayerLastUpdateObject();                              // 0x005CAD70
    void ClearKnownSpellUsesLeft();                             // 0x005CA060
    void ClearActionQueue();                                    // 0x005CA0D0
    void ClearKnownSpells();                                    // 0x005CAB20
    void ClearSpellAddDeleteLists();                            // 0x005CAC30
    void ClearFeats();                                          // 0x005CAB60
    unsigned char GetKnownSpellUsesLeft(int nClass, int nLevel); // 0x005CA070

    char pad_0x0[0x18];
    CExoArrayList<unsigned short> m_aFeats;                     // 0x18
    CExoArrayList<unsigned short> m_aBonusFeats;                // 0x24
    CExoArrayList<CSWSStats_FeatUses*> m_aFeatUses;             // 0x30
    char pad_0x3c[0x40 - 0x3c];
    CSWSActionQueueEntry m_aActionQueue[10];                    // 0x40
    CExoArrayList<CSWSCreatureVisibility*> m_aVisibility;      // 0x158
    char pad_0x164[0x1b0 - 0x164];
    CExoArrayList<CSWSStats_SpellLikeAbility>* m_pSpellLikeAbilities; // 0x1b0
};

class CLastUpdateObject {
public:
    ~CLastUpdateObject();                                       // 0x005CA6A0

    CSWSCreatureAppearanceInfo m_cAppearance;                   // 0x0
    int m_bUpdated;                                             // 0x24
    char pad_0x28[0x2a - 0x28];
    unsigned short m_n2A;                                       // 0x2a
    unsigned long m_nId;                                        // 0x2c
    char pad_0x30[0x90 - 0x30];
    CExoLocString m_lsFirstName;                                // 0x90
    CExoLocString m_lsLastName;                                 // 0x98
};

// 0xc-byte entry of the client info list (dtor 0x0059EA20).
class CSWSPlayerClientEntry {
public:
    ~CSWSPlayerClientEntry();                                   // 0x0059EA20

    char pad_0x0[0xc];
};

class CSWSPlayerClientInfo {
public:
    CSWSPlayerClientInfo() { m_bDirty = 1; m_n0 = 0; }

    int m_n0;                                                   // 0x00
    CExoArrayList<CSWSPlayerClientEntry> m_aEntries;            // 0x04
    int m_bDirty;                                               // 0x10
};

// Really CSWCreatureStatsUpdate (0x5c bytes).
class CSWSPlayerInventoryItems {
public:
    CSWSPlayerInventoryItems();                                 // 0x005CAF10
    ~CSWSPlayerInventoryItems();                                // 0x005CB040
    void ClearEffectIcons();                                    // 0x005CAEB0

    char pad_0x0[0x5c];
};

// Player +0x50; functions.csv calls it CSWSPlayerCharSheetGUI.
class CSWSPlayerInventoryGUI {
public:
    CSWSPlayerInventoryGUI();                                   // 0x00560B30
    ~CSWSPlayerInventoryGUI() { delete m_pItems; }
    void SetCreatureDisplayed(unsigned long oidCreature);       // 0x00560BA0

    unsigned long m_oidCreature;                                // 0x00
    unsigned char m_nDisplayMode;                               // 0x04
    char pad_0x5[0x3];
    CSWSPlayerInventoryItems* m_pItems;                         // 0x08
    int m_nC;                                                   // 0x0c
};

class CSWSPlayerStore {
public:
    CSWSPlayerStore();                                          // 0x005CA7F0
    ~CSWSPlayerStore();                                         // 0x005CA900
    void ClearItems();                                          // 0x005CA030 (name guessed)
    void ClearRepository();                                     // 0x005CA470
    void Clear();                                               // 0x005CA4A0

    unsigned long m_aSlotItems[18];                             // 0x00
    int m_aSlotStackSize[18];                                   // 0x48
    char pad_0x90[0x10];
};

class CSWSPlayerStoreGUI {
public:
    CSWSPlayerStoreGUI();                                       // 0x00560C20
    void SetOpen(int bOpen);                                    // 0x00560C90
    ~CSWSPlayerStoreGUI() { delete m_pStore; }

    CSWSPlayerStore* m_pStore;                                  // 0x00
    int m_bOpen;                                                // 0x04
};

class CSWSPlayerObjectRef {
public:
    CSWSPlayerObjectRef() { m_n4 = 0; m_oid = OBJECT_INVALID_ID; }

    unsigned long m_oid;                                        // 0x00
    int m_n4;                                                   // 0x04
};

// Base class (0x005CDA10 ctor, 0x005CDA30 dtor).
class CSWSClient {
public:
    CSWSClient(unsigned long nPlayerId);
    virtual ~CSWSClient();
    virtual class CSWSPlayer* AsSWSPlayer();                    // 0x04
    unsigned long GetPlayerId() { return m_nPlayerID; }

    unsigned long m_nPlayerID;                                  // 0x04
    char pad_0x8[0x4];
};

class CSWSPlayer : public CSWSClient {
public:
    CSWSPlayer(unsigned long nPlayerId);
    virtual ~CSWSPlayer();
    virtual CSWSPlayer* AsSWSPlayer();                          // `mov eax, ecx; ret`

    CGameObject* GetGameObject();
    CExoString GetPlayerName();
    CSWSPlayerLastUpdateObject* CreateNewPlayerLastUpdateObject();
    int PermittedToDisplayCharacterSheet(unsigned long nId);
    CLastUpdateObject* GetLastUpdateObject(unsigned long nId);
    void SetGameObject(CGameObject* pObject);
    void ClearPlayerOnDestroyGame();
    int GetCharacterInfoFromIFO(unsigned char* pClassCount, int* pClasses,
                                unsigned char* pLevels, unsigned long* pExperience);
    int LoadCreatureData(CResRef cResRef, CSWSCreature* pCreature);
    int LoadLocalCharacter();
    int LoadServerCharacter(CResRef cResRef);
    int LoadCharacterFromIFO(int nIndex, int bArg, int bFinish);
    int PackCreatureIntoMessage();
    int HasCharacterFile();                                     // 0x005624C0 (V0, name guessed)
    int GetIsSaveAllowed();                                     // 0x00560CC0 (name guessed)
    void SetAreaTransitionBMP(int nMode, const CExoString& sBMP); // 0x00560CD0

    CExoLinkedList<CLastUpdateObject>* m_pLastUpdateObjects;    // 0x0c
    CExoLinkedList<CLastUpdateObject>* m_pList10;               // 0x10
    int m_n14;                                                  // 0x14
    CExoString m_s18;                                           // 0x18
    int m_n20;                                                  // 0x20
    unsigned char m_n24;                                        // 0x24
    unsigned long m_oidNWSObject;                               // 0x28
    int m_n2C;                                                  // 0x2c
    int m_n30;                                                  // 0x30
    unsigned long m_oid34;                                      // 0x34
    unsigned long m_oid38;                                      // 0x38
    int m_n3C;                                                  // 0x3c
    int m_n40;                                                  // 0x40
    int m_n44;                                                  // 0x44
    int m_n48;                                                  // 0x48
    CSWSPlayerClientInfo* m_pClientInfo;                        // 0x4c
    CSWSPlayerInventoryGUI* m_pInventoryGUI;                    // 0x50
    CSWSPlayerStoreGUI* m_pStoreGUI;                            // 0x54
    CSWSPlayerObjectRef* m_pObjectRef;                          // 0x58
    CSWSPlayerLastUpdateObject* m_pLastUpdatePlayer;            // 0x5c
    int m_n60;                                                  // 0x60
    unsigned char m_n64;                                        // 0x64
    CResRef m_cResRef;                                          // 0x65
    char pad_0x75[0x7c - 0x75];
    int m_n7C;                                                  // 0x7c
    int m_n80;                                                  // 0x80
};

#endif
