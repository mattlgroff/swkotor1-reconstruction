// CSWSDoor / CSWSTrigger lifetime, open-state and mine helpers
// (b0058_f2_door_trigger, 0x005890D0..0x0058F690).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const char *s);
    int operator==(const CExoString &s) const;
    int CompareNoCase(const char *s) const;
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

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;
    CExoArrayList() { element = 0; num = 0; array_size = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
};

class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0x28];
    C2DA *m_pDoorTable;              // +0x28
    C2DA *GetDoorTable() { return m_pDoorTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bFlag);
};

extern CVirtualMachine *g_pVirtualMachine;

extern unsigned short g_nEffectTypeVisual;
extern unsigned short g_nEffectSubTypeMagical;
extern int g_nEffectVisualParam;
extern int g_nDoorStateFirst;
extern int g_nDoorStateLast;
extern unsigned long EVENT_DESTROY_OBJECT;

class CGameEffect {
public:
    CGameEffect(int bCreate);
    void SetInteger(int nIndex, int nValue);
    void SetSubType_Magical() { m_nSubType = (m_nSubType & 0xfff8) | g_nEffectSubTypeMagical; }
    char pad_0x0[0x8];
    unsigned short m_nType;          // +0x8
    unsigned short m_nSubType;       // +0xa
    char pad_0xc[0x14];
    unsigned long m_oidCreator;      // +0x20
    char pad_0x24[0x68];
};

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation); // +0x7c

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
    unsigned long GetID() { return m_idSelf; }
};

class CServerAIMaster {
public:
    void RemoveObject(CGameObject *pObject);
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CSWSModule {
public:
    void RemoveObjectFromLookupTable(CExoString *pTag, unsigned long oid);
};

class CSWSCreature;
class CSWSItem;

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CServerAIMaster *GetServerAIMaster();
    CSWSCreature *GetCreatureByGameObjectID(unsigned long id);
    CSWSItem *GetItemByGameObjectID(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSObject : public CGameObject {
public:
    virtual ~CSWSObject();
    void *GetArea();
    void ApplyEffect(CGameEffect *pEffect, int a, int b);

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x50];
    int m_bF8;                       // +0xf8
    char pad_0xfc[0xf0];
    int m_bApplyingPostProcess;      // +0x1ec
    char pad_0x1f0[0x38];
};

class CSWDoorSurfaceMesh {
public:
    ~CSWDoorSurfaceMesh();
};

class CSWSDoor : public CSWSObject {
public:
    virtual ~CSWSDoor();
    int GetIsLinkedToDoorOrTrigger();
    void SetOpenState(unsigned char nState, int bNextOnly);
    void PostProcess();
    void RemoveFromArea();

