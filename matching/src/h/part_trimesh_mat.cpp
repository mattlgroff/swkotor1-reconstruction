// PartTriMesh material colour state (0x00473900).

#include "h/renderpaths.h"

extern "C" __declspec(dllimport) void __stdcall glMaterialfv(unsigned int, unsigned int, const float*);

float* AurMakeColor(const Vector& vColor, const float& fAlpha);     // 0x004AAAB0
void AurSetVertexProgramParameter(unsigned int n, float x, float y, float z, float w);  // 0x0044F820
int AurVertexProgramsAvailable();           // 0x0045F770

class Material {
public:
    char m_pad00[0x40];
    unsigned char m_bDecal;         // +0x40

    void BindTexture0();                        // 0x0047ADF0
    void SetBlendingMode(int nMode);            // 0x0047AF70
    void SetMaterialColor(Vector vColor, float fAlpha);     // 0x0047B070
};

class Gob {
public:
    char m_pad00[0x164];
    unsigned char m_bCustomMaterial;    // +0x164, descriptive
};

class MdlNodeTriMesh {
public:
    char m_pad00[0x8c];
    Vector m_vAmbient;              // +0x8c
    Vector m_vDiffuse;              // +0x98
};

class Scene {
public:
    char m_pad00[0x60];
    int m_bFogEnabled;              // +0x60
    char m_pad64[0x88 - 0x64];
    Vector m_vAmbient;              // +0x88
};

class PartTriMesh {
public:
    virtual void V00();
    MdlNodeTriMesh* m_pNode;        // +0x04
    char m_pad08[0x40 - 0x08];
    Gob* m_pGob;                    // +0x40
    Material* m_pMaterial;          // +0x44
    char m_pad48[0x64 - 0x48];
    Vector m_vSelfIllum;            // +0x64
    char m_pad70[0x7d - 0x70];
    unsigned char m_bFog;           // +0x7d
    char m_pad7E[0x84 - 0x7e];
    float m_fAlpha;                 // +0x84

    void Mat();
};

bool StaticPartNeedsMat(PartTriMesh* pPart, MdlNodeTriMesh* pNode, unsigned char* pbLightmap,
                        unsigned char* pbNeedsMat);     // 0x0046F410
void DynamicPartCalculateMaterial(Gob* pGob, Vector* pColor, Vector* pOffset, float* pAlpha);  // 0x0046F490

extern Scene* g_pAnimatingScene;            // 0x007B92A0
extern unsigned int g_eGLFog;               // 0x0078D450
extern unsigned int g_eGLFront;             // 0x0073F2EC
extern unsigned int g_eGLAmbient;           // 0x0073F2FC
extern unsigned int g_eGLDiffuse;           // 0x0073F300
extern unsigned int g_eGLEmission;          // 0x0073F304
extern Vector g_vWhite;                     // 0x0078E894
extern unsigned char g_bTextureStateDirty;  // 0x0078E330, descriptive
extern unsigned char g_bLastMatDynamic;     // 0x0078E8BC, descriptive
// File-static: the store is scheduled after the m_bFog reload only when
// the compiler knows it cannot alias the part.
static unsigned char g_bMatFog;             // 0x0078E8BD, descriptive
extern float g_fLastMatAlpha;               // 0x0078E8B8, descriptive
extern int g_bNVVertexProgram;              // 0x0078E8C0
extern int g_bEMBMEnabled;                  // 0x0078E66C
extern int g_bMatDisabled;                  // 0x00828040, descriptive
extern int g_bMatForce;                     // 0x00828048, descriptive

