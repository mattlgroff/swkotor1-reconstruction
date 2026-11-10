// CSWGuiPowersLevelUp (swguipowerslvlup.o, layout "pwrlvlup"): picks the
// Force powers learned at a level-up, 0x006F0EE0-0x006F2910.
#include "o/sw_gui_powers_levelup.h"

// FUNCTION: SWKOTOR 0x006F0EE0
CSWGuiPowersLevelUp::~CSWGuiPowersLevelUp()
{
    m_lbDesc.ClearControls();
    m_lbPowers.ClearControls();
}

// FUNCTION: SWKOTOR 0x006F10C0
void CSWGuiPowersLevelUp::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x13, 0x7f000000, 0x7f000000, 0);
}

// FUNCTION: SWKOTOR 0x006F10F0
void CSWGuiPowersLevelUp::HandleCancelButton()
{
    if (m_pParent) {
        m_pManager->PopModalPanel();
        m_pParent->SetVisible(1);
        m_nFlags = (m_nFlags & ~0x300) | 0x400;
        m_pParent->CancelledByChild();
    }
}

// With powers left to pick a message box asks for them; otherwise the
// chosen powers are learned in the creature's newest class.
// FUNCTION: SWKOTOR 0x006F1130
void CSWGuiPowersLevelUp::OnAcceptButton()
{
    if (m_nRemaining > 0 && m_lstAvailable.num > 0) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetCloseCallback(0, 0, 0);
        pBox->SetMessageStrRef(0xbc52);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    int nClass = (unsigned char)(GetStats()->m_nNumClasses - 1);
    for (int i = 0; i < m_lstChosen.num; i++) {
        GetStats()->AddKnownSpell(nClass, m_lstChosen[i]);
    }
    m_pManager->PopModalPanel();
    m_pParent->SetVisible(1);
    m_nFlags = (m_nFlags & ~0x300) | 0x400;
    m_pParent->PowersSelectionCompleted();
}

