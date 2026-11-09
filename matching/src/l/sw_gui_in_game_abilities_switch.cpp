// CSWGuiInGameAbilities: the party portrait buttons cycle through the
// available party members (left 0x006AC3E0, right 0x006AC660).
#include "l/sw_gui_in_game_abilities.h"

// FUNCTION: SWKOTOR 0x006AC3E0
void CSWGuiInGameAbilities::OnSwitchLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_btn3860.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_btn3A24.m_cBorderParams.SetFillImage(m_btn3860.m_cBorderParams.GetFillImage(), 0);
            m_btn3A24.m_cHilightParams.SetFillImage(m_btn3860.m_cHilightParams.GetFillImage(), 0);
            m_btn3A24.m_n58 = m_btn3860.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart - 1; i >= 0 && !bFound; i--) {
                if (i != (char)m_nPartyMember &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_btn3860.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btn3860.SetHilightFillImage(
                            ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btn3860.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nPartyMember != -1 && (char)m_btn3A24.m_n58 != -1) {
                    m_btn3860.SetFillImage(((CSWCCreatureVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_btn3860.SetHilightFillImage(
                        ((CSWCCreatureVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_btn3860.m_n58 = -1;
                } else {
                    for (i = 8; i > nStart && !bFound; i--) {
                    if (i != (char)m_nPartyMember &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_btn3860.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btn3860.SetHilightFillImage(
                                ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btn3860.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006AC660
void CSWGuiInGameAbilities::OnSwitchRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_btn3A24.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_btn3860.m_cBorderParams.SetFillImage(m_btn3A24.m_cBorderParams.GetFillImage(), 0);
            m_btn3860.m_cHilightParams.SetFillImage(m_btn3A24.m_cHilightParams.GetFillImage(), 0);
            m_btn3860.m_n58 = m_btn3A24.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart + 1; i < 9 && !bFound; i++) {
                if (i != (char)m_nPartyMember &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_btn3A24.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btn3A24.SetHilightFillImage(
                            ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btn3A24.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nPartyMember != -1) {
                    m_btn3A24.SetFillImage(((CSWCCreatureVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_btn3A24.SetHilightFillImage(
                        ((CSWCCreatureVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_btn3A24.m_n58 = -1;
                } else {
                    for (i = 0; i < nStart && !bFound; i++) {
                    if (i != (char)m_nPartyMember &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_btn3A24.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btn3A24.SetHilightFillImage(
                                ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btn3A24.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006AC8D0
void CSWGuiInGameAbilities::SetButtonColor(CSWGuiControl*)
{
    switch (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nAbilityMode) {
    case 2:
        m_btn2B90.m_cTextParams.SetColor(g_vAbilityColorGranted);
        break;
    case 1:
        m_btn2D54.m_cTextParams.SetColor(g_vAbilityColorGranted);
        break;
    case 0:
        m_btn2F18.m_cTextParams.SetColor(g_vAbilityColorGranted);
        break;
    }
}
