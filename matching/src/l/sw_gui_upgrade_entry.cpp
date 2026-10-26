// CSWUpgradeItemEntry (swguiupgrade.o): an upgradable item line in the
// workbench item list. Draw needs the control's virtual slot 0x38.
#define L_GUI_CONTROL_DRAW_SLOT
#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C3E00
CSWUpgradeItemEntry::CSWUpgradeItemEntry()
{
    m_bEquipped = 0;
    m_oidItem = 0x7f000000;
    m_nType = 0xff;
}

// FUNCTION: SWKOTOR 0x006C5AB0 SYMBOL: ??_GCSWUpgradeItemEntry@@UAEPAXI@Z
// Emitted by the constructor above; the regular destructor is external.

// FUNCTION: SWKOTOR 0x006C2650
void CSWUpgradeItemEntry::SetExtent(const CSWGuiExtent& extent)
{
    if (extent.width <= 0x38) {
        return;
    }
    m_extent = extent;
    CSWGuiExtent cIcon;
    cIcon.left = m_extent.left;
    cIcon.top = m_extent.top;
    cIcon.width = 0x38;
    cIcon.height = 0x38;
    m_cIcon.SetExtent(cIcon);
    m_cIconHilight.SetExtent(cIcon);
    m_cIconOverlay.SetExtent(cIcon);
    CSWGuiExtent cCount = cIcon;
    int nWidth = m_cCount.m_cParams.m_sText.GetLength() > 2 ? 0x2a : 0x15;
    cCount.height = 0x38;
    cCount.width = nWidth;
    cCount.left += 0x38 - nWidth;
    m_cCount.SetExtent(cCount);
    cIcon.width = m_extent.width - 0x38;
    cIcon.left += 0x38;
    GetBorder().SetExtent(cIcon);
    GetHilight().SetExtent(cIcon);
    m_cTextBox.SetExtent(IntersectExtents(GetBorder().GetInnerExtent(), GetHilight().GetInnerExtent()));
}

// FUNCTION: SWKOTOR 0x006C27C0
void CSWUpgradeItemEntry::Draw(float fDelta)
{
    if (m_bFlag0) {
        GetHilight().Draw(fDelta);
        m_cIconHilight.Draw(fDelta);
    } else {
        GetBorder().Draw(fDelta);
        m_cIcon.Draw(fDelta);
    }
    m_cIconOverlay.Draw(fDelta);
    m_cTextBox.Draw(fDelta);
    m_cCount.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006C2830
void CSWUpgradeItemEntry::Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder, CSWGuiBorder* pHilight, int nWidth)
{
    m_cTextParams = pText->m_cParams;
    CSWGuiBorderParams& rBorder = m_cBorderParams;
    rBorder = pBorder->m_cParams;
    rBorder.SetColor(g_vDifficultyExitColor);
    CSWGuiBorderParams& rHilight = m_cHilightParams;
    rHilight = pHilight->m_cParams;
    rHilight.SetColor(g_vAbilityColorGranted);
    rHilight.SetFlags(1, 1, 0);
    CSWGuiBorderParams& rIcon = m_cIcon.m_cParams;
    {
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        Vector vWhite(1.0f, 1.0f, 1.0f);
        rIcon = CSWGuiBorderParams(cCorner, cEdge, cFill, 0, 0, 1.0f, vWhite, 0);
        rIcon.SetColor(g_vDifficultyExitColor);
    }
    CSWGuiBorderParams& rIconHilight = m_cIconHilight.m_cParams;
    {
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        Vector vWhite(1.0f, 1.0f, 1.0f);
        rIconHilight = CSWGuiBorderParams(cCorner, cEdge, cFill, 0, 0, 1.0f, vWhite, 0);
        rIconHilight.SetColor(g_vAbilityColorGranted);
    }
    rIconHilight.SetFlags(1, 0, 1);
    {
        CSWGuiExtent extent;
        extent.left = 0;
        extent.top = 0;
        extent.width = nWidth;
        extent.height = 0x38;
        SetExtent(extent);
    }
}
