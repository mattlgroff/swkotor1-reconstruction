// Pazaak tutorial message box CSWGuiTutorial (vtable 0x00753260,
// 0x0067D110-0x0067EFE0): a CSWGuiMessageBox that pages through a list of
// help string references and can place itself next to a control.

#include "m/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x0067D110
void CSWGuiTutorial::PlaceRightOf(CSWGuiControl* pControl)
{
    CSWGuiExtent cExtent = pControl->m_extent;
    int x = cExtent.left + cExtent.width;
    int y = cExtent.top + cExtent.height / 2;
    if (pControl->m_pOwner) {
        CSWGuiPanel* pPanel = pControl->m_pOwner->GetPanel();
        if (pPanel) {
            pPanel->GetAbsoluteExtent(&cExtent);
        }
        x += cExtent.left;
        y += cExtent.top;
    }
    cExtent = m_extent;
    cExtent.left = x;
    cExtent.top = y - cExtent.height / 2;
    SetExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x0067D1C0
void CSWGuiTutorial::PlaceAbove(CSWGuiControl* pControl)
{
    m_pAnchor = pControl;
    CSWGuiExtent cExtent = pControl->m_extent;
    int x = cExtent.left;
    int y = cExtent.top;
    if (pControl->m_pOwner) {
        CSWGuiPanel* pPanel = pControl->m_pOwner->GetPanel();
        if (pPanel) {
            pPanel->GetAbsoluteExtent(&cExtent);
        }
        x += cExtent.left;
        y += cExtent.top;
    }
    cExtent = m_extent;
    cExtent.left = x;
    cExtent.top = y - cExtent.height;
    SetExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x0067D260
void CSWGuiTutorial::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x1f6:
            if (m_nMessage < m_lstMessages.num - 1) {
                m_pManager->PlayGuiSound(0);
                m_nMessage++;
                SetAllowCancel(1);
                SetMessageStrRef(m_lstMessages.element[m_nMessage]);
                if (m_pAnchor) {
                    PlaceAbove(m_pAnchor);
                }
                return;
            }
            break;
        case 0x28:
        case 0x2e:
        case 0x1f7:
            if (m_nMessage > 0) {
                m_pManager->PlayGuiSound(0);
                m_nMessage--;
                if (m_nMessage == 0) {
                    SetAllowCancel(0);
                }
                SetMessageStrRef(m_lstMessages.element[m_nMessage]);
                if (m_pAnchor) {
                    PlaceAbove(m_pAnchor);
                }
                return;
            }
            break;
        }
    }
    CSWGuiMessageBox::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0067DEF0
CSWGuiTutorial::CSWGuiTutorial(CSWGuiManager* pManager)
    : CSWGuiMessageBox(pManager)
{
    m_nMessage = 0;
    m_pAnchor = 0;
    m_bEnabled = 1;
}

// FUNCTION: SWKOTOR 0x0067DF60 SYMBOL: ??1CSWGuiTutorial@@UAE@XZ

// FUNCTION: SWKOTOR 0x0067DFC0
void CSWGuiTutorial::ShowHelp(unsigned int nMessage, int nButtonA, unsigned int nButtonB)
{
    unsigned long nFlags = m_nFlags;
    m_lstMessages.num = 0;
    m_nMessage = 0;
    m_pAnchor = 0;
    if (nFlags & 0x600) {
        m_nFlags = nFlags & ~0x700;
        m_pManager->RemovePanel(this);
    }
    if (!m_bEnabled) {
        return;
    }
    m_lstMessages.Add(nMessage);
    if (nButtonA == -1 && nButtonB == 0x96e2) {
        SetAllowCancel(0);
        m_btnOk.m_cText.m_cParams.SetStrRef(0x96df);
    }
    else {
        SetAllowCancel(1);
        m_btnOk.m_cText.m_cParams.SetStrRef(nButtonA);
        m_btnCancel.m_cText.m_cParams.SetStrRef(nButtonB);
    }
    SetMessageStrRef(nMessage);
    m_pManager->AddPanel(this, 1, 1);
}

// FUNCTION: SWKOTOR 0x0067E0B0
void CSWGuiTutorial::ShowMultipleHelp(const unsigned int* pMessages, int nCount)
{
    if (!m_bEnabled) {
        return;
    }
    ShowHelp(pMessages[0], -1, 0x96e2);
    for (int i = 1; i < nCount; i++) {
        m_lstMessages.Add(pMessages[i]);
    }
    m_btnCancel.m_cText.m_cParams.SetStrRef(0x96e0);
}

// FUNCTION: SWKOTOR 0x0067EFE0 SYMBOL: ??_GCSWGuiTutorial@@UAEPAXI@Z
