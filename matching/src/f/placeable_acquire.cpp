// CSWSPlaceable::AcquireItemsFromObject (0x00588690), own TU so that
// AcquireItem (placeable.cpp) stays out of line.

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

class CResRef {
public:
    CResRef(const char *szName);
    char m_resRef[16];
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList()
    {
        element = 0;
        num = 0;
        array_size = 0;
    }
    ~CExoArrayList() { delete[] element; }
    void Allocate(int nSize);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

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
    CSWSItem(unsigned long oid);
    void SetPossessor(unsigned long oid, int a, int b, unsigned long oidPrev);
    void RemoveFromArea();
    int LoadFromTemplate(const CResRef &cResRef);
    unsigned long m_nBaseItem;       // +0xc
    char pad_0x10[0x258];
    unsigned long m_oidPossessor;    // +0x268
    CItemRepository *m_pItemRepository; // +0x26c
    char pad_0x270[0x18];
    unsigned char m_nItemFlags;      // +0x288 (bit 3: droppable; placeholder name)
    char pad_0x289[0x3];
    unsigned long m_nStackSize;      // +0x28c
    char pad_0x290[0xc];
};

class CSWInventory {
public:
    CSWSItem *GetItemInSlot(unsigned long nSlot);
};

class CSWSCreature : public CGameObject {
public:
    int RemoveItem(CSWSItem *pItem, int a, int b, CSWSItem **ppItem);
    CItemRepository *GetItemRepository(int bPlayer);
    unsigned long GetGold();
    void SetGold(int nGold);
    char pad_0xc[0xa20];
    CSWInventory *m_pInventory;      // +0xa2c
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
    CSWSCreature *GetCreatureByGameObjectID(unsigned long oid);
    CSWSPlaceable *GetPlaceableByGameObjectID(unsigned long oid);
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
    int AcquireItemsFromObject(unsigned long oidObject, int bDroppableOnly);
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


// NONMATCH (105): first draft. The original keeps the local list's num / size in
// esi / edi across both collection loops (written back after each Add), counts the
// equipment-slot loop down from 14 in a stack slot, and duplicates the early
// return epilogue; cl reloads the list fields from memory every time.
// FUNCTION: SWKOTOR 0x00588690
int CSWSPlaceable::AcquireItemsFromObject(unsigned long oidObject, int bDroppableOnly)
{
    CSWSCreature *pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidObject);
    CSWSPlaceable *pPlaceable = g_pAppManager->m_pServerExoApp->GetPlaceableByGameObjectID(oidObject);
    if (!pCreature && !pPlaceable)
        return 0;
    if (pCreature && (!pCreature->m_pInventory || !pCreature->GetItemRepository(1)))
        return 0;
    if (pPlaceable && !pPlaceable->m_pRepository)
        return 0;
    CItemRepository *pRepository;
    if (pCreature) {
        pRepository = pCreature->GetItemRepository(1);
    } else if (pPlaceable) {
        pRepository = pPlaceable->m_pRepository;
        bDroppableOnly = 0;
    }
    CSWSItem *pItem;
    int nAcquired = 0;
    CExoArrayList<CSWSItem *> lItems;
    if (pCreature) {
        for (int nSlot = 0; nSlot < 0xe; nSlot++) {
            pItem = pCreature->m_pInventory->GetItemInSlot(1 << nSlot);
            if (!pItem)
                continue;
            if (bDroppableOnly && !(pItem->m_nItemFlags & 8))
                continue;
            lItems.Add(pItem);
        }
    }
    for (unsigned long i = 0; i < pRepository->m_nItemCount; i++) {
        pItem = pRepository->ItemListGetItem(i);
        if (!pItem)
            continue;
        if (bDroppableOnly && !(pItem->m_nItemFlags & 8))
            continue;
        lItems.Add(pItem);
    }
    for (int j = 0; j < lItems.num; j++) {
        pItem = lItems.element[j];
        if (pItem && AcquireItem(&pItem, oidObject, 1))
            nAcquired++;
    }
    if (pCreature && pCreature->GetGold() > 0) {
        pItem = new CSWSItem(0x7f000000);
        pItem->LoadFromTemplate(CResRef("g_i_credits001"));
        pItem->m_nStackSize = pCreature->GetGold();
        pCreature->SetGold(0);
        AcquireItem(&pItem, 0x7f000000, 1);
    }
    return nAcquired;
}
