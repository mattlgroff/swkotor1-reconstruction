// CSWGuiAbilitiesCharGen (swguiabilitieschargen.o, layout "ABCHRGEN"):
// spends the ability points of a new character or a level-up,
// 0x006F6B60-0x006F89D0.
#include "o/sw_gui_abilities_chargen.h"

// FUNCTION: SWKOTOR 0x006F6B60
int CSWGuiAbilitiesCharGen::GetAbilityPointRefund(int nScore)
{
    if (m_bLevelUp) {
        return -1;
    }
    if (nScore <= 14 && nScore > 8) {
        return -1;
    }
    if (nScore >= 15 && nScore <= 16) {
        return -2;
    }
    if (nScore >= 17) {
        return -3;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006F6BB0
int CSWGuiAbilitiesCharGen::GetAbilityPointCost(int nScore)
{
    if (m_bLevelUp) {
        return 1;
    }
    if (nScore > 15) {
        return 3;
    }
    if (nScore > 13) {
        return 2;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006F6BF0
CSWGuiAbilitiesCharGen::~CSWGuiAbilitiesCharGen()
{
    m_lbDesc.ClearControls();
}

// FUNCTION: SWKOTOR 0x006F6DB0
void CSWGuiAbilitiesCharGen::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x10, 0x7f000000, 0x7f000000, 0);
}

// The custom character gets its 30 points back before going back.
// FUNCTION: SWKOTOR 0x006F6DE0
void CSWGuiAbilitiesCharGen::HandleCancelButton()
{
    if (m_pParent) {
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        if (m_bLevelUp) {
            ((CSWGuiLevelUpPanel*)m_pParent)->CancelledByChild();
        } else {
            GetStats()->m_nAbilityPoints = 30;
            ((CSWGuiCustomPanel*)m_pParent)->CancelledByChild();
        }
    }
}

// FUNCTION: SWKOTOR 0x006F6E40
void CSWGuiAbilitiesCharGen::OnExitPointsButton(CSWGuiControl* pControl)
{
    m_aLabels[pControl->m_n58].m_cText.SetHighlighted(0);
    m_aPoints[pControl->m_n58].m_cTextParams.SetHighlighted(0);
}

// FUNCTION: SWKOTOR 0x006F6E70
void CSWGuiAbilitiesCharGen::CompletePanel()
{
    if (m_bLevelUp) {
        ((CSWGuiLevelUpPanel*)m_pParent)->NewAbilityScores(m_anScores[0], m_anScores[5], m_anScores[4],
                                                           m_anScores[3], m_anScores[2], m_anScores[1],
                                                           m_nPoints);
    } else {
        GetStats()->SetSTRBase(m_anScores[0]);
        GetStats()->SetCHABase(m_anScores[5]);
        GetStats()->SetINTBase(m_anScores[4]);
        GetStats()->SetWISBase(m_anScores[3]);
        GetStats()->SetCONBase(m_anScores[2]);
        GetStats()->SetDEXBase(m_anScores[1], 1);
        GetStats()->m_nAbilityPoints = 0;
    }
    m_pManager->PopModalPanel();
    m_pParent->SetVisible(1);
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
    if (m_bLevelUp) {
        ((CSWGuiLevelUpPanel*)m_pParent)->AbilitySelectionCompleted();
    } else {
        ((CSWGuiCustomPanel*)m_pParent)->AbilityCreationCompleted();
    }
}

// FUNCTION: SWKOTOR 0x006F6FA0
void CSWGuiAbilitiesCharGen::SetDescription(unsigned long nStrRef)
{
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
    m_lblDescItem.m_cText.SetText(strRes.sString);
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDescItem.m_extent;
    if (strRes.sString.GetLength() > 0) {
        extent.height = m_lblDescItem.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescItem.m_cTextBox.GetLineHeight();
    }
    m_lblDescItem.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDescItem;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDescItem, 0);
}

// FUNCTION: SWKOTOR 0x006F70E0
void CSWGuiAbilitiesCharGen::OnEnterPointsButton(CSWGuiControl* pControl)
{
    for (int i = 0; i < 6; i++) {
        if (pControl == &m_aPoints[i]) {
            m_nCurrent = i;
            m_aPoints[i].m_cTextParams.SetColor(g_vAbilityColorGranted);
            m_aLabels[i].m_cText.SetColor(g_vAbilityColorGranted);
        } else {
            m_aPoints[i].m_cTextParams.SetColor(g_vDifficultyExitColor);
            m_aLabels[i].m_cText.SetColor(g_vDifficultyExitColor);
        }
    }
    CExoString sCost(GetAbilityPointCost(m_anScores[m_nCurrent]));
    CExoString sMod;
    int nMod = GetStats()->GetAbilityMod(m_anScores[m_nCurrent]);
    if (nMod < 0) {
        CExoString sNumber(nMod);
        sMod = sNumber;
    } else if (nMod == 0) {
        sMod = "-";
    } else if (nMod > 0) {
        sMod = CExoString("+") + CExoString(nMod);
    }
    m_lblCostPoints.m_cText.SetText(sCost);
    m_lblAbilityMod.m_cText.SetText(sMod);
    SetDescription(m_anDescriptions[m_nCurrent]);
    m_aLabels[m_nCurrent].m_cText.SetHighlighted(1);
    m_aPoints[m_nCurrent].m_cTextParams.SetHighlighted(1);
}

// With points left a message box says so.
// FUNCTION: SWKOTOR 0x006F7330
void CSWGuiAbilitiesCharGen::OnAcceptButton()
{
    if (m_nPoints > 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xbc59);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    CompletePanel();
}

// Level-up: one point on the class's primary ability; otherwise the
// class's recommended scores.
// NONMATCH: this and the constant 0 swap ESI/EDI (and AL/DL for the
// class index); the rest is identical.
// FUNCTION: SWKOTOR 0x006F7390
void CSWGuiAbilitiesCharGen::OnRecommendButton()
{
    if (m_bLevelUp) {
        CSWClass* pClass = GetStats()->GetClassObject(GetStats()->m_nNumClasses - 1);
        switch (pClass->m_nPrimaryAbility) {
        case 0:
            m_anScores[0]++;
            break;
        case 3:
            m_anScores[4]++;
            break;
        case 5:
            m_anScores[5]++;
            break;
        case 4:
            m_anScores[3]++;
            break;
        case 1:
            m_anScores[1]++;
            break;
        case 2:
            m_anScores[2]++;
            break;
        }
    } else {
        CSWClass* pClass = GetStats()->GetClassObject(GetStats()->m_nNumClasses - 1);
        m_anScores[0] = pClass->m_nRecommendedSTR;
        m_anScores[1] = pClass->m_nRecommendedDEX;
        m_anScores[2] = pClass->m_nRecommendedCON;
        m_anScores[3] = pClass->m_nRecommendedWIS;
        m_anScores[4] = pClass->m_nRecommendedINT;
        m_anScores[5] = pClass->m_nRecommendedCHA;
    }
    m_nPoints = 0;
    {
        CExoString sPoints(m_nPoints);
        m_lblRemaining.m_cText.SetText(sPoints);
    }
    for (int i = 0; i < 6; i++) {
        CExoString sScore(m_anScores[i]);
        m_aPoints[i].m_cTextParams.SetText(sScore);
        if (m_bLevelUp) {
            m_aMinus[i].m_bVisible = m_anScores[i] != m_anBaseScores[i];
        } else {
            m_aMinus[i].m_bVisible = m_anScores[i] != 8;
            m_aPlus[i].m_bVisible = m_anScores[i] != 18;
        }
    }
    OnEnterPointsButton(&m_aPoints[m_nCurrent]);
}

// A score stays between 8 (or its level-up value) and 18.
// FUNCTION: SWKOTOR 0x006F8480
void CSWGuiAbilitiesCharGen::OnMinusButton()
{
    int nCurrent = m_nCurrent;
    int nScore = m_anScores[nCurrent];
    if (m_bLevelUp && nScore == m_anBaseScores[nCurrent]) {
        return;
    }
    if (!m_bLevelUp && nScore == 8) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xa4c4);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    if (nScore > 8) {
        m_nPoints -= GetAbilityPointRefund(nScore);
        m_anScores[nCurrent]--;
    }
    CExoString sScore(m_anScores[m_nCurrent]);
    m_aPoints[m_nCurrent].m_cTextParams.SetText(sScore);
    CExoString sPoints(m_nPoints);
    m_lblRemaining.m_cText.SetText(sPoints);
    OnEnterPointsButton(&m_aPoints[m_nCurrent]);
    if ((m_bLevelUp && m_anScores[m_nCurrent] == m_anBaseScores[m_nCurrent]) ||
        (!m_bLevelUp && m_anScores[m_nCurrent] == 8)) {
        HideControl(&m_aMinus[m_nCurrent]);
        m_aMinus[m_nCurrent].SetFlag0(0);
    }
    ShowControl(&m_aPlus[m_nCurrent]);
}

