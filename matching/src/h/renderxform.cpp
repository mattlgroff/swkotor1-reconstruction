// transform gates and env-map texture matrix (0x0046F6B0-0x0046F870).

#include "h/renderpaths.h"
#include "h/glcaps.h"

extern "C" __declspec(dllimport) void __stdcall glLoadIdentity();

namespace GLRender {
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);    // 0x0046F060
}

extern int g_bTransformParts;       // 0x0078E5F0

// FUNCTION: SWKOTOR 0x0046F6B0 SYMBOL: ?Transform@@YAXPBVVector@@PBVQuaternion@@H@Z
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce)
{
    if (g_bTransformParts)
        GLRender::Transform(pPos, pOri, bForce);
}

class TextureRotation {
public:
    char m_pad00[0x94];
    float m_fAngle;                 // +0x94

    void Push();
    void Pop();
};

// FUNCTION: SWKOTOR 0x0046F6C0
void TextureRotation::Push()
{
    float fAngle = m_fAngle;
    if (fAngle != g_fZeroEpsilon) {
        glMatrixMode(0x1702);
        glPushMatrix();
        glLoadIdentity();
        glRotatef(fAngle, 0.0f, 0.0f, 1.0f);
        glMatrixMode(0x1700);
    }
}

// FUNCTION: SWKOTOR 0x0046F710
void TextureRotation::Pop()
{
    if (m_fAngle != g_fZeroEpsilon) {
        glMatrixMode(0x1702);
        glPopMatrix();
        glMatrixMode(0x1700);
    }
}

class PartTriMesh {
public:
    char m_pad00[8];
    Vector m_vPosition;             // +0x08
    Quaternion m_qOrientation;      // +0x14
    char m_pad24[0x40 - 0x24];
    int m_bNoTransform;             // +0x40

    void TransformModelviewMatrix();
    void UntransformModelviewMatrix();
};

// FUNCTION: SWKOTOR 0x0046F740
void PartTriMesh::TransformModelviewMatrix()
{
    if (!m_bNoTransform) {
        glPushMatrix();
        if (g_bTransformParts)
            GLRender::Transform(&m_vPosition, &m_qOrientation, 0);
    }
}

// FUNCTION: SWKOTOR 0x0046F770
void PartTriMesh::UntransformModelviewMatrix()
{
    if (!m_bNoTransform)
        glPopMatrix();
}

class CAurTexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual unsigned char HasCubeMap();         // +0x0c
};

struct EnvMapSource {
    char m_pad00[0x88];
    Quaternion m_qOrientation;      // +0x88
};

typedef void (__stdcall* PFNCLIENTACTIVETEXTURE)(unsigned int);
extern PFNCLIENTACTIVETEXTURE g_pfnClientActiveTexture;    // 0x007BB80C
extern unsigned int g_nActiveTextureUnit;   // 0x007A6898
extern unsigned int g_eTextureMatrix;       // 0x0073F29C
extern unsigned int g_eModelviewMatrix;     // 0x0073F298
extern EnvMapSource* g_pEnvMapSource;       // 0x007BB4E8
extern Vector g_vEnvMapPosition;            // 0x00828074

// FUNCTION: SWKOTOR 0x0046F780
void DoEnvMapTransform(CAurTexture* pTexture, unsigned int nUnit)
{
    if (!AurGLCaps8B0())
        return;
    if (!pTexture->HasCubeMap())
        return;
    g_nActiveTextureUnit = nUnit;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
    glMatrixMode(g_eTextureMatrix);
    glPushMatrix();
    float aMatrix[16];
    if (g_pEnvMapSource) {
        BuildMatrix(aMatrix, &g_vEnvMapPosition, &g_pEnvMapSource->m_qOrientation);
    } else {
        Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
        BuildMatrix(aMatrix, &g_vEnvMapPosition, &q);
    }
    glMultMatrixf(aMatrix);
    glMatrixMode(g_eModelviewMatrix);
    g_nActiveTextureUnit = 0;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
}

// FUNCTION: SWKOTOR 0x0046F870
void UndoEnvMapTransform(CAurTexture* pTexture, unsigned int nUnit)
{
    if (!AurGLCaps8B0())
        return;
    if (!pTexture->HasCubeMap())
        return;
    g_nActiveTextureUnit = nUnit;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
    glMatrixMode(g_eTextureMatrix);
    glPopMatrix();
    glMatrixMode(g_eModelviewMatrix);
    g_nActiveTextureUnit = 0;
    if (AurGLCaps710())
        g_pfnClientActiveTexture(g_nActiveTextureUnit + 0x84C0);
}
