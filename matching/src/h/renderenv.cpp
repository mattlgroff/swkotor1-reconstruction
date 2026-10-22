// environment-mapped render paths (0x0046FF70-0x00470750).

#include "h/render.h"

static __forceinline void SetupEnvTexGen(CAurTexture* pTex)
{
    if (pTex->HasCubeMap()) {
        glTexGeni(0x2000, 0x2500, 0x8512);
        glTexGeni(0x2001, 0x2500, 0x8512);
        glTexGeni(0x2002, 0x2500, 0x8512);
        glEnable(g_eGLTexGenS);
        glEnable(g_eGLTexGenT);
        glEnable(g_eGLTexGenR);
    } else {
        glTexGeni(0x2000, 0x2500, 0x2402);
        glTexGeni(0x2001, 0x2500, 0x2402);
        glEnable(g_eGLTexGenS);
        glEnable(g_eGLTexGenT);
    }
}

// FUNCTION: SWKOTOR 0x0046FF70
void RenderEnvironmentMapped(PartTriMesh* pPart, int, unsigned char bDecal)
{
    if (!g_bRenderEnvMapped || !g_bRenderFlat)
        return;
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    PartDynamic* pDyn = pMesh->m_pDynamic;
    if (!g_bEnvMapTexturesOnly || AurTextureEnvMapDisabled()) {
        int nKind = pMesh->m_pMaterial->m_pTexture0->GetKind();
        if (nKind != 7 && nKind != 8) {
            RenderFlat(pPart);
            return;
        }
    }
    float fAlpha = pMesh->m_fAlpha;
    CAurTexture* pTex = 0;
    if (pDyn) {
        pTex = pDyn->m_pEnvMap;
        if (pDyn->m_bAlphaAnimated)
            fAlpha *= pDyn->m_fAlpha;
    }
    if (!pTex)
        pTex = pMesh->m_pMaterial->m_pEnvMap;
    if (g_bOnePassEnvMap && AurOnePassEnvMapAvailable()) {
        glDisable(g_eGLTexture2D);
        if (bDecal && AurGLCaps640())
            g_pfnCombiner(0x14, pMesh->m_pMaterial->m_pTexture0, pTex, 0, 0);
        else
            g_pfnCombiner(0x13, pMesh->m_pMaterial->m_pTexture0, pTex, 0, 0);
        DoEnvMapTransform(pTex, 1);
        Vector vColor(1.0f, 1.0f, 1.0f);
        g_pfnCombinerConst(&vColor, fAlpha);
        RenderFlat(pPart);
        g_pfnCombiner(0x20, 0, 0, 0, 0);
        UndoEnvMapTransform(pTex, 1);
        GLRender::ActivateTextureUnit(1);
        if (AurGLCaps8B0())
            glDisable(g_eGLTexCubeMap);
        GLRender::ActivateTextureUnit(0);
        glEnable(g_eGLTexture2D);
        return;
    }
    glDisable(g_eGLTexture2D);
    if (g_bEnvMapBlendAdd) {
        glBlendFunc(g_aBlendFactors[2], g_aBlendFactors[3]);
        RenderFlat(pPart);
        glBlendFunc(g_aBlendFactors[6], g_aBlendFactors[2]);
        DoEnvMapTransform(pTex, 0);
        pTex->Bind();
        SetupEnvTexGen(pTex);
        UndoEnvMapTransform(pTex, 0);
        RenderFlat(pPart);
        if (!pMesh->m_pDynamic)
            pMesh->m_pMaterial->BindTexture0();
    } else {
        glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
        DoEnvMapTransform(pTex, 0);
        pTex->Bind();
        SetupEnvTexGen(pTex);
        RenderFlat(pPart);
        UndoEnvMapTransform(pTex, 0);
        glDisable(g_eGLTexGenS);
        glDisable(g_eGLTexGenT);
        glDisable(g_eGLTexGenR);
        glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
        pMesh->m_pMaterial->BindTexture0();
        RenderFlat(pPart);
    }
    if (AurGLCaps8B0())
        GLRender::BindTexture(0xc, 0);
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    glEnable(g_eGLTexture2D);
    g_nActiveTextureUnit = 1;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
    glDisable(g_eGLTexGenS);
    glDisable(g_eGLTexGenT);
    glDisable(g_eGLTexGenR);
    if (AurGLCaps8B0())
        glDisable(g_eGLTexCubeMap);
    g_nActiveTextureUnit = 0;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
}

