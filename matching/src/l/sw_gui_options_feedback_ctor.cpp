// Feedback options panel constructor (layout "optfeedback").
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E12E0 SYMBOL: ??_GCSWGuiOptionsFeedback@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006E2A70
CSWGuiOptionsFeedback::CSWGuiOptionsFeedback(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optfeedback");
    InitControl(&m_control_64, "LB_OPTIONS", 1);
    InitControl(&m_control_484, "LBL_TITLE", 1);
    InitControl(&m_control_5C4, "LB_DESC", 1);
    m_control_5C4.m_n2c1 = 1;
    InitControl(&m_control_8A4, "BTN_BACK", 1);
    InitControl(&m_control_A68, "BTN_DEFAULT", 1);
    UnloadResource();
    PopulateOptionsListbox();
    m_control_344.CopyStyle(m_control_5C4.m_pProtoItem->m_extent, m_control_5C4.m_pProtoItem, 1.0f);
    CSWGuiButton* pDefault = &m_control_A68;
    pDefault->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnDefault);
    pDefault->m_bNavigable = 0;
    pDefault->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnDefault);
    CSWGuiButton* pBack = &m_control_8A4;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiPanel::SetActiveControl(&m_control_64, 0);
    m_control_64.SelectControl(0);
    SetUpOptions();
}
