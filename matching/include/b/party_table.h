// CSWPartyTable: the player's party (NPC roster, members, items, journal).
#ifndef B_PARTY_TABLE_H
#define B_PARTY_TABLE_H

#include "b/exo_string.h"
#include "b/res_gff.h"
#include "b/exo_array_list.h"
#include "b/vector.h"

class CSWSCreature;

class CSWSItem {
public:
    CSWSItem(unsigned long nId);                                // 0x005530A0
    virtual ~CSWSItem();
    int LoadItem(CResGFF* pRes, CResStruct* pStruct);           // 0x00560970
    void SetPossessor(unsigned long oidPossessor, int a2, int a3, unsigned long oidArea); // 0x00553210
    void SaveItem(CResGFF* pRes, CResStruct* pStruct);          // 0x0055CCD0
};

class CItemRepository {
public:
    CSWSItem* GetItemInRepository(unsigned long nIndex);        // 0x005560B0
    int AddItem(CSWSItem** ppItem, int a2, int a3, int a4);     // 0x0055D330
    CItemRepository(unsigned long nParentId, int bOwnsItems);  // 0x0055D290
    ~CItemRepository();                                         // 0x00555DE0
    void ClearItems();                                          // 0x0055F330
    void SetParentId(unsigned long nId);                        // 0x00556150

    char pad_0x0[0x10];
    unsigned long m_nItemCount;                                 // 0x10
    char pad_0x14[0x4];
};

class CSWSJournalEntry {
public:
    CSWSJournalEntry();
    ~CSWSJournalEntry();

    char pad_0x0[0x10];
    unsigned long m_nDate;                                      // 0x10
    unsigned long m_nTime;                                      // 0x14
    CExoString m_sPlotID;                                       // 0x18
    int m_nState;                                               // 0x20
    char pad_0x24[0x3c - 0x24];
};

class CSWSJournal {
public:
    CSWSJournal();                                              // 0x005C5A30
    ~CSWSJournal();                                             // 0x005C58F0
    void SetState(const CExoString& sPlotID, int nState, int bArg);          // 0x005C5A40
    void SetDate(const CExoString& sPlotID, unsigned long nDate, int bArg);  // 0x005C63F0
    void SetTime(const CExoString& sPlotID, unsigned long nTime, int bArg);  // 0x005C6650
    void RemoveEntry(const CExoString& sPlotID);                // 0x005C68B0 (name guessed)
    int GetState(const CExoString& sPlotID);                    // 0x005C58A0
    int GetQuestEntryXP(const CExoString& sPlotID);             // 0x005C5850

    CExoArrayList<CSWSJournalEntry> m_aEntries;                 // 0x00
};

extern int g_nJournalSortOrder;                                 // 0x00833A90

#define PARTY_NUM_NPCS 9

