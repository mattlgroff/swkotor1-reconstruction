// CFactionManager / CFaction.
#ifndef B_FACTION_MANAGER_H
#define B_FACTION_MANAGER_H

#include "b/exo_string.h"
#include "b/exo_array_list.h"
#include "b/res_gff.h"

class CFaction {
public:
    CFaction();                 // 0x005BF860
    ~CFaction();                // 0x005BE8A0
    void AddMember(unsigned long oidMember, int bArg);  // 0x005BFA70

    char pad_0x0[0x10];
    int m_nParentFactionId;     // 0x10
    char pad_0x14[0x4];
    CExoString m_sFactionName;  // 0x18
    int m_bGlobal;              // 0x20
};

class CFactionManager {
public:
    CFactionManager();
    ~CFactionManager();
    int IsValidFactionId(int nFactionId);
    void ResolveFactionName(const CExoString& sName, CFaction* pFaction);
    void DeleteFaction(int nFactionId);
    CFaction* GetFaction(int nFactionId);
    void SetNPCReputation(int nFactionId1, int nFactionId2, int nReputation);
    int GetNPCReputation(int nFactionId1, int nFactionId2);
    int LoadFactions(int bAppend);
    int LoadFactionsFromSaveGame(CResGFF* pRes, CResList* pList);
    int SaveFactions(CResGFF* pRes, CResList* pList);
    int SaveReputations(CResGFF* pRes, CResList* pList);
    int LoadReputations();
    int LoadReputationsFromSaveGame(CResGFF* pRes, CResList* pList);
    void CreateDefaultFactions();

    CFaction* m_pNeutral;                       // 0x00
    CFaction* m_pInsane;                        // 0x04
    CFaction* m_pFriendly2;                     // 0x08
    CFaction* m_pFriendly1;                     // 0x0c
    CFaction* m_pHostile1;                      // 0x10
    CFaction* m_pHostile2;                      // 0x14
    CFaction* m_pPlayer;                        // 0x18
    int m_nNumFactions;                         // 0x1c
    CExoArrayList<int>* m_pReputations;         // 0x20
    CExoArrayList<CFaction*>* m_pFactionList;   // 0x24
};

#endif
