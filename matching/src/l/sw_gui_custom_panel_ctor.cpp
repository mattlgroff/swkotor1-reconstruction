// CSWGuiCustomPanel constructor (layout "CUSTPNL"); the scalar deleting
// destructor is emitted with the vtable here.
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EF730
CSWGuiCustomPanel::CSWGuiCustomPanel(CSWGuiManager* pManager, CSWCCreature* pCreature)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    m_pCreature = pCreature;
    StartLoadFromLayout("CUSTPNL");
    InitControl(&m_lblBG, "LBL_BG", 1);
    for (int i = 0; i < 6; i++) {
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
    m_aSteps[0].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectPortrait);
    m_aSteps[1].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectAbilities);
    m_aSteps[2].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectSkills);
    m_aSteps[3].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectFeats);
    m_aSteps[4].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectName);
    m_aSteps[5].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectPlay);
    m_aSteps[0].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectPortrait);
    m_aSteps[1].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectAbilities);
    m_aSteps[2].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectSkills);
    m_aSteps[3].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectFeats);
    m_aSteps[4].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectName);
    m_aSteps[5].AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnSelectPlay);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pCancel = &m_btnCancel;
    pCancel->m_bNavigable = 0;
    pCancel->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiCustomPanel::OnCancelPressed);
    SetState(0);
    m_nFlags |= 0x60;
}

// FUNCTION: SWKOTOR 0x006EFB20 SYMBOL: ??_GCSWGuiCustomPanel@@UAEPAXI@Z
// Emitted by the constructor above.
