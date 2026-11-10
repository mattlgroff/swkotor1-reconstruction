// CSWGuiNameChargen constructor (layout "NAME"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_main_chargen.h"

// NONMATCH: the original keeps &m_btnDone in EDI and &m_btnBack in EBX
// (ours swaps them) and clears each m_bNavigable with its own immediate.
// FUNCTION: SWKOTOR 0x006F9E70
CSWGuiNameChargen::CSWGuiNameChargen(CSWGuiManager* pManager, CSWCCreature* pCreature)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_pCreature = pCreature;
    StartLoadFromLayout("NAME");
    InitControl(&m_btnDone, "END_BTN", 1);
    InitControl(&m_editName, "NAME_BOX_EDIT", 1);
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblSubTitle, "SUB_TITLE_LBL", 1);
    InitControl(&m_btnRandom, "BTN_RANDOM", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    UnloadResource();
    m_btnDone.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiNameChargen::HandleDoneButton);
    m_btnDone.AddEvent(0x2D, this, (PFN_GUI_CALLBACK)&CSWGuiNameChargen::HandleDoneButton);
    m_btnRandom.m_bNavigable = 0;
    m_btnRandom.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent2A);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    m_btnDone.m_bNavigable = 0;
    m_bRandomName = 0;
    m_editName.m_cText.m_nMaxLength = 18;
    m_editName.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiNameChargen::HandleDoneButton);
}

// FUNCTION: SWKOTOR 0x006FA2C0 SYMBOL: ??_GCSWGuiNameChargen@@UAEPAXI@Z
// Emitted by the constructor above.
