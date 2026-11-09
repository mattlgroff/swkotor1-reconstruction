// CSWSModule constructor / destructor (nwsmodule.o, 0x004C84A0 / 0x004C68A0)
// with the real two-base layout: CResHelper<CResIFO, 2014> at +0x00 and
// CGameObject at +0x1C. The other module files use a flat partial copy of
// the class (i/sws_module.h); this file is self-contained so that the base
// and member constructors are visible here.
#include <stddef.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    ~CExoLocString();                               // 0x005EA000
    void* m_pInternal;
    unsigned long m_n4;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* szResRef);                  // 0x00406D80
    CResRef operator=(const char* szResRef);        // 0x00406290
    char m_resRef[16];
};

class CRes {
public:
    virtual ~CRes();
    int Release();                                  // 0x00409B80
};

class CResIFO : public CRes {
public:
    virtual ~CResIFO();
};

template <class T, int nType>
class CResHelper {
public:
    CResHelper()
    {
        m_pRes = NULL;
        m_cResRef = "";
        m_bAutoRequest = 0;
    }
    virtual ~CResHelper();                          // 0x004C4C60
    void SetResRef(const CResRef& cResRef, int bAutoRequest);  // 0x004C4CC0
    int m_bAutoRequest;                             // +0x04
    T* m_pRes;                                      // +0x08
    CResRef m_cResRef;                              // +0x0c
};

class CSWSModule;

class CGameObject {
public:
    CGameObject(unsigned char nType, unsigned long oid);    // 0x004D7D60
    virtual ~CGameObject() {}                       // 0x004C3030
    virtual void SetId(unsigned long oid);          // +0x04 (placeholder name)
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime);    // +0x08 (placeholder name)
    virtual void* As0c();
    virtual void* As10();
    virtual void* As14();
    virtual void* As18();
    virtual void* As1c();
    virtual CSWSModule* AsSWSModule();              // +0x20
    virtual void* As24();
    virtual void* As28();
    virtual void* As2c();
    virtual void* AsSWSCreature();                  // +0x30
    virtual void* As34();
    virtual void* As38();
    virtual void* As3c();
    virtual void* As40();
    virtual void* As44();
    virtual void* As48();
    virtual void* As4c();
    virtual void* As50();
    virtual void* As54();
    virtual void* As58();
    virtual void* As5c();
    virtual void* As60();
    virtual void* As64();
    virtual void* As68();
    virtual void* As6c();

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80
    void* m_pHead;
    void* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();                               // 0x004C4E20
    ~CExoLinkedList() { delete m_pList; }
    int IsEmpty() { return m_pList->m_pHead == NULL; }
    T* RemoveHead() { return (T*)m_pList->RemoveHead(); }
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = NULL;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = NULL;
    }
    T* element;
    int num;
    int array_size;
};

struct CSWSModuleExpansion {
    CExoLocString m_sName;                          // +0x00
    int m_nID;                                      // +0x08
};

struct CSWSModuleCutScene {
    char m_cName[16];                               // +0x00 (CResRef)
    unsigned long m_nID;                            // +0x10
};

struct CSWSModulePlayerEntry {
    CExoString m_sPlayerName;                       // +0x00
    CExoLocString m_sFirstName;                     // +0x08
    CExoLocString m_sLastName;                      // +0x10
    int m_bPrimary;                                 // +0x18
};

struct CSWSModuleID {
    char m_aID[0x20];                               // +0x00
    int m_nCreatorID;                               // +0x20
    unsigned long m_nVersion;                       // +0x24
};

struct CSWSModuleEntryInfo {
    CResRef m_cArea;                                // +0x00
    float m_fX;                                     // +0x10
    float m_fY;                                     // +0x14
    float m_fZ;                                     // +0x18
    float m_fDirX;                                  // +0x1c
    float m_fDirY;                                  // +0x20
};

class CSWSScriptVarTable {
public:
    CSWSScriptVarTable()
    {
        m_nC = 0;
    }
    ~CSWSScriptVarTable();                          // 0x0059A980
    CExoArrayList<void*> m_aVars;                   // +0x00 (destroyed by the out-of-line dtor)
    int m_nC;                                       // +0x0c
};

