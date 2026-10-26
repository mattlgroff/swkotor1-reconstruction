// RenderShutdown (0x00432180): releases the soft-shadow and framebuffer
// pbuffers, the effect textures, programs and ATI fragment shaders.
// Behavioural reference: src/units/0642/12dc18aa35-render_shutdown.cpp.
extern "C" {
__declspec(dllimport) void __stdcall glDeleteTextures(int n, const unsigned int* textures);
__declspec(dllimport) int __stdcall wglMakeCurrent(void* hdc, void* hglrc);
__declspec(dllimport) int __stdcall wglDeleteContext(void* hglrc);
__declspec(dllimport) int __stdcall ReleaseDC(void* hWnd, void* hDC);
}

typedef int(__stdcall* PFNWGLMAKECONTEXTCURRENTARB)(void* hDrawDC, void* hReadDC, void* hglrc);
typedef int(__stdcall* PFNWGLRELEASEPBUFFERDCARB)(void* hPbuffer, void* hDC);
typedef int(__stdcall* PFNWGLDESTROYPBUFFERARB)(void* hPbuffer);
typedef void(__stdcall* PFNGLDELETEPROGRAMSARB)(int n, const unsigned int* programs);
typedef void(__stdcall* PFNGLDELETEFRAGMENTSHADERATI)(unsigned int id);

extern PFNWGLMAKECONTEXTCURRENTARB wglMakeContextCurrentARB;      // 0x007bb818
extern PFNWGLRELEASEPBUFFERDCARB wglReleasePbufferDCARB;          // 0x007bb578
extern PFNWGLDESTROYPBUFFERARB wglDestroyPbufferARB;              // 0x007bb598
extern PFNGLDELETEPROGRAMSARB glDeleteProgramsARB;                // 0x007bb724
extern PFNGLDELETEFRAGMENTSHADERATI glDeleteFragmentShaderATI;    // 0x007bb550

class RequestPoolManager {
public:
    virtual ~RequestPoolManager();
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24();
    virtual void LoseBuffers();                                     // 0x28
};

extern RequestPoolManager* g_pRequestPoolManager;  // 0x007a68a4
extern void* g_hWnd;                        // 0x007a39d8
extern void* g_hGLDC;                       // 0x007a47e4
extern void* g_hGLContext;                  // 0x007a6854

extern void* g_hSoftShadowPbuffer1;         // 0x007a68e8
extern void* g_hSoftShadowDC1;              // 0x007a68ec
extern void* g_hSoftShadowContext1;         // 0x007a68f0
extern unsigned int g_nSoftShadowTexture1;  // 0x007a68f4
extern unsigned int g_nSoftShadowTexture;   // 0x007a68f8
extern void* g_hSoftShadowPbuffer2;         // 0x007a68fc
extern void* g_hSoftShadowDC2;              // 0x007a6900
extern void* g_hSoftShadowContext2;         // 0x007a6904
extern unsigned int g_nSoftShadowTexture2;  // 0x007a6908

extern unsigned int g_nRootTexture;         // 0x007a687c
extern unsigned int g_nColorRamp0;          // 0x007a6870
extern unsigned int g_nColorRamp1;          // 0x007a6874
extern unsigned int g_nColorRamp2;          // 0x007a6878
extern unsigned int g_nBloomRamp0;          // 0x007a6864
extern unsigned int g_nBloomRamp1;          // 0x007a6868
extern unsigned int g_nBloomRamp2;          // 0x007a686c
extern unsigned int g_nSaturationProgram;   // 0x007a68e4
extern unsigned int g_nDistortionProgram;   // 0x007a68d4

extern char g_bFrameBufferActive;           // 0x007a6860
extern void* g_ahBloomDC[6];                // 0x007a47ec
extern void* g_ahBloomContext[6];           // 0x007a4804
extern void* g_ahBloomPbuffer[6];           // 0x007a6824
extern unsigned int g_anBloomTexture[6];    // 0x007a47c4
extern unsigned int g_nBloomTexture;        // 0x007a68c0
extern void* g_hBloomPbuffer;               // 0x007a68c4
extern void* g_hBloomDC;                    // 0x007a68c8
extern void* g_hBloomContext;               // 0x007a68cc

