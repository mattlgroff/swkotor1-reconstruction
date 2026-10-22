// CSWSPlaceable inventory / effect glue
// (b0058_f2_surfmesh_placeable, 0x00584870..0x00587770).

class Vector {
public:
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
};

struct CExoLinkedListNode;

class CExoLinkedListInternal {
public:
    void *GetAtPos(CExoLinkedListNode *pos);
    void *GetNext(CExoLinkedListNode *&pos);
    CExoLinkedListNode *pHead;
    CExoLinkedListNode *pTail;
    unsigned long nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListNode *GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T *GetAtPos(CExoLinkedListNode *pos) { return (T *)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T *GetNext(CExoLinkedListNode *&pos) { return (T *)m_pcExoLinkedListInternal->GetNext(pos); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

class CSWSCreature;
class CSWSItem;
class CSWSPlaceable;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature *AsSWSCreature();     // +0x30
    virtual void v34();
    virtual CSWSItem *AsSWSItem();             // +0x38
    virtual void v3c(); virtual void v40(); virtual void v44();
    virtual CSWSPlaceable *AsSWSPlaceable();   // +0x48
    virtual void v4c(); virtual void v50(); virtual void v54(); virtual void v58();
    virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation); // +0x7c

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CGameObjectArray {
public:
    char GetGameObject(unsigned long id, CGameObject **ppObject);
};

class CItemRepository {
public:
    int GetItemInRepository(CSWSItem *pItem, int bRecursive);
    int RemoveItem(CSWSItem *pItem);
    CSWSItem *ItemListGetItem(unsigned long nIndex);
    int AddItem(CSWSItem **ppItem, int a, int b, int c);
    char pad_0x0[0x10];
    unsigned long m_nItemCount;      // +0x10
};

class CSWSItem : public CGameObject {
public:
    void SetPossessor(unsigned long oid, int a, int b, unsigned long oidPrev);
    void RemoveFromArea();
    unsigned long m_nBaseItem;       // +0xc
    char pad_0x10[0x258];
    unsigned long m_oidPossessor;    // +0x268
    CItemRepository *m_pItemRepository; // +0x26c
};

class CSWSCreature : public CGameObject {
public:
    int RemoveItem(CSWSItem *pItem, int a, int b, CSWSItem **ppItem);
    CItemRepository *GetItemRepository(int bPlayer);
};

class CSWBaseItem {
public:
    char pad_0x0[0x70];
    int m_n70;                       // +0x70
};

class CSWBaseItemArray {
public:
    CSWBaseItem *GetBaseItem(int n);
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray *m_pBaseItemArray; // +0x34
};

class CSWSPlayer {
public:
    CGameObject *GetGameObject();
    char pad_0x0[0x58];
    unsigned long *m_poidOpened;     // +0x58
};

class CSWSMessage {
public:
    int SendServerToPlayerOpenInventory(CSWSPlayer *pPlayer, unsigned long oid, int bHasItems);
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEvent, void *pData);
};

class CServerExoApp {
public:
    CSWSPlayer *GetClientObjectByObjectId(unsigned long id);
    CGameObjectArray *GetObjectArray();
    CServerAIMaster *GetServerAIMaster();
    CExoLinkedList<CSWSPlayer> *GetPlayerList();
    CSWSMessage *GetSWSMessage();
};

class CClientOptions {
public:
    char pad_0x0[0xb4];
    int m_nB4;                       // +0xb4
};

class CClientExoApp {
public:
    CClientOptions *GetClientOptions();
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;  // +0x4
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;
extern CSWRules *g_pRules;
extern char g_cObjectArraySuccess;
extern unsigned long EVENT_SIGNAL_EVENT;
extern unsigned long EVENT_PLACEABLE_CLOSED;

class CScriptEvent {
public:
    CScriptEvent();
    unsigned short m_nType;
    char pad_0x2[0x32];
};

extern unsigned short g_nEffectTypeVisual;
extern unsigned short g_nEffectSubTypeMagical;
extern int g_nEffectVisualParam;

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

class CSWCollisionMesh {
public:
    int NoNonWalkPolys(float a, float b, float c, float d, float e, int f);
    char pad_0x0[0x20];
    Vector m_vPosition;              // +0x20
    char pad_0x2c[0xc];
    Quaternion m_qOrientation;       // +0x38
};

class CSWSPlaceable : public CGameObject {
public:
    void PostProcess();
    int RemoveItem(CSWSItem *pItem);
    int AcquireItem(CSWSItem **ppItem, unsigned long oidPossessor, int bFlag);
    int NoNonWalkPolys(float a, float b, const float &c, const float &d, const float &e, int f);
    void OpenInventory(unsigned long oidPlayer, int bNotify);
    void CloseInventory(unsigned long oidPlayer, int bDrop);
    void ClosePlaceableForAllPlayers();
    void ApplyEffect(CGameEffect *pEffect, int a, int b);

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    char pad_0x9c[0x150];
    int m_bApplyingPostProcess;      // +0x1ec
    char pad_0x1f0[0x68];
    unsigned char m_nVisualEffect;   // +0x258
    char pad_0x259[0xcb];
    int m_bHasInventory;             // +0x324
    int m_n328;                      // +0x328
    char pad_0x32c[0x8];
    int m_n334;                      // +0x334
    int m_bOpen;                     // +0x338
    char pad_0x33c[0x30];
    CItemRepository *m_pRepository;  // +0x36c
    CItemRepository *GetItemRepository() { return m_pRepository; }
    char pad_0x370[0x30];
    CSWCollisionMesh m_cWalkMesh;    // +0x3a0
    char pad_0x3e8[0x40];
    Quaternion m_qOrientation;       // +0x428
};

// FUNCTION: SWKOTOR 0x00584870
void CSWSPlaceable::PostProcess()
{
    if (m_nVisualEffect > 0) {
        m_bApplyingPostProcess = 1;
        CGameEffect *pEffect = new CGameEffect(1);
        pEffect->m_nType = g_nEffectTypeVisual;
        pEffect->SetSubType_Magical();
        pEffect->SetInteger(0, g_nEffectVisualParam);
        pEffect->SetInteger(1, m_nVisualEffect);
        pEffect->m_oidCreator = 0;
        ApplyEffect(pEffect, 0, 0);
        m_bApplyingPostProcess = 0;
    }
}

// FUNCTION: SWKOTOR 0x00584AC0
int CSWSPlaceable::RemoveItem(CSWSItem *pItem)
{
    if (GetItemRepository()->GetItemInRepository(pItem, 0) == 1) {
        GetItemRepository()->RemoveItem(pItem);
        pItem->SetPossessor(0x7f000000, 1, 1, 0x7f000000);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00584B10
int CSWSPlaceable::AcquireItem(CSWSItem **ppItem, unsigned long oidPossessor, int bFlag)
{
    CGameObjectArray *pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    CItemRepository *pRepository = m_pRepository;
    CGameObject *pObject = 0;
    unsigned long oidPrevious = 0x7f000000;
    if ((*ppItem)->m_oidPossessor != oidPossessor)
        return 0;
    if (pArray->GetGameObject(oidPossessor, &pObject) == g_cObjectArraySuccess && pObject) {
        unsigned char nType = pObject->m_nObjectType;
        oidPrevious = pObject->m_idSelf;
        if (nType == 5) {
            pObject->AsSWSCreature()->RemoveItem(*ppItem, 1, bFlag, ppItem);
        } else if (nType == 9) {
            pObject->AsSWSPlaceable()->RemoveItem(*ppItem);
        } else if (nType == 6) {
            unsigned long nBaseItem = pObject->AsSWSItem()->m_nBaseItem;
            if (g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem)->m_n70 == 1) {
                CSWSItem *pOther = pObject->AsSWSItem();
                pOther->m_pItemRepository->RemoveItem(*ppItem);
            }
        }
    } else {
        (*ppItem)->RemoveFromArea();
    }
    if (pRepository) {
        pRepository->AddItem(ppItem, 1, 0, 1);
        if (*ppItem)
            (*ppItem)->SetPossessor(m_idSelf, 1, 1, oidPrevious);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00585350
int CSWSPlaceable::NoNonWalkPolys(float a, float b, const float &c, const float &d,
                                  const float &e, int f)
{
    m_cWalkMesh.m_vPosition = m_vPosition;
    m_cWalkMesh.m_qOrientation = m_qOrientation;
    return m_cWalkMesh.NoNonWalkPolys(a, b, c, d, e, f);
}

// NONMATCH (~18 insns): register allocation only (oidPlayer in ebx vs edi,
// the count>0 temp and the AddEventDeltaTime operands); control flow and
// stack layout match.
// FUNCTION: SWKOTOR 0x00587420
void CSWSPlaceable::OpenInventory(unsigned long oidPlayer, int bNotify)
{
    if (m_bOpen || !m_bHasInventory)
        return;
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nB4) {
        SetAnimation(0x275c);
        return;
    }
    CSWSPlayer *pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(oidPlayer);
    if (pPlayer)
        g_pAppManager->m_pServerExoApp->GetSWSMessage()->SendServerToPlayerOpenInventory(
            pPlayer, m_idSelf, m_pRepository->m_nItemCount > 0);
    CScriptEvent *pEvent = new CScriptEvent;
    pEvent->m_nType = 0x16;
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
        0, 0, oidPlayer, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
    if (bNotify)
        SetAnimation(0x275b);
    m_bOpen = 1;
}

// NONMATCH (~15 insns): register allocation only (loop temps edx/ecx and the
// AddEventDeltaTime operand registers); control flow and stack layout match.
// FUNCTION: SWKOTOR 0x00587560
void CSWSPlaceable::CloseInventory(unsigned long oidPlayer, int bDrop)
{
    if (!m_bOpen)
        return;
    if (bDrop) {
        CSWSPlayer *pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(oidPlayer);
        if (pPlayer) {
            CSWSCreature *pCreature = pPlayer->GetGameObject()->AsSWSCreature();
            while (m_pRepository->m_nItemCount > 0) {
                CSWSItem *pItem = m_pRepository->ItemListGetItem(m_pRepository->m_nItemCount - 1);
                m_pRepository->RemoveItem(pItem);
                pCreature->GetItemRepository(1)->AddItem(&pItem, 1, 1, 1);
                if (pItem)
                    pItem->SetPossessor(oidPlayer, 1, 1, 0x7f000000);
            }
        }
    }
    CScriptEvent *pEvent = new CScriptEvent;
    pEvent->m_nType = 0x17;
    g_pAppManager->m_pServerExoApp->GetServerAIMaster()->AddEventDeltaTime(
        0, 0, oidPlayer, m_idSelf, EVENT_SIGNAL_EVENT, pEvent);
    SetAnimation(0x275c);
    m_bOpen = 0;
    if (m_bHasInventory && m_n334 && m_pRepository->m_nItemCount == 0) {
        m_n328 = 0;
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, m_idSelf, m_idSelf, EVENT_PLACEABLE_CLOSED, 0);
    }
}

// FUNCTION: SWKOTOR 0x00587770
void CSWSPlaceable::ClosePlaceableForAllPlayers()
{
    CExoLinkedList<CSWSPlayer> *pList = g_pAppManager->m_pServerExoApp->GetPlayerList();
    CExoLinkedListNode *pos = pList->GetHeadPos();
    while (pos) {
        CSWSPlayer *pPlayer = pList->GetAtPos(pos);
        if (pPlayer) {
            CGameObject *pObject = pPlayer->GetGameObject();
            if (pObject && *pPlayer->m_poidOpened == m_idSelf)
                CloseInventory(pObject->m_idSelf, 1);
        }
        pList->GetNext(pos);
    }
}
