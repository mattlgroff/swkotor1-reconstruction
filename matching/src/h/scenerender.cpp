// Scene debug / gob render passes and fog setup
// (0x00450D60-0x004511F0).

#include "h/scene.h"

extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glFogi(unsigned int, int);
extern "C" __declspec(dllimport) void __stdcall glFogf(unsigned int, float);
extern "C" __declspec(dllimport) void __stdcall glFogfv(unsigned int, const float*);
extern "C" __declspec(dllimport) void __stdcall glPushMatrix();
extern "C" __declspec(dllimport) void __stdcall glPopMatrix();
extern "C" __declspec(dllimport) void __stdcall glBegin(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glEnd();
extern "C" __declspec(dllimport) void __stdcall glVertex3f(float, float, float);
extern "C" __declspec(dllimport) void __stdcall glColor3f(float, float, float);

void AurVertexProgramMakeCurrent();         // 0x004A23B0
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);    // 0x0046F6B0
void DrawDebugCircle(float fRadius, Vector* pCenter, Vector* pColor, float fScale, int n);  // 0x00431A50

extern unsigned int g_eGLTexture2D;     // 0x0078D44C
extern unsigned int g_eGLTexture2DB;    // 0x0078D458
extern unsigned int g_eGLLighting;      // 0x0078D45C
extern unsigned int g_eGLDepthTest;     // 0x0078D474
extern unsigned int g_eGLFog;           // 0x0078D450
extern unsigned int g_eGLFogStart;      // 0x0078D484
extern unsigned int g_eGLFogEnd;        // 0x0078D488
extern unsigned int g_eGLFogDensity;    // 0x0078D48C
extern unsigned int g_eGLFogColor;      // 0x0078D490
extern int g_aFogModes[3];              // 0x0073F25C
extern int g_bForceFog;                 // 0x007B930C
extern int g_bBlackFog;                 // 0x008305E0
extern int g_bShowAllPersonalSpace;     // 0x007B9314
extern int g_bShowPaths;                // 0x007B9318
extern float g_fZeroEpsilon;            // 0x0073D700

class GobPart {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void Render(int n);             // +0x10
    char pad_0x4[0x74];
    Vector m_vPosition;                     // +0x78
    Quaternion m_qOrientation;              // +0x84
    char pad_0x94[0x108 - 0x94];
    float m_fPersonalSpace;                 // +0x108
    float m_fCreaturePersonalSpace;         // +0x10c
    char pad_0x110[0x114 - 0x110];
    int m_nPathFloats;                      // +0x114
    Vector* m_pPath;                        // +0x118
    char pad_0x11c[0x154 - 0x11c];
    unsigned char m_bShowPersonalSpace;     // +0x154

    float GetCreaturePersonalSpace() { return m_fCreaturePersonalSpace; }
};

static inline const float* AsArray(const Vector& v)
{
    return &v.x;
}

// FUNCTION: SWKOTOR 0x00450D60
void Scene::RenderOnlyGobs()
{
    m_pLights->BeginFrame();
    glDisable(g_eGLTexture2D);
    AurVertexProgramMakeCurrent();
    for (int i = 0; i < m_lChildren.num; i++) {
        m_pLights->GetNearestLights(this, ((GobPart*)m_lChildren.element[i])->m_vPosition, 2.0f, 0);
        ((GobPart*)m_lChildren.element[i])->Render(0);
    }
}

// FUNCTION: SWKOTOR 0x00450DE0
void Scene::SetupFog()
{
    if (!g_bForceFog) {
        if (m_bFogEnabled) {
            glEnable(g_eGLFog);
            glFogi(0xb65, g_aFogModes[m_nFogMode == 0 ? 0 : (m_nFogMode == 1 ? 1 : 2)]);
            if (g_bBlackFog) {
                glFogfv(g_eGLFogColor, AsArray(Vector(0.0f, 0.0f, 0.0f)));
            } else
                glFogfv(g_eGLFogColor, &m_vFogColor.x);
            glFogf(g_eGLFogStart, GetFogStart());
            glFogf(g_eGLFogEnd, GetFogEnd());
            glFogf(g_eGLFogDensity, GetFogDensity());
        } else
            glDisable(g_eGLFog);
    } else {
        m_bFogEnabled = 1;
        glEnable(g_eGLFog);
        glFogi(0xb65, g_aFogModes[m_nFogMode == 0 ? 0 : (m_nFogMode == 1 ? 1 : 2)]);
        glFogfv(g_eGLFogColor, AsArray(Vector(1.0f, 0.0f, 0.0f)));
        m_vFogColor = Vector(1.0f, 0.0f, 0.0f);
        glFogf(g_eGLFogStart, 0.1f);
        m_fFogStart = 0.0f;
        m_fFogEnd = 20.0f;
        m_fFogDensity = 0.1f;
        glFogf(g_eGLFogEnd, 20.0f);
        glFogf(g_eGLFogDensity, 0.1f);
    }
}

// FUNCTION: SWKOTOR 0x00450FA0
void RenderPersonalSpace(Scene* pScene)
{
    glDisable(g_eGLDepthTest);
    for (int i = 0; i < pScene->m_lChildren.num; i++) {
        GobPart* pPart = (GobPart*)pScene->m_lChildren.element[i];
        if (!g_bShowAllPersonalSpace && !pPart->m_bShowPersonalSpace)
            continue;
        if (pPart->m_fPersonalSpace > g_fZeroEpsilon) {
            glPushMatrix();
            Transform(&pPart->m_vPosition, &pPart->m_qOrientation, 1);
            Vector vCenter(0.0f, 0.0f, 0.0f);
            Vector vColor(1.0f, 0.6f, 1.0f);
            DrawDebugCircle(0.001f, &vCenter, &vColor, 1.0f, 0);
            DrawDebugCircle(pPart->m_fPersonalSpace, &vCenter, &vColor, 1.0f, 0);
            vColor.y = 0.0f;
            DrawDebugCircle(pPart->GetCreaturePersonalSpace(), &vCenter, &vColor, 1.0f, 0);
            glPopMatrix();
        }
    }
    glEnable(g_eGLDepthTest);
}

// FUNCTION: SWKOTOR 0x004510E0
void RenderPaths(Scene* pScene)
{
    if (!g_bShowPaths)
        return;
    glDisable(g_eGLDepthTest);
    glDisable(g_eGLLighting);
    glDisable(g_eGLTexture2DB);
    glColor3f(0.6f, 1.0f, 1.0f);
    for (int i = 0; i < pScene->m_lChildren.num; i++) {
        GobPart* pPart = (GobPart*)pScene->m_lChildren.element[i];
        glBegin(3);
        for (int j = 0; j < pPart->m_nPathFloats / 3; j++)
            glVertex3f(pPart->m_pPath[j].x, pPart->m_pPath[j].y, pPart->m_pPath[j].z);
        glEnd();
    }
    glEnable(g_eGLLighting);
    glEnable(g_eGLTexture2DB);
    glEnable(g_eGLDepthTest);
}
