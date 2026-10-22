// PartTriMesh flat / light-mapped render paths (0x0046F900-0x0046FB10).

#include "h/render.h"

// FUNCTION: SWKOTOR 0x0046F900
void RenderFlat(PartTriMesh* pPart)
{
    if (!g_bRenderFlat)
        return;
    if (!g_bRenderFlatKeepState) {
        glDisable(0xb50);
        glDisableClientState(0x8076);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    g_nFlatParts++;
    MdlNodeTriMesh* pMesh = pPart->m_pMesh;
    int nStrips = pMesh->m_lStripLengths.num;
    if (pMesh->m_nTexCoords < 0)
        glDisableClientState(g_eTexCoordArray);
    if (nStrips) {
        for (int i = 0; i < nStrips; i++) {
            int nLength = pMesh->m_lStripLengths.element[i];
            GLRender::DrawElements(pMesh->m_eMode, nLength, 0, pMesh->m_lStrips.element[i]);
            g_nFlatTriangles += nLength / 3;
        }
    }
    if (pMesh->m_eMode == 4 && pMesh->m_nIndices > 0)
        GLRender::DrawElements(3, pMesh->m_nIndices, pMesh->m_nIndexOffset, pMesh->m_pIndices);
    if (pMesh->m_nTexCoords < 0)
        glEnableClientState(g_eTexCoordArray);
    if (!g_bRenderFlatKeepState) {
        glEnable(0xb50);
        glEnableClientState(0x8076);
    }
}

// FUNCTION: SWKOTOR 0x0046FA40
void RenderLightMapped(PartTriMesh* pPart)
{
    if (!g_bRenderLightMapped)
        return;
    if (!g_bRegisterCombiners) {
        RenderFlat(pPart);
        return;
    }
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    if (pMesh->m_pMaterial->m_pTexture0->IsProcedural()) {
        RenderFlat(pPart);
        return;
    }
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    GLRender::ActivateTextureStage(1);
    glEnableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
    g_pfnCombiner(0x1a, pMesh->m_pMaterial->m_pTexture0, pMesh->m_pMaterial->m_pTexture1, 0, 0);
    RenderFlat(pPart);
    g_pfnCombiner(0x20, 0, 0, 0, 0);
    GLRender::ActivateTextureStage(1);
    glDisableClientState(g_eTexCoordArray);
    GLRender::ActivateTextureStage(0);
}

// FUNCTION: SWKOTOR 0x00470750
void RenderFlatNonTextured(PartTriMesh* pPart)
{
    if (!g_bRenderFlat)
        return;
    glDisableClientState(g_eTexCoordArray);
    glDisable(g_eGLTexture2DB);
    RenderFlat(pPart);
    glEnable(g_eGLTexture2DB);
    glEnableClientState(g_eTexCoordArray);
}

// FUNCTION: SWKOTOR 0x004707A0
void RenderFlatDecal(PartTriMesh* pPart)
{
    if (!g_bRenderFlat)
        return;
    if (!g_bRegisterCombiners) {
        RenderFlat(pPart);
        return;
    }
    pPart->m_pObject->m_pMaterial->BindTexture0();
    g_pfnCombiner(1, pPart->m_pObject->m_pMaterial->m_pTexture0, 0, 0, 0);
    if (AurGLCaps710())
        g_pfnActiveTexture(0x84C0);
    g_nActiveTextureUnit = 0;
    glEnableClientState(g_eTexCoordArray);
    glEnableClientState(g_eVertexArray);
    glDisableClientState(g_eNormalArray);
    glDisableClientState(g_eColorArray);
    RenderFlat(pPart);
    glEnableClientState(g_eNormalArray);
    glEnableClientState(g_eColorArray);
}
