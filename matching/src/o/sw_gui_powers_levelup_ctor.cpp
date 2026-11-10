// CSWGuiPowersLevelUp constructor (layout "pwrlvlup"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_powers_levelup.h"

// NONMATCH: register picks from the m_bIsPC copy on (EAX/ECX in the
// original), which shift every later temporary.
// FUNCTION: SWKOTOR 0x006F2180
CSWGuiPowersLevelUp::CSWGuiPowersLevelUp(CSWGuiManager* pManager, unsigned long oidCreature)
    : CSWGuiPanel(pManager)
{
    m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oidCreature);
    m_pCreature = pCreature;
    m_bIsPC = pCreature->AsServerCreature()->m_pStats->m_bIsPC;
    m_nCurrentSpell = 0xffffffff;
    StartLoadFromLayout("pwrlvlup");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblSubTitle, "SUB_TITLE_LBL", 1);
    InitControl(&m_lblRemainingBox, "REMAINING_BOX_LBL", 1);
    InitControl(&m_lblRemaining, "REMAINING_SELECTIONS_LBL", 1);
    InitControl(&m_lblSelectionsRemaining, "SELECTIONS_REMAINING_LBL", 1);
    InitControl(&m_lblDesc, "DESC_LBL", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lbPowers, "LB_POWERS", 1);
    InitControl(&m_lblName, "LBL_POWER", 1);
    InitControl(&m_btnRecommended, "RECOMMENDED_BTN", 1);
    InitControl(&m_btnSelect, "SELECT_BTN", 1);
    InitControl(&m_btnAccept, "ACCEPT_BTN", 1);
    InitControl(&m_btnBack, "BACK_BTN", 1);
    UnloadResource();
    CExoString sPower = g_pTlkTable->GetSimpleString(0xa5f8);
    m_sSelectText = g_pTlkTable->GetSimpleString(0x9637) + " " + sPower;
    m_sRemoveText = g_pTlkTable->GetSimpleString(0x9638) + " " + sPower;
    CSWGuiControl* pProto = m_lbDesc.m_pProtoItem;
    m_lblDescItem.CopyStyle(pProto->m_extent, pProto, 1.0f);
    CSWSCreatureStats* pServerStats = m_pCreature->AsServerCreature()->m_pStats;
    m_cChart.SetCreature(pServerStats, 1);
    InitiatePowers();
    m_lbPowers.AddEvent(0x1f8, this, (PFN_GUI_CALLBACK)&CSWGuiPowersLevelUp::OnPowerSelectionChanged);
    m_lbPowers.AddEvent(0x1f9, this, (PFN_GUI_CALLBACK)&CSWGuiPowersLevelUp::OnDoubleClick);
    m_lbPowers.SetListFlag0(1);
    CSWGuiButton* pButton = &m_btnRecommended;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent2A);
    pButton = &m_btnSelect;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent27);
    pButton = &m_btnAccept;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent29);
    pButton = &m_btnBack;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    SetActiveControl(&m_lbPowers, 0);
}

// FUNCTION: SWKOTOR 0x006F1440 SYMBOL: ??_GCSWGuiPowersLevelUp@@UAEPAXI@Z
// Emitted by the constructor above.
