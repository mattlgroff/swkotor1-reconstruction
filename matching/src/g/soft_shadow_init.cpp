// InitializeSoftShadows: the scene / scratch textures and the two 512x512
// pbuffers the soft-shadow blur renders through; NV (texture rectangle)
// and ATI (fragment shader) variants.
// Behavioural reference: src/units/0545/949769dd56-soft_shadow.cpp.
extern "C" {
__declspec(dllimport) void __stdcall glGenTextures(int n, unsigned int* textures);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int texture);
__declspec(dllimport) void __stdcall glCopyTexImage2D(unsigned int target, int level,
                                                      unsigned int format, int x, int y,
                                                      int w, int h, int border);
__declspec(dllimport) void __stdcall glTexParameteri(unsigned int target, unsigned int pname,
                                                     int param);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void* __stdcall wglCreateContext(void* hdc);
__declspec(dllimport) int __stdcall wglShareLists(void* hglrc1, void* hglrc2);
__declspec(dllimport) int __stdcall wglMakeCurrent(void* hdc, void* hglrc);
}

#define GL_TEXTURE_2D 0x0de1
#define GL_TEXTURE_RECTANGLE_NV 0x84f5
#define GL_RGBA8 0x8058
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_CLAMP 0x2900
#define GL_FRAGMENT_SHADER_ATI 0x8920
#define GL_REG_0_ATI 0x8921
#define GL_REG_1_ATI 0x8922
#define GL_REG_2_ATI 0x8923
#define GL_REG_3_ATI 0x8924
#define GL_REG_4_ATI 0x8925
#define GL_CON_0_ATI 0x8941
#define GL_CON_1_ATI 0x8942
#define GL_CON_2_ATI 0x8943
#define GL_CON_3_ATI 0x8944
#define GL_MAD_ATI 0x8968
#define GL_MUL_ATI 0x8964
#define GL_SWIZZLE_STR_ATI 0x8976
#define GL_TEXTURE0_ARB 0x84c0
#define GL_TEXTURE1_ARB 0x84c1
#define GL_TEXTURE2_ARB 0x84c2
#define GL_TEXTURE3_ARB 0x84c3

typedef void*(__stdcall* PFNWGLGETPBUFFERDCARB)(void* hPbuffer);
typedef int(__stdcall* PFNWGLMAKECONTEXTCURRENTARB)(void* hDrawDC, void* hReadDC, void* hglrc);
typedef unsigned int(__stdcall* PFNGLGENFRAGMENTSHADERSATI)(unsigned int range);
typedef void(__stdcall* PFNGLBINDFRAGMENTSHADERATI)(unsigned int id);
typedef void(__stdcall* PFNGLBEGINFRAGMENTSHADERATI)();
typedef void(__stdcall* PFNGLENDFRAGMENTSHADERATI)();
typedef void(__stdcall* PFNGLSAMPLEMAPATI)(unsigned int dst, unsigned int interp,
                                           unsigned int swizzle);
typedef void(__stdcall* PFNGLCOLORFRAGMENTOP2ATI)(unsigned int op, unsigned int dst,
                                                  unsigned int dstMask, unsigned int dstMod,
                                                  unsigned int arg1, unsigned int arg1Rep,
                                                  unsigned int arg1Mod, unsigned int arg2,
                                                  unsigned int arg2Rep, unsigned int arg2Mod);
typedef void(__stdcall* PFNGLCOLORFRAGMENTOP3ATI)(unsigned int op, unsigned int dst,
                                                  unsigned int dstMask, unsigned int dstMod,
                                                  unsigned int arg1, unsigned int arg1Rep,
                                                  unsigned int arg1Mod, unsigned int arg2,
                                                  unsigned int arg2Rep, unsigned int arg2Mod,
                                                  unsigned int arg3, unsigned int arg3Rep,
                                                  unsigned int arg3Mod);
typedef void(__stdcall* PFNGLALPHAFRAGMENTOP2ATI)(unsigned int op, unsigned int dst,
                                                  unsigned int dstMod, unsigned int arg1,
                                                  unsigned int arg1Rep, unsigned int arg1Mod,
                                                  unsigned int arg2, unsigned int arg2Rep,
                                                  unsigned int arg2Mod);
typedef void(__stdcall* PFNGLALPHAFRAGMENTOP3ATI)(unsigned int op, unsigned int dst,
                                                  unsigned int dstMod, unsigned int arg1,
                                                  unsigned int arg1Rep, unsigned int arg1Mod,
                                                  unsigned int arg2, unsigned int arg2Rep,
                                                  unsigned int arg2Mod, unsigned int arg3,
                                                  unsigned int arg3Rep, unsigned int arg3Mod);

