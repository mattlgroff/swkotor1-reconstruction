// CSWGuiCustomPanel (swguicustompanel.o, layout "CUSTPNL"): the six-step
// custom character creation list (portrait, abilities, skills, feats, name,
// play). CSWGuiMainCharGen::NameChanged is emitted in this object.
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EEF70
void CSWGuiMainCharGen::NameChanged()
{
    SetNameString(GetCreatureV()->GetDisplayName());
}

// FUNCTION: SWKOTOR 0x006EEFD0
void CSWGuiCustomPanel::SetState(int nState)
{
    m_nState = nState;
    if (nState == 0) {
        m_btnBack.SetEnabled(0);
        m_btnBack.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
    } else {
        m_btnBack.SetEnabled(1);
        m_btnBack.m_cTextParams.SetColor(g_vDifficultyExitColor);
    }
    for (int i = 0; i < 6; i++) {
        if (i == m_nState) {
            m_aHilights[i].SetFlag0(1);
            m_aNumbers[i].SetEnabled(1);
            m_aSteps[i].SetEnabled(1);
            m_aNumbers[i].m_cText.SetColor(g_vAbilityColorGranted);
            m_aSteps[i].m_cTextParams.SetColor(g_vAbilityColorGranted);
            m_aNumbers[i].m_cText.m_nFlags |= 0x40;
            m_aSteps[i].m_cTextParams.m_nFlags |= 0x40;
        } else {
            m_aHilights[i].SetFlag0(0);
            m_aNumbers[i].m_cText.m_nFlags &= ~0x40;
            m_aSteps[i].m_cTextParams.m_nFlags &= ~0x40;
            m_aNumbers[i].SetEnabled(0);
            m_aSteps[i].SetEnabled(0);
        }
    }
    SetActiveControl(&m_aSteps[m_nState], 0);
}

// FUNCTION: SWKOTOR 0x006EF120
void CSWGuiCustomPanel::OnSelectPortrait(CSWGuiControl* pControl)
{
    if (m_nState >= 0 && pControl->IsActivated()) {
        CSWGuiPortraitCharGen* pPanel = new CSWGuiPortraitCharGen(m_pManager, m_pCreature);
        pPanel->SetParentPanel(this, 2);
        if (m_pCreature->GetAnimBase()) {
            m_pCreature->GetAnimBase()->GetModel(0xff)->PlayAnimation("pause2", 1.0f, 0, 0);
            m_pCreature->GetAnimBase()->GetModel(0xfe)->PlayAnimation("pause2", 1.0f, 0, 0);
        }
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EF220
void CSWGuiCustomPanel::OnSelectPlay(CSWGuiControl* pControl)
{
    if (m_nState >= 5 && pControl->IsActivated()) {
        m_pParent->BeginPlayGame();
    }
}

// FUNCTION: SWKOTOR 0x006EF240
void CSWGuiCustomPanel::OnSelectName(CSWGuiControl* pControl)
{
    if (m_nState >= 4 && pControl->IsActivated()) {
        CSWGuiNameChargen* pPanel = new CSWGuiNameChargen(m_pManager, m_pCreature);
        pPanel->SetParentPanel(this, 2);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EF2D0
void CSWGuiCustomPanel::OnSelectFeats(CSWGuiControl* pControl)
{
    if (m_nState >= 3 && pControl->IsActivated()) {
        CSWGuiFeatsCharGen* pPanel = new CSWGuiFeatsCharGen(m_pManager, m_pCreature, 0, 0);
        pPanel->m_pLevelUpPanel = (CSWGuiLevelUpPanel*)this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EF360
void CSWGuiCustomPanel::OnSelectSkills(CSWGuiControl* pControl)
{
    if (m_nState >= 2 && pControl->IsActivated()) {
        CSWGuiSkillsCharGen* pPanel = new CSWGuiSkillsCharGen(m_pManager, m_pCreature, 0);
        pPanel->m_pLevelUpPanel = (CSWGuiLevelUpPanel*)this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EF3F0
void CSWGuiCustomPanel::OnSelectAbilities(CSWGuiControl* pControl)
{
    if (m_nState >= 1 && pControl->IsActivated()) {
        CSWGuiAbilitiesCharGen* pPanel = new CSWGuiAbilitiesCharGen(m_pManager, m_pCreature, 0);
        pPanel->m_pLevelUpPanel = (CSWGuiLevelUpPanel*)this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EF480
void CSWGuiCustomPanel::CancelledByChild()
{
    m_aSteps[m_nState].m_cTextParams.SetColor(g_vAbilityColorGranted);
    m_aSteps[m_nState].m_cTextParams.SetHighlighted(1);
}

// FUNCTION: SWKOTOR 0x006EF5B0
void CSWGuiCustomPanel::ConfirmCancel(CSWGuiControl* pControl)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        if (m_pParent) {
            m_pParent->SetVisiblePanel(1);
            if (m_pParent) {
                m_pParent->ClearAbilities();
            }
            m_pCreature->m_pStats->m_nAbilityPoints = 30;
        }
        SetState(0);
    }
}

// FUNCTION: SWKOTOR 0x006EF610
void CSWGuiCustomPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            if (m_nState == 0) {
                CancelPanel();
            } else {
                SetState(--m_nState);
                switch (m_nState) {
                case 1:
                    ClearAbilities();
                    break;
                }
            }
            break;
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006EF6A0
void CSWGuiCustomPanel::OnCancelPressed(CSWGuiControl* pControl)
{
    if (m_nState != 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(1);
        pBox->SetMessageStrRef(0xBD9D);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::ConfirmCancel, 0);
        m_pManager->AddPanel(pBox, 1, 1);
    } else {
        if (m_pParent) {
            m_pParent->SetVisiblePanel(1);
            if (m_pParent) {
                m_pParent->ClearAbilities();
            }
            m_pCreature->m_pStats->m_nAbilityPoints = 30;
        }
        SetState(0);
    }
}