// FUNCTION: SWKOTOR 0x006F1210
void CSWGuiPowersLevelUp::Draw(float fDelta)
{
    switch (m_nStatus) {
    case 2:
        m_btnSelect.m_cTextParams.SetText(m_sRemoveText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    case 3:
        m_btnSelect.m_cTextParams.SetText(m_sSelectText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006F1280
void CSWGuiPowersLevelUp::DeterminePower(unsigned long nSpell, unsigned char* pStatus)
{
    if (m_lstKnown.IndexOf(nSpell) != -1) {
        *pStatus = 2;
        return;
    }
    if (m_lstChosen.IndexOf(nSpell) != -1) {
        *pStatus = 1;
        return;
    }
    if (m_lstAvailable.IndexOf(nSpell) != -1) {
        *pStatus = 0;
    } else {
        *pStatus = 3;
    }
}

// FUNCTION: SWKOTOR 0x006F1320
void CSWGuiPowersLevelUp::SetDescription(unsigned long nStrRef)
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

// FUNCTION: SWKOTOR 0x006F1460
void CSWGuiPowersLevelUp::OnEnterPower(unsigned long nSpell)
{
    DeterminePower(nSpell, &m_nStatus);
    m_nCurrentSpell = nSpell;
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(nSpell);
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
        m_btnSelect.m_cTextParams.SetText(m_sRemoveText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    case 3:
        m_btnSelect.m_cTextParams.SetText(m_sSelectText);
        m_btnSelect.m_cTextParams.SetColor(g_vGuiDisabledTextColor);
        m_btnSelect.m_cBorderParams.SetColor(g_vGuiDisabledBorderColor);
        break;
    }
    m_lblName.m_cText.SetStrRef(pSpell->m_nNameStrRef);
    SetDescription(pSpell->m_nDescriptionStrRef);
}

// FUNCTION: SWKOTOR 0x006F15A0
void CSWGuiPowersLevelUp::BuildButtons()
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
        if (!m_bIsPC && (m_lstAvailable[i] == 6 || m_lstAvailable[i] == 14)) {
            pChart->SetStatus(m_lstAvailable[i], 3);
        } else {
            pChart->SetStatus(m_lstAvailable[i], 0);
        }
    }
    if (m_nCurrentSpell == -1) {
        m_lbPowers.AddControls(*(CExoArrayList<CSWGuiControl*>*)pChart, 0, 1, 0);
        m_nCurrentSpell = pChart->Refresh();
    }
    pChart->SetSelected(m_nCurrentSpell);
    unsigned char nPage = m_cChart.m_nPage;
    m_lbPowers.SetSelectedControl(nPage, 0);
    OnEnterPower(m_nCurrentSpell);
    CExoString sRemaining(m_nRemaining);
    m_lblRemaining.m_cText.SetText(sRemaining);
}

// Adds every power the creature can choose now, sorted by name.
// NONMATCH: the original calls Add out of line (ours inlines it), keeps the
// name loop unrotated and has this in EDI.
// FUNCTION: SWKOTOR 0x006F1730
void CSWGuiPowersLevelUp::BuildAvailableList()
{
    CExoString sName;
    CSWCLevelUpStatsP* pStats = GetStats();
    for (unsigned short i = 0; i < g_pRules->m_pSpellArray->m_nNumSpells; i++) {
        if (m_lstKnown.IndexOf(i) != -1) {
            continue;
        }
        if (m_lstChosen.IndexOf(i) != -1) {
            continue;
        }
        if (m_lstAvailable.IndexOf(i) != -1) {
            continue;
        }
        if (!pStats->CanChooseSpell(i, m_nSpellLevel, &m_lstChosen)) {
            continue;
        }
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(i);
        if (pSpell->m_lstPrereqSpells[0] == -2 || !pSpell->m_nNameStrRef) {
            continue;
        }
        sName = pSpell->GetSpellNameText();
        int bInserted = 0;
        for (int j = 0; j < m_lstAvailable.num; j++) {
            if (g_pRules->m_pSpellArray->GetSpellI(m_lstAvailable[j])->GetSpellNameText() > sName) {
                m_lstAvailable.Insert(i, j);
                bInserted = 1;
                break;
            }
        }
        if (!bInserted) {
            m_lstAvailable.Add(i);
        }
    }
}

// FUNCTION: SWKOTOR 0x006F1940
void CSWGuiPowersLevelUp::OnPowerSelectionChanged(CSWGuiControl* pControl)
{
    CSWGuiAbilityChain* pChain = (CSWGuiAbilityChain*)m_lbPowers.GetSelectedControl();
    int nSpell = pChain->HitTest(m_lbPowers.m_ptLocalMouse);
    m_cChart.SetSelected(nSpell);
    OnEnterPower((unsigned short)nSpell);
}

// The known powers of every class, sorted by name, then the powers left.
// NONMATCH: the original keeps the name loop unrotated (test at the top).
// FUNCTION: SWKOTOR 0x006F1990
void CSWGuiPowersLevelUp::InitiatePowers()
{
    CExoString sName;
    CSWCLevelUpStatsP* pStats = GetStats();
    for (unsigned char nClass = 0; nClass < pStats->m_nNumClasses; nClass++) {
        unsigned char nIndex = 0;
        if (pStats->GetNumberKnownSpells(nClass, 0) <= 0) {
            continue;
        }
        do {
            unsigned long nSpell = pStats->GetKnownSpell(nClass, 0, nIndex);
            sName = g_pRules->m_pSpellArray->GetSpellI(nSpell)->GetSpellNameText();
            int bInserted = 0;
            for (int i = 0; i < m_lstKnown.num; i++) {
                if (g_pRules->m_pSpellArray->GetSpellI(m_lstKnown[i])->GetSpellNameText() > sName) {
                    m_lstKnown.Insert(nSpell, i);
                    bInserted = 1;
                    break;
                }
            }
            if (!bInserted) {
                m_lstKnown.Add(nSpell);
            }
            nIndex++;
        } while (nIndex < pStats->GetNumberKnownSpells(nClass, 0));
    }
    m_nRemaining = pStats->ComputeNumberKnownSpellsLeft();
    m_nSpellLevel = m_nRemaining + 1;
    BuildAvailableList();
    BuildButtons();
}

// Moves the power to the chosen list, which is kept sorted by name.
// NONMATCH: the original keeps the name loop unrotated (test at the top).
// FUNCTION: SWKOTOR 0x006F1C90
void CSWGuiPowersLevelUp::AddChosenPower(unsigned long nSpell)
{
    int nIndex = m_lstAvailable.IndexOf(nSpell);
    if (nIndex == -1) {
        return;
    }
    m_lstAvailable.DelIndex(nIndex);
    int bInsert = 0;
    CExoString sName = g_pRules->m_pSpellArray->GetSpellI(nSpell)->GetSpellNameText();
    int i;
    for (i = 0; i < m_lstChosen.num; i++) {
        if (g_pRules->m_pSpellArray->GetSpellI(m_lstChosen[i])->GetSpellNameText() > sName) {
            bInsert = 1;
            break;
        }
    }
    m_nRemaining--;
    if (bInsert) {
        m_lstChosen.Insert(nSpell, i);
    } else {
        m_lstChosen.Add(nSpell);
    }
    BuildAvailableList();
}

// Chosen powers whose prerequisites are gone are removed as well. The flag
// keeps the recursive call (a plain `return` after it becomes a loop).
// NONMATCH: loop alignment and the EAX/EDX picks in the loop.
// FUNCTION: SWKOTOR 0x006F1E40
void CSWGuiPowersLevelUp::RemoveChosenPower(unsigned long nSpell)
{
    int nIndex = m_lstChosen.IndexOf(nSpell);
    if (nIndex == -1) {
        return;
    }
    m_lstChosen.DelIndex(nIndex);
    m_nRemaining++;
    CSWCLevelUpStatsP* pStats = GetStats();
    int bRemoved = 0;
    for (int i = 0; i < m_lstChosen.num; i++) {
        if (!pStats->HasSpellPrereq(g_pRules->m_pSpellArray->GetSpellI(m_lstChosen[i]), &m_lstChosen)) {
            bRemoved = 1;
            RemoveChosenPower(m_lstChosen[i]);
            break;
        }
    }
    if (!bRemoved) {
        m_lstAvailable.num = 0;
        BuildAvailableList();
    }
}

// FUNCTION: SWKOTOR 0x006F1F30
void CSWGuiPowersLevelUp::OnRecommendButton()
{
    CExoArrayListM<unsigned long> lstSpells;
    GetStats()->AddDefaultPackageSpells(&lstSpells);
    while (m_lstChosen.num) {
        RemoveChosenPower(m_lstChosen[0]);
    }
    for (int i = 0; i < lstSpells.num; i++) {
        AddChosenPower(lstSpells[i]);
    }
    CSWGuiSkillInfo* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pSkillInfo;
    pBox->SetSkills(lstSpells, 2);
    pBox->m_lblMessage.m_cText.SetStrRef(0xa511);
    m_pManager->AddPanel(pBox, 1, 1);
    BuildButtons();
}

// FUNCTION: SWKOTOR 0x006F2030
void CSWGuiPowersLevelUp::OnPowerPicked(unsigned long nSpell)
{
    unsigned char nStatus;
    DeterminePower(nSpell, &nStatus);
    unsigned long nStrRef;
    switch (nStatus) {
    case 0:
        if (IsLocked(nSpell)) {
            nStrRef = 0xa5e6;
            break;
        }
        if (m_nRemaining > 0) {
            AddChosenPower(nSpell);
            BuildButtons();
            return;
        }
        nStrRef = 0xa621;
        break;
    case 1:
        RemoveChosenPower(nSpell);
        BuildButtons();
        return;
    case 2:
        nStrRef = 0xa4c9;
        break;
    case 3:
        nStrRef = 0xa4ca;
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

// NONMATCH: the zero-extended argument is built in ECX in the original.
// FUNCTION: SWKOTOR 0x006F2110
void CSWGuiPowersLevelUp::OnDoubleClick(CSWGuiControl* pControl)
{
    int nSelected;
    if (m_cChart.m_lstChains.num > 0) {
        nSelected = m_cChart.GetSelectedNode().m_nId;
    } else {
        nSelected = -1;
    }
    CSWGuiAbilityChain* pChain = (CSWGuiAbilityChain*)m_lbPowers.GetSelectedControl();
    int nSpell = pChain->HitTest(m_lbPowers.m_ptLocalMouse);
    if (nSpell == nSelected) {
        OnPowerPicked((unsigned short)nSpell);
    }
}

// FUNCTION: SWKOTOR 0x006F28C0
void CSWGuiPowersLevelUp::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            HandleCancelButton();
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
        case 0x29:
            OnPowerPicked(m_cChart.GetSelectedId());
            m_pManager->PlayGuiSound(0);
            break;
        case 0x2F:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x40:
            OnEnterPower(m_cChart.HandleInputEvent(nEvent));
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
