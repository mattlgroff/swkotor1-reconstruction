// CPathfindInformation / CWalkHitInfo leaves (0x004BB340-0x004BB570)
// and the CSWSModule inter-area path / update / journal / limbo cluster with
// its CExoArrayList / CResIFO leaves (0x004C30C0-0x004C8940).
// Self-contained declarations; only the members these functions use are
// placed.
#ifndef I_SWS_MODULE_H
#define I_SWS_MODULE_H

#include <stddef.h>

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

// 0x48 bytes (+0x40 not touched by Initialize).
class CWalkHitInfo {
public:
    void Initialize(int nType, const Vector& v1, const Vector& v2, const Vector& v3,
                    const Vector& v4, int nIndex, const Vector& v5);    // 0x004BB570

    int m_nType;                                    // +0x00
    Vector m_v04;                                   // +0x04
    Vector m_v10;                                   // +0x10
    Vector m_v1c;                                   // +0x1c
    Vector m_v28;                                   // +0x28
    Vector m_v34;                                   // +0x34
    int m_n40;                                      // +0x40
    int m_n44;                                      // +0x44
};

class CPathfindInformation {
public:
    void FUN_004bb340(float* p);                    // 0x004BB340
    void FUN_004bb380(Vector* pA, Vector* pB);      // 0x004BB380
    void FUN_004bb3c0(const Vector* pA, const Vector* pB);  // 0x004BB3C0
    void FUN_004bb400(int n, float* p);             // 0x004BB400
    void FUN_004bb440(int n, float* p);             // 0x004BB440
    void FUN_004bb480(float* p);                    // 0x004BB480
    void FUN_004bb4d0(float* p);                    // 0x004BB4D0
    void FUN_004bb520(float* p);                    // 0x004BB520
    void FUN_005cec50();                            // 0x005CEC50 (flips the path direction)
    void FUN_005cec90();                            // 0x005CEC90
    void ResetGridSearchData();                     // 0x005CEFA0
    void ResetInterTileSearchData();                // 0x005D00A0
    void ResetInterAreaPathSearchData();            // 0x005D0E00
    void ResetInterAreaSearchData();                // 0x005D0E70

    char m_pad0[4];
    float m_f04;                                    // +0x04
    char m_pad08[0x2c - 8];
    unsigned long m_oidCreature;                    // +0x2c
    unsigned long m_oidTransition;                  // +0x30
    char m_pad34[0x3c - 0x34];
    int m_n3c;                                      // +0x3c
    char m_pad40[4];
    Vector m_v44;                                   // +0x44
    float m_f50;                                    // +0x50
    unsigned long m_oid54;                          // +0x54
    int m_nState;                                   // +0x58
    Vector m_vGoal;                                 // +0x5c
    float m_f68;                                    // +0x68
    char m_pad6c[4];
    unsigned long m_oidTargetArea;                  // +0x70
    Vector m_vTarget;                               // +0x74
    unsigned long m_oidStartArea;                   // +0x80
    int m_n84;                                      // +0x84
    int m_n88;                                      // +0x88
    char m_pad8c[0xa0 - 0x8c];
    int m_bInterAreaPathFound;                      // +0xa0
    int m_nA4;                                      // +0xa4
    int m_nAreas;                                   // +0xa8
    unsigned long* m_pAreas;                        // +0xac
    unsigned long* m_pTransitions;                  // +0xb0
    int m_nExcluded;                                // +0xb4
    unsigned long* m_pExcluded;                     // +0xb8
    char m_padbc[0x1ac - 0xbc];
    float* m_p1ac;                                  // +0x1ac
    char m_pad1b0[0x1e4 - 0x1b0];
    Vector m_v1e4;                                  // +0x1e4
    Vector m_v1f0;                                  // +0x1f0
    int m_n1fc;                                     // +0x1fc
    float* m_p200;                                  // +0x200
    int m_n204;                                     // +0x204
    float* m_p208;                                  // +0x208
    char m_pad20c[0x218 - 0x20c];
    int m_n218;                                     // +0x218
    float* m_p21c;                                  // +0x21c
    int m_n220;                                     // +0x220
    int m_n224;                                     // +0x224
    float* m_p228;                                  // +0x228
    float* m_p22c;                                  // +0x22c
};

// ---- module cluster ---------------------------------------------------------

