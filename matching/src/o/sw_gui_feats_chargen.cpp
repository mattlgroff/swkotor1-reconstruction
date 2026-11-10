// CSWGuiFeatsCharGen (swguifeatschargen.o, layout "FTCHRGEN"): picks the
// character's feats for the custom character panel and the level-up panel,
// 0x006F2A20-0x006F47D0.
#include "o/sw_gui_feats_chargen.h"

// FUNCTION: SWKOTOR 0x006F2A20 SYMBOL: ?IndexOf@?$CExoArrayListM@G@@QAEHG@Z
// Out-of-line copy of the inline IndexOf, called by DetermineFeat.

// FUNCTION: SWKOTOR 0x006F2A50
CSWGuiFeatsCharGen::~CSWGuiFeatsCharGen()
{
    m_lbDesc.ClearControls();
    m_lbFeats.ClearControls();
}

// FUNCTION: SWKOTOR 0x006F2C40
void CSWGuiFeatsCharGen::ShowGranted(CSWGuiControl* pControl)
{
    CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
    m_pManager->AddPanel((CSWGuiPanel*)pGui->m_pSkillInfo, 1, 1);
}

// FUNCTION: SWKOTOR 0x006F2C70
void CSWGuiFeatsCharGen::HandleCancelButton()
{
    if (m_pParent) {
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

// FUNCTION: SWKOTOR 0x006F2CC0
void CSWGuiFeatsCharGen::Draw(float fDelta)
{
    switch (m_nStatus) {
    case 2:
    case 3:
        m_btnSelect.m_cTextParams.SetText(m_sRemoveText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    case 4:
        m_btnSelect.m_cTextParams.SetText(m_sSelectText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006F2D30
void CSWGuiFeatsCharGen::DetermineFeat(unsigned short nFeat, unsigned char* pStatus)
{
    if (m_lstAvailable.IndexOf(nFeat) != -1) {
        *pStatus = 0;
        return;
    }
    if (m_lstChosen.IndexOf(nFeat) != -1) {
        *pStatus = 1;
        return;
    }
    if (m_lstKnown.IndexOf(nFeat) != -1) {
        *pStatus = 3;
        return;
    }
    if (m_lstGranted.IndexOf(nFeat) != -1) {
        *pStatus = 2;
    } else {
        *pStatus = 4;
    }
}

// FUNCTION: SWKOTOR 0x006F2DF0
void CSWGuiFeatsCharGen::SetDescription(unsigned long nStrRef)
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

// FUNCTION: SWKOTOR 0x006F2F30
void CSWGuiFeatsCharGen::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    if (m_bShowGranted) {
        if (g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x12, 0x7f000000, 0x7f000000, 0)) {
            CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
            ((CSWGuiPanel*)pGui->m_pTutorialBox)
                ->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiFeatsCharGen::ShowGranted, 0);
        } else {
            CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
            m_pManager->AddPanel((CSWGuiPanel*)pGui->m_pSkillInfo, 1, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x006F2FB0
void CSWGuiFeatsCharGen::OnEnterFeat(unsigned short nFeat)
{
    DetermineFeat(nFeat, &m_nStatus);
    m_nCurrentFeat = nFeat;
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    switch (m_nStatus) {
    case 0:
        m_btnSelect.m_cTextParams.SetText(m_sSelectText);
        m_btnSelect.m_cTextParams.SetColor(g_vDifficultyExitColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vUpgradeAvailableColor);
        break;
    case 1:
        m_btnSelect.m_cTextParams.SetText(m_sRemoveText);
        m_btnSelect.m_cTextParams.SetColor(g_vDifficultyExitColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vUpgradeAvailableColor);
        break;
    case 2:
    case 3:
        m_btnSelect.m_cTextParams.SetText(m_sRemoveText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    case 4:
        m_btnSelect.m_cTextParams.SetText(m_sSelectText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    }
    m_lblName.m_cText.SetStrRef(pFeat->m_nNameStrRef);
    SetDescription(pFeat->m_nDescriptionStrRef);
}

// FUNCTION: SWKOTOR 0x006F30F0
void CSWGuiFeatsCharGen::BuildButtons()
{
    CSWGuiAbilityList* pChart = &m_cChart;
    pChart->SetMode(3);
    int i;
    for (i = 0; i < m_lstKnown.num; i++) {
        pChart->SetStatus(m_lstKnown[i], 1);
    }
    for (i = 0; i < m_lstChosen.num; i++) {
        pChart->SetStatus(m_lstChosen[i], 4);
    }
    for (i = 0; i < m_lstAvailable.num; i++) {
        pChart->SetStatus(m_lstAvailable[i], 0);
    }
    for (i = 0; i < m_lstGranted.num; i++) {
        pChart->SetStatus(m_lstGranted[i], 2);
    }
    if (m_nCurrentFeat == 0xffff) {
        m_lbFeats.AddControls(*(CExoArrayList<CSWGuiControl*>*)pChart, 0, 1, 0);
        m_nCurrentFeat = pChart->Refresh();
    }
    pChart->SetSelected(m_nCurrentFeat);
    unsigned char nPage = m_cChart.m_nPage;
    m_lbFeats.SetSelectedControl(nPage, 0);
    OnEnterFeat(m_nCurrentFeat);
    CExoString sRemaining(m_nRemaining);
    m_lblRemaining.m_cText.SetText(sRemaining);
}

// NONMATCH: register picks (this in EDI, the original ESI) and the loop entry test.
// FUNCTION: SWKOTOR 0x006F3290
void CSWGuiFeatsCharGen::BuildAvailableList()
{
    CExoString sUnused;
    CSWCLevelUpStatsF* pStats = GetStats();
    for (unsigned short i = 0; i < g_pRules->m_nNumFeatIds; i++) {
        unsigned short nFeat = g_pRules->GetFeatId(i);
        if (m_lstKnown.IndexOf(nFeat) != -1) {
            continue;
        }
        if (m_lstGranted.IndexOf(nFeat) != -1) {
            continue;
        }
        if (m_lstChosen.IndexOf(nFeat) != -1) {
            continue;
        }
        if (m_lstAvailable.IndexOf(nFeat) != -1) {
            continue;
        }
        if (pStats->CanChooseFeat(nFeat, m_nNormalLeft, m_nBonusLeft, &m_lstChosen)) {
            m_lstAvailable.Add(nFeat);
        }
    }
}

// FUNCTION: SWKOTOR 0x006F3420
void CSWGuiFeatsCharGen::OnFeatSelectionChanged(CSWGuiControl* pControl)
{
    CSWGuiAbilityChain* pChain = (CSWGuiAbilityChain*)m_lbFeats.GetSelectedControl();
    int nFeat = pChain->HitTest(m_lbFeats.m_ptLocalMouse);
    m_cChart.SetSelected(nFeat);
    OnEnterFeat(nFeat);
}

// NONMATCH: the original tests the feat count with a dword load and keeps the
// redundant "granted count > 0" loop test; ours folds both.
// FUNCTION: SWKOTOR 0x006F3460
void CSWGuiFeatsCharGen::InitiateFeats()
{
    CSWCLevelUpStatsF* pStats = GetStats();
    if (!m_bLevelUp && !m_bBonusOnly) {
        pStats->ClearFeats();
    }
    for (int i = 0; i < pStats->GetNumFeats(); i++) {
        m_lstKnown.Add(pStats->GetFeat(i));
    }
    m_bShowGranted = 0;
    if (!m_bBonusOnly) {
        CExoArrayListM<int> lstGranted;
        pStats->AddGrantedFeats(&lstGranted);
        if (lstGranted.num > 0) {
            int j;
            for (j = 0; j < lstGranted.num; j++) {
                m_lstGranted.Add(lstGranted[j]);
            }
            for (j = lstGranted.num - 1; j >= 0; j--) {
                int nFound = 0;
                for (int k = 0; k < m_lstKnown.num; k++) {
                    if (m_lstKnown[k] == (unsigned short)lstGranted[j]) {
                        nFound++;
                    }
                }
                if (nFound) {
                    lstGranted.DelIndex(j);
                }
            }
            CSWGuiSkillInfo* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pSkillInfo;
            pBox->SetSkills(lstGranted, 1);
            pBox->m_lblMessage.m_cText.SetStrRef(0xa512);
            m_bShowGranted = 1;
        }
    }
    pStats->GetPackageFeatsLeft(&m_nNormalFeats, &m_nBonusFeats);
    m_nNormalLeft = m_nNormalFeats ? m_nNormalFeats + 1 : 0;
    m_nBonusLeft = m_nBonusFeats ? m_nBonusFeats + 1 : 0;
    if (m_bBonusOnly) {
        m_nNormalFeats = 0;
    } else {
        m_nBonusFeats = 0;
    }
    m_nRemaining = m_nNormalFeats + m_nBonusFeats;
    if (!m_nRemaining) {
        OnAcceptButton();
        return;
    }
    m_cChart.SetLevelUpFeats(pStats, m_bBonusOnly);
    BuildAvailableList();
    BuildButtons();
}

// Moves the feat to the chosen list, which is kept sorted by name.
// NONMATCH: the original keeps the name loop unrotated (test at the top).
// FUNCTION: SWKOTOR 0x006F37E0
void CSWGuiFeatsCharGen::AddChosenFeat(unsigned short nFeat)
{
    int nIndex = m_lstAvailable.IndexOf(nFeat);
    if (nIndex == -1) {
        return;
    }
    m_lstAvailable.DelIndex(nIndex);
    int bInsert = 0;
    CExoString sName = g_pRules->GetFeat(nFeat)->GetNameText();
    int i;
    for (i = 0; i < m_lstChosen.num; i++) {
        if (g_pRules->GetFeat(m_lstChosen[i])->GetNameText() > sName) {
            bInsert = 1;
            break;
        }
    }
    m_nRemaining--;
    if (bInsert) {
        m_lstChosen.Insert(nFeat, i);
    } else {
        m_lstChosen.Add(nFeat);
    }
    BuildAvailableList();
}

// Chosen feats whose prerequisites are gone are removed as well.
// The flag keeps the recursive call (no tail-recursion loop).
// FUNCTION: SWKOTOR 0x006F3990
void CSWGuiFeatsCharGen::RemoveChosenFeat(unsigned short nFeat)
{
    int nIndex = m_lstChosen.IndexOf(nFeat);
    if (nIndex == -1) {
        return;
    }
    m_lstChosen.DelIndex(nIndex);
    m_nRemaining++;
    CSWCLevelUpStatsF* pStats = GetStats();
    int bRemoved = 0;
    for (int i = 0; i < m_lstChosen.num; i++) {
        if (!pStats->HasFeatPrereq(g_pRules->GetFeat(m_lstChosen[i]), &m_lstChosen)) {
            bRemoved = 1;
            RemoveChosenFeat(m_lstChosen[i]);
            break;
        }
    }
    if (!bRemoved) {
        m_lstAvailable.num = 0;
        BuildAvailableList();
    }
}

// NONMATCH: register picks for the class argument (EAX vs EDX).
// FUNCTION: SWKOTOR 0x006F3A80
void CSWGuiFeatsCharGen::OnRecommendButton()
{
    CExoArrayListM<unsigned short> lstFeats;
    CExoArrayListM<int> lstShown;
    int nClass = GetStats()->GetClass(GetStats()->m_nNumClasses - 1);
    GetStats()->AddDefaultPackageFeats(nClass, &lstFeats, !m_bBonusOnly, m_bBonusOnly);
    while (m_lstChosen.num) {
        RemoveChosenFeat(m_lstChosen[0]);
    }
    lstShown.SetSize(lstFeats.num);
    for (int i = 0; i < lstFeats.num; i++) {
        AddChosenFeat(lstFeats[i]);
        lstShown[i] = lstFeats[i];
    }
    CSWGuiSkillInfo* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pSkillInfo;
    pBox->SetSkills(lstShown, 1);
    pBox->m_lblMessage.m_cText.SetStrRef(0xa510);
    m_pManager->AddPanel(pBox, 1, 1);
    BuildButtons();
}

// FUNCTION: SWKOTOR 0x006F3C20
void CSWGuiFeatsCharGen::OnFeatPicked(unsigned short nFeat)
{
    unsigned char nStatus;
    DetermineFeat(nFeat, &nStatus);
    unsigned long nStrRef;
    switch (nStatus) {
    case 0:
        if (m_nRemaining > 0) {
            AddChosenFeat(nFeat);
            BuildButtons();
            return;
        }
        nStrRef = 0xa622;
        break;
    case 1:
        RemoveChosenFeat(nFeat);
        BuildButtons();
        return;
    case 2:
        nStrRef = 0xa4c7;
        break;
    case 3:
        nStrRef = 0xa4c6;
        break;
    case 4:
        nStrRef = 0xa4c8;
        break;
    default:
        return;
    }
    CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(0);
    pBox->SetCloseCallback(0, 0, 0);
    pBox->SetMessageStrRef(nStrRef);
    m_pManager->AddPanel(pBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x006F3CF0
void CSWGuiFeatsCharGen::OnDoubleClick(CSWGuiControl* pControl)
{
    int nSelected;
    if (m_cChart.m_lstChains.num > 0) {
        nSelected = m_cChart.GetSelectedNode().m_nId;
    } else {
        nSelected = -1;
    }
    CSWGuiAbilityChain* pChain = (CSWGuiAbilityChain*)m_lbFeats.GetSelectedControl();
    int nFeat = pChain->HitTest(m_lbFeats.m_ptLocalMouse);
    if (nFeat == nSelected) {
        OnFeatPicked(nFeat);
    }
}

// With feats left to pick a message box asks for them; otherwise the
// chosen feats are added, and a bonus-feat picker follows when the class
// grants bonus feats.
// FUNCTION: SWKOTOR 0x006F44C0
void CSWGuiFeatsCharGen::OnAcceptButton()
{
    if (m_nRemaining > 0 && m_lstAvailable.num > 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetCloseCallback(0, 0, 0);
        pBox->SetMessageStrRef(0xbc57);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    for (int i = 0; i < m_lstChosen.num; i++) {
        GetStats()->AddFeat(m_lstChosen[i]);
    }
    m_pManager->PopModalPanel();
    m_pParent->SetVisible(1);
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
    m_lbFeats.ClearControls();
    m_cChart.Reset();
    unsigned char nNormal;
    unsigned char nBonus;
    GetStats()->GetPackageFeatsLeft(&nNormal, &nBonus);
    if (!m_bBonusOnly && nBonus > 0) {
        CSWGuiFeatsCharGen* pBonus = new CSWGuiFeatsCharGen(m_pManager, m_pCreature, m_bLevelUp, 1);
        pBonus->m_pParent = m_pParent;
        m_pManager->AddPanel(pBonus, 3, 1);
        return;
    }
    if (m_bLevelUp) {
        ((CSWGuiLevelUpPanel*)m_pParent)->FeatSelectionCompleted();
    } else {
        ((CSWGuiCustomPanel*)m_pParent)->FeatCreationCompleted();
    }
}

// FUNCTION: SWKOTOR 0x006F4680
void CSWGuiFeatsCharGen::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            HandleCancelButton();
            break;
        case 0x29:
            m_pManager->PlayGuiSound(0);
            OnAcceptButton();
            break;
        case 0x27:
        case 0x2D:
            m_pManager->PlayGuiSound(0);
            OnFeatPicked(m_cChart.GetSelectedId());
            break;
        case 0x2A:
            m_pManager->PlayGuiSound(0);
            OnRecommendButton();
            break;
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x40:
            OnEnterFeat(m_cChart.HandleInputEvent(nEvent));
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
