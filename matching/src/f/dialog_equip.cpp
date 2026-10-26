// CSWSDialog: client-side creature presentation during a conversation (the
// creatures whose equipped items are hidden or that head-track the speaker),
// 0x005A0BA0..0x005A0DD0. Names are descriptive (no debug-map names).

class CResRef {
public:
    CResRef();
    char m_resRef[16];
};

struct CExoLinkedListNode {
    CExoLinkedListNode *pPrev;
    CExoLinkedListNode *pNext;
    void *pObject;
};

class CExoLinkedListInternal {
public:
    void *GetAtPos(CExoLinkedListNode *pPos);
    void *GetNext(CExoLinkedListNode *&pPos);
    CExoLinkedListNode *AddHead(void *pObject);
    CExoLinkedListNode *m_pHead;
    CExoLinkedListNode *m_pTail;
    int m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListNode *GetHeadPos() { return m_pcExoLinkedListInternal->m_pHead; }
    T *GetAtPos(CExoLinkedListNode *pPos) { return (T *)m_pcExoLinkedListInternal->GetAtPos(pPos); }
    T *GetNext(CExoLinkedListNode *&pPos) { return (T *)m_pcExoLinkedListInternal->GetNext(pPos); }
    CExoLinkedListNode *AddHead(T *pObject) { return m_pcExoLinkedListInternal->AddHead(pObject); }
    CExoLinkedListInternal *m_pcExoLinkedListInternal;
};

class CSWCCreature;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual CSWCCreature *AsSWCCreature();   // +0x2c
    unsigned long m_idSelf;          // +0x4
};

class CSWCCreature : public CGameObject {
public:
    void HideEquippedItems(const CResRef &cLeft, const CResRef &cRight);
    void ShowEquippedItems();
    void StartHeadTracking();
    void StopHeadTracking();
};

class CClientExoApp {
public:
    unsigned long ServerToClientObjectId(unsigned long oid);
    CGameObject *GetGameObject(unsigned long oid);
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp *m_pClientExoApp;  // +0x4
    CClientExoApp *GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSDialog {
public:
    void HideCreatureEquippedItems(unsigned long oidCreature);
    void ShowHiddenEquippedItems();
    void StartCreatureHeadTracking(unsigned long oidCreature);
    void StopCreatureHeadTracking();

    char pad_0x0[0x114];
    CExoLinkedList<unsigned long> m_lHiddenItemCreatures;  // +0x114
    CExoLinkedList<unsigned long> m_lHeadTrackingCreatures; // +0x118
};

// FUNCTION: SWKOTOR 0x005A0BA0
void CSWSDialog::HideCreatureEquippedItems(unsigned long oidCreature)
{
    if (oidCreature == 0x7f000000)
        return;
    CGameObject *pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
        g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oidCreature));
    if (!pObject || !pObject->AsSWCCreature())
        return;
    CSWCCreature *pCreature = pObject->AsSWCCreature();
    CExoLinkedListNode *pPos = m_lHiddenItemCreatures.GetHeadPos();
    while (pPos) {
        if (*m_lHiddenItemCreatures.GetAtPos(pPos) == pCreature->m_idSelf)
            return;
        m_lHiddenItemCreatures.GetNext(pPos);
    }
    unsigned long *pId = new unsigned long;
    if (!pId)
        return;
    *pId = pCreature->m_idSelf;
    m_lHiddenItemCreatures.AddHead(pId);
    pCreature->HideEquippedItems(CResRef(), CResRef());
}

// FUNCTION: SWKOTOR 0x005A0C80
void CSWSDialog::ShowHiddenEquippedItems()
{
    CExoLinkedListNode *pPos = m_lHiddenItemCreatures.GetHeadPos();
    while (pPos) {
        unsigned long *pId = m_lHiddenItemCreatures.GetAtPos(pPos);
        if (pId) {
            CGameObject *pObject = g_pAppManager->GetClientExoApp()->GetGameObject(*pId);
            if (pObject && pObject->AsSWCCreature())
                pObject->AsSWCCreature()->ShowEquippedItems();
            delete pId;
        }
        m_lHiddenItemCreatures.GetNext(pPos);
    }
}

// FUNCTION: SWKOTOR 0x005A0D10
void CSWSDialog::StartCreatureHeadTracking(unsigned long oidCreature)
{
    if (oidCreature == 0x7f000000)
        return;
    CGameObject *pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
        g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oidCreature));
    if (!pObject || !pObject->AsSWCCreature())
        return;
    CSWCCreature *pCreature = pObject->AsSWCCreature();
    CExoLinkedListNode *pPos = m_lHeadTrackingCreatures.GetHeadPos();
    while (pPos) {
        if (*m_lHeadTrackingCreatures.GetAtPos(pPos) == pCreature->m_idSelf)
            return;
        m_lHeadTrackingCreatures.GetNext(pPos);
    }
    unsigned long *pId = new unsigned long;
    if (!pId)
        return;
    *pId = pCreature->m_idSelf;
    m_lHeadTrackingCreatures.AddHead(pId);
    pCreature->StartHeadTracking();
}

// FUNCTION: SWKOTOR 0x005A0DD0
void CSWSDialog::StopCreatureHeadTracking()
{
    CExoLinkedListNode *pPos = m_lHeadTrackingCreatures.GetHeadPos();
    while (pPos) {
        unsigned long *pId = m_lHeadTrackingCreatures.GetAtPos(pPos);
        if (pId) {
            CGameObject *pObject = g_pAppManager->GetClientExoApp()->GetGameObject(*pId);
            if (pObject && pObject->AsSWCCreature())
                pObject->AsSWCCreature()->StopHeadTracking();
            delete pId;
        }
        m_lHeadTrackingCreatures.GetNext(pPos);
    }
}
