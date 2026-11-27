// environment-mapped bump render path (0x00470860).

#include "h/render.h"

extern "C" __declspec(dllimport) void __stdcall glPushAttrib(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glPopAttrib();

int AurTextureGetTID(CAurTexture* p);       // 0x00420BF0

struct CombinerCaps {
    char m_pad00[0x60];
    int m_bFogPass;                 // +0x60
};

#include "shared/gl_ext.h"

#include "shared/globals.h"
#include "shared/combiners.h"

// FUNCTION: SWKOTOR 0x00470860
void RenderEMBM(PartTriMesh* pPart)
{
    if (!g_bRenderEMBM)
        return;
    if (!g_bRegisterCombiners) {
        RenderFlat(pPart);
        return;
    }
    MdlNode* pNode;
    MdlNodeTriMesh* pMesh = pPart->m_pObject->GetTriMesh();
    pNode = pMesh->m_pNode;
    glEnable(g_eGLAlphaTest);
    glDisable(g_eGLTexture2D);
    bool bLightmap = false;
    if (pPart->m_pMesh->m_bLightmapped && g_bEMBMLightmaps) {
        CAurTexture* pTex = pMesh->m_pMaterial->m_pTexture1;
        bLightmap = true;
        if (pTex && !AurTextureGetTID(pTex))
            bLightmap = false;
    }
    bool bFog;
    if (!g_pCombinerCaps->m_bFogPass || !pNode->m_pOwner->GetNode()->m_bFogged) {
        bFog = false;
    } else {
        bFog = true;
        glDisable(g_eGLFog);
    }
    glBlendFunc(g_aBlendFactors[2], g_aBlendFactors[3]);
    if (pMesh->m_pMaterial->m_fWaterAlpha == g_fOne) {
        if (!pMesh->m_pMaterial->m_pTexture0->IsProcedural() && bLightmap) {
            if (g_bEMBMBasePass) {
                GLRender::ActivateTextureStage(1);
                glEnableClientState(g_eTexCoordArray);
                GLRender::ActivateTextureStage(0);
                g_pfnEnableCombiners(0x1a, pMesh->m_pMaterial->m_pTexture0, pMesh->m_pMaterial->m_pTexture1, 0, 0);
                glEnable(g_eGLTexture3D);
                g_pVPLightmapped->Bind(pMesh, 1);
                SetCombinerConstant(0xf, 1.0f, 1.0f, 1.0f, 0.0f);
                RenderFlat(pPart);
                g_pfnEnableCombiners(0x20, 0, 0, 0, 0);
                GLRender::ActivateTextureStage(1);
                glDisableClientState(g_eTexCoordArray);
                GLRender::ActivateTextureStage(0);
            }
        } else if (g_bEMBMBasePass) {
            g_pVPBase->Bind(pMesh, 1);
            glEnable(g_eGLTexture3D);
            g_pfnEnableCombiners(0, 0, 0, 0, 0);
            pMesh->m_pMaterial->BindTexture0();
            RenderFlat(pPart);
        }
    }
    glDisable(g_eGLTexture2D);
    glPushAttrib(0x6040);
    glEnable(g_eGLTexture3D);
    if (g_bEMBMBumpPass) {
        if (g_bARBVertexProgram) {
            glEnableVertexAttribArrayARB(0xc);
            glEnableVertexAttribArrayARB(0xd);
            glEnableVertexAttribArrayARB(0xe);
        } else {
            glEnableClientState(g_eTangentArray);
            glEnableClientState(g_eBinormalArray);
            glEnableClientState(g_eTexNormalArray);
        }
        glDisableClientState(g_eNormalArray);
        (bLightmap ? g_pVPBumpLightmapped : g_pVPBump)->Bind(pMesh, 0);
        SetCombinerConstant(0xf, 1.0f, 0.0f, 1.0f, 0.0f);
        g_pfnEnableCombiners(0x1d, pMesh->m_pMaterial->m_pBumpMap, 0, 0, pMesh->m_pMaterial->m_pEnvMap);
        g_pfnEnableCombiners(0x18, 0, 0, 0, 0);
        if (pMesh->m_pMaterial->m_fWaterAlpha < g_fOne) {
            glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
            float fAlpha = pMesh->m_pMaterial->m_fWaterAlpha;
            Vector vColor(1.0f, 1.0f, 1.0f);
            g_pfnSetCombinerConstant0(vColor, fAlpha);
        } else {
            glBlendFunc(g_aBlendFactors[6], g_aBlendFactors[2]);
        }
        if (g_bEMBMAdditive)
            glBlendFunc(g_aBlendFactors[2], g_aBlendFactors[3]);
        RenderFlat(pPart);
        g_pfnEnableCombiners(0, 0, 0, 0, 0);
        g_pfnEnableCombiners(0xd, 0, 0, 0, 0);
        g_pfnEnableCombiners(0x1c, 0, 0, 0, 0);
        if (g_bARBVertexProgram) {
            glDisableVertexAttribArrayARB(0xc);
            glDisableVertexAttribArrayARB(0xd);
            glDisableVertexAttribArrayARB(0xe);
        } else {
            glDisableClientState(g_eTangentArray);
            glDisableClientState(g_eBinormalArray);
            glDisableClientState(g_eTexNormalArray);
        }
        glEnableClientState(g_eNormalArray);
        SetCombinerConstant(0xf, 1.0f, 1.0f, 1.0f, 0.0f);
    }
    if (bFog) {
        glEnable(g_eGLFog);
        glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
        g_pfnEnableCombiners(2, pMesh->m_pMaterial->m_pTexture0, 0, 0, 0);
        glEnable(g_eGLTexture3D);
        g_pVPFog->Bind(pMesh, 0);
        RenderFlat(pPart);
    }
    glPopAttrib();
    SetCombinerConstant(0xf, 1.0f, 1.0f, 1.0f, 0.0f);
    glDisable(g_eGLTexture3D);
    g_pfnEnableCombiners(0, 0, 0, 0, 0);
    glEnable(g_eGLTexture2D);
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
}
