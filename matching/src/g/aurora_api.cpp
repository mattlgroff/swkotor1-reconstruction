// Aurora API front end (global render switches, instance
// lifetime, display mode), 0x0044d8d0-0x0044f880.
#include "g/aurora.h"

struct _MEMORYSTATUS {
    unsigned long dwLength;
    unsigned long dwMemoryLoad;
    unsigned long dwTotalPhys;
    unsigned long dwAvailPhys;
    unsigned long dwTotalPageFile;
    unsigned long dwAvailPageFile;
    unsigned long dwTotalVirtual;
    unsigned long dwAvailVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(_MEMORYSTATUS* p);
extern "C" int __cdecl clock();
extern "C" __declspec(dllimport) void __stdcall glMaterialfv(unsigned int face, unsigned int pname, const float* params);
extern "C" __declspec(dllimport) void* __stdcall GetDC(void* hWnd);
extern "C" __declspec(dllimport) int __stdcall ReleaseDC(void* hWnd, void* hDC);
extern "C" int __cdecl sprintf(char* d, const char* fmt, ...);
extern "C" void* __cdecl memset(void* d, int c, unsigned int n);
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(memset, pow)
extern "C" __declspec(dllimport) int __stdcall SetDeviceGammaRamp(void* hDC, void* pRamp);

class Vector {
public:
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
    float x, y, z;
};

class Quaternion {
public:
    Vector GetXAxis() const;
    Vector GetYAxis() const;
    float w, x, y, z;
};

// --- globals ----------------------------------------------------------------
extern short g_bHeadLight;              // 0x0078e388
extern int g_bShadows;                  // 0x0078e3a8
extern int g_bGrass;                    // 0x0078e3ac
extern int g_bSoftShadows;              // 0x0078e420
extern int g_bFrameBufferEffects;       // 0x0078d98c
extern char g_bDisableVBO;              // 0x007b927d
extern char g_bVBOWriteOnly;            // 0x0078e398
extern int g_bATISoftShadowsUnlocked;   // 0x007bb860
extern char g_bSpeedBlur;               // 0x007a6880
extern char g_bSaturation;              // 0x007a6881
extern char g_bScanNoise;               // 0x007a6882
extern float g_fSpeedBlurRatio;         // 0x0078d43c
extern char g_bInterleavedAnimations;   // 0x007b92f4
extern char g_bVSync;                   // 0x0078d438
extern char g_szVersion[];              // 0x007b9228
extern float g_vFogColor[3];            // 0x0078db24
extern float g_fFogDistance;            // 0x007a6884
extern unsigned long g_nClearColor;     // 0x0078e394
extern float g_fMaxAnisotropy;          // 0x007a6858
extern int g_nAnisotropyRequested;      // 0x007a685c
extern int g_nAnisotropy;               // 0x007b9278
extern int g_nPrevAntiAliasingMode;     // 0x0078d440
extern int g_nAntiAliasingMode;         // 0x007a6888
extern int g_bUseARBVertexProgram;      // 0x0078e5ec
extern float g_fStartupDelay;           // 0x007b9270
extern float g_fAvailPhysKB;            // 0x007b9274
extern short g_nScreenWidthUnknown;     // 0x007b9220
extern CAurInternalGL* g_pAurora;       // 0x007b921c
extern int g_nAuroraRefs;               // 0x007b9218

typedef void (__stdcall *PFNSWAPINTERVAL)(int);
typedef void (__stdcall *PFNPROGRAMPARAM4F)(unsigned int, unsigned int,
                                            float, float, float, float);
extern PFNSWAPINTERVAL g_pfnSwapInterval;            // 0x007bb538
extern PFNPROGRAMPARAM4F g_pfnProgramEnvParameter4f; // 0x007bb850
extern PFNPROGRAMPARAM4F g_pfnProgramParameter4fNV;  // 0x007bb7ac

bool AurHasVSyncControl();               // 0x0045f580
int AurHasAnisotropy();                  // 0x0045f8e0
int AurTestAntiAliasingMode(int nMode);  // 0x0044f140
void AurTextureMakeCurrent();            // 0x00421ee0
void RemoveSquareFromRender(int nId);    // 0x00435b90
void FUN_00436490();
void AurFreeConsoleFuncs(const char* s); // 0x0044d250
void AurFreeConsoleVars();               // 0x0044d1e0
void AurFreeFont();                      // 0x0044cc60
void AurFreeScenes();                    // 0x004a22e0

// Vertex / fragment program objects released at shutdown.
class CAurProgram {
public:
    ~CAurProgram();                      // 0x004a2830
};

extern CAurProgram* g_pProgram83037c;    // 0x0083037c
extern CAurProgram* g_pProgram830380;    // 0x00830380
extern CAurProgram* g_pProgram830384;    // 0x00830384
extern CAurProgram* g_pProgram827fec;    // 0x00827fec
extern CAurProgram* g_pProgram827ff0;    // 0x00827ff0
extern CAurProgram* g_pProgram827ff4;    // 0x00827ff4
extern CAurProgram* g_pProgram827ff8;    // 0x00827ff8
extern CAurProgram* g_pProgram827ffc;    // 0x00827ffc
extern CAurProgram* g_pProgram830388;    // 0x00830388
extern CAurProgram* g_pProgram83038c;    // 0x0083038c
extern CAurProgram* g_pProgram828014;    // 0x00828014
extern char* g_szProgramText83028c;      // 0x0083028c
extern char* g_szProgramText830290;      // 0x00830290
extern void* g_hWnd;                     // 0x007a39d8

void ResetVertexPrograms_004a2470();     // 0x004a2470
void AurReleaseNoOp();                   // 0x004015a0 (empty)
void RenderShutdown(int bRelease);       // 0x00432180

extern "C" __declspec(dllimport) int __stdcall wglDeleteContext(void* hRC);
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void* hWnd);

