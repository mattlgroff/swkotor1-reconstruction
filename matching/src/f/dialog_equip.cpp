// CSWSDialog: client-side creature presentation during a conversation (the
// creatures whose equipped items are hidden or that head-track the speaker),
// 0x005A0BA0..0x005A0DD0. Names are descriptive (no debug-map names).

#include "shared/res_ref.h"

#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



class CSWCCreature;

#include "shared/game_object.h"

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

#include "shared/app_manager.h"

#include "shared/globals.h"

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
