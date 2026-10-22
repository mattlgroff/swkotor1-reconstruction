// PartTriMesh::Draw render-path dispatch (0x00477830-0x00477B90).

#include "h/render.h"

class PartRenderer {
public:
    virtual void v00();
    virtual PartTriMesh* GetPart();     // +0x04
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void Update();              // +0x18
};

class TriMeshPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual int IsAlwaysDrawn();        // +0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84();
    virtual void SetupMaterial();       // +0x88
    virtual void RestoreMaterial();     // +0x8c
    virtual void SetupState();          // +0x90
    virtual void RestoreState();        // +0x94

    MdlNode* m_pNode;                   // +0x04
    char m_pad08[0x40 - 0x08];
    PartDynamic* m_pDynamic;            // +0x40
    Material* m_pMaterial;              // +0x44
    char m_pad48[0x70 - 0x48];
    PartRenderer* m_pRenderer;          // +0x70

    int GetRenderPath();                // 0x00470D30
    void Draw(int);
    void DrawPrepared(int);
};

int PartIsRenderable(TriMeshPart* pPart);   // 0x0046EE40

void RenderEMBM(PartTriMesh* pPart);                    // 0x00470860
void RenderLightMappedBumpSpecular(PartTriMesh* pPart); // 0x00475450
void RenderLightMappedBumpDiffuse(PartTriMesh* pPart);  // 0x00474EA0
void RenderNormalMapped(PartTriMesh* pPart);            // 0x00477050
void RenderBumpMapped(PartTriMesh* pPart);              // 0x00476690
void RenderBumpmapFallback(PartRenderer* pRenderer);    // 0x004713D0
void RenderEMLM(PartTriMesh* pPart);                    // 0x004703F0
void RenderNV1xEMLM(PartTriMesh* pPart);                // 0x00470590
void RenderEnvironmentMapped(PartTriMesh* pPart, int, unsigned char bDecal);   // 0x0046FF70
void RenderLightMappedNoShaders(PartTriMesh* pPart);    // 0x0046FB10
void RenderLightMapped(PartTriMesh* pPart);             // 0x0046FA40
void RenderEMLMVP(PartRenderer* pRenderer, TriMeshPart* pMesh);             // 0x004712B0
void RenderLightmappedVP(PartRenderer* pRenderer, TriMeshPart* pMesh);      // 0x004710B0
void RenderFlatVP(PartRenderer* pRenderer, TriMeshPart* pMesh);             // 0x00470FE0
void RenderEnvironmentMappedVP(PartRenderer* pRenderer, TriMeshPart* pMesh);    // 0x00471180

extern int g_bDrawAllParts;         // 0x0078E68C
extern int g_bDrawPreparedParts;    // 0x0078E844
extern VertexProgram* g_pVPSelfIllum;   // 0x00828014

// FUNCTION: SWKOTOR 0x00477830
void TriMeshPart::Draw(int)
{
    if (!g_bDrawAllParts && !IsAlwaysDrawn() && m_pDynamic)
        return;
    if (!PartIsRenderable(this))
        return;
    LightmapData* pLm = m_pNode->GetLightmap();
    m_pMaterial->HasBumpMap();
    m_pMaterial->IsBumpyShiny();
    pLm->HasTangentBases();
    SetupState();
    SetupMaterial();
    m_pRenderer->Update();
    switch (GetRenderPath()) {
    case 2:
        RenderEMBM(m_pRenderer->GetPart());
        break;
    case 3:
        RenderLightMappedBumpSpecular(m_pRenderer->GetPart());
        break;
    case 4:
        RenderLightMappedBumpDiffuse(m_pRenderer->GetPart());
        break;
    case 15:
        RenderNormalMapped(m_pRenderer->GetPart());
        break;
    case 5:
        RenderBumpMapped(m_pRenderer->GetPart());
        break;
    case 6:
        RenderBumpmapFallback(m_pRenderer);
        break;
    case 7:
        RenderEMLM(m_pRenderer->GetPart());
        break;
    case 9:
        RenderNV1xEMLM(m_pRenderer->GetPart());
        break;
    case 10:
        RenderEnvironmentMapped(m_pRenderer->GetPart(), 0, 0);
        break;
    case 14:
        RenderLightMappedNoShaders(m_pRenderer->GetPart());
        break;
    case 12:
        RenderLightMapped(m_pRenderer->GetPart());
        break;
    case 8:
        RenderEMLMVP(m_pRenderer, this);
        break;
    case 13:
        RenderLightmappedVP(m_pRenderer, this);
        break;
    case 1:
        RenderFlatVP(m_pRenderer, this);
        break;
    case 11:
        RenderEnvironmentMappedVP(m_pRenderer, this);
        break;
    case 0:
        RenderFlat(m_pRenderer->GetPart());
        break;
    }
    if (AurGLCaps680() && m_pDynamic && m_pDynamic->m_fSelfIllum != g_fZeroEpsilon
        && g_bRegisterCombiners) {
        g_nFlatParts++;
        MdlNodeTriMesh* pMesh = m_pRenderer->GetPart()->m_pMesh;
        glEnable(g_eGLTexture3D);
        SetCombinerConstant(0xc, m_pDynamic->m_fSelfIllum, 0.0f, 0.0f, 1.0f);
        g_pVPSelfIllum->Bind(this, 0);
        m_pMaterial->m_pTexture3->Bind();
        if (m_pMaterial->m_nSrcBlends > 1)
            m_pMaterial->SetBlendingMode(1);
        for (int i = 0; i < pMesh->m_lStripLengths.num; i++) {
            int nLength = pMesh->m_lStripLengths.element[i];
            GLRender::DrawElements(pMesh->m_eMode, nLength, 0, pMesh->m_lStrips.element[i]);
            g_nFlatTriangles += nLength / 3;
        }
        if (pMesh->m_eMode == 4 && pMesh->m_nIndices > 0)
            GLRender::DrawElements(3, pMesh->m_nIndices, pMesh->m_nIndexOffset, pMesh->m_pIndices);
        glDisable(g_eGLTexture3D);
    }
    RestoreMaterial();
    RestoreState();
}