extern "C" char* strncpy(char* d, const char* s, size_t n);
extern "C" int strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const char* s) const;            // 0x005E5290
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    int Find(char c, int nStart) const;             // 0x005E5F20
    CExoString LowerCase() const;                   // 0x005E6080
    CExoString Right(int nCount) const;             // 0x005E6170
    char* m_sString;
    unsigned long m_nBufferLength;
};

// 8 bytes; Compare is 0x005EA020 (non-zero when equal).
class CExoLocString {
public:
    ~CExoLocString();                               // 0x005EA000
    int Compare(const CExoLocString& o) const;      // 0x005EA020
    void* m_pInternal;
    unsigned long m_n4;
};

struct CResRef {
    int operator!=(const char* s) const;            // 0x00406060
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CRes {
public:
    virtual ~CRes();
    void CancelRequest();                           // 0x00409B00
};

class CExoResMan {
public:
    int ReleaseResObject(CRes* pRes);               // 0x00409CF0
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CResGFF : public CRes {
public:
    CResGFF();                                      // 0x004105A0
    virtual ~CResGFF();
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    char m_pad4[0x8d - 4];
    char m_pFileType[4];                            // +0x8d
    char m_pad91[3];
};

class CResIFO : public CResGFF {
public:
    CResIFO();                                      // 0x004C30C0
    virtual ~CResIFO();
};

template <class T, int nType>
class CResHelper {
public:
    CResHelper() {}
    virtual ~CResHelper();                          // <CResIFO, 2014> 0x004C4C60
    int m_bAutoRequest;
    T* m_pRes;
    CResRef m_cResRef;
};

template <class T>
class CExoArrayList {
public:
    void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
    void Insert(T t, int nIndex)
    {
        int i = num;
        Add(t);
        for (; i > nIndex; i--)
            element[i] = element[i - 1];
        element[nIndex] = t;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void Remove(T t)
    {
        for (int i = num - 1; i >= 0; i--) {
            if (t == element[i]) {
                DelIndex(i);
                return;
            }
        }
    }
    int Contains(T t)
    {
        for (int i = num - 1; i >= 0; i--) {
            if (t == element[i]) {
                return 1;
            }
        }
        return 0;
    }
    int contains(T t);
    void Clear()
    {
        if (element) {
            delete[] element;
            element = NULL;
            array_size = 0;
        }
        num = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = NULL;
    }
    T& operator[](int i) { return element[i]; }

    T* element;
    int num;
    int array_size;
};

// 0x28 bytes, sorted by tag.
struct CSWSTagNode {
    char m_szTag[0x20];
    unsigned long m_n20;
    unsigned long m_n24;
};

// 8-byte element of the list grown by 0x004C4F50 (layout unknown).
struct CSWSModulePair {
    unsigned long m_n0;
    unsigned long m_n4;
};

struct NWPlayerCharacterList_st {
    int operator==(const NWPlayerCharacterList_st& o) const
    {
        return m_sFirstName.Compare(o.m_sFirstName) && m_sLastName.Compare(o.m_sLastName);
    }
    ~NWPlayerCharacterList_st()                     // 0x004C5DF0
    {
        delete m_pData;
        m_pData = NULL;
    }
    CExoLocString m_sFirstName;                     // +0x00
    CExoLocString m_sLastName;                      // +0x08
    char m_pad10[0x34 - 0x10];
    unsigned char* m_pData;                         // +0x34
};

// 0x10 bytes; ctor 0x0059EA90 and dtor 0x0059EA30 are out of line.
class CSWSModuleStringEntry {
public:
    CSWSModuleStringEntry();                        // 0x0059EA90
    ~CSWSModuleStringEntry();                       // 0x0059EA30
    CExoString m_sName;                             // +0x00
    unsigned long m_n8;                             // +0x08
    unsigned long m_nC;                             // +0x0c
};

// 0x24 bytes.
class CWorldJournalEntry {
public:
    CWorldJournalEntry() {}                         // 0x004C3FE0
    CExoString m_sName;                             // +0x00
    CExoString m_sText;                             // +0x08
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned long m_n18;
    unsigned long m_nID;                            // +0x1c (id or strref)
    unsigned long m_bStrref : 1;                    // +0x20
};

class CGameObject;
class CSWSCreature;
class CSWSArea;
class CSWSPlayer;
class CSWSWaypoint;

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_TRIGGER;     // 0x00746636
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern const unsigned char OBJECT_TYPE_WAYPOINT;    // 0x0074663B
extern const int EFFECT_TYPE_TIMESTOP;              // 0x0074677C
extern const int PLOT_PATH_OK;                      // 0x00745DBC
extern const int PLOT_PATH_DONE;                    // 0x00745DC0
extern const int PLOT_PATH_FAIL;                    // 0x00745DC4
extern int g_bWalkDebug;                            // 0x00832838

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject); // 0x004D8230
};

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
    CExoLinkedListInternal* m_pList;
};

class CSWSObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    char m_pad0[4];
    unsigned long m_idSelf;                         // +0x04
};

class CSWSTrigger : public CSWSObject {
public:
    unsigned long GetTargetArea();                  // 0x0058C830
    int FUN_0058d8c0(Vector* pIn, Vector* pOut);    // 0x0058D8C0
    char m_pad8[0x2b4 - 8];
    int m_bAreaTransition;                          // +0x2b4
};

class CSWSDoor : public CSWSObject {
public:
    int FUN_005890d0();                             // 0x005890D0
    void FUN_00589240(Vector* pOut, const Vector& vIn, int n);    // 0x00589240
    CSWSObject* GetLinkedObject();                  // 0x00589580
};

class CGameEffect {
public:
    char m_pad0[8];
    unsigned short m_nType;                         // +0x08
};

class CSWSCreature {
public:
    float FUN_005111c0();                           // 0x005111C0
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);  // 0x00500610 (placeholder name)
    char m_pad0[4];
    unsigned long m_idSelf;                         // +0x04
    char m_pad8[0x124 - 8];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // +0x124
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual CSWSDoor* AsSWSDoor();                  // +0x18
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual CSWSArea* AsSWSArea();                  // +0x28
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual CSWSTrigger* AsSWSTrigger();            // +0x40

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CSWSArea {
public:
    int CountAreaTransitionTriggers(unsigned long* pList, int nCount,
                                    unsigned long* pExclude);       // 0x004BC400
    void FUN_00508a30();                            // 0x00508A30
    int LoadAreaFromData(void* pData, CExoString& sGitName,
                         unsigned long nSize);      // 0x0050BA00 (placeholder name)
    char m_pad0[0x120];
    unsigned long m_oid120;                         // +0x120
    char m_pad124[0x1e0 - 0x124];
    int m_n1e0;                                     // +0x1e0
};

class CSWSPlayer {
public:
    CGameObject* GetGameObject();                   // 0x00561080
};

class CSWSMessage {
public:
    void WriteBYTE(unsigned char nValue, int nBits);                    // 0x004D62C0 (CNWMessage)
    void WriteCResRef(CResRef cResRef, int nBits);                      // 0x004D6380 (CNWMessage)
    void WriteCExoString(const CExoString& sValue, int nBits);          // 0x004D6960 (CNWMessage)
    void WriteCExoLocStringServer(const CExoLocString& sValue,
                                  unsigned char nGender);               // 0x005785C0
    int SendServerToPlayerJournalAddWorld(CSWSPlayer* p, unsigned long nID, const CExoString& sName,
                                          const CExoString& sText, unsigned long n10,
                                          unsigned long n14);                           // 0x0056A1A0
    int SendServerToPlayerJournalAddWorldStrref(CSWSPlayer* p, unsigned long nStrref, unsigned long n18,
                                                unsigned long n10, unsigned long n14);  // 0x0056A260
    void SendServerToPlayerLoadBar_UpdateStallEvent(unsigned long nStep, unsigned long nTotal);  // 0x0056C910
    int SendServerToPlayerJournalDeleteWorld(CSWSPlayer* p, unsigned long nID);         // 0x0056A300
    int SendServerToPlayerJournalDeleteWorldStrref(CSWSPlayer* p, unsigned long nID);   // 0x0056A370
    int SendServerToPlayerJournalDeleteWorldAll(CSWSPlayer* p);                         // 0x0056A3E0
    int SendServerToPlayerModuleUpdate_Time(CSWSPlayer* p, unsigned char nFlags,
                                            unsigned long nYear, unsigned long nMonth,
                                            unsigned long nDay, unsigned long nHour,
                                            unsigned char nPhase, unsigned long nTime); // 0x0056A5F0
};

