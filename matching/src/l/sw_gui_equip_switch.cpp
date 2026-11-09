// CSWGuiInGameEquip: the party portrait buttons cycle through the available
// party members (left 0x006B60F0, right 0x006B6370).
#include "l/sw_gui_inventory.h"

// FUNCTION: SWKOTOR 0x006B60F0
void CSWGuiInGameEquip::OnSwitchLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_button3a20.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_button3be4.m_cBorderParams.SetFillImage(m_button3a20.m_cBorderParams.GetFillImage(), 0);
            m_button3be4.m_cHilightParams.SetFillImage(m_button3a20.m_cHilightParams.GetFillImage(), 0);
            m_button3be4.m_n58 = m_button3a20.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart - 1; i >= 0 && !bFound; i--) {
                if (i != (char)m_nSelected &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_button3a20.SetFillImage(((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                        m_button3a20.SetHilightFillImage(
                            ((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                        m_button3a20.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nSelected != -1 && (char)m_button3be4.m_n58 != -1) {
                    m_button3a20.SetFillImage(((CSWCCreaturePortraitVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_button3a20.SetHilightFillImage(
                        ((CSWCCreaturePortraitVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_button3a20.m_n58 = -1;
                } else {
                    for (i = 8; i > nStart && !bFound; i--) {
                    if (i != (char)m_nSelected &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_button3a20.SetFillImage(((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                            m_button3a20.SetHilightFillImage(
                                ((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                            m_button3a20.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006B6370
void CSWGuiInGameEquip::OnSwitchRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_button3be4.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_button3a20.m_cBorderParams.SetFillImage(m_button3be4.m_cBorderParams.GetFillImage(), 0);
            m_button3a20.m_cHilightParams.SetFillImage(m_button3be4.m_cHilightParams.GetFillImage(), 0);
            m_button3a20.m_n58 = m_button3be4.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart + 1; i < 9 && !bFound; i++) {
                if (i != (char)m_nSelected &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_button3be4.SetFillImage(((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                        m_button3be4.SetHilightFillImage(
                            ((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                        m_button3be4.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nSelected != -1) {
                    m_button3be4.SetFillImage(((CSWCCreaturePortraitVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_button3be4.SetHilightFillImage(
                        ((CSWCCreaturePortraitVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_button3be4.m_n58 = -1;
                } else {
                    for (i = 0; i < nStart && !bFound; i++) {
                    if (i != (char)m_nSelected &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_button3be4.SetFillImage(((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                            m_button3be4.SetHilightFillImage(
                                ((CSWCCreaturePortraitVirtuals*)pCreature)->GetPortrait(1));
                            m_button3be4.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}
