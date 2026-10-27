// CClientExoAppInternal::AddCreatureModelToIgnoreList, worker C s9
// (clientexoappinternal.o; translation-unit-local layout).

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);                           // 0x005E03C0 family
    void Add(T t);                                  // 0x00444FE0 (out of line)
    __forceinline void AddInline(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
};

class CGob;
class CSWCCreature;

class CItemModel;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30();
    virtual CItemModel* GetItemModel();             // 0x34

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635

class CSWCObject : public CGameObject {
public:
    virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned char nPart, int bRecurse);  // 0x98
};

class CSWCCreature : public CSWCObject {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
};

class CItemGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned char nPart, int bRecurse);  // 0x98
};

class CSWItemBase {
public:
    virtual void v00();
    char pad_0x4[0xc];
};

class CSWCItem : public CSWItemBase, public CItemGob {
};

class CItemModelBase {
public:
    virtual void v00();
    char pad_0x4[0xc];
};

class CItemModel : public CItemModelBase, public CItemGob {
};

class CClientExoApp {
public:
    CSWCItem* GetItemByGameObjectID(unsigned long oid);     // 0x005ED5A0
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CClientExoAppInternal {
public:
    void AddCreatureModelToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);
    void AddItemModelDirectlyToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList);

    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
};

// FUNCTION: SWKOTOR 0x005F97E0
void CClientExoAppInternal::AddCreatureModelToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
        CSWCCreature* pCreature = pObject->AsSWCCreature();
        if (pCreature) {
            CGob* pGob = pCreature->FindPart(0xff, 1);
            if (pGob) {
                pList->AddInline(pGob);
            }
            pGob = pCreature->FindPart(0xfe, 1);
            if (pGob) {
                pList->AddInline(pGob);
            }
            CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->GetEquippedItemID(1));
            if (pItem) {
                pGob = pItem->FindPart(0xff, 1);
                if (pGob) {
                    pList->Add(pGob);
                }
            }
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->GetEquippedItemID(0x10));
            if (pItem) {
                for (unsigned char nPart = 0xff; nPart >= 0xfb; nPart--) {
                    pGob = pItem->FindPart(nPart, 1);
                    if (pGob) {
                        pList->AddInline(pGob);
                    }
                }
            }
            pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->GetEquippedItemID(0x20));
            if (pItem) {
                for (unsigned char nPart = 0xff; nPart >= 0xfb; nPart--) {
                    pGob = pItem->FindPart(nPart, 1);
                    if (pGob) {
                        pList->AddInline(pGob);
                    }
                }
            }
        }
    }
}

// Moved from client_exo_app_internal.cpp: FindPart takes the part number as
// an unsigned char, which gives the original `or bl, 0xff` counter.
// FUNCTION: SWKOTOR 0x005F99F0
void CClientExoAppInternal::AddItemModelDirectlyToIgnoreList(unsigned long oid, CExoArrayList<CGob*>* pList)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID && pObject &&
        pObject->m_nObjectType == OBJECT_TYPE_ITEM) {
        CItemModel* pModel = pObject->GetItemModel();
        if (pModel) {
            for (unsigned char nPart = 0xff; nPart >= 0xfb; nPart--) {
                CGob* pGob = pModel->FindPart(nPart, 1);
                if (pGob) {
                    pList->AddInline(pGob);
                }
            }
        }
    }
}
