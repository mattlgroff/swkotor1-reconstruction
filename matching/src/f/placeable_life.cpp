// CSWSPlaceable lifetime / area / 2DA helpers
// (b0058_f2_surfmesh_placeable, 0x00584950..0x00587A50).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
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

class CResRef {
public:
    char m_resRef[16];
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
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0xc];
    C2DA *m_pBodyBagTable;           // +0xc
    char pad_0x10[0x1c];
    C2DA *m_pPlaceableTable;         // +0x2c
    char pad_0x30[0xa4];
    C2DA *m_pAppearanceTable;        // +0xd4
    C2DA *GetAppearanceTable() { return m_pAppearanceTable; }
    C2DA *GetBodyBagTable() { return m_pBodyBagTable; }
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

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers *m_pcExoTimers;       // +0x4
};

extern CExoBase *g_pExoBase;

class CSWSObject;
class CSWCPlaceable;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject *AsSWSObject();         // +0x10
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable *AsSWCPlaceable();   // +0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90();
    virtual int GetDead();                     // +0x94

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
    unsigned long GetID() { return m_idSelf; }
};

class CSWSItem : public CGameObject {
public:
    char pad_0xc[0x27c];
    unsigned char m_nFlags288;       // +0x288
};

class CItemRepository {
public:
    ~CItemRepository();
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
    char pad_0x14[0x4];
};

class CServerAIMaster {
public:
    void RemoveObject(CGameObject *pObject);
};

class CSWSModule {
public:
    void RemoveObjectFromLookupTable(CExoString *pTag, unsigned long oid);
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long *pnCalendarDay, unsigned long *pnTimeOfDay);
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                           unsigned long nTime2, unsigned long *pnDay, unsigned long *pnTime);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
    CServerAIMaster *GetServerAIMaster();
    CGameObject *GetGameObject(unsigned long id);
    CWorldTimer *GetWorldTimer();
};

class CSWCCreature {
public:
    char pad_0x0[0x3dc];
    unsigned long m_oidPlaceable;    // +0x3dc
};

class CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);
    CSWCCreature *GetCreatureByGameObjectID(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;  // +0x4
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWArea {
public:
    int RemoveStaticBoundingBox(unsigned long oid);
};

class CSWSArea : public CSWArea {
public:
    void RemoveObjectFromArea(unsigned long oid);
};

class CSWSObject : public CGameObject {
public:
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);
    CGameObject *GetClientObject();
    void UpdateEffects(unsigned long nCalendarDay, unsigned long nTimeOfDay);
    void UpdateActions(unsigned long nCalendarDay, unsigned long nTimeOfDay,
                       unsigned __int64 nTimer);
    int AddAction(unsigned long nAction, unsigned short nGroup,
                  unsigned long t1, void *p1, unsigned long t2, void *p2,
                  unsigned long t3, void *p3, unsigned long t4, void *p4,
                  unsigned long t5, void *p5, unsigned long t6, void *p6,
                  unsigned long t7, void *p7, unsigned long t8, void *p8,
                  unsigned long t9, void *p9, unsigned long t10, void *p10,
                  unsigned long t11, void *p11, unsigned long t12, void *p12,
                  unsigned long t13, void *p13);

    char pad_0xc[0xc];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x70];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x180];
};

class CSWPlaceableSurfaceMesh {
public:
    ~CSWPlaceableSurfaceMesh();
    char pad_0x0[0x88];
};

class CSWSPlaceable : public CSWSObject {
public:
    virtual ~CSWSPlaceable();
    void RemoveFromArea();
    void AIUpdate();
    int AddCastFakeSpellActions(unsigned long nSpell, int nArg, const Vector &vPosition,
                                unsigned long oidTarget, int bFlag, unsigned char nProjectile);
    void SetLightIsOn(int bOn);
    int GetLightIsOn();
    unsigned char GetBodyBagAppearance();
    void OnEndDialog(int nUnused);
    CSWCPlaceable *GetClientPlaceable();
    CResRef GetConversation();
    unsigned long GetLastUsedBy();
    void SetLastUsedBy(unsigned long oid);
    int GetItemCount(int bFlag);
    unsigned char GetAppearance() { return m_nAppearance; }