extern unsigned int g_nColorRampAlphaShaderATI; // 0x007a690c
extern unsigned int g_nCopyShaderATI;           // 0x007a6910
extern unsigned int g_nConvolutionShaderATI;    // 0x007a6914
extern unsigned int g_nColorMatrixShaderATI;    // 0x007a6918
extern unsigned int g_nAccumBlurShaderATI;      // 0x007a691c
extern unsigned int g_nFilmNoiseShaderATI;      // 0x007a6920
extern unsigned int g_nDistortionShaderATI;     // 0x007a6924
extern unsigned int g_nColorRampShaderATI;      // 0x007a692c

int AurATISoftShadowsAvailable();               // 0x0045f7e0
int AurATIFrameBufferModificationsAvailable();  // 0x0045f860

inline void DeleteTexture(unsigned int& nTexture)
{
    if (nTexture) {
        glDeleteTextures(1, &nTexture);
        nTexture = 0;
    }
}

// FUNCTION: SWKOTOR 0x00432180
void RenderShutdown(int bRelease)
{
    if (g_pRequestPoolManager)
        g_pRequestPoolManager->LoseBuffers();
    if (g_hSoftShadowDC1) {
        if (AurATISoftShadowsAvailable()) {
            wglMakeContextCurrentARB(g_hSoftShadowDC1, g_hSoftShadowDC1, g_hSoftShadowContext1);
            glDeleteTextures(1, &g_nSoftShadowTexture1);
            wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
            wglReleasePbufferDCARB(g_hSoftShadowPbuffer1, g_hSoftShadowDC1);
            wglDestroyPbufferARB(g_hSoftShadowPbuffer1);
            g_hSoftShadowDC1 = 0;
            g_hSoftShadowContext1 = 0;
            g_hSoftShadowPbuffer1 = 0;
            g_nSoftShadowTexture1 = 0;
            wglMakeContextCurrentARB(g_hSoftShadowDC2, g_hSoftShadowDC2, g_hSoftShadowContext2);
            glDeleteTextures(1, &g_nSoftShadowTexture2);
            wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
            wglReleasePbufferDCARB(g_hSoftShadowPbuffer2, g_hSoftShadowDC2);
            wglDestroyPbufferARB(g_hSoftShadowPbuffer2);
            g_hSoftShadowDC2 = 0;
            g_hSoftShadowContext2 = 0;
            g_hSoftShadowPbuffer2 = 0;
            g_nSoftShadowTexture2 = 0;
            glDeleteTextures(1, &g_nSoftShadowTexture);
            g_nSoftShadowTexture = 0;
        } else {
            wglMakeCurrent(g_hSoftShadowDC1, g_hSoftShadowContext1);
            glDeleteTextures(1, &g_nSoftShadowTexture1);
            wglMakeCurrent(g_hGLDC, g_hGLContext);
            wglDeleteContext(g_hSoftShadowContext1);
            wglReleasePbufferDCARB(g_hSoftShadowPbuffer1, g_hSoftShadowDC1);
            wglDestroyPbufferARB(g_hSoftShadowPbuffer1);
            g_hSoftShadowDC1 = 0;
            g_hSoftShadowContext1 = 0;
            g_hSoftShadowPbuffer1 = 0;
            g_nSoftShadowTexture1 = 0;
            wglMakeCurrent(g_hSoftShadowDC2, g_hSoftShadowContext2);
            glDeleteTextures(1, &g_nSoftShadowTexture2);
            wglMakeCurrent(g_hGLDC, g_hGLContext);
            wglDeleteContext(g_hSoftShadowContext2);
            wglReleasePbufferDCARB(g_hSoftShadowPbuffer2, g_hSoftShadowDC2);
            wglDestroyPbufferARB(g_hSoftShadowPbuffer2);
            g_hSoftShadowDC2 = 0;
            g_hSoftShadowContext2 = 0;
            g_hSoftShadowPbuffer2 = 0;
            g_nSoftShadowTexture2 = 0;
            glDeleteTextures(1, &g_nSoftShadowTexture);
            g_nSoftShadowTexture = 0;
        }
    }
    DeleteTexture(g_nRootTexture);
    DeleteTexture(g_nColorRamp0);
    DeleteTexture(g_nColorRamp1);
    DeleteTexture(g_nColorRamp2);
    DeleteTexture(g_nBloomRamp0);
    DeleteTexture(g_nBloomRamp1);
    DeleteTexture(g_nBloomRamp2);
    if (g_nSaturationProgram) {
        glDeleteProgramsARB(1, &g_nSaturationProgram);
        g_nSaturationProgram = 0;
    }
    if (g_bFrameBufferActive == 1) {
        if (AurATIFrameBufferModificationsAvailable()) {
            for (int i = 0; i < 6; i++) {
                wglMakeContextCurrentARB(g_ahBloomDC[i], g_ahBloomDC[i], g_ahBloomContext[i]);
                glDeleteTextures(1, &g_anBloomTexture[i]);
                wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
                wglReleasePbufferDCARB(g_ahBloomPbuffer[i], g_ahBloomDC[i]);
                wglDestroyPbufferARB(g_ahBloomPbuffer[i]);
                g_ahBloomDC[i] = 0;
                g_ahBloomContext[i] = 0;
                g_ahBloomPbuffer[i] = 0;
                g_anBloomTexture[i] = 0;
            }
            wglMakeContextCurrentARB(g_hBloomDC, g_hBloomDC, g_hBloomContext);
            glDeleteTextures(1, &g_nBloomTexture);
            wglMakeContextCurrentARB(g_hGLDC, g_hGLDC, g_hGLContext);
            wglReleasePbufferDCARB(g_hBloomPbuffer, g_hBloomDC);
            wglDestroyPbufferARB(g_hBloomPbuffer);
            g_hBloomDC = 0;
            g_hBloomContext = 0;
            g_hBloomPbuffer = 0;
            g_nBloomTexture = 0;
        } else {
            for (int i = 0; i < 6; i++) {
                wglMakeCurrent(g_ahBloomDC[i], g_ahBloomContext[i]);
                glDeleteTextures(1, &g_anBloomTexture[i]);
                wglMakeCurrent(g_hGLDC, g_hGLContext);
                wglDeleteContext(g_ahBloomContext[i]);
                wglReleasePbufferDCARB(g_ahBloomPbuffer[i], g_ahBloomDC[i]);
                wglDestroyPbufferARB(g_ahBloomPbuffer[i]);
                g_ahBloomDC[i] = 0;
                g_ahBloomContext[i] = 0;
                g_ahBloomPbuffer[i] = 0;
                g_anBloomTexture[i] = 0;
            }
            if (g_hBloomPbuffer) {
                wglMakeCurrent(g_hBloomDC, g_hBloomContext);
                glDeleteTextures(1, &g_nBloomTexture);
                wglMakeCurrent(g_hGLDC, g_hGLContext);
                wglDeleteContext(g_hBloomContext);
                wglReleasePbufferDCARB(g_hBloomPbuffer, g_hBloomDC);
                wglDestroyPbufferARB(g_hBloomPbuffer);
                g_hBloomDC = 0;
                g_hBloomContext = 0;
                g_hBloomPbuffer = 0;
                g_nBloomTexture = 0;
            }
        }
        g_bFrameBufferActive = 0;
    }
    if (g_nDistortionProgram) {
        glDeleteProgramsARB(1, &g_nDistortionProgram);
        g_nDistortionProgram = 0;
    }
    if (g_nColorRampAlphaShaderATI) {
        glDeleteFragmentShaderATI(g_nColorRampAlphaShaderATI);
        g_nColorRampAlphaShaderATI = 0;
        glDeleteFragmentShaderATI(g_nCopyShaderATI);
        g_nCopyShaderATI = 0;
        glDeleteFragmentShaderATI(g_nConvolutionShaderATI);
        g_nConvolutionShaderATI = 0;
        glDeleteFragmentShaderATI(g_nColorMatrixShaderATI);
        g_nColorMatrixShaderATI = 0;
        glDeleteFragmentShaderATI(g_nAccumBlurShaderATI);
        g_nAccumBlurShaderATI = 0;
        glDeleteFragmentShaderATI(g_nFilmNoiseShaderATI);
        g_nFilmNoiseShaderATI = 0;
        glDeleteFragmentShaderATI(g_nDistortionShaderATI);
        g_nDistortionShaderATI = 0;
        glDeleteFragmentShaderATI(g_nColorRampShaderATI);
        g_nColorRampShaderATI = 0;
    }
    ReleaseDC(g_hWnd, g_hGLDC);
    if (bRelease) {
        if (g_pRequestPoolManager)
            delete g_pRequestPoolManager;
        g_pRequestPoolManager = 0;
    }
}
