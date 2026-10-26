// CClientAIList and CClientAIMaster (b005f3_client_internal_lists).
// The AI list stores object IDs; each client object keeps its own list
// position at +0x1c so it can be removed in constant time.

#include "c/client_exo_app.h"

template <class T>
class CExoLinkedListS2 {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListS2();                             // 0x004B25D0 (folded)
    ~CExoLinkedListS2() { delete m_pcExoLinkedListInternal; }

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T GetAtPos(CExoLinkedListPosition pos) { return (T)m_pcExoLinkedListInternal->GetAtPos(pos); }
    void GetNext(CExoLinkedListPosition& pos) { m_pcExoLinkedListInternal->GetNext(pos); }
    void Remove(CExoLinkedListPosition pos) { m_pcExoLinkedListInternal->Remove(pos); }
    CExoLinkedListPosition AddTail(T t) { return m_pcExoLinkedListInternal->AddTail((void*)t); }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == 0; }
};

static inline CGameObject* ClientGetGameObject(unsigned long oid)
{
    CClientExoAppInternal* pInternal = g_pAppManager->m_pClientExoApp->m_pcInternal;
    return pInternal->GetGameObject(oid);
}

// Client object's position in the AI list (+0x1c).
static inline CExoLinkedListPosition& AIListPosition(CSWCObject* pObject)
{
    return *(CExoLinkedListPosition*)((char*)pObject + 0x1c);
}

class CClientAIList {
public:
    CClientAIList() { m_pPosition = 0; m_nCount = 0; }
    ~CClientAIList();                               // 0x00604A40
    int AddObject(CSWCObject* pObject);             // 0x006047A0
    int RemoveObject(CSWCObject* pObject);          // 0x00604810
    CSWCObject* GetNextObject();                    // 0x00604870

    int m_nCount;                                   // 0x00
    CExoLinkedListS2<unsigned long> m_lstActions;   // 0x04
    CExoLinkedListPosition m_pPosition;             // 0x08
};

class CClientAIMaster {
public:
    CClientAIMaster();                              // 0x00604B20
    void UpdateState();                             // 0x006048C0

    int m_nUnknown00;                               // 0x00
    CClientAIList m_lstActions;                     // 0x04
};

// Profiling counters for the client AI update.
extern unsigned __int64 g_nClientAIEndTime;         // 0x00832C58
extern unsigned __int64 g_nClientAIStartTime;       // 0x00832C60
extern float g_fClientModuleUpdateTime;             // 0x00832C68
extern float g_fClientAIUpdateTime;                 // 0x00832C6C

// FUNCTION: SWKOTOR 0x006047A0
int CClientAIList::AddObject(CSWCObject* pObject)
{
    CExoLinkedListPosition position = m_lstActions.m_pcExoLinkedListInternal->pHead;
    if (position != 0) {
        do {
            if (m_lstActions.m_pcExoLinkedListInternal->GetAtPos(position) == (void*)pObject->m_idSelf) {
                return 0;
            }
            m_lstActions.m_pcExoLinkedListInternal->GetNext(position);
        } while (position != 0);
    }
    m_nCount++;
    AIListPosition(pObject) = m_lstActions.AddTail(pObject->m_idSelf);
    if (m_pPosition == 0) {
        m_pPosition = AIListPosition(pObject);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00604810
int CClientAIList::RemoveObject(CSWCObject* pObject)
{
    CExoLinkedListPosition position = AIListPosition(pObject);
    if (position == 0) {
        return 1;
    }
    if (m_pPosition == position) {
        m_lstActions.m_pcExoLinkedListInternal->GetNext(m_pPosition);
        if (m_pPosition == 0) {
            m_pPosition = m_lstActions.m_pcExoLinkedListInternal->pHead;
            if (m_pPosition == position) {
                m_pPosition = 0;
            }
        }
    }
    m_nCount--;
    m_lstActions.m_pcExoLinkedListInternal->Remove(position);
    AIListPosition(pObject) = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x00604870
CSWCObject* CClientAIList::GetNextObject()
{
    if (m_lstActions.IsEmpty()) {
        return 0;
    }
    m_lstActions.GetNext(m_pPosition);
    if (m_pPosition == 0) {
        m_pPosition = m_lstActions.GetHeadPos();
    }
    unsigned long oid = m_lstActions.GetAtPos(m_pPosition);
    CGameObject* pObject = ClientGetGameObject(oid);
    if (pObject != 0) {
        return pObject->AsSWCObject();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00604A40
CClientAIList::~CClientAIList()
{
    while (m_lstActions.GetHeadPos() != 0) {
        CExoLinkedListPosition pos = m_lstActions.GetHeadPos();
        CExoLinkedListNode* pNode = (CExoLinkedListNode*)pos;
        unsigned long oid = pNode ? (unsigned long)pNode->pObject : 0;
        CClientExoAppInternal* pInternal = g_pAppManager->m_pClientExoApp->m_pcInternal;
        RemoveObject(pInternal->GetGameObject(oid)->AsSWCObject());
    }
}

// FUNCTION: SWKOTOR 0x00604B20
CClientAIMaster::CClientAIMaster()
{
}

// FUNCTION: SWKOTOR 0x006048C0
void CClientAIMaster::UpdateState()
{
    CSWCObject* pObject = m_lstActions.GetNextObject();
    int nCount = m_lstActions.m_nCount;
    g_nClientAIStartTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    if (pObject != 0 && nCount > 0) {
        do {
            if (pObject != 0) {
                pObject->v74();
            }
            pObject = m_lstActions.GetNextObject();
        } while (--nCount != 0);
    }
    g_nClientAIEndTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    g_fClientAIUpdateTime = (float)(int)(g_nClientAIEndTime - g_nClientAIStartTime) * 0.001f;
    g_nClientAIStartTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    pApp->GetModule()->Update((float)(__int64)pApp->m_pcInternal->m_pPauseTimer->GetSnapshotTimeDifference() * 0.000001f);
    g_nClientAIEndTime = g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    g_fClientModuleUpdateTime = (float)(int)(g_nClientAIEndTime - g_nClientAIStartTime) * 0.001f;
}
