// CSWGuiQuickPanel (swguiquickpanel.o, layout "QUICKPNL"): the three-step
// quick character creation list (portrait, name, play).
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EFC10
void CSWGuiQuickPanel::SetState(int nState)
{
    m_nState = nState;
    if (nState == 0) {
        m_btnBack.SetEnabled(0);
        m_btnBack.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
    } else {
        m_btnBack.SetEnabled(1);
        m_btnBack.m_cTextParams.SetColor(g_vDifficultyExitColor);
    }
    for (int i = 0; i < 3; i++) {
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

// FUNCTION: SWKOTOR 0x006EFD60
void CSWGuiQuickPanel::OnSelectPlay(CSWGuiControl* pControl)
{
    if (m_nState >= 2 && pControl->IsActivated()) {
        m_pParent->BeginPlayGame();
    }
}

// FUNCTION: SWKOTOR 0x006EFD80
void CSWGuiQuickPanel::OnSelectName(CSWGuiControl* pControl)
{
    if (m_nState >= 1 && pControl->IsActivated()) {
        CSWGuiNameChargen* pPanel = new CSWGuiNameChargen(m_pManager, m_pCreature);
        pPanel->SetParentPanel(this, 1);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EFE10
void CSWGuiQuickPanel::OnSelectPortrait(CSWGuiControl* pControl)
{
    if (m_nState >= 0 && pControl->IsActivated()) {
        CSWGuiPortraitCharGen* pPanel = new CSWGuiPortraitCharGen(m_pManager, m_pCreature);
        pPanel->SetParentPanel(this, 1);
        if (m_pCreature->GetAnimBase()) {
            m_pCreature->GetAnimBase()->GetModel(0xff)->PlayAnimation("pause2", 1.0f, 0, 0);
            m_pCreature->GetAnimBase()->GetModel(0xfe)->PlayAnimation("pause2", 1.0f, 0, 0);
        }
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006EFF10
void CSWGuiQuickPanel::CancelledByChild()
{
    if (m_nState == 0) {
        m_pParent->PortraitChanged();
    }
    m_aSteps[m_nState].m_cTextParams.SetColor(g_vAbilityColorGranted);
    m_aSteps[m_nState].m_cTextParams.SetHighlighted(1);
}

// Package abilities, feats and skills for the quick character. NONMATCH:
// every byte argument is passed through a stack slot in the original.
// FUNCTION: SWKOTOR 0x006EFFD0
void CSWGuiQuickPanel::DoQuickStats()
{
    CSWCLevelUpStatsL* pStats = GetStats();
    unsigned char nClass = pStats->GetClass(pStats->m_nNumClasses - 1);
    CSWClass* pClass = &g_pRules->m_pClasses[nClass];
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetSTRBase(pClass->m_nPackageSTR);
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetDEXBase(pClass->m_nPackageDEX, 1);
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetCONBase(pClass->m_nPackageCON);
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetWISBase(pClass->m_nPackageWIS);
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetINTBase(pClass->m_nPackageINT);
    ((CSWCLevelUpStatsL*)m_pCreature->m_pStats)->SetCHABase(pClass->m_nPackageCHA);
    CExoArrayListL<unsigned short> lstFeats;
    GetStats()->AddGrantedFeats(0);
    GetStats()->AddDefaultPackageFeats(nClass, &lstFeats, 1, 1);
    for (int i = 0; i < lstFeats.num; i++) {
        CExoArrayListL<unsigned short>* pFeats = (CExoArrayListL<unsigned short>*)&m_pCreature->m_pStats->m_pFeats;
        unsigned short nFeat = lstFeats[i];
        int nFound = 0;
        for (int j = 0; j < pFeats->num; j++) {
            if (pFeats->element[j] == nFeat) {
                nFound++;
            }
        }
        if (!nFound) {
            pFeats->AddAlloc(nFeat);
        }
    }
    m_pCreature->m_pStats->m_nSkillPoints = 0;
    GetStats()->AddSkillPointsPerLevel();
    GetStats()->AddDefaultPackageSkills(nClass, 1);
    GetStats()->UpdateAbilityModifiedSkillRanks();
}
