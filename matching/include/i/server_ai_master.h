// CServerAIMaster and its helpers (0x004AF0F0-0x004B1330).
// Self-contained declarations.
#ifndef I_SERVER_AI_MASTER_H
#define I_SERVER_AI_MASTER_H

#include <stddef.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList : public CResStruct {
    char m_pLabel[16];
};

class CResGFF {
public:
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x004118C0
    unsigned long GetListCount(CResList* pList);                                    // 0x00411940
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned long nDefault); // 0x00411C20
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault);  // 0x00411D00
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pOut, CResList* pList, unsigned long nType);     // 0x004124E0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList() { num = 0; array_size = 0; element = NULL; }
    ~CExoArrayList() { delete[] element; element = NULL; }
    void Add(T t);                                  // <unsigned long>: 0x006449D0
    void DelIndex(int nIndex);                      // <unsigned long>: 0x006857A0
    int count(T t)
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

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                       // 0x005E9AA0
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    CExoLinkedListPosition AddBefore(void* p, CExoLinkedListPosition pos);  // 0x005E9AD0
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
    CExoLinkedListPosition m_pHead;
    CExoLinkedListPosition m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList() { m_pList = new CExoLinkedListInternal; }
    ~CExoLinkedList() { delete m_pList; }
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    unsigned long GetCount() { return m_pList->m_nCount; }
    int IsEmpty() { return m_pList->m_pHead == NULL; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    T* RemoveHead() { return (T*)m_pList->RemoveHead(); }
    CExoLinkedListPosition AddTail(T* p) { return m_pList->AddTail(p); }
    CExoLinkedListPosition AddBefore(T* p, CExoLinkedListPosition pos) { return m_pList->AddBefore(p, pos); }
    CExoLinkedListInternal* m_pList;
};

class CSWSObject {
public:
    char m_pad0[4];
    unsigned long m_idSelf;                         // +0x04
    char m_pad8[0x78 - 8];
    int m_nAILevel;                                 // +0x78
};

class CSWSItem;
class CSWItemProperty;
class CSWSCreature;
class CGameEffect;

class CGameEffectApplierRemover {
public:
    virtual ~CGameEffectApplierRemover();           // 0x004AF2D0
    virtual void InitializeEffects() = 0;
    virtual int OnApplyEffect(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame) = 0;
    virtual int OnRemoveEffect(CSWSObject* pObject, CGameEffect* pEffect) = 0;
};

class CItemPropertyApplierRemover {
public:
    virtual ~CItemPropertyApplierRemover();         // 0x004AF300
    virtual void InitializeItemProperties() = 0;
    virtual int OnApplyItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                    CSWSCreature* pCreature, unsigned long nSlot, int bArg) = 0;
    virtual int OnRemoveItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                     CSWSCreature* pCreature, unsigned long nSlot) = 0;
};

class CSWSEffectListHandler : public CGameEffectApplierRemover {
public:
    virtual ~CSWSEffectListHandler();               // 0x004D82B0
    virtual void InitializeEffects();               // 0x004E4A10
    virtual int OnApplyEffect(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);   // 0x004D8320
    virtual int OnRemoveEffect(CSWSObject* pObject, CGameEffect* pEffect);  // 0x004D8360
    void* m_p4;
    void* m_p8;
};

class CSWSItemPropertyHandler : public CItemPropertyApplierRemover {
public:
    virtual ~CSWSItemPropertyHandler();             // 0x004E53A0
    virtual void InitializeItemProperties();        // 0x004EA9F0
    virtual int OnApplyItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                    CSWSCreature* pCreature, unsigned long nSlot, int bArg);  // 0x004E5410
    virtual int OnRemoveItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                     CSWSCreature* pCreature, unsigned long nSlot);
    void* m_p4;
    void* m_p8;
};

struct CSWSSpellScriptData {
    CSWSSpellScriptData();                          // 0x004AF130
    ~CSWSSpellScriptData();                         // 0x004AF150
    char m_pad0[0x10];
    unsigned long m_n10;                            // +0x10
    unsigned long m_n14;                            // +0x14
    unsigned long m_n18;                            // +0x18
    CExoString m_s1c;                               // +0x1c
};

struct CServerAIBodyBagInfo {
    int SaveBodyBag(CResGFF* pGFF, CResStruct* pStruct);    // 0x004AF160
    int LoadBodyBag(CResGFF* pGFF, CResStruct* pStruct);    // 0x004AF1C0
    unsigned long m_oidBodyBag;                     // +0x00
    float m_fX;                                     // +0x04
    float m_fY;                                     // +0x08
    float m_fZ;                                     // +0x0c
};

