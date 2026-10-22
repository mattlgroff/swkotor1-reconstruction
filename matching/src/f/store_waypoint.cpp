// CSWSStore and CSWSWaypoint lifetime / area glue
// (b005c_f2_journal_store, 0x005C6AB0-0x005C8630).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    void *m_pList;
    void *m_pPad;
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class CResStruct;

class CResGFF {
public:
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, const char *szField);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, const char *szField);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, const char *szField);
    int WriteFieldCExoLocString(CResStruct *pStruct, const CExoLocString &sValue,
                                const char *szField);
};

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CSWSItem;

class CItemRepository {
public:
    CItemRepository(unsigned long oidParent, int bFlag);
    ~CItemRepository();
    int GetItemInRepository(CSWSItem *pItem, int bRecursive);
    int RemoveItem(CSWSItem *pItem);
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    int AddItem(CSWSItem **ppItem, int a, int b, int c);
    int InsertItem(CSWSItem *pItem, int nIndex);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
    char pad_0x14[0x4];
};

class CSWSItem : public CGameObject {
public:
    unsigned long GetCost();
    char pad_0xc[0x8];
    unsigned long m_n14;             // +0x14
};

class CServerAIMaster {
public:
    void AddObject(CGameObject *pObject, int bFlag);
    void RemoveObject(CGameObject *pObject);
};

class CSWSModule {
public:
    void RemoveObjectFromLookupTable(CExoString *pTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CServerAIMaster *GetServerAIMaster();
    CSWSItem *GetItemByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern const unsigned char g_nObjectTypeStore;     // 0x0074663D
extern const unsigned char g_nObjectTypeWaypoint;  // 0x0074663B
extern const unsigned long g_oidInvalid;

class CSWSArea {
public:
    void RemoveObjectFromArea(unsigned long oid);
    void AddObjectToArea(unsigned long oid, int bFlag);
};

class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nType, unsigned long oid, int bFlag);
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);
    Vector GetPosition() { return m_vPosition; }
    Vector GetOrientation() { return m_vOrientation; }

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x2c];
    unsigned long m_nD4;             // +0xd4
    char pad_0xd8[0x150];
};

class CSWSStore : public CSWSObject {
public:
    CSWSStore(unsigned long oid);
    virtual ~CSWSStore();
    int RemoveItem(CSWSItem *pItem);
    int GetItemInInventory(unsigned long oid);
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bRunScripts);
    int AddItemToInventory(CSWSItem **ppItem);

    CExoString m_sScripts[1];        // +0x228
    CItemRepository *m_pItemRepository; // +0x230
    CExoLocString m_sLocName;        // +0x234
    char pad_0x23c[0x4];
    unsigned long m_oidLastOpener;   // +0x240
    char pad_0x244[0x8];
    int m_n24C;                      // +0x24c
    int m_n250;                      // +0x250
    unsigned short m_n254;           // +0x254
    unsigned char m_n256;            // +0x256
};

// FUNCTION: SWKOTOR 0x005C6AB0
CSWSStore::CSWSStore(unsigned long oid)
    : CSWSObject(g_nObjectTypeStore, oid, 0)
{
    m_n254 = 0;
    m_pItemRepository = new CItemRepository(m_idSelf, 0);
    m_n24C = 0;
    m_n250 = 0;
    m_n256 = 3;
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddObject(this, 0);
}

// FUNCTION: SWKOTOR 0x005C6BF0
CSWSStore::~CSWSStore()
{
    if (m_pItemRepository)
        delete m_pItemRepository;
    m_oidLastOpener = 0x7f000000;
    GetArea();
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
}

// FUNCTION: SWKOTOR 0x005C6BA0
int CSWSStore::RemoveItem(CSWSItem *pItem)
{
    return m_pItemRepository->RemoveItem(pItem);
}

// FUNCTION: SWKOTOR 0x005C6BB0
int CSWSStore::GetItemInInventory(unsigned long oid)
{
    CSWSItem *pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oid);
    if (pItem && m_pItemRepository->GetItemInRepository(pItem, 0))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x005C6E80
void CSWSStore::AddToArea(CSWSArea *pArea, float x, float y, float z, int bRunScripts)
{
    if (pArea) {
        SetArea(pArea);
        SetPosition(Vector(x, y, z), 0, 1, 0);
        GetArea()->AddObjectToArea(m_idSelf, bRunScripts);
    }
}

