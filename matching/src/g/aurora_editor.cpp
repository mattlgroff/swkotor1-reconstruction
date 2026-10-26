// CAurInternalGL::InitEditor (0x0044e3b0, vtable +0x10): attaches the GL
// context to an editor window (optionally switching the display mode) and
// builds the vertex programs once.
#include "g/aurora.h"

extern "C" {
__declspec(dllimport) int __stdcall ShowWindow(void* hWnd, int nCmdShow);
__declspec(dllimport) void* __stdcall GetDC(void* hWnd);
__declspec(dllimport) int __stdcall ReleaseDC(void* hWnd, void* hDC);
__declspec(dllimport) int __stdcall ChoosePixelFormat(void* hDC, const void* pPfd);
__declspec(dllimport) int __stdcall SetPixelFormat(void* hDC, int nFormat, const void* pPfd);
__declspec(dllimport) int __stdcall DescribePixelFormat(void* hDC, int nFormat, unsigned int nBytes,
                                                        void* pPfd);
__declspec(dllimport) void* __stdcall wglCreateContext(void* hDC);
__declspec(dllimport) int __stdcall wglDeleteContext(void* hRC);
__declspec(dllimport) int __stdcall AdjustWindowRect(void* pRect, unsigned long dwStyle, int bMenu);
__declspec(dllimport) long __stdcall SetWindowLongA(void* hWnd, int nIndex, long nValue);
__declspec(dllimport) int __stdcall SetWindowPos(void* hWnd, void* hAfter, int x, int y, int cx,
                                                 int cy, unsigned int nFlags);
__declspec(dllimport) long __stdcall SendMessageA(void* hWnd, unsigned int nMsg, unsigned int wParam,
                                                  long lParam);
void* __cdecl memset(void* d, int c, unsigned int n);
}
#pragma intrinsic(memset)

#define WS_POPUP 0x80000000
#define WS_CHILD 0x40000000
#define GWL_STYLE (-16)
#define WM_SIZE 5
#define SWP_NOZORDER_SHOWWINDOW 0x120

struct tagRECT {
    long left, top, right, bottom;
};

// PIXELFORMATDESCRIPTOR, 0x28 bytes.
struct tagPIXELFORMATDESCRIPTOR {
    unsigned short nSize;               // +0x00
    unsigned short nVersion;            // +0x02
    unsigned long dwFlags;              // +0x04
    unsigned char iPixelType;           // +0x08
    unsigned char cColorBits;           // +0x09
    unsigned char pad0a[0x17 - 0x0a];
    unsigned char cDepthBits;           // +0x17
    unsigned char cStencilBits;         // +0x18
    unsigned char cAuxBuffers;          // +0x19
    unsigned char iLayerType;           // +0x1a
    unsigned char pad1b[0x28 - 0x1b];
};

class VertexProgram {
public:
    VertexProgram(char* szSource, int nParameters);     // 0x004a27d0
    void Bind(void* p, int n);                          // 0x004a24d0
};

extern char g_bEditorProgramsBuilt;     // 0x007b9226
extern int g_bVertexPrograms;           // 0x0078e5ec
extern unsigned int g_eVertexProgramTarget; // 0x0078d46c
extern char* g_szProgramText0;          // 0x0083028c
extern char* g_szProgramText1;          // 0x00830290
extern char g_szVertexProgramA[];       // 0x007990f0
extern char g_szVertexProgramB[];       // 0x00792658
extern char g_szVertexProgramBNV[];     // 0x0079b5a8
extern char g_szVertexProgramC[];       // 0x0079c310
extern char g_szVertexProgramD[];       // 0x0078feb8
extern char g_szVertexProgramDNV[];     // 0x0079ab28
extern char g_szVertexProgramENV[];     // 0x00798cc8
extern int g_nProgramParams544;         // 0x00830544
extern int g_nProgramParams548;         // 0x00830548
extern int g_nProgramParams54c;         // 0x0083054c
extern int g_nProgramParams560;         // 0x00830560
extern int g_nProgramParams564;         // 0x00830564
extern int g_nProgramParams580;         // 0x00830580
extern VertexProgram* g_pProgram37c;    // 0x0083037c
extern VertexProgram* g_pProgram380;    // 0x00830380
extern VertexProgram* g_pProgram384;    // 0x00830384
extern VertexProgram* g_pProgramfec;    // 0x00827fec
extern VertexProgram* g_pProgramff0;    // 0x00827ff0
extern VertexProgram* g_pProgramff4;    // 0x00827ff4
extern VertexProgram* g_pProgramff8;    // 0x00827ff8
extern VertexProgram* g_pProgramffc;    // 0x00827ffc

void LoadExtensions();                  // 0x00436490
int AurVertexProgramsAvailable();       // 0x0045f770

inline void ClearDevMode(_devicemodeA* p)
{
    memset(p, 0, sizeof(*p));
    p->dmSize = sizeof(*p);
}

