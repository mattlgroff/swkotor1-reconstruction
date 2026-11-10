// CSWGuiSkillsCharGen (swguiskillschargen.o, layout "SKCHRGEN"): spends the
// skill points of a new character or a level-up, 0x006F47E0-0x006F6A60.
#include "o/sw_gui_skills_chargen.h"

// FUNCTION: SWKOTOR 0x006F47E0
CSWGuiSkillsCharGen::~CSWGuiSkillsCharGen()
{
    m_lbDesc.ClearControls();
}

// FUNCTION: SWKOTOR 0x006F49A0
void CSWGuiSkillsCharGen::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x11, 0x7f000000, 0x7f000000, 0);
}

// The custom character's ranks are reset before going back.
// FUNCTION: SWKOTOR 0x006F49D0
void CSWGuiSkillsCharGen::HandleCancelButton()
{
    if (m_pParent) {
        if (!m_bLevelUp) {
            for (int i = 0; i < 8; i++) {
                GetStats()->SetSkillRankBase(i, 0);
                GetStats()->SetModifiedSkillRank(i, 0);
            }
            GetStats()->m_nSkillPoints = 0;
        }
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_bLevelUp) {
            ((CSWGuiLevelUpPanel*)m_pParent)->CancelledByChild();
        } else {
            ((CSWGuiCustomPanel*)m_pParent)->CancelledByChild();
        }
    }
}

// FUNCTION: SWKOTOR 0x006F4A80
void CSWGuiSkillsCharGen::OnExitPointsButton(CSWGuiControl* pControl)
{
    m_aLabels[pControl->m_n58].m_cText.SetHighlighted(0);
    m_aPoints[pControl->m_n58].m_cTextParams.SetHighlighted(0);
}

// FUNCTION: SWKOTOR 0x006F4AB0
void CSWGuiSkillsCharGen::SetDescription(const CExoString& sText)
{
    m_lblDescItem.m_cText.SetText(sText);
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDescItem.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_lblDescItem.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescItem.m_cTextBox.GetLineHeight();
    }
    m_lblDescItem.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDescItem;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDescItem, 0);
}

// NONMATCH: the original keeps a dead `movzx eax, al` of the class id.
// FUNCTION: SWKOTOR 0x006F4B60
int CSWGuiSkillsCharGen::IsClassSkill(unsigned short nSkill)
{
    for (int i = 0; i < GetStats()->m_nNumClasses; i++) {
        unsigned char nClass = GetStats()->GetClass(i);
        if (g_pRules->m_pClasses[nClass].GetClassSkillValue(nSkill)) {
            return 1;
        }
    }
    return 0;
}

// NONMATCH: register picks (the original keeps the flag in ECX and the
// index in EBX).
// FUNCTION: SWKOTOR 0x006F4BF0
void CSWGuiSkillsCharGen::OnEnterPointsButton(CSWGuiControl* pControl)
{
    for (int i = 0; i < 8; i++) {
        int bGrey = 0;
        if (!m_bAllSkills && IsUnusable(i)) {
            bGrey = 1;
        }
        if (pControl == &m_aPoints[i]) {
            m_nCurrent = i;
            if (bGrey) {
                m_aPoints[i].m_cTextParams.SetColor(g_vGuiDisabledTextColor);
                m_aLabels[i].m_cText.SetColor(g_vGuiDisabledTextColor);
            } else {
                m_aPoints[i].m_cTextParams.SetColor(g_vAbilityColorGranted);
                m_aLabels[i].m_cText.SetColor(g_vAbilityColorGranted);
            }
        } else {
            if (bGrey) {
                m_aPoints[i].m_cTextParams.SetColor(g_vGuiDisabledTextColor);
                m_aLabels[i].m_cText.SetColor(g_vGuiDisabledTextColor);
            } else {
                m_aPoints[i].m_cTextParams.SetColor(g_vDifficultyExitColor);
                m_aLabels[i].m_cText.SetColor(g_vDifficultyExitColor);
            }
        }
    }
    if (IsClassSkill(m_nCurrent)) {
        m_lblClassSkill.m_cText.SetStrRef(0x9508);
    } else {
        m_lblClassSkill.m_cText.SetStrRef(0x9509);
    }
    CExoString sCost(GetStats()->GetSkillPointCostForSkillRank(m_nCurrent, 1));
    m_lblCostPoints.m_cText.SetText(sCost);
    SetDescription(m_aDescriptions[m_nCurrent]);
    m_aLabels[m_nCurrent].m_cText.SetHighlighted(1);
    m_aPoints[m_nCurrent].m_cTextParams.SetHighlighted(1);
}

// Message box callback of OnAcceptButton.
// FUNCTION: SWKOTOR 0x006F4DC0
void CSWGuiSkillsCharGen::ConfirmAccept(CSWGuiControl* pControl)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_bLevelUp) {
            ((CSWGuiLevelUpPanel*)m_pParent)->SkillSelectionCompleted();
        } else {
            ((CSWGuiCustomPanel*)m_pParent)->SkillCreationCompleted();
        }
    }
}

// With points left a message box asks for confirmation.
// FUNCTION: SWKOTOR 0x006F6980
void CSWGuiSkillsCharGen::OnAcceptButton()
{
    if (m_nPoints > 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(1);
        pBox->SetMessageStrRef(0xa357);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiSkillsCharGen::ConfirmAccept, 0);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    m_pManager->PopModalPanel();
    m_pParent->SetVisible(1);
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
    if (m_bLevelUp) {
        ((CSWGuiLevelUpPanel*)m_pParent)->SkillSelectionCompleted();
    } else {
        ((CSWGuiCustomPanel*)m_pParent)->SkillCreationCompleted();
    }
}

// FUNCTION: SWKOTOR 0x006F6A10
void CSWGuiSkillsCharGen::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            HandleCancelButton();
            break;
        case 0x2F:
        case 0x3F:
            m_pManager->PlayGuiSound(1);
            OnMinusButton();
            break;
        case 0x30:
        case 0x40:
            m_pManager->PlayGuiSound(1);
            OnPlusButton();
            break;
        case 0x27:
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            OnAcceptButton();
            break;
        case 0x2A:
            m_pManager->PlayGuiSound(0);
            OnRecommendButton();
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