class CSWVarTable {
public:
    CSWVarTable();                                  // 0x0059AFE0
    char m_pad0[0x14];
};

struct CSWSTagNode {
    char m_pad0[0x44];
};

// 0x24 bytes.
class CWorldJournalEntry {
public:
    ~CWorldJournalEntry();                          // 0x004C4030
    CExoString m_sName;                             // +0x00
    CExoString m_sText;                             // +0x08
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned long m_n18;
    unsigned long m_nID;                            // +0x1c
    unsigned long m_bStrref : 1;                    // +0x20
};

class CSWSAreaMap {
public:
    CSWSAreaMap();                                  // 0x00578BE0
    ~CSWSAreaMap();                                 // 0x00578C30
    char m_pad0[0x4c];
};

class CSWSArea {
public:
    virtual ~CSWSArea();
    void DeleteAllObjects();                        // 0x00508710
};

class CGameObjectArray {
public:
    unsigned char AddInternalObject(unsigned long* pOid, CGameObject* pObject, int bCharacter);  // 0x004D80E0
    unsigned char Delete(unsigned long oid);        // 0x004D8290
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);    // 0x004D8230
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
};

class CServerExoApp {
public:
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);     // 0x004AE780
    void ClearLastModuleFound();                            // 0x004AEB90
    CGameObjectArray* GetObjectArray();                     // 0x004AED70
    CWorldTimer* GetWorldTimer();                           // 0x004AEDE0
};

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                          // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned char OBJECT_TYPE_MODULE;      // 0x00746632
extern const unsigned long g_nModuleDefaultA4;      // 0x0074410C

class Vector {
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    float x, y, z;
};

// 0x34 bytes.
class CScriptEvent {
public:
    ~CScriptEvent();                                // 0x004D7590
    int GetInteger(int nIndex);                     // 0x004D6480
    float GetFloat(int nIndex);                     // 0x004D64A0
    unsigned long GetObjectID(int nIndex);          // 0x004D64C0
    unsigned short m_nType;                         // +0x00
    char m_pad2[0x34 - 2];
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psScript, unsigned long oid, int bOid);  // 0x005D0FC0
    int RunScriptSituation(void* pScript, unsigned long oid, int bOid);    // 0x005D0FD0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

extern const unsigned long EVENT_TIMED_EVENT;       // 0x007441B8
extern const unsigned long EVENT_SIGNAL_EVENT;      // 0x007441DC

class CSWSModule : public CResHelper<CResIFO, 2014>, public CGameObject {
public:
    CSWSModule(const CExoString& sModule, int bAutoRequest, int nUnused);   // 0x004C84A0
    virtual ~CSWSModule();                          // 0x004C68A0
    virtual CSWSModule* AsSWSModule() { return this; }  // 0x004C6BB0

    void RemoveModuleResources(const CExoString& sModule);  // 0x004C44D0
    void AddModuleResources(const CExoString& sModule);     // 0x004C4150
    void EventHandler(unsigned long nEventId, unsigned long nCallerObjectId, void* pScript,
                      unsigned long nCalendarDay, unsigned long nTimeOfDay);    // 0x004C5120
    void ClearAreaCache()
    {
        m_pCachedArea = NULL;
        m_oidCachedArea = 0x7f000000;
    }

