// CServerExoApp (0x004AE6B0-0x004AF0E0, 0x004B2820), the thin
// facade over CServerExoAppInternal. Self-contained declarations.
#ifndef I_SERVER_EXO_APP_H
#define I_SERVER_EXO_APP_H

#include <stddef.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* CStr() const;                             // 0x005E5670
    int IsEmpty() const;                            // 0x005E58A0
    char* m_sString;
    unsigned long m_nBufferLength;
};

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

// Passed by value with an inline (member-wise) copy.
struct CResRef {
    CResRef(const char* s);                         // 0x00406D80
#ifdef I_CRESREF_DECLARED_COPY
    // Declared, not defined: a temporary is then built in the argument slot.
    CResRef(const CResRef& c);
#endif
    char m_resRef[16];
};

extern const float g_fZero;                         // 0x0073D700

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();                                // 0x00413D80
    int Load2DArray();                              // 0x004143B0
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue); // 0x00414110
    char m_pad4[0x54 - 4];
};

class CExoLocString;
class CSWSItem {
public:
    CSWSItem(unsigned long oid);                    // 0x005530A0
    int LoadFromTemplate(const CResRef& cTemplate); // 0x005608B0
    char m_pad0[0x288];
    unsigned long m_nItemFlags;                     // +0x288 (unnamed)
    char m_pad28c[0x29c - 0x28c];
};
class CSWSTrigger {
public:
    char m_pad0[0x2b8];
    int m_nFactionID;                               // +0x2b8
};
class CSWSPlaceable {
public:
    char m_pad0[0x23c];
    int m_nFactionID;                               // +0x23c
};
class CSWSStore;
class CSWSDoor {
public:
    char m_pad0[0x2b8];
    int m_nFactionID;                               // +0x2b8
};
class CSWSAreaOfEffectObject;
class CSWSWaypoint;
class CSWSEncounter {
public:
    char m_pad0[0x22c];
    int m_nFactionID;                               // +0x22c
};
class CSWSSoundObject;
class CWorldTimer;
class CFactionManager;
class CPathfindInformation;
class CSWSModule;
class CSWSPlayer;

class CSWSMessage {
public:
    int SendServerToPlayerModule_ExportReply(CSWSPlayer* pPlayer);  // 0x0056CAC0
    int SendServerToPlayerLogin_CharacterQuery(CSWSPlayer* pPlayer, unsigned char* pCount,
                                               unsigned __int64* p1, unsigned long* p2,
                                               unsigned long* p3); // 0x0056C730
};
class CSWSArea;
class CSWSCreature;

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633
extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635
extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT;      // 0x0074663A
extern const unsigned char OBJECT_TYPE_WAYPOINT;            // 0x0074663B
extern const unsigned char OBJECT_TYPE_ENCOUNTER;           // 0x0074663C
extern const unsigned char OBJECT_TYPE_STORE;               // 0x0074663D
extern const unsigned char OBJECT_TYPE_SOUND;               // 0x0074663E
extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned long CLIENT_OBJECT_MASK;              // 0x0074665C

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pList->Remove(pos); }
    CExoLinkedListPosition AddTail(T* p) { return m_pList->AddTail(p); }
    CExoLinkedListInternal* m_pList;
};

class CSWSObject {
public:
    char m_pad0[0x1f4];
    int m_bSkipDialog;                              // +0x1f4 (unnamed)
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void SetWorldTime(unsigned long nDay, unsigned long nTime);     // +0x08
    virtual void* AsSWCObject();                    // +0x0c
    virtual CSWSObject* AsSWSObject();              // +0x10
    virtual void* AsSWCDoor();                      // +0x14
    virtual CSWSDoor* AsSWSDoor();                  // +0x18
    virtual void* AsSWCModule();                    // +0x1c
    virtual CSWSModule* AsSWSModule();              // +0x20
    virtual void* AsSWCArea();                      // +0x24
    virtual CSWSArea* AsSWSArea();                  // +0x28
    virtual void* AsSWCCreature();                  // +0x2c
    virtual CSWSCreature* AsSWSCreature();          // +0x30
    virtual void* AsSWCItem();                      // +0x34
    virtual CSWSItem* AsSWSItem();                  // +0x38
    virtual void* AsSWCTrigger();                   // +0x3c
    virtual CSWSTrigger* AsSWSTrigger();            // +0x40
    virtual void* AsSWCPlaceable();                 // +0x44
    virtual CSWSPlaceable* AsSWSPlaceable();        // +0x48
    virtual void* AsSWCAreaOfEffectObject();        // +0x4c
    virtual CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject();  // +0x50
    virtual void* AsSWCWaypoint();                  // +0x54
    virtual CSWSWaypoint* AsSWSWaypoint();          // +0x58
    virtual CSWSEncounter* AsSWSEncounter();        // +0x5c
    virtual void* AsSWCStore();                     // +0x60
    virtual CSWSStore* AsSWSStore();                // +0x64
    virtual CSWSSoundObject* AsSWSSoundObject();    // +0x68

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject); // 0x004D8230
};