// FUNCTION: SWKOTOR 0x006F8670
void CSWGuiAbilitiesCharGen::OnPlusButton()
{
    if (m_nPoints <= 0) {
        return;
    }
    if (!m_bLevelUp && m_anScores[m_nCurrent] == 18) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetMessageStrRef(0xa4c5);
        pBox->SetCloseCallback(0, 0, 0);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    int& nScore = m_anScores[m_nCurrent];
    if (m_nPoints - GetAbilityPointCost(nScore) >= 0 && (m_bLevelUp || nScore < 18)) {
        m_nPoints -= GetAbilityPointCost(nScore);
        nScore++;
    }
    CExoString sScore(m_anScores[m_nCurrent]);
    m_aPoints[m_nCurrent].m_cTextParams.SetText(sScore);
    CExoString sPoints(m_nPoints);
    m_lblRemaining.m_cText.SetText(sPoints);
    OnEnterPointsButton(&m_aPoints[m_nCurrent]);
    if (!m_bLevelUp && m_anScores[m_nCurrent] == 18) {
        HideControl(&m_aPlus[m_nCurrent]);
        m_aPlus[m_nCurrent].SetFlag0(0);
    }
    ShowControl(&m_aMinus[m_nCurrent]);
}

// FUNCTION: SWKOTOR 0x006F8880
void CSWGuiAbilitiesCharGen::HandleInputEvent(int nEvent, int bDown)
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
            if (m_nPoints > 0) {
                OnRecommendButton();
            }
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