    CExoLinkedList<CSWSModuleExpansion> m_lstExpansions;    // +0x28
    CExoLinkedList<CSWSModuleCutScene> m_lstCutScenes;      // +0x2c
    CResRef m_cAreaResRef;                          // +0x30 (placeholder name)
    unsigned long m_oidArea;                        // +0x40
    int m_bAIUpdateToggle;                          // +0x44
    CExoLinkedList<CSWSModulePlayerEntry> m_lPlayers;   // +0x48
    CExoLocString m_cDescription;                   // +0x4c
    CSWSModuleID* m_pModuleID;                      // +0x54
    CSWSModuleEntryInfo* m_pEntryInfo;              // +0x58
    CExoString m_sModuleName;                       // +0x5c (placeholder name)
    CExoLocString m_cName;                          // +0x64 (placeholder name)
    CResRef m_cResRef6c;                            // +0x6c (placeholder name)
    CResRef m_cResRef7c;                            // +0x7c (placeholder name)
    CSWSScriptVarTable m_cScriptVarTable;           // +0x8c
    CSWVarTable m_cVarTable;                        // +0x9c
    CExoString m_sScripts[15];                      // +0xb0
    unsigned long m_nLastHeartbeatDay;              // +0x128
    unsigned long m_nLastHeartbeatTime;             // +0x12c
    CExoArrayList<CSWSTagNode> m_aTagNodes;         // +0x130
    CExoArrayList<unsigned long> m_a13c;            // +0x13c (placeholder name)
    int m_n148;                                     // +0x148
    unsigned long m_oid14c;                         // +0x14c
    unsigned long m_oid150;                         // +0x150
    unsigned long m_oid154;                         // +0x154
    unsigned long m_oid158;                         // +0x158
    unsigned long m_oid15c;                         // +0x15c
    unsigned long m_oid160;                         // +0x160
    unsigned long m_oid164;                         // +0x164
    unsigned char m_n168;                           // +0x168
    char m_pad169[3];
    unsigned long m_oid16c;                         // +0x16c
    unsigned long m_oid170;                         // +0x170
    unsigned long m_oid174;                         // +0x174
    unsigned long m_oid178;                         // +0x178
    unsigned long m_oid17c;                         // +0x17c
    unsigned long m_oid180;                         // +0x180
    unsigned long m_oid184;                         // +0x184
    Vector m_v188;                                  // +0x188 (placeholder name)
    unsigned long m_oid194;                         // +0x194
    unsigned long m_oid198;                         // +0x198
    unsigned long m_oid19c;                         // +0x19c
    unsigned char m_n1a0;                           // +0x1a0 (placeholder name)
    unsigned char m_nDawnHour;                      // +0x1a1
    unsigned char m_nDuskHour;                      // +0x1a2
    unsigned char m_n1a3;                           // +0x1a3
    unsigned char m_n1a4;                           // +0x1a4
    unsigned char m_n1a5;                           // +0x1a5
    unsigned char m_nXPScale;                       // +0x1a6
    char m_pad1a7;
    unsigned long m_nYear;                          // +0x1a8
    unsigned long m_nMonth;                         // +0x1ac
    unsigned long m_nDay;                           // +0x1b0
    unsigned long m_nHour;                          // +0x1b4
    unsigned long m_nTransitionTime;                // +0x1b8
    unsigned char m_nPhase;                         // +0x1bc
    char m_pad1bd[3];
    unsigned long m_nLastUpdateDay;                 // +0x1c0
    unsigned long m_nLastUpdateTime;                // +0x1c4
    int m_bIsSaveGame;                              // +0x1c8 (placeholder name)
    CExoArrayList<CWorldJournalEntry> m_aWorldJournal;  // +0x1cc
    int m_bLoaded;                                  // +0x1d8 (placeholder name)
    CExoArrayList<unsigned long> m_aLimbo;          // +0x1dc
    void* m_pTempAreaData;                          // +0x1e8 (placeholder name)
    void* m_pSaveIFOStruct;                         // +0x1ec
    void* m_pSaveIFO;                               // +0x1f0
    unsigned long m_nTempAreaSize;                  // +0x1f4 (placeholder name)
    CExoString m_sLowerCaseName;                    // +0x1f8 (placeholder name)
    int m_bIsNWM;                                   // +0x200 (placeholder name)
    CExoString m_sNWMResName;                       // +0x204
    unsigned long m_nSaveEntries;                   // +0x20c (placeholder name)
    unsigned char m_n210;                           // +0x210
    char m_pad211[3];
    int m_n214;                                     // +0x214
    CSWSAreaMap* m_pAreaMap;                        // +0x218
    unsigned long m_oidCachedArea;                  // +0x21c
    CSWSArea* m_pCachedArea;                        // +0x220
};

