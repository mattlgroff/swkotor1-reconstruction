// CAppManager / CServerExoApp / CClientExoApp (only what worker B uses).
#ifndef B_SERVER_EXO_APP_H
#define B_SERVER_EXO_APP_H

#include "b/exo_string.h"

class CSWSCreature;
class CSWSObject;
class CGameObject;
class CSWPartyTable;
class CNetLayer;
class CSWParty;
class CSWGuiInGame;
class CSWSClient;
template <class T> class CExoLinkedList;

class CFactionManager;

class CServerExoAppInternal {
public:
    char pad_0x0[0x10054];
    CFactionManager* m_pFactionManager;                                 // 0x10054
};

class CServerExoApp {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CNetLayer* GetNetLayer();                                   // 0x10

    unsigned long GetPlayerCreatureId();                                // 0x004AEA40
    unsigned long GetFirstPCObject();                                   // 0x004AEA10
    unsigned long GetNextPCObject();                                    // 0x004AEA20
    unsigned long ClientToServerObjectID(unsigned long nId);            // 0x004AEA30
    CGameObject* GetGameObject(unsigned long nId);                      // 0x004AE750
    class CSWSItem* GetItemByGameObjectID(unsigned long nId);           // 0x004AE760
    CSWSCreature* GetCreatureByGameObjectID(unsigned long nId);         // 0x004AE770
    class CSWSArea* GetAreaByGameObjectID(unsigned long nId);           // 0x004AE780
    class CSWSPlaceable* GetPlaceableByGameObjectID(unsigned long nId); // 0x004AE7A0
    class CServerAIMaster* GetServerAIMaster();                         // 0x004AED80
    void SetAutoSavePending(int bPending);                              // 0x004AED10
    CGameObject* GetClientObjectByObjectId(unsigned long nId);          // 0x004AE910
    CSWPartyTable* GetPartyTable();                                     // 0x004AEE70
    void SetActionConversation(unsigned long oidConversation);          // 0x004AF010
    unsigned long GetActionConversation();                              // 0x004AF020
    void SetMoveToModulePending(int bPending);                          // 0x004AECC0
    void SetMoveToModuleString(const CExoString& sModule);              // 0x004AECD0
    void SetMoveToModuleStartWaypoint(const CExoString& sWaypoint);     // 0x004AED30
    class CSWSModule* GetModule();                                      // 0x004AE6B0
    short GetServerMode();                                              // 0x004AEDD0
    void AddCharListRequest(unsigned long nPlayerId);                   // 0x004AEA00
    class CGameObjectArray* GetObjectArray();                           // 0x004AED70
    int ComputeSafeLocation(const struct Vector& vPosition, float fRadius, unsigned long oid,
                            int bArg, struct Vector* pResult);          // 0x004AEB60
    int GetLoadFromSaveGame();                                          // 0x004AF050
    class CSWSMessage* GetSWSMessage();                                 // 0x004AEDB0
    class CWorldTimer* GetWorldTimer();                                 // 0x004AEDE0
    class CSWSPlayer* GetClientObjectByPlayerID(unsigned long nPlayerId,
                                                unsigned char nType);   // 0x004AE920
    CExoLinkedList<CSWSClient>* GetPlayerList();                        // 0x004AED90
    int GetFactionOfObject(unsigned long oid, int* pFaction);           // 0x004AE950
    void Shutdown(int bArg1, int bArg2);                                // 0x004AE890
    void MovePlayerToArea(class CSWSPlayer* pPlayer);                   // 0x004AE940
    int GetPauseState(unsigned char nState);                            // 0x004AE980
    CExoLinkedList<unsigned long>* GetActiveExclusionList();             // 0x004AE9D0
    class CSWGlobalVariableTable* GetGlobalVariableTable();             // 0x004AEE60
    void SetExportAllCharactersPending(int bPending);                   // 0x004AECE0
    class CSWSDoor* GetDoorByGameObjectID(unsigned long nId);           // 0x004AE7C0
    class CSWSEncounter* GetEncounterByGameObjectID(unsigned long nId); // 0x004AE7F0
    class CSWSWaypoint* GetWaypointByGameObjectID(unsigned long nId);   // 0x004AE7E0
    class CSWSSoundObject* GetSoundObjectByGameObjectID(unsigned long nId); // 0x004AE800
    int GetPlayerLanguage(unsigned long nPlayerId);                     // 0x004AE810
    int GetModuleLanguage();                                            // 0x004AE820
    class CServerInfo* GetServerInfo();                                 // 0x004AEC90
    CExoString GetPlayerListString();                                  // 0x004AE8D0
    CExoString GetModuleName();                                       // 0x004AE6C0
    int GetButtonMashCheck();                                           // 0x004AF070
    void SetButtonMashCheck(int bCheck);                                // 0x004AF080
    CExoString GetLastConversation();                                   // 0x004AEFE0 (name guessed)
#ifdef B_OUT_OF_LINE_FACTION_MANAGER
    CFactionManager* GetFactionManager();                   // 0x004AEFC0
#else
    CFactionManager* GetFactionManager() { return m_pInternal->m_pFactionManager; }
#endif