class CSWSPlayer;

class CSWSClient {
public:
    virtual ~CSWSClient();
    virtual CSWSPlayer* AsSWSPlayer();              // +0x04
    unsigned long m_nPlayerID;                      // +0x04
    int m_nLanguage;                                // +0x08
};

class CSWSPlayer : public CSWSClient {
public:
    CGameObject* GetGameObject();                   // 0x00561080
    void FUN_005624c0();                            // 0x005624C0
    void GetCharacterInfoFromIFO(unsigned char* pCount, unsigned __int64* p1, unsigned long* p2,
                                 unsigned long* p3); // 0x00560D00
    char m_pad0c[0x38 - 0x0c];
    unsigned long m_oidPCObject;                    // +0x38
    char m_pad3c[0x44 - 0x3c];
    int m_nPrimaryPlayerIndex;                      // +0x44 (unnamed)
    char m_pad48[0x64 - 0x48];
    unsigned char m_nCharacterType;                 // +0x64 (unnamed)
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    void* m_pIniInternal;
};

class CNetLayer {
public:
    int StartServerMode(const CExoString& sMode, unsigned long nMaxPlayers);   // 0x005D4FB0
};

class CServerInfo {
public:
    void FUN_0054ea70(int n);                       // 0x0054EA70
    char m_pad0[0x80];
    unsigned long m_nMaxPlayers;                    // +0x80 (unnamed)
    char m_pad84[0x98 - 0x84];
    int m_n98;                                      // +0x98 (unnamed)
    char m_pad9c[0xc4 - 0x9c];
    int m_nc4;                                      // +0xc4 (unnamed)
    int m_nc8;                                      // +0xc8 (unnamed)
    char m_padcc[0xe0 - 0xcc];
    int m_ne0;                                      // +0xe0 (unnamed)
    char m_pade4[0xec - 0xe4];
    int m_nec;                                      // +0xec (unnamed)
    char m_padf0[0xfc - 0xf0];
    CExoString m_sModuleName;                       // +0xfc
    char m_pad104[4];
    int m_nDifficulty;                              // +0x108
};

