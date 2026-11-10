// Gob::Render (Gob vtable 0x00741268 slot +0x10).

extern "C" __declspec(dllimport) void __stdcall glPushMatrix();
extern "C" __declspec(dllimport) void __stdcall glPopMatrix();
extern "C" __declspec(dllimport) void __stdcall glScalef(float, float, float);

#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))

class Vector {
public:
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Add(T t);                          // 0x00444FE0
};

class CHitInfo {
public:
    CHitInfo(int nType, int nMask, int nFlags); // 0x004578F0
    ~CHitInfo();                                // 0x00456B30

    char m_pad00[0x5d];
    unsigned char m_b5D;                        // +0x5d
    char m_pad5E[0x64 - 0x5e];
};

class Material {
public:
    void BindTexture0();                        // 0x0047ADF0
    void SetBlendingMode(int nMode);            // 0x0047AF70
};

class Model {
public:
    char m_pad00[0x50];
    unsigned short m_bb0 : 1;                   // +0x50
    unsigned short m_bb1 : 1;
    unsigned short m_bHidden : 1;               // bit 2, descriptive
    unsigned short m_bFadeable : 1;             // bit 3, descriptive

    unsigned char IsHidden() { return m_bHidden; }
    unsigned char IsFadeable() { return m_bFadeable; }
};

class Scene {
public:
    char m_pad00[0xa8];
    Vector m_vFadeTarget;                       // +0xa8, descriptive
};

class RenderCamera {
public:
    char m_pad00[0x7c];
    Vector m_vPosition;                         // +0x7c
};

class Part;

namespace GLRender {
void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);     // 0x0046F060
void DrawAxisAlignedFrame(const Vector& a, const Vector& b, const Vector& c);  // 0x0046EF10
}
void DrawSphereBound(float fRadius, Vector* pCenter, float* pColor, float fAlpha,
                     unsigned char bWire);      // 0x00431A50

class Gob {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c();
    virtual void Render(unsigned char bPropagate);  // +0x010
    virtual void v014(); virtual void v018(); virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04c();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05c();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07c();
    virtual void v080(); virtual void v084(); virtual void v088();
    virtual Gob* IntersectLine(Vector vStart, Vector vEnd, Vector* pHit, CHitInfo* pInfo);  // +0x08c
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c();
    virtual void v0a0(); virtual void v0a4(); virtual void v0a8(); virtual void v0ac();
    virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc();
    virtual void v0d0(); virtual void v0d4(); virtual void v0d8(); virtual void v0dc();
    virtual void v0e0(); virtual void v0e4(); virtual void v0e8(); virtual void v0ec();
    virtual void v0f0(); virtual void v0f4(); virtual void v0f8(); virtual void v0fc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12c();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13c();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14c();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15c();
    virtual void v160(); virtual void v164(); virtual void v168();
    virtual void DoEvent(char* szEvent, void* pData);  // +0x16c

    void ProxyPartDraw(Part* pPart, int nFlags);    // 0x00474C90

    __forceinline void KeepTransform()
    {
        m_vLastPosition = m_vPosition;
        m_qLastOrientation = m_qOrientation;
    }

    char m_pad04[0x13 - 0x04];
    unsigned char m_bRendered;                  // +0x13
    char m_pad14[0x58 - 0x14];
    Model* m_pModel;                            // +0x58
    char m_pad5C[0x6c - 0x5c];
    Part* m_pPart;                              // +0x6c
    char m_pad70[0x78 - 0x70];
    Vector m_vPosition;                         // +0x78
    Quaternion m_qOrientation;                  // +0x84
    Vector m_vLastPosition;                     // +0x94
    Quaternion m_qLastOrientation;              // +0xa0
    char m_padB0[0xc8 - 0xb0];
    short m_nShowBounds;                        // +0xc8, descriptive
    char m_padCA[0xdc - 0xca];
    float m_fFade;                              // +0xdc
    char m_padE0[0x108 - 0xe0];
    float m_fPersonalRadius;                    // +0x108
    float m_fCreatureRadius;                    // +0x10c
    void* m_pPath;                              // +0x110
    char m_pad114[0x120 - 0x114];
    Vector m_vBoundsMin;                        // +0x120
    Vector m_vBoundsMax;                        // +0x12c
    float m_fRadius;                            // +0x138
    Vector m_vCenter;                           // +0x13c
    Vector m_vBoundsColor;                      // +0x148
    char m_pad154;
    unsigned char m_bMotionBlur;                // +0x155
    char m_pad156[0x164 - 0x156];
    unsigned char m_bOwnMaterial;               // +0x164, descriptive
    char m_pad165;
    unsigned char m_bAlwaysRender;              // +0x166, descriptive
    char m_pad167;
    Material* m_pMaterial;                      // +0x168
    char m_pad16C[0x1b4 - 0x16c];
    float m_fScale;                             // +0x1b4
};

