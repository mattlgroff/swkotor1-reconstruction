// vertex-program render paths and small part helpers
// (0x00470FE0-0x00471560).

#include "h/render.h"

class PartRenderer {
public:
    virtual void v00();
    virtual PartTriMesh* GetPart();     // +0x04
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void Update();              // +0x18
};

typedef void (__stdcall* PFNPROGRAMPARAMETER)(unsigned int, unsigned int, float, float, float, float);
extern PFNPROGRAMPARAMETER g_pfnProgramEnvParameter4fARB;  // 0x007BB850
extern PFNPROGRAMPARAMETER g_pfnProgramParameter4fNV;      // 0x007BB7AC
extern int g_nVertexProgramContext;     // 0x00830560
extern char g_szNoVertexProgram[];      // 0x0079AB28
extern char g_szFlatVP[];               // 0x0078FEB8
extern char g_szEnvMappedVP[];          // 0x00790300
extern char g_szEMLMVP[];               // 0x007908B8

void RenderEnvironmentMapped(PartTriMesh* pPart, int, unsigned char bDecal);   // 0x0046FF70
void RenderEMLM(PartTriMesh* pPart);            // 0x004703F0
void RenderLightMapped(PartTriMesh* pPart);     // 0x0046FA40

static inline void SetVertexProgramParameter(unsigned int nIndex, float x, float y, float z, float w)
{
    if (g_bARBVertexProgram)
        g_pfnProgramEnvParameter4fARB(0x8620, nIndex, x, y, z, w);
    else
        g_pfnProgramParameter4fNV(0x8620, nIndex, x, y, z, w);
}

// FUNCTION: SWKOTOR 0x00470FE0
void RenderFlatVP(PartRenderer* pRenderer, MdlNodeTriMesh* pMesh)
{
    glEnable(g_eGLTexture3D);
    static VertexProgram* s_pProgram = new VertexProgram(
        g_bARBVertexProgram ? g_szFlatVP : g_szNoVertexProgram, g_nVertexProgramContext);
    s_pProgram->Bind(pMesh, 1);
    RenderFlat(pRenderer->GetPart());
    glDisable(g_eGLTexture3D);
}

// FUNCTION: SWKOTOR 0x004710B0
void RenderLightmappedVP(PartRenderer* pRenderer, MdlNodeTriMesh* pMesh)
{
    glEnable(g_eGLTexture3D);
    static VertexProgram* s_pProgram = new VertexProgram(
        g_bARBVertexProgram ? g_szFlatVP : g_szNoVertexProgram, g_nVertexProgramContext);
    s_pProgram->Bind(pMesh, 1);
    RenderLightMapped(pRenderer->GetPart());
    glDisable(g_eGLTexture3D);
}

// FUNCTION: SWKOTOR 0x00471180
void RenderEnvironmentMappedVP(PartRenderer* pRenderer, MdlNodeTriMesh* pMesh)
{
    glEnable(g_eGLTexture3D);
    static VertexProgram* s_pProgram = new VertexProgram(
        g_bARBVertexProgram ? g_szEnvMappedVP : g_szNoVertexProgram, g_nVertexProgramContext);
    s_pProgram->Bind(pMesh, 1);
    SetVertexProgramParameter(0xf, 0.0f, 0.0f, 1.0f, 0.0f);
    SetVertexProgramParameter(0x10, 0.0f, 2.0f, 3.0f, 1.0f);
    RenderEnvironmentMapped(pRenderer->GetPart(), 0, 0);
    glDisable(g_eGLTexture3D);
}