class CSWRules {
public:
    int GetDifficultyOption(int nOption, int nDifficulty);  // 0x00550C30 (unnamed)
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CExoDebug {
public:
    void GetCurrentTimestamp(CExoString& s);        // 0x005E9410
};

class CExoTimers {
public:
    unsigned long GetLowResolutionTimer();          // 0x005E9250
};

class CExoBase {
public:
    void* m_p0;
    CExoTimers* m_pcExoTimers;                      // +0x04
    CExoDebug* m_pcExoDebug;                        // +0x08
};
extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CSWSCreatureStats {
public:
    char m_pad0[0x78];
    int m_nFactionID;                               // +0x78
};

class CSWSCombatRound {
public:
    void RemoveAllActions();                        // 0x004D3770
    char m_pad0[0x9cc];
    unsigned long m_oidNewAttackTarget;             // +0x9cc
};

class CSWSCreature {
public:
    void ClearAllActions(int bClearCombat);         // 0x004CCD80 (CSWSObject)
    void SetOrientation(const Vector& v);           // 0x004CD170 (CSWSObject)
    void SetPosition(const Vector& v, int a, int b, int c);  // 0x004CD1A0 (CSWSObject)
    int IsInjured();                                // 0x004EFF30 (unnamed)
    int AcquireItem(CSWSItem** ppItem, unsigned long oidPossessor, unsigned long oidTarget,
                    int bDisplayFeedback);          // 0x005158E0 (unnamed)
    CSWSArea* GetArea();                            // 0x004CB120 (CSWSObject)
    char m_pad0[4];
    unsigned long m_idSelf;                         // +0x04
    char m_pad8[0x8c - 8];
    unsigned long m_oidArea;                        // +0x8c
    char m_pad90[0xfc - 0x90];
    CExoLinkedList<void> m_lActions;                // +0xfc
    char m_pad100[0x340 - 0x100];
    CPathfindInformation* m_pPathfindInformation;   // +0x340
    char m_pad344[0x504 - 0x344];
    unsigned long m_oid504;                         // +0x504
    unsigned long m_pad508;
    unsigned long m_oid50c;                         // +0x50c
    char m_pad510[0x524 - 0x510];
    unsigned long m_oid524;                         // +0x524
    unsigned long m_oid528;                         // +0x528
    char m_pad52c[0x9c8 - 0x52c];
    CSWSCombatRound* m_pcCombatRound;               // +0x9c8
    char m_pad9cc[0xa74 - 0x9cc];
    CSWSCreatureStats* m_pStats;                    // +0xa74
};

class CSWSRoom {
public:
    Vector GetNormal(int nFace);                    // 0x00579840 (unnamed)
};

class CExoResMan {
public:
    int GetFreeDiskSpace(const CExoString& sPath, unsigned __int64* pFreeBytes);  // 0x004067B0
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CSWSArea {
public:
    float ComputeHeight(float fX, float fY, int nFlags);                    // 0x004BC380
    int GetRoomIndex(float fX, float fY);                                   // 0x004BB730
    int TestDirectLine(float fX1, float fY1, float fX2, float fY2,
                       float fZ1, float fZ2);                               // 0x004BCB70
    int ShrinkLineByCreatures(Vector* a, Vector* b, unsigned long c);       // 0x004BF650 (unnamed)
    int TestSafeLocationPoint(const Vector& v, CPathfindInformation* p);    // 0x004BE5E0
    int ComputeSafeLocation(const Vector& v, float f, CPathfindInformation* pPathfind,
                            void* p, Vector* out);                          // 0x004BE860 (unnamed)
    CSWSRoom* GetRoom(const Vector& v, int* pFace);                         // 0x004BB600
    char m_pad0[0x120];
    unsigned long m_oid120;                         // +0x120 (unnamed)
    char m_pad124[0x17c - 0x124];
    CExoString m_sScriptOnExit;                     // +0x17c
    char m_pad184[0x1ac - 0x184];
    CPathfindInformation* m_pPathfindInformation;   // +0x1ac
    char m_pad1b0[0x1d8 - 0x1b0];
    unsigned long m_oidLastExited;                  // +0x1d8
};

class CSWSAreaMap {
public:
    void SetWorldPointExplored(Vector v, int bExplored);                // 0x005792D0
    void SetPartyMemberWorldLocation(int nIndex, Vector v);             // 0x005790C0
    void SetPartyMemberWorldOrientation(int nIndex, Vector v);          // 0x00579130
};

class CSWSModule {
public:
    CSWSArea* GetArea();                            // 0x004C30F0
    int GetPrimaryPlayerIndex();                    // 0x004C5D90
    CExoString* GetModuleNamePtr() { return &m_sModuleName; }
    char m_pad0[0x4c];
    char m_cDescription[8];                         // +0x4c (CExoLocString)
    char m_pad54[0x5c - 0x54];
    CExoString m_sModuleName;                       // +0x5c (placeholder name)
    char m_pad64[0x218 - 0x64];
    CSWSAreaMap* m_pAreaMap;                        // +0x218
};

struct CSWGlobalVariableTable {
    void ReadCatalogue(const CResRef& cResRef);     // 0x0052AE50
    char m_data[0x1b770 - 0x100fc];
};

struct CServerExoAppInternal1b770 {
    char m_data[0x1b908 - 0x1b770];
};

#pragma pack(push, 4)
class CServerExoAppInternal {
public:
    CServerExoAppInternal();                        // 0x004B55E0
    ~CServerExoAppInternal();                       // 0x004B7230

