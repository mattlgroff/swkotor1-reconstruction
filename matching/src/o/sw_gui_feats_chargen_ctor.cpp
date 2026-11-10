// CSWGuiFeatsCharGen constructor (layout "FTCHRGEN"); the scalar deleting
// destructor is emitted with the vtable here.
#include "o/sw_gui_feats_chargen.h"

// FUNCTION: SWKOTOR 0x006F3D60
CSWGuiFeatsCharGen::CSWGuiFeatsCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature, int bLevelUp,
                                       int bBonusOnly)
    : CSWGuiPanel(pManager)
{
    m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    m_pCreature = pCreature;
    m_bLevelUp = bLevelUp;
    m_bBonusOnly = bBonusOnly;
    m_nCurrentFeat = 0xffff;
    StartLoadFromLayout("FTCHRGEN");
    InitControl(&m_lblTitle, "MAIN_TITLE_LBL", 1);
    InitControl(&m_lblSubTitle, "SUB_TITLE_LBL", 1);
    InitControl(&m_lblRemainingBox, "REMAINING_BOX_LBL", 1);
    InitControl(&m_lblRemaining, "STD_REMAINING_SELECTIONS_LBL", 1);
    InitControl(&m_lblSelectionsRemaining, "STD_SELECTIONS_REMAINING_LBL", 1);
    InitControl(&m_lblDesc, "DESC_LBL", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lbFeats, "LB_FEATS", 1);
    InitControl(&m_lblName, "LBL_NAME", 1);
    InitControl(&m_btnAccept, "BTN_ACCEPT", 1);
    InitControl(&m_btnRecommended, "BTN_RECOMMENDED", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_btnSelect, "BTN_SELECT", 1);
    UnloadResource();
    CExoString sFeat = g_pTlkTable->GetSimpleString(0xa5f7);
    m_sSelectText = g_pTlkTable->GetSimpleString(0x9637) + " " + sFeat;
    m_sRemoveText = g_pTlkTable->GetSimpleString(0x9638) + " " + sFeat;
    if (bLevelUp) {
        m_lblTitle.m_cText.SetStrRef(0x42f);
    }
    if (bBonusOnly) {
        m_lblSubTitle.m_cText.SetStrRef(0x524);
    }
    CSWGuiControl* pProto = m_lbDesc.m_pProtoItem;
    m_lblDescItem.CopyStyle(pProto->m_extent, pProto, 1.0f);
    InitiateFeats();
    m_lbFeats.AddEvent(0x1f8, this, (PFN_GUI_CALLBACK)&CSWGuiFeatsCharGen::OnFeatSelectionChanged);
    m_lbFeats.AddEvent(0x1f9, this, (PFN_GUI_CALLBACK)&CSWGuiFeatsCharGen::OnDoubleClick);
    m_lbFeats.SetListFlag0(1);
    CSWGuiButton* pButton = &m_btnAccept;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent29);
    pButton = &m_btnRecommended;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent2A);
    pButton = &m_btnBack;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pButton = &m_btnSelect;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent27);
    SetActiveControl(&m_lbFeats, 0);
}

// FUNCTION: SWKOTOR 0x006F2F10 SYMBOL: ??_GCSWGuiFeatsCharGen@@UAEPAXI@Z
// Emitted by the constructor above.
