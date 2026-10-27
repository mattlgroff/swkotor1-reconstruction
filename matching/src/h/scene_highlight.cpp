// Scene::DoHighlightTriggers (0x00456300): stencil-masked trigger volume
// highlight. Its own unit so the list growth stays out of line.

#include <math.h>

extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int cap);
extern "C" __declspec(dllimport) void __stdcall glDisable(unsigned int cap);
extern "C" __declspec(dllimport) void __stdcall glColorMask(unsigned char r, unsigned char g,
                                                            unsigned char b, unsigned char a);
extern "C" __declspec(dllimport) void __stdcall glStencilOp(unsigned int fail, unsigned int zfail,
                                                            unsigned int zpass);
extern "C" __declspec(dllimport) void __stdcall glStencilMask(unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glStencilFunc(unsigned int func, int ref,
                                                              unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);
extern "C" __declspec(dllimport) void __stdcall glClearStencil(int s);
extern "C" __declspec(dllimport) void __stdcall glClear(unsigned int mask);
extern "C" __declspec(dllimport) void __stdcall glDepthMask(unsigned char flag);
extern "C" __declspec(dllimport) void __stdcall glPushMatrix();
extern "C" __declspec(dllimport) void __stdcall glPopMatrix();
extern "C" __declspec(dllimport) void __stdcall glFrontFace(unsigned int mode);
extern "C" __declspec(dllimport) void __stdcall glTranslatef(float x, float y, float z);
extern "C" __declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
extern "C" __declspec(dllimport) void __stdcall glColor4fv(const float* v);
extern "C" __declspec(dllimport) void __stdcall glBegin(unsigned int mode);
extern "C" __declspec(dllimport) void __stdcall glEnd();
extern "C" __declspec(dllimport) void __stdcall glVertex3fv(const float* v);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);       // 0x005E03C0 (pointer lists), 0x00421310 (Vector)
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

struct Vector {
    float x, y, z;
};

class Plane {
public:
    Vector n;
    float d;
};

Vector CrossProduct(const Vector& a, const Vector& b);  // 0x004AB210
Vector Normalize(const Vector& v);              // 0x004AB180
int AurGetAvailableStencilBits();               // 0x0045FD00

struct TriggerColor {
    float r, g, b;
};

// Trigger part (the scene trigger list at 0x0080AA74).
class TriggerPart {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20(); virtual void V24(); virtual void V28(); virtual void V2C();
    virtual void V30(); virtual void V34(); virtual void V38(); virtual void V3C();
    virtual void V40(); virtual void V44(); virtual void V48(); virtual void V4C();
    virtual void V50(); virtual void V54(); virtual void V58(); virtual void V5C();
    virtual void V60(); virtual void V64(); virtual void V68(); virtual void V6C();
    virtual void V70(); virtual void V74(); virtual void V78();
    virtual void DrawVolume();                  // +0x7c, descriptive
    char pad_0x4[0xa8 - 0x4];
    unsigned char m_bA8;                        // +0xa8
    char pad_0xa9[7];
    unsigned char m_bHighlightGroup;            // +0xb0, descriptive
    char pad_0xb1[3];
    TriggerColor m_cColor;                      // +0xb4
    float m_fPulseAmplitude;                    // +0xc0, descriptive
    unsigned char m_bPulse;                     // +0xc4, descriptive
    char pad_0xc5[3];
    float m_fPulseRate;                         // +0xc8, descriptive
};

class RenderCamera {
public:
    char pad_0x0[0x7c];
    Vector m_vPosition;                         // +0x7c
    char pad_0x88[0x1f8 - 0x88];
    CExoArrayList<Plane> m_lViewVolume;         // +0x1f8
};

class Scene {
public:
    char pad_0x0[0xb4];
    float m_fHighlightTime;                     // +0xb4, descriptive

    void DoHighlightTriggers(unsigned char bFlag);  // Scene vtable +0x118
};

extern RenderCamera* g_pRenderCamera;           // 0x007BB4E8
extern float g_fFrameTime;                      // 0x0078E574
extern int g_bHighlightTriggers;                // 0x0078E614, descriptive
extern CExoArrayList<TriggerPart*> g_l80AA74;   // 0x0080AA74 trigger parts
extern unsigned int g_nGLBlend;                 // 0x0078D448
extern unsigned int g_nGLAlphaTest;             // 0x0078D44C
extern unsigned int g_nGLTexture2D;             // 0x0078D458
extern unsigned int g_nGLDepthTest;             // 0x0078D45C
extern unsigned int g_nGLStencilTest;           // 0x0078D470
extern unsigned int g_aBlendFactors[];          // 0x0073F270
extern unsigned int g_eGLLequal;                // 0x0073F2AC
extern unsigned int g_nGLAlways;                // 0x0073F2BC
extern unsigned int g_eGLCW;                    // 0x0073F2C0
extern unsigned int g_eGLCCW;                   // 0x0073F2C4
extern unsigned int g_nGLKeep;                  // 0x0073F2C8
extern unsigned int g_eGLIncr;                  // 0x0073F2D4
extern unsigned int g_eGLDecr;                  // 0x0073F2D8
extern unsigned int g_aPrimitiveModes[];        // 0x0073F334