// NONMATCH (14): block layout only; the original lays the AddItem path out first
// and pushes edi only on the insertion-loop path (if/else, do-while, for and
// flipped-condition variants all put the loop first here).
// FUNCTION: SWKOTOR 0x005C70C0
int CSWSStore::AddItemToInventory(CSWSItem **ppItem)
{
    unsigned long nCost = (*ppItem)->GetCost();
    CSWSItem *pItem = m_pItemRepository->ItemListGetItem(0);
    if (pItem == 0 || pItem->GetCost() <= nCost) {
        m_pItemRepository->AddItem(ppItem, 1, 0, 1);
        return 1;
    }
    for (int nIndex = m_pItemRepository->m_nItemCount - 1; ; nIndex--) {
        pItem = m_pItemRepository->ItemListGetItem(nIndex);
        if (pItem == 0)
            continue;
        if (pItem->m_n14 == (*ppItem)->m_n14)
            return 0;
        if (pItem->GetCost() > nCost) {
            m_pItemRepository->InsertItem(*ppItem, nIndex);
            return 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x005C7160 SYMBOL: ??_GCSWSStore@@UAEPAXI@Z

class CSWSWaypoint : public CSWSObject {
public:
    CSWSWaypoint(unsigned long oid);
    virtual ~CSWSWaypoint();
    int SaveWaypoint(CResGFF *pRes, CResStruct *pStruct);
    void RemoveFromArea();
    void AddToArea(CSWSArea *pArea, float x, float y, float z, int bRunScripts);

    int m_bHasMapNote;               // +0x228
    int m_bMapNoteEnabled;           // +0x22c
    CExoLocString m_sMapNote;        // +0x230
    CExoLocString m_sLocalizedName;  // +0x238
};

// FUNCTION: SWKOTOR 0x005C7E70
CSWSWaypoint::CSWSWaypoint(unsigned long oid)
    : CSWSObject(g_nObjectTypeWaypoint, oid, 0)
{
    m_nD4 = 0x2711;
    m_bHasMapNote = 0;
    m_bMapNoteEnabled = 0;
}

// FUNCTION: SWKOTOR 0x005C8230
int CSWSWaypoint::SaveWaypoint(CResGFF *pRes, CResStruct *pStruct)
{
    pRes->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pRes->WriteFieldCExoLocString(pStruct, m_sLocalizedName, "LocalizedName");
    Vector vPosition = GetPosition();
    pRes->WriteFieldFLOAT(pStruct, vPosition.x, "XPosition");
    pRes->WriteFieldFLOAT(pStruct, vPosition.y, "YPosition");
    pRes->WriteFieldFLOAT(pStruct, vPosition.z, "ZPosition");
    Vector vOrientation = GetOrientation();
    pRes->WriteFieldFLOAT(pStruct, vOrientation.x, "XOrientation");
    pRes->WriteFieldFLOAT(pStruct, vOrientation.y, "YOrientation");
    pRes->WriteFieldFLOAT(pStruct, vOrientation.z, "ZOrientation");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bHasMapNote, "HasMapNote");
    if (m_bHasMapNote) {
        pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bMapNoteEnabled, "MapNoteEnabled");
        pRes->WriteFieldCExoLocString(pStruct, m_sMapNote, "MapNote");
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005C8350
void CSWSWaypoint::RemoveFromArea()
{
    if (GetArea())
        GetArea()->RemoveObjectFromArea(m_idSelf);
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x005C84E0
CSWSWaypoint::~CSWSWaypoint()
{
    if (GetArea())
        RemoveFromArea();
    CSWSModule *pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
}

// FUNCTION: SWKOTOR 0x005C8580
void CSWSWaypoint::AddToArea(CSWSArea *pArea, float x, float y, float z, int bRunScripts)
{
    RemoveFromArea();
    if (pArea) {
        CExoString sUnused;
        SetArea(pArea);
        SetPosition(Vector(x, y, z), 0, 1, 0);
        GetArea()->AddObjectToArea(m_idSelf, bRunScripts);
    }
}

// FUNCTION: SWKOTOR 0x005C8630 SYMBOL: ??_GCSWSWaypoint@@UAEPAXI@Z
