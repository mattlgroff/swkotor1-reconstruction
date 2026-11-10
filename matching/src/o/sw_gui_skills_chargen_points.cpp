// CSWGuiSkillsCharGen point buttons (kept out of the main file so that
// HandleInputEvent calls them, as the original does).
#include "o/sw_gui_skills_chargen.h"

// Resets the ranks to the opening state and spends the points on the
// class package skills.
// NONMATCH: register picks (AL/DL for the class index) and the class id
// shares the index's stack dword in the original (byte at +0x13).
// FUNCTION: SWKOTOR 0x006F4E20
void CSWGuiSkillsCharGen::OnRecommendButton()
{
    unsigned char nClass = GetStats()->GetClassId(GetStats()->m_nNumClasses - 1);
    if (!m_bLevelUp) {
        GetStats()->m_nSkillPoints = 0;
        GetStats()->AddSkillPointsPerLevel();
        GetStats()->ClearSkills();
        GetStats()->UpdateAbilityModifiedSkillRanks();
    } else {
        GetStats()->m_nSkillPoints = m_nInitialPoints;
    }
    unsigned char i;
    for (i = 0; i < g_pRules->m_nNumSkills; i++) {
        GetStats()->SetSkillRankBase(i, m_aBaseRanks[i]);
    }
    GetStats()->AddDefaultPackageSkills(nClass, m_bAllSkills);
    for (i = 0; i < g_pRules->m_nNumSkills; i++) {
        int nMax = GetStats()->GetTotalLevel() + 3;
        if (!IsClassSkill(i)) {
            nMax /= 2;
        }
        CExoString sRank(GetStats()->GetSkillRank(i));
        m_aPoints[i].m_cTextParams.SetText(sRank);
        if (m_bLevelUp) {
            m_aMinus[i].m_bVisible = GetStats()->GetSkillRank(i) != m_aBaseRanks[i];
        } else {
            m_aMinus[i].m_bVisible = GetStats()->GetSkillRank(i) != 0;
        }
        m_aPlus[i].m_bVisible = GetStats()->GetSkillRank(i) != nMax;
        if (!m_bAllSkills && IsUnusable(i)) {
            m_aLabels[i].m_cText.SetColor(g_vGuiDisabledTextColor);
            m_aPoints[i].m_cTextParams.SetColor(g_vGuiDisabledTextColor);
            m_aPlus[i].m_bVisible = 0;
        }
    }
    m_nPoints = GetStats()->m_nSkillPoints;
    CExoString sPoints(m_nPoints);
    m_lblRemaining.m_cText.SetText(sPoints);
    GetStats()->UpdateAbilityModifiedSkillRanks();
}

// FUNCTION: SWKOTOR 0x006F6370
void CSWGuiSkillsCharGen::OnMinusButton()
{
    CSWCLevelUpStatsS* pStats = GetStats();
    if (pStats->GetSkillRank(m_nCurrent) == m_aBaseRanks[m_nCurrent]) {
        return;
    }
    pStats->DecreaseSkillRank(m_nCurrent, m_aBaseRanks[m_nCurrent]);
    m_nPoints = GetStats()->m_nSkillPoints;
    CExoString sRank(GetStats()->GetSkillRank(m_nCurrent));
    m_aPoints[m_nCurrent].m_cTextParams.SetText(sRank);
    CExoString sPoints(m_nPoints);
    m_lblRemaining.m_cText.SetText(sPoints);
    OnEnterPointsButton(&m_aPoints[m_nCurrent]);
    ShowControl(&m_aPlus[m_nCurrent]);
    if (GetStats()->GetSkillRank(m_nCurrent) == m_aBaseRanks[m_nCurrent]) {
        HideControl(&m_aMinus[m_nCurrent]);
        m_aMinus[m_nCurrent].SetFlag0(0);
    }
}

// Party members cannot learn Stealth/Treat Injury as droids or Persuade;
// otherwise the rank goes up when there are points and the cap allows it.
// NONMATCH: the original passes m_nCurrent as a dword and uses a smaller
// frame (8 bytes) and EBX for the class-skill flag.
// FUNCTION: SWKOTOR 0x006F6570
void CSWGuiSkillsCharGen::OnPlusButton()
{
    int bEnough = 0;
    int bBelowMax = 0;
    unsigned long nStrRef;
    switch (m_nCurrent) {
    case 4:
        if (!m_bAllSkills) {
            nStrRef = 0xa5e5;
            goto refuse;
        }
        break;
    case 2:
    case 7:
        if (m_bDroid) {
            nStrRef = 0xa5e7;
            goto refuse;
        }
        break;
    }
    {
        int nMax = GetStats()->GetTotalLevel() + 3;
        int bClassSkill = IsClassSkill(m_nCurrent);
        if (!bClassSkill) {
            nMax /= 2;
        }
        if (GetStats()->m_nSkillPoints >= GetStats()->GetSkillPointCostForSkillRank(m_nCurrent, 1)) {
            bEnough = 1;
        }
        if (GetStats()->GetSkillRank(m_nCurrent) < nMax) {
            bBelowMax = 1;
        }
        if (bEnough != 1 || bBelowMax != 1) {
            CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
            CExoString sMessage;
            if (!bEnough) {
                sMessage = g_pTlkTable->GetSimpleString(0xa5e0);
            } else if (!bBelowMax) {
                if (bClassSkill) {
                    sMessage = g_pTlkTable->GetSimpleString(0xa4c2);
                } else {
                    sMessage = g_pTlkTable->GetSimpleString(0xa4c3);
                }
                CExoString sMax(nMax);
                g_pTlkTable->SetCustomToken(0, sMax);
                g_pTlkTable->ParseTokens(&sMessage);
            }
            pBox->SetAllowCancel(0);
            pBox->SetCloseCallback(0, 0, 0);
            pBox->SetMessage(sMessage);
            m_pManager->AddPanel(pBox, 1, 1);
            return;
        }
        GetStats()->IncreaseSkillRank(m_nCurrent);
        m_nPoints = GetStats()->m_nSkillPoints;
        CExoString sRank(GetStats()->GetSkillRankOOL(m_nCurrent));
        m_aPoints[m_nCurrent].m_cTextParams.SetText(sRank);
        CExoString sPoints(m_nPoints);
        m_lblRemaining.m_cText.SetText(sPoints);
        OnEnterPointsButton(&m_aPoints[m_nCurrent]);
        ShowControl(&m_aMinus[m_nCurrent]);
        if (GetStats()->GetSkillRankOOL(m_nCurrent) == nMax) {
            HideControl(&m_aPlus[m_nCurrent]);
            m_aPlus[m_nCurrent].SetFlag0(0);
        }
        return;
    }
refuse:
    CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(0);
    pBox->SetCloseCallback(0, 0, 0);
    pBox->SetMessageStrRef(nStrRef);
    m_pManager->AddPanel(pBox, 1, 1);
}