// NONMATCH: identical except that cl enregisters the constant 1 in ebx
// (guard bit, fog flag and bDynamic); the original keeps immediates.
// FUNCTION: SWKOTOR 0x00473900
void PartTriMesh::Mat()
{
    MdlNodeTriMesh* pNode = m_pNode;
    static Vector s_vLastAmbient(-1.0f, -1.0f, -1.0f);
    static Vector s_vLastDiffuse(-1.0f, -1.0f, -1.0f);
    static Vector s_vLastEmission(-1.0f, -1.0f, -1.0f);
    float fAlpha = m_fAlpha;
    unsigned char bDynamic = 0;
    unsigned char bNeedsMat;
    if (g_pAnimatingScene && g_pAnimatingScene->m_bFogEnabled) {
        if (m_bFog) {
            glEnable(g_eGLFog);
            g_bMatFog = 1;
        }
        if (!m_bFog) {
            glDisable(g_eGLFog);
            g_bMatFog = 0;
        }
    }
    if (m_pGob) {
        if (!m_pGob->m_bCustomMaterial) {
            m_pMaterial->BindTexture0();
            m_pMaterial->SetBlendingMode(0);
        }
        bDynamic = 1;
    } else {
        bDynamic = StaticPartNeedsMat(this, pNode, &bDynamic, &bNeedsMat);
    }
    if (g_bMatDisabled)
        return;
    static Vector s_vAmbient(0.0f, 0.0f, 0.0f);
    static Vector s_vDiffuse(0.0f, 0.0f, 0.0f);
    static Vector s_vEmission(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    if (!bDynamic) {
        if (!g_bLastMatDynamic && !g_bMatForce)
            return;
        s_vAmbient.x = 1.0f;
        s_vAmbient.y = 1.0f;
        s_vAmbient.z = 1.0f;
        s_vDiffuse.x = 1.0f;
        s_vDiffuse.y = 1.0f;
        s_vDiffuse.z = 1.0f;
        s_vEmission.x = 0.0f;
        s_vEmission.y = 0.0f;
        s_vEmission.z = 0.0f;
    } else {
        s_vAmbient = pNode->m_vAmbient;
        s_vDiffuse = pNode->m_vDiffuse;
        s_vEmission = m_pMaterial->m_bDecal ? g_vWhite : m_vSelfIllum;
        if (m_pGob)
            DynamicPartCalculateMaterial(m_pGob, &s_vAmbient, &s_vEmission, &fAlpha);
    }
    if ((!g_bLastMatDynamic && !bDynamic)
        || (s_vLastAmbient == s_vAmbient && s_vLastDiffuse == s_vDiffuse
            && s_vLastEmission == s_vEmission && g_fLastMatAlpha == fAlpha
            && !g_bTextureStateDirty)) {
        if (!g_bMatForce)
            return;
    }
    if ((g_bNVVertexProgram || g_bEMBMEnabled) && AurVertexProgramsAvailable()) {
        Vector v = s_vEmission;
        if (g_pAnimatingScene && m_pGob)
            v = v + g_pAnimatingScene->m_vAmbient * s_vDiffuse;
        AurSetVertexProgramParameter(0x5d, v.x, v.y, v.z, 1.0f);
        AurSetVertexProgramParameter(0x5e, s_vAmbient.x, s_vAmbient.y, s_vAmbient.z, fAlpha);
        AurSetVertexProgramParameter(0x5f, s_vDiffuse.x, s_vDiffuse.y, s_vDiffuse.z, 1.0f);
    }
    glMaterialfv(g_eGLFront, g_eGLAmbient, AurMakeColor(s_vDiffuse, 1.0f));
    glMaterialfv(g_eGLFront, g_eGLDiffuse, AurMakeColor(s_vAmbient, fAlpha));
    glMaterialfv(g_eGLFront, g_eGLEmission, AurMakeColor(s_vEmission, 1.0f));
    if (m_pGob) {
        m_pMaterial->SetMaterialColor(s_vAmbient, fAlpha);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }
    g_bLastMatDynamic = bDynamic;
    s_vLastAmbient = s_vAmbient;
    s_vLastDiffuse = s_vDiffuse;
    s_vLastEmission = s_vEmission;
    g_fLastMatAlpha = fAlpha;
    g_bTextureStateDirty = 0;
}
