// CSWGuiPartySelection (vtable 0x00756D28): choosing the two party members.
// Member buttons are CSWGuiPartySelectionSlot. Destructors are in
// sw_gui_panel_dtors_novtable.cpp.
#include "l/sw_gui_party_selection.h"

// FUNCTION: SWKOTOR 0x006BE310
void CSWGuiPartySelection::SetForcedNPC(int nForced1, int nForced2)
{
    if (nForced1 >= 0 && nForced1 < 9) {
        m_bForced = 1;
        m_aSlots[nForced1].m_nSlotFlags |= 4;
    }
    if (nForced2 >= 0 && nForced2 < 9) {
        m_bForced = 1;
        m_aSlots[nForced2].m_nSlotFlags |= 4;
    }
}

// FUNCTION: SWKOTOR 0x006BE370
void CSWGuiPartySelectionSlot::SetSelected(int bSelected)
{
    m_bSelected = bSelected;
    if (bSelected) {
        m_cBorderParams.m_fAlpha = 1.0f;
        m_cBorderParams.SetColor(g_vAbilityColorLocked);
        m_cHilightParams.SetColor(g_vAbilityColorLocked);
        m_cHilightParams.SetFlags(1, 1, 0);
    } else {
        m_cBorderParams.m_fAlpha = 0.0f;
        m_cHilightParams.SetColor(g_vAbilityColorGranted);
        m_cHilightParams.SetFlags(1, 1, 0);
    }
}

// FUNCTION: SWKOTOR 0x006BE490
void CSWGuiPartySelection::OnPanelRemoved()
{
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    for (int i = 0; i < 9; i++) {
        if (!m_aSlots[i].m_bMember) {
            pTable->KillNPCObject(i, 1);
        }
    }
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006BE4E0
void CSWGuiPartySelection::UpdateCount()
{
    CExoString sCount;
    sCount.Format("%d", 2 - m_nSelected);
    m_lblCount.m_cText.SetText(sCount);
}

// FUNCTION: SWKOTOR 0x006BEC90
void CSWGuiPartySelection::ConfirmPartySelection(CSWGuiControl* pControl)
{
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        AcceptParty();
    }
}

// FUNCTION: SWKOTOR 0x006BECC0
void CSWGuiPartySelection::SetAddRemoveLabel(int bAdd)
{
    if (bAdd) {
        m_aLabels[2].m_cText.SetStrRef(0x9637);
        m_btnAccept.m_cTextParams.SetStrRef(0x9637);
        if (m_nSelected >= 2) {
            m_btnAccept.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
            m_btnAccept.m_cBorderParams.SetColor(g_vKeyMapLockedColor);
            return;
        }
    } else {
        m_aLabels[2].m_cText.SetStrRef(0x9638);
        m_btnAccept.m_cTextParams.SetStrRef(0x9638);
    }
    m_btnAccept.m_cTextParams.SetColor(g_vDifficultyExitColor);
    m_btnAccept.m_cBorderParams.SetColor(g_vUpgradeAvailableColor);
}

// FUNCTION: SWKOTOR 0x006BEDE0
void CSWGuiPartySelection::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || nEvent == 0x2e)) {
        m_pManager->PlayGuiSound(0);
        if (!m_bFromScript && !m_bForced) {
            for (int i = 0; i < 9; i++) {
                m_aSlots[i].SetSelected(m_aSlots[i].m_nPartyIndex != -1);
            }
            AcceptParty();
        } else {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(0);
            pBox->SetCloseCallback(0, 0, 0);
            pBox->SetMessageStrRef(0xa5a5);
            m_pManager->AddPanel(pBox, 1, 1);
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006BF2A0
void CSWGuiPartySelection::OnToggled(CSWGuiControl* pControl)
{
    if (!pControl->IsActivated()) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(pControl->m_nSound);
    }
    CSWGuiPartySelectionSlot* pSlot;
    if (pControl == &m_btnAccept) {
        pSlot = m_pEntered;
        if (!pSlot) {
            return;
        }
        pControl = pSlot;
    } else {
        pSlot = (CSWGuiPartySelectionSlot*)pControl->m_n58;
    }
    if (pSlot->m_bAvailable) {
        CSWGuiPartySelectionSlot* pButton = (CSWGuiPartySelectionSlot*)pControl;
        if (pButton->m_bSelected) {
            m_nSelected--;
            pButton->SetSelected(0);
        } else if (m_nSelected < 2) {
            m_nSelected++;
            pButton->SetSelected(1);
        }
        SetAddRemoveLabel(!pButton->m_bSelected);
        UpdateCount();
        return;
    }
    CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(0);
    pBox->SetCloseCallback(0, 0, 0);
    if (pSlot->m_bForced) {
        pBox->SetMessageStrRef(0xa5a6);
    } else {
        pBox->SetMessageStrRef(0xa588);
    }
    m_pManager->AddPanel(pBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x006BF3B0
void CSWGuiPartySelection::OnDone(CSWGuiControl* pControl)
{
    if (!pControl->IsActivated()) {
        return;
    }
    if (pControl->m_n48 == 0x2d) {
        m_pManager->PlayGuiSound(0);
    }
    if (m_bFromScript) {
        unsigned char nSelected = 0;
        unsigned char nForced = 0;
        unsigned char nAvailable = 0;
        for (int i = 0; i < 9; i++) {
            if (m_aSlots[i].m_bAvailable) {
                nAvailable++;
            }
            if (m_aSlots[i].m_bForced) {
                nForced++;
            }
            if (m_aSlots[i].m_bSelected) {
                nSelected++;
            }
        }
        if (nAvailable == 0 || nForced == 2) {
            AcceptParty();
            g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bPartySelectionOpen = 0;
            return;
        }
        if (nAvailable > 0) {
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(1);
            if (nAvailable > 1 && nSelected < 2) {
                pBox->SetMessageStrRef(0x95b9);
            } else {
                pBox->SetMessageStrRef(0x95b8);
            }
            pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::ConfirmPartySelection, 0);
            m_pManager->AddPanel(pBox, 1, 1);
        }
    } else {
        AcceptParty();
    }
}
