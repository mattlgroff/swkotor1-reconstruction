// CSWGuiMainInterfaceAction::Initialize: binds one action column
// ("BTN_ACTION%d" / "BTN_TARGET%d" and its icon / arrow buttons) and wires
// the click, focus and scroll events.
#include "k/sw_gui_main_interface.h"

typedef void (CSWGuiPanel::*GuiEventHandler)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x0068B9D0
// NONMATCH: register rotation only (the original keeps this in esi, the
// panel in edi, nIndex in ebx and spills the icon pointer; ours puts this
// in ebp). Frame layout, EH states, events and the jump table agree with
// the up / down / icon pointer locals; member forms shrink the frame.
void CSWGuiMainInterfaceAction::Initialize(CSWGuiPanel* pPanel, int nIndex, int bTarget)
{
    CExoString sName;
    CSWGuiLabel lblUnused;
    m_n718 = bTarget;
    CExoString sKind(bTarget ? "TARGET" : "ACTION");
    sName.Format("BTN_%s%d", sKind.CStr(), nIndex);
    pPanel->InitControl(&m_btnFrame, sName, bTarget == 0);
    sName.Format("LBL_%s%d", sKind.CStr(), nIndex);
    CSWGuiButton* pIcon = &m_btnIcon;
    pPanel->InitControl(pIcon, sName, bTarget == 0);
    sName.Format("BTN_%sUP%d", sKind.CStr(), nIndex);
    CSWGuiButton* pUp = &m_btnUp;
    pPanel->InitControl(pUp, sName, bTarget == 0);
    sName.Format("BTN_%sDOWN%d", sKind.CStr(), nIndex);
    CSWGuiButton* pDown = &m_btnDown;
    pPanel->InitControl(pDown, sName, bTarget == 0);
    if (m_n718) {
        m_btnFrame.AddChildControl(pIcon);
        pIcon->m_pParentControl = &m_btnFrame;
        m_btnFrame.AddChildControl(pUp);
        pUp->m_pParentControl = &m_btnFrame;
        m_btnFrame.AddChildControl(pDown);
        pDown->m_pParentControl = &m_btnFrame;
        nIndex += 4;
    }
    GuiEventHandler pfnPrev;
    GuiEventHandler pfnNext;
    GuiEventHandler pfnFocus;
    if (bTarget) {
        pfnPrev = (GuiEventHandler)&CSWGuiMainInterface::OnTargetPrevClicked;
        pfnNext = (GuiEventHandler)&CSWGuiMainInterface::OnTargetNextClicked;
        pfnFocus = (GuiEventHandler)&CSWGuiMainInterface::OnTargetActionFocus;
    }
    else {
        pfnPrev = (GuiEventHandler)&CSWGuiMainInterface::OnPersonalPrevClicked;
        pfnNext = (GuiEventHandler)&CSWGuiMainInterface::OnPersonalNextClicked;
        pfnFocus = (GuiEventHandler)&CSWGuiMainInterface::OnPersonalActionFocus;
    }
    m_btnIcon.SetNavigable(0);
    m_btnIcon.AddEvent(0x27, pPanel, (GuiEventHandler)&CSWGuiMainInterface::OnActionClicked);
    m_btnIcon.AddEvent(0, pPanel, pfnFocus);
    m_btnIcon.AddEvent(0x1f4, pPanel, pfnPrev);
    m_btnIcon.AddEvent(0x1f5, pPanel, pfnNext);
    m_btnIcon.m_nId = nIndex;
    m_btnFrame.SetNavigable(0);
    m_btnFrame.AddEvent(0, pPanel, pfnFocus);
    m_btnFrame.AddEvent(0x1f4, pPanel, pfnPrev);
    m_btnFrame.AddEvent(0x1f5, pPanel, pfnNext);
    m_btnFrame.m_nId = nIndex;
    pUp->SetNavigable(0);
    pUp->AddEvent(0x27, pPanel, pfnPrev);
    pUp->AddEvent(0, pPanel, pfnFocus);
    pUp->AddEvent(0x1f4, pPanel, pfnPrev);
    pUp->AddEvent(0x1f5, pPanel, pfnNext);
    pUp->m_nId = nIndex;
    pDown->SetNavigable(0);
    pDown->AddEvent(0x27, pPanel, pfnNext);
    pDown->AddEvent(0, pPanel, pfnFocus);
    pDown->AddEvent(0x1f4, pPanel, pfnPrev);
    pDown->AddEvent(0x1f5, pPanel, pfnNext);
    pDown->m_cBorder.m_fRotation = 180.0f;
    pDown->m_cHilite.m_fRotation = 180.0f;
    pDown->m_nId = nIndex;
    pUp->m_nToolTipStrRef = 0xbd51;
    pUp->m_nKeyBinding = 0xf8;
    pDown->m_nToolTipStrRef = 0xbd52;
    pDown->m_nKeyBinding = 0xf9;
    switch (nIndex) {
    case 0:
        pIcon->m_nToolTipStrRef = 0xbd66;
        pIcon->m_nKeyBinding = 0xe8;
        break;
    case 1:
        pIcon->m_nToolTipStrRef = 0xbca3;
        pIcon->m_nKeyBinding = 0xea;
        break;
    case 2:
        pIcon->m_nToolTipStrRef = 0xbcab;
        pIcon->m_nKeyBinding = 0xee;
        break;
    case 3:
        pIcon->m_nToolTipStrRef = 0xbca7;
        pIcon->m_nKeyBinding = 0xec;
        break;
    case 4:
        pIcon->m_nToolTipStrRef = 0xbcaf;
        pIcon->m_nKeyBinding = 0xe2;
        break;
    case 5:
        pIcon->m_nToolTipStrRef = 0xbcb3;
        pIcon->m_nKeyBinding = 0xe4;
        break;
    case 6:
        pIcon->m_nToolTipStrRef = 0xbcb7;
        pIcon->m_nKeyBinding = 0xe6;
        break;
    }
}
