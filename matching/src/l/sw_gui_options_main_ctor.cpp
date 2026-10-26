// Options main panel constructor (layout "optionsmain").
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E3E80
CSWGuiOptionsMain::CSWGuiOptionsMain(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CSWGuiExtent extent;
    int i;
    StartLoadFromLayout("optionsmain");
    InitControl(&m_control_C3C, "LBL_TITLE", 1);
    InitControl(&m_control_F40, "LB_DESC", 1);
    InitControl(&m_control_64[0], "BTN_GAMEPLAY", 1);
    InitControl(&m_control_64[1], "BTN_FEEDBACK", 1);
    InitControl(&m_control_64[2], "BTN_AUTOPAUSE", 1);
    InitControl(&m_control_64[3], "BTN_GRAPHICS", 1);
    InitControl(&m_control_64[4], "BTN_SOUND", 1);
    m_control_64[0].m_n58 = 0xbb86;
    m_control_64[1].m_n58 = 0xbd99;
    m_control_64[2].m_n58 = 0xbdbc;
    m_control_64[3].m_n58 = 0xbb8a;
    m_control_64[4].m_n58 = 0xbb8c;
    InitControl(&m_control_D7C, "BTN_BACK", 1);
    for (i = 0; i < 6; i++) {
        extent = m_control_64[i].m_extent;
        extent.top += 0x14;
        m_control_64[i].SetExtent(extent);
    }
    m_control_F40.m_n2c1 = 1;
    UnloadResource();
    CSWGuiButton* pBack = &m_control_D7C;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pBack->m_bNavigable = 0;
    m_control_AFC.CopyStyle(m_control_F40.m_pProtoItem->m_extent, m_control_F40.m_pProtoItem, 1.0f);
    m_control_64[0].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnGameplayOpt);
    m_control_64[1].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnFeedbackOpt);
    m_control_64[2].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnAutopauseOpt);
    m_control_64[3].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnGraphicsOpt);
    m_control_64[4].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnSoundOpt);
    m_control_64[0].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnGameplayOpt);
    m_control_64[1].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnFeedbackOpt);
    m_control_64[2].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnAutopauseOpt);
    m_control_64[3].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnGraphicsOpt);
    m_control_64[4].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnSoundOpt);
    for (i = 0; i < 6; i++) {
        m_control_64[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsMain::OnEnterSlot);
        m_control_64[i].m_cHilightParams.SetFlags(1, 1, 0);
    }
    CSWGuiPanel* pPanel = this;
    pPanel->SetActiveControl(&m_control_64[0], 0);
}
