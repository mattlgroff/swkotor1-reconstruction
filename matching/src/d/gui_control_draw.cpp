// CSWGuiControl leaves 0x00417660-0x004192d0: navigation ids,
// Draw / SetExtent / SetStyle overrides of the label, button, slider and
// progress bar controls, slider and progress bar value setters.
// Semantics from src/batches/b0041_gui_controls.cpp and the x86.
#include "d/gui_control.h"

// Scene control (vtable 0x00752e30): a plain CSWGuiControl with an embedded
// part at +0x5c (CSWGuiScene; slot 0x0c is Draw, slot 0x04 SetExtent).
// Class name is a guess.
class CSWGuiScenePart {
public:
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
};

class CSWGuiSceneControl : public CSWGuiControl {
public:
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    CSWGuiScenePart m_cScene;                       // 0x5c
};

#pragma auto_inline(off)

// ---------------------------------------------------------------------------
// CSWGuiNavigable / CSWGuiControl

// FUNCTION: SWKOTOR 0x00417660
void CSWGuiNavigable::SetNavigation(int nDir, int nId)
{
    if (nDir >= 0 && nDir < 4) {
        m_aNavigation[nDir] = nId;
    }
}

// FUNCTION: SWKOTOR 0x00417680
int CSWGuiNavigable::GetNavigationId(int nDir)
{
    if (nDir >= 0 && nDir < 4) {
        return m_aNavigation[nDir];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004176a0
void CSWGuiControl::SetEnabled(int b)
{
    m_bSelectable = b;
}

// CSWGuiControl::SetEnabled above leaves ecx intact; cl reuses it here.
// FUNCTION: SWKOTOR 0x00418d00
void CSWGuiLabel::SetEnabled(int b)
{
    CSWGuiControl::SetEnabled(b);
    if (b) {
        m_cText.m_cParams.ResetColor();
    } else {
        m_cText.m_cParams.SetColor(&g_vDisabledColor);
    }
}

// FUNCTION: SWKOTOR 0x004184b0
void CSWGuiEditbox::SetEnabled(int b)
{
    CSWGuiControl::SetEnabled(b);
    if (b) {
        m_cText.m_cParams.ResetColor();
    } else {
        m_cText.m_cParams.SetColor(&g_vDisabledColor);
    }
}

// FUNCTION: SWKOTOR 0x004176c0
void CSWGuiControl::HideToolTip()
{
    g_pGuiManager->RemoveToolTip();
    m_bToolTipShown = 0;
}

// ---------------------------------------------------------------------------
// CSWGuiLabel / CSWGuiLabelHilight

// FUNCTION: SWKOTOR 0x004176e0
void CSWGuiLabel::SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                           float fScale)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    CSWGuiExtent cInner = m_cBorder.GetInnerExtent();
    m_cText.Initialize(cInner, pText, fScale);
}

// FUNCTION: SWKOTOR 0x00417750
void CSWGuiLabel::Draw(float fDelta)
{
    m_cBorder.Draw(fDelta);
    m_cText.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00417780
void CSWGuiLabel::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    m_cBorder.SetExtent(m_extent);
    CSWGuiExtent cInner = m_cBorder.GetInnerExtent();
    m_cText.SetExtent(cInner);
}

// FUNCTION: SWKOTOR 0x004177e0
void CSWGuiLabelHilight::SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                                  CSWGuiBorderParams* pHilight)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    m_cHilight.Initialize(m_extent, pHilight);
    CSWGuiExtent cText = IntersectExtents(m_cBorder.GetInnerExtent(), m_cHilight.GetInnerExtent());
    m_cText.Initialize(cText, pText, 1.0f);
}

// FUNCTION: SWKOTOR 0x00417880
void CSWGuiLabelHilight::Draw(float fDelta)
{
    if (m_bFlag0) {
        m_cHilight.Draw(fDelta);
    } else {
        m_cBorder.Draw(fDelta);
    }
    m_cText.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x004178e0
void CSWGuiLabelHilight::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    m_cBorder.SetExtent(m_extent);
    m_cHilight.SetExtent(m_extent);
    CSWGuiExtent cText = IntersectExtents(m_cBorder.GetInnerExtent(), m_cHilight.GetInnerExtent());
    m_cText.SetExtent(cText);
}

