// static / dynamic part material checks (0x0046F410-0x0046F550).

#include "h/renderpaths.h"

struct PartMaterial {
    char m_pad00[0x40];
    unsigned char m_bDecal;         // +0x40
};

struct PartLightmap {
    char m_pad00[0x34];
    int m_nTexture;                 // +0x34
};

struct PartModelFlags {
    char m_pad00[0x50];
    unsigned char m_nFlags;         // +0x50 (bit 3: fade alpha)

    unsigned char IsFadeAlpha() { return (unsigned char)(m_nFlags >> 3) & 1; }
};

struct PartDyn {
    char m_pad00[0x44];
    PartMaterial* m_pMaterial;      // +0x44
    char m_pad48[0x58 - 0x48];
    PartModelFlags* m_pModel;       // +0x58
    char m_pad5C[0x64 - 0x5c];
    Vector m_vSelfIllum;            // +0x64
    char m_pad70[0x7c - 0x70];
    unsigned char m_bNoLightmap;    // +0x7c
    char m_pad7D[0x80 - 0x7d];
    PartLightmap* m_pLightmap;      // +0x80
    float m_fAlpha;                 // +0x84
    char m_pad88[0xc4 - 0x88];
    unsigned short m_bColorAnimated;    // +0xc4
    unsigned short m_bOffsetAnimated;   // +0xc6
    char m_padC8[0xcc - 0xc8];
    Vector m_vColor;                // +0xcc
    float m_fAnimAlpha;             // +0xd8
    float m_fFadeAlpha;             // +0xdc
    Vector m_vOffset;               // +0xe0
};

struct PartOwner {
    char m_pad00[0x8c];
    Vector m_vColor;                // +0x8c
};

extern Vector g_vWhite;             // 0x0078E894
extern Vector g_vEnvMapPosition;    // 0x00828074

// FUNCTION: SWKOTOR 0x0046F410
bool StaticPartNeedsMat(PartDyn* pPart, PartOwner* pOwner, unsigned char* pbLightmap,
                        unsigned char* pbNeedsMat)
{
    if (!pPart->m_bNoLightmap) {
        if (pPart->m_pLightmap)
            *pbLightmap = pPart->m_pLightmap->m_nTexture != 0;
        if (*pbLightmap) {
            *pbNeedsMat = 1;
            return true;
        }
    }
    if (!(pOwner->m_vColor == g_vWhite) || pPart->m_fAlpha != 1.0f
        || !(pPart->m_vSelfIllum == g_vEnvMapPosition) || pPart->m_pMaterial->m_bDecal)
        return true;
    return false;
}

// FUNCTION: SWKOTOR 0x0046F490
void DynamicPartCalculateMaterial(PartDyn* pPart, Vector* pColor, Vector* pOffset, float* pAlpha)
{
    if (pPart->m_bColorAnimated) {
        *pAlpha *= pPart->m_fAnimAlpha;
        *pColor = *pColor * pPart->m_vColor;
    }
    if (pPart->m_pModel->IsFadeAlpha())
        *pAlpha *= pPart->m_fFadeAlpha;
    if (pPart->m_bOffsetAnimated)
        *pOffset = pPart->m_vOffset + *pOffset;
}
