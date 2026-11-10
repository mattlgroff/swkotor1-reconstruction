// CSWGuiDialogCinematic (swguidialog.o, vtable 0x00755800): the
// letterboxed conversation panel with its reply list and message label,
// plus the CSWGuiDialogTop panel that shows the spoken line.
#define L_GUI_TEXT_PARAMS_EXACT
#include "l/sw_gui_tutorial_box.h"

extern Vector g_vDifficultyExitColor;               // 0x007A23B4

// FUNCTION: SWKOTOR 0x006A8B40
CSWGuiDialogCinematic::CSWGuiDialogCinematic(CSWGuiManager* pManager)
    : CSWGuiDialog(pManager)
{
    m_vReplyColor = g_vDifficultyExitColor;
    StartLoadFromLayout("dialog");
    InitControl(&m_lbReplies, "LB_REPLIES", 1);
    InitControl(&m_lblMessage, "LBL_MESSAGE", 1);
    UnloadResource();
    int nMarginX;
    int nMarginY;
    m_pManager->GetSafeMargins(&nMarginX, &nMarginY);
    CSWGuiManager* pGuiManager = m_pManager;
    CSWGuiZeroExtent extent;
    extent.left = nMarginX;
    extent.top = (pGuiManager->m_nHeight - (int)(pGuiManager->m_nWidth * -0.4285715f)) / 2;
    extent.width = pGuiManager->m_nWidth - 2 * nMarginX;
    extent.height = 100;
    SetExtent(extent);
    CSWGuiControl* pProto = m_lbReplies.m_pProtoItem;
    CSWGuiListBox* pReplies = &m_lbReplies;
    for (int i = 0; i < 20; i++) {
        int nWidth = pReplies->m_nAreaWidth - 2 * pReplies->m_nPadding;
        CSWGuiExtent cReply = {0, 0, nWidth, 20};
        m_aReplies[i].CopyStyle(cReply, pProto, 1.0f);
        m_aReplies[i].AddEvent(0, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiDialog::ResetReplyColor);
        m_aReplies[i].AddEvent(1, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiDialog::SetReplyColor);
    }
    m_lblMessage.m_cText.SetColor(m_vReplyColor);
    CResRef cFont;
    {
        CExoString sFont(m_lblMessage.m_cText.m_szFont);
        cFont = sFont;
    }
    m_pTop = new CSWGuiDialogTop(m_pManager, cFont, m_vReplyColor);
    m_nFlags |= 1;
}
