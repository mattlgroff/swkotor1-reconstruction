// CSWSArea leaves, object-list saves and stealth XP
// (0x00506A10-0x00508C4F). Self-contained declarations; only the members
// these functions use are placed.
#ifndef A_SWS_AREA_H
#define A_SWS_AREA_H

#include <stddef.h>

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {}
    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    float x, y, z;
};

struct Vector;
class Quaternion {
public:
    Quaternion() {}
    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960
    float w, x, y, z;
};

class CResRef {
public:
    CResRef(const char* szResRef);                  // 0x00406D80
    char m_resRef[16];
};

template <class T> class CExoArrayList {
public:
    void Add(T t);                                  // folded instantiation, e.g. 0x00671C00
    void Remove(T t);                               // folded instantiation, e.g. 0x0066DCC0
    void Insert(T t, int nIndex);                   // folded instantiation, e.g. 0x00616940
    void Allocate(int nSize);                       // folded instantiation, e.g. 0x005E03C0
    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                n++;
            }
        }
        return n;
    }

    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                return i;
            }
        }
        return -1;
    }

    T* element;
    int num;
    int array_size;
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
    void* Demand();                                 // 0x00409B10
    int Release();                                  // 0x00409B80
};

class CResGFF : public CRes {
public:
    CResGFF(unsigned short nType, char* szFileType, const CResRef& cResRef); // 0x00410630
    int GetTopLevelStruct(CResStruct* pStruct);                             // 0x00411240
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x004118C0
    unsigned long GetListCount(CResList* pList);                                    // 0x00411940
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel); // 0x00411A10

    char pad_0x4[0x94 - 0x4];
    int m_bLoaded94;                                // 0x94 (placeholder name)
    char pad_0x98[0xa0 - 0x98];
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault);         // 0x00411C90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault);   // 0x00411D00
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess,
                             const CResRef& cDefault);                  // 0x00411E10
    unsigned long GetStructID(CResStruct* pStruct);                     // 0x004111C0
    Quaternion ReadFieldQuaternion(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const Quaternion& qDefault);         // 0x004121B0
    Vector ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess,
                           const Vector& vDefault);                     // 0x004122A0
    void* ReadFieldVOID(CResStruct* pStruct, void* pBuffer, unsigned long nSize, char* szLabel,
                        int& bSuccess, void* pDefault);                 // 0x00412380
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldQuaternion(CResStruct* pStruct, const Quaternion& qValue, char* szLabel); // 0x00412CA0
    int WriteFieldVector(CResStruct* pStruct, const Vector& vValue, char* szLabel); // 0x00412D30
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                // 0x00411A60
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);               // 0x00411C20
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int AddStructToStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel,
                          unsigned long nType);                         // 0x004125B0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize,
                       char* szLabel);                                    // 0x00412C10
};

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    int GetLength() const;                          // 0x005E5790
    ~CExoString();                                  // 0x005E5C20

    char* m_sString;
    unsigned long m_nBufferLength;
};

// 8-byte value copied to CSWSArea +0x08 before the layout loads (unknown type).
struct CSWSAreaValue8 {
    int m_n0;
    int m_n4;
};
extern CSWSAreaValue8 g_cAreaValue8;               // 0x007A1B48 (placeholder name)

Vector NormalizeVector(const Vector& vVector);      // 0x004AB180

class CSWSScriptVarTable {
public:
    void LoadVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059AA80
    char pad_0x0[0x10];
};

class CSWVarTable {
public:
    void LoadVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059B0F0
    char pad_0x0[0x10];
};

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize); // 0x00408BC0
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

extern "C" double __cdecl ceil(double x);

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                           unsigned long nTime2, unsigned long* pDay, unsigned long* pTime); // 0x004AE460
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psFile, unsigned long oidSelf, int bOid);   // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

extern "C" int __cdecl rand(void);