    CSWSModule* GetModule();                                                // 0x004B14F0
    CExoString GetModuleName();                                             // 0x004B1580
    int SaveGame(unsigned long nSlot, CExoString& sName, CExoString& sDesc);// 0x004B58A0
    int LoadGame(unsigned long nSlot, CExoString& sName);                   // 0x004BA640
    int LoadCharacterStart(unsigned char nType, CSWSPlayer* pPlayer, CResRef cResRef,
                           void* pData, unsigned long nSize);               // 0x004B7470
    int LoadCharacterFinish(CSWSPlayer* pPlayer, int bArg);             // 0x004B5C50
    int LoadPrimaryPlayer(CSWSPlayer* pPlayer);                             // 0x004B5F50
    void UpdateMapData();                                                   // 0x004B4E80
    void ForceRunAreaOnExitScripts();                                       // 0x004B4F50
    int SendCharacterQuery(CSWSPlayer* pPlayer);                            // 0x004B3070
    int IncludeModuleInSave(const CExoString& sModule);                     // 0x004B20E0
    void GetServerInfoFromIniFile();                                        // 0x004B1B00
    int HasEnoughDiskSpaceForSaveGame();                                    // 0x004B2520
    CGameObject* GetGameObject(unsigned long oid);                          // 0x004B1700
    CSWSItem* GetItemByGameObjectID(unsigned long oid);                     // 0x004B1740
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);             // 0x004B1790
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);                     // 0x004B17E0
    CSWSTrigger* GetTriggerByGameObjectID(unsigned long oid);               // 0x004B1880
    CSWSPlaceable* GetPlaceableByGameObjectID(unsigned long oid);           // 0x004B18D0
    CSWSStore* GetStoreByGameObjectID(unsigned long oid);                   // 0x004B1AB0
    CSWSDoor* GetDoorByGameObjectID(unsigned long oid);                     // 0x004B1920
    CSWSAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid); // 0x004B1970
    CSWSWaypoint* GetWaypointByGameObjectID(unsigned long oid);             // 0x004B19C0
    CSWSEncounter* GetEncounterByGameObjectID(unsigned long oid);           // 0x004B1A10
    CSWSSoundObject* GetSoundObjectByGameObjectID(unsigned long oid);       // 0x004B1A60
    int GetPlayerLanguage(unsigned long nPlayerId);                         // 0x004B4050
    int GetModuleLanguage();                                                // 0x0063E7F0
    CWorldTimer* GetActiveTimer(unsigned long oid);                         // 0x004B6C40
    int Initialize();                                                       // 0x004B1650
    int MainLoop();                                                         // 0x004BABB0
    void Shutdown(int bExit, int bShutdownNetwork);                         // 0x004B69C0
    int UnloadModule();                                                     // 0x004B9240
    int LoadModule(CExoString& sModule, int bLoadFromSave);                 // 0x004B95B0 (RET 8)
    int RunModule();                                                        // 0x004B6270
    CExoString GetPlayerListString();                                       // 0x004B41C0
    int StartServices(CExoString& sModule, int a, int b);                    // 0x004B63E0
    void StopServices();                                                    // 0x004B7C60
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);               // 0x004B2A50
    CSWSClient* GetClientObjectByPlayerID(unsigned long nPlayerId, unsigned char b); // 0x004B2AE0
    void InitiateModuleForPlayer(void* pPlayer);                            // 0x004B3A90
    void MovePlayerToArea(void* pPlayer);                                   // 0x004B3D10
    int GetFactionOfObject(unsigned long oid, int* pFaction);               // 0x004B40C0
    void TogglePauseState(unsigned char nState);                            // 0x004BA610
    void SkipPastDialog(unsigned long oid);                                                  // 0x004B1F90
    int GetPauseState(unsigned char nMask);                                          // 0x004B1D80
    unsigned char GetActivePauseState();                                              // 0x004B1DA0
    void SetPauseState(unsigned char nState, int bPause);                   // 0x004B8110
    void AddToExclusionList(unsigned long oid, unsigned char nList);         // 0x004B4730
    void RemoveFromExclusionList(unsigned long oid, unsigned char nList);    // 0x004B4800
    CExoLinkedList<unsigned long>* GetActiveExclusionList();                                           // 0x004B1DC0
    CSWSPlayer* ResolvePlayerByFirstName(const CExoString& sName);          // 0x004B4960
    int GetDifficultyOption(int nOption);                                   // 0x004B1DF0
    int AddCharListRequest(unsigned long nPlayerId);                        // 0x004B6CB0
    unsigned long GetFirstPCObject();                                       // 0x004B4C50
    unsigned long GetNextPCObject();                                        // 0x004B4C80
    unsigned long ClientToServerObjectID(unsigned long oid);                // 0x004B1F70
    unsigned long GetPlayerCreatureID();                                    // 0x004B4CB0
    void UpdatePlayerPosition(unsigned long oid, const Vector& vPosition, const Vector& vOrientation);         // 0x004B1CB0
    void UpdateSubareasOnMoveTo(unsigned long oid, float a, float b);       // 0x004B6CC0
    int IsActionsPending(unsigned long oid);                                                 // 0x004B4CF0
    int CreatureAcquireItem(unsigned long oidCreature, const CResRef& cTemplate); // 0x004B1FE0
    int TestSafeLocation(unsigned long oidCreature, const Vector& v);           // 0x004B4D70
    int ComputeSafeLocation(const Vector& v, float f, unsigned long oidCreature, void* p, Vector* out); // 0x004B4DF0
    Vector GetNormalOfPoint(const Vector& vPoint);                         // 0x004B4FF0
    void ClearLastModuleFound();                                            // 0x004B1560
    void ClearAreaCache();                                                  // 0x004B1860
    int ShrinkLineByCreatures(Vector* a, Vector* b, unsigned long c);       // 0x004B2200
    void SetMoveToWorldTime(unsigned long n0, unsigned char n1, unsigned char n2, unsigned char n3, unsigned short n4, unsigned short n5, unsigned short n6);       // 0x004B22D0
    void GetMoveToWorldTime(unsigned long* p0, unsigned char* p1, unsigned char* p2, unsigned char* p3, unsigned short* p4, unsigned short* p5, unsigned short* p6);     // 0x004B2320
    int GetModulePreviouslyVisited(CExoString& sModule);                    // 0x004B51A0
    int ExecuteLoadModule(CExoString& sModule);                             // 0x004BA820
    void DealWithLoadGameError(unsigned long nError);                       // 0x004BA5B0
    int IsInjured(unsigned long oid);                                       // 0x004B1D20
    int IsOnActiveExclusionList(unsigned long oid);                         // 0x004B48E0
    int ExportAllPlayers();                                                 // 0x004B3000
    int CopyModuleToCurrentGame(CExoString& sSource, CExoString& sDest, int nUnused); // 0x004B1680
    void DumpServerOptionsToLog();                                          // 0x004B1C40
    int FUN_004b1e10(unsigned __int64 nTime);                               // 0x004B1E10
    int UpdateAutoSaveTimer();                                              // 0x004B1EE0
    int FUN_004b4030();                                                     // 0x004B4030

    char m_pad0[0x10004];
    CServerInfo* m_pServerInfo;                     // +0x10004
    unsigned short m_nServerMode;                   // +0x10008
    unsigned short m_pad1000a;
    unsigned long m_pad1000c;
    CSWSMessage* m_pServerMessage;                  // +0x10010
    char m_pad10014[0x10040 - 0x10014];
    CNetLayer* m_pNetLayer;                         // +0x10040
    unsigned long m_10044;                          // +0x10044
    unsigned long m_10048;                          // +0x10048
    CWorldTimer* m_pTimestopTimer;                  // +0x1004c
    CWorldTimer* m_pPauseTimer;                     // +0x10050
    CFactionManager* m_pFactionManager;             // +0x10054
    unsigned long m_pad10058;
    CGameObjectArray* m_pGameObjArray;              // +0x1005c
    unsigned long m_oidModule;                      // +0x10060
    CExoLinkedList<CSWSPlayer>* m_pPlayerList;      // +0x10064
    CExoLinkedList<CSWSPlayer>* m_pSysAdminList;    // +0x10068
    int m_bDebugMode;                               // +0x1006c
    CExoLinkedList<unsigned long>* m_pExclusionList2;   // +0x10070
    CExoLinkedList<unsigned long>* m_pExclusionList1;   // +0x10074
    unsigned char m_nPauseState;                    // +0x10078
    unsigned char m_pad10079[3];
    int m_bDoingStartNewModule;                     // +0x1007c
    int m_bMoveToModulePending;                     // +0x10080
    CExoString m_sMoveToModuleString;               // +0x10084
    CExoString m_sMoveToModuleStartWaypoint;        // +0x1008c
    unsigned long m_pad10094[2];
    unsigned long m_nMoveToTime0;                   // +0x1009c
    unsigned char m_nMoveToTime1;                   // +0x100a0
    unsigned char m_nMoveToTime2;                   // +0x100a1
    unsigned char m_nMoveToTime3;                   // +0x100a2
    unsigned char m_pad100a3;
    unsigned short m_nMoveToTime4;                  // +0x100a4
    unsigned short m_nMoveToTime5;                  // +0x100a6
    unsigned short m_nMoveToTime6;                  // +0x100a8
    unsigned short m_pad100aa;
    unsigned long m_nMoveToPausedTimeOfDay;         // +0x100ac
    unsigned long m_nMoveToPausedCalendarDay;       // +0x100b0
    int m_bLoadingModule;                           // +0x100b4
    int m_bAutoSavePending;                         // +0x100b8
    int m_bExportAllCharactersPending;              // +0x100bc
    unsigned long m_pad100c0;
    int m_bLoadFromSaveGame;                        // +0x100c4
    unsigned long m_pad100c8[4];
    int m_bButtonMashCheck;                         // +0x100d8
    unsigned __int64 m_nThrottleRemaining;          // +0x100dc (unnamed)
    unsigned __int64 m_nThrottleLast;               // +0x100e4 (unnamed)
    unsigned long m_pad100ec[3];
    CExoLinkedListPosition m_posPC;                 // +0x100f8
    CSWGlobalVariableTable m_cGlobalVariableTable;  // +0x100fc
    CServerExoAppInternal1b770 m_c1b770;            // +0x1b770
    CExoString m_sLastConversation;                 // +0x1b908
    unsigned long m_oidActionConversation;          // +0x1b910
    int m_bIgnoreConversationStartRange;            // +0x1b914
    unsigned long m_oidLastModule;                  // +0x1b918
    CSWSModule* m_pLastModule;                      // +0x1b91c
    unsigned long m_oidLastArea;                    // +0x1b920
    CSWSArea* m_pLastArea;                          // +0x1b924
    unsigned long m_pad1b928[2];
    unsigned long m_nAutoSaveElapsed;               // +0x1b930
    unsigned long m_nAutoSaveLastTime;              // +0x1b934
    int m_bAutoSaveDue;                             // +0x1b938
    CExoString m_sSaveGameName;                     // +0x1b93c
};
#pragma pack(pop)

