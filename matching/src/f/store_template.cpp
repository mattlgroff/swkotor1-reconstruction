// CSWSStore::LoadStore / LoadFromTemplate / SaveStore
// (b005c_f2_journal_store): UTM template load and GFF load / save of a store.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    void Format(const char *szFormat, ...);
    char *CStr();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    unsigned long m_nStrRef;
};

class CResRef {
public:
    CResRef(const CExoString &s);
    CResRef(const char *s);
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
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
    CResGFF(unsigned short nType, char *szFileType, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pElement, CResList *pList, unsigned long nType);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
    int WriteFieldCExoLocString(CResStruct *pStruct, CExoLocString &sValue, char *szLabel);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldCResRef(CResStruct *pStruct, const CResRef &cValue, char *szLabel);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);
    CExoLocString ReadFieldCExoLocString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                         const CExoLocString &sDefault);
    CResRef ReadFieldCResRef(CResStruct *pStruct, char *szLabel, int &bSuccess,
                             const CResRef &cDefault);
    int ReadFieldINT(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                 unsigned long nDefault);
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                unsigned char nDefault);
    int GetList(CResList *pList, CResStruct *pStruct, char *szLabel);
    unsigned long GetListCount(CResList *pList);
    int GetListElement(CResStruct *pElement, CResList *pList, unsigned long nIndex);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94 (placeholder name)
    char pad_0x98[0x8];
};

class CSWSItem {
public:
    CSWSItem(unsigned long oid);
    virtual ~CSWSItem();
    int SaveItem(CResGFF *pGFF, CResStruct *pStruct);
    int LoadFromTemplate(const CResRef &cResRef);
    int LoadItem(CResGFF *pGFF, CResStruct *pStruct);
    unsigned long GetCost();
    void SetPossessor(unsigned long oidPossessor, int a, int b, unsigned long oidPrevious);
    char pad_0x4[0x10];
    unsigned long m_nObjectId;       // +0x14 (placeholder name)
    char pad_0x18[0x270];
    unsigned long m_bFlag0 : 1;      // +0x288 bit 0 (placeholder names)
    unsigned long m_bFlag1 : 1;      // +0x288 bit 1
    unsigned long m_bInfinite : 1;   // +0x288 bit 2
    char pad_0x28c[0x10];
};

class CItemRepository {
public:
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int n);
    void Insert(T t, int nIndex);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class CGameObject;

class CSWSModule {
public:
    int AddObjectToLookupTable(const CExoString &sTag, unsigned long oid);
};

class CServerExoApp {
public:
    CGameObject *GetGameObject(unsigned long oid);
    CSWSModule *GetModule();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class Vector {
public:
    float x, y, z;
};

class CSWSStore {
public:
    void SetTag(CExoString &sTag);
    int AddItemToInventory(CSWSItem **ppItem);
    int LoadStore(CResGFF *pGFF, CResStruct *pStruct, int bFromTemplate);
    int LoadFromTemplate(const CResRef &cResRef, int bFromTemplate);
    int SaveStore(CResGFF *pGFF, CResStruct *pStruct);

    char pad_0x0[0x4];
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x180];
    CExoString m_sScripts[1];        // +0x228
    CItemRepository *m_pItemRepository; // +0x230
    CExoLocString m_sLocName;        // +0x234
    char pad_0x23c[0x8];
    int m_nMarkDown;                 // +0x244
    int m_nMarkUp;                   // +0x248
    char pad_0x24c[0xa];
    unsigned char m_nBuySellFlag;    // +0x256
};