// FUNCTION: SWKOTOR 0x004C84A0
CSWSModule::CSWSModule(const CExoString& sModule, int bAutoRequest, int nUnused)
    : CGameObject(OBJECT_TYPE_MODULE, 0x7f000000)
{
    CExoString sName;
    m_pModuleID = new CSWSModuleID;
    m_pEntryInfo = new CSWSModuleEntryInfo;
    m_pAreaMap = new CSWSAreaMap;
    m_nLastHeartbeatDay = 0;
    m_nLastHeartbeatTime = 0;
    sName = CExoString("CURRENTGAME:") + sModule;
    m_sModuleName = sName;
    m_bAIUpdateToggle = 1;
    m_n148 = 0;
    m_oid14c = OBJECT_INVALID;
    m_oid150 = OBJECT_INVALID;
    m_oid154 = OBJECT_INVALID;
    m_oid158 = OBJECT_INVALID;
    m_oid16c = OBJECT_INVALID;
    m_oid15c = OBJECT_INVALID;
    m_oid160 = OBJECT_INVALID;
    m_oid164 = 0x7f000000;
    m_n168 = 0;
    m_oid178 = 0x7f000000;
    m_oid17c = 0x7f000000;
    m_oid180 = 0x7f000000;
    m_oid184 = 0x7f000000;
    m_oid194 = 0x7f000000;
    m_oid198 = 0x7f000000;
    m_oid19c = 0x7f000000;
    m_oid170 = OBJECT_INVALID;
    m_oid174 = OBJECT_INVALID;
    m_n214 = 0;
    m_bIsNWM = 0;
    m_sNWMResName = "";
    m_bIsSaveGame = 0;
    AddModuleResources(m_sModuleName);
    m_n210 = 2;
    m_n1a0 = 5;
    m_nDawnHour = 0;
    m_nDuskHour = 0;
    m_n1a3 = 1;
    m_n1a4 = 1;
    m_n1a5 = 0;
    m_nXPScale = 10;
    m_nYear = g_nModuleDefaultA4;
    m_nMonth = 1;
    m_nDay = 1;
    m_nHour = 0;
    m_nTransitionTime = 0;
    m_nPhase = 0;
    m_bLoaded = 1;
    m_pTempAreaData = NULL;
    m_pSaveIFOStruct = NULL;
    m_pSaveIFO = NULL;
    m_nTempAreaSize = 0;
    {
        CResRef cResRef("");
        SetResRef(cResRef, 0);
    }
    {
        CResRef cResRef("MODULE");
        SetResRef(cResRef, bAutoRequest);
    }
    GetServerExoApp()->GetObjectArray()->AddInternalObject(&m_idSelf, this, 0);
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&m_nLastUpdateDay, &m_nLastUpdateTime);
    m_sLowerCaseName = "";
    m_nSaveEntries = 0xffffffff;
    ClearAreaCache();
}

// FUNCTION: SWKOTOR 0x004C68A0
CSWSModule::~CSWSModule()
{
    if (!m_bLoaded) {
        m_pRes->Release();
    }
    while (!m_lPlayers.IsEmpty()) {
        delete m_lPlayers.RemoveHead();
    }
    CSWSArea* pArea = GetServerExoApp()->GetAreaByGameObjectID(m_oidArea);
    if (pArea) {
        pArea->DeleteAllObjects();
        delete pArea;
    }
    while (!m_lstCutScenes.IsEmpty()) {
        delete m_lstCutScenes.RemoveHead();
    }
    while (!m_lstExpansions.IsEmpty()) {
        delete m_lstExpansions.RemoveHead();
    }
    RemoveModuleResources(m_sModuleName);
    m_sModuleName = "";
    if (m_pEntryInfo) {
        delete m_pEntryInfo;
    }
    if (m_pModuleID) {
        delete m_pModuleID;
    }
    if (m_pAreaMap) {
        delete m_pAreaMap;
        m_pAreaMap = NULL;
    }
    GetServerExoApp()->GetObjectArray()->Delete(m_idSelf);
    g_pAppManager->m_pServerExoApp->ClearLastModuleFound();
    ClearAreaCache();
}

