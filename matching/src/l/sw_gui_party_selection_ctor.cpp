// CSWGuiPartySelection constructor (layout "partyselection") and the member
// button constructor.
#include "l/sw_gui_party_selection.h"

// The radio button base constructor is inlined into the member button.
CSWGuiRadioButton::CSWGuiRadioButton() {}

// FUNCTION: SWKOTOR 0x006BED70
CSWGuiPartySelectionSlot::CSWGuiPartySelectionSlot()
    : m_bSelected(0)
{
}

// FUNCTION: SWKOTOR 0x006BFA40
CSWGuiPartySelection::CSWGuiPartySelection(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_bForced = 0;
    StartLoadFromLayout("partyselection");
    for (int i = 0; i < 9; i++) {
        sName.Format("LBL_NA%d", i);
        InitControl(&m_aSlots[i].m_lblPortrait, sName, 1);
        sName.Format("LBL_CHAR%d", i);
        InitControl(&m_aSlots[i].m_lblBorder, sName, 1);
        sName.Format("BTN_NPC%d", i);
        InitControl(&m_aSlots[i], sName, 1);
        m_aSlots[i].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnToggled);
        m_aSlots[i].AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnToggled);
        m_aSlots[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnEnter);
        m_aSlots[i].AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnLeave);
        m_aSlots[i].m_n58 = (int)&m_aSlots[i];
        m_aSlots[i].m_cHilightParams.SetColor(g_vAbilityColorGranted);
        m_aSlots[i].m_cHilightParams.SetFlags(1, 1, 0);
        m_aSlots[i].m_nNPC = i;
        m_aSlots[i].m_bForced = 0;
    }
    InitControl(&m_lblCount, "LBL_COUNT", 1);
    InitControl(&m_lblNPCName, "LBL_NPC_NAME", 1);
    InitControl(&m_lblNPCLevel, "LBL_NPC_LEVEL", 1);
    InitControl(&m_lbl3D, "LBL_3D", 1);
    InitControl(&m_aLabels[0], "LBL_TITLE", 1);
    InitControl(&m_aLabels[3], "LBL_AVAILABLE", 1);
    InitControl(&m_aLabels[4], "LBL_BEVEL_L", 1);
    InitControl(&m_aLabels[5], "LBL_BEVEL_R", 1);
    InitControl(&m_aLabels[6], "LBL_BEVEL_M", 1);
    CSWGuiButton* pDone = &m_btnDone;
    InitControl(pDone, "BTN_DONE", 1);
    CSWGuiButton* pButton = &m_btnBack;
    InitControl(pButton, "BTN_BACK", 1);
    InitControl(&m_btnAccept, "BTN_ACCEPT", 1);
    UnloadResource();
    m_aLabels[5].m_cBorder.m_fRotation = 180.0f;
    pDone->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnDone);
    pDone->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnDone);
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_btnAccept;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPartySelection::OnToggled);
    m_pEntered = 0;
    pDone->m_bNavigable = 0;
    m_bFromScript = 0;
}