extern PFNWGLGETPBUFFERDCARB wglGetPbufferDCARB;                // 0x007bb6b8
extern PFNWGLMAKECONTEXTCURRENTARB wglMakeContextCurrentARB;    // 0x007bb818
extern PFNGLGENFRAGMENTSHADERSATI glGenFragmentShadersATI;      // 0x007bb754
extern PFNGLBINDFRAGMENTSHADERATI glBindFragmentShaderATI;      // 0x007bb6cc
extern PFNGLBEGINFRAGMENTSHADERATI glBeginFragmentShaderATI;    // 0x007bb7f4
extern PFNGLENDFRAGMENTSHADERATI glEndFragmentShaderATI;        // 0x007bb554
extern PFNGLSAMPLEMAPATI glSampleMapATI;                        // 0x007bb620
extern PFNGLCOLORFRAGMENTOP2ATI glColorFragmentOp2ATI;          // 0x007bb840
extern PFNGLALPHAFRAGMENTOP2ATI glAlphaFragmentOp2ATI;          // 0x007bb5e8
extern PFNGLCOLORFRAGMENTOP3ATI glColorFragmentOp3ATI;          // 0x007bb7fc
extern PFNGLALPHAFRAGMENTOP3ATI glAlphaFragmentOp3ATI;          // 0x007bb680

extern unsigned char g_bSoftShadowsAllowed;     // 0x0078dae9
extern int g_bSoftShadows;                      // 0x0078e420
extern int g_nScreenWidth;                      // 0x0078d1d4
extern int g_nScreenHeight;                     // 0x0078d1d8
extern int g_nPowerWidth;                       // 0x007a6890
extern int g_nPowerHeight;                      // 0x007a6894
extern unsigned int g_nRootTexture;             // 0x007a687c
extern unsigned int g_nSoftShadowTexture;       // 0x007a68f8
extern void* g_hSoftShadowPbuffer1;             // 0x007a68e8
extern void* g_hSoftShadowDC1;                  // 0x007a68ec
extern void* g_hSoftShadowContext1;             // 0x007a68f0
extern unsigned int g_nSoftShadowTexture1;      // 0x007a68f4
extern void* g_hSoftShadowPbuffer2;             // 0x007a68fc
extern void* g_hSoftShadowDC2;                  // 0x007a6900
extern void* g_hSoftShadowContext2;             // 0x007a6904
extern unsigned int g_nSoftShadowTexture2;      // 0x007a6908
extern unsigned int g_nBloomShaderATI;          // 0x007a6928 (also the soft-shadow blur)
extern void* g_hGLContext;                      // 0x007a6854
extern void* g_hGLDC;                           // 0x007a47e4

int AurSoftShadowsAvailable();                  // 0x0045f7b0
int AurATISoftShadowsAvailable();               // 0x0045f7e0
void* CreatePBuffer(int nWidth, int nHeight);   // 0x00426560
void FindSmallestPowerOf2WidthHeight(int nWidth, int nHeight, int* pWidth,
                                     int* pHeight);     // 0x00427450

