// CSWGuiInGameCharacter: the party portrait buttons select the shown
// party member (0x006AF350) and cycle through the available ones (left
// 0x006AF450, right 0x006AF6D0).
#include "l/sw_gui_in_game_character.h"

// FUNCTION: SWKOTOR 0x006AEC80
void CSWGuiInGameCharacter::DoAutoLevelup(CSWGuiControl*)
{
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        CSWSCreature* pCreature =
            g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter()->AsServerCreature();
        if (pCreature) {
            pCreature->m_pStats->AutoLevelUp(0);
        }
    }
}

// FUNCTION: SWKOTOR 0x006AF350
void CSWGuiInGameCharacter::OnChangeCharacter(CSWGuiControl* pControl)
{
    int nMember = 1;
    if (pControl->m_n4c) {
        if (g_pAppManager->m_pServerExoApp->GetServerInfo()->m_bPartyMode) {
            char nNPC = (char)pControl->m_n58;
            CSWCCreature* pCreature;
            if (nNPC == -1) {
                pCreature = g_pAppManager->m_pClientExoApp->GetPlayerCreature();
            } else {
                CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                    g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(nNPC, 0, 1)));
            }
            if (pCreature) {
                if (m_btnCharRight.m_bVisible) {
                    if (pControl == &m_btnChange1) {
                        m_btnChange1.m_n58 = m_btnChange2.m_n58;
                    }
                } else {
                    m_btnChange1.m_n58 = 0xfffffffe;
                }
                m_nPartyMember = nNPC;
            }
        } else {
            if (pControl == &m_btnChange2) {
                nMember = 2;
            }
            if (!g_pAppManager->m_pClientExoApp->ChangeCharacterToNextLivingPartyMember(0, 0, nMember)) {
                return;
            }
            m_pManager->PlayGuiSound(1);
        }
        m_pManager->PlayGuiSound(1);
        UpdatePortraits();
    }
}

// FUNCTION: SWKOTOR 0x006AF450
void CSWGuiInGameCharacter::OnSwitchLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_btnChange1.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_btnChange2.m_cBorderParams.SetFillImage(m_btnChange1.m_cBorderParams.GetFillImage(), 0);
            m_btnChange2.m_cHilightParams.SetFillImage(m_btnChange1.m_cHilightParams.GetFillImage(), 0);
            m_btnChange2.m_n58 = m_btnChange1.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart - 1; i >= 0 && !bFound; i--) {
                if (i != (char)m_nPartyMember &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_btnChange1.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btnChange1.SetHilightFillImage(
                            ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btnChange1.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nPartyMember != -1 && (char)m_btnChange2.m_n58 != -1) {
                    m_btnChange1.SetFillImage(((CSWCCreatureVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_btnChange1.SetHilightFillImage(
                        ((CSWCCreatureVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_btnChange1.m_n58 = -1;
                } else {
                    for (i = 8; i > nStart && !bFound; i--) {
                    if (i != (char)m_nPartyMember &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_btnChange1.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btnChange1.SetHilightFillImage(
                                ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btnChange1.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006AF6D0
void CSWGuiInGameCharacter::OnSwitchRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        char nCurrent = (char)m_btnChange2.m_n58;
        int bFound = 0;
        if (nCurrent != -2) {
            m_btnChange1.m_cBorderParams.SetFillImage(m_btnChange2.m_cBorderParams.GetFillImage(), 0);
            m_btnChange1.m_cHilightParams.SetFillImage(m_btnChange2.m_cHilightParams.GetFillImage(), 0);
            m_btnChange1.m_n58 = m_btnChange2.m_n58;
            int nStart = nCurrent;
            int i;
            for (i = nStart + 1; i < 9 && !bFound; i++) {
                if (i != (char)m_nPartyMember &&
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                    CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                    if (pCreature) {
                        m_btnChange2.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btnChange2.SetHilightFillImage(
                            ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                        m_btnChange2.m_n58 = i;
                        bFound = 1;
                    }
                }
            }
            if (!bFound) {
                if ((char)m_nPartyMember != -1) {
                    m_btnChange2.SetFillImage(((CSWCCreatureVirtuals*)g_pAppManager->m_pClientExoApp
                                                   ->GetPlayerCreature())
                                                  ->GetPortrait(1));
                    m_btnChange2.SetHilightFillImage(
                        ((CSWCCreatureVirtuals*)g_pAppManager->GetClientExoApp()->GetPlayerCreature())
                            ->GetPortrait(1));
                    m_btnChange2.m_n58 = -1;
                } else {
                    for (i = 0; i < nStart && !bFound; i++) {
                    if (i != (char)m_nPartyMember &&
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(i)) {
                        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
                        CSWCCreature* pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                            g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(i, 0, 1)));
                        if (pCreature) {
                            m_btnChange2.SetFillImage(((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btnChange2.SetHilightFillImage(
                                ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
                            m_btnChange2.m_n58 = i;
                            bFound = 1;
                        }
                    }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006AECD0
void CSWGuiInGameCharacter::SetLabelBonusPenaltyColour(CSWGuiLabel* pLabel, int nValue)
{
    if (nValue > 0) {
        pLabel->m_cText.SetColor(g_vAbilityColorLocked);
    } else if (nValue < 0) {
        pLabel->m_cText.SetColor(g_vAbilityColorAvailable);
    } else {
        pLabel->m_cText.SetColor(g_vDifficultyExitColor);
    }
}
