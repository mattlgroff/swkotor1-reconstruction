// CSWGuiAbilitiesCharGen constructor (layout "ABCHRGEN"); the scalar
// deleting destructor is emitted with the vtable here.
#include "o/sw_gui_abilities_chargen.h"

// A level-up starts from the server scores (the client bases without a
// server creature) and keeps them as the lower bound.
// FUNCTION: SWKOTOR 0x006F7600
CSWGuiAbilitiesCharGen::CSWGuiAbilitiesCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp)
    : CSWGuiPanel(pManager)
{
    STR_RES strRes;
    m_bLevelUp = bLevelUp;
    m_pCreature = pCreature;
    StartLoadFromLayout("ABCHRGEN");
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
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lblAbilityMod, "LBL_ABILITY_MOD", 1);
    InitControl(&m_lblModifier, "LBL_MODIFIER", 1);
    InitControl(&m_btnAccept, "BTN_ACCEPT", 1);
    InitControl(&m_btnRecommended, "BTN_RECOMMENDED", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_aPoints[0], "STR_POINTS_BTN", 1);
    InitControl(&m_aLabels[0], "STR_LBL", 1);
    InitControl(&m_aMinus[0], "STR_MINUS_BTN", 1);
    InitControl(&m_aPlus[0], "STR_PLUS_BTN", 1);
    m_anDescriptions[0] = 0xde;
    InitControl(&m_aPoints[1], "DEX_POINTS_BTN", 1);
    InitControl(&m_aLabels[1], "DEX_LBL", 1);
    InitControl(&m_aMinus[1], "DEX_MINUS_BTN", 1);
    InitControl(&m_aPlus[1], "DEX_PLUS_BTN", 1);
    m_anDescriptions[1] = 0xdf;
    InitControl(&m_aPoints[2], "CON_POINTS_BTN", 1);
    InitControl(&m_aLabels[2], "CON_LBL", 1);
    InitControl(&m_aMinus[2], "CON_MINUS_BTN", 1);
    InitControl(&m_aPlus[2], "CON_PLUS_BTN", 1);
    m_anDescriptions[2] = 0xe0;
    InitControl(&m_aPoints[3], "WIS_POINTS_BTN", 1);
    InitControl(&m_aLabels[3], "WIS_LBL", 1);
    InitControl(&m_aMinus[3], "WIS_MINUS_BTN", 1);
    InitControl(&m_aPlus[3], "WIS_PLUS_BTN", 1);
    m_anDescriptions[3] = 0xe1;
    InitControl(&m_aPoints[4], "INT_POINTS_BTN", 1);
    InitControl(&m_aLabels[4], "INT_LBL", 1);
    InitControl(&m_aMinus[4], "INT_MINUS_BTN", 1);
    InitControl(&m_aPlus[4], "INT_PLUS_BTN", 1);
    m_anDescriptions[4] = 0xe2;
    InitControl(&m_aPoints[5], "CHA_POINTS_BTN", 1);
    InitControl(&m_aLabels[5], "CHA_LBL", 1);
    InitControl(&m_aMinus[5], "CHA_MINUS_BTN", 1);
    InitControl(&m_aPlus[5], "CHA_PLUS_BTN", 1);
    m_anDescriptions[5] = 0xe3;
    UnloadResource();
    if (m_bLevelUp) {
        CSWSCreature* pServer = m_pCreature->AsServerCreature();
        if (pServer) {
            m_anScores[0] = pServer->GetStats()->GetSTRBase();
            m_anScores[1] = pServer->GetStats()->GetDEXBase();
            m_anScores[2] = pServer->GetStats()->GetCONBase();
            m_anScores[3] = pServer->GetStats()->GetWISBase();
            m_anScores[4] = pServer->GetStats()->GetINTBase();
            m_anScores[5] = pServer->GetStats()->GetCHABase();
        } else {
            m_anScores[0] = GetStats()->GetSTRBase();
            m_anScores[1] = GetStats()->GetDEXBase();
            m_anScores[2] = GetStats()->GetCONBase();
            m_anScores[3] = GetStats()->GetWISBase();
            m_anScores[4] = GetStats()->GetINTBase();
            m_anScores[5] = GetStats()->GetCHABase();
        }
        for (int i = 0; i < 6; i++) {
            m_anBaseScores[i] = m_anScores[i];
        }
    } else {
        m_anScores[0] = GetStats()->GetSTRBase();
        m_anScores[1] = GetStats()->GetDEXBase();
        m_anScores[2] = GetStats()->GetCONBase();
        m_anScores[3] = GetStats()->GetWISBase();
        m_anScores[4] = GetStats()->GetINTBase();
        m_anScores[5] = GetStats()->GetCHABase();
    }
    for (int i = 0; i < 6; i++) {
        m_aPoints[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiAbilitiesCharGen::OnEnterPointsButton);
        m_aPoints[i].AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiAbilitiesCharGen::OnExitPointsButton);
        {
            CExoString sScore(m_anScores[i]);
            m_aPoints[i].m_cTextParams.SetText(sScore);
        }
        m_aPoints[i].m_n58 = i;
        if (m_bLevelUp) {
            HideControl(&m_aMinus[i]);
        } else {
            m_aMinus[i].SetVisibleBit(m_anScores[i] != 8);
            m_aPlus[i].SetVisibleBit(m_anScores[i] != 18);
        }
        CSWGuiControl* pMinus = &m_aMinus[i];
        pMinus->m_bNavigable = 0;
        m_aPlus[i].m_bNavigable = 0;
        m_aMinus[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiAbilitiesCharGen::OnMinusButtonPushed);
        m_aPlus[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiAbilitiesCharGen::OnPlusButtonPushed);
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
    if (m_bLevelUp) {
        if (GetStats()->GetTotalLevel() % 4 == 0) {
            m_nPoints = 1;
        } else {
            m_nPoints = 0;
        }
        CExoString sText("Points Remaining");
        m_lblSelectionsRemaining.m_cText.SetText(sText);
    } else {
        m_nPoints = GetStats()->m_nAbilityPoints;
    }
    {
        CExoString sPoints(m_nPoints);
        m_lblRemaining.m_cText.SetText(sPoints);
    }
    CSWGuiControl* pProto = m_lbDesc.m_pProtoItem;
    m_lblDescItem.CopyStyle(pProto->m_extent, pProto, 1.0f);
    SetActiveControl(&m_aPoints[0], 0);
    m_nCurrent = 0;
}

// FUNCTION: SWKOTOR 0x006F70C0 SYMBOL: ??_GCSWGuiAbilitiesCharGen@@UAEPAXI@Z
// Emitted by the constructor above.