class CWorldTimer {
public:
    void FUN_004add50(unsigned long nDay, unsigned long* pYear, unsigned long* pMonth,
                      unsigned long* pDay);         // 0x004ADD50
    void FUN_004add90(unsigned long nTime, unsigned long* pHour, unsigned long* pMinute,
                      unsigned long* pSecond, unsigned long* pMs);  // 0x004ADD90
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    void FUN_004ae460(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDays,
                      unsigned long* pTime);        // 0x004AE460
};

class CServerAIMaster {
public:
    char m_pad0[0x68];
    int m_n68;                                      // +0x68
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);         // 0x004AE770
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);                 // 0x004AE780
    void TogglePauseState(unsigned char nState);                        // 0x004AE960
    unsigned char GetActivePauseState();                                // 0x004AE990
    CExoLinkedList<unsigned long>* GetActiveExclusionList();            // 0x004AE9D0
    CGameObjectArray* GetObjectArray();                                 // 0x004AED70
    CServerAIMaster* GetServerAIMaster();                               // 0x004AED80
    CExoLinkedList<CSWSPlayer>* GetPlayerList();                        // 0x004AED90
    CSWSMessage* GetSWSMessage();                                       // 0x004AEDB0
    CWorldTimer* GetWorldTimer();                                       // 0x004AEDE0
};

