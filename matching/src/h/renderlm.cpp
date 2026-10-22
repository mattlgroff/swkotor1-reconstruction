// light-mapped render path without pixel shaders (0x0046FB10).

#include "h/render.h"

static __forceinline void SetupLightmap(LightmapData* pLm)
{
    int nOffset = pLm->m_nOffset;
    int nStage = pLm->m_nStage + 1;
    int nLightmap = pLm->m_pOwner->GetNode()->m_nLightmap;
    SetLightmapTexCoordToStage(pLm->m_pTexCoords, pLm->m_pTexCoords2, nLightmap, nStage, pLm, nOffset);
}

static __forceinline void SetupDecal(LightmapData* pLm)
{
    int nOffset = pLm->m_nOffset;
    int nStage = pLm->m_nStage + 1;
    int nLightmap = pLm->m_pOwner->GetNode()->m_nLightmap;
    SetDecalTexCoordToStage0(pLm->m_pTexCoords, pLm->m_pTexCoords2, nLightmap, nStage, pLm, nOffset);
}

// FUNCTION: SWKOTOR 0x0046FB10
void RenderLightMappedNoShaders(PartTriMesh* pPart)
{
    if (!g_bRenderLightMappedNoShaders)
        return;
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    LightmapData* pLm = pMesh->m_pNode->GetLightmap();
    if (pMesh->m_pMaterial->m_pTexture0->IsProcedural()
        || pMesh->m_pMaterial->m_pTexture1->IsProcedural()
        || !g_bRegisterCombiners) {
        RenderFlat(pPart);
        return;
    }
    if (AurGLCaps710() && !pMesh->m_pMaterial->HasEnvMap()) {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glDisable(g_eGLLighting);
        _glBlendFunc(0, 1);
        GLRender::ActivateTextureUnit(0);
        glEnable(g_eGLTexture2DB);
        pMesh->m_pMaterial->BindTexture1();
        glTexEnvi(0x2300, 0x2200, 0x2101);
        GLRender::ActivateTextureUnit(1);
        glEnable(g_eGLTexture2DB);
        pMesh->m_pMaterial->BindTexture0();
        glTexEnvi(0x2300, 0x2200, 0x2100);
        GLRender::ActivateTextureUnit(0);
        GLRender::ActivateTextureStage(1);
        glEnableClientState(g_eTexCoordArray);
        SetupLightmap(pLm);
        RenderFlat(pPart);
        SetupDecal(pLm);
        GLRender::ActivateTextureStage(1);
        glDisableClientState(g_eTexCoordArray);
        GLRender::ActivateTextureStage(0);
        GLRender::ActivateTextureUnit(1);
        glTexEnvi(0x2300, 0x2200, 0x2100);
        glDisable(g_eGLTexture2DB);
        GLRender::ActivateTextureUnit(0);
        glTexEnvi(0x2300, 0x2200, 0x2100);
        pMesh->m_pMaterial->BindTexture0();
        _glBlendFunc(0, 1);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(g_eGLLighting);
        return;
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(g_eGLLighting);
    pMesh->m_pMaterial->BindTexture0();
    _glBlendFunc(2, 3);
    RenderFlat(pPart);
    pMesh->m_pMaterial->BindTexture1();
    _glBlendFunc(5, 3);
    SetupLightmap(pLm);
    RenderFlat(pPart);
    SetupDecal(pLm);
    if (pMesh->m_pMaterial->HasEnvMap() && AurGLCaps8B0()) {
        glDisable(g_eGLTexture2D);
        pMesh->m_pMaterial->BindTexture0();
        _glBlendFunc(3, 2);
        RenderFlat(pPart);
        pMesh->m_pMaterial->BindEnvMap();
        _glBlendFunc(6, 2);
        DoEnvMapTransform(pMesh->m_pMaterial->m_pEnvMap, 0);
        glTexGeni(0x2000, 0x2500, 0x8512);
        glTexGeni(0x2001, 0x2500, 0x8512);
        glTexGeni(0x2002, 0x2500, 0x8512);
        glEnable(g_eGLTexGenS);
        glEnable(g_eGLTexGenT);
        glEnable(g_eGLTexGenR);
        RenderFlat(pPart);
        UndoEnvMapTransform(pMesh->m_pMaterial->m_pEnvMap, 0);
        glDisable(g_eGLTexGenS);
        glDisable(g_eGLTexGenT);
        glDisable(g_eGLTexGenR);
        glEnable(g_eGLTexture2D);
    }
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(g_eGLLighting);
    _glBlendFunc(0, 1);
}
