// Workbench upgrade selection panels (swguiupgrade.o): the constructors of
// CSWGuiUpgradeSelection and CSWGuiUpgradeItemSelect.

#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C7630
CSWGuiUpgradeItemSelect::CSWGuiUpgradeItemSelect(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("upgradeitems");
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_lbItems, "LB_ITEMS", 1);
    InitControl(&m_lbDescription, "LB_DESCRIPTION", 1);
    m_lbDescription.m_n2c1 = 1;
    InitControl(&m_btnUpgradeItem, "BTN_UPGRADEITEM", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    UnloadResource();
    m_lblDescription.CopyStyle(m_lbDescription.m_pProtoItem->m_extent, m_lbDescription.m_pProtoItem, 1.0f);
    m_pUpgrade = new CSWGuiUpgrade(pManager);
    m_pUpgrade->m_pItemSelect = this;
    CSWGuiButton* pUpgrade = &m_btnUpgradeItem;
    pUpgrade->m_bNavigable = 0;
    pUpgrade->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeItemSelect::OnUpgradableSelected);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    m_lbItems.SetListFlag0(1);
    m_nUpgradeType = 0;
    m_pItem = 0;
    CSWGuiPanel::SetActiveControl(&m_lbItems, 0);
}

// FUNCTION: SWKOTOR 0x006C78D0
CSWGuiUpgradeSelection::CSWGuiUpgradeSelection(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("upgradesel");
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_aTypes[0], "BTN_LIGHTSABER", 1);
    InitControl(&m_aTypes[1], "BTN_RANGED", 1);
    InitControl(&m_aTypes[2], "BTN_MELEE", 1);
    InitControl(&m_aTypes[3], "BTN_ARMOR", 1);
    InitControl(&m_aTypeLabels[0], "LBL_LSABER", 1);
    InitControl(&m_aTypeLabels[1], "LBL_RANGED", 1);
    InitControl(&m_aTypeLabels[2], "LBL_MELEE", 1);
    InitControl(&m_aTypeLabels[3], "LBL_ARMOR", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_btnUpgradeItems, "BTN_UPGRADEITEMS", 1);
    UnloadResource();
    m_oidItem = 0x7f000000;
    m_pItemSelect = new CSWGuiUpgradeItemSelect(pManager);
    m_pItemSelect->m_pSelection = this;
    for (int i = 0; i < 4; i++) {
        m_aTypes[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeSelection::OnTypeSelected);
        m_aTypes[i].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeSelection::OnTypeSelected);
        m_aTypes[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeSelection::OnEnter);
        m_aTypes[i].m_n58 = i;
        m_aTypes[i].m_cBorderParams.m_vColor = g_vDifficultyExitColor;
        m_aTypes[i].m_cHilightParams.m_vColor = g_vAbilityColorGranted;
        m_aTypes[i].m_cHilightParams.SetFlags(1, 1, 0);
        HideControl(&m_aTypeLabels[i]);
    }
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pUpgrade = &m_btnUpgradeItems;
    pUpgrade->m_bNavigable = 0;
    pUpgrade->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgradeSelection::OnTypeSelected);
    m_pEntered = 0;
}
