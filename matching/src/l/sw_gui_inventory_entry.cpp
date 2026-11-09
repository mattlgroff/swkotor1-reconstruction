// CSWGuiInGameItemEntry (vtable 0x007565E8): an item line in the inventory
// and equipment list boxes. The constructor and destructors are in
// sw_gui_inventory_equip_entries.cpp; Draw needs the control's slot 0x38.
#define L_GUI_CONTROL_DRAW_SLOT
#include "l/sw_gui_inventory.h"

// FUNCTION: SWKOTOR 0x006B4D40
void CSWGuiInGameItemEntry::Draw(float fDelta)
{
    if (m_bFlag0) {
        GetHilight().Draw(fDelta);
        m_iconHighlight.Draw(fDelta);
    } else {
        GetBorder().Draw(fDelta);
        m_icon.Draw(fDelta);
    }
    m_iconOverlay.Draw(fDelta);
    m_cTextBox.Draw(fDelta);
    m_quantity.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006B4DB0
void CSWGuiInGameItemEntry::Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder, CSWGuiBorder* pHilight,
                                       int nWidth, int bPulse)
{
    m_cTextParams = pText->m_cParams;
    CSWGuiBorderParams& rBorder = m_cBorderParams;
    rBorder = pBorder->m_cParams;
    rBorder.SetColor(g_vDifficultyExitColor);
    CSWGuiBorderParams& rHilight = m_cHilightParams;
    rHilight = pHilight->m_cParams;
    rHilight.SetColor(g_vAbilityColorGranted);
    rHilight.SetFlags(bPulse, 1, 0);
    CSWGuiBorderParams& rIcon = m_icon.m_cParams;
    {
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        Vector vWhite(1.0f, 1.0f, 1.0f);
        rIcon = CSWGuiBorderParams(cCorner, cEdge, cFill, 0, 0, 1.0f, vWhite, 0);
        rIcon.SetColor(g_vDifficultyExitColor);
    }
    CSWGuiBorderParams& rIconHilight = m_iconHighlight.m_cParams;
    {
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        Vector vWhite(1.0f, 1.0f, 1.0f);
        rIconHilight = CSWGuiBorderParams(cCorner, cEdge, cFill, 0, 0, 1.0f, vWhite, 0);
        rIconHilight.SetColor(g_vAbilityColorGranted);
    }
    rIconHilight.SetFlags(bPulse, 0, 1);
    {
        CSWGuiExtent extent;
        extent.left = 0;
        extent.top = 0;
        extent.width = nWidth;
        extent.height = 0x38;
        SetExtent(extent);
    }
    m_bPulse = bPulse;
}

// FUNCTION: SWKOTOR 0x006B5060
void CSWGuiInGameItemEntry::SetCanUse(unsigned char nState)
{
    switch (nState) {
    case 0:
    case 1:
        m_cBorderParams.SetColor(g_vDifficultyExitColor);
        m_icon.m_cParams.SetColor(g_vDifficultyExitColor);
        m_cHilightParams.SetColor(g_vAbilityColorGranted);
        m_iconHighlight.m_cParams.SetColor(g_vAbilityColorGranted);
        break;
    case 2:
    case 3:
        m_cBorderParams.SetColor(g_vAbilityColorAvailable);
        m_icon.m_cParams.SetColor(g_vAbilityColorAvailable);
        m_cHilightParams.SetColor(g_vAbilityColorAvailable);
        m_iconHighlight.m_cParams.SetColor(g_vAbilityColorAvailable);
        break;
    }
    m_cHilightParams.SetFlags(m_bPulse, 1, 0);
    m_iconHighlight.m_cParams.SetFlags(m_bPulse, 0, 1);
    m_n398 = nState;
}
