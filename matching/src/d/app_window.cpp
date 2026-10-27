// Main and render window creation (0x004036f0 / 0x00403f20).
#include "d/exo_key_table.h"

struct WNDCLASSA {
    unsigned int style;
    long(__stdcall* lpfnWndProc)(void* hWnd, unsigned int nMsg, unsigned int wParam, long lParam);
    int cbClsExtra;
    int cbWndExtra;
    void* hInstance;
    void* hIcon;
    void* hCursor;
    void* hbrBackground;
    const char* lpszMenuName;
    const char* lpszClassName;
};

struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

struct MONITORINFO {
    unsigned long cbSize;
    RECT rcMonitor;
    RECT rcWork;
    unsigned long dwFlags;
};

extern "C" {
__declspec(dllimport) void* __stdcall LoadIconA(void* hInstance, const char* szIcon);
__declspec(dllimport) void* __stdcall GetStockObject(int i);
__declspec(dllimport) unsigned short __stdcall RegisterClassA(const WNDCLASSA* p);
__declspec(dllimport) void* __stdcall CreateWindowExA(unsigned long dwExStyle, const char* szClass,
                                                      const char* szName, unsigned long dwStyle, int x, int y,
                                                      int w, int h, void* hParent, void* hMenu, void* hInstance,
                                                      void* p);
__declspec(dllimport) void* __stdcall MonitorFromWindow(void* hWnd, unsigned long dwFlags);
__declspec(dllimport) int __stdcall GetMonitorInfoA(void* hMonitor, MONITORINFO* pInfo);
void* __cdecl memset(void* p, int c, unsigned int n);
}
#pragma intrinsic(memset)

long __stdcall WndProc(void* hWnd, unsigned int nMsg, unsigned int wParam, long lParam); // 0x00402800
void __cdecl LoadVersionInfo();                     // 0x005e6b10
char* __cdecl AurGetVersionString();                // 0x0044eec0

extern void* g_hInstance;                           // 0x007a39c0
extern void* g_hWindow;                             // 0x007a39d4
extern void* g_hRenderWindow;                       // 0x007a39d8
extern CExoString g_sBuildNumber;                   // 0x007a3cc4
extern CExoString g_sWindowSubTitle;                // 0x007a3ccc
extern CExoString g_sWindowTitle;                   // 0x007a3cdc
extern char g_szWindowClassName[];                  // 0x0078d200

// FUNCTION: SWKOTOR 0x004036f0
void __cdecl InitRenderWindow(void* hInstance, int nCmdShow)
{
    char szClassName[] = "Render Window";
    CExoString sText(g_sWindowSubTitle);
    WNDCLASSA wc;
    wc.style = 0xb;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconA(hInstance, (const char*)0x65);
    wc.hCursor = 0;
    wc.hbrBackground = GetStockObject(4);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szClassName;
    RegisterClassA(&wc);
    void* hWnd = CreateWindowExA(0x40000, szClassName, sText.CStr(), 0x2ca0000, 0x80000000, 0x80000000,
                                 0x80000000, 0x80000000, 0, 0, hInstance, 0);
    if (!hWnd) {
        return;
    }
    g_hRenderWindow = hWnd;
}

// FUNCTION: SWKOTOR 0x00403f20
void* __cdecl InitApplication(void* hInstance, int nCmdShow)
{
    LoadVersionInfo();
    g_sBuildNumber = g_sBuildNumber + " Graphics " + AurGetVersionString();
    CExoString sTitle = g_sWindowTitle + " " + g_sWindowSubTitle;
    CExoString sText(g_sWindowSubTitle);
    WNDCLASSA wc;
    wc.style = 0xb;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconA(hInstance, (const char*)0x65);
    wc.hCursor = 0;
    wc.hbrBackground = GetStockObject(4);
    wc.lpszMenuName = 0;
    wc.lpszClassName = g_szWindowClassName;
    RegisterClassA(&wc);
    void* hWnd = CreateWindowExA(0x80, g_szWindowClassName, sText.CStr(), 0x2ca0000, 0x80000000, 0x80000000,
                                 0x80000000, 0x80000000, 0, 0, hInstance, 0);
    if (!hWnd) {
        return 0;
    }
    void* hMonitor = MonitorFromWindow(hWnd, 0);
    if (hMonitor) {
        MONITORINFO mi;
        memset(&mi, 0, sizeof(mi));
        mi.cbSize = sizeof(mi);
        if (!GetMonitorInfoA(hMonitor, &mi) || mi.dwFlags != 1) {
            return 0;
        }
    }
    InitRenderWindow(hInstance, nCmdShow);
    g_hWindow = hWnd;
    g_hInstance = hInstance;
    return hWnd;
}
