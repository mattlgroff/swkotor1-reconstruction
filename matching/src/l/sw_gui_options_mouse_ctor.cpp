// Mouse options panel constructor (layout "optmouse").
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E2600
CSWGuiOptionsMouse::CSWGuiOptionsMouse(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optmouse");
    InitControl(&m_control_94C, "SLI_MOUSESEN", 1);
    InitControl(&m_control_B04, "LBL_MOUSESEN", 1);
    InitControl(&m_control_C44, "CB_REVBUTTONS", 1);
    InitControl(&m_control_1A4, "LBL_TITLE", 1);
    InitControl(&m_control_2E4, "LB_DESC", 1);
    InitControl(&m_control_5C4, "BTN_BACK", 1);
    InitControl(&m_control_788, "BTN_DEFAULT", 1);
    m_control_2E4.m_n2c1 = 1;
    UnloadResource();
    m_control_64.CopyStyle(m_control_2E4.m_pProtoItem->m_extent, m_control_2E4.m_pProtoItem, 1.0f);
    m_control_C44.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnRevMouseButtons);
    m_control_C44.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnRevMouseButtons);
    m_control_C44.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnEnterRevMouseButtons);
    m_control_94C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnMoveMouseSenSlider);
    m_control_94C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnEnterSlider);
    m_control_94C.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnExitSlider);
    m_control_94C.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnMoveMouseSenSlider);
    m_control_94C.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnMoveMouseSenSlider);
    m_control_94C.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnMoveMouseSenSlider);
    m_control_94C.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnMoveMouseSenSlider);
    m_control_94C.m_n58 = 0xbb8d;
    m_control_B04.m_n58 = 0xbb8d;
    m_control_C44.m_n58 = 0xbd42;
    CSWGuiButton* pButton = &m_control_5C4;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_788;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMouse::OnDefault);
    CSWGuiPanel::SetActiveControl(&m_control_94C, 0);
    SetUpOptions();
}
