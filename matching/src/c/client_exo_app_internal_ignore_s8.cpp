// CClientExoAppInternal::SetHitcheckIgnoreList, worker C s8
// (clientexoappinternal.o; translation-unit-local layout).

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    void* GetNext(CExoLinkedListNode*& pos);        // 0x005E9B90
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListNode* GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListNode* pos)
    {
        void* p = pos;
        if (p) {
            p = pos->pObject;
        }
        return (T*)p;
    }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(CExoLinkedListNode*& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);                           // 0x005E03C0 family
    void Add(T t);                                  // 0x00444FE0 (out of line)
    void AddInline(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
};

class CGob;
class CSWCObject;
class CSWCCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CSWCObject : public CGameObject {
public:
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature* AsSWCCreature();          // 0x2c
    virtual void v30();
    virtual void* GetItemModel();                   // 0x34
    virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
};

struct CSWCCreatureItem21C {
    char pad_0x0[0x10];
    unsigned long m_oidItem;                        // 0x10
};

class CSWCCreature : public CSWCObject {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80

    char pad_0x4[0x21c - 0x4];
    CSWCCreatureItem21C* m_p21C;                    // 0x21c (descriptive)
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
    virtual CGob* FindPart(unsigned long nPart, int bRecurse);  // 0x98
};

class CSWItemBase {
public:
    virtual void v00();
    char pad_0x4[0x8];
    unsigned long m_nBaseItem;                      // 0x0c
};

class CSWCItem : public CSWItemBase, public CItemGob {
};

class CSWBaseItemArray {
public:
    void* GetBaseItem(int nBaseItem);               // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItemArray;             // 0x34
};

extern CSWRules* g_pRules;                          // 0x007A3A28

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

class CSWCArea {
public:
    char pad_0x0[0x204];
    CExoLinkedList<CGob> m_lstIgnoreModels;         // 0x204 (descriptive)
};

class CSWCModule {
public:
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

class CClientExoAppInternal {
public:
    void SetHitcheckIgnoreList(CExoArrayList<CGob*>* pList);

    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
    CSWCModule* m_pModule;                          // 0x18
    char pad_0x1c[0x190 - 0x1c];
    CExoLinkedList<unsigned long> m_lstHitcheckIgnore;  // 0x190
};

// Descriptive name. Collects the models of every hit-check-ignored object
// (and a creature's extra item, head and both weapons) plus the area's
// ignore-model list. The first and last adds are expanded inline in the
// original, the others call the out-of-line Add.
// NONMATCH (13 diff insns): argument order of GetGameObject (the original
// loads the array pointer before taking &pFound) and the register pair of
// the final inline Add.
// FUNCTION: SWKOTOR 0x005F94D0
void CClientExoAppInternal::SetHitcheckIgnoreList(CExoArrayList<CGob*>* pList)
{
    CExoLinkedListNode* pos = m_lstHitcheckIgnore.GetHeadPos();
    unsigned long* pId = m_lstHitcheckIgnore.GetHead();
    while (pos) {
        if (pId) {
            CGameObject* pFound = 0;
            if (m_pGameObjectArray->GetGameObject(*pId, &pFound) == GAME_OBJECT_VALID && pFound) {
                CSWCObject* pObject = pFound->AsSWCObject();
                if (pObject) {
                    CSWCCreature* pCreature = pObject->AsSWCCreature();
                    pObject->GetItemModel();
                    if (pObject->FindPart(0xff, 1)) {
                        pList->AddInline(pObject->FindPart(0xff, 1));
                    }
                    if (pCreature) {
                        if (pCreature->m_p21C) {
                            CSWCItem* pItem =
                                g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->m_p21C->m_oidItem);
                            if (pItem && pItem->FindPart(0xff, 1)) {
                                pList->Add(pItem->FindPart(0xff, 1));
                            }
                            if (pCreature->FindPart(0xfe, 1)) {
                                pList->Add(pCreature->FindPart(0xfe, 1));
                            }
                        }
                        CSWCItem* pItem =
                            g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->GetEquippedItemID(0x10));
                        if (pItem) {
                            g_pRules->m_pBaseItemArray->GetBaseItem(pItem->m_nBaseItem);
                            if (pItem->FindPart(0xff, 1)) {
                                pList->Add(pItem->FindPart(0xff, 1));
                            }
                        }
                        pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pCreature->GetEquippedItemID(0x20));
                        if (pItem && pItem->FindPart(0xff, 1)) {
                            pList->Add(pItem->FindPart(0xff, 1));
                        }
                    }
                }
            }
        }
        pId = m_lstHitcheckIgnore.GetNext(pos);
    }
    if (m_pModule && m_pModule->m_pArea) {
        CExoLinkedList<CGob>& lstModels = m_pModule->m_pArea->m_lstIgnoreModels;
        CExoLinkedListNode* posModel = lstModels.GetHeadPos();
        CGob* pGob = lstModels.GetHead();
        while (posModel) {
            if (pGob) {
                pList->AddInline(pGob);
            }
            pGob = lstModels.GetNext(posModel);
        }
    }
}
