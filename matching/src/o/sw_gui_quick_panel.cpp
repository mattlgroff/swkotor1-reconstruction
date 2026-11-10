// CSWGuiQuickPanel (swguiquickpanel.o, layout "QUICKPNL"): cancel handling
// and input of the three-step quick character list, 0x006F0220-0x006F0380.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F0220
void CSWGuiQuickPanel::ConfirmCancel(CSWGuiControl* pControl)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        if (m_pParent) {
            m_pParent->SetVisiblePanel(1);
            m_pParent->ClearAbilities();
        }
        SetState(0);
    }
}

// FUNCTION: SWKOTOR 0x006F0260
void CSWGuiQuickPanel::OnPanelAdded()
{
    DoQuickStats();
    m_pParent->AbilitiesChanged();
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006F0280
void CSWGuiQuickPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            if (m_nState == 0) {
                if (m_pParent) {
                    m_pParent->SetVisiblePanel(1);
                    m_pParent->ClearAbilities();
                }
                SetState(0);
            } else {
                SetState(--m_nState);
            }
            break;
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006F0310
void CSWGuiQuickPanel::OnCancelPressed(CSWGuiControl* pControl)
{
    if (m_nState != 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(1);
        pBox->SetMessageStrRef(0xBD9D);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::ConfirmCancel, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    } else {
        if (m_pParent) {
            m_pParent->SetVisiblePanel(1);
            m_pParent->ClearAbilities();
        }
        SetState(0);
    }
}
