// SetStyle / CopyStyle of the toggle button and slider, the
// edit box character handler and the radio button mouse handlers
// (0x00417ca0-0x00418ef0). Semantics from the x86.
#include "d/gui_control.h"

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00417ca0
void CSWGuiButtonToggle::SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText,
                                  CSWGuiBorderParams* pBorder, CSWGuiBorderParams* pHilight,
                                  CSWGuiBorderParams* pSelected, CSWGuiBorderParams* pHilightSelected)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    m_cHilight.Initialize(m_extent, pHilight);
    m_cSelected.Initialize(m_extent, pSelected);
    m_cHilightSelected.Initialize(m_extent, pHilightSelected);
    CSWGuiExtent cBorder = m_cBorder.GetInnerExtent();
    CSWGuiExtent cHilight = m_cHilight.GetInnerExtent();
    CSWGuiExtent cSelected = m_cSelected.GetInnerExtent();
    CSWGuiExtent cHilightSelected = m_cHilightSelected.GetInnerExtent();
    CSWGuiExtent cText = IntersectExtents(IntersectExtents(cBorder, cHilight),
                                          IntersectExtents(cSelected, cHilightSelected));
    m_cText.Initialize(cText, pText, 1.0f);
}

// FUNCTION: SWKOTOR 0x00417dc0
void CSWGuiSlider::SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder,
                            CSWGuiBorderParams* pThumb, const CResRef& cImage)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    m_cThumb.Initialize(m_extent, pThumb);
    CSWGuiImageParams cParams(cImage, 0, 0, 0.0f, 0x12, 1.0f, Vector(1.0f, 1.0f, 1.0f));
    m_cImage.Initialize(m_extent, &cParams);
    SetExtent(m_extent);
}

static char s_szEditChar[2];                        // 0x007a4580

// FUNCTION: SWKOTOR 0x004184f0
void CSWGuiEditbox::HandleChar(int nChar)
{
    switch (nChar) {
    case 0x1c:
        HandleInputEvent(0x2f, 1);
        return;
    case 0x1d:
        HandleInputEvent(0x31, 1);
        return;
    case 0x1e:
        HandleInputEvent(0x30, 1);
        return;
    case 0x1f:
        HandleInputEvent(0x32, 1);
        return;
    case 0x08:
    case 0x7f:
        m_cText.RemoveLastChar();
        return;
    case 0x1b:
        if (m_pOwner) {
            CSWGuiPanel* pPanel = m_pOwner->GetPanel();
            if (pPanel) {
                pPanel->HandleInputEvent(0x28, 1);
            }
        }
        return;
    case 0x0a:
    case 0x0d:
        if (m_pOwner) {
            CSWGuiPanel* pPanel = m_pOwner->GetPanel();
            if (pPanel) {
                pPanel->HandleInputEvent(0x27, 1);
            }
        }
        return;
    }
    if (nChar == '_' || nChar == '/' || nChar == '\\' || nChar < ' ') {
        return;
    }
    s_szEditChar[0] = (char)nChar;
    s_szEditChar[1] = 0;
    m_cText.AddNewChar(s_szEditChar);
}

// FUNCTION: SWKOTOR 0x00418e40
void CSWGuiButtonToggle::CopyStyle(const CSWGuiExtent& extent, CSWGuiButtonToggle* pFrom)
{
    SetStyle(extent, &pFrom->m_cText.m_cParams, &pFrom->m_cBorder.m_cParams,
             &pFrom->m_cHilight.m_cParams, &pFrom->m_cSelected.m_cParams,
             &pFrom->m_cHilightSelected.m_cParams);
}

// FUNCTION: SWKOTOR 0x00418e80
void CSWGuiRadioButton::HandleMouseOver(int bOver)
{
    CSWGuiControl::HandleMouseOver(bOver);
}

// FUNCTION: SWKOTOR 0x00418e90
void CSWGuiRadioButton::HandleLMouseUp()
{
    CSWGuiControl* pOver = g_pGuiManager->m_pMouseOverControl;
    if (pOver && (pOver == this || pOver->GetOwner() == this) && g_pGuiManager->m_bLeftDown) {
        HandleInputEvent(0x27, 1);
        HandleInputEvent(0x1f9, 1);
    }
    g_pGuiManager->ReleaseCapture(this);
    OnCaptureLost();
}

// FUNCTION: SWKOTOR 0x00418ef0
void CSWGuiSlider::CopyStyle(const CSWGuiExtent& extent, CSWGuiSlider* pFrom)
{
    SetStyle(extent, &pFrom->m_cBorder.m_cParams, &pFrom->m_cThumb.m_cParams,
             pFrom->m_cImage.m_cParams.m_cResRef);
}