// FUNCTION: SWKOTOR 0x005C7760
int CSWSStore::LoadFromTemplate(const CResRef &cResRef, int bFromTemplate)
{
    CResGFF *pGFF = new CResGFF(0x803, "UTM ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        CExoString sError;
        CExoString sResRef;
        cResRef.CopyToString(sResRef);
        sError.Format("Store template %s doesn't exist.\n", sResRef.CStr());
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    LoadStore(pGFF, &cTop, bFromTemplate);
    delete pGFF;
    return 1;
}

// FUNCTION: SWKOTOR 0x005C6CD0
int CSWSStore::SaveStore(CResGFF *pGFF, CResStruct *pStruct)
{
    pGFF->WriteFieldCExoString(pStruct, m_sTag, "Tag");
    pGFF->WriteFieldCExoLocString(pStruct, m_sLocName, "LocName");
    pGFF->WriteFieldINT(pStruct, m_nMarkDown, "MarkDown");
    pGFF->WriteFieldINT(pStruct, m_nMarkUp, "MarkUp");
    pGFF->WriteFieldCResRef(pStruct, m_sScripts[0], "OnOpenStore");
    pGFF->WriteFieldBYTE(pStruct, m_nBuySellFlag, "BuySellFlag");
    CResList cList;
    pGFF->AddList(&cList, pStruct, "ItemList");
    for (unsigned long i = 0; i < m_pItemRepository->m_nItemCount; i++) {
        CSWSItem *pItem = m_pItemRepository->ItemListGetItem(i);
        CResStruct cElement;
        pGFF->AddListElement(&cElement, &cList, i);
        pGFF->WriteFieldDWORD(&cElement, pItem->m_nObjectId, "ObjectId");
        pGFF->WriteFieldBYTE(&cElement, pItem->m_bInfinite, "Infinite");
        pItem->SaveItem(pGFF, &cElement);
    }
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.x, "XPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.y, "YPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vPosition.z, "ZPosition");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.x, "XOrientation");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.y, "YOrientation");
    pGFF->WriteFieldFLOAT(pStruct, m_vOrientation.z, "ZOrientation");
    return 1;
}

// NONMATCH (70): the original keeps pItem / nPosition / the BuySellFlag
// default in dead argument slots (frame 4 bytes smaller) and loads ebx = 0
// later; the loop and list logic otherwise line up.
// FUNCTION: SWKOTOR 0x005C7180
int CSWSStore::LoadStore(CResGFF *pGFF, CResStruct *pStruct, int bFromTemplate)
{
    int bSuccess;
    SetTag(pGFF->ReadFieldCExoString(pStruct, "Tag", bSuccess, ""));
    m_sLocName = pGFF->ReadFieldCExoLocString(pStruct, "LocName", bSuccess, CExoLocString());
    m_nMarkDown = pGFF->ReadFieldINT(pStruct, "MarkDown", bSuccess, 0);
    m_nMarkUp = pGFF->ReadFieldINT(pStruct, "MarkUp", bSuccess, 0);
    CExoString sScript;
    pGFF->ReadFieldCResRef(pStruct, "OnOpenStore", bSuccess, "").CopyToString(sScript);
    m_sScripts[0] = sScript;
    m_nBuySellFlag = pGFF->ReadFieldBYTE(pStruct, "BuySellFlag", bSuccess, m_nBuySellFlag);
    CExoArrayList<CSWSItem *> aItems;
    // The original leaves this uninitialised; its stack home is pGFF's
    // argument slot, so an empty ItemList leaves it equal to pGFF.
    CSWSItem *pItem = (CSWSItem *)pGFF;
    CResList cList;
    if (pGFF->GetList(&cList, pStruct, "ItemList")) {
        for (unsigned long i = 0; i < pGFF->GetListCount(&cList); i++) {
            pItem = 0;
            CResStruct cElement;
            pGFF->GetListElement(&cElement, &cList, i);
            unsigned long oid = pGFF->ReadFieldDWORD(&cElement, "ObjectId", bSuccess, 0x7f000000);
            if (g_pAppManager->GetServerExoApp()->GetGameObject(oid))
                continue;
            pItem = new CSWSItem(oid);
            if (!pItem)
                continue;
            if (bFromTemplate) {
                CResRef cResRef = pGFF->ReadFieldCResRef(&cElement, "InventoryRes", bSuccess, "");
                if (!bSuccess || !pItem->LoadFromTemplate(cResRef)) {
                    delete pItem;
                    pItem = 0;
                    continue;
                }
            } else if (!pItem->LoadItem(pGFF, &cElement)) {
                delete pItem;
                pItem = 0;
                continue;
            }
            pItem->m_bInfinite = pGFF->ReadFieldBYTE(&cElement, "Infinite", bSuccess, 0);
            unsigned long nCost = pItem->GetCost();
            int nPosition = -1;
            for (int k = 0; k < aItems.num && nPosition == -1; k++) {
                if (aItems.element[k]->GetCost() < nCost)
                    nPosition = k;
            }
            if (nPosition != -1)
                aItems.Insert(pItem, nPosition);
            else
                aItems.Add(pItem);
        }
    }
    for (int j = aItems.num - 1; j >= 0; j--) {
        if (AddItemToInventory(&aItems.element[j]) && pItem)
            aItems.element[j]->SetPossessor(m_idSelf, 1, 1, 0x7f000000);
    }
    g_pAppManager->m_pServerExoApp->GetModule()->AddObjectToLookupTable(m_sTag, m_idSelf);
    return 1;
}
