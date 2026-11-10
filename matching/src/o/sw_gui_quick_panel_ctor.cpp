// CSWGuiQuickPanel constructor (layout "QUICKPNL"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F0390
CSWGuiQuickPanel::CSWGuiQuickPanel(CSWGuiManager* pManager, CSWCCreature* pCreature)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_pCreature = pCreature;
    StartLoadFromLayout("QUICKPNL");
    InitControl(&m_lblDecoration, "LBL_DECORATION", 1);
    for (int i = 0; i < 3; i++) {
        sName.Format("LBL_%d", i + 1);
        InitControl(&m_aHilights[i], sName, 1);
        m_aHilights[i].m_cHilight.m_cParams.SetFlags(1, 0, 1);
        sName.Format("BTN_STEPNAME%d", i + 1);
        InitControl(&m_aSteps[i], sName, 1);
        sName.Format("LBL_NUM%d", i + 1);
        InitControl(&m_aNumbers[i], sName, 1);
    }
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_btnCancel, "BTN_CANCEL", 1);
    UnloadResource();
    m_aSteps[0].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnSelectPortrait);
    m_aSteps[1].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnSelectName);
    m_aSteps[2].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnSelectPlay);
    m_aSteps[0].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnSelectPortrait);
    m_aSteps[1].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnSelectName);
    m_aSteps[2].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnCancelPressed);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pCancel = &m_btnCancel;
    pCancel->m_bNavigable = 0;
    pCancel->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiQuickPanel::OnCancelPressed);
    SetState(0);
    m_nFlags |= 0x60;
}

// FUNCTION: SWKOTOR 0x006F06E0 SYMBOL: ??_GCSWGuiQuickPanel@@UAEPAXI@Z
// Emitted by the constructor above.
