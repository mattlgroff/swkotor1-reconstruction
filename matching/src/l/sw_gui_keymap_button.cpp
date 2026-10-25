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

CSWGuiKeyMapButton::~CSWGuiKeyMapButton()
{
}

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
        unsigned long* pColor = (unsigned long*)&m_cBorderParams.m_vColor;
        pColor[0] = *(unsigned long*)0x007A23FC;
        pColor[1] = *(unsigned long*)0x007A2400;
        pColor[2] = *(unsigned long*)0x007A2404;
        unsigned long* pButtonColor = (unsigned long*)&m_cEventButton.m_cBorderParams.m_vColor;
        pButtonColor[0] = *(unsigned long*)0x007A23FC;
        pButtonColor[1] = *(unsigned long*)0x007A2400;
        pButtonColor[2] = *(unsigned long*)0x007A2404;
    } else {
        unsigned long* pColor = (unsigned long*)&m_cBorderParams.m_vColor;
        pColor[0] = *(unsigned long*)0x007A2390;
        pColor[1] = *(unsigned long*)0x007A2394;
        pColor[2] = *(unsigned long*)0x007A2398;
        unsigned long* pButtonColor = (unsigned long*)&m_cEventButton.m_cBorderParams.m_vColor;
        pButtonColor[0] = *(unsigned long*)0x007A2390;
        pButtonColor[1] = *(unsigned long*)0x007A2394;
        pButtonColor[2] = *(unsigned long*)0x007A2398;
    }
}

// FUNCTION: SWKOTOR 0x006EC770
void CSWGuiKeyMapButton::SetUnchangable()
{
    m_bUnchangeable = 1;
    unsigned long* pColor = (unsigned long*)&m_cBorderParams.m_vColor;
    pColor[0] = *(unsigned long*)0x007A2384;
    pColor[1] = *(unsigned long*)0x007A2388;
    pColor[2] = *(unsigned long*)0x007A238C;
    m_cTextParams.SetColor(*(Vector*)0x007A2408);
    unsigned long* pHilightColor = (unsigned long*)&m_cHilightParams.m_vColor;
    pHilightColor[0] = *(unsigned long*)0x007A2384;
    pHilightColor[1] = *(unsigned long*)0x007A2388;
    pHilightColor[2] = *(unsigned long*)0x007A238C;
}
