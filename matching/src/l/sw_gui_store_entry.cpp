// CSWGuiStoreItemEntry (vtable 0x00756850): an item line in the store's
// shop and inventory list boxes. The constructor and destructors live with
// the inventory entries (sw_gui_inventory_equip_entries.cpp).
#include "l/sw_gui_store.h"

// FUNCTION: SWKOTOR 0x006B5270
void CSWGuiStoreItemEntry::SetExtent(const CSWGuiExtent& extent)
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
    m_icon.SetExtent(cIcon);
    m_iconHighlight.SetExtent(cIcon);
    m_iconOverlay.SetExtent(cIcon);
    CSWGuiExtent cCount = cIcon;
    int nWidth = m_quantity.m_cParams.m_sText.GetLength() > 2 ? 0x2a : 0x15;
    cCount.height = 0x13;
    cCount.width = nWidth;
    cCount.left += 0x38 - nWidth;
    cCount.top += 0x25;
    m_quantity.SetExtent(cCount);
    cIcon.width = m_extent.width - 0x38;
    cIcon.left += 0x38;
    GetBorder().SetExtent(cIcon);
    GetHilight().SetExtent(cIcon);
    m_cTextBox.SetExtent(IntersectExtents(GetBorder().GetInnerExtent(), GetHilight().GetInnerExtent()));
}

// FUNCTION: SWKOTOR 0x006B53F0
void CSWGuiStoreItemEntry::Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder, CSWGuiBorder* pHilight, int nWidth)
{
    m_cTextParams = pText->m_cParams;
    CSWGuiBorderParams& rBorder = m_cBorderParams;
    rBorder = pBorder->m_cParams;
    rBorder.SetColor(g_vDifficultyExitColor);
    CSWGuiBorderParams& rHilight = m_cHilightParams;
    rHilight = pHilight->m_cParams;
    rHilight.SetColor(g_vAbilityColorGranted);
    rHilight.SetFlags(1, 1, 0);
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

// FUNCTION: SWKOTOR 0x006B7270
void CSWGuiStoreItemEntry::SetItem(unsigned long oidItem, int bUnused)
{
    CResRef cIcon;
    CExoString sName;
    CExoString sCount;
    CExoString sUnusedC;
    CExoString sUnusedD;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    pServer->GetItemByGameObjectID(oidItem);
    m_oidItem = oidItem;
    if (oidItem != 0x7f000000) {
        CSWSItem* pItem = pServer->GetItemByGameObjectID(oidItem);
        if (pItem) {
            cIcon = pItem->GetIcon();
            pItem->m_sLocName.GetString(g_pAppManager->m_pClientExoApp->GetClientLanguage(), &sName, 0);
            m_cTextParams.SetText(sName);
            int nStack = pItem->m_nStackSize;
            if (nStack > 1) {
                if (nStack >= 100) {
                    {
                        CResRef cFill("lbl_hex_7");
                        m_icon.m_cParams.SetFillImage(cFill, 0);
                    }
                    {
                        CResRef cFill("lbl_hex_7");
                        m_iconHighlight.m_cParams.SetFillImage(cFill, 0);
                    }
                } else {
                    {
                        CResRef cFill("lbl_hex_6");
                        m_icon.m_cParams.SetFillImage(cFill, 0);
                    }
                    {
                        CResRef cFill("lbl_hex_6");
                        m_iconHighlight.m_cParams.SetFillImage(cFill, 0);
                    }
                }
                sCount.Format("%d", nStack);
            } else {
                {
                    CResRef cFill("lbl_hex_3");
                    m_icon.m_cParams.SetFillImage(cFill, 0);
                }
                {
                    CResRef cFill("lbl_hex_3");
                    m_iconHighlight.m_cParams.SetFillImage(cFill, 0);
                }
            }
            {
                CResRef cFont("fnt_d16x16");
                m_quantity.m_cParams = CSWGuiTextParams(sCount, cFont, g_vDifficultyExitColor, 0x22, 1.0f);
            }
        } else {
            m_cTextParams.SetText(g_pAppManager->m_pClientExoApp->GetGUIString(0x16b));
            cIcon = "inone";
        }
    } else {
        m_cTextParams.SetText(g_pAppManager->m_pClientExoApp->GetGUIString(0x16b));
        cIcon = "inone";
    }
    CSWGuiBorderParams& rOverlay = m_iconOverlay.m_cParams;
    rOverlay.m_bStyle0 = 0;
    rOverlay.m_bStyle1 = 1;
    rOverlay.SetFillImage(cIcon, 0);
}