struct CSWSPathPoint {
    float m_fX;                                     // 0x00
    float m_fY;                                     // 0x04
    unsigned long m_nConnections;                   // 0x08
    unsigned long m_nFirstConnection;               // 0x0c
};

class CLYT;
class CSWSArea;
class CSWSRoom;
class CSWSObject;
class CSWSModule;
class CSWSCreature;
class CSWSItem;
class CSWSDoor;
class CSWSTrigger;
class CSWSPlaceable;
class CSWSAreaOfEffectObject;
class CSWSWaypoint;
class CSWSEncounter;
class CSWSStore;
class CSWSSoundObject;

class CGameObject {
public:
    virtual ~CGameObject() {}                       // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject* AsSWSObject();              // 0x10
    virtual void v14();
    virtual CSWSDoor* AsSWSDoor();                  // 0x18
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
    virtual void v34();
    virtual CSWSItem* AsSWSItem();                  // 0x38
    virtual void v3c();
    virtual CSWSTrigger* AsSWSTrigger();            // 0x40
    virtual void v44();
    virtual CSWSPlaceable* AsSWSPlaceable();        // 0x48
    virtual void v4c();
    virtual CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject();  // 0x50
    virtual void v54();
    virtual CSWSWaypoint* AsSWSWaypoint();          // 0x58
    virtual CSWSEncounter* AsSWSEncounter();        // 0x5c
    virtual void v60();
    virtual CSWSStore* AsSWSStore();                // 0x64
    virtual CSWSSoundObject* AsSWSSoundObject();    // 0x68

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CSWSObject : public CGameObject {
public:
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7c(); virtual void v80(); virtual void v84(); virtual void v88();
    virtual void v8c(); virtual void v90();
    virtual int GetDead();                          // 0x94 (placeholder name)

    void SaveObjectState(CResGFF* pRes, CResStruct* pStruct);   // 0x004CEC50
    void LoadObjectState(CResGFF* pRes, CResStruct* pStruct);   // 0x004D1CF0
    void SetPosition(const Vector& vPosition, int bArg2, int bArg3, int bArg4); // 0x004CD1A0
    void SetOrientation(const Vector& vOrientation);            // 0x004CD170

    char pad_0xc[0x78 - 0xc];
    int m_nAILevel;                                 // 0x78 (placeholder name)
    char pad_0x7c[0x90 - 0x7c];
    Vector m_vPosition;                             // 0x90
};

class CSWSCreature : public CSWSObject {
public:
    CSWSCreature(unsigned long oidSelf, int bArg2);          // 0x004F7A10
    int LoadFromTemplate(const CResRef& cResRef, int bArg2); // 0x005026D0
    int LoadCreature(CResGFF* pRes, CResStruct* pStruct, int bArg3); // 0x00500350
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg5, int bArg6); // 0x004FA100
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);   // 0x00500610 (placeholder name)

    char pad_0x9c[0x340 - 0x9c];
    class CPathfindInformation* m_pPathfindInformation; // 0x340
    char pad_0x344[0x398 - 0x344];
    int m_n398;                                     // 0x398 (placeholder name)
    char pad_0x39c[0xa88 - 0x39c];
    int m_bPartyMember;                             // 0xa88 (placeholder name)
    char pad_0xa8c[0xac8 - 0xa8c];
};

class CSWItem {
public:
    virtual ~CSWItem();
    char pad_0x4[0x10 - 0x4];
};

class CSWSItem : public CSWItem, public CSWSObject {
public:
    CSWSItem(unsigned long oidSelf);                        // 0x005530A0
    virtual ~CSWSItem();
    int LoadFromTemplate(const CResRef& cResRef);           // 0x005608B0
    int LoadItem(CResGFF* pRes, CResStruct* pStruct);       // 0x00560970
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x00555720
    void SaveItem(CResGFF* pRes, CResStruct* pStruct);      // 0x0055CCD0

    char pad_0xac[0x29c - 0xac];
};