extern int g_bRenderGobs2;                      // 0x0078E860, descriptive
extern int g_bRenderHiddenModels;               // 0x0078E85C, descriptive
extern int g_bDrawPartAxes;                     // 0x00828024
extern int g_bGobFading;                        // 0x0082806C, descriptive
extern Scene* g_pScene;                         // 0x007B92A0
extern RenderCamera* g_pRenderCamera;           // 0x007BB4E8
extern float g_fFrameTime;                      // 0x0078E574
extern float g_fGobFadeTime;                    // 0x0078E5A0, descriptive
extern float g_fGobFadeMin;                     // 0x0078E858, descriptive
extern int g_nGobsRendered;                     // 0x00827FAC, descriptive
extern int g_bTransformParts;                   // 0x0078E5F0
extern int g_nDrawGobSpheres;                   // 0x00828060, descriptive
extern int g_bDrawGobBounds;                    // 0x0082805C, descriptive
extern CExoArrayList<Gob*> g_l7FBF90;           // 0x007FBF90 blur bucket

static inline float ReadFloat(const float& f)
{
    return f;
}

// FUNCTION: SWKOTOR 0x00478260
void Gob::Render(unsigned char bPropagate)
{
    if (!g_bRenderGobs2)
        return;
    if (!m_pPart) {
        KeepTransform();
        return;
    }
    if (!g_bRenderHiddenModels && m_pModel && m_pModel->IsHidden()) {
        KeepTransform();
        return;
    }
    if (!m_bAlwaysRender && !g_bDrawPartAxes) {
        KeepTransform();
        return;
    }
    if (g_bGobFading && m_pModel->IsFadeable() && g_pScene) {
        Vector vEye = g_pRenderCamera->m_vPosition;
        Vector vTarget = g_pScene->m_vFadeTarget;
        CHitInfo hit(0, 0xbfffffff, 0);
        hit.m_b5D = 0;
        Gob* pHit = IntersectLine(vEye, vTarget, &vTarget, &hit);
        if (pHit)
            m_fFade = m_fFade - g_fFrameTime / g_fGobFadeTime;
        else
            m_fFade = g_fFrameTime / g_fGobFadeTime + m_fFade;
        m_fFade = __max(g_fGobFadeMin, __min(1.0f, m_fFade));
    }
    DoEvent("GetPersonalRadius", &m_fPersonalRadius);
    DoEvent("GetCreatureRadius", &m_fCreatureRadius);
    DoEvent("GetPath", &m_pPath);
    g_nGobsRendered++;
    m_bRendered = 1;
    glPushMatrix();
    if (g_bTransformParts)
        GLRender::Transform(&m_vPosition, &m_qOrientation, 1);
    if (*(int*)&m_fScale != 0x3f800000)
        glScalef(m_fScale, m_fScale, m_fScale);
    if (m_bOwnMaterial) {
        m_pMaterial->BindTexture0();
        m_pMaterial->SetBlendingMode(0);
    }
    ProxyPartDraw(m_pPart, 0);
    if (m_bMotionBlur)
        g_l7FBF90.Add(this);
    else
        KeepTransform();
    if (g_nDrawGobSpheres == 1) {
        float aColor[3] = { 1.0f, 1.0f, 0.0f };
        DrawSphereBound(ReadFloat(m_fRadius), &m_vCenter, aColor, 1.0f, 1);
    }
    if (g_bDrawGobBounds || m_nShowBounds)
        GLRender::DrawAxisAlignedFrame(m_vBoundsMin, m_vBoundsMax, m_vBoundsColor);
    glPopMatrix();
    if (bPropagate)
        DoEvent("propagaterender", 0);
}
