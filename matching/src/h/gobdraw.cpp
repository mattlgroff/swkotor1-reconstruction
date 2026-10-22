// Gob proxy / blur drawing (0x00474C90-0x00474EA0).

#include "h/renderpaths.h"

extern "C" __declspec(dllimport) void __stdcall glFogf(unsigned int, float);
extern "C" __declspec(dllimport) void __stdcall glFogfv(unsigned int, const float*);
extern "C" __declspec(dllimport) void __stdcall glScalef(float, float, float);

namespace GLRender {
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);    // 0x0046F060
}

extern int g_bTransformParts;       // 0x0078E5F0
extern unsigned int g_eGLFog;       // 0x0078D450
extern unsigned int g_eGLFogStart;  // 0x0078D484
extern unsigned int g_eGLFogEnd;    // 0x0078D488
extern unsigned int g_eGLFogDensity;    // 0x0078D48C
extern unsigned int g_eGLFogColor;  // 0x0078D490

struct FogSettings {
    char m_pad00[0x60];
    int m_bFogEnabled;              // +0x60
    float m_fFogStart;              // +0x64
    float m_fFogEnd;                // +0x68
    char m_pad6C[0x70 - 0x6c];
    float m_fFogDensity;            // +0x70
    Vector m_vFogColor;             // +0x74

    float GetFogStart() { return m_fFogStart; }
    float GetFogEnd() { return m_fFogEnd; }
    float GetFogDensity() { return m_fFogDensity; }
};

struct GobFade {
    char m_pad00[0x94];
    float m_fFade;                  // +0x94
};

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

class Part;

class Gob {
public:
    char m_pad00[0x68];
    FogSettings* m_pFog;            // +0x68
    Part* m_pPart;                  // +0x6c
    char m_pad70[0x78 - 0x70];
    Vector m_vPosition;             // +0x78
    Quaternion m_qOrientation;      // +0x84
    Vector m_vLastPosition;         // +0x94
    Quaternion m_qLastOrientation;  // +0xa0
    char m_padB0[0x170 - 0xb0];
    GobFade* m_pFade;               // +0x170
    char m_pad174[0x1b4 - 0x174];
    float m_fScale;                 // +0x1b4

    void PartDraw(Part* pPart, int nFlags);         // 0x00473EA0
    void ProxyPartDraw(Part* pPart, int nFlags);
    void RenderBlur();
};

// FUNCTION: SWKOTOR 0x00474C90
void Gob::ProxyPartDraw(Part*, int nFlags)
{
    if (m_pFade && m_pFade->m_fFade < 1.0f) {
        glEnable(g_eGLFog);
        float fFade = m_pFade->m_fFade;
        float fFade2 = fFade * fFade;
        Vector vColor = m_pFog->m_vFogColor * fFade2;
        glFogfv(g_eGLFogColor, &vColor.x);
        glFogf(g_eGLFogDensity, m_pFog->m_fFogDensity * 2.0f);
        glFogf(g_eGLFogStart, m_pFog->m_fFogStart * m_pFade->m_fFade);
        glFogf(g_eGLFogEnd, m_pFog->m_fFogEnd * m_pFade->m_fFade + 0.1f);
        PartDraw(m_pPart, nFlags);
        if (!m_pFog->m_bFogEnabled)
            glDisable(g_eGLFog);
        glFogf(g_eGLFogDensity, m_pFog->GetFogDensity());
        glFogf(g_eGLFogStart, m_pFog->GetFogStart());
        glFogf(g_eGLFogEnd, m_pFog->GetFogEnd());
        glFogfv(g_eGLFogColor, &m_pFog->m_vFogColor.x);
        return;
    }
    PartDraw(m_pPart, nFlags);
}

// FUNCTION: SWKOTOR 0x00474E10
void Gob::RenderBlur()
{
    glPushMatrix();
    if (g_bTransformParts)
        GLRender::Transform(&m_vPosition, &m_qOrientation, 1);
    if (m_fScale != 1.0f)
        glScalef(m_fScale, m_fScale, m_fScale);
    ProxyPartDraw(m_pPart, 1);
    m_vLastPosition = m_vPosition;
    m_qLastOrientation = m_qOrientation;
    glPopMatrix();
}