class CSWSDoor : public CSWSObject {
public:
    CSWSDoor(unsigned long oidSelf);                        // 0x00589EE0
    int LoadDoorExternal(CResGFF* pRes, CResStruct* pStruct, int bArg3); // 0x0058C5F0
    void SetOrientation(Quaternion qOrientation);           // 0x005896E0
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x0058B6D0
    int SaveDoor(CResGFF* pRes, CResStruct* pStruct);       // 0x00588AD0

    char pad_0x9c[0x250 - 0x9c];
    CExoString m_sLinkedTo;                         // 0x250 (placeholder name)
    char pad_0x258[0x2b4 - 0x258];
    float m_fBearing;                               // 0x2b4
    char pad_0x2b8[0x2e8 - 0x2b8];
    int m_bTrapFlag2E8;                             // 0x2e8 (placeholder name)
    char pad_0x2ec[0x3c0 - 0x2ec];
    int m_bLocked3C0;                               // 0x3c0 (placeholder name)
    char pad_0x3c4[0x3d0 - 0x3c4];
};

class CSWSTrigger : public CSWSObject {
public:
    int SaveTrigger(CResGFF* pRes, CResStruct* pStruct);    // 0x0058E660

    char pad_0x9c[0x2bc - 0x9c];
    int m_bIsTrap;                                  // 0x2bc (placeholder name)
    char pad_0x2c0[0x314 - 0x2c0];
    int m_bCreatedByPlayer;                         // 0x314 (placeholder name)
};

class CSWSPlaceable : public CSWSObject {
public:
    CSWSPlaceable(unsigned long oidSelf);                   // 0x005877E0
    int LoadFromTemplate(const CResRef& cResRef, int bArg2); // 0x00587A70 (placeholder name)
    int LoadPlaceable(CResGFF* pRes, CResStruct* pStruct, int bArg3); // 0x00585670
    void SetOrientation(const Quaternion& qOrientation);    // 0x005853C0 (placeholder name)
    void AddToArea(CSWSArea* pArea, const Vector& vPosition, int bArg3); // 0x00587170
    int SavePlaceable(CResGFF* pRes, CResStruct* pStruct);  // 0x00586A70

    char pad_0x9c[0x25c - 0x9c];
    float m_fBearing;                               // 0x25c
    char pad_0x260[0x278 - 0x260];
    int m_bTrapFlag278;                             // 0x278 (placeholder name)
    char pad_0x27c[0x398 - 0x27c];
    int m_bStatic398;                               // 0x398 (placeholder name)
    char pad_0x39c[0x44c - 0x39c];
    int m_bPartyOwned;                              // 0x44c (placeholder name)
    char pad_0x450[0x454 - 0x450];
};

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    CSWSAreaOfEffectObject(unsigned long oidSelf);          // 0x00594480
    int LoadEffect(CResGFF* pRes, CResStruct* pStruct);     // 0x00594B00
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x00595F30
    int SaveEffect(CResGFF* pRes, CResStruct* pStruct);     // 0x00594D80 (placeholder name)

    char pad_0x9c[0x290 - 0x9c];
};

class CSWSWaypoint : public CSWSObject {
public:
    CSWSWaypoint(unsigned long oidSelf);                    // 0x005C7E70
    int LoadWaypoint(CResGFF* pRes, CResStruct* pStruct);   // 0x005C7F30
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x005C8580
    int SaveWaypoint(CResGFF* pRes, CResStruct* pStruct);   // 0x005C8230

    char pad_0x9c[0x240 - 0x9c];
};

class CSWSEncounter : public CSWSObject {
public:
    CSWSEncounter(unsigned long oidSelf);                   // 0x00593C70
    int LoadFromTemplate(const CResRef& cResRef, int bArg2, Vector* pPosition); // 0x00593A90
    int LoadEncounter(CResGFF* pRes, CResStruct* pStruct);  // 0x00593830
    int LoadEncounterGeometry(CResGFF* pRes, CResList& cList);     // 0x00590580
    int LoadEncounterSpawnPoints(CResGFF* pRes, CResList& cList);  // 0x00590410
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x00593520
    int SaveEncounter(CResGFF* pRes, CResStruct* pStruct);  // 0x00591350