// FUNCTION: SWKOTOR 0x004703F0
void RenderEMLM(PartTriMesh* pPart)
{
    if (!g_bRenderEMLM || !g_bRenderEnvMapped || !g_bRenderFlat)
        return;
    if (!AurNV2xLevelShadersAvailable())
        return;
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    PartDynamic* pDyn = pMesh->m_pDynamic;
    if (!g_bEnvMapTexturesOnly || AurTextureEnvMapDisabled()) {
        int nKind = pMesh->m_pMaterial->m_pTexture0->GetKind();
        if (nKind != 7 && nKind != 8) {
            RenderFlat(pPart);
            return;
        }
    }
    float fAlpha = pMesh->m_fAlpha;
    CAurTexture* pTex = 0;
    if (pDyn) {
        pTex = pDyn->m_pEnvMap;
        if (pDyn->m_bAlphaAnimated)
            fAlpha *= pDyn->m_fAlpha;
    }
    if (!pTex)
        pTex = pMesh->m_pMaterial->m_pEnvMap;
    glDisable(g_eGLTexture2D);
    GLRender::ActivateTextureStage(1);
    glEnableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
    g_pfnCombiner(0x16, pMesh->m_pMaterial->m_pTexture0, pTex, pMesh->m_pMaterial->m_pTexture1, 0);
    DoEnvMapTransform(pTex, 2);
    Vector vColor(1.0f, 1.0f, 1.0f);
    g_pfnCombinerConst(&vColor, fAlpha);
    RenderFlat(pPart);
    UndoEnvMapTransform(pTex, 2);
    g_pfnCombiner(0x25, 0, 0, 0, 0);
    GLRender::ActivateTextureStage(1);
    glDisableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
    glEnable(g_eGLTexture2D);
    GLRender::ActivateTextureUnit(1);
    glDisable(g_eGLTexCubeMap);
    GLRender::ActivateTextureUnit(0);
}

// FUNCTION: SWKOTOR 0x00470590
void RenderNV1xEMLM(PartTriMesh* pPart)
{
    if (!aurATIFragmentShadersBumpMapAvailable())
        return;
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    if (pMesh->m_pDynamic)
        return;
    if (!g_bRegisterCombiners
        || pMesh->m_pMaterial->m_pTexture0->IsProcedural()
        || pMesh->m_pMaterial->m_pTexture1->IsProcedural()
        || pMesh->m_pMaterial->m_pEnvMap->IsProcedural()) {
        RenderFlat(pPart);
        return;
    }
    glDisable(g_eGLTexture2D);
    glDisable(g_eGLTexture3D);
    _glBlendFunc(2, 3);
    GLRender::ActivateTextureStage(1);
    glEnableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
    g_pfnCombiner(0x1a, pMesh->m_pMaterial->m_pTexture0, pMesh->m_pMaterial->m_pTexture1, 0, 0);
    RenderFlat(pPart);
    g_pfnCombiner(0x20, 0, 0, 0, 0);
    GLRender::ActivateTextureStage(1);
    glDisableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
    _glBlendFunc(2, 2);
    g_pfnCombiner(0x15, pMesh->m_pMaterial->m_pTexture0, pMesh->m_pMaterial->m_pEnvMap, 0, 0);
    DoEnvMapTransform(pMesh->m_pMaterial->m_pEnvMap, 1);
    Vector vColor(1.0f, 1.0f, 1.0f);
    g_pfnCombinerConst(&vColor, 1.0f);
    RenderFlat(pPart);
    g_pfnCombiner(0x20, 0, 0, 0, 0);
    glDisable(g_eGLTexGenS);
    glDisable(g_eGLTexGenT);
    glDisable(g_eGLTexGenR);
    UndoEnvMapTransform(pMesh->m_pMaterial->m_pEnvMap, 1);
    glEnable(g_eGLTexture2D);
    _glBlendFunc(0, 1);
}
