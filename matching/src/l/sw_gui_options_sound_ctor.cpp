// Sound options panel constructor (layout "optsound").
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E3550
CSWGuiOptionsSound::CSWGuiOptionsSound(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optsound");
    InitControl(&m_control_64, "SLI_MUSIC", 1);
    InitControl(&m_control_21C, "SLI_VO", 1);
    InitControl(&m_control_3D4, "SLI_FX", 1);
    InitControl(&m_control_58C, "SLI_MOVIE", 1);
    InitControl(&m_control_744, "LBL_MUSIC", 1);
    InitControl(&m_control_9C4, "LBL_VO", 1);
    InitControl(&m_control_884, "LBL_FX", 1);
    InitControl(&m_control_B04, "LBL_MOVIE", 1);
    InitControl(&m_control_152C, "BTN_ADVANCED", 1);
    InitControl(&m_control_D84, "LBL_TITLE", 1);
    InitControl(&m_control_EC4, "LB_DESC", 1);
    m_control_64.m_n58 = 0xa547;
    m_control_21C.m_n58 = 0xa548;
    m_control_3D4.m_n58 = 0xa549;
    m_control_58C.m_n58 = 0xbb31;
    m_control_152C.m_n58 = 0xbfb1;
    m_control_EC4.m_n2c1 = 1;
    InitControl(&m_control_11A4, "BTN_BACK", 1);
    InitControl(&m_control_1368, "BTN_DEFAULT", 1);
    UnloadResource();
    m_control_C44.CopyStyle(m_control_EC4.m_pProtoItem->m_extent, m_control_EC4.m_pProtoItem, 1.0f);
    m_control_64.SetValue(0x32);
    m_control_21C.SetValue(0x32);
    m_control_3D4.SetValue(0x32);
    m_control_58C.SetValue(0x32);
    m_control_64.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnEnterSlider);
    m_control_21C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnEnterSlider);
    m_control_3D4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnEnterSlider);
    m_control_58C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnEnterSlider);
    m_control_64.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnExitSlider);
    m_control_21C.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnExitSlider);
    m_control_3D4.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnExitSlider);
    m_control_58C.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnExitSlider);
    m_control_64.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_21C.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_3D4.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_58C.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_64.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_21C.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_3D4.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_58C.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_64.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_21C.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_3D4.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_58C.AddEvent(0x3f, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_64.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_21C.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_3D4.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_58C.AddEvent(0x40, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_64.AddEvent(0x2a, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnDefault);
    m_control_21C.AddEvent(0x2a, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnDefault);
    m_control_3D4.AddEvent(0x2a, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnDefault);
    m_control_58C.AddEvent(0x2a, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnDefault);
    m_control_64.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_21C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_3D4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_58C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnMoveVolumeSlider);
    m_control_152C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnAdvanced);
    SetUpOptions();
    CSWGuiButton* pButton = &m_control_11A4;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_1368;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsSound::OnDefault);
    CSWGuiPanel::SetActiveControl(&m_control_64, 0);
}