    CExoLocString m_sLocName;        // +0x228
    unsigned char m_nAppearance;     // +0x230
    char pad_0x231[0x3];
    CExoLocString m_sDescription;    // +0x234
    char pad_0x23c[0x4];
    CResRef m_cConversation;         // +0x240
    char pad_0x250[0x14];
    CExoString m_sPortrait;          // +0x264
    char pad_0x26c[0x28];
    CExoString m_sScripts[16];       // +0x294
    char pad_0x314[0x4];
    CExoArrayList<unsigned long> m_aList; // +0x318
    int m_bHasInventory;             // +0x324
    char pad_0x328[0x1c];
    int m_nBoundingBoxIndex;         // +0x344
    unsigned long m_nLastHeartbeatDay;  // +0x348
    unsigned long m_nLastHeartbeatTime; // +0x34c
    char pad_0x350[0x1c];
    CItemRepository *m_pItemRepository; // +0x36c
    char pad_0x370[0x18];
    unsigned long m_oidLastUsedBy;   // +0x388
    int m_bLightIsOn;                // +0x38c
    int m_bLightStateChanged;        // +0x390
    unsigned char m_nBodyBag;        // +0x394
    char pad_0x395[0xb];
    CSWPlaceableSurfaceMesh m_cWalkMesh; // +0x3a0
    char pad_0x428[0x14];
    int m_bInConversation;           // +0x43c
    int m_bHasClientCreature;        // +0x440
    char pad_0x444[0x4];
    unsigned long m_oidClientCreature;  // +0x448
};

// FUNCTION: SWKOTOR 0x00584950
void CSWSPlaceable::RemoveFromArea()
{
    if (GetArea()) {
        if (m_nBoundingBoxIndex >= 0) {
            GetArea()->RemoveStaticBoundingBox(GetID());
            m_nBoundingBoxIndex = -1;
        }
        GetArea()->RemoveObjectFromArea(m_idSelf);
    }
    SetArea(0);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}

// FUNCTION: SWKOTOR 0x005849D0
void CSWSPlaceable::AIUpdate()
{
    unsigned __int64 nTimer = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    unsigned long nTimeOfDay, nCalendarDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    unsigned long nDayDiff, nTimeDiff;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(
        nCalendarDay, nTimeOfDay, m_nLastHeartbeatDay, m_nLastHeartbeatTime, &nDayDiff, &nTimeDiff);
    if (nTimeDiff >= 6000) {
        if (m_nLastHeartbeatTime && !GetDead())
            g_pVirtualMachine->RunScript(&m_sScripts[4], m_idSelf, 1);
        m_nLastHeartbeatDay = nCalendarDay;
        m_nLastHeartbeatTime = nTimeOfDay;
    }
    UpdateEffects(nCalendarDay, nTimeOfDay);
    UpdateActions(nCalendarDay, nTimeOfDay, nTimer);
}

