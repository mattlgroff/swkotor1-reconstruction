// In-game auto-pause options panel constructor (layout "optautopause").
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E76A0
CSWGuiInGameAutoPause::CSWGuiInGameAutoPause(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optautopause");
    InitControl(&m_aCheckboxes[0], "CB_ENDROUND", 1);
    InitControl(&m_aCheckboxes[1], "CB_ENEMYSIGHTED", 1);
    InitControl(&m_aCheckboxes[2], "CB_MINESIGHTED", 1);
    InitControl(&m_aCheckboxes[3], "CB_PARTYKILLED", 1);
    InitControl(&m_aCheckboxes[4], "CB_ACTIONMENU", 1);
    InitControl(&m_aCheckboxes[5], "CB_TRIGGERS", 1);
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_lbDetails, "LB_DETAILS", 1);
    InitControl(&m_btnDefault, "BTN_DEFAULT", 1);
    m_lbDetails.m_n2c1 = 1;
    InitControl(&m_btnBack, "BTN_BACK", 1);
    for (int i = 0; i < 6; i++) {
        m_aCheckboxes[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameAutoPause::OnEnterSlot);
    }
    m_aCheckboxes[0].m_n58 = 0xa5cd;
    m_aCheckboxes[1].m_n58 = 0xa5ce;
    m_aCheckboxes[2].m_n58 = 0xbfdd;
    m_aCheckboxes[3].m_n58 = 0xa5cf;
    m_aCheckboxes[4].m_n58 = 0xbd20;
    m_aCheckboxes[5].m_n58 = 0xbc56;
    UnloadResource();
    m_lblDescription.CopyStyle(m_lbDetails.m_pProtoItem->m_extent, m_lbDetails.m_pProtoItem, 1.0f);
    CSWGuiButton* pButton = &m_btnDefault;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameAutoPause::OnDefault);
    SetUpOptions();
    pButton = &m_btnBack;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiPanel::SetActiveControl(&m_aCheckboxes[0], 0);
}