class CServerAIList {
public:
    CServerAIList() { m_nPosition = 0; }
    int RemoveObject(unsigned long oid);            // 0x004AF330
    CExoArrayList<unsigned long> m_aoGameObjects;   // +0x00
    int m_nPosition;                                // +0x0c
};

struct CServerAIEventNode {
    int SaveNode(CResGFF* pGFF, CResStruct* pStruct);   // 0x004AFEA0 (unnamed)
    int LoadNode(CResGFF* pGFF, CResStruct* pStruct);   // 0x004B0290 (unnamed)
    unsigned long m_nDay;                           // +0x00
    unsigned long m_nTime;                          // +0x04
    unsigned long m_nCallerObjectId;                // +0x08
    unsigned long m_nObjectId;                      // +0x0c
    unsigned long m_nEventId;                       // +0x10
    void* m_pEventData;                             // +0x14
};

struct CServerAIBlock {
    ~CServerAIBlock() { delete m_pData; m_pData = NULL; }
    void* m_pData;
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    int AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDay, unsigned long* pTime); // 0x004ADF50 (unnamed)
    int CompareWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                          unsigned long nTime2);                    // 0x004ADFA0 (unnamed)
};

class CServerExoAppInternal {
public:
    char m_pad0[0x10048];
    CWorldTimer* GetWorldTimer() { return m_pWorldTimer; }
    CWorldTimer* m_pWorldTimer;                     // +0x10048
};

class CServerExoApp {
public:
    unsigned char GetActivePauseState();            // 0x004AE990
};

class CAppManager {
public:
    void* m_p0;
    void* m_p4;
    CServerExoApp* m_pServerExoApp;                 // +0x08
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CVirtualMachine {
public:
    void DeleteScriptSituation(void* pScript);      // 0x005D1110
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

extern int g_bLogEvents;                            // 0x00831FD8 (unnamed)
void LogEventAdd(unsigned long nCaller, unsigned long nObject, void* pEventData);  // 0x004AF630 (unnamed)

struct CSWSCombatAttackData {
    ~CSWSCombatAttackData();                        // 0x004D4480
};
struct CUnknown004d6a40 {
    ~CUnknown004d6a40();                            // 0x004D6A40
};
struct CScriptEvent {
    ~CScriptEvent();                                // 0x004D7590
};

class CServerAIMaster {
public:
    CServerAIMaster();                              // 0x004B0780
    ~CServerAIMaster();                             // 0x004B1200
    void OnEffectApplied(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004AF0F0
    void OnEffectRemoved(CSWSObject* pObject, CGameEffect* pEffect);    // 0x004AF100
    void OnItemPropertyApplied(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot, int bArg); // 0x004AF110
    void OnItemPropertyRemoved(CSWSItem* pItem, CSWItemProperty* pProperty,
                               CSWSCreature* pCreature, unsigned long nSlot);   // 0x004AF120
    int RemoveObject(CSWSObject* pObject);          // 0x004AF3D0
    int AddEventAbsoluteTime(unsigned long nDay, unsigned long nTime, unsigned long nCaller,
                             unsigned long nObject, unsigned long nEventId, void* pEventData); // 0x004AFDB0
    int AddObject(CSWSObject* pObject, int nLevel); // 0x004B0850
    int SetAILevel(CSWSObject* pObject, int nLevel);    // 0x004B08A0
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                          unsigned long nObject, unsigned long nEventId, void* pEventData); // 0x004B08D0
    int SaveEventQueue(CResGFF* pGFF, CResStruct* pStruct); // 0x004B0970
    int LoadEventQueue(CResGFF* pGFF, CResStruct* pStruct); // 0x004B0A00
    void DeleteEventData(unsigned long nEventId, void* pEventData); // 0x004B0AB0
    void ClearEventQueue();                         // 0x004B11C0

    CServerExoAppInternal* m_pServer;               // +0x00
    CServerAIList m_aGameAIList[5];                 // +0x04
    CExoLinkedList<CServerAIEventNode> m_lEventQueue;   // +0x54
    CSWSEffectListHandler* m_pEffectListHandler;    // +0x58
    CSWSItemPropertyHandler* m_pItemPropertyHandler;    // +0x5c
    CExoString m_s60;                               // +0x60
    char m_pad68[0x80 - 0x68];
    CServerAIBlock* m_pBlock80;                     // +0x80
    CServerAIBlock* m_pBlock84;                     // +0x84
    CServerAIBlock* m_pBlock88;                     // +0x88
    CServerAIBlock* m_pBlock8c;                     // +0x8c
};

#endif
