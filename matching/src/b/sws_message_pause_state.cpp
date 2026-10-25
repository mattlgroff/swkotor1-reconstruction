// Convert server exclusions to client object IDs when forwarding pause state.
#include "b/sws_message.h"
#include "b/exo_linked_list.h"

// FUNCTION: SWKOTOR 0x0056C9F0
int CSWSMessage::SendServerToPlayerModule_SetPauseState(unsigned char nType, int nState)
{
    CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
    CExoLinkedList<unsigned long>* pClientExcluded = NULL;
    CExoLinkedList<unsigned long>* pExcluded =
        g_pAppManager->GetServerExoApp()->GetActiveExclusionList();
    if (pExcluded && pExcluded->GetHeadPos()) {
        pClientExcluded = new CExoLinkedList<unsigned long>;
        void* pPos = pExcluded->GetHeadPos();
        while (pPos) {
            unsigned long oidObject = (unsigned long)pExcluded->GetAtPos(pPos);
            unsigned long oidClientObject = pClient->ServerToClientObjectId(oidObject);
            pClientExcluded->m_pList->AddTail((void*)oidClientObject);
            pExcluded->GetNext(pPos);
        }
    }
    pClient->SetPauseState(nType, nState, pClientExcluded);
    return 1;
}