inline void ReleaseProgram(CAurProgram*& p)
{
    if (p) {
        delete p;
        p = 0;
    }
}

inline void ReleaseText(char*& p)
{
    if (p) {
        delete p;
        p = 0;
    }
}

// FUNCTION: SWKOTOR 0x0044d6d0
int CAurInternalGL::GetDisplayStyle()
{
    return m_nUnknownAC;
}

inline void ClearDevMode(_devicemodeA* p)
{
    memset(p, 0, sizeof(*p));
    p->dmSize = sizeof(*p);
}

// FUNCTION: SWKOTOR 0x0044d6e0
bool CAurInternalGL::GetValidMode(unsigned long* pWidth, unsigned long* pHeight,
                                           unsigned char* pBitsPerPel,
                                           unsigned long* pFrequency,
                                           unsigned char bApply)
{
    _devicemodeA dmBest;
    _devicemodeA dm;
    ClearDevMode(&dm);
    ClearDevMode(&dmBest);
    unsigned long nMode = 0;
    unsigned char bFound = 0;
    while (EnumDisplaySettingsA(0, nMode, &dm)) {
        if (dm.dmPelsWidth <= *pWidth && dm.dmPelsHeight <= *pHeight
            && dm.dmBitsPerPel <= *pBitsPerPel && dm.dmDisplayFrequency <= *pFrequency
            && dm.dmPelsWidth >= dmBest.dmPelsWidth && dm.dmPelsHeight >= dmBest.dmPelsHeight
            && dm.dmBitsPerPel >= dmBest.dmBitsPerPel
            && (dm.dmDisplayFrequency >= dmBest.dmDisplayFrequency
                || dm.dmPelsWidth != dmBest.dmPelsWidth
                || dm.dmPelsHeight != dmBest.dmPelsHeight
                || dm.dmBitsPerPel != dmBest.dmBitsPerPel)) {
            dmBest = dm;
            bFound = 1;
        }
        ClearDevMode(&dm);
        nMode++;
    }
    if (!bFound)
        return false;
    if (ChangeDisplaySettingsA(&dmBest, CDS_TEST))
        return false;
    if (bApply) {
        *pWidth = dmBest.dmPelsWidth;
        *pHeight = dmBest.dmPelsHeight;
        *pBitsPerPel = (unsigned char)dmBest.dmBitsPerPel;
        *pFrequency = dmBest.dmDisplayFrequency;
        return true;
    }
    return dmBest.dmPelsWidth == *pWidth && dmBest.dmPelsHeight == *pHeight
           && dmBest.dmBitsPerPel == *pBitsPerPel
           && dmBest.dmDisplayFrequency == *pFrequency;
}

// FUNCTION: SWKOTOR 0x0044d8d0
void CAurInternal::RestoreDesktopDisplayMode()
{
    SetGamma(1.0f, 1);
    if (m_bFullScreen)
        ChangeDisplaySettingsA(0, 0);
}

