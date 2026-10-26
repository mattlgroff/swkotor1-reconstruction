// Advanced graphics options panel constructor (layout "optgraphicsadv").
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"

extern int AurTestAntiAliasingMode(int nSamples); // 0x0044F140
extern Vector g_vKeyMapLockedTextColor; // 0x007A2408 (disabled text)

// FUNCTION: SWKOTOR 0x006E16E0
CSWGuiOptionsGraphicsAdvanced::CSWGuiOptionsGraphicsAdvanced(CSWGuiManager* pManager, CSWGuiOptionsGraphics* pParent)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optgraphicsadv");
    InitControl(&m_control_1A4, "LBL_TITLE", 1);
    InitControl(&m_control_2E4, "LB_DESC", 1);
    InitControl(&m_control_132C, "BTN_ANTIALIAS", 1);
    InitControl(&m_control_14F0, "BTN_ANTIALIASLEFT", 1);
    InitControl(&m_control_16B4, "BTN_ANTIALIASRIGHT", 1);
    InitControl(&m_control_1878, "BTN_TEXQUAL", 1);
    InitControl(&m_control_1A3C, "BTN_TEXQUALLEFT", 1);
    InitControl(&m_control_1C00, "BTN_TEXQUALRIGHT", 1);
    InitControl(&m_control_5C4, "CB_FRAMEBUFF", 1);
    InitControl(&m_control_DE0, "BTN_ANISOTROPY", 1);
    InitControl(&m_control_FA4, "BTN_ANISOTROPYLEFT", 1);
    InitControl(&m_control_1168, "BTN_ANISOTROPYRIGHT", 1);
    InitControl(&m_control_878, "CB_SOFTSHADOWS", 1);
    InitControl(&m_control_B2C, "CB_VSYNC", 1);
    m_control_132C.m_n58 = 0xbb63;
    m_control_14F0.m_n58 = 0xbb63;
    m_control_16B4.m_n58 = 0xbb63;
    m_control_1878.m_n58 = 0xbb5d;
    m_control_1A3C.m_n58 = 0xbb5d;
    m_control_1C00.m_n58 = 0xbb5d;
    m_control_5C4.m_n58 = 0xbb55;
    m_control_DE0.m_n58 = 0xbb59;
    m_control_FA4.m_n58 = 0xbb59;
    m_control_1168.m_n58 = 0xbb59;
    m_control_878.m_n58 = 0xbb51;
    m_control_B2C.m_n58 = 0xbb61;
    m_control_2E4.m_n2c1 = 1;
    InitControl(&m_control_1DC4, "BTN_BACK", 1);
    InitControl(&m_control_1F88, "BTN_DEFAULT", 1);
    InitControl(&m_control_214C, "BTN_CANCEL", 1);
    UnloadResource();
    m_control_64.CopyStyle(m_control_2E4.m_pProtoItem->m_extent, m_control_2E4.m_pProtoItem, 1.0f);
    m_control_132C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_14F0.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_16B4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_1878.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_1A3C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_1C00.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_5C4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_DE0.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_FA4.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_1168.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_878.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_B2C.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnEnterSlot);
    m_control_14F0.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnAntiAliasLeft);
    m_control_16B4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnAntiAliasRight);
    m_control_1A3C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnTexQualLeft);
    m_control_1C00.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnTexQualRight);
    m_control_FA4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnAnisotropyLeft);
    m_control_1168.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnAnisotropyRight);
    m_control_5C4.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnFrameBuffer);
    m_control_878.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnSoftShadows);
    m_control_B2C.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnVSync);
    SetUpOptions();
    CSWGuiButton* pButton = &m_control_1DC4;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_control_1F88;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnDefault);
    pButton = &m_control_214C;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsGraphicsAdvanced::OnDiscardChanges);
    CSWGuiPanel::SetActiveControl(&m_control_132C, 0);
    if (!AurTestAntiAliasingMode(2) && !AurTestAntiAliasingMode(4) && !AurTestAntiAliasingMode(6)
        && !AurTestAntiAliasingMode(8)) {
        m_control_132C.SetEnabled(0);
        m_control_132C.m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
        m_control_14F0.SetEnabled(0);
        m_control_16B4.SetEnabled(0);
    }
    m_bDiscard = 0;
    m_bChanged = 0;
    m_pParent = pParent;
}