    CServerExoAppInternal* m_pInternal;                                 // 0x4
};

class CClientOptions {
public:
    char pad_0x0[0x4];
    unsigned char m_nDifficulty;                                        // 0x4
    char pad_0x5[0x3];
    unsigned char m_nFlags;                                             // 0x8
    char pad_0x9[0x14 - 0x9];
    unsigned long m_bOption0 : 1;                                       // 0x14 bit 0
    unsigned long m_bTutorialWindowsEnabled : 1;                        // 0x14 bit 1
    char pad_0x18[0x8c - 0x18];
    void SetTutorialWindowsEnabled(int bEnabled) { m_bTutorialWindowsEnabled = bEnabled; }
    int m_bInfiniteUses;                                                // 0x8c
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);       // 0x004ADE40
    void AdvanceToTime(int nHour, int nMinute, int nSecond, int nMillisecond); // 0x004AE260
    unsigned long GetWorldTimeHour();                                   // 0x004AE090
    unsigned long GetWorldTimeMinute();                                 // 0x004AE0D0
    unsigned long GetWorldTimeSecond();                                 // 0x004AE110
    unsigned long GetWorldTimeMillisecond();                            // 0x004AE140

    char pad_0x0[0x38];
    unsigned char m_nMinutesPerHour;                                    // 0x38
};

class CSWCCreature;

class CSWCItem {
public:
    virtual ~CSWCItem();
};

class CGameObjectArray {
public:
    unsigned char Delete(unsigned long nId, CGameObject** ppObject);    // 0x004D81A0
    unsigned char GetGameObject(unsigned long nId, CGameObject** ppObject); // 0x004D8230
};

class CClientExoApp {
public:
    class CSWCModule* GetModule();                                      // 0x005ED530
    void SetGameOver(int bGameOver);                                    // 0x005EDC50
    void FadeToBlack(int bArg, float fDelay, int bShowGui);             // 0x005EDE10 (name guessed)
    void QueueMovie(int nMovie);                                        // 0x005EE220 (name guessed)
    int IsMoviePlaying();                                               // 0x005EDB40
    void AddMovieToMovieQueue(const CExoString& sMovie, int bSkippable); // 0x005EE210
    void ResetDriveAcceleration();                                      // 0x005EDBA0
    void SetPauseState(unsigned char nType, int nState,
                        CExoLinkedList<unsigned long>* pExcludedObjects); // 0x005ED650
    void GetPazaakResult(int* pResult, int* pWager);                    // 0x005EDDE0 (second name guessed)
    void HideLoadScreen(int a1, int a2);                                // 0x005EDA90
    void PlayMovie(const CExoString& sMovie, int* pResult, int a3, int a4); // 0x005EDB30
    void StartPazaakGame(int nOpponentDeck, const CExoString& sEndScript, int nMaxWager,
                         int bShowTutorial, unsigned long oidOpponent); // 0x005EDDC0
    int PlayRumblePattern(int nPattern);                                // 0x005EDE40
    int StopRumblePattern(int nPattern);                                // 0x005EDE50
    int SetPausedByCombat(int bPaused, int a2, int a3);                 // 0x005EDC20
    void EnableVideoEffect(int nEffect);                                // 0x005EDF10
    void DisableVideoEffect();                                          // 0x005EDF20
    void ShowTutorialWindow(int nWindow, unsigned long oid1, unsigned long oid2, int nArg); // 0x005EDF40
    int GetCreditSequenceInProgress();                                  // 0x005EE1E0
    void StartCreditSequence(const struct Vector& vColor,
                             const CExoString& sMovie);                 // 0x005EE1C0
    CGameObjectArray* GetObjectArray();                                 // 0x005ED570
    CGameObject* GetGameObject(unsigned long nId);                     // 0x005ED580
    CSWCItem* GetItemByGameObjectID(unsigned long nId);                 // 0x005ED5A0
    CClientOptions* GetOptions();                                       // 0x005ED700
    unsigned long ServerToClientObjectId(unsigned long nId);            // 0x005EDA50
    CSWCCreature* GetCreatureByGameObjectID(unsigned long nId);         // 0x005ED5B0
    CSWGuiInGame* GetInGameGui();                                       // 0x005ED690
    void AddMoveToModuleMovie(const CExoString& sMovie);                // 0x005EDB50 (Mac map: CResRef const&)
    CSWParty* GetSWParty();                                             // 0x005ED8B0
    void ChangeCharacter(int nDirection, int bArg);                     // 0x005EDC30
    void SetPlayerCreature(unsigned long oidCreature);                  // 0x005ED780
    void AttachPlayerGameObjectToArea();                                // 0x005ED720
    CExoString GetGUIString(unsigned long nStrRef);                     // 0x005EDEB0
    int GetLanguage();                                                  // 0x005ED4C0
    void BarkString(unsigned long oidSpeaker, const CExoString& sText,
                    const class CResRef& cSound, int bArg);             // 0x005EDBB0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;     // 0x4
    CServerExoApp* m_pServerExoApp;     // 0x8

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;      // 0x007A39FC

#define INVALID_OBJECT_ID 0x7f000000

#endif
