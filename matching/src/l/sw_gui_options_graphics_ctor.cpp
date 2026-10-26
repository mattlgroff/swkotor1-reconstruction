// Graphics options panel constructor (layout "optgraphics").
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E2E70
CSWGuiOptionsGraphics::CSWGuiOptionsGraphics(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optgraphics");
    InitControl(&m_control_64, "SLI_GAMMA", 1);
    InitControl(&m_control_49C, "LBL_TITLE", 1);
    InitControl(&m_control_5DC, "LB_DESC", 1);
    InitControl(&m_control_21C, "LBL_GAMMA", 1);
    InitControl(&m_control_8BC, "BTN_RESOLUTION", 1);
    InitControl(&m_control_A80, "BTN_RESOLUTIONLEFT", 1);
    InitControl(&m_control_C44, "BTN_RESOLUTIONRIGHT", 1);
    InitControl(&m_control_E08, "CB_SHADOWS", 1);
    InitControl(&m_control_10BC, "CB_GRASS", 1);
    InitControl(&m_control_1370, "BTN_ADVANCED", 1);
    m_control_64.m_n58 = 0xa546;
    m_control_21C.m_n58 = 0xa546;
    m_control_8BC.m_n58 = 0xbb5f;
    m_control_A80.m_n58 = 0xbb5f;
    m_control_C44.m_n58 = 0xbb5f;
    m_control_E08.m_n58 = 0xbb4f;
    m_control_10BC.m_n58 = 0xbb53;
    m_control_1370.m_n58 = 0xbfae;
    m_control_5DC.m_n2c1 = 1;
    InitControl(&m_control_1534, "BTN_BACK", 1);
    InitControl(&m_control_16F8, "BTN_DEFAULT", 1);
    UnloadResource();
    m_control_35C.CopyStyle(m_control_5DC.m_pProtoItem->m_extent, m_control_5DC.m_pProtoItem, 1.0f);
    m_control_64.SetValue(0x39);
    m_control_64.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnEnterSlider);
    m_control_64.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnExitSlider);
    m_control_64.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnMoveGammaSlider);
    m_control_64.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnMoveGammaSlider);
    m_control_64.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnMoveGammaSlider);
    m_control_64.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnMoveGammaSlider);
    m_control_64.AddEvent(0x2a, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnDefault);
    m_control_64.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnMoveGammaSlider);
    m_control_8BC.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnEnterSlot);
    m_control_E08.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnEnterSlot);
    m_control_10BC.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnEnterSlot);
    m_control_8BC.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnResolution);
    m_control_E08.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnShadows);
    m_control_10BC.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnGrass);
    SetUpOptions();
    CSWGuiButton* pButton = &m_control_1534;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_16F8;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnDefault);
    pButton = &m_control_1370;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphics::OnAdvanced);
    CSWGuiPanel::SetActiveControl(&m_control_64, 0);
    m_bReinitPending = 0;
}
