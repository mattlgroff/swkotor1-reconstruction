// Switching the display mode around fullscreen movies (0x00403cf0 /
// 0x00403de0): the movie plays at 640x480x32 and the previous mode is
// restored afterwards. Names for the 0x007a3cb8-0x007a3cc0 and
// 0x0078d1f0-0x0078d1fc cells are descriptive.

extern "C" __declspec(dllimport) void* __stdcall GetCurrentThread();
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* hThread, int nPriority);

class CClientOptions {
public:
    char pad_0x00[0x44];
    unsigned char m_nAntiAliasing;                  // 0x44
};

class CClientExoAppInternal {
public:
    void HideMoviePlayerFadePanel();                // 0x005f4c50
    int ReadVideoModeSettings(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pDepth,
                              unsigned long* pRefresh); // 0x005f0ce0
    char pad_0x00[0x1f8];
    int m_n1f8;                                     // 0x1f8
    int m_n1fc;                                     // 0x1fc
    char pad_0x200[0x2bc];
    int m_bMoviePlaying;                            // 0x4bc
};

class CClientExoApp {
public:
    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
    CClientOptions* GetOptions();                   // 0x005ed700
};

class CAppManager {
public:
    void* m_pLargeTempBuffer;                       // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

class CRenderWindow {
public:
    virtual void Slot00();
    virtual int IsFullScreen();                     // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007a39fc
extern CRenderWindow* g_pRenderWindow;              // 0x007a39f8
extern int g_nScreenWidth;                          // 0x0078d1d4
extern int g_nScreenHeight;                         // 0x0078d1d8
extern unsigned char g_nScreenDepth;                // 0x0078d1e0
extern unsigned long g_nSavedScreenWidth;           // 0x0078d1f0
extern unsigned long g_nSavedScreenHeight;          // 0x0078d1f4
extern unsigned char g_nSavedScreenDepth;           // 0x0078d1f8
extern int g_bRereadVideoMode;                      // 0x0078d1fc
extern int g_bMovieVideoMode;                       // 0x007a3c54
extern int g_nMovieResolutionNesting;               // 0x007a3cb8
extern int g_bMovieResolutionChanged;               // 0x007a3cbc
extern int g_bMovieResolutionLocked;                // 0x007a3cc0

void __cdecl AurSetAntiAliasingMode(int nMode);     // 0x0044f2f0
int __cdecl ReInitAurora(int nWidth, int nHeight, unsigned char nDepth, int bFullScreen, int nArg); // 0x00403800
void __cdecl AppActivate();                         // 0x00401e00

// FUNCTION: SWKOTOR 0x00403cf0
void __cdecl ChangeResolutionForMovie()
{
    g_nMovieResolutionNesting++;
    g_nSavedScreenHeight = g_nScreenHeight;
    g_nSavedScreenWidth = g_nScreenWidth;
    g_nSavedScreenDepth = g_nScreenDepth;
    if (g_pRenderWindow->IsFullScreen()) {
        g_bMovieResolutionChanged = 1;
        CClientExoAppInternal* pInternal = g_pAppManager->m_pClientExoApp->m_pInternal;
        if (!g_bMovieVideoMode || pInternal->m_n1f8 || pInternal->m_n1fc) {
            if (g_nScreenWidth != 640 || g_nScreenHeight != 480 || g_nScreenDepth != 32) {
                AurSetAntiAliasingMode(0);
                ReInitAurora(640, 480, 32, 1, 0);
            }
        }
        g_bMovieVideoMode = 0;
        SetThreadPriority(GetCurrentThread(), -2);
    } else {
        g_bMovieResolutionChanged = 0;
        SetThreadPriority(GetCurrentThread(), -2);
    }
}

// FUNCTION: SWKOTOR 0x00403de0
void __cdecl RestoreResolutionFromMovie()
{
    CClientExoAppInternal* pInternal = g_pAppManager->m_pClientExoApp->m_pInternal;
    if (pInternal && pInternal->m_bMoviePlaying) {
        SetThreadPriority(GetCurrentThread(), 0);
        pInternal->HideMoviePlayerFadePanel();
        if (g_bMovieResolutionChanged == 1 && !g_bMovieResolutionLocked) {
            AppActivate();
        }
        return;
    }
    if (--g_nMovieResolutionNesting < 0) {
        g_nMovieResolutionNesting = 0;
        return;
    }
    SetThreadPriority(GetCurrentThread(), 0);
    if (pInternal) {
        pInternal->HideMoviePlayerFadePanel();
    }
    if (g_bRereadVideoMode) {
        g_bRereadVideoMode = 0;
        if (pInternal) {
            unsigned long nRefresh;
            pInternal->ReadVideoModeSettings(&g_nSavedScreenWidth, &g_nSavedScreenHeight, &g_nSavedScreenDepth,
                                             &nRefresh);
        }
    }
    if (g_bMovieResolutionChanged == 1 && !g_bMovieResolutionLocked) {
        if (g_nSavedScreenWidth == 640 && g_nSavedScreenHeight == 480 && g_nSavedScreenDepth == 32) {
            AppActivate();
            return;
        }
        AurSetAntiAliasingMode(g_pAppManager->m_pClientExoApp->GetOptions()->m_nAntiAliasing);
        ReInitAurora(g_nSavedScreenWidth, g_nSavedScreenHeight, g_nSavedScreenDepth, g_bMovieResolutionChanged, 0);
    }
}
