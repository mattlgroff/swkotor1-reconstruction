// Callback leaves for the inferred CSWGuiLevelUpPanel (vtable 0x00759568).
// The parent at +0x68 and current step at +0x1CCC are documented in the
// original GUI reconstruction.
#include "l/sw_gui_levelup_chargen.h"

// FUNCTION: SWKOTOR 0x006EE0D0
void CSWGuiLevelUpPanel::NewAbilityScores(unsigned int strength, unsigned int charisma,
                                          unsigned int intelligence, unsigned int wisdom,
                                          unsigned int constitution, unsigned int dexterity,
                                          unsigned int points)
{
    m_pParent->NewAbilityScores(strength, charisma, intelligence, wisdom,
                                constitution, dexterity, points);
}

// FUNCTION: SWKOTOR 0x006EE580
void CSWGuiLevelUpPanel::CancelledByChild()
{
    --m_nState;
    ChangeState(0);
    m_pParent->SetVisible(1);
}

// FUNCTION: SWKOTOR 0x006EE5B0
void CSWGuiLevelUpPanel::AbilitySelectionCompleted()
{
    m_pParent->SetVisible(1);
    m_pParent->AbilitiesChanged();
    ChangeState(0);
}

// FUNCTION: SWKOTOR 0x006EE5D0
void CSWGuiLevelUpPanel::PowersSelectionCompleted()
{
    m_pParent->SetVisible(1);
    ChangeState(0);
}

// FUNCTION: SWKOTOR 0x006EE640
void CSWGuiLevelUpPanel::SetCanCancel(int bCanCancel)
{
    m_bCanCancel = bCanCancel;
    int bEnableBack;
    if (m_nState == m_nLastState && bCanCancel == 0) {
        bEnableBack = 0;
    } else {
        bEnableBack = 1;
    }
    m_btnBack.SetEnabled(bEnableBack);
    m_btnBack.m_cTextParams.SetColor(bEnableBack ? g_vDifficultyExitColor : g_vKeyMapLockedTextColor);
    m_btnCancel.SetVisibleBit(m_bCanCancel);
}

// FUNCTION: SWKOTOR 0x006EE6C0
void CSWGuiLevelUpPanel::OnPanelAdded()
{
    m_nState = -1;
    ChangeState(0);
    m_nLastState = m_nState;
    if (m_bCanCancel == 0) {
        m_btnBack.SetEnabled(0);
        m_btnBack.m_cTextParams.SetColor(*(Vector*)0x007A2408);
        m_btnCancel.m_nFlags &= ~2ul;
    }
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006EE720
void CSWGuiLevelUpPanel::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            ChangeState(1);
            break;
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006EE0E0
void CSWGuiLevelUpPanel::ChangeState(int bBack)
{
    if (bBack) {
        if (m_nState == m_nLastState) {
            if (m_bCanCancel == 0) {
                return;
            }
            m_pParent->CancelLevelUp();
            CSWGuiSkillInfo* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pSkillInfo;
            if (m_pManager->PanelExists(pBox)) {
                m_pManager->RemovePanel(pBox);
            }
            return;
        }
    }
    int nStep;
    if (bBack) {
        nStep = m_nState - 1;
    } else {
        nStep = m_nState + 1;
    }
    for (;;) {
        if (nStep < 0 || nStep >= 5 || m_aStepEnabled[nStep] == 1) {
            break;
        }
        if (bBack) {
            --nStep;
        } else {
            ++nStep;
        }
    }
    if (nStep >= 0 && nStep < 5) {
        switch (nStep) {
        case 3:
            m_pParent->ClearPowers();
            break;
        case 0:
            m_pParent->ClearAbilities();
            break;
        case 1:
            m_pParent->ClearSkills();
            break;
        case 2:
            m_pParent->ClearFeats();
            break;
        }
        m_nState = nStep;
        for (int i = 0; i < 5; i++) {
            if (i == m_nState) {
                m_aNumbers[i].SetEnabled(1);
                m_aSteps[i].SetEnabled(1);
                m_aHilights[i].SetFlag0(1);
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
    int bEnableBack;
    if (m_nState == m_nLastState && m_bCanCancel == 0) {
        bEnableBack = 0;
    } else {
        bEnableBack = 1;
    }
    m_btnBack.SetEnabled(bEnableBack);
    m_btnBack.m_cTextParams.SetColor(bEnableBack ? g_vDifficultyExitColor : g_vKeyMapLockedTextColor);
}

// FUNCTION: SWKOTOR 0x006EE350
void CSWGuiLevelUpPanel::OnSelectPowersButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        CSWGuiPowersLevelUp* pPanel = new CSWGuiPowersLevelUp(m_pManager, m_pCreature->GetId());
        pPanel->m_pLevelUpPanel = this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EE3D0
void CSWGuiLevelUpPanel::OnSelectFeatsButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        unsigned char nNormal = 0;
        unsigned char nBonus = 0;
        GetLevelUpStats()->GetPackageFeatsLeft(&nNormal, &nBonus);
        CSWGuiFeatsCharGen* pPanel = new CSWGuiFeatsCharGen(m_pManager, m_pCreature, 1, nNormal == 0);
        pPanel->m_pLevelUpPanel = this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EE480
void CSWGuiLevelUpPanel::OnSelectSkillsButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        CSWGuiSkillsCharGen* pPanel = new CSWGuiSkillsCharGen(m_pManager, m_pCreature, 1);
        pPanel->m_pLevelUpPanel = this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EE500
void CSWGuiLevelUpPanel::OnSelectAbilitiesButton(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        CSWGuiPanel* pPanel = new CSWGuiAbilitiesCharGen(m_pManager, m_pCreature, 1);
        ((CSWGuiLevelUpChild*)pPanel)->m_pLevelUpPanel = this;
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EE5F0
void CSWGuiLevelUpPanel::OnCancelPressed(CSWGuiControl* pControl)
{
    if (m_bCanCancel == 0) {
        return;
    }
    m_pParent->ClearPowers();
    m_pParent->ClearFeats();
    m_pParent->ClearSkills();
    m_pParent->ClearAbilities();
    m_nState = 0;
    ChangeState(1);
}

// FUNCTION: SWKOTOR 0x006EE780
void CSWGuiLevelUpPanel::OnSelectAcceptButton(CSWGuiControl* pControl)
{
    if (pControl->m_n4c == 0) {
        return;
    }
    if (m_aStepEnabled[2] == 0) {
        ((CSWCLevelUpStats*)m_pCreature->m_pStats)->AddGrantedFeats(0);
        m_pParent->SetVisible(1);
        ChangeState(0);
    }
    m_pParent->BeginPlayGame();
}