class CServerExoApp {
public:
    CServerExoApp();
    virtual ~CServerExoApp();

    CSWSModule* GetModule();
    CExoString GetModuleName();
    int SaveGame(unsigned long nSlot, CExoString& sName, CExoString& sDesc);
    int LoadGame(unsigned long nSlot, CExoString& sName);
    int LoadCharacterStart(unsigned char nType, CSWSPlayer* pPlayer, CResRef cResRef,
                           void* pData, unsigned long nSize);
    int LoadCharacterFinish(CSWSPlayer* pPlayer, int bArg);
    CGameObject* GetGameObject(unsigned long oid);
    CSWSItem* GetItemByGameObjectID(unsigned long oid);
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);
    CSWSTrigger* GetTriggerByGameObjectID(unsigned long oid);
    CSWSPlaceable* GetPlaceableByGameObjectID(unsigned long oid);
    CSWSStore* GetStoreByGameObjectID(unsigned long oid);
    CSWSDoor* GetDoorByGameObjectID(unsigned long oid);
    CSWSAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid);
    CSWSWaypoint* GetWaypointByGameObjectID(unsigned long oid);
    CSWSEncounter* GetEncounterByGameObjectID(unsigned long oid);
    CSWSSoundObject* GetSoundObjectByGameObjectID(unsigned long oid);
    int GetPlayerLanguage(unsigned long nPlayerId);
    int GetModuleLanguage();
    CWorldTimer* GetActiveTimer(unsigned long oid);
    int Initialize();
    int MainLoop();
    void Shutdown(int bExit, int bShutdownNetwork);
    int UnloadModule();
    int LoadModule(CExoString& sModule, int bLoadFromSave);
    int RunModule();
    CExoString GetPlayerListString();
    int StartServices(CExoString& sModule, int a, int b);
    void StopServices();
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);
    CSWSClient* GetClientObjectByPlayerID(unsigned long nPlayerId, unsigned char b);
    void InitiateModuleForPlayer(void* pPlayer);
    void MovePlayerToArea(void* pPlayer);
    int GetFactionOfObject(unsigned long oid, int* pFaction);
    void TogglePauseState(unsigned char nState);
    void SkipPastDialog(unsigned long oid);
    int GetPauseState(unsigned char nMask);
    unsigned char GetActivePauseState();
    void SetPauseState(unsigned char nState, int bPause);
    void AddToExclusionList(unsigned long oid, unsigned char nList);
    void RemoveFromExclusionList(unsigned long oid, unsigned char nList);
    CExoLinkedList<unsigned long>* GetActiveExclusionList();
    CSWSPlayer* ResolvePlayerByFirstName(const CExoString& sName);
    int GetDifficultyOption(int nOption);
    int AddCharListRequest(unsigned long nPlayerId);
    unsigned long GetFirstPCObject();
    unsigned long GetNextPCObject();
    unsigned long ClientToServerObjectID(unsigned long oid);
    unsigned long GetPlayerCreatureID();
    float ComputeHeight(unsigned long oidArea, float fX, float fY);
    void UpdatePlayerPosition(unsigned long oid, const Vector& vPosition, const Vector& vOrientation);
    void UpdateSubareasOnMoveTo(unsigned long oid, float a, float b);
    int IsActionsPending(unsigned long oid);
    int GetRoomIndex(unsigned long oidArea, float fX, float fY);
    int CreatureAcquireItem(unsigned long oidCreature, const CResRef& cTemplate);
    int TestSafeLocation(unsigned long oidCreature, const Vector& v);
    int ComputeSafeLocation(const Vector& v, float f, unsigned long oidCreature, void* p, Vector* out);
    Vector GetNormalOfPoint(const Vector& vPoint);
    void ClearLastModuleFound();
    void ClearAreaCache();
    int ShrinkLineByCreatures(Vector* a, Vector* b, unsigned long c);
    void SetMoveToWorldTime(unsigned long n0, unsigned char n1, unsigned char n2, unsigned char n3, unsigned short n4, unsigned short n5, unsigned short n6);
    void GetMoveToWorldTime(unsigned long* p0, unsigned char* p1, unsigned char* p2, unsigned char* p3, unsigned short* p4, unsigned short* p5, unsigned short* p6);
    int GetModulePreviouslyVisited(CExoString& sModule);
    int ExecuteLoadModule(CExoString& sModule);
    CServerInfo* FUN_004aec90();
    void* GetModuleDescription();
    void SetMoveToModulePending(int b);
    void SetMoveToModuleString(const CExoString& s);
    void SetExportAllCharactersPending(int b);
    int GetLoadingModule();
    void SetLoadingModule(int b);
    void SetAutoSavePending(int b);
    int GetAutoSavePending();
    void SetMoveToModuleStartWaypoint(const CExoString& s);
    CExoString GetMoveToModuleStartWaypoint();
    CGameObjectArray* FUN_004aed70();
    unsigned long FUN_004aed80();
    CExoLinkedList<CSWSPlayer>* FUN_004aed90();
    CSWSMessage* FUN_004aedb0();
    CExoLinkedList<CSWSPlayer>* GetSysAdminList();
    unsigned short GetServerMode();
    unsigned long FUN_004aede0();
    CWorldTimer* GetTimestopTimer();
    CWorldTimer* GetPauseTimer();
    void SetDebugMode(int b);
    int GetDebugMode();
    int IsInjured(unsigned long oid);
    CSWGlobalVariableTable* GetGlobalVariableTable();
    CServerExoAppInternal1b770* FUN_004aee70();
    int TestDirectLine(unsigned long oidCreature, unsigned long oidArea,
                       float fX1, float fY1, float fX2, float fY2, float fZ1, float fZ2);
    CPathfindInformation* GetCreaturePathfindInformation(unsigned long oid);
    void ClearAllActions(unsigned long oid);
    void ClearAllQueuedCombatActions(unsigned long oid);
    CFactionManager* GetFactionManager();
    void SetLastConversation(const CExoString& s);
    CExoString GetLastConversation();
    void SetActionConversation(unsigned long oid);
    unsigned long GetActionConversation();
    void SetIgnoreConversationStartRange(int b);
    int GetIgnoreConversationStartRange();
    int GetLoadFromSaveGame();
    void SetLoadFromSaveGame(int b);
    int GetButtonMashCheck();
    void SetButtonMashCheck(int b);
    void SetDoingStartNewModule(int b);
    void SetMoveToPausedCalendarDay(unsigned long n);
    unsigned long GetMoveToPausedCalendarDay();
    void SetMoveToPausedTimeOfDay(unsigned long n);
    unsigned long GetMoveToPausedTimeOfDay();
    void SetSaveGameName(const CExoString& s);
    CSWSCreature* GetPlayerCreature();

    CServerExoAppInternal* m_pcExoAppInternal;      // +0x04
};

#endif
