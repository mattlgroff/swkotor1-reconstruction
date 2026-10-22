// CSWGuiMainInterfaceChar (party member slot), its portrait button
// CSWGuiCharButton and the status labels of the main interface,
// 0x00684F50-0x00686A20.
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x00684F50
void CSWGuiCharButton::BuildToolTip()
{
    if (m_pChar) {
        if (m_pChar->m_nMaxFP) {
            m_sToolTip.Format("%s %s\n%s: %d/%d\n%s: %d/%d\n%s: %d\n%s: %d",
                              m_pChar->m_sName.CStr(), m_pChar->m_sClass.CStr(),
                              g_pTlkTable->GetSimpleString(0x425).CStr(), m_pChar->m_nCurHP, m_pChar->m_nMaxHP,
                              g_pTlkTable->GetSimpleString(0x424).CStr(), m_pChar->m_nCurFP, m_pChar->m_nMaxFP,
                              g_pTlkTable->GetSimpleString(0x7d9a).CStr(), m_pChar->m_nDefense,
                              g_pTlkTable->GetSimpleString(0xbd4b).CStr(), m_pChar->m_nLevel);
        } else {
            m_sToolTip.Format("%s %s\n%s: %d/%d\n%s: %d\n%s: %d",
                              m_pChar->m_sName.CStr(), m_pChar->m_sClass.CStr(),
                              g_pTlkTable->GetSimpleString(0x425).CStr(), m_pChar->m_nCurHP, m_pChar->m_nMaxHP,
                              g_pTlkTable->GetSimpleString(0x7d9a).CStr(), m_pChar->m_nDefense,
                              g_pTlkTable->GetSimpleString(0xbd4b).CStr(), m_pChar->m_nLevel);
        }
    }
}

// FUNCTION: SWKOTOR 0x006851B0
// NONMATCH: x87 scheduling only (the original issues the 1.0 - t fsubr after
// the extent copy; statement order and temporaries tried).
void CSWGuiMainInterfaceChar::SlideIn(float fTime)
{
    float t = 1.0f - fTime * 4.0f;
    CSWGuiExtent ext = m_aBars[0].m_extent;
    int nDX = (int)((m_nToX - m_nFromX) * t + m_nFromX - ext.left);
    int nDY = (int)((m_nToY - m_nFromY) * t + m_nFromY - ext.top);
    int i;
    for (i = 0; i < 6; i++) {
        ext = m_aLabels[i].m_extent;
        ext.left += nDX;
        ext.top += nDY;
        m_aLabels[i].SetExtent(ext);
    }
    for (i = 0; i < 2; i++) {
        ext = m_aBars[i].m_extent;
        ext.left += nDX;
        ext.top += nDY;
        m_aBars[i].SetExtent(ext);
    }
    ext = m_btnChar.m_extent;
    ext.left += nDX;
    ext.top += nDY;
    m_btnChar.SetExtent(ext);
}

// FUNCTION: SWKOTOR 0x00685330
// NONMATCH: register allocation in the prologue (the original spills the
// clamped count into the bBig argument slot and reloads bTop); the loop and
// jump table agree.
void CSWGuiMainInterfaceChar::DrawStack(float fDelta, int bBig, int bTop)
{
    CSWGuiExtent extView = m_aLabels[5].m_extent;
    AurGUIStartLayer();
    if (AurGUISetupViewport(extView.left, extView.top, extView.width, extView.height,
                            &g_vGuiViewportColor, 0, 1.0f)) {
        unsigned char nLevel = bTop ? m_nStackA : m_nStackB;
        unsigned char nCount;
        if (bBig) {
            nCount = nLevel < 9 ? nLevel : 9;
        } else {
            nCount = nLevel;
            if (nLevel >= 5) {
                nCount = 5;
            }
        }
        CSWGuiLabel* pStack = bTop ? &m_lblStackA : &m_lblStackB;
        CSWGuiExtent extSaved = pStack->m_extent;
        CSWGuiExtent ext = extSaved;
        unsigned char nHeight = bBig ? 16 : 8;
        unsigned char nStep;
        if (nCount < 5) {
            nStep = bBig ? 10 : 5;
            if (bTop) {
                ext.top -= (nStep >> 1) * (nCount - 1);
            } else {
                ext.top += (nStep >> 1) * (nCount - 1);
            }
        } else {
            if (bTop) {
                ext.top = 1;
            } else {
                ext.top = extView.height - nHeight - 1;
            }
            if (bBig) {
                switch (nCount) {
                case 5: nStep = 10; break;
                case 6: nStep = 8; break;
                case 7: nStep = 7; break;
                case 8: nStep = 6; break;
                case 9: nStep = 5; break;
                default: nStep = (unsigned char)bTop; break;
                }
            } else {
                nStep = 5;
            }
        }
        for (int i = 0; i < nCount; i++) {
            pStack->SetExtent(ext);
            pStack->Draw(fDelta);
            if (bTop) {
                ext.top += nStep;
            } else {
                ext.top -= nStep;
            }
            if ((nCount == 7 || nCount == 8) && (i == 0 || i == 2)) {
                if (bTop) {
                    ext.top--;
                } else {
                    ext.top++;
                }
            }
        }
        pStack->SetExtent(extSaved);
        AurGUICloseViewport();
    }
    AurGUIStopLayer();
}

