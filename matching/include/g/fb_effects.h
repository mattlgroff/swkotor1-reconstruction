// Framebuffer effects (soft shadows, overlays), 0x0042d140-0x00437a60.
#ifndef G_FB_EFFECTS_H
#define G_FB_EFFECTS_H

extern "C" {
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int texture);
__declspec(dllimport) void __stdcall glCopyTexSubImage2D(unsigned int target, int level, int xoff,
                                                         int yoff, int x, int y, int w, int h);
__declspec(dllimport) void __stdcall glFlush();
__declspec(dllimport) void __stdcall glClearColor(float r, float g, float b, float a);
__declspec(dllimport) void __stdcall glClear(unsigned int mask);
__declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
__declspec(dllimport) void __stdcall glTexCoord2f(float s, float t);
__declspec(dllimport) void __stdcall glVertex3f(float x, float y, float z);
__declspec(dllimport) void __stdcall glTexParameteri(unsigned int target, unsigned int pname, int param);
__declspec(dllimport) void __stdcall glMatrixMode(unsigned int mode);
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glLoadIdentity();
__declspec(dllimport) void __stdcall glOrtho(double l, double r, double b, double t, double n, double f);
__declspec(dllimport) void __stdcall glViewport(int x, int y, int w, int h);
__declspec(dllimport) void __stdcall glStencilFunc(unsigned int func, int ref, unsigned int mask);
__declspec(dllimport) void __stdcall glStencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass);
__declspec(dllimport) void __stdcall glDrawBuffer(unsigned int mode);
__declspec(dllimport) int __stdcall wglMakeCurrent(void* hdc, void* hglrc);
__declspec(dllimport) void __stdcall glDepthMask(unsigned char flag);
__declspec(dllimport) void __stdcall glPushAttrib(unsigned int mask);
__declspec(dllimport) void __stdcall glPopAttrib();
__declspec(dllimport) void __stdcall glStencilMask(unsigned int mask);
__declspec(dllimport) void __stdcall glVertex4f(float x, float y, float z, float w);
}

#define GL_TEXTURE_2D 0x0de1
#define GL_TEXTURE_RECTANGLE_NV 0x84f5
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100
#define GL_QUADS 7
#define GL_TEXTURE0_ARB 0x84c0
#define GL_TEXTURE1_ARB 0x84c1
#define GL_TEXTURE2_ARB 0x84c2
#define GL_TEXTURE3_ARB 0x84c3
#define GL_VERTEX_PROGRAM_ARB 0x8620
#define GL_BLEND 0x0be2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_LIGHTING 0x0b50
#define GL_STENCIL_TEST 0x0b90
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_DEPTH_TEST 0x0b71
#define WGL_FRONT_LEFT_ARB 0x2083
#define GL_CON_1_ATI 0x8942
#define GL_CON_0_ATI 0x8941
#define GL_PER_STAGE_CONSTANTS_NV 0x8535
#define GL_COMBINER0_NV 0x8550
#define GL_COMBINER1_NV 0x8551
#define GL_COMBINER2_NV 0x8552
#define GL_CONSTANT_COLOR0_NV 0x852a
#define GL_CONSTANT_COLOR1_NV 0x852b
#define GL_CON_2_ATI 0x8943
#define GL_CON_3_ATI 0x8944
#define GL_LESS 0x0201
#define GL_KEEP 0x1e00
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_FRAGMENT_SHADER_ATI 0x8920

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

// GL extension entry points (function-pointer globals).
typedef void(__stdcall* PFNGLACTIVETEXTUREARB)(unsigned int tex);
typedef void(__stdcall* PFNGLMULTITEXCOORD2FARB)(unsigned int tex, float s, float t);
typedef void(__stdcall* PFNGLBINDFRAGMENTSHADERATI)(unsigned int id);
extern PFNGLACTIVETEXTUREARB glActiveTextureARB;           // 0x007bb80c
extern PFNGLMULTITEXCOORD2FARB glMultiTexCoord2fARB;       // 0x007bb6e8
extern PFNGLBINDFRAGMENTSHADERATI glBindFragmentShaderATI; // 0x007bb6cc
typedef void(__stdcall* PFNGLBINDPROGRAMARB)(unsigned int target, unsigned int id);
typedef void(__stdcall* PFNGLPROGRAMENVPARAMETER4FARB)(unsigned int target, unsigned int index,
                                                       float x, float y, float z, float w);
