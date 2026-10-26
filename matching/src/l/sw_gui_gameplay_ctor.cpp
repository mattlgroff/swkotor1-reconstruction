// In-game gameplay options panel constructor (layout "optgameplay").
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E69A0
CSWGuiInGameGameplay::CSWGuiInGameGameplay(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optgameplay");
    InitControl(&m_control_CD4, "BTN_DIFFICULTY", 1);
    InitControl(&m_control_E98, "BTN_DIFFRIGHT", 1);
    InitControl(&m_control_105C, "BTN_DIFFLEFT", 1);
    InitControl(&m_control_14D4, "CB_INVERTCAM", 1);
    InitControl(&m_control_1220, "CB_LEVELUP", 1);
    InitControl(&m_control_1788, "CB_AUTOSAVE", 1);
    InitControl(&m_control_1A3C, "CB_REVERSE", 1);
    InitControl(&m_control_1CF0, "CB_DISABLEMOVE", 1);
    InitControl(&m_control_1FA4, "SLI_MOUSESEN", 1);
    InitControl(&m_control_215C, "LBL_MOUSESEN", 1);
    InitControl(&m_control_1A4, "LBL_TITLE", 1);
    InitControl(&m_control_2E4, "LB_DESC", 1);
    InitControl(&m_control_5C4, "BTN_BACK", 1);
    InitControl(&m_control_788, "BTN_DEFAULT", 1);
    InitControl(&m_control_94C, "BTN_KEYMAP", 1);
    InitControl(&m_control_B10, "BTN_MOUSE", 1);
    m_control_2E4.m_n2c1 = 1;
    UnloadResource();
    m_control_64.CopyStyle(m_control_2E4.m_pProtoItem->m_extent, m_control_2E4.m_pProtoItem, 1.0f);
    m_control_CD4.AddEvent(0x2f, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnDifficultyLeft);
    m_control_CD4.AddEvent(0x30, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnDifficultyRight);
    m_control_CD4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterDiff);
    m_control_CD4.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnExitDiff);
    CSWGuiButton* pButton = &m_control_105C;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnDifficultyLeft);
    pButton = &m_control_E98;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnDifficultyRight);
    m_control_1220.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnAutoLevelUp);
    m_control_1220.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnAutoLevelUp);
    m_control_1220.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterSlot);
    m_control_14D4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnInvertCam);
    m_control_14D4.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnInvertCam);
    m_control_14D4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterSlot);
    m_control_1788.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnAutoSave);
    m_control_1788.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnAutoSave);
    m_control_1788.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterSlot);
    m_control_1A3C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnReverse);
    m_control_1A3C.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnReverse);
    m_control_1A3C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterSlot);
    m_control_1CF0.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnCombatMovement);
    m_control_1CF0.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnCombatMovement);
    m_control_1CF0.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnEnterSlot);
    pButton = &m_control_94C;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnKeyMap);
    pButton = &m_control_B10;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnMouse);
    pButton = &m_control_5C4;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_788;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGameGameplay::OnDefault);
    m_control_14D4.m_n58 = 0xbb8e;
    m_control_1220.m_n58 = 0xa51a;
    m_control_1788.m_n58 = 0x9496;
    m_control_1A3C.m_n58 = 0xbc55;
    m_control_1CF0.m_n58 = 0xa5f4;
    m_control_B10.m_n58 = 0xbfaf;
    m_control_94C.m_n58 = 0xbfb0;
    CSWGuiPanel::SetActiveControl(&m_control_CD4, 0);
    SetUpOptions();
}