__forceinline void SetClampLinear(unsigned int nTarget)
{
    glTexParameteri(nTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(nTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(nTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(nTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

// FUNCTION: SWKOTOR 0x00426cc0
void InitializeSoftShadows()
{
    if (AurSoftShadowsAvailable() && g_bSoftShadowsAllowed && g_bSoftShadows) {
        if (!g_nRootTexture) {
            glGenTextures(1, &g_nRootTexture);
            glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nRootTexture);
            glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, g_nScreenWidth,
                             g_nScreenHeight, 0);
            glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
        }
        glGenTextures(1, &g_nSoftShadowTexture);
        glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_nSoftShadowTexture);
        glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, g_nScreenWidth,
                         g_nScreenHeight, 0);
        SetClampLinear(GL_TEXTURE_RECTANGLE_NV);

        g_hSoftShadowPbuffer1 = CreatePBuffer(512, 512);
        g_hSoftShadowDC1 = wglGetPbufferDCARB(g_hSoftShadowPbuffer1);
        g_hSoftShadowContext1 = wglCreateContext(g_hSoftShadowDC1);
        wglShareLists(g_hGLContext, g_hSoftShadowContext1);
        wglMakeCurrent(g_hSoftShadowDC1, g_hSoftShadowContext1);
        glGenTextures(1, &g_nSoftShadowTexture1);
        glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture1);
        SetClampLinear(GL_TEXTURE_2D);

        g_hSoftShadowPbuffer2 = CreatePBuffer(512, 512);
        g_hSoftShadowDC2 = wglGetPbufferDCARB(g_hSoftShadowPbuffer2);
        g_hSoftShadowContext2 = wglCreateContext(g_hSoftShadowDC2);
        wglShareLists(g_hGLContext, g_hSoftShadowContext2);
        wglMakeCurrent(g_hSoftShadowDC2, g_hSoftShadowContext2);
        glGenTextures(1, &g_nSoftShadowTexture2);
        glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture2);
        SetClampLinear(GL_TEXTURE_2D);

        wglMakeCurrent(g_hGLDC, g_hGLContext);
        return;
    }
    if (!AurATISoftShadowsAvailable() || !g_bSoftShadowsAllowed || !g_bSoftShadows)
        return;
    FindSmallestPowerOf2WidthHeight(g_nScreenWidth, g_nScreenHeight, &g_nPowerWidth,
                                    &g_nPowerHeight);
    if (!g_nRootTexture) {
        glGenTextures(1, &g_nRootTexture);
        glBindTexture(GL_TEXTURE_2D, g_nRootTexture);
        SetClampLinear(GL_TEXTURE_2D);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, g_nPowerWidth, g_nPowerHeight, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glGenTextures(1, &g_nSoftShadowTexture);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, g_nPowerWidth, g_nPowerHeight, 0);
    SetClampLinear(GL_TEXTURE_2D);

    g_hSoftShadowPbuffer1 = CreatePBuffer(512, 512);
    g_hSoftShadowDC1 = wglGetPbufferDCARB(g_hSoftShadowPbuffer1);
    g_hSoftShadowContext1 = g_hGLContext;
    wglMakeContextCurrentARB(g_hSoftShadowDC1, g_hSoftShadowDC1, g_hSoftShadowContext1);
    glGenTextures(1, &g_nSoftShadowTexture1);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture1);
    SetClampLinear(GL_TEXTURE_2D);
    wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);

    g_hSoftShadowPbuffer2 = CreatePBuffer(512, 512);
    g_hSoftShadowDC2 = wglGetPbufferDCARB(g_hSoftShadowPbuffer2);
    g_hSoftShadowContext2 = g_hGLContext;
    wglMakeContextCurrentARB(g_hSoftShadowDC2, g_hSoftShadowDC2, g_hSoftShadowContext2);
    glGenTextures(1, &g_nSoftShadowTexture2);
    glBindTexture(GL_TEXTURE_2D, g_nSoftShadowTexture2);
    SetClampLinear(GL_TEXTURE_2D);
    wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);

    glEnable(GL_FRAGMENT_SHADER_ATI);
    g_nBloomShaderATI = glGenFragmentShadersATI(1);
    glBindFragmentShaderATI(g_nBloomShaderATI);
    glBeginFragmentShaderATI();
    glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
    glSampleMapATI(GL_REG_3_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
    glColorFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, 0, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glAlphaFragmentOp2ATI(GL_MUL_ATI, GL_REG_4_ATI, 0, GL_REG_0_ATI, 0, 0, GL_CON_0_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_4_ATI, 0, 0, GL_REG_1_ATI, 0, 0, GL_CON_1_ATI, 0, 0,
                          GL_REG_4_ATI, 0, 0);
    glAlphaFragmentOp3ATI(GL_MAD_ATI, GL_REG_4_ATI, 0, GL_REG_1_ATI, 0, 0, GL_CON_1_ATI, 0, 0,
                          GL_REG_4_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_1_ATI, 0, 0, GL_REG_2_ATI, 0, 0, GL_CON_2_ATI, 0, 0,
                          GL_REG_4_ATI, 0, 0);
    glAlphaFragmentOp3ATI(GL_MAD_ATI, GL_REG_1_ATI, 0, GL_REG_2_ATI, 0, 0, GL_CON_2_ATI, 0, 0,
                          GL_REG_4_ATI, 0, 0);
    glColorFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, 0, 0, GL_REG_3_ATI, 0, 0, GL_CON_3_ATI, 0, 0,
                          GL_REG_1_ATI, 0, 0);
    glAlphaFragmentOp3ATI(GL_MAD_ATI, GL_REG_0_ATI, 0, GL_REG_3_ATI, 0, 0, GL_CON_3_ATI, 0, 0,
                          GL_REG_1_ATI, 0, 0);
    glEndFragmentShaderATI();
    glDisable(GL_FRAGMENT_SHADER_ATI);
}
