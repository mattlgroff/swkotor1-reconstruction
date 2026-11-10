// CSWGuiSkillsCharGen constructor (layout "SKCHRGEN"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_skills_chargen.h"

// Every skill can be raised for a new character and for the player's own
// level-up (no party member controlled).
// NONMATCH: register picks (the temporaries rotate one register off from
// the g_pAppManager loads on) and the minus button's m_bNavigable store.
// FUNCTION: SWKOTOR 0x006F51D0
CSWGuiSkillsCharGen::CSWGuiSkillsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp)
    : CSWGuiPanel(pManager)
{
    m_pCreature = pCreature;
    m_bLevelUp = bLevelUp;
    m_bAllSkills = !bLevelUp || (m_pCreature->AsServerCreature()->m_pStats->m_bIsPC && g_pAppManager->m_pServerExoApp &&
                                 g_pAppManager->GetServerExoApp()->GetPartyTable() &&
                                 g_pAppManager->GetServerExoApp()->GetPartyTable()->m_nControlledNPC == -1);
    m_bDroid = GetStats()->m_nRace == RACIAL_TYPE_DROID;
    GetStats()->GetLastClass();
    StartLoadFromLayout("SKCHRGEN");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    if (bLevelUp) {
        m_lblTitle.m_cText.SetStrRef(0x42f);
    }
    InitControl(&m_lblSubTitle, "SUB_TITLE_LBL", 1);
    InitControl(&m_lblRemainingBox, "REMAINING_BOX_LBL", 1);
    InitControl(&m_lblRemaining, "REMAINING_SELECTIONS_LBL", 1);
    InitControl(&m_lblSelectionsRemaining, "SELECTIONS_REMAINING_LBL", 1);
    InitControl(&m_lblDesc, "DESC_LBL", 1);
    InitControl(&m_lblCost, "COST_LBL", 1);
    InitControl(&m_lblCostPoints, "COST_POINTS_LBL", 1);
    InitControl(&m_lblClassSkill, "CLASSSKL_LBL", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_btnAccept, "BTN_ACCEPT", 1);
    InitControl(&m_btnRecommended, "BTN_RECOMMENDED", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_aPoints[0], "COMPUTER_USE_POINTS_BTN", 1);
    InitControl(&m_aLabels[0], "COMPUTER_USE_LBL", 1);
    m_aDescriptions[0] = g_pRules->m_pSkills[0].GetDescriptionText();
    InitControl(&m_aMinus[0], "COM_MINUS_BTN", 1);
    InitControl(&m_aPlus[0], "COM_PLUS_BTN", 1);
    InitControl(&m_aPoints[1], "DEMOLITIONS_POINTS_BTN", 1);
    InitControl(&m_aLabels[1], "DEMOLITIONS_LBL", 1);
    m_aDescriptions[1] = g_pRules->m_pSkills[1].GetDescriptionText();
    InitControl(&m_aMinus[1], "DEM_MINUS_BTN", 1);
    InitControl(&m_aPlus[1], "DEM_PLUS_BTN", 1);
    InitControl(&m_aPoints[2], "STEALTH_POINTS_BTN", 1);
    InitControl(&m_aLabels[2], "STEALTH_LBL", 1);
    m_aDescriptions[2] = g_pRules->m_pSkills[2].GetDescriptionText();
    InitControl(&m_aMinus[2], "STE_MINUS_BTN", 1);
    InitControl(&m_aPlus[2], "STE_PLUS_BTN", 1);
    InitControl(&m_aPoints[3], "PERSUADE_POINTS_BTN", 1);
    InitControl(&m_aLabels[3], "PERSUADE_LBL", 1);
    m_aDescriptions[3] = g_pRules->m_pSkills[3].GetDescriptionText();
    InitControl(&m_aMinus[3], "PER_MINUS_BTN", 1);
    InitControl(&m_aPlus[3], "PER_PLUS_BTN", 1);
    InitControl(&m_aPoints[4], "REPAIR_POINTS_BTN", 1);
    InitControl(&m_aLabels[4], "REPAIR_LBL", 1);
    m_aDescriptions[4] = g_pRules->m_pSkills[4].GetDescriptionText();
    InitControl(&m_aMinus[4], "REP_MINUS_BTN", 1);
    InitControl(&m_aPlus[4], "REP_PLUS_BTN", 1);
    InitControl(&m_aPoints[5], "SECURITY_POINTS_BTN", 1);
    InitControl(&m_aLabels[5], "SECURITY_LBL", 1);
    m_aDescriptions[5] = g_pRules->m_pSkills[5].GetDescriptionText();
    InitControl(&m_aMinus[5], "SEC_MINUS_BTN", 1);
    InitControl(&m_aPlus[5], "SEC_PLUS_BTN", 1);
    InitControl(&m_aPoints[6], "AWARENESS_POINTS_BTN", 1);
    InitControl(&m_aLabels[6], "AWARENESS_LBL", 1);
    m_aDescriptions[6] = g_pRules->m_pSkills[6].GetDescriptionText();
    InitControl(&m_aMinus[6], "AWA_MINUS_BTN", 1);
    InitControl(&m_aPlus[6], "AWA_PLUS_BTN", 1);
    InitControl(&m_aPoints[7], "TREAT_INJURY_POINTS_BTN", 1);
    InitControl(&m_aLabels[7], "TREAT_INJURY_LBL", 1);
    m_aDescriptions[7] = g_pRules->m_pSkills[7].GetDescriptionText();
    InitControl(&m_aMinus[7], "TRE_MINUS_BTN", 1);
    InitControl(&m_aPlus[7], "TRE_PLUS_BTN", 1);
    UnloadResource();
    CSWGuiControl* pProto = m_lbDesc.m_pProtoItem;
    m_lblDescItem.CopyStyle(pProto->m_extent, pProto, 1.0f);
    m_nCurrent = 0;
    for (int i = 0; i < 8; i++) {
        m_aPoints[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiSkillsCharGen::OnEnterPointsButton);
        m_aPoints[i].AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiSkillsCharGen::OnExitPointsButton);
        m_aPoints[i].m_n58 = i;
        if (!m_bLevelUp) {
            GetStats()->SetSkillRankBase(i, 0);
            GetStats()->SetModifiedSkillRank(i, 0);
        }
        m_aBaseRanks[i] = GetStats()->GetSkillRank(i);
        {
            CExoString sRank(m_aBaseRanks[i]);
            m_aPoints[i].m_cTextParams.SetText(sRank);
        }
        int nMax = GetStats()->GetTotalLevel() + 3;
        if (!IsClassSkill(i)) {
            nMax /= 2;
        }
        if (m_bLevelUp) {
            m_aMinus[i].SetVisibleBit(GetStats()->GetSkillRank(i) != m_aBaseRanks[i]);
        } else {
            m_aMinus[i].m_bVisible = GetStats()->GetSkillRank(i) != 0;
        }
        m_aPlus[i].m_bVisible = GetStats()->GetSkillRank(i) != nMax;
        m_aMinus[i].m_bNavigable = 0;
        CSWGuiControl* pPlus = &m_aPlus[i];
        pPlus->m_bNavigable = 0;
        m_aMinus[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSkillsCharGen::OnMinusButtonPushed);
        m_aPlus[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSkillsCharGen::OnPlusButtonPushed);
        if (!m_bAllSkills && IsUnusable(i)) {
            m_aLabels[i].m_cText.SetColor(g_vGuiDisabledTextColor);
            m_aPoints[i].m_cTextParams.SetColor(g_vGuiDisabledTextColor);
            m_aPlus[i].m_bVisible = 0;
        }
    }
    CSWGuiButton* pButton = &m_btnAccept;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent27);
    pButton = &m_btnRecommended;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent2A);
    pButton = &m_btnBack;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    GetStats()->AddSkillPointsPerLevel();
    m_nPoints = GetStats()->m_nSkillPoints;
    m_nInitialPoints = m_nPoints;
    {
        CExoString sPoints(m_nPoints);
        m_lblRemaining.m_cText.SetText(sPoints);
    }
    SetActiveControl(&m_aPoints[m_nCurrent], 0);
}

// FUNCTION: SWKOTOR 0x006F4BD0 SYMBOL: ??_GCSWGuiSkillsCharGen@@UAEPAXI@Z
// Emitted by the constructor above.