    char pad_0x9c[0x31c - 0x9c];
};

class CSWSStore : public CSWSObject {
public:
    CSWSStore(unsigned long oidSelf);                       // 0x005C6AB0
    int LoadFromTemplate(const CResRef& cResRef, int bArg2); // 0x005C7760
    int LoadStore(CResGFF* pRes, CResStruct* pStruct, int bArg3); // 0x005C7180
    void AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bRunScripts); // 0x005C6E80
    int SaveStore(CResGFF* pRes, CResStruct* pStruct);      // 0x005C6CD0

    char pad_0x9c[0x258 - 0x9c];
};

class CSWSSoundObject : public CSWSObject {
public:
    CSWSSoundObject(unsigned long oidSelf);                 // 0x005C8F30
    int LoadFromTemplate(const CResRef& cResRef);           // 0x005C94E0
    int LoadSound(CResGFF* pRes, CResStruct* pStruct);      // 0x005C9040
    void AddToArea(CSWSArea* pArea, int bRunScripts);       // 0x005C8950
    int Save(CResGFF* pRes, CResStruct* pStruct);           // 0x005C86D0

    char pad_0x9c[0x24c - 0x9c];
    unsigned char m_nGeneratedType;                 // 0x24c
    char pad_0x24d[0x280 - 0x24d];
};

extern const unsigned char g_nGameObjectFound;      // 0x0074666C
extern const unsigned char g_nObjectTypeArea;       // 0x00746633 (placeholder name)

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);  // 0x004D8230
};

class CServerAIMaster {
public:
    int SetAILevel(CSWSObject* pObject, int nLevel);    // 0x004B08A0
    void AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                           unsigned long oidObject, unsigned long nEvent, void* pData);  // 0x004B08D0
};

class CScriptEvent {
public:
    CScriptEvent();                                 // 0x004D7540
    void SetInteger(int nIndex, int nValue);        // 0x004D76B0
    unsigned short m_nType;                         // 0x00
    char pad_0x2[0x34 - 0x2];
};

extern const unsigned long EVENT_0x7441DC;          // 0x007441DC (placeholder name)

class CSWPartyTable {
public:
    void UnstealthParty(int bValue);                    // 0x00563C60
    void DistributeExperience(unsigned long nXP, int bShow);    // 0x005653A0
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
    CSWSTrigger* GetTriggerByGameObjectID(unsigned long oid);   // 0x004AE790
    class CSWSClient* GetClientObjectByObjectID(unsigned long oid); // 0x004AE910
    int GetLoadFromSaveGame();                                  // 0x004AF050
    CGameObjectArray* GetObjectArray();                         // 0x004AED70
    CServerAIMaster* GetServerAIMaster();                       // 0x004AED80
    CSWPartyTable* GetPartyTable();                             // 0x004AEE70
    CSWSModule* GetModule();                                     // 0x004AE6B0
    CWorldTimer* GetWorldTimer();                                // 0x004AEDE0
};

class CSWCArea {
public:
    void SetServerArea(class CSWSArea* pArea);          // 0x00605690 (placeholder name)
};

struct CSWPlaceableCamera {
    int m_nCameraID;                                // 0x00
    Vector m_vPosition;                             // 0x04
    Quaternion m_qOrientation;                      // 0x10
    float m_fPitch;                                 // 0x20
    float m_fHeight;                                // 0x24
    float m_fFieldOfView;                           // 0x28
    float m_fMicRange;                              // 0x2c
};

class CGuiInGame {
public:
    void SetPlaceableCameraCount(int nCount);       // 0x0062AC80 (placeholder name)
    void SetPlaceableCamera(int nIndex, int nCameraID, Vector* pPosition, Quaternion* pOrientation,
                            float fPitch, float fHeight, float fFieldOfView,
                            float fMicRange);       // 0x0062ACA0 (placeholder name)
    CSWPlaceableCamera* GetPlaceableCamera(int nIndex);  // 0x0062AD30
    void ShowStealthXP(int nType, unsigned long nXP);   // 0x0062EEB0 (placeholder name)