// FUNCTION: SWKOTOR 0x00477B60
void TriMeshPart::DrawPrepared(int n)
{
    if (g_bDrawPreparedParts) {
        m_pNode->Prepare();
        Draw(n);
    }
}

int AurTextureGetTID(CAurTexture* p);       // 0x00420BF0
extern int g_bEMBMEnabled;          // 0x0078E66C
extern int g_bUseLightMapTexture;   // 0x0078E5FC
extern int g_bEMBMLightmaps;        // 0x0078E608
extern float g_fOne;                // 0x0073D764

// FUNCTION: SWKOTOR 0x00470D30
int TriMeshPart::GetRenderPath()
{
    LightmapData* pLm = m_pNode->GetLightmap();
    unsigned char bBump = m_pMaterial->HasBumpMap();
    unsigned char bNormal = m_pMaterial->HasNormalMap();
    unsigned char bBumpyShiny = m_pMaterial->IsBumpyShiny();
    unsigned char bTangent = pLm->HasTangentBases();
    unsigned char bEnv = m_pMaterial->HasEnvMap();
    unsigned char bLm = pLm->m_bLightmapped;
    unsigned char bTransparent = 1;
    if (!(m_pMaterial->m_fWaterAlpha < g_fOne))
        bTransparent = 0;
    CAurTexture* pLmTex = m_pMaterial->m_pTexture1;
    if (pLmTex && !AurTextureGetTID(pLmTex))
        bLm = 0;
    if (!bEnv && m_pDynamic && m_pDynamic->m_pEnvMap)
        bEnv = 1;
    int nPath = 0;
    if (!AurGLCaps5E0())
        nPath = 1;
    if (bBumpyShiny && bBump && bTangent && g_bEMBMEnabled) {
        if (AurNV2xLevelShadersAvailable())
            nPath = 2;
        else if (bLm) {
            if (AurNV2xLevelShadersAvailable()) {
                if (AurGLCaps5E0())
                    nPath = 7;
                else
                    nPath = 8;
            } else if (bTransparent)
                nPath = 10;
            else if (aurATIFragmentShadersBumpMapAvailable())
                nPath = 9;
            else
                nPath = 14;
        } else
            nPath = 10;
    } else if (bLm && bBump && bTangent && AurNV2xLevelShadersAvailable() && g_bUseLightMapTexture
               && g_bEMBMLightmaps) {
        if (m_pMaterial->m_nRenderType)
            nPath = 3;
        else
            nPath = 4;
    } else if (bBump && bTangent && g_bUseLightMapTexture && AurNV2xLevelShadersAvailable()) {
        if (AurGLCaps5A0())
            nPath = 5;
        else
            nPath = 6;
    } else if (bNormal && !bTangent && g_bUseLightMapTexture && AurNV2xLevelShadersAvailable()) {
        nPath = 15;
    } else if (bLm && bEnv && g_bEMBMLightmaps) {
        if (AurNV2xLevelShadersAvailable()) {
            if (AurGLCaps5E0())
                nPath = 7;
            else
                nPath = 8;
        } else if (aurATIFragmentShadersBumpMapAvailable())
            nPath = 9;
        else
            nPath = 14;
    } else if (bEnv) {
        nPath = 10;
    } else if (bLm && g_bEMBMLightmaps) {
        if (AurNV2xLevelShadersAvailable()) {
            if (AurGLCaps5E0())
                nPath = 12;
            else
                nPath = 13;
        } else
            nPath = 14;
    } else if (bBump && bTangent && g_bUseLightMapTexture && !AurNV2xLevelShadersAvailable()) {
        nPath = 6;
    }
    if (nPath == 10 && !AurGLCaps5E0())
        nPath = 11;
    return nPath;
}
