// Options checkbox drawing and layout-load colour overrides.
#include "l/sw_gui_options_in_game.h"

extern Vector g_vAbilityColorGranted; // 0x007A23C0 (selected text)
extern Vector g_vDifficultyExitColor; // 0x007A23B4 (enabled text)
extern Vector g_vKeyMapLockedTextColor; // 0x007A2408 (disabled text)

// FUNCTION: SWKOTOR 0x006DE0E0
void CSWGuiOptionsCheckbox::Draw(float fDelta)
{
    if (m_nFlags & 1) {
        m_cTextParams.SetColor(g_vAbilityColorGranted);
        m_cTextParams.SetHighlighted(1);
    } else if (!(m_nFlags & 8)) {
        m_cTextParams.SetColor(g_vKeyMapLockedTextColor);
    } else {
        m_cTextParams.SetColor(g_vDifficultyExitColor);
        m_cTextParams.SetHighlighted(0);
    }
    CSWGuiButtonToggle::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006DE160
void CSWGuiOptionsCheckbox::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    CSWGuiButtonToggle::Load(pGFF, pStruct);
    m_cBorderParams.m_vColor = g_vDifficultyExitColor;
    m_cSelectedBorderParams.m_vColor = g_vDifficultyExitColor;
    m_cHilightParams.m_vColor = g_vAbilityColorGranted;
    m_cSelectedHilightParams.m_vColor = g_vAbilityColorGranted;
}