// FUNCTION: SWKOTOR 0x0044e3b0
// NONMATCH (90): every instruction agrees; only the return paths are laid
// out differently. The original gives the `ShowWindow(hWnd, 1); return 0`
// path its own epilogue and sends the GetDC failure (eax already zeroed
// for the pixel-format memset) to the final epilogue; cl merges both into
// the shared return-0 block. Tried wrapping the tail in `if (hDC)`.
int CAurInternalGL::InitEditor(void* hTopWnd, void* hWnd, unsigned long nWidth,
                               unsigned long nHeight, unsigned char nBitsPerPel,
                               unsigned long nFrequency, int bFullScreen)
{
    ShowWindow(hWnd, 0);
    unsigned long dwStyle;
    void* hInsertAfter;
    if (bFullScreen) {
        _devicemodeA dm;
        ClearDevMode(&dm);
        unsigned long nMode = 0;
        while (EnumDisplaySettingsA(0, nMode, &dm)) {
            if (dm.dmPelsWidth == nWidth && dm.dmPelsHeight == nHeight &&
                dm.dmBitsPerPel == nBitsPerPel && dm.dmDisplayFrequency == nFrequency) {
                if (ChangeDisplaySettingsA(&dm, CDS_TEST))
                    return 0;
                m_devMode = dm;
                ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN);
                m_bFullScreen = 1;
                break;
            }
            ClearDevMode(&dm);
            nMode++;
        }
        if (!m_bFullScreen) {
            ShowWindow(hWnd, 1);
            return 0;
        }
        dwStyle = WS_POPUP;
        hInsertAfter = (void*)-1;
        m_nUnknownAC = 1;
    } else {
        dwStyle = WS_CHILD;
        hInsertAfter = (void*)-2;
        m_nUnknownAC = 0;
    }
    void* hDC = GetDC(hWnd);
    if (!hDC)
        return 0;
    tagPIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = 0x25;
    pfd.iPixelType = 0;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType = 0;
    int nFormat = ChoosePixelFormat(hDC, &pfd);
    if (!nFormat || !SetPixelFormat(hDC, nFormat, &pfd)) {
        ReleaseDC(hWnd, hDC);
        return 0;
    }
    DescribePixelFormat(hDC, nFormat, sizeof(pfd), &pfd);
    m_nUnknownB4 = (int)wglCreateContext(hDC);
    if (!m_nUnknownB4) {
        ReleaseDC(hWnd, hDC);
        return 0;
    }
    if (!wglMakeCurrent(hDC, (void*)m_nUnknownB4)) {
        wglDeleteContext((void*)m_nUnknownB4);
        ReleaseDC(hWnd, hDC);
        return 0;
    }
    ReleaseDC(hWnd, hDC);
    tagRECT rc;
    rc.left = 0;
    rc.right = nWidth;
    rc.top = 0;
    rc.bottom = nHeight;
    AdjustWindowRect(&rc, dwStyle, 0);
    SetWindowLongA(hWnd, GWL_STYLE, dwStyle);
    SetWindowPos(hWnd, hInsertAfter, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                 SWP_NOZORDER_SHOWWINDOW);
    SetWindowPos(hWnd, 0, 0, 0, nWidth, nHeight, SWP_NOZORDER_SHOWWINDOW);
    SendMessageA(hWnd, WM_SIZE, 0, ((unsigned short)nHeight << 16) | (unsigned short)nWidth);
    ShowWindow(hWnd, 1);
    if (g_bEditorProgramsBuilt)
        return 1;
    g_bEditorProgramsBuilt = 1;
    LoadExtensions();
    if (!AurVertexProgramsAvailable())
        return 1;
    glEnable(g_eVertexProgramTarget);
    g_pProgram37c = new VertexProgram(g_bVertexPrograms ? g_szProgramText1 : g_szVertexProgramA,
                                      g_nProgramParams54c);
    g_pProgram37c->Bind(0, 0);
    g_pProgram380 = new VertexProgram(g_bVertexPrograms ? g_szVertexProgramB : g_szVertexProgramBNV,
                                      g_nProgramParams54c);
    g_pProgram380->Bind(0, 1);
    g_pProgram384 = new VertexProgram(g_szVertexProgramC, g_nProgramParams580);
    g_pProgram384->Bind(0, 1);
    g_pProgramfec = new VertexProgram(g_bVertexPrograms ? g_szVertexProgramD : g_szVertexProgramDNV,
                                      g_nProgramParams564);
    g_pProgramfec->Bind(0, 1);
    g_pProgramff0 = new VertexProgram(g_bVertexPrograms ? g_szVertexProgramD : g_szVertexProgramDNV,
                                      g_nProgramParams560);
    g_pProgramff0->Bind(0, 0);
    g_pProgramff4 = new VertexProgram(g_bVertexPrograms ? g_szVertexProgramD : g_szVertexProgramDNV,
                                      g_nProgramParams544);
    g_pProgramff4->Bind(0, 1);
    g_pProgramff8 = new VertexProgram(g_bVertexPrograms ? g_szProgramText0 : g_szVertexProgramENV,
                                      g_nProgramParams544);
    g_pProgramff8->Bind(0, 0);
    g_pProgramffc = new VertexProgram(g_bVertexPrograms ? g_szProgramText0 : g_szVertexProgramENV,
                                      g_nProgramParams548);
    g_pProgramffc->Bind(0, 0);
    glDisable(g_eVertexProgramTarget);
    return 1;
}