typedef int(__stdcall* PFNWGLBINDTEXIMAGEARB)(void* hPbuffer, int iBuffer);
typedef void(__stdcall* PFNGLSETFRAGMENTSHADERCONSTANTATI)(unsigned int dst, const float* value);
extern PFNWGLBINDTEXIMAGEARB wglBindTexImageARB;                      // 0x007bb7b8
extern PFNWGLBINDTEXIMAGEARB wglReleaseTexImageARB;                   // 0x007bb6d0
extern PFNGLSETFRAGMENTSHADERCONSTANTATI glSetFragmentShaderConstantATI; // 0x007bb5d8
typedef int(__stdcall* PFNWGLMAKECONTEXTCURRENTARB)(void* hDrawDC, void* hReadDC, void* hglrc);
typedef void(__stdcall* PFNGLCOMBINERSTAGEPARAMETERFVNV)(unsigned int stage, unsigned int pname,
                                                         const float* params);
extern PFNGLCOMBINERSTAGEPARAMETERFVNV glCombinerStageParameterfvNV;  // 0x007bb6f8
extern PFNWGLMAKECONTEXTCURRENTARB wglMakeContextCurrentARB;          // 0x007bb818
extern PFNGLBINDPROGRAMARB g_pfnBindProgramARB;                       // 0x007bb788
extern PFNGLPROGRAMENVPARAMETER4FARB g_pfnProgramEnvParameter4fARB;   // 0x007bb850



inline Vector operator*(const Vector& v, float f)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

float* AurMakeColor(const Vector& vColor, const float& fAlpha);   // 0x004aaab0

class CAurTexture {
public:
    virtual ~CAurTexture();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual int Activate();                 // 0x48
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void Bind();                    // 0xb4

    unsigned char IsNull();                 // 0x00420600
};

int HasMultiTexture();                      // 0x0045f710
int AurGLCaps820();                         // 0x0045f820
int AurATIFrameBufferModificationsAvailable();   // 0x0045f860
void _glBlendFunc(int nSrc, int nDst);      // 0x00401670
extern const unsigned int g_aGLBlendFactors[];   // 0x0073f270
int AurGLCaps7E0();                         // 0x0045f7e0

extern unsigned int g_nSoftShadowTexture;   // 0x007a68f8
extern unsigned int g_nPowerWidth;          // 0x007a6890
extern unsigned int g_nPowerHeight;         // 0x007a6894
extern int g_nScreenWidth;                  // 0x0078d1d4
extern int g_nScreenHeight;                 // 0x0078d1d8
extern int g_nCurrentStage;                 // 0x007a6898
extern CAurTexture* g_pFilmNoiseTexture;    // 0x007a68d0
extern unsigned int g_nFilmNoiseShaderATI;  // 0x007a6920
extern unsigned int g_nDistortionShaderATI; // 0x007a6924
extern unsigned int g_nDistortionProgram;   // 0x007a68d4
extern CAurTexture* g_pDistortionTexture;   // 0x007a68d8
extern unsigned int g_nGLStencilTest;       // 0x0078d470
extern float g_fSpeedBlurRatio;             // 0x0078d43c
extern unsigned int g_nBloomTexture;        // 0x007a68c0
extern void* g_hBloomPbuffer;               // 0x007a68c4
extern unsigned int g_nRootTexture;         // 0x007a687c
extern unsigned int g_nAccumBlurShaderATI;  // 0x007a691c
extern void* g_hGLDC;                       // 0x007a47e4
extern void* g_hGLContext;                  // 0x007a6854
extern void* g_hBloomDC;                    // 0x007a68c8
extern void* g_hBloomContext;               // 0x007a68cc
extern float g_fSaturation;                 // 0x007a6884
extern Vector g_vSaturationTint;            // 0x0078db24
extern float g_fBloomAlpha;                 // 0x0078daf8
extern float g_fBloomOffset;                // 0x0078db00
extern float g_fBloomScale;                 // 0x0078db04
extern float g_afBloomKernelATI[25];        // 0x0078e230
extern float g_afBloomKernel[25];           // 0x0078e2a0
extern unsigned int g_nBloomShaderATI;      // 0x007a6928
extern void* g_ahBloomDC[6];                // 0x007a47ec
extern void* g_ahBloomContext[6];           // 0x007a4804
extern void* g_ahBloomPbuffer[6];           // 0x007a6824
extern unsigned int g_anBloomTexture[6];    // 0x007a47c4
extern int g_nBloomSource;                  // 0x007a6938
extern int g_nBloomLevel;                   // 0x0078db08
extern float g_fBloomSize;                  // 0x0078db0c
extern unsigned int g_nCapBlend;            // 0x0078d448
extern unsigned int g_nCapAlphaTest;        // 0x0078d44c
extern unsigned int g_nCapLighting;         // 0x0078d45c
extern unsigned int g_eGLDepthTest;         // 0x0078d474
extern int g_bConvolutionKeep;              // 0x0078db20
extern int g_bFrameBufferEffects;           // 0x0078d98c
extern int g_bSaturationAvailable;          // 0x0078d97c
extern char g_bSaturation;                  // 0x007a6881
extern char g_bFrameBufferActive;           // 0x007a6860
extern char g_bFrameBufferEnabled;          // 0x0078daf4
extern int g_bSpeedBlurAvailable;           // 0x0078d984
extern char g_bSpeedBlur;                   // 0x007a6880
extern char g_bBloomRendered;               // 0x0078d990
extern char g_bAccumFrameRendered;          // 0x007a698c
extern int g_bOverbrighten;                 // 0x007a68bc
extern int g_bConvolutionAvailable;         // 0x0078d974
extern int g_bDistortionAvailable;          // 0x0078d980
extern int g_bScanNoiseAvailable;           // 0x0078d978
extern char g_bScanNoise;                   // 0x007a6882
extern int g_bFilmNoiseAvailable;           // 0x0078d988
extern char g_bFilmNoise;                   // 0x007a6883
extern char g_bFrameRendered;               // 0x007a698d
extern unsigned int g_eGLFog;               // 0x0078d450
extern void* g_hSoftShadowPbuffer1;         // 0x007a68e8
extern void* g_hSoftShadowDC1;              // 0x007a68ec
extern void* g_hSoftShadowContext1;         // 0x007a68f0
extern unsigned int g_nSoftShadowTexture1;  // 0x007a68f4
extern void* g_hSoftShadowPbuffer2;         // 0x007a68fc
extern void* g_hSoftShadowDC2;              // 0x007a6900
extern void* g_hSoftShadowContext2;         // 0x007a6904
extern unsigned int g_nSoftShadowTexture2;  // 0x007a6908
extern int g_nBloomRamp0;                   // 0x007a6864
extern int g_nBloomRamp1;                   // 0x007a6868
extern int g_nBloomRamp2;                   // 0x007a686c