// FUNCTION: SWKOTOR 0x006857D0
void CSWGuiMainInterfaceChar::SetHealth(int nCurrent, int nMax, int bFriendly)
{
    m_aBars[0].SetMaxValue(nMax);
    m_aBars[0].SetCurValue(nCurrent);
    CResRef cFill(bFriendly ? "greenfill" : "redfill");
    m_aBars[0].m_cProgress.SetFillImage(cFill, 0);
    m_nMaxHP = nMax;
    m_nCurHP = nCurrent;
    m_bDirty = 1;
}

// FUNCTION: SWKOTOR 0x00686750
int CSWGuiCharButton::ShowToolTip()
{
    if (!m_bVisible) {
        return 0;
    }
    BuildToolTip();
    g_pGuiManager->AddToolTip(&m_sToolTip);
    m_nFlags |= 0x40;
    return 1;
}

// FUNCTION: SWKOTOR 0x00686790
void CSWGuiCharButton::Update(float fDelta)
{
    if ((m_nFlags & 0x40) && m_pChar && m_pChar->m_bDirty) {
        BuildToolTip();
        g_pGuiManager->UpdateToolTip(&m_sToolTip);
        m_pChar->m_bDirty = 0;
    }
    CSWGuiButton::Update(fDelta);
}

// FUNCTION: SWKOTOR 0x006867F0
void CSWGuiMainInterfaceChar::Show(int bShow, unsigned char nParts, unsigned char nStackA,
                                   unsigned char nStackB)
{
    m_aLabels[5].m_bVisible = bShow;
    m_aLabels[0].m_bVisible = bShow;
    m_aBars[0].m_bVisible = bShow;
    m_aBars[1].m_bVisible = bShow;
    m_aLabels[1].m_bVisible = bShow && (nParts & 2);
    m_aLabels[2].m_bVisible = bShow && (nParts & 1);
    m_aLabels[3].m_bVisible = bShow && (nParts & 4);
    m_aLabels[4].m_bVisible = bShow && (nParts & 4);
    m_nStackA = nParts ? 0 : nStackA;
    m_nStackB = nParts ? 0 : nStackB;
    CSWGuiControl* pButton = &m_btnChar;
    pButton->m_bVisible = bShow;
}

// FUNCTION: SWKOTOR 0x0068AD10
CSWGuiMainInterfaceStatus::~CSWGuiMainInterfaceStatus()
{
}

// FUNCTION: SWKOTOR 0x00686920
void CSWGuiMainInterfaceStatus::Init(CSWGuiPanel* pPanel, unsigned char nStatus)
{
    CExoString sName;
    m_nStatus = nStatus;
    switch (nStatus) {
    case 0: sName = "LBL_JOURNAL"; break;
    case 1: sName = "LBL_CASH"; break;
    case 2: sName = "LBL_PLOTXP"; break;
    case 3: sName = "LBL_STEALTHXP"; break;
    case 4: sName = "LBL_DARKSHIFT"; break;
    case 5: sName = "LBL_LIGHTSHIFT"; break;
    case 6:
        m_bVisible = 0;
        return;
    case 7: sName = "LBL_ITEMRCVD"; break;
    case 8: sName = "LBL_ITEMLOST"; break;
    }
    pPanel->InitControl(this, sName, 1);
    m_bVisible = 0;
    m_cBorder.SetFlags(1, 0, 1);
}

// FUNCTION: SWKOTOR 0x00686AA0
CSWGuiMainInterfaceStatus::CSWGuiMainInterfaceStatus()
{
}

// Defined (already matched in matching/src/c) to emit the vtable with the
// implicit destructor and its scalar deleting destructor.
CSWGuiCharButton::CSWGuiCharButton()
{
}

// FUNCTION: SWKOTOR 0x00686B00 SYMBOL: ??1CSWGuiCharButton@@UAE@XZ
// CSWGuiCharButton::~CSWGuiCharButton (implicit: no vtable store).

// FUNCTION: SWKOTOR 0x00686B50 SYMBOL: ??_GCSWGuiCharButton@@UAEPAXI@Z
// CSWGuiCharButton::`scalar deleting destructor' (compiler generated).

// FUNCTION: SWKOTOR 0x00689C00
CSWGuiMainInterfaceChar::CSWGuiMainInterfaceChar()
{
}

// FUNCTION: SWKOTOR 0x00689CC0
CSWGuiMainInterfaceChar::~CSWGuiMainInterfaceChar()
{
}
