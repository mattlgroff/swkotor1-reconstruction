// Advanced sound options panel constructor (layout "optsoundadv").
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E20A0
CSWGuiOptionsSoundAdvanced::CSWGuiOptionsSoundAdvanced(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optsoundadv");
    InitControl(&m_control_64, "CB_FORCESOFTWARE", 1);
    InitControl(&m_control_31C, "BTN_EAX", 1);
    InitControl(&m_control_4E0, "BTN_EAXRIGHT", 1);
    InitControl(&m_control_6A4, "BTN_EAXLEFT", 1);
    InitControl(&m_control_9A8, "LBL_TITLE", 1);
    InitControl(&m_control_AE8, "LB_DESC", 1);
    InitControl(&m_control_DC8, "BTN_BACK", 1);
    InitControl(&m_control_F8C, "BTN_DEFAULT", 1);
    InitControl(&m_control_1150, "BTN_CANCEL", 1);
    m_control_64.m_n58 = 0xbdc1;
    m_control_31C.m_n58 = 0xbdc2;
    m_control_AE8.m_n2c1 = 1;
    UnloadResource();
    m_control_868.CopyStyle(m_control_AE8.m_pProtoItem->m_extent, m_control_AE8.m_pProtoItem, 1.0f);
    m_control_64.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnForceSoftware);
    m_control_64.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnForceSoftware);
    m_control_64.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEnterOption);
    m_control_6A4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEAXLeft);
    m_control_6A4.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEAXLeft);
    m_control_6A4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEnterOption);
    m_control_4E0.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEAXRight);
    m_control_4E0.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEAXRight);
    m_control_4E0.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEnterOption);
    m_control_31C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnEnterOption);
    SetUpOptions();
    CSWGuiButton* pButton = &m_control_DC8;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_F8C;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnDefault);
    pButton = &m_control_1150;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSoundAdvanced::OnDiscardChanges);
    CSWGuiPanel::SetActiveControl(&m_control_64, 0);
    m_bDiscard = 0;
}