void FrameBufferModificationPixelShaders(int nMode, int a, int b, int c, int d);    // 0x004299c0
void FrameBufferModificationPixelShadersATI(int nMode, int a, int b, int c, int d); // 0x004293f0

void MipmapFilterATI(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                     unsigned int nTexture, float fSize);                      // 0x0042b370
void MipmapFilter(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                  unsigned int nTexture, float fSize);                         // 0x0042b5d0
void BloomFilterATI(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                    unsigned int nTexture, float fSize, int bScale);           // 0x0042b810
void BloomFilter(void* hDC, void* hRC, float fWidth, float fHeight, void* hPbuffer,
                 unsigned int nTexture, float fSize, int bScale);              // 0x0042bf80

void RenderSaturationAdjustmentATI(unsigned int nTexture);   // 0x004343d0
void RenderSaturationAdjustment(unsigned int nTexture);      // 0x00434890
void FrameBufferModificationsATI2(unsigned char nMask);      // 0x00436070
void RenderOverlayOverbrighten(int nTexture);                // 0x004340f0
void RenderOverlayOverbrightenATI(int nTexture);             // 0x00434240
void RenderOverlayFilmNoiseATI(unsigned int nTexture, unsigned char bStatic);   // 0x00432780
void RenderOverlayFilmNoise(unsigned int nTexture, unsigned char bStatic);      // 0x00432ad0
void RenderForceDistortionATI(unsigned int nTexture);        // 0x00432d70
void RenderForceDistortion(unsigned int nTexture);           // 0x004331d0
void RenderAccumulationBlurATI(unsigned int nTexture);       // 0x00433970
void RenderAccumulationBlur(unsigned int nTexture);          // 0x00433da0
void RenderFrameToTextureATI(unsigned int nTexture);         // 0x00433580
void RenderFrameToTexture(unsigned int nTexture);            // 0x004337a0
void RenderOverlayConvolutionATI(unsigned int nTexture);     // 0x0042c6e0
void RenderOverlayConvolution(unsigned int nTexture);        // 0x0042cc60

void FrameBufferModificationsATI(unsigned char nMask);       // 0x00435bd0

class GLRender {
public:
    static void ActivateTextureStage(int nStage);            // 0x0041fe80
};

void CopySoftShadowsATI();                  // 0x0042d140
void SetUpSoftShadows();                    // 0x0042d1c0
void FinishSoftShadowsATI();                // 0x0042d250
void FinishSoftShadows();                   // 0x0042d9a0

#endif // G_FB_EFFECTS_H