    char pad_0x0[0x1b0];
    int m_nPlaceableCameras;                        // 0x1b0
};

class CClientExoApp {
public:
    CSWCArea* GetAreaByGameObjectID(unsigned long oid); // 0x005ED5C0
    CGuiInGame* GetInGameGui();                         // 0x005ED690
    void SetInputActive(int bActive, int bArg);         // 0x005EDA10
    unsigned long ServerToClientObjectId(unsigned long oid);    // 0x005EDA50
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x4
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CSWSAmbientSound {
public:
    int Load(CResGFF* pRes, CResStruct* pStruct);   // 0x005C95F0
    int Save(CResGFF* pRes, CResStruct* pStruct);   // 0x005C96E0
};

class CSWSAreaMap {
public:
    unsigned long* GetExploredData(unsigned long* pWords, int* pGridWidth,
                                   int* pGridHeight); // 0x00579090
    void SetExploredData(unsigned long* pData, unsigned long nWords, int nGridWidth,
                         int nGridHeight);          // 0x00578FC0
    unsigned long* m_pBits;
    unsigned long m_nWords;
};

class CSWSModule {
public:
    char pad_0x0[0x218];
    CSWSAreaMap* m_pAreaMap; // 0x218

    int AddObjectToLookupTable(const CExoString& sTag, unsigned long oid); // 0x004C7DE0
};

// CSWArea at 0x0, CGameObject sub-object at 0x11c (only data placed here).
class CSWSArea {
public:
    class CSWSRoom* GetRoom(Vector* pPosition, int* pRoomIndex); // 0x004BB600
    int GetSurfaceMaterial(Vector vPosition);           // 0x005063C0
    void SaveMaps(CResGFF* pRes, CResStruct* pStruct);  // 0x005061D0
    int LoadMaps(CResGFF* pRes, CResStruct* pStruct);   // 0x00505DA0
    int LoadPlaceableCameras(CResGFF* pRes, CResStruct* pStruct);   // 0x00505EB0
    void SaveProperties(CResGFF* pRes, CResStruct* pStruct);        // 0x00506090
    void SavePlaceableCameras(CResGFF* pRes, CResStruct* pStruct);  // 0x005062A0
    int PositionWalkable(Vector vPosition);                         // 0x00506400
    void AllocateRoomArrays(int nRooms);                            // 0x00506910 (placeholder name)
    int GetFirstObjectIndiceByX(int* pIndex, float fX);             // 0x00506F20
    int UpdatePositionInObjectsArray(CGameObject* pObject);         // 0x00506FE0
    int LoadRoomInfo(CResStruct* pStruct);                          // 0x005073D0
    int LoadArea(int bLoadObjectState);                             // 0x0050E190
    int AIUpdate();                                                 // 0x00508A30
    void SetCurrentWeather(unsigned char nWeather, int bStarted);   // 0x00508050
    int LoadGIT(int bLoadObjectState);                              // 0x0050DD80
    int LoadAreaHeader(CResStruct* pStruct);                        // 0x00508C50
    int LoadPathPoints(CResStruct* pStruct);                        // 0x00508400
    int LoadTriggers(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates);  // 0x0050A350
    int LoadWaypoints(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates); // 0x00505360
    int LoadSounds(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates);    // 0x00505560
    float ComputeHeight(const Vector& vPosition, int bArg2, CSWSRoom** ppRoom); // 0x004BC380
    int ComputeSafeLocation(const Vector& vPosition, float fRange,
                            class CPathfindInformation* pPathfindInfo, int bArg4,
                            Vector* pSafe);                 // 0x004BE860
    int LoadItems(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates);     // 0x00504DE0
    int LoadEncounters(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates); // 0x00505060
    int LoadStores(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates);     // 0x005057A0
    int LoadAreaEffects(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates); // 0x00505AF0
    int LoadDoors(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates);     // 0x0050A0E0
    int LoadPlaceables(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates); // 0x0050A7B0
    int LoadCreatures(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates); // 0x00504A70
    static CSWSRoom* LoadRooms(CLYT* pLayout);                      // 0x00504870
    int NoNonWalkPolysDetailed(const Vector& vStart, const Vector& vEnd, float fRadius,
                               float fHeight, float fZ, unsigned long oidIgnore,
                               unsigned long& nHit, int bArg8, CExoArrayList<CGameObject*>* pList,
                               int bArg10, class CWalkHitInfo* pHitInfo, int bArg12); // 0x00506650
    int NoNonWalkPolys(const Vector& vStart, const Vector& vEnd, float fRadius, float fHeight,
                       float fZ, unsigned long oidIgnore, class CWalkHitInfo* pHitInfo,
                       int bArg8);                                  // 0x005086D0
    CSWCArea* GetClientArea();                                  // 0x00506A10 (placeholder name)
    void AttachmentFromClientArea(CSWCArea* pArea);             // 0x00506A60
    void DetachFromClientArea();                                // 0x00506A70
    void SetStealthXPMax(unsigned long nMax);                   // 0x00506A80 (placeholder name)
    void SetStealthXPCurrent(unsigned long nCurrent);           // 0x00506AA0 (placeholder name)
    unsigned char GetNextTransPendingID();                      // 0x00506AC0 (placeholder name)
    void SetSunFogColor(const Vector& vColor);                  // 0x00506AE0 (placeholder name)
    void SetTransitionPending(int bPending, unsigned char nID); // 0x00506B30
    void SetPlayerRestrictMode(int nMode);                      // 0x00506B90
    int GetFirstObjectIdInArea(unsigned long* pOid);            // 0x005071B0 (placeholder name)
    int GetNextObjectIdInArea(unsigned long* pOid);             // 0x005071E0 (placeholder name)
    void ActivateAI();                                          // 0x00507220 (placeholder name)
    void DeactivateAI();                                        // 0x00507340 (placeholder name)
    int LoadProperties(CResGFF* pRes, CResStruct* pStruct);     // 0x00507490
    int GetUnescapable() { return m_bUnescapable; }
    int GetPlayerRestrictMode() { return m_nRestrictMode; }
    int GetStealthXPEnabled() { return m_bStealthXPEnabled; }
    void SaveCreatures(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);   // 0x00507680
    void SaveItems(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);       // 0x00507750
    void SaveDoors(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);       // 0x00507810
    void SaveTriggers(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);    // 0x005078D0
    void SaveEncounters(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);  // 0x00507990
    void SaveWaypoints(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);   // 0x00507A50
    void SaveSounds(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);      // 0x00507B10
    void SavePlaceables(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);  // 0x00507BD0
    void SaveStores(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList);      // 0x00507CA0
    void SaveAreaEffects(CResGFF* pRes, CResStruct* pStruct, CExoArrayList<unsigned long>* pList); // 0x00507D60
    void DeleteAllObjects();                                    // 0x00508710 (placeholder name)
    void AwardStealthXP(unsigned long oidCreature);             // 0x00508770 (placeholder name)
    unsigned char GetNumMinesSetByPlayerParty();                // 0x00508960
    int GetPlayerCanSetMines();                                 // 0x005089D0
    int RemoveObjectFromArea(unsigned long oidObject);          // 0x0050DC40
    int AddObjectToArea(unsigned long oidObject, int bRunScripts); // 0x0050DFD0
    void IncrementPlayersInArea();                              // 0x00508C20
    void DecrementPlayersInArea();                              // 0x00508C40

