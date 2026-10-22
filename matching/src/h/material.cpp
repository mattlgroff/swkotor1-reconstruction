// Material bind / blend helpers (0x0047ABC0-0x0047B0E0).

#include "h/material.h"

// FUNCTION: SWKOTOR 0x0047ABC0 SYMBOL: ??1?$CExoArrayList@H@@QAE@XZ
template CExoArrayList<int>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x0047ADF0
int Material::BindTexture0()
{
    CheckTexture(m_tTexture.m_p->GetTexture());
    if (g_bUseLightMapTexture && m_tLightMap.m_p)
        m_tLightMap.m_p->Bind();
    else
        m_tTexture.m_p->Bind();
    return m_tTexture.m_p->GetTID();
}

// FUNCTION: SWKOTOR 0x0047AE60
int Material::BindTexture1()
{
    CheckTexture(m_tTexture1.m_p->GetTexture());
    m_tTexture1.m_p->Bind();
    return m_tTexture1.m_p->GetTID();
}

// FUNCTION: SWKOTOR 0x0047AEB0
int Material::BindEnvMap()
{
    CheckTexture(m_tEnvMap.m_p->GetTexture());
    m_tEnvMap.m_p->Bind();
    return m_tEnvMap.m_p->GetTID();
}

// FUNCTION: SWKOTOR 0x0047AF00
int Material::GetTextureTID()
{
    return m_tTexture.m_p->GetTID();
}

// FUNCTION: SWKOTOR 0x0047AF10
void Material::SetDownSampling(int nMax)
{
    char szField[20];
    sprintf(szField, "downsamplemax %d", nMax);
    AurTextureSetField(m_tTexture.m_p, szField);
}

// FUNCTION: SWKOTOR 0x0047AF50
int Material::IsBumpMapBumpTexture()
{
    if (m_tBumpMap.m_p)
        return m_tBumpMap.m_p->IsBumpTexture();
    return 0;
}

// FUNCTION: SWKOTOR 0x0047AF70
void Material::SetBlendingMode(int i)
{
    glBlendFunc(g_aBlendFactors[m_lSrcBlend[i]], g_aBlendFactors[m_lDstBlend[i]]);
    if (m_lSrcBlend[i] == 0 && m_lDstBlend[i] == 2)
        glDepthMask(0);
    else
        glDepthMask(1);
    if (g_bAlphaTest) {
        if (m_lSrcBlend[i] == 2 && m_lDstBlend[i] == 3)
            glAlphaFunc(g_eAlphaFunc, g_fAlphaRef);
        else
            glAlphaFunc(g_eAlphaFunc, 0.0f);
    }
}

// FUNCTION: SWKOTOR 0x0047B000
void Material::SetGUIBlendingMode()
{
    glBlendFunc(g_aBlendFactors[m_lSrcBlend[0]], g_aBlendFactors[m_lDstBlend[0]]);
    if (g_bAlphaTest) {
        if (m_lSrcBlend[0] == 2 && m_lDstBlend[0] == 3)
            glAlphaFunc(g_eAlphaFunc, g_fAlphaRef);
        else
            glAlphaFunc(g_eAlphaFunc, 0.0f);
    }
}

// FUNCTION: SWKOTOR 0x0047B070
void Material::SetMaterialColor(Vector vColor, float fAlpha)
{
    if (m_tTexture.m_p->HasAlpha()) {
        fAlpha = g_fMaterialAlpha * fAlpha;
        glMaterialfv(g_eMaterialFace, g_eMaterialDiffuse, AurMakeColor(&vColor, &fAlpha));
    }
}

// FUNCTION: SWKOTOR 0x0047B0C0
bool Material::HasCubeEnvMap()
{
    if (m_tEnvMap.m_p && m_tEnvMap.m_p->HasCubeMap())
        return true;
    return false;
}

// FUNCTION: SWKOTOR 0x0047B0E0
bool Material::HasEnvMap()
{
    if (m_tEnvMap.m_p && !m_tEnvMap.m_p->IsProcedural() && !m_tEnvMap.m_p->IsInvalid())
        return true;
    return false;
}
