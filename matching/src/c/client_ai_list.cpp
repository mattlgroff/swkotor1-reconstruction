// CClientAIList (b005f3_client_internal_lists).

#include "c/client_exo_app.h"

class CClientAIList {
public:
    int AddObject(CSWCObject* pObject);             // 0x006047A0
    CSWCObject* GetNextObject();                    // 0x00604870
    unsigned long m_nCount;                         // 0x00
    CExoLinkedListInternal* m_lstActions;            // 0x04
    CExoLinkedListPosition m_pPosition;              // 0x08
};

// FUNCTION: SWKOTOR 0x006047A0
int CClientAIList::AddObject(CSWCObject* pObject)
{
    CExoLinkedListPosition position = m_lstActions->pHead;
    if (position != 0) {
        do {
            if (m_lstActions->GetAtPos(position) == (void*)pObject->m_idSelf) {
                return 0;
            }
            m_lstActions->GetNext(position);
        } while (position != 0);
    }
    m_nCount = m_nCount + 1;
    *(CExoLinkedListPosition*)((char*)pObject + 0x1c) = m_lstActions->AddTail((void*)pObject->m_idSelf);
    if (m_pPosition == 0) {
        m_pPosition = *(CExoLinkedListPosition*)((char*)pObject + 0x1c);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00604870
CSWCObject* CClientAIList::GetNextObject()
{
    if (m_lstActions->pHead == 0) {
        return 0;
    }
    m_lstActions->GetNext(m_pPosition);
    if (m_pPosition == 0) {
        m_pPosition = m_lstActions->pHead;
    }
    unsigned long oid = (unsigned long)m_lstActions->GetAtPos(m_pPosition);
    CAppManager* pManager = *(CAppManager**)0x007A39FC;
    CClientExoAppInternal* pInternal = pManager->m_pClientExoApp->m_pcInternal;
    CGameObject* pObject = pInternal->GetGameObject(oid);
    if (pObject != 0) {
        return pObject->AsSWCObject();
    }
    return 0;
}
