// InitializeAntiAliasing: probe a throw-away GL window for
// WGL_ARB_multisample and record it in the capability mask.
// Behavioural reference: src/units/0530/7bdebf0c35-aa_probe.cpp.
typedef void* HWND;
typedef void* HDC;
typedef void* HGLRC;
typedef void* HINSTANCE;
typedef void* HGDIOBJ;
typedef long(__stdcall* WNDPROC)(HWND, unsigned int, unsigned int, long);

struct WNDCLASSA {
    unsigned int style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    void* hIcon;
    void* hCursor;
    HGDIOBJ hbrBackground;
    const char* lpszMenuName;
    const char* lpszClassName;
};

struct PIXELFORMATDESCRIPTOR {
    unsigned short nSize;
    unsigned short nVersion;
    unsigned long dwFlags;
    unsigned char iPixelType;
    unsigned char cColorBits;
    unsigned char cRedBits, cRedShift, cGreenBits, cGreenShift, cBlueBits, cBlueShift;
    unsigned char cAlphaBits, cAlphaShift;
    unsigned char cAccumBits, cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits;
    unsigned char cDepthBits;
    unsigned char cStencilBits;
    unsigned char cAuxBuffers;
    unsigned char iLayerType;
    unsigned char bReserved;
    unsigned long dwLayerMask;
    unsigned long dwVisibleMask;
    unsigned long dwDamageMask;
};

struct OSVERSIONINFOA {
    unsigned long dwOSVersionInfoSize;
    unsigned long dwMajorVersion;
    unsigned long dwMinorVersion;
    unsigned long dwBuildNumber;
    unsigned long dwPlatformId;
    char szCSDVersion[128];
};

extern "C" {
__declspec(dllimport) long __stdcall DefWindowProcA(HWND, unsigned int, unsigned int, long);
__declspec(dllimport) HGDIOBJ __stdcall GetStockObject(int i);
__declspec(dllimport) unsigned short __stdcall RegisterClassA(const WNDCLASSA* p);
__declspec(dllimport) HWND __stdcall CreateWindowExA(unsigned long dwExStyle, const char* szClass,
                                                     const char* szName, unsigned long dwStyle,
                                                     int x, int y, int w, int h, HWND hParent,
                                                     void* hMenu, HINSTANCE hInstance, void* p);
__declspec(dllimport) HDC __stdcall GetDC(HWND hWnd);
__declspec(dllimport) int __stdcall ReleaseDC(HWND hWnd, HDC hDC);
__declspec(dllimport) int __stdcall DestroyWindow(HWND hWnd);
__declspec(dllimport) int __stdcall ChoosePixelFormat(HDC hDC, const PIXELFORMATDESCRIPTOR* p);
__declspec(dllimport) int __stdcall SetPixelFormat(HDC hDC, int n, const PIXELFORMATDESCRIPTOR* p);
__declspec(dllimport) int __stdcall DescribePixelFormat(HDC hDC, int n, unsigned int nBytes,
                                                        PIXELFORMATDESCRIPTOR* p);
__declspec(dllimport) HGLRC __stdcall wglCreateContext(HDC hDC);
__declspec(dllimport) int __stdcall wglMakeCurrent(HDC hDC, HGLRC hRC);
__declspec(dllimport) int __stdcall wglDeleteContext(HGLRC hRC);
__declspec(dllimport) void* __stdcall wglGetProcAddress(const char* szName);
__declspec(dllimport) const unsigned char* __stdcall glGetString(unsigned int name);
__declspec(dllimport) int __stdcall GetVersionExA(OSVERSIONINFOA* p);
void* __cdecl memset(void* p, int c, unsigned int n);
}
#pragma intrinsic(memset)

typedef const char*(__stdcall* PFNWGLGETEXTENSIONSSTRINGARB)(HDC hDC);
typedef int(__stdcall* PFNWGLCHOOSEPIXELFORMATARB)(HDC hdc, const int* piAttribs,
                                                   const float* pfAttribs, unsigned int nMax,
                                                   int* piFormats, unsigned int* pnFormats);

extern HINSTANCE g_hInstance;                                   // 0x007a39c0
extern char g_szWindowClass[];                                  // 0x0078e310 "Test GL Window"
extern PFNWGLGETEXTENSIONSSTRINGARB wglGetExtensionsStringARB;  // 0x007bb7ec
extern PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB;      // 0x007bb730
extern const char* g_szWGLExtensions;                           // 0x007a688c
extern unsigned int g_nCapabilities;                            // 0x007bb85c
extern unsigned int g_nMultisampleCapability;                   // 0x0078e4ec

bool __cdecl CheckExtension(const char* szName, const char* szList);   // 0x0045f980

// FUNCTION: SWKOTOR 0x0042e040
void InitializeAntiAliasing()
{
    WNDCLASSA wc;
    wc.style = 0xb;
    wc.lpfnWndProc = DefWindowProcA;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = GetStockObject(4);
    wc.lpszMenuName = 0;
    wc.lpszClassName = g_szWindowClass;
    RegisterClassA(&wc);
    HWND hWnd = CreateWindowExA(0, g_szWindowClass, "Exo Base Window", 0x06000000, 0, 0,
                                0x80000000, 0x80000000, 0, 0, 0, 0);
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = 0x25;
    pfd.iPixelType = 0;
    pfd.cDepthBits = 24;
    pfd.cColorBits = 32;
    pfd.iLayerType = 0;
    pfd.cStencilBits = 8;
    HDC hDC = GetDC(hWnd);
    int nFormat = ChoosePixelFormat(hDC, &pfd);
    if (!SetPixelFormat(hDC, nFormat, 0)) {
        ReleaseDC(hWnd, hDC);
        return;
    }
    DescribePixelFormat(hDC, nFormat, sizeof(pfd), &pfd);
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    wglGetExtensionsStringARB =
        (PFNWGLGETEXTENSIONSSTRINGARB)wglGetProcAddress("wglGetExtensionsStringARB");
    wglChoosePixelFormatARB =
        (PFNWGLCHOOSEPIXELFORMATARB)wglGetProcAddress("wglChoosePixelFormatARB");
    const char* szGL = (const char*)glGetString(0x1f03);
    int bATI = 0;
    if (CheckExtension("GL_ATI_fragment_shader", szGL))
        bATI = 1;
    if (wglGetExtensionsStringARB) {
        const char* szExtensions = wglGetExtensionsStringARB(hDC);
        g_szWGLExtensions = szExtensions;
        if (szExtensions && CheckExtension("WGL_ARB_multisample", szExtensions)) {
            if (bATI == 1) {
                OSVERSIONINFOA vi;
                memset(&vi, 0, sizeof(vi));
                vi.dwOSVersionInfoSize = sizeof(vi);
                if (GetVersionExA(&vi) && vi.dwMajorVersion > 4)
                    g_nCapabilities |= g_nMultisampleCapability;
            } else {
                g_nCapabilities |= g_nMultisampleCapability;
            }
        }
    }
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);
}