    CExoString m_sScripts[15];       // +0x228
    char pad_0x2a0[0x1];
    unsigned char m_nAppearance;     // +0x2a1
    char pad_0x2a2[0x20];
    unsigned char m_nVisualEffect;   // +0x2c2
    char pad_0x2c3[0x9];
    unsigned char m_nOpenState;      // +0x2cc
    unsigned char m_nNextOpenState;  // +0x2cd
    char pad_0x2ce[0x2];
    CExoString m_s2D0;               // +0x2d0
    char pad_0x2d8[0x4];
    CExoArrayList<unsigned long> m_aList; // +0x2dc
    char pad_0x2e8[0x20];
    int m_bStatic;                   // +0x308
    char pad_0x30c[0x20];
    CSWDoorSurfaceMesh *m_apWalkMeshes[3]; // +0x32c
    char pad_0x338[0x48];
    int m_n380;                      // +0x380
    unsigned char m_nLinkedToFlags;  // +0x384
    char pad_0x385[0x3];
    CExoString m_sLinkedTo;          // +0x388
    CExoString m_sTransitionDestination; // +0x390
    char pad_0x398[0x4];
    CExoLocString m_sLocName;        // +0x39c
    CExoLocString m_sDescription;    // +0x3a4
    char pad_0x3ac[0x14];
    int m_b3C0;                      // +0x3c0
    char pad_0x3c4[0x4];
    CExoLocString m_s3C8;            // +0x3c8
};

// FUNCTION: SWKOTOR 0x005890D0
int CSWSDoor::GetIsLinkedToDoorOrTrigger()
{
    if (m_nLinkedToFlags == 1 || m_nLinkedToFlags == 2)
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x00589600
void CSWSDoor::SetOpenState(unsigned char nState, int bNextOnly)
{
    unsigned char nNew = !m_bStatic ? 3 : nState;
    m_nNextOpenState = nNew;
    if (!bNextOnly)
        m_nOpenState = nNew;
    switch (nNew) {
    case 0:
        SetAnimation(0x2726);
        break;
    case 1:
        SetAnimation(0x2742);
        break;
    case 2:
        SetAnimation(0x2743);
        break;
    case 3:
        SetAnimation(0x2758);
        break;
    }
}

// FUNCTION: SWKOTOR 0x00589D40
void CSWSDoor::PostProcess()
{
    if (m_nVisualEffect > 0) {
        m_bApplyingPostProcess = 1;
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->m_nType = g_nEffectTypeVisual;
        pEffect->SetSubType_Magical();
        pEffect->m_oidCreator = 0;
        pEffect->SetInteger(0, g_nEffectVisualParam);
        pEffect->SetInteger(1, m_nVisualEffect);
        ApplyEffect(pEffect, 0, 0);
        m_bApplyingPostProcess = 0;
    }
    if (!m_bStatic) {
        SetOpenState(3, 1);
        m_bF8 = 1;
    }
    if (m_b3C0) {
        CExoString sAnim;
        if (!g_pRules->Get2DArrays()->GetDoorTable()->GetCExoStringEntry(m_nAppearance, "StaticAnim",
                                                                       &sAnim))
            sAnim = "";
        if (sAnim.CompareNoCase("busted"))
            SetAnimation(0x27a9);
    }
}

// FUNCTION: SWKOTOR 0x0058B550
CSWSDoor::~CSWSDoor()
{
    if (GetArea())
        RemoveFromArea();
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->m_pServerExoApp->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
    for (int i = g_nDoorStateFirst; i <= g_nDoorStateLast; i++) {
        if (m_apWalkMeshes[i]) {
            delete m_apWalkMeshes[i];
            m_apWalkMeshes[i] = 0;
        }
    }
    m_n380 = 0;
}

// FUNCTION: SWKOTOR 0x0058C810 SYMBOL: ??_GCSWSDoor@@UAEPAXI@Z

// ---------------------------------------------------------------------------

class CSWSItem {
public:
    virtual ~CSWSItem();
    char pad_0x4[0x10];
    unsigned long m_idSelf;          // +0x14
    char pad_0x18[0x10];
    CExoString m_sTag;               // +0x28
};

class CItemRepository {
public:
    unsigned long FindItemWithTag(const CExoString &sTag);
    int RemoveItem(CSWSItem *pItem);
};

class CSWInventory {
public:
    int RemoveItem(CSWSItem *pItem);
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreature : public CSWSObject {
public:
    CItemRepository *GetItemRepository(int bPlayer);
    void SendFeedbackMessage(unsigned short nMessage, void *pData);
    char pad_0x228[0x804];
    CSWInventory *m_pInventory;      // +0xa2c
    CSWInventory *GetInventory() { return m_pInventory; }
};

class CSWSTrigger : public CSWSObject {
public:
    virtual ~CSWSTrigger();
    int GetCanFireMineOnObject(unsigned long oidTarget, int bFlag);
    void OnEnterMine(int bFlag);
    void RemoveFromArea();

    CExoLocString m_sLocName;        // +0x228
    CExoString m_sTransitionDestination; // +0x230
    CExoString m_sLinkedTo;          // +0x238
    char pad_0x240[0x4];
    CExoString m_sScripts[7];        // +0x244
    CExoString m_sKeyName;           // +0x27c
    char pad_0x284[0x4];
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0xc];
    int *m_pnPointIndices;           // +0x298
    unsigned long m_oidLastEntered;  // +0x29c
    char pad_0x2a0[0x4];
    unsigned long m_oidLastDisarmed; // +0x2a4
    CExoArrayList<unsigned long> m_aList; // +0x2a8
    char pad_0x2b4[0x18];
    int m_bAutoRemoveKey;            // +0x2cc
    int m_bDestroyOnFire;            // +0x2d0
    char pad_0x2d4[0x38];
    CExoLocString m_s30C;            // +0x30c
};

// FUNCTION: SWKOTOR 0x0058D570
void CSWSTrigger::OnEnterMine(int bFlag)
{
    CSWSCreature *pCreature =
        g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(m_oidLastEntered);
    if (!pCreature)
        return;
    if (!GetCanFireMineOnObject(pCreature->m_idSelf, bFlag))
        return;
    unsigned long oidItem = pCreature->GetItemRepository(1)->FindItemWithTag(m_sKeyName);
    if (oidItem == 0x7f000000) {
        for (int i = 0; i < 18; i++) {
            CSWSItem *pItem = pCreature->GetInventory()->GetItemInSlot(1 << i);
            if (pItem && pItem->m_sTag == m_sKeyName)
                oidItem = pItem->m_idSelf;
        }
    }
    CSWSItem *pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(oidItem);
    if (pItem) {
        if (m_bAutoRemoveKey) {
            pCreature->GetItemRepository(1)->RemoveItem(pItem);
            pCreature->m_pInventory->RemoveItem(pItem);
            delete pItem;
        }
        m_oidLastDisarmed = m_oidLastEntered;
        g_pVirtualMachine->RunScript(&m_sScripts[5], m_idSelf, 1);
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, m_idSelf, m_idSelf, EVENT_DESTROY_OBJECT, 0);
    } else {
        pCreature->SendFeedbackMessage(0x52, 0);
        g_pVirtualMachine->RunScript(&m_sScripts[4], m_idSelf, 1);
        g_pVirtualMachine->RunScript(&m_sScripts[1], m_idSelf, 1);
        if (m_bDestroyOnFire)
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, m_idSelf, m_idSelf, EVENT_DESTROY_OBJECT, 0);
    }
}

// FUNCTION: SWKOTOR 0x0058EEF0
CSWSTrigger::~CSWSTrigger()
{
    delete[] m_pvVertices;
    delete[] m_pnPointIndices;
    if (GetArea())
        RemoveFromArea();
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->m_pServerExoApp->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
}

// FUNCTION: SWKOTOR 0x0058F690 SYMBOL: ??_GCSWSTrigger@@UAEPAXI@Z
