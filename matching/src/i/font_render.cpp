// CAurFontRender teardown (0x004A21D0) and AurDeleteAllFonts (0x004A22E0);
// descriptive names, class name from symbols.csv.
#include "i/font_render.h"

// FUNCTION: SWKOTOR 0x004A21D0
CAurFontRender::~CAurFontRender()
{
    CAurTexture* pTexture = m_pTexture.m_p;
    m_pTexture.Reset();
    AurTextureRelease(pTexture);
    m_pTexture.Reset();
    if (m_nIndex < 0) {
        return;
    }
    for (int i = m_nIndex + 1; i < g_aFonts.num; i++) {
        g_aFonts.element[i]->m_nIndex--;
    }
    g_aFonts.DelIndexInline(m_nIndex);
}

// FUNCTION: SWKOTOR 0x004A22E0
void AurDeleteAllFonts()
{
    for (int i = g_aFonts.num - 1; i >= 0; i--) {
        delete g_aFonts.element[i];
    }
}
