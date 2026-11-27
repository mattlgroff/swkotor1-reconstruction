#include "l/sw_gui_skill_info.h"

// FUNCTION: SWKOTOR 0x006AC930
// NONMATCH (50 diff insns): the main remaining differences are temporary
// border-parameter stack slots.
void CSWGuiInGameSkillEntryV::SetStyle(CSWGuiLabelText* pText, CSWGuiBorder* pBorder,
                                      CSWGuiBorder* pHilight, int nWidth)
{
    m_cText = *(CSWGuiTextParams*)((char*)pText + 0x18);

    CSWGuiBorderParams* pBorderParams = &m_cBorder;
    pBorderParams->operator=(pBorder->m_cParams);
    pBorderParams->m_vColor = g_vDifficultyExitColor;

    CSWGuiBorderParams* pHilightParams = &m_cHilight.m_cParams;
    pHilightParams->operator=(pHilight->m_cParams);
    pHilightParams->m_vColor = g_vAbilityColorGranted;
    pHilightParams->SetFlags(1, 1, 0);

    {
        Vector white;
        white.x = 1.0f;
        white.y = 1.0f;
        white.z = 1.0f;
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        CSWGuiBorderParams cParams(cCorner, cEdge, cFill, 0, 0, 1.0f, white, 0);
        CSWGuiBorderParams* pParams = &m_cIconBorder.m_cParams;
        pParams->operator=(cParams);
        pParams->m_vColor = g_vAbilityColorAvailable;
    }
    {
        Vector white;
        white.x = 1.0f;
        white.y = 1.0f;
        white.z = 1.0f;
        CResRef cFill("lbl_hex_3");
        CResRef cEdge("");
        CResRef cCorner("");
        CSWGuiBorderParams cParams(cCorner, cEdge, cFill, 0, 0, 1.0f, white, 0);
        CSWGuiBorderParams* pParams = &m_cIconHilight.m_cParams;
        pParams->operator=(cParams);
        pParams->m_vColor = g_vAbilityColorGranted;
        pParams->SetFlags(1, 0, 1);
    }

    CSWGuiExtent extent;
    extent.left = 0;
    extent.top = 0;
    extent.width = nWidth;
    extent.height = 0x2a;
    SetExtent(extent);
}
