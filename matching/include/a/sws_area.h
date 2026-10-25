// CSWSArea leaves, object-list saves and stealth XP
// (0x00506A10-0x00508C4F). Self-contained declarations; only the members
// these functions use are placed.
#ifndef A_SWS_AREA_H
#define A_SWS_AREA_H

#include <stddef.h>

struct Vector {
    float x, y, z;
};

template <class T> class CExoArrayList {
public:
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

class CResGFF {
public:
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel); // 0x00411A10
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
    void SaveObjectState(CResGFF* pRes, CResStruct* pStruct);   // 0x004CEC50

    char pad_0xc[0x78 - 0xc];
    int m_nAILevel;                                 // 0x78 (placeholder name)
};

class CSWSCreature : public CSWSObject {
public:
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);   // 0x00500610 (placeholder name)

    char pad_0x7c[0x398 - 0x7c];
    int m_n398;                                     // 0x398 (placeholder name)
    char pad_0x39c[0xa88 - 0x39c];
    int m_bPartyMember;                             // 0xa88 (placeholder name)
};

class CSWItem {
public:
    char pad_0x0[0x10];
};

class CSWSItem : public CSWItem, public CSWSObject {
public:
    void SaveItem(CResGFF* pRes, CResStruct* pStruct);      // 0x0055CCD0
};

class CSWSDoor : public CSWSObject {
public:
    int SaveDoor(CResGFF* pRes, CResStruct* pStruct);       // 0x00588AD0
};

class CSWSTrigger : public CSWSObject {
public:
    int SaveTrigger(CResGFF* pRes, CResStruct* pStruct);    // 0x0058E660

    char pad_0x7c[0x2bc - 0x7c];
    int m_bIsTrap;                                  // 0x2bc (placeholder name)
    char pad_0x2c0[0x314 - 0x2c0];
    int m_bCreatedByPlayer;                         // 0x314 (placeholder name)
};

class CSWSPlaceable : public CSWSObject {
public:
    int SavePlaceable(CResGFF* pRes, CResStruct* pStruct);  // 0x00586A70

    char pad_0x7c[0x44c - 0x7c];
    int m_bPartyOwned;                              // 0x44c (placeholder name)
};

class CSWSAreaOfEffectObject : public CSWSObject {
public:
    int SaveEffect(CResGFF* pRes, CResStruct* pStruct);     // 0x00594D80 (placeholder name)
};

class CSWSWaypoint : public CSWSObject {
public:
    int SaveWaypoint(CResGFF* pRes, CResStruct* pStruct);   // 0x005C8230
};

class CSWSEncounter : public CSWSObject {
public:
    int SaveEncounter(CResGFF* pRes, CResStruct* pStruct);  // 0x00591350
};

class CSWSStore : public CSWSObject {
public:
    int SaveStore(CResGFF* pRes, CResStruct* pStruct);      // 0x005C6CD0
};

class CSWSSoundObject : public CSWSObject {
public:
    int Save(CResGFF* pRes, CResStruct* pStruct);           // 0x005C86D0
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
};

class CSWPartyTable {
public:
    void UnstealthParty(int bValue);                    // 0x00563C60
    void DistributeExperience(unsigned long nXP, int bShow);    // 0x005653A0
};

class CServerExoApp {
public:
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid); // 0x004AE770
    CSWSTrigger* GetTriggerByGameObjectID(unsigned long oid);   // 0x004AE790
    CGameObjectArray* GetObjectArray();                         // 0x004AED70
    CServerAIMaster* GetServerAIMaster();                       // 0x004AED80
    CSWPartyTable* GetPartyTable();                             // 0x004AEE70
    CSWSModule* GetModule();                                     // 0x004AE6B0
};

class CSWCArea {
public:
    void SetServerArea(class CSWSArea* pArea);          // 0x00605690 (placeholder name)
};

class CGuiInGame {
public:
    void ShowStealthXP(int nType, unsigned long nXP);   // 0x0062EEB0 (placeholder name)
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
};

class CSWSAreaMap {
public:
    unsigned long* GetExploredData(unsigned long* pWords, int* pGridWidth,
                                   int* pGridHeight); // 0x00579090
    unsigned long* m_pBits;
    unsigned long m_nWords;
};

class CSWSModule {
public:
    char pad_0x0[0x218];
    CSWSAreaMap* m_pAreaMap; // 0x218
};

// CSWArea at 0x0, CGameObject sub-object at 0x11c (only data placed here).
class CSWSArea {
public:
    class CSWSRoom* GetRoom(Vector* pPosition, int* pRoomIndex); // 0x004BB600
    int GetSurfaceMaterial(Vector vPosition);           // 0x005063C0
    void SaveMaps(CResGFF* pRes, CResStruct* pStruct);  // 0x005061D0
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
    int LoadAreaProperties(CResGFF* pRes, CResStruct* pStruct); // 0x00507490 (placeholder name)
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
    void IncrementPlayersInArea();                              // 0x00508C20
    void DecrementPlayersInArea();                              // 0x00508C40

    char pad_0x0[0x8c];
    unsigned long m_nSunFogColor;                   // 0x8c
    char pad_0x90[0x108 - 0x90];
    CResGFF* m_pGIT;                                // 0x108 (placeholder name)
    char pad_0x10c[0x120 - 0x10c];
    unsigned long m_idSelf;                         // 0x120 (CGameObject at 0x11c)
    char pad_0x124[0x128 - 0x124];
    int m_nPlayersInArea;                           // 0x128
    CExoArrayList<unsigned long> m_aTriggers;       // 0x12c (placeholder name)
    char pad_0x138[0x190 - 0x138];
    CExoArrayList<unsigned long> m_aGameObjects;    // 0x190
    int m_nObjectIterator;                          // 0x19c (placeholder name)
    char pad_0x1a0[0x208 - 0x1a0];
    CSWSAmbientSound* m_pAmbientSound;              // 0x208 (placeholder name)
    char pad_0x20c[0x2ac - 0x20c];
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