// Hidden start (functions.csv merges it into 0x004C5000). Script slots:
// 0 heartbeat, 1 user defined, 2..14 the module events below.
// FUNCTION: SWKOTOR 0x004C5120
void CSWSModule::EventHandler(unsigned long nEventId, unsigned long nCallerObjectId, void* pScript,
                              unsigned long nCalendarDay, unsigned long nTimeOfDay)
{
    if (nEventId == EVENT_TIMED_EVENT) {
        if (pScript) {
            g_pVirtualMachine->RunScriptSituation(pScript, m_idSelf, 1);
        }
    } else if (nEventId == EVENT_SIGNAL_EVENT) {
        CScriptEvent* pEvent = (CScriptEvent*)pScript;
        switch (pEvent->m_nType) {
        case 0:
            m_nLastHeartbeatDay = nCalendarDay;
            m_nLastHeartbeatTime = nTimeOfDay;
            g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
            break;
        case 11:
            m_n148 = pEvent->GetInteger(0);
            g_pVirtualMachine->RunScript(&m_sScripts[1], m_idSelf, 1);
            break;
        case 14:
            m_oid14c = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScripts[4], m_idSelf, 1);
            break;
        case 15:
            m_oid150 = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScripts[5], m_idSelf, 1);
            break;
        case 16:
            g_pVirtualMachine->RunScript(&m_sScripts[3], m_idSelf, 1);
            break;
        case 17:
            g_pVirtualMachine->RunScript(&m_sScripts[2], m_idSelf, 1);
            break;
        case 19:
            m_oid158 = pEvent->GetObjectID(1);
            m_oid154 = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScripts[7], m_idSelf, 1);
            break;
        case 20:
            m_oid15c = pEvent->GetObjectID(0);
            m_oid160 = nCallerObjectId;
            g_pVirtualMachine->RunScript(&m_sScripts[8], m_idSelf, 1);
            break;
        case 18:
            m_oid178 = pEvent->GetObjectID(0);
            m_oid17c = pEvent->GetObjectID(1);
            m_oid180 = pEvent->GetObjectID(2);
            m_oid184 = pEvent->GetObjectID(3);
            m_v188 = Vector(pEvent->GetFloat(0), pEvent->GetFloat(1), pEvent->GetFloat(2));
            g_pVirtualMachine->RunScript(&m_sScripts[6], m_idSelf, 1);
            break;
        case 10:
            m_oid16c = nCallerObjectId;
            g_pVirtualMachine->RunScript(&m_sScripts[9], m_idSelf, 1);
            break;
        case 32:
            m_oid170 = nCallerObjectId;
            g_pVirtualMachine->RunScript(&m_sScripts[10], m_idSelf, 1);
            break;
        case 33:
            m_oid174 = nCallerObjectId;
            g_pVirtualMachine->RunScript(&m_sScripts[11], m_idSelf, 1);
            break;
        case 35:
            m_oid164 = nCallerObjectId;
            m_n168 = pEvent->GetInteger(0);
            g_pVirtualMachine->RunScript(&m_sScripts[12], m_idSelf, 1);
            break;
        case 37:
            m_oid194 = nCallerObjectId;
            g_pVirtualMachine->RunScript(&m_sScripts[13], m_idSelf, 1);
            break;
        case 36: {
            CGameObject* pObject = NULL;
            GetServerExoApp()->GetObjectArray()->GetGameObject(nCallerObjectId, &pObject);
            if (pObject && pObject->AsSWSCreature()) {
                GetServerExoApp()->GetObjectArray()->Delete(nCallerObjectId);
            }
            break;
        }
        case 38:
            m_oid19c = pEvent->GetObjectID(0);
            g_pVirtualMachine->RunScript(&m_sScripts[14], m_idSelf, 1);
            break;
        }
        delete pEvent;
    }
}

// FUNCTION: SWKOTOR 0x004C6BB0 SYMBOL: ?AsSWSModule@CSWSModule@@UAEPAV1@XZ
// FUNCTION: SWKOTOR 0x004C6BC0 SYMBOL: ??_ECSWSModule@@WBM@AEPAXI@Z
