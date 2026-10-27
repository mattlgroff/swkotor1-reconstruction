// CSWGuiMainInterfaceChar::Initialize: binds one party slot's controls
// ("LBL_CHAR%d", "PB_VIT%d", ...) on the main interface panel.
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x006898A0
void CSWGuiMainInterfaceChar::Initialize(CSWGuiMainInterface* pPanel, int nIndex)
{
    CExoString sName;
    sName.Format("LBL_CHAR%d", nIndex);
    pPanel->InitControl(&m_aLabels[5], sName, 1);
    sName.Format("PB_VIT%d", nIndex);
    pPanel->InitControl(&m_aBars[0], sName, 1);
    sName.Format("PB_FORCE%d", nIndex);
    pPanel->InitControl(&m_aBars[1], sName, 1);
    sName.Format("LBL_LEVELUP%d", nIndex);
    pPanel->InitControl(&m_aLabels[3], sName, 1);
    sName.Format("LBL_LVLUPBG%d", nIndex);
    pPanel->InitControl(&m_aLabels[4], sName, 1);
    sName.Format("LBL_BACK%d", nIndex);
    pPanel->InitControl(&m_aLabels[0], sName, 1);
    sName.Format("LBL_DEBILATATED%d", nIndex);
    pPanel->InitControl(&m_aLabels[1], sName, 1);
    sName.Format("LBL_DISABLED%d", nIndex);
    pPanel->InitControl(&m_aLabels[2], sName, 1);
    sName.Format("LBL_CMBTEFCTINC%d", nIndex);
    pPanel->InitControl(&m_lblStackA, sName, 0);
    sName.Format("LBL_CMBTEFCTRED%d", nIndex);
    pPanel->InitControl(&m_lblStackB, sName, 0);
    sName.Format("BTN_CHAR%d", nIndex);
    pPanel->InitControl(&m_btnChar, sName, 1);
    m_aLabels[3].m_cBorder.SetFlags(1, 0, 1);
    CSWGuiExtent base = m_aLabels[5].m_extent;
    CSWGuiExtent ext = m_lblStackA.m_extent;
    ext.left -= base.left;
    ext.top -= base.top;
    m_lblStackA.SetExtent(ext);
    ext = m_lblStackB.m_extent;
    ext.left -= base.left;
    ext.top -= base.top;
    m_lblStackB.SetExtent(ext);
    ext = m_aBars[0].m_extent;
    m_btnChar.m_nId = nIndex;
    m_nFromX = (short)ext.left;
    m_nFromY = (short)ext.top;
    CSWGuiCharButton* pButton = &m_btnChar;
    pButton->m_nFlags &= ~4;
    pButton->AddEvent(0x27, pPanel, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiMainInterface::OnCharacterClicked);
    pButton->AddEvent(0x44, pPanel, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiMainInterface::OnCharacterEvent44);
    m_btnChar.m_pChar = this;
    m_nCurHP = 0;
    m_nMaxHP = 0;
    m_nCurFP = 0;
    m_nMaxFP = 0;
    m_nDefense = 0;
    m_nLevel = 0;
    m_bDirty = 0;
}
