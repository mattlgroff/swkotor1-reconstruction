// CSWGuiMainInterface::Update (panel vtable slot 0x38): floaty text timers,
// the combat message fade and the toggle / minimap button states.
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x00686BA0
void CSWGuiMainInterface::Update(float fDelta)
{
    int i;
    for (i = m_lstControls.num - 1; i >= 0; i--) {
        CSWGuiControl* pControl = m_lstControls.element[i];
        if (pControl) {
            pControl->TimedUpdate(fDelta);
        }
    }
    if (m_fQueueTime != -1.0f) {
        fDelta = m_fQueueTime - fDelta;
        m_fQueueTime = fDelta;
        if (fDelta <= 0.0f) {
            m_fQueueTime = -1.0f;
            CClientExoApp* pApp = g_pAppManager->GetClientExoApp();
            int nIndex = pApp->GetParty()->IsPartyMember(pApp->GetPlayerCreatureId());
            if (nIndex >= 0) {
                pApp->GetParty()->SetCombatMessage(nIndex, 0xbc50);
            }
        }
        else {
            float fAlpha = 1.0f;
            fDelta = fDelta / m_fMessageDuration;
            if (fDelta < 0.5) {
                fAlpha = fDelta + fDelta;
            }
            m_lblDescription.m_cText.m_fAlpha = fAlpha;
        }
    }
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
    m_tglC360.m_bVisible = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_nNPCCount > 0;
    m_tglC360.m_bSelected = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_bSoloMode;
    m_tglC614.m_bVisible = pCreature ? pCreature->IsStealthCapable() : 0;
    m_tglC614.m_bSelected = pCreature ? pCreature->m_bStealthMode : 0;
    m_tglC0ac.m_bSelected = g_pAppManager->GetClientExoApp()->GetPausedByCombat();
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    int bShow = pOptions ? pOptions->ShowMinimap() : 1;
    m_btnMinimap.SetVisible(bShow);
    m_btnB1d0.SetVisible(bShow);
    m_btnB394.SetVisible(bShow);
    m_btnB558.SetVisible(bShow);
    m_btnB71c.SetVisible(bShow);
    m_btnB8e0.SetVisible(bShow);
    m_btnBaa4.SetVisible(bShow);
    m_btnBc68.SetVisible(bShow);
    m_lblBe2c.SetVisible(bShow);
}

// FUNCTION: SWKOTOR 0x0068A010
// NONMATCH: stack-slot tie: the original frames the locals as nIcons
// 0x10, bShown 0x14, posPending 0x18, creature spill 0x1c, pos 0x20 (ours
// nIcons, pos, bShown, posPending, creature), which also moves the outer
// list position from eax to ecx. Declaration order and loop shape tried.
void CSWGuiMainInterface::UpdateQueue()
{
    CSWSCreature* pCreature =
        g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter()->GetServerCreature();
    CSWSCombatRound* pRound = pCreature->m_pCombatRound;
    int bActive;
    if (pRound->m_bRoundStarted && pRound->GetCurrentAction()) {
        bActive = 1;
    }
    else {
        bActive = 0;
    }
    CExoString sUnused;
    CExoString sDash("-");
    CResRef cIcon;
    CResRef cAlt;
    CExoLinkedListPosition pos = pCreature->m_lstActions.GetHeadPos();
    int nIcons = 0;
    int bShown = 0;
    CExoLinkedListPosition posPending;
    while (pos) {
        if (nIcons >= 4) {
            break;
        }
        CSWSObjectActionNode* pNode = pCreature->m_lstActions.GetAtPos(pos);
        if (pNode) {
            if (((pNode->m_nActionId == 0x3f && bActive) || pNode->m_nActionId == 0xc ||
                 pNode->m_nActionId == 0xf || pNode->m_nActionId == 0x2e) && !bShown) {
                if (pRound->m_pScheduledAction) {
                    bShown = 1;
                    GetActionIcon(pRound, pNode, pRound->m_pScheduledAction, pos, cIcon, cAlt);
                    ShowActionIcon(&nIcons, cIcon, cAlt, bActive, 0);
                }
                else if (pNode->m_nActionId != 0x3f) {
                    GetActionIcon(pRound, pNode, 0, pos, cIcon, cAlt);
                    ShowActionIcon(&nIcons, cIcon, cAlt, bActive, 0);
                    bShown = 1;
                }
            }
            if (pNode->m_nActionId == 0x3f && pRound->m_pPendingActions) {
                posPending = pRound->m_pPendingActions->GetHeadPos();
                if (posPending) {
                    while (nIcons < 4 && posPending) {
                        CSWSCombatRoundAction* pAction = pRound->m_pPendingActions->GetAtPos(posPending);
                        GetActionIcon(pRound, pNode, pAction, pos, cIcon, cAlt);
                        ShowActionIcon(&nIcons, cIcon, cAlt, bActive, 0);
                        pRound->m_pPendingActions->GetNext(posPending);
                    }
                }
                else if (!bShown) {
                    CSWSCombatRoundAction* pAction = pRound->m_pScheduledAction;
                    GetActionIcon(pRound, pNode, pAction, pos, cIcon, cAlt);
                    ShowActionIcon(&nIcons, cIcon, cAlt, bActive, 0);
                }
            }
        }
        pCreature->m_lstActions.GetNext(pos);
    }
    for (int i = nIcons; i < 4; i++) {
        m_aActionIcons[i].SetVisible(0);
    }
}
