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

// Parses one inline markup tag at p (p[1] selects it): "/c" and "/f"
// restore the default colour / font, "c" sets an RGB colour from the next
// three bytes, "f" selects font p[2] - '0', "<" is an escaped bracket.
// Returns the tag length (0 when unknown).
// FUNCTION: SWKOTOR 0x004A1600
int CAurFontRender::ParseMarkup(const char* p, CAurFontRender** ppFont, CAurColor* pColor)
{
    *pColor = g_cDefaultStringColor;
    *ppFont = this;
    int nLength = 0;
    switch (p[1]) {
    case '/':
        switch (p[2]) {
        case 'f':
            *ppFont = 0;
            return 4;
        case 'c':
            *pColor = g_cDefaultStringColor;
            return 4;
        }
        break;
    case 'c':
        pColor->r = (unsigned char)p[2] * (1.0f / 255.0f);
        pColor->g = (unsigned char)p[3] * (1.0f / 255.0f);
        pColor->b = (unsigned char)p[4] * (1.0f / 255.0f);
        pColor->a = 1.0f;
        return 6;
    case 'f':
        *ppFont = g_aFonts.element[(unsigned char)p[2] - '0'];
        return 4;
    case '<':
        nLength = 1;
        break;
    }
    return nLength;
}