static __forceinline void ClearStencil()
{
    int nBits = AurGetAvailableStencilBits();
    if (nBits)
        glClearStencil((1 << (nBits - 1)) - 1);
    glStencilMask(0xff);
    glClear(0x400);
}

// NONMATCH: structure and calls match, but the original keeps the trigger
// list's element/count in ebp/edi around the out-of-line growth call and
// gives every cross/normalize temporary its own slot (0xcc-byte frame).
// FUNCTION: SWKOTOR 0x00456300
void Scene::DoHighlightTriggers(unsigned char bFlag)
{
    CExoArrayList<TriggerPart*> lTriggers;
    int nBits = AurGetAvailableStencilBits();
    if (!nBits)
        return;
    m_fHighlightTime = g_fFrameTime * 6.2831855f + m_fHighlightTime;
    if (g_bHighlightTriggers) {
        glDisable(g_nGLBlend);
        glDisable(g_nGLTexture2D);
        glDisable(g_nGLDepthTest);
        glDisable(g_nGLAlphaTest);
        for (int i = 0; i < g_l80AA74.num; i++) {
            TriggerPart* pTrigger = g_l80AA74.element[i];
            if (pTrigger->m_bA8 && bFlag == pTrigger->m_bHighlightGroup) {
                if (lTriggers.num == lTriggers.array_size)
                    lTriggers.Allocate(lTriggers.array_size ? lTriggers.array_size * 2 : 8);
                lTriggers.element[lTriggers.num++] = pTrigger;
            }
        }
        int nRef = 1 << (nBits - 1);
        for (int k = 0; k < lTriggers.num; k++) {
            ClearStencil();
            glStencilMask(0xff);
            glEnable(g_nGLStencilTest);
            glDepthMask(0);
            glColorMask(0, 0, 0, 0);
            glStencilFunc(g_nGLAlways, 0xf, 0xf);
            glStencilOp(g_nGLKeep, g_eGLIncr, g_nGLKeep);
            glFrontFace(g_eGLCW);
            lTriggers.element[k]->DrawVolume();
            glStencilOp(g_nGLKeep, g_eGLDecr, g_nGLKeep);
            glFrontFace(g_eGLCCW);
            lTriggers.element[k]->DrawVolume();
            glFrontFace(g_eGLCCW);
            glColorMask(1, 1, 1, 1);
            CExoArrayList<Vector> lQuad;
            lQuad.Add(Normalize(CrossProduct(g_pRenderCamera->m_lViewVolume.element[5].n,
                                             g_pRenderCamera->m_lViewVolume.element[2].n)));
            lQuad.Add(Normalize(CrossProduct(g_pRenderCamera->m_lViewVolume.element[3].n,
                                             g_pRenderCamera->m_lViewVolume.element[5].n)));
            lQuad.Add(Normalize(CrossProduct(g_pRenderCamera->m_lViewVolume.element[4].n,
                                             g_pRenderCamera->m_lViewVolume.element[3].n)));
            lQuad.Add(Normalize(CrossProduct(g_pRenderCamera->m_lViewVolume.element[2].n,
                                             g_pRenderCamera->m_lViewVolume.element[4].n)));
            glPushMatrix();
            Vector vEye = g_pRenderCamera->m_vPosition;
            glTranslatef(vEye.x, vEye.y, vEye.z);
            glEnable(g_nGLBlend);
            glBlendFunc(g_aBlendFactors[0], g_aBlendFactors[1]);
            glStencilFunc(g_eGLLequal, nRef, 0xffffffff);
            glStencilOp(g_nGLKeep, g_nGLKeep, g_nGLKeep);
            TriggerPart* pTrigger = lTriggers.element[k];
            if (pTrigger->m_bPulse) {
                float fPulse = ((float)sin(pTrigger->m_fPulseRate * m_fHighlightTime) + 1.0f)
                    * pTrigger->m_fPulseAmplitude * 0.5f;
                TriggerColor c = pTrigger->m_cColor;
                glColor4f(c.r, c.g, c.b, fPulse);
            } else {
                glColor4fv(&pTrigger->m_cColor.r);
            }
            glBegin(g_aPrimitiveModes[6]);
            for (int n = 0; n < lQuad.num; n++)
                glVertex3fv(&lQuad.element[n].x);
            glEnd();
            glPopMatrix();
            glDisable(g_nGLStencilTest);
            glDepthMask(1);
        }
        glEnable(g_nGLAlphaTest);
        glEnable(g_nGLBlend);
        glEnable(g_nGLTexture2D);
        glEnable(g_nGLDepthTest);
    }
}