    char pad_0x0[0x4];
    unsigned long m_nAreaFlags;                     // 0x04 (placeholder name)
    CSWSAreaValue8 m_cValue8;                       // 0x08 (placeholder name)
    char pad_0x10[0x8c - 0x10];
    unsigned long m_nSunFogColor;                   // 0x8c
    char pad_0x90[0xa8 - 0x90];
    unsigned char m_nChanceRain;                    // 0xa8
    unsigned char m_nChanceSnow;                    // 0xa9
    char pad_0xaa[0x108 - 0xaa];
    CResGFF* m_pGIT;                                // 0x108 (placeholder name)
    CResRef m_cResRef;                              // 0x10c
    char pad_0x11c[0x120 - 0x11c];
    unsigned long m_idSelf;                         // 0x120 (CGameObject at 0x11c)
    char pad_0x124[0x128 - 0x124];
    int m_nPlayersInArea;                           // 0x128
    CExoArrayList<unsigned long> m_aTriggers;       // 0x12c (placeholder name)
    char pad_0x138[0x13c - 0x138];
    unsigned long m_nLastHeartbeatDay;              // 0x13c
    unsigned long m_nLastHeartbeatTime;             // 0x140
    char pad_0x144[0x158 - 0x144];
    CExoString m_sTag;                              // 0x158
    char pad_0x160[0x164 - 0x160];
    CExoString m_sScriptOnHeartbeat;                // 0x164
    char pad_0x16c[0x190 - 0x16c];
    CExoArrayList<unsigned long> m_aGameObjects;    // 0x190
    int m_nObjectIterator;                          // 0x19c (placeholder name)
    char pad_0x1a0[0x1e4 - 0x1a0];
    CSWSScriptVarTable m_cScriptVarTable;           // 0x1e4
    CSWVarTable m_cVarTable;                        // 0x1f4
    char pad_0x204[0x208 - 0x204];
    CSWSAmbientSound* m_pAmbientSound;              // 0x208 (placeholder name)
    unsigned char m_nCurrentWeather;                // 0x20c
    char pad_0x20d[0x210 - 0x20d];
    int m_bWeatherStarted;                          // 0x210
    unsigned long m_nWeatherTimer;                  // 0x214
    unsigned long m_nLastWeatherDay;                // 0x218
    unsigned long m_nLastWeatherTime;               // 0x21c
    unsigned char m_nWeatherType;                   // 0x220 (placeholder name)
    char pad_0x221[0x223 - 0x221];
    unsigned char m_nMapGrid223;                    // 0x223 (placeholder name)
    char pad_0x224[0x22c - 0x224];
    int m_nRooms;                                   // 0x22c
    CSWSRoom* m_pRooms;                             // 0x230
    char pad_0x234[0x238 - 0x234];
    unsigned long m_nPathPoints;                    // 0x238
    CSWSPathPoint* m_pPathPoints;                   // 0x23c
    unsigned long m_nPathConnections;               // 0x240
    unsigned long* m_pPathConnections;              // 0x244
    char pad_0x248[0x25c - 0x248];
    CExoString* m_pRoomNames;                       // 0x25c (placeholder name)
    unsigned long* m_pRoomData260;                  // 0x260 (placeholder name)
    unsigned long* m_pRoomData264;                  // 0x264 (placeholder name)
    char pad_0x268[0x2ac - 0x268];
    int m_bUnescapable;                             // 0x2ac
    int m_nRestrictMode;                            // 0x2b0
    unsigned long m_nStealthXPMax;                  // 0x2b4
    unsigned long m_nStealthXPCurrent;              // 0x2b8
    unsigned long m_nStealthXPLoss;                 // 0x2bc
    int m_bStealthXPEnabled;                        // 0x2c0
    int m_bTransPending;                            // 0x2c4
    unsigned char m_nTransPendNextID;               // 0x2c8
    unsigned char m_nTransPendCurrID;               // 0x2c9
    char pad_0x2ca[0x2d0 - 0x2ca];
    CSWCArea* m_pClientArea;                        // 0x2d0
};

#endif
