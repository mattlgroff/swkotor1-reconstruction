// CSWGuiQuickOrCustomPanel constructor (layout "QORCPNL"); the scalar
// deleting destructor is emitted with the vtable here.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F09F0
CSWGuiQuickOrCustomPanel::CSWGuiQuickOrCustomPanel(CSWGuiManager* pManager, CSWCCreature* pCreature)
    : CSWGuiPanel(pManager)
{
    m_pCreature = pCreature;
    StartLoadFromLayout("QORCPNL");
    InitControl(&m_btnQuick, "QUICK_CHAR_BTN", 1);
    InitControl(&m_btnCustom, "CUST_CHAR_BTN", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lblDecoration, "LBL_DECORATION", 1);
    InitControl(&m_lblRAnalog, "LBL_RANALOG", 1);
    InitControl(&m_lblRBG, "LBL_RBG", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    UnloadResource();
    m_lblDesc.CopyStyle(m_lbDesc.m_pProtoItem->m_extent, m_lbDesc.m_pProtoItem, 1.0f);
    m_btnQuick.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnEnterQuickCharacterButton);
    m_btnCustom.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnEnterCustomCharacterButton);
    m_btnQuick.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnSelectQuickCharacterButton);
    m_btnCustom.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnSelectCustomCharacterButton);
    m_btnQuick.AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnSelectQuickCharacterButton);
    m_btnCustom.AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiQuickOrCustomPanel::OnSelectCustomCharacterButton);
    m_btnQuick.m_cHilightParams.SetFlags(1, 1, 0);
    m_btnCustom.m_cHilightParams.SetFlags(1, 1, 0);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    SetActiveControl(&m_btnQuick, 0);
    m_nFlags |= 0x60;
}

// FUNCTION: SWKOTOR 0x006F0D30 SYMBOL: ??_GCSWGuiQuickOrCustomPanel@@UAEPAXI@Z
// Emitted by the constructor above.