struct CAppManager14 {
    char m_pad0[8];
    unsigned long m_n8;                             // +0x08
    unsigned long m_nC;                             // +0x0c
};

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                          // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
    char m_pad0c[0x14 - 0x0c];
    CAppManager14* m_p14;                           // +0x14
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CVirtualMachine {
public:
    int RunScript(CExoString* psScript, unsigned long oid, int bOid);  // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

// Player list entry walked by GetPrimaryPlayerIndex.
struct CSWSModulePlayerEntry {
    char m_pad0[0x18];
    int m_bPrimary;                                 // +0x18
};

class CSWSModule {
public:
    virtual ~CSWSModule();                          // 0x004C68A0

    CSWSArea* GetArea();                            // 0x004C30F0
    void ClearAreaCache()
    {
        m_pCachedArea = NULL;
        m_oidCachedArea = 0x7f000000;
    }                                               // 0x004C3150
    void GetTime(unsigned long* pYear, unsigned long* pMonth, unsigned long* pDay,
                 unsigned long* pHour, unsigned char* pPhase,
                 unsigned long* pTime);             // 0x004C4100 (FUN_004c4100)
    int GetPrimaryPlayerIndex();                    // 0x004C5D90
    int FindTagNode(const char* szTag);             // 0x004C5E60 (FUN_004c5e60)
    void SaveLimboCreatures(CResGFF* pRes, CResStruct* pStruct);  // 0x004C5BB0
    unsigned long GetObjectByTagAndType(const CExoString& sTag, int nType,
                                        int nNth);  // 0x004C60B0 (placeholder name)
    void SetLowerCaseName(const CExoString& sName); // 0x004C3060 (placeholder name)
    void WriteModuleInfo(void* pUnused);            // 0x004C3170 (placeholder name)
    CExoString GetModuleShortName();                // 0x004C4B80 (placeholder name)
    int RemoveObjectFromLookupTable(const CExoString& sTag, unsigned long oid);  // 0x004C5EF0 (placeholder name)
    unsigned long GetObjectByTag(const CExoString& sTag, int nNth);   // 0x004C5FE0 (placeholder name)
    int AddObjectToLookupTable(const CExoString& sTag, unsigned long oid);  // 0x004C7DE0 (placeholder name)
    int LoadTemporaryArea();                        // 0x004C3B10 (placeholder name)
    void AddWorldJournalEntry(unsigned long nID, const CExoString& sName, const CExoString& sText,
                              unsigned long n10, unsigned long n14);    // 0x004C7ED0 (placeholder name)
    void AddWorldJournalEntryStrref(unsigned long nStrref, unsigned long n18, unsigned long n10,
                                    unsigned long n14);                 // 0x004C8030 (placeholder name)
    int GetNextWorldJournalEntryID();               // 0x004C61D0 (FUN_004c61d0)
    void UpdateTime(unsigned long nDay, unsigned long nTime, unsigned long nDelta); // 0x004C6210
    void RemoveObjectFromLimbo(unsigned long oid);  // 0x004C63F0 (FUN_004c63f0)
    int IsObjectInLimbo(unsigned long oid);         // 0x004C6450 (FUN_004c6450)
    void TimeStopSanityCheck();                     // 0x004C6480
    int AIUpdate();                                 // 0x004C6C90
    void DoUpdate();                                // 0x004C6DC0
    unsigned long GetWaypoint(const CExoString& sTag);  // 0x004C6E00
    void DeleteWorldJournalEntry(unsigned long nID);        // 0x004C8170
    void DeleteWorldJournalEntryStrref(unsigned long nStrref);  // 0x004C8230
    void DeleteWorldJournalAllEntries();            // 0x004C82F0
    void AddObjectToLimbo(unsigned long oid);       // 0x004C83B0
    void SetIntraAreaGoal(CPathfindInformation* pPath);                     // 0x004C3EA0
    int PlotPathInArea(CPathfindInformation* pPath, int nArg);             // 0x004C3260
    int GenerateInterAreaDFSSuccessors(int nDepth, CPathfindInformation* pPath,
                                       unsigned long** ppList);             // 0x004C54A0
    int InterAreaDFS(int nDepth, int nMaxDepth, CPathfindInformation* pPath); // 0x004C5510
    int ComputeInterAreaPath(CPathfindInformation* pPath);                  // 0x004C6BD0
    int PlotInterAreaPath(CPathfindInformation* pPath, int nArg);          // 0x004C6E20
    int PlotPath(CPathfindInformation* pPath, int nArg);                   // 0x004C6F70

    char m_pad4[0x20 - 4];
    unsigned long m_idSelf;                         // +0x20
    char m_pad24[0x40 - 0x24];
    unsigned long m_oidArea;                        // +0x40
    int m_bAIUpdateToggle;                          // +0x44
    CExoLinkedList<CSWSModulePlayerEntry> m_lPlayers;   // +0x48
    char m_pad4c[0x5c - 0x4c];
    CExoString m_sModuleName;                       // +0x5c (placeholder name)
    CExoLocString m_cName;                          // +0x64 (placeholder name)
    CResRef m_cResRef6c;                            // +0x6c (placeholder name)
    CResRef m_cResRef7c;                            // +0x7c (placeholder name)
    char m_pad8c[0xb0 - 0x8c];
    CExoString m_sScripts[1];                       // +0xb0 (heartbeat first)
    char m_padb8[0x128 - 0xb8];
    unsigned long m_nLastHeartbeatDay;              // +0x128
    unsigned long m_nLastHeartbeatTime;             // +0x12c
    CExoArrayList<CSWSTagNode> m_aTagNodes;         // +0x130
    char m_pad13c[0x1a0 - 0x13c];
    unsigned char m_n1a0;                           // +0x1a0 (placeholder name)
    unsigned char m_nDawnHour;                      // +0x1a1
    unsigned char m_nDuskHour;                      // +0x1a2
    char m_pad1a3[0x1a8 - 0x1a3];
    unsigned long m_nYear;                          // +0x1a8
    unsigned long m_nMonth;                         // +0x1ac
    unsigned long m_nDay;                           // +0x1b0
    unsigned long m_nHour;                          // +0x1b4
    unsigned long m_nTransitionTime;                // +0x1b8
    unsigned char m_nPhase;                         // +0x1bc
    char m_pad1bd[3];
    unsigned long m_nLastUpdateDay;                 // +0x1c0
    unsigned long m_nLastUpdateTime;                // +0x1c4
    char m_pad1c8[4];
    CExoArrayList<CWorldJournalEntry> m_aWorldJournal;  // +0x1cc
    char m_pad1d8[4];
    CExoArrayList<unsigned long> m_aLimbo;          // +0x1dc
    void* m_pTempAreaData;                          // +0x1e8 (placeholder name)
    char m_pad1ec[0x1f4 - 0x1ec];
    unsigned long m_nTempAreaSize;                  // +0x1f4 (placeholder name)
    CExoString m_sLowerCaseName;                    // +0x1f8 (placeholder name)
    char m_pad200[0x21c - 0x200];
    unsigned long m_oidCachedArea;                  // +0x21c
    CSWSArea* m_pCachedArea;                        // +0x220
};

#endif