class CSWPartyTable {
public:
    CSWPartyTable();
    ~CSWPartyTable();
    void ClearTable();
    CItemRepository* GetPartyItems();
    CSWSJournal* GetPartyJournal();
    void LoadJournal(CResGFF* pRes, CResStruct* pStruct);
    void ClearInventory();
    CExoString GetFilename(int nNPC);
    unsigned long GetNPCObjectId(int nNPC) { return m_aNPCObjectIds[nNPC]; }
    int GetIsMember(int nNPC);                                  // 0x00563710
    int KillNPCObject(int nNPC, int bFade);
    int AddGameInProgress();
    void RemoveGameInProgress();
    void SetPartyFollowState(int nState);
    void AutoLevelUpNPCs();
    unsigned long GetTotalPlayedSeconds();
    void UnstealthParty(int nArg);
    void ResetCurrentSessionStartTime();
    void SaveJournal(CResGFF* pRes, CResStruct* pStruct);
    void SaveMember(int nNPC, int bClearActions);
    void UpdateInventory(int bClear);
    void TransferInventory(CSWSCreature* pCreature);
    int AddNPC(int nNPC, CSWSCreature* pCreature);
    int AddNPC(int nNPC, const CExoString& sTemplate);
    unsigned long GetNPCObject(int nNPC, int bSpawn, int bArg);
    int SaveTable(int nArg);                                    // 0x005648C0
    unsigned long SpawnNPC(int nNPC, int bUsePosition, const Vector* pPosition,
                           const Vector* pOrientation, int bRevive);
    void DistributeExperience(int nXP, int bSignal);
    void SetSoloMode(int bSolo, int bUnstealth);
    void UpdateMembers(int bArg);
    int RemoveMember(int nNPC);
    int AddMember(int nNPC, unsigned long oidCreature);
    void CreateParty();
    void PositionParty();
    void Save(int nArg, int bSaveMembers);
    void GivePlotXP(const CExoString& sLabel, int nPercent);
    void GivePlotXP(int nRow, int nPercent);
    int SwitchPlayerCharacter(int nNPC);
    // Small accessors (0x005636B0-0x005646D0, names guessed).
    int GetNPCAvailable(int nNPC);                              // 0x005636B0
    unsigned char GetNumAvailableNPCs();                        // 0x005636D0
    void SetNPCObjectId(int nNPC, unsigned long oid);           // 0x005636F0
    int GetIsNPCObjectMember(unsigned long oid);                // 0x00563740
    int GetIsNPCObject(unsigned long oid);                      // 0x00563790
    unsigned char GetNPCSelectable(int nNPC);                   // 0x005637C0
    void SetNPCSelectable(int nNPC, unsigned char nValue);      // 0x005637F0
    int GetNPCIndexOfObject(unsigned long oid);                 // 0x005638A0
    void ClearNPCObjectIds();                                   // 0x005639C0
    void ClearNPCObjectId(int nNPC);                            // 0x005639E0
    int GetIsSelectedNPCObject(unsigned long oid);              // 0x00563A00
    void SetSelectedNPCObject(unsigned long oid);               // 0x00563A40
    void SetPartyValues(int a, int b, int c);                   // 0x00563AB0
    void SetTokenValue(unsigned int nIndex, int nValue);        // 0x00563AE0
    int GetTokenValue(unsigned int nIndex);                     // 0x00563B00
    void SetToken(unsigned int nIndex, int nValue);             // 0x00563B20
    int GetToken(unsigned int nIndex);                          // 0x00563B40
    void SetLeaderToken(int nIndex);                            // 0x00563B60
    int RemoveAvailableNPC(int nNPC);                           // 0x005646D0

    int m_nNumMembers;                          // 0x00
    int m_aMembers[2];                          // 0x04
    unsigned long m_aNPCObjectIds[PARTY_NUM_NPCS]; // 0x0c
    int m_aNPCAvailable[PARTY_NUM_NPCS];        // 0x30
    unsigned char m_aNPCSelectable[PARTY_NUM_NPCS]; // 0x54
    char pad_0x5d[0x3];
    int m_aTokens[16];                          // 0x60
    int m_aTokenValues[16];                     // 0xa0
    int m_nLeaderNPC;                           // 0xe0
    int m_nE4;                                  // 0xe4
    int m_nFollowState;                         // 0xe8
    int m_nEC;                                  // 0xec
    int m_nControlledNPC;                       // 0xf0
    int m_nGameInProgress;                      // 0xf4
    int m_nF8;                                  // 0xf8
    int m_nFC;                                  // 0xfc
    int m_n100;                                 // 0x100
    int m_n104;                                 // 0x104
    int m_n108;                                 // 0x108
    int m_n10C;                                 // 0x10c
    int m_bPartyCreated;                        // 0x110
    unsigned long m_nPlayedSeconds;             // 0x114
    CItemRepository* m_pItems;                  // 0x118
    CSWSJournal* m_pJournal;                    // 0x11c
    int m_a120[5];                              // 0x120
    int m_a134[13];                             // 0x134
    int m_a168[10];                             // 0x168
    int m_bSoloMode;                            // 0x190
    int m_n194;                                 // 0x194
};

#endif