// ---------------------------------------------------------------------------
// CSWGuiButton / CSWGuiButtonToggle

// FUNCTION: SWKOTOR 0x00417970
void CSWGuiButton::SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                            CSWGuiBorderParams* pHilight)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    m_cHilight.Initialize(m_extent, pHilight);
    CSWGuiExtent cBorder = m_cBorder.GetInnerExtent();
    CSWGuiExtent cHilight = m_cHilight.GetInnerExtent();
    CSWGuiExtent cText = IntersectExtents(m_cBorder.GetInnerExtent(), m_cHilight.GetInnerExtent());
    m_cText.Initialize(cText, pText, 1.0f);
}

// FUNCTION: SWKOTOR 0x00417a20
void CSWGuiButton::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    m_cBorder.SetExtent(extent);
    m_cHilight.SetExtent(extent);
    m_cText.SetExtent(IntersectExtents(m_cBorder.GetInnerExtent(), m_cHilight.GetInnerExtent()));
}

// FUNCTION: SWKOTOR 0x00417ab0
void CSWGuiButton::Draw(float fDelta)
{
    if (m_bFlag0) {
        m_cHilight.Draw(fDelta);
    } else {
        m_cBorder.Draw(fDelta);
    }
    m_cText.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00417b10
void CSWGuiButtonToggle::Draw(float fDelta)
{
    if (m_bSelected) {
        if (m_bFlag0) {
            m_cHilightSelected.Draw(fDelta);
        } else {
            m_cSelected.Draw(fDelta);
        }
    } else {
        if (m_bFlag0) {
            m_cHilight.Draw(fDelta);
        } else {
            m_cBorder.Draw(fDelta);
        }
    }
    m_cText.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00417ba0
void CSWGuiButtonToggle::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    m_cBorder.SetExtent(extent);
    m_cHilight.SetExtent(extent);
    m_cSelected.SetExtent(extent);
    m_cHilightSelected.SetExtent(extent);
    CSWGuiExtent cBorder = m_cBorder.GetInnerExtent();
    CSWGuiExtent cHilight = m_cHilight.GetInnerExtent();
    CSWGuiExtent cSelected = m_cSelected.GetInnerExtent();
    CSWGuiExtent cHilightSelected = m_cHilightSelected.GetInnerExtent();
    IntersectExtents(IntersectExtents(cBorder, cHilight), IntersectExtents(cSelected, cHilightSelected));
    m_cText.SetExtent(extent);
}

// ---------------------------------------------------------------------------
// CSWGuiSlider

// FUNCTION: SWKOTOR 0x00417e60
void CSWGuiSlider::Draw(float fDelta)
{
    if (m_bFlag0) {
        m_cThumb.Draw(fDelta);
    } else {
        m_cBorder.Draw(fDelta);
    }
    m_cImage.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00417ec0
void CSWGuiSlider::SetMaxValue(int nMax)
{
    if (m_nMaxValue != nMax && nMax >= 0) {
        m_nMaxValue = nMax;
        SetExtent(m_extent);
    }
}

// FUNCTION: SWKOTOR 0x00417ee0
void CSWGuiSlider::SetValue(int nValue)
{
    if (nValue < 0) {
        nValue = 0;
    } else if (nValue > m_nMaxValue) {
        nValue = m_nMaxValue;
    }
    m_nValue = nValue;
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x00419060
int CSWGuiSlider::HitCheckSlider(int x, int y)
{
    CSWGuiExtent cThumb = m_cImage.m_extent;
    CSWGuiExtent cInner;
    cInner = m_cBorder.GetInnerExtent();
    if (m_extent.height > m_extent.width) {
        if (y >= cThumb.top && y <= cThumb.height + cThumb.top && x >= cThumb.left &&
            x <= cThumb.width + cThumb.left) {
            return 1;
        }
        if (y > cInner.top && y < cThumb.top && x > cInner.left && x < cInner.width + cInner.left) {
            return 2;
        }
        if (y > cThumb.height + cThumb.top && y < cInner.height + cInner.top && x > cInner.left &&
            x < cInner.width + cInner.left) {
            return 3;
        }
        return 0;
    }
    {
        if (x >= cThumb.left && x <= cThumb.left + cThumb.width && y >= cThumb.top &&
            y <= cThumb.height + cThumb.top) {
            return 1;
        }
        if (x > cInner.left && x < cThumb.left && y > cInner.top && y < cInner.height + cInner.top) {
            return 2;
        }
        if (x > cThumb.width + cThumb.left && x < cInner.width + cInner.left && y > cInner.top &&
            y < cInner.height + cInner.top) {
            return 3;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004191d0
void CSWGuiSlider::HandleLMouseDown()
{
    int x;
    int y;
    ((CSWGuiPanel*)m_pOwner)->GetLocalMouseCoords(&x, &y);
    m_nHitPart = HitCheckSlider(x, y);
    switch (m_nHitPart) {
    case 3:
        HandleInputEvent(500, 1);
        break;
    case 2:
        HandleInputEvent(501, 1);
        break;
    }
    g_pGuiManager->SetCapture(this, 1);
}

// FUNCTION: SWKOTOR 0x00419250
int CSWGuiSlider::v10(int a, int b)
{
    if (g_pGuiManager->m_bMouseDown && m_nHitPart == 1) {
        int x;
        int y;
        ((CSWGuiPanel*)m_pOwner)->GetLocalMouseCoords(&x, &y);
        int nValue;
        if (m_extent.height > m_extent.width) {
            int d = y - m_extent.top;
            if (d < 0) {
                nValue = 0;
            } else if (d > m_extent.height) {
                nValue = m_nMaxValue;
            } else {
                nValue = m_nMaxValue * d / m_extent.height;
            }
        } else {
            int d = x - m_extent.left;
            if (d < 0) {
                nValue = 0;
            } else if (d > m_extent.width) {
                nValue = m_nMaxValue;
            } else {
                nValue = m_nMaxValue * d / m_extent.width;
            }
        }
        SetValue(nValue);
    }
    return 1;
}

// ---------------------------------------------------------------------------
// CSWGuiProgressBar

// FUNCTION: SWKOTOR 0x00417f10
void CSWGuiProgressBar::SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder,
                                 CSWGuiBorderParams* pProgress)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    m_cProgress.Initialize(m_extent, pProgress);
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x00417f60
void CSWGuiProgressBar::Draw(float fDelta)
{
    m_cBorder.Draw(fDelta);
    m_cProgress.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00417f90
void CSWGuiProgressBar::SetMaxValue(int n)
{
    if (m_nMaxValue != n && n >= 0) {
        m_nMaxValue = n;
        SetExtent(m_extent);
    }
}

// FUNCTION: SWKOTOR 0x00417fb0
void CSWGuiProgressBar::SetCurValue(int n)
{
    if (n < 0) {
        n = 0;
    } else if (n > m_nMaxValue) {
        n = m_nMaxValue;
    }
    if (m_nCurValue != n) {
        m_nCurValue = n;
        SetExtent(m_extent);
    }
}

// FUNCTION: SWKOTOR 0x00417fe0
void CSWGuiProgressBar::SetStartFromLeft(int b)
{
    if (m_bStartFromLeft != b) {
        m_bStartFromLeft = b;
        SetExtent(m_extent);
    }
}

// FUNCTION: SWKOTOR 0x004192d0
void CSWGuiProgressBar::CopyStyle(const CSWGuiExtent& extent, CSWGuiProgressBar* pFrom)
{
    SetStyle(extent, &pFrom->m_cBorder.m_cParams, &pFrom->m_cProgress.m_cParams);
}

// FUNCTION: SWKOTOR 0x00419300
void CSWGuiProgressBar::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    CSWGuiExtent cBar;
    m_cBorder.SetExtent(m_extent);
    cBar = m_cBorder.GetInnerExtent();
    if (m_nMaxValue > 0) {
        if (m_extent.height > m_extent.width) {
            int nHeight = (int)((float)m_nCurValue / m_nMaxValue * cBar.height);
            if (!m_bStartFromLeft) {
                cBar.top += cBar.height - nHeight;
            }
            cBar.height = nHeight;
        } else {
            int nWidth = (int)((float)m_nCurValue / m_nMaxValue * cBar.width);
            if (!m_bStartFromLeft) {
                cBar.left += cBar.width - nWidth;
            }
            cBar.width = nWidth;
        }
    }
    m_cProgress.SetExtent(cBar);
}

// ---------------------------------------------------------------------------
// CSWGuiSceneControl

// FUNCTION: SWKOTOR 0x00418380
void CSWGuiSceneControl::SetExtent(const CSWGuiExtent& extent)
{
    m_cScene.SetExtent(extent);
    m_extent = extent;
}

// ---------------------------------------------------------------------------
// CSWGuiScrollBar / CSWGuiListBox styles

#define GUI_MAX(a, b) ((a) > (b) ? (a) : (b))

// FUNCTION: SWKOTOR 0x00418100
void CSWGuiScrollBar::SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, const CResRef& cDir,
                               const CResRef& cThumb)
{
    m_extent = extent;
    m_cBorder.Initialize(m_extent, pBorder);
    CSWGuiImageParams cParams(CResRef(), 0, 0, 0.0f, 0x12, 1.0f, Vector(1.0f, 1.0f, 1.0f));
    cParams.SetImage(cDir, 0);
    m_cDir.Initialize(m_extent, &cParams);
    cParams.SetImage(cThumb, 0);
    m_cThumb.Initialize(m_extent, &cParams);
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x00419790
void CSWGuiScrollBar::CopyStyle(const CSWGuiExtent& extent, CSWGuiScrollBar* pFrom)
{
    SetStyle(extent, &pFrom->m_cBorder.m_cParams, pFrom->m_cDir.m_cParams.m_cResRef,
             pFrom->m_cThumb.m_cParams.m_cResRef);
}

// FUNCTION: SWKOTOR 0x004197c0
int CSWGuiScrollBar::HitCheckScrollbar(int x, int y)
{
    CSWGuiExtent cDir = m_cDir.m_extent;
    CSWGuiExtent cExtent = m_extent;
    CSWGuiExtent cThumb = m_cThumb.m_extent;
    CSWGuiExtent cInner;
    cInner = m_cBorder.GetInnerExtent();
    if (m_extent.height > m_extent.width) {
        if (y >= cThumb.top && y <= cThumb.height + cThumb.top && x >= cThumb.left &&
            x <= cThumb.width + cThumb.left) {
            return 3;
        }
        if (y > cExtent.top && y <= cDir.height + cExtent.top && x > cExtent.left &&
            x < cExtent.left + cExtent.width) {
            return 1;
        }
        int nDownTop = cExtent.height - cDir.height + cExtent.top;
        if (y >= nDownTop && y < cExtent.height + cExtent.top && x > cExtent.left &&
            x < cExtent.width + cExtent.left) {
            return 2;
        }
        if (y > cDir.height + cExtent.top && y < cThumb.top && x > cInner.left &&
            x < cInner.width + cInner.left) {
            return 4;
        }
        if (y > cThumb.height + cThumb.top && y < nDownTop && x > cInner.left &&
            x < cInner.width + cInner.left) {
            return 5;
        }
        return 0;
    }
    if (x > cExtent.left && x <= cDir.width + cExtent.width + cExtent.left && y > cExtent.top &&
        y < cExtent.height + cExtent.top) {
        return 1;
    }
    int nRightLeft = cExtent.width - cDir.width + cExtent.left;
    if (x >= nRightLeft && x < cExtent.width + cExtent.left && y > cExtent.top &&
        y < cExtent.height + cExtent.top) {
        return 2;
    }
    if (x >= cThumb.left && x <= cThumb.width + cThumb.left && y >= cThumb.top &&
        y <= cThumb.height + cThumb.top) {
        return 3;
    }
    if (x > cDir.width + cExtent.left && x < cThumb.left && y > cInner.top &&
        y < cInner.height + cInner.top) {
        return 4;
    }
    if (x > cThumb.width + cThumb.left && x < nRightLeft && y > cInner.top &&
        y < cInner.height + cInner.top) {
        return 5;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00419b80
void CSWGuiListBox::CopyStyle(const CSWGuiExtent& extent, CSWGuiListBox* pFrom)
{
    SetStyle(extent, &pFrom->m_cBorder.m_cParams, pFrom->m_n2c0);
    CSWGuiScrollBar* pScroll = &pFrom->m_cScrollBar;
    m_cScrollBar.CopyStyle(pScroll->m_extent, pScroll);
}

// FUNCTION: SWKOTOR 0x00419bd0
void CSWGuiListBox::SetScrollBarStyle(CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, const CResRef& cDir,
                                      const CResRef& cThumb, int bLeft)
{
    m_bScrollBarOnLeft = bLeft;
    if (extent.width > m_extent.width) {
        extent.width = m_extent.width;
    }
    extent.top = m_extent.top;
    extent.height = m_extent.height;
    if (m_bScrollBarOnLeft) {
        extent.left = m_extent.left;
    } else {
        extent.left = m_extent.left - extent.width + m_extent.width;
    }
    m_cScrollBar.SetStyle(extent, pBorder, cDir, cThumb);
    m_cScrollBar.SetMaxValue(GUI_MAX(m_lstControls.num - GUI_MAX(1, m_nVisible) + 1, 1), m_bFlag9);
    SetExtent(m_extent);
}

// FUNCTION: SWKOTOR 0x0041cd60
void CSWGuiListBox::SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, int nPadding)
{
    m_extent = extent;
    CSWGuiExtent cBorder = extent;
    CSWGuiExtent cScroll = m_cScrollBar.m_extent;
    if (m_bScrollBarOnLeft) {
        cBorder.left += cScroll.width;
    }
    cBorder.width -= cScroll.width;
    m_cBorder.Initialize(cBorder, pBorder);
    m_cArea = m_cBorder.GetInnerExtent();
    SetPadding(nPadding);
}

// ---------------------------------------------------------------------------
// CSWGuiScrollBar

// FUNCTION: SWKOTOR 0x00419660
void CSWGuiScrollBar::Draw(float fDelta)
{
    int bDir = 0;
    int bThumb = 0;
    switch (m_bDrawMode) {
    case 0:
        m_cBorder.Draw(fDelta);
        m_cThumb.Draw(fDelta);
        bDir = 1;
        bThumb = 1;
        break;
    case 1:
        bDir = CanScroll();
        bThumb = IsVisible();
        break;
    }
    CSWGuiExtent cArrow = m_cDir.m_extent;
    if (bDir) {
        if (m_extent.height > m_extent.width) {
            cArrow.top = m_extent.top;
        } else {
            cArrow.left = m_extent.left;
        }
        m_cDir.m_cParams.m_nOrient = 0;
        m_cDir.SetExtent(cArrow);
        m_cDir.Draw(fDelta);
    }
    if (bThumb) {
        if (m_extent.height > m_extent.width) {
            cArrow.top = m_extent.height + m_extent.top - cArrow.height;
            m_cDir.m_cParams.m_nOrient = 4;
        } else {
            cArrow.left = m_extent.width - cArrow.width + m_extent.left;
            m_cDir.m_cParams.m_nOrient = 8;
        }
        m_cDir.SetExtent(cArrow);
        m_cDir.Draw(fDelta);
    }
}

// FUNCTION: SWKOTOR 0x0041adf0
void CSWGuiSlider::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        int bChanged;
        if (m_extent.height > m_extent.width) {
            switch (nEvent) {
            case 0x31:
            case 0x3d:
            case 501:
                goto decrement;
            case 0x32:
            case 0x3e:
            case 500:
                goto increment;
            }
        } else {
            switch (nEvent) {
            case 0x2f:
            case 0x3f:
            case 501:
                goto decrement;
            case 0x30:
            case 0x40:
            case 500:
                goto increment;
            }
        }
        goto done;
    decrement:
        bChanged = m_nValue != 0;
        SetValue(m_nValue - 1);
        goto changed;
    increment:
        bChanged = m_nValue != m_nMaxValue;
        SetValue(m_nValue + 1);
    changed:
        if (bChanged && m_pOwner) {
            CSWGuiPanel* pPanel;
            if (m_pOwner->GetPanel()) {
                pPanel = m_pOwner->GetPanel();
            } else {
                if (!m_pOwner->GetListBox()) {
                    goto done;
                }
                pPanel = m_pOwner->GetListBox()->m_pOwner->GetPanel();
            }
            if (pPanel->m_pManager) {
                pPanel->m_pManager->PlayGuiSound(m_nSound);
            }
        }
    }
done:
    CSWGuiNavigable::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x00415b30
CSWGuiExtent CSWGuiImage::GetImageSize()
{
    CSWGuiExtent cSize;
    if (m_pImage) {
        cSize.width = m_pImage->GetWidth();
        cSize.height = m_pImage->GetHeight();
    }
    return cSize;
}

// FUNCTION: SWKOTOR 0x00418f20
void CSWGuiSlider::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent cThumb;
    m_extent = extent;
    m_cBorder.SetExtent(m_extent);
    m_cThumb.SetExtent(m_extent);
    cThumb = m_cImage.GetImageSize();
    if (m_nMaxValue > 0) {
        if (m_extent.height > m_extent.width) {
            cThumb.width = m_extent.width;
            cThumb.left = m_extent.left;
            if (m_extent.height > cThumb.height) {
                cThumb.top = (m_extent.height - cThumb.height) * m_nValue / m_nMaxValue + m_extent.top;
            } else {
                cThumb.top = 0;
                cThumb.height = m_extent.height;
            }
        } else {
            cThumb.height = m_extent.height;
            cThumb.top = m_extent.top;
            if (m_extent.width > cThumb.width) {
                cThumb.left = (m_extent.width - cThumb.width) * m_nValue / m_nMaxValue + m_extent.left;
            } else {
                cThumb.left = 0;
                cThumb.width = m_extent.width;
            }
        }
    } else {
        if (m_extent.height > m_extent.width) {
            cThumb.width = m_extent.width;
            if (cThumb.height >= m_extent.height) {
                cThumb.height = m_extent.height;
            }
        } else {
            cThumb.height = m_extent.height;
            if (cThumb.width >= m_extent.width) {
                cThumb.width = m_extent.width;
            }
        }
        cThumb.left = m_extent.left;
        cThumb.top = m_extent.top;
    }
    m_cImage.SetExtent(cThumb);
}

void AurStartupWait(); // 0x0044ed90

// FUNCTION: SWKOTOR 0x004193f0
void CSWGuiScrollBar::SetExtent(const CSWGuiExtent& extent)
{
    m_extent = extent;
    AurStartupWait();
    CSWGuiExtent cDir = m_cDir.GetImageSize();
    CSWGuiExtent cBorder;
    CSWGuiExtent cThumb;
    if (m_extent.height > m_extent.width) {
        if (cDir.height * 2 > m_extent.height) {
            cDir.height = m_extent.height / 2;
        }
        if (cDir.width < m_extent.width) {
            cDir.left = (m_extent.width - cDir.width) / 2 + m_extent.left;
        } else {
            cDir.left = m_extent.left;
            cDir.width = m_extent.width;
        }
        m_cDir.SetExtent(cDir);
        cBorder.left = m_extent.left;
        cBorder.top = m_extent.top + cDir.height;
        cBorder.height = m_extent.height - cDir.height * 2;
        cBorder.width = m_extent.width;
    } else {
        if (cDir.width * 2 > m_extent.width) {
            cDir.width = m_extent.width / 2;
        }
        if (cDir.height < m_extent.height) {
            cDir.top = (m_extent.height - cDir.height) / 2 + m_extent.top;
        } else {
            cDir.height = m_extent.height;
            cDir.top = m_extent.top;
        }
        m_cDir.SetExtent(cDir);
        cBorder.left = m_extent.left + cDir.width;
        cBorder.top = m_extent.top;
        cBorder.width = m_extent.width - cDir.width * 2;
        cBorder.height = m_extent.height;
    }
    m_cBorder.SetExtent(cBorder);
    CSWGuiExtent cInner = m_cBorder.GetInnerExtent();
    if (m_extent.height > m_extent.width) {
        cThumb.width = cInner.width;
        cThumb.left = cInner.left;
        if (m_nMaxValue > 1) {
            cThumb.height = (int)((float)m_nVisibleValue / (m_nVisibleValue + m_nMaxValue) * cInner.height);
            if (cThumb.height <= 4) {
                cThumb.height = 4;
            }
            cThumb.top = (cInner.height - cThumb.height) * m_nCurValue / (m_nMaxValue - 1) + cInner.top;
        } else {
            cThumb.height = cInner.height;
            cThumb.top = cInner.top;
        }
    } else {
        cThumb.height = cInner.height;
        cThumb.top = cInner.top;
        if (m_nMaxValue) {
            cThumb.width = (int)((float)m_nVisibleValue / (m_nVisibleValue + m_nMaxValue) * cInner.width);
            if (cThumb.width <= 4) {
                cThumb.width = 4;
            }
            cThumb.left = (cInner.width - cThumb.width) * m_nCurValue / m_nMaxValue + cInner.left;
        } else {
            cThumb.width = cInner.width;
            cThumb.left = cInner.left;
        }
    }
    m_cThumb.SetExtent(cThumb);
}

// ---------------------------------------------------------------------------
// CSWGuiListBox

extern float g_fGuiPulseAlpha;                  // 0x0078d3e4
int GuiBeginClip(int nLeft, int nTop, int nWidth, int nHeight, CSWGuiListBoxScroll* pScroll, int nArg,
                 float fScale);                 // 0x004592f0
void GuiEndClip();                              // 0x00459580

// FUNCTION: SWKOTOR 0x0041a3e0
void CSWGuiListBox::Draw(float fDelta)
{
    int nCount = m_lstControls.num;
    if (m_bFlag5) {
        if (m_bFlag11) {
            m_cScrollBar.m_cDir.m_cParams.m_fAlpha = g_fGuiPulseAlpha;
        }
        m_cScrollBar.Draw(fDelta);
    }
    m_cBorder.Draw(fDelta);
    if (GuiBeginClip(m_cArea.left, m_cArea.top, m_cArea.width, m_cArea.height, &m_cScroll, 0, 1.0f)) {
        if (m_n2c0 + m_nItemSize > m_cArea.height && m_lstControls.num > 0 && !m_bUnequal) {
            m_lstControls.element[0]->Draw(fDelta);
        } else {
            for (int i = 0; i < nCount; i++) {
                CSWGuiControl* pControl = m_lstControls.element[i];
                int nTop = pControl->m_extent.top;
                int nBottom = pControl->m_extent.height + nTop;
                if (nBottom > 0 && nTop <= m_cArea.height &&
                    (!m_bFlag10 || (nBottom < m_cArea.height && nTop >= 0))) {
                    pControl->Draw(fDelta);
                }
            }
        }
        GuiEndClip();
    }
    if (g_pGuiManager->m_pCaptureControl == this && m_n88) {
        int x;
        int y;
        ((CSWGuiPanel*)m_pOwner)->GetLocalMouseCoords(&x, &y);
        if (m_cScrollBar.HitCheckScrollbar(x, y) == m_n70) {
            m_fRepeatTimer -= fDelta;
            while (m_fRepeatTimer < 0.0f) {
                m_fRepeatTimer += 0.1f;
                HandleInputEvent(m_n88, 1);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0041c4a0
void CSWGuiListBox::HandleLMouseDown()
{
    int nIndex;
    CSWGuiControl* pControl = HitCheckMouseLocal(&nIndex);
    if (!pControl) {
        return;
    }
    if (pControl->v6c()) {
        int x;
        int y;
        ((CSWGuiPanel*)m_pOwner)->GetLocalMouseCoords(&x, &y);
        m_n70 = pControl->v6c()->HitCheckScrollbar(x, y);
        m_n88 = 0;
        m_fRepeatTimer = 0.5f;
        switch (m_n70) {
        case 1:
            m_n88 = 0x1fb;
            HandleInputEvent(0x1fb, 1);
            break;
        case 4:
            m_n88 = 0x1fd;
            HandleInputEvent(0x1fd, 1);
            break;
        case 2:
            m_n88 = 0x1fc;
            HandleInputEvent(0x1fc, 1);
            break;
        case 5:
            m_n88 = 0x1fe;
            HandleInputEvent(0x1fe, 1);
            break;
        case 3:
            m_n74 = x;
            m_n78 = y;
            m_n7c = IsOversized() ? m_nPages : m_cScrollBar.m_nCurValue;
            break;
        }
    } else if (m_bSelectable) {
        CSWGuiControl* pMouse = g_pGuiManager->m_pMouseOverControl;
        if (pMouse && (pMouse == this || pMouse->GetOwner() == this) && g_pGuiManager->m_bLeftDown) {
            HandleInputEvent(0x1f9, 1);
        }
        if (m_bListFlag0 || (m_lstControls.num > 0 && m_lstControls.element[0]->v60())) {
            if (m_nSelected != nIndex) {
                SetSelectedControl(nIndex, 1);
            }
        } else {
            m_n6c = nIndex;
        }
        HandleInputEvent(0x1f8, 1);
    }
    g_pGuiManager->SetCapture(this, 1);
}
