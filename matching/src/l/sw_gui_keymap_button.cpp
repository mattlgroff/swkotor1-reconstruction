// CSWGuiKeyMapButton methods and lifecycle, vtable 0x007593C8.
#include "l/sw_gui_keymap_button.h"

// FUNCTION: SWKOTOR 0x006EC630
CSWGuiKeyMapButton::CSWGuiKeyMapButton(CSWGuiInGameOptKeyMappings* pOwner)
    : CSWGuiButton(), m_cEventButton()
{
    m_pOwner = pOwner;
    m_nOriginalKey = 0xFFFFFFFFul;
    m_nSortPosition = -1;
    m_cEventButton.m_pOwner = pOwner;
    m_nId = 0;
    m_bChanged = 0;
    m_nNewKey = 0;
    m_bUnchangeable = 0;
    m_nPage = 0;
    m_nClassFlags = 0;
    m_cEventButton.m_pParentControl = this;
    CSWGuiControl* pChild = (CSWGuiControl*)m_cEventButton.IsSelectable();
    AddChildControl(pChild);
}

// The regular destructor 0x006ED570 is in sw_gui_keymap_button_dtor_novtable.cpp.
// FUNCTION: SWKOTOR 0x006ED550 SYMBOL: ??_GCSWGuiKeyMapButton@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006EC380
CSWGuiControl* CSWGuiKeyMapButton::HitCheckMouse(int x, int y)
{
    CSWGuiControl* pControl = CSWGuiControl::HitCheckMouse(x, y);
    if (pControl) {
        return pControl;
    }
    return m_cEventButton.HitCheckMouse(x, y);
}

// FUNCTION: SWKOTOR 0x006EC6E0
void CSWGuiKeyMapButton::UpdateColor()
{
    if (m_bChanged) {
        m_cBorderParams.m_vColor = g_vKeyMapChangedColor;
        m_cEventButton.m_cBorderParams.m_vColor = g_vKeyMapChangedColor;
    } else {
        m_cBorderParams.m_vColor = g_vKeyMapNormalColor;
        m_cEventButton.m_cBorderParams.m_vColor = g_vKeyMapNormalColor;
    }
}

// FUNCTION: SWKOTOR 0x006EC770
void CSWGuiKeyMapButton::SetUnchangable()
{
    m_bUnchangeable = 1;
    m_cBorderParams.m_vColor = g_vKeyMapLockedColor;
    m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
    m_cHilightParams.m_vColor = g_vKeyMapLockedColor;
}
