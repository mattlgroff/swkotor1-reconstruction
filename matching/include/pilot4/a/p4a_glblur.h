// Pilot 4 arm A: GL accumulation blur (0x00433DA0) declarations.
#ifndef P4A_GLBLUR_H
#define P4A_GLBLUR_H

extern "C" {
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int texture);
__declspec(dllimport) void __stdcall glTexParameteri(unsigned int target, unsigned int pname, int param);
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
__declspec(dllimport) void __stdcall glVertex3f(float x, float y, float z);
}

#define GL_BLEND 0x0be2
#define GL_DEPTH_TEST 0x0b71
#define GL_LIGHTING 0x0b50
#define GL_QUADS 7
#define GL_TEXTURE_2D 0x0de1
#define GL_TEXTURE_RECTANGLE_NV 0x84f5
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_CLAMP 0x2900
#define GL_TEXTURE0_ARB 0x84c0
#define GL_TEXTURE1_ARB 0x84c1
#define GL_COMBINER0_NV 0x8550
#define GL_COMBINER1_NV 0x8551
#define GL_CONSTANT_COLOR0_NV 0x852a
#define GL_CONSTANT_COLOR1_NV 0x852b
#define WGL_FRONT_LEFT_ARB 0x2083

typedef void(__stdcall* PFNGLACTIVETEXTUREARB)(unsigned int tex);
typedef void(__stdcall* PFNGLMULTITEXCOORD2FARB)(unsigned int tex, float s, float t);
typedef void(__stdcall* PFNGLCOMBINERSTAGEPARAMETERFVNV)(unsigned int stage, unsigned int pname,
                                                        const float* params);
typedef int(__stdcall* PFNWGLBINDTEXIMAGEARB)(void* hPbuffer, int iBuffer);

extern PFNGLACTIVETEXTUREARB glActiveTextureARB;                    // 0x007BB80C
extern PFNGLMULTITEXCOORD2FARB glMultiTexCoord2fARB;                // 0x007BB6E8
extern PFNGLCOMBINERSTAGEPARAMETERFVNV glCombinerStageParameterfvNV; // 0x007BB6F8
extern PFNWGLBINDTEXIMAGEARB wglBindTexImageARB;                    // 0x007BB7B8
extern PFNWGLBINDTEXIMAGEARB wglReleaseTexImageARB;                 // 0x007BB6D0

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

float* AurMakeColor(const Vector& vColor, const float& fAlpha);     // 0x004AAAB0
void FrameBufferModificationPixelShaders(int nMode, int a, int b, int c, int d); // 0x004299C0

extern float g_fSpeedBlurRatio;                         // 0x0078D43C
extern unsigned int g_nBloomTexture;                    // 0x007A68C0
extern void* g_hBloomPbuffer;                           // 0x007A68C4
extern unsigned int g_nRootTexture;                     // 0x007A687C
extern int g_nWindowWidth;                              // 0x0078D1D4
extern int g_nWindowHeight;                             // 0x0078D1D8

void RenderAccumulationBlur(unsigned int nTexture);     // 0x00433DA0

#endif
