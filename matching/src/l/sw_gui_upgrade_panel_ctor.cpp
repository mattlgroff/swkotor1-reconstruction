// CSWGuiUpgrade (swguiupgrade.o): the workbench upgrade panel constructor.

#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C6B60
CSWGuiUpgrade::CSWGuiUpgrade(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_pUpgrade2DA = new C2DA(CResRef("upgrade"), 0);
    m_pCrystals2DA = new C2DA(CResRef("upcrystals"), 0);
    m_pUpgrade2DA->Load2DArray();
    m_pCrystals2DA->Load2DArray();
    StartLoadFromLayout("upgrade");
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_lbItems, "LB_ITEMS", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lbDescLS, "LB_DESC_LS", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lblDescBG, "LBL_DESCBG", 1);
    InitControl(&m_lblDescBGLS, "LBL_DESCBG_LS", 1);
    InitControl(&m_cModel, "3D_MODEL", 1);
    InitControl(&m_cModelLS, "3D_MODEL_LS", 1);
    InitControl(&m_lblSlotName, "LBL_SLOTNAME", 1);
    InitControl(&m_lblLSSlotName, "LBL_LSSLOTNAME", 1);
    InitControl(&m_lblUpgrades, "LBL_UPGRADES", 1);
    InitControl(&m_lblUpgradeCount, "LBL_UPGRADE_COUNT", 1);
    InitControl(&m_lblProperty, "LBL_PROPERTY", 1);
    InitControl(&m_btnAssemble, "BTN_ASSEMBLE", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    m_btnAssemble.m_nSound = 0x10;
    m_btnAssemble.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnAssemble);
    m_btnAssemble.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnAssemble);
    m_btnAssemble.m_bNavigable = 0;
    for (int i = 0; i < 4; i++) {
        if (i < 3) {
            sName.Format("LBL_UPGRADE3%d", i + 1);
            InitControl(&m_aSlotLabels3[i], sName, 1);
            sName.Format("BTN_UPGRADE3%d", i + 1);
            InitControl(&m_aSlots3[i], sName, 1);
            m_aSlots3[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnSlotSelected);
            m_aSlots3[i].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnSlotSelected);
            m_aSlots3[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnEnterSlot);
            m_aSlots3[i].m_n58 = i;
            m_aSlots3[i].m_cHilightParams.SetFlags(1, 1, 0);
        }
        sName.Format("LBL_UPGRADE4%d", i + 1);
        InitControl(&m_aSlotLabels4[i], sName, 1);
        sName.Format("BTN_UPGRADE4%d", i + 1);
        InitControl(&m_aSlots4[i], sName, 1);
        m_aSlots4[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnSlotSelected);
        m_aSlots4[i].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnSlotSelected);
        m_aSlots4[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiUpgrade::OnEnterSlot);
        m_aSlots4[i].m_n58 = i;
        m_aSlots4[i].m_cHilightParams.SetFlags(1, 1, 0);
    }
    UnloadResource();
    m_lblSlotName.m_cBorder.m_vColor = g_vDifficultyExitColor;
    m_lblLSSlotName.m_cBorder.m_vColor = g_vDifficultyExitColor;
    m_lblUpgradeCount.m_cBorder.m_vColor = g_vDifficultyExitColor;
    m_lblUpgrades.m_cBorder.m_vColor = g_vDifficultyExitColor;
    m_lblProperty.m_cBorder.m_vColor = g_vDifficultyExitColor;
    m_lbItems.GetBorderParams().m_vColor = g_vDifficultyExitColor;
    m_lblDescription.CopyStyle(m_lbDescLS.m_pProtoItem->m_extent, m_lbDescLS.m_pProtoItem, 1.0f);
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbItems.m_pProtoItem;
    pProto->m_cBorderParams.m_vColor = g_vDifficultyExitColor;
    pProto->m_cHilightParams.m_vColor = g_vAbilityColorGranted;
    pProto->m_cHilightParams.SetFlags(1, 1, 0);
    m_bItemsOpen = 0;
    m_nType = 0;
    m_pItem = 0;
    m_pOriginalItem = 0;
    m_aSlotItems[0] = 0;
    m_aSlotItems[1] = 0;
    m_aSlotItems[2] = 0;
    m_aSlotItems[3] = 0;
    if (g_bDo3DGui) {
        m_cModel.m_cScene.m_pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        m_cModel.m_cScene.AddModel("upgitem_light", -1);
        m_cModelLS.m_cScene.m_pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        m_cModelLS.m_cScene.AddModel("upgitem_light", -1);
    }
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
}