// FUNCTION: SWKOTOR 0x00584DB0
int CSWSPlaceable::AddCastFakeSpellActions(unsigned long nSpell, int nArg,
                                           const Vector &vPosition, unsigned long oidTarget,
                                           int bFlag, unsigned char nProjectile)
{
    Vector vTarget = vPosition;
    if (oidTarget != 0x7f000000) {
        CGameObject *pGameObject = g_pAppManager->m_pServerExoApp->GetGameObject(oidTarget);
        if (!pGameObject)
            return 0;
        CSWSObject *pTarget = pGameObject->AsSWSObject();
        if (pTarget) {
            CSWSArea *pTargetArea = pTarget->GetArea();
            if (pTargetArea == GetArea())
                vTarget = pTarget->m_vPosition;
            else
                return 0;
        }
    }
    unsigned long nFlags = nProjectile;
    if (bFlag)
        nFlags |= 0x80000000;
    AddAction(0xf, 0xffff, 1, &nSpell, 1, &nArg, 3, &oidTarget, 2, &vTarget.x, 2, &vTarget.y,
              2, &vTarget.z, 1, &nFlags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x00585080
void CSWSPlaceable::SetLightIsOn(int bOn)
{
    if (m_bLightIsOn != bOn) {
        int nValue;
        if (!g_pRules->Get2DArrays()->m_pPlaceableTable->GetINTEntry(GetAppearance(), "LightColor",
                                                                   &nValue))
            return;
        m_bLightStateChanged = 1;
        m_bLightIsOn = bOn;
    }
}

// FUNCTION: SWKOTOR 0x00585130
int CSWSPlaceable::GetLightIsOn()
{
    int nValue;
    if (!g_pRules->Get2DArrays()->m_pPlaceableTable->GetINTEntry(GetAppearance(), "LightColor",
                                                               &nValue))
        return 0;
    return m_bLightIsOn;
}

// NONMATCH (20): the original gives nAppearance the lower frame slot and keeps
// two epilogues (`mov eax,3` path); ours swaps the slots and merges the returns.
// FUNCTION: SWKOTOR 0x005851E0
unsigned char CSWSPlaceable::GetBodyBagAppearance()
{
    int nAppearance;
    if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(m_nBodyBag, "Appearance",
                                                                &nAppearance)) {
        int nBodyBag;
        if (!g_pRules->Get2DArrays()->GetBodyBagTable()->GetINTEntry(m_nAppearance, "BodyBag",
                                                                 &nBodyBag))
            nBodyBag = 0;
        if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(nBodyBag, "Appearance",
                                                                    &nAppearance))
            return 3;
    }
    return (unsigned char)nAppearance;
}

// FUNCTION: SWKOTOR 0x00585460
void CSWSPlaceable::OnEndDialog(int nUnused)
{
    g_pVirtualMachine->RunScript(&m_sScripts[15], m_idSelf, 1);
    m_bInConversation = 0;
}

// FUNCTION: SWKOTOR 0x00585490
CSWCPlaceable *CSWSPlaceable::GetClientPlaceable()
{
    if (GetClientObject())
        return GetClientObject()->AsSWCPlaceable();
    return 0;
}

// FUNCTION: SWKOTOR 0x005854B0
CSWSPlaceable::~CSWSPlaceable()
{
    if (m_pItemRepository) {
        delete m_pItemRepository;
        m_pItemRepository = 0;
    }
    if (GetArea())
        RemoveFromArea();
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->RemoveObject(this);
    CSWSModule *pModule = g_pAppManager->m_pServerExoApp->GetModule();
    if (pModule) {
        unsigned long oid = m_idSelf;
        pModule->RemoveObjectFromLookupTable(&m_sTag, oid);
    }
    if (m_bHasClientCreature) {
        unsigned long oidClient =
            g_pAppManager->m_pClientExoApp->ServerToClientObjectId(m_oidClientCreature);
        CSWCCreature *pCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(oidClient);
        if (pCreature)
            pCreature->m_oidPlaceable = 0x7f000000;
    }
}

// FUNCTION: SWKOTOR 0x00585620
CResRef CSWSPlaceable::GetConversation()
{
    return m_cConversation;
}

// FUNCTION: SWKOTOR 0x00585650
unsigned long CSWSPlaceable::GetLastUsedBy()
{
    return m_oidLastUsedBy;
}

// FUNCTION: SWKOTOR 0x00585660
void CSWSPlaceable::SetLastUsedBy(unsigned long oid)
{
    m_oidLastUsedBy = oid;
}

// FUNCTION: SWKOTOR 0x00587710
int CSWSPlaceable::GetItemCount(int bFlag)
{
    if (!m_bHasInventory)
        return 0;
    int nCount = 0;
    CItemRepository *pRepository = m_pItemRepository;
    if (pRepository) {
        for (unsigned long i = 0; i < pRepository->m_nItemCount; i++) {
            CSWSItem *pItem = pRepository->ItemListGetItem(i);
            if (pItem && (!bFlag || (pItem->m_nFlags288 & 8)))
                nCount++;
        }
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x00587A50 SYMBOL: ??_GCSWSPlaceable@@UAEPAXI@Z