// FUNCTION: SWKOTOR 0x004712B0
void RenderEMLMVP(PartRenderer* pRenderer, MdlNodeTriMesh* pMesh)
{
    glEnable(g_eGLTexture3D);
    static VertexProgram* s_pProgram = new VertexProgram(
        g_bARBVertexProgram ? g_szEMLMVP : g_szNoVertexProgram, g_nVertexProgramContext);
    s_pProgram->Bind(pMesh, 1);
    SetVertexProgramParameter(0xf, 0.0f, 0.0f, 1.0f, 0.0f);
    SetVertexProgramParameter(0x10, 0.0f, 2.0f, 3.0f, 1.0f);
    RenderEMLM(pRenderer->GetPart());
    glDisable(g_eGLTexture3D);
}

// FUNCTION: SWKOTOR 0x004713D0
void RenderBumpmapFallback(PartRenderer* pRenderer)
{
    glDisable(g_eGLTexture2D);
    glBlendFunc(g_aBlendFactors[2], g_aBlendFactors[3]);
    RenderFlat(pRenderer->GetPart());
    glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
    glEnable(g_eGLTexture2D);
}

class PartTextureForwarder {
public:
    char m_pad00[0x10];
    CAurTexture* m_pTexture;        // +0x10

    int Forward(int n);
};

class CAurTextureEx {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84();
    virtual void SetFrame(int n);   // +0x88
};

// FUNCTION: SWKOTOR 0x00471430
int PartTextureForwarder::Forward(int n)
{
    ((CAurTextureEx*)m_pTexture)->SetFrame(n);
    return n;
}

extern "C" __declspec(dllimport) void __stdcall glColorMask(unsigned char, unsigned char, unsigned char, unsigned char);
extern "C" __declspec(dllimport) void __stdcall glColor4fv(const float*);
extern "C" __declspec(dllimport) void __stdcall glStencilMask(unsigned int);

float* AurMakeColor(Vector* pColor, float* pAlpha);     // 0x004AAAB0
extern Vector g_vBeamColor;         // 0x0078E8A0
extern float g_fBeamAlpha;          // 0x0078E6A0
extern int g_bBeamStencil;          // 0x0078E69C

// FUNCTION: SWKOTOR 0x00471450
void SetBeaming()
{
    glBlendFunc(g_aBlendFactors[2], g_aBlendFactors[0]);
    glColorMask(1, 1, 1, 1);
    glDisable(g_eGLLighting);
    glColor4fv(AurMakeColor(&g_vBeamColor, &g_fBeamAlpha));
    if (g_bBeamStencil)
        glStencilMask(0);
}

void DrawDebugAxis(float fSize, Vector* pPos, Vector* pDir, float fScale, int bFlag);    // 0x00431A50
extern int g_bDrawPartAxes;         // 0x00828024

class PartAxis {
public:
    char m_pad00[8];
    Vector m_vPosition;             // +0x08

    void DrawAxis(int);
};

// FUNCTION: SWKOTOR 0x004714B0
void PartAxis::DrawAxis(int)
{
    if (g_bDrawPartAxes) {
        Vector vDir(1.0f, 0.0f, 0.0f);
        DrawDebugAxis(0.1f, &m_vPosition, &vDir, 1.0f, 1);
    }
}

class PartAnimMesh {
public:
    char m_pad00[8];
    Vector m_vPosition;             // +0x08
    Quaternion m_qOrientation;      // +0x14
    char m_pad24[0x40 - 0x24];
    int m_bNoTransform;             // +0x40
    char m_pad44[0x70 - 0x44];
    PartRenderer* m_pRenderer;      // +0x70

    void Draw(int);
};

namespace GLRender {
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);    // 0x0046F060
}
extern int g_bTransformParts;       // 0x0078E5F0

// FUNCTION: SWKOTOR 0x00471500
void PartAnimMesh::Draw(int)
{
    if (!m_bNoTransform) {
        glPushMatrix();
        if (g_bTransformParts)
            GLRender::Transform(&m_vPosition, &m_qOrientation, 0);
    }
    m_pRenderer->Update();
    RenderFlat(m_pRenderer->GetPart());
    if (!m_bNoTransform)
        glPopMatrix();
}