// FUNCTION: SWKOTOR 0x0044d900
void CAurInternal::RestoreAuroraDisplayMode()
{
    SetGamma(m_fGamma, 1);
    if (m_bFullScreen) {
        if (ChangeDisplaySettingsA(&m_devMode, CDS_TEST) == 0)
            ChangeDisplaySettingsA(&m_devMode, CDS_FULLSCREEN);
    }
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0044d940
void AurReleaseInstance()
{
    if (g_pAurora) {
        if (--g_nAuroraRefs == 0) {
            delete g_pAurora;
            g_pAurora = 0;
        }
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0044d970
void* CAurInternalGL::GetContext()
{
    return (void*)m_nUnknownB4;
}

// FUNCTION: SWKOTOR 0x0044d980
int CAurInternalGL::MakeCurrent(void* hDC, void* hRC)
{
    return wglMakeCurrent(hDC, hRC);
}

// FUNCTION: SWKOTOR 0x0044d990
int CAurInternalGL::SetGamma(float fGamma, int bTemporary)
{
    if (!bTemporary)
        m_fGamma = fGamma;
    unsigned short aRamp[3][256];
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < 256; i++)
            aRamp[c][i] = ((unsigned char)(pow(i * (1.0 / 255.0), fGamma) * 255.0) << 8) | i;
    }
    void* hDC = GetDC(g_hWnd);
    if (!hDC)
        return 0;
    SetDeviceGammaRamp(hDC, aRamp);
    return 1;
}

// FUNCTION: SWKOTOR 0x0044da40 SYMBOL: ??_GCAurora@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0044da60
void AurEnableHeadLight()
{
    if (g_bHeadLight != 1)
        glEnable(GL_LIGHT0);
    g_bHeadLight = 1;
}

// FUNCTION: SWKOTOR 0x0044da80
void AurDisableHeadLight()
{
    if (g_bHeadLight != 0)
        glDisable(GL_LIGHT0);
    g_bHeadLight = 0;
}

// FUNCTION: SWKOTOR 0x0044daa0
CAurora::~CAurora()
{
}

// FUNCTION: SWKOTOR 0x0044ec60
CAurInternalGL::~CAurInternalGL()
{
    AurFreeConsoleFuncs(0);
    AurFreeConsoleVars();
    AurFreeFont();
    AurFreeScenes();
    Shutdown(1, 1);
}

extern int g_nScreenWidth;                  // 0x0078d1d4
extern int g_nScreenHeight;                 // 0x0078d1d8

// FUNCTION: SWKOTOR 0x0044e9e0
void CAurInternalGL::Reshape(int nWidth, int nHeight)
{
    if (!m_nUnknownB4)
        return;
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    g_nScreenWidth = nWidth;
    g_nScreenHeight = nHeight;
    if (!nHeight)
        nHeight = 1;
    if (!nWidth)
        nWidth = 1;
    glViewport(0, 0, nWidth, nHeight);
}

// FUNCTION: SWKOTOR 0x0044ea30
void CAurInternalGL::Shutdown(int bRelease, int bRestore)
{
    ResetVertexPrograms_004a2470();
    ReleaseProgram(g_pProgram83037c);
    ReleaseProgram(g_pProgram830380);
    ReleaseProgram(g_pProgram830384);
    ReleaseProgram(g_pProgram827fec);
    ReleaseProgram(g_pProgram827ff0);
    ReleaseProgram(g_pProgram827ff4);
    ReleaseProgram(g_pProgram827ff8);
    ReleaseProgram(g_pProgram827ffc);
    ReleaseProgram(g_pProgram830388);
    ReleaseProgram(g_pProgram83038c);
    ReleaseProgram(g_pProgram828014);
    if (bRelease)
        AurReleaseNoOp();
    RenderShutdown(bRelease);
    ReleaseText(g_szProgramText83028c);
    ReleaseText(g_szProgramText830290);
    if (m_nUnknownB4) {
        wglMakeCurrent(0, 0);
        wglDeleteContext((void*)m_nUnknownB4);
        m_nUnknownB4 = 0;
    }
    if (m_bFullScreen) {
        if (bRestore)
            ChangeDisplaySettingsA(0, 0);
        m_bFullScreen = 0;
        m_nUnknownAC = 0;
    }
    if (bRelease)
        SetGamma(1.0f, 0);
    if (g_hWnd && bRestore) {
        DestroyWindow(g_hWnd);
        g_hWnd = 0;
    }
}

inline CAurInternal::CAurInternal()
{
    m_bFullScreen = 0;
    m_nUnknownAC = 0;
    m_fGamma = 1.0f;
    g_nScreenWidthUnknown = 0;
    m_nUnknownB4 = 0;
}

inline CAurInternalGL::CAurInternalGL()
{
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x0044ecd0
CAurora* AurCreateInstance()
{
    if (g_pAurora == 0)
        g_pAurora = new CAurInternalGL;
    g_nAuroraRefs++;
    return g_pAurora;
}

CAurora* AurCreateInstanceThunk()
{
    return AurCreateInstance();
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0044ed40 SYMBOL: ??_GCAurInternalGL@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0044ed70
CAurora* NewCAurora()
{
    return AurCreateInstanceThunk();
}

// FUNCTION: SWKOTOR 0x0044ed80
void CAurInternal::DestroyCAuroraInstance()
{
    AurReleaseInstance();
}

// FUNCTION: SWKOTOR 0x0044ed90
void AurStartupWait()
{
    if (g_fStartupDelay != 0.0f) {
        int nDelay = (int)(g_fStartupDelay * 1000.0f);
        int nEnd = clock() + nDelay;
        while (nEnd > clock())
            ;
    }
    _MEMORYSTATUS ms;
    GlobalMemoryStatus(&ms);
    g_fAvailPhysKB = (float)ms.dwAvailPhys * (1.0f / 1024.0f);
    AurTextureMakeCurrent();
}

// FUNCTION: SWKOTOR 0x0044ee20
void AurEnableShadows()
{
    g_bShadows = 1;
}

// FUNCTION: SWKOTOR 0x0044ee30
void AurEnableGrass()
{
    g_bGrass = 1;
}

// FUNCTION: SWKOTOR 0x0044ee40
void AurDisableGrass()
{
    g_bGrass = 0;
}

// FUNCTION: SWKOTOR 0x0044ee50
void AurEnableSoftShadows()
{
    g_bSoftShadows = 1;
    g_bShadows = 1;
}

// FUNCTION: SWKOTOR 0x0044ee60
void AurDisableSoftShadows()
{
    g_bSoftShadows = 0;
}

// FUNCTION: SWKOTOR 0x0044ee70
void AurEnableFrameBufferEffects()
{
    g_bFrameBufferEffects = 1;
}

// FUNCTION: SWKOTOR 0x0044ee80
void AurDisableFrameBufferEffects()
{
    g_bFrameBufferEffects = 0;
}

// FUNCTION: SWKOTOR 0x0044ee90
void AurSetVSyncState(char bEnabled)
{
    if (AurHasVSyncControl()) {
        g_pfnSwapInterval(bEnabled != 0);
        g_bVSync = bEnabled;
    }
}

// FUNCTION: SWKOTOR 0x0044eec0
char* AurGetVersionString()
{
    sprintf(g_szVersion, "SWPC 0027");
    return g_szVersion;
}

// FUNCTION: SWKOTOR 0x0044eee0
void AurGetGLError()
{
    unsigned int nError = glGetError();
    if (nError)
        gluErrorString(nError);
}

// FUNCTION: SWKOTOR 0x0044ef00
void AurSetClearColor(float r, float g, float b, float a)
{
    g_nClearColor = ((((int)(a * 255.0f) << 8 | (int)(b * 255.0f)) << 8
                      | (int)(g * 255.0f)) << 8) | (int)(r * 255.0f);
}

// FUNCTION: SWKOTOR 0x0044ef60
void AurDisableVertexBufferObjects()
{
    g_bDisableVBO = 1;
}

// FUNCTION: SWKOTOR 0x0044ef70
void AurDisableVertexBufferObjectsWriteOnly()
{
    g_bVBOWriteOnly = 0;
}

// FUNCTION: SWKOTOR 0x0044ef80
void AurEnableVertexBufferObjectsWriteOnly()
{
    g_bVBOWriteOnly = 1;
}

// FUNCTION: SWKOTOR 0x0044ef90
void AurATIUnlockSoftShadowsForHotFix()
{
    g_bATISoftShadowsUnlocked = 1;
    g_bSoftShadows = 0;
}

// FUNCTION: SWKOTOR 0x0044f050
void AurRemoveDrawableSquare(int nId)
{
    if (nId != -1)
        RemoveSquareFromRender(nId);
}

// FUNCTION: SWKOTOR 0x0044f070
void thunk_FUN_00436490()
{
    FUN_00436490();
}

// FUNCTION: SWKOTOR 0x0044f080
void AurDisableShadows()
{
    g_bShadows = 0;
    g_bSoftShadows = 0;
}

// FUNCTION: SWKOTOR 0x0044f090
int AurGetMaxAnisotropy()
{
    return (int)g_fMaxAnisotropy;
}

// FUNCTION: SWKOTOR 0x0044f0a0
void AurEnableSpeedBlur()
{
    g_bSpeedBlur = 1;
}

// FUNCTION: SWKOTOR 0x0044f0b0
void AurDisableSpeedBlur()
{
    g_bSpeedBlur = 0;
}

// FUNCTION: SWKOTOR 0x0044f0c0
void AurEnableSaturation()
{
    g_bSaturation = 1;
}

// FUNCTION: SWKOTOR 0x0044f0d0
void AurDisableSaturation()
{
    g_bSaturation = 0;
}

// FUNCTION: SWKOTOR 0x0044f0e0
void AurEnableScanNoise()
{
    g_bScanNoise = 1;
}

// FUNCTION: SWKOTOR 0x0044f0f0
void AurDisableScanNoise()
{
    g_bScanNoise = 0;
}

// FUNCTION: SWKOTOR 0x0044f100
void AurSetFog(float r, float g, float b, float fDistance)
{
    g_vFogColor[0] = r;
    g_vFogColor[1] = g;
    g_vFogColor[2] = b;
    g_fFogDistance = fDistance;
}

// FUNCTION: SWKOTOR 0x0044f130
void AurSetSpeedBlurRatio(float fRatio)
{
    g_fSpeedBlurRatio = fRatio;
}

// FUNCTION: SWKOTOR 0x0044f2c0
int AurSetAnisotropy(int nLevel)
{
    if (AurHasAnisotropy() && (float)nLevel <= g_fMaxAnisotropy) {
        g_nAnisotropyRequested = nLevel;
        g_nAnisotropy = nLevel;
        return nLevel;
    }
    return g_nAnisotropy;
}

// FUNCTION: SWKOTOR 0x0044f2f0
int AurSetAntiAliasingMode(int nMode)
{
    if (!AurTestAntiAliasingMode(nMode))
        return 0;
    g_nPrevAntiAliasingMode = g_nAntiAliasingMode;
    g_nAntiAliasingMode = nMode;
    return 1;
}

// FUNCTION: SWKOTOR 0x0044f3d0
Vector Quaternion::GetXAxis() const
{
    return Vector(1.0f - (y * y + z * z) * 2.0f,
                  (y * x + w * z) * 2.0f,
                  (x * z - y * w) * 2.0f);
}

// FUNCTION: SWKOTOR 0x0044f420
Vector Quaternion::GetYAxis() const
{
    return Vector((x * y - w * z) * 2.0f,
                  1.0f - (x * x + z * z) * 2.0f,
                  (y * z + x * w) * 2.0f);
}

// FUNCTION: SWKOTOR 0x0044f760
void AurEnableInterleavedAnimations()
{
    g_bInterleavedAnimations = 1;
}

// FUNCTION: SWKOTOR 0x0044f770
void AurDisableInterleavedAnimations()
{
    g_bInterleavedAnimations = 0;
}

// FUNCTION: SWKOTOR 0x0044f820
void AurSetVertexProgramParameter(unsigned int nIndex, float x, float y,
                                  float z, float w)
{
    if (g_bUseARBVertexProgram)
        g_pfnProgramEnvParameter4f(0x8620, nIndex, x, y, z, w);
    else
        g_pfnProgramParameter4fNV(0x8620, nIndex, x, y, z, w);
}

extern unsigned int g_aMaterialFaces[];  // 0x0073f2ec
extern unsigned int g_aMaterialParams[]; // 0x0073f2f8

// FUNCTION: SWKOTOR 0x0044f880
void AurMaterialfv(int nFace, int nParam, const float* pValues)
{
    glMaterialfv(g_aMaterialFaces[nFace], g_aMaterialParams[nParam], pValues);
}

typedef int (__stdcall *PFNCHOOSEPIXELFORMATARB)(void* hdc, const int* piAttribs,
                                                 const float* pfAttribs,
                                                 unsigned int nMaxFormats,
                                                 int* piFormats,
                                                 unsigned int* pnNumFormats);
typedef int (__stdcall *PFNGETPIXELFORMATATTRIBIVARB)(void* hdc, int iPixelFormat,
                                                      int iLayerPlane,
                                                      unsigned int nAttributes,
                                                      const int* piAttributes,
                                                      int* piValues);
extern PFNCHOOSEPIXELFORMATARB g_pfnChoosePixelFormatARB;           // 0x007bb730
extern PFNGETPIXELFORMATATTRIBIVARB g_pfnGetPixelFormatAttribivARB; // 0x007bb63c
extern void* g_hWnd;                                                // 0x007a39d8
extern short g_nColorBits;                                          // 0x007b9220
extern short g_nDepthBits;                                          // 0x007b9224
int AurHasMultisample();                                            // 0x0046ed70

// FUNCTION: SWKOTOR 0x0044f140
int AurTestAntiAliasingMode(int nSamples)
{
    if (nSamples >= 0) {
        if (nSamples == 0)
            return 1;
        if (AurHasMultisample()) {
            int iAttribs[30];
            float fAttribs[2];
            unsigned int nFormats;
            int nAttrib;
            int nValue;
            int nFormat;
            iAttribs[0] = 0x2001;
            iAttribs[1] = 1;
            iAttribs[2] = 0x2003;
            iAttribs[3] = 0x2027;
            iAttribs[4] = 0x2014;
            void* hDC = GetDC(g_hWnd);
            iAttribs[5] = g_nColorBits;
            iAttribs[6] = 0x201b;
            iAttribs[7] = 8;
            iAttribs[8] = 0x2022;
            iAttribs[9] = g_nDepthBits;
            iAttribs[10] = 0x2023;
            iAttribs[11] = 8;
            iAttribs[12] = 0x2011;
            iAttribs[13] = 1;
            iAttribs[14] = 0x2041;
            iAttribs[15] = 1;
            iAttribs[16] = 0x2042;
            iAttribs[17] = nSamples;
            iAttribs[18] = 0;
            iAttribs[19] = 0;
            fAttribs[0] = 0.0f;
            fAttribs[1] = 0.0f;
            if (g_pfnChoosePixelFormatARB(hDC, iAttribs, fAttribs, 1, &nFormat,
                                          &nFormats)
                && nFormats > 0) {
                nAttrib = 0x2042;
                g_pfnGetPixelFormatAttribivARB(hDC, nFormat, 0, 1, &nAttrib,
                                               &nValue);
                ReleaseDC(g_hWnd, hDC);
                return nValue == nSamples;
            }
            ReleaseDC(g_hWnd, hDC);
        }
    }
    return 0;
}

struct AurSquareVertex {
    Vector v;
    unsigned long nColor;
};

int AurCreateVertexBuffer(int a, int nSize, int c, int nFormat, int* pHandle); // 0x00423ee0
AurSquareVertex* AurLockVertexBuffer(int h, int a, int b, int c);             // 0x00423f10
void AurUnlockVertexBuffer(int h);                                            // 0x00423f30
void AddSquareToRender(int h);                                                // 0x00435b40

// FUNCTION: SWKOTOR 0x0044efb0
int AurAddDrawableSquare(const Vector* pCorners, unsigned long nColor)
{
    int hBuffer;
    hBuffer = AurCreateVertexBuffer(0, 0x50, 0, 0x11, &hBuffer);
    AurSquareVertex* pVerts = AurLockVertexBuffer(hBuffer, 0, 0, 1);
    for (int i = 0; i < 4; i++) {
        pVerts[i].v.x = pCorners[i].x;
        pVerts[i].v.y = pCorners[i].y;
        pVerts[i].v.z = pCorners[i].z;
        pVerts[i].nColor = nColor;
    }
    pVerts[4].v.x = pCorners[0].x;
    pVerts[4].v.y = pCorners[0].y;
    pVerts[4].v.z = pCorners[0].z;
    pVerts[4].nColor = nColor;
    AurUnlockVertexBuffer(hBuffer);
    AddSquareToRender(hBuffer);
    return hBuffer;
}
