// CSWGuiNameChargen (swguinamechargen.o, layout "NAME"): the first-name
// entry with a random name button, opened by the quick and custom
// character panels, 0x006F9B80-0x006FA290.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F9B80
void CSWGuiNameChargen::SetParentPanel(CSWGuiPanel* pParent, unsigned char nType)
{
    m_pParent = pParent;
    m_nParentType = nType;
}

// FUNCTION: SWKOTOR 0x006F9BA0
void CSWGuiNameChargen::HandleCancelButton()
{
    if (m_pParent) {
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_nParentType == 1) {
            ((CSWGuiQuickPanel*)m_pParent)->CancelledByChild();
        } else if (m_nParentType == 2) {
            ((CSWGuiCustomPanel*)m_pParent)->CancelledByChild();
        }
    }
}

// NONMATCH: register picks for the stats pointer and the race argument.
// FUNCTION: SWKOTOR 0x006F9BF0
void CSWGuiNameChargen::HandleRandomName()
{
    CExoString sName;
    if (m_pCreature->m_pStats->m_nGender == 1) {
        sName = m_cNameGen.GetRandomName(RACIAL_TYPE_HUMAN, 6, 15);
    } else {
        sName = m_cNameGen.GetRandomName(RACIAL_TYPE_HUMAN, 5, 15);
    }
    m_editName.m_cText.SetText(sName);
    m_bRandomName = 1;
}

// Trailing spaces are trimmed; an empty name shows a message box instead.
// NONMATCH: the original keeps this in EDI and the message box in ESI.
// FUNCTION: SWKOTOR 0x006F9CD0
void CSWGuiNameChargen::HandleDoneButton(CSWGuiControl* pControl)
{
    CExoString sName;
    sName = m_editName.m_cText.m_sText;
    if (!sName.GetLength()) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetCloseCallback(0, 0, 0);
        pBox->SetMessageStrRef(0xA578);
        m_pManager->AddPanel(pBox, 1, 1);
    } else {
        while (sName[sName.GetLength() - 1] == ' ') {
            sName = sName.Left(sName.GetLength() - 1);
        }
        {
            CExoString sLastName("");
            m_pCreature->SetFirstAndLastName(sName, sLastName);
        }
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_nParentType == 1) {
            ((CSWGuiQuickPanel*)m_pParent)->NameCreationCompleted();
        } else if (m_nParentType == 2) {
            ((CSWGuiCustomPanel*)m_pParent)->NameCreationCompleted();
        }
    }
}

// FUNCTION: SWKOTOR 0x006FA140
void CSWGuiNameChargen::OnPanelAdded()
{
    CExoString sName;
    m_editName.SetFocus();
    CSWGuiPanel::OnPanelAdded();
    if (m_nParentType == 1) {
        sName = ((CSWGuiQuickPanel*)m_pParent)->GetCurrentName();
    } else if (m_nParentType == 2) {
        sName = ((CSWGuiCustomPanel*)m_pParent)->GetCurrentName();
    }
    if (!sName.GetLength()) {
        HandleRandomName();
    } else {
        m_editName.m_cText.SetText(sName);
    }
}

// FUNCTION: SWKOTOR 0x006FA220
void CSWGuiNameChargen::HandleInputEvent(int nEvent, int bDown)
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
        case 0x2A:
            m_pManager->PlayGuiSound(0);
            HandleRandomName();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}
