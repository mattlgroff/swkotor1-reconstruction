// CSWGuiQuickOrCustomPanel (swguiquickorcustompanel.o, layout "QORCPNL"):
// the quick / custom character choice with its scrolling description,
// 0x006F07D0-0x006F0EB7.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F07D0
void CSWGuiQuickOrCustomPanel::HandleCancelButton()
{
    if (m_pParent) {
        m_pParent->RemoveSubPanels();
    }
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
}

// FUNCTION: SWKOTOR 0x006F0800
void CSWGuiQuickOrCustomPanel::OnSelectQuickCharacterButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated() && m_pParent) {
        m_pParent->SetVisiblePanel(2);
    }
}

// FUNCTION: SWKOTOR 0x006F0830
void CSWGuiQuickOrCustomPanel::OnSelectCustomCharacterButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated() && m_pParent) {
        m_pParent->SetVisiblePanel(3);
    }
}

// The right-stick hint is shown only while the text overflows the list.
// FUNCTION: SWKOTOR 0x006F0860
void CSWGuiQuickOrCustomPanel::SetDescription(unsigned long nStrRef)
{
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
    m_lblDesc.m_cText.SetText(strRes.sString);
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDesc.m_extent;
    if (strRes.sString.GetLength() > 0) {
        extent.height = m_lblDesc.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDesc.m_cTextBox.GetLineHeight();
    }
    m_lblDesc.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDesc;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDesc, 0);
    m_lblRAnalog.m_bVisible = m_lbDesc.IsOversized();
}

// FUNCTION: SWKOTOR 0x006F09D0
void CSWGuiQuickOrCustomPanel::OnEnterQuickCharacterButton(CSWGuiControl* pControl)
{
    SetDescription(0xF1);
}

// FUNCTION: SWKOTOR 0x006F09E0
void CSWGuiQuickOrCustomPanel::OnEnterCustomCharacterButton(CSWGuiControl* pControl)
{
    SetDescription(0xF2);
}

// FUNCTION: SWKOTOR 0x006F0E10
void CSWGuiQuickOrCustomPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            break;
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            HandleCancelButton();
            break;
        case 0x3A:
            m_lbDesc.HandleInputEvent(0x32, 1);
            break;
        case 0x39:
            m_lbDesc.HandleInputEvent(0x31, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}
