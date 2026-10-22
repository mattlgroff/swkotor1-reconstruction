// small application-level leaves at 0x00401000-0x0040168f
// (server stats dtor, CAppManager helpers, Win32 mouse-trail / screen-saver
// state, the glBlendFunc wrapper).
#include "d/exo_key_table.h"

extern "C" __declspec(dllimport) int __stdcall SystemParametersInfoA(unsigned int uiAction, unsigned int uiParam,
                                                                    void* pvParam, unsigned int fWinIni);
extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int sfactor, unsigned int dfactor);

class CSWReentrantServerStats {
public:
    CSWReentrantServerStats();
    ~CSWReentrantServerStats();

    int m_n00;                      // 0x00
    int m_n04;                      // 0x04
    int m_n08;                      // 0x08
    int m_n0c;                      // 0x0c
    int m_n10;                      // 0x10
    int m_n14;                      // 0x14
    CExoString m_s18;               // 0x18
    CExoString m_s20;               // 0x20
    CExoString m_s28;               // 0x28
    char pad_0x30[0x0c];
};

// FUNCTION: SWKOTOR 0x00401000
CSWReentrantServerStats::~CSWReentrantServerStats()
{
}

// FUNCTION: SWKOTOR 0x004010c0
CSWReentrantServerStats::CSWReentrantServerStats()
{
    m_n00 = 0;
    m_n04 = 0;
    m_n0c = 0;
    m_n08 = 0;
    m_n10 = 0;
    m_n14 = 0;
}

void __cdecl SaveScreenShot(char* szName, int nSize, int bA, int bB); // 0x00420f20
void __cdecl UpdateWindowTitle();   // 0x00401440

class CNetLayer {
public:
    void ProcessReceivedFrames(int bFlag);             // 0x005d57d0
    void UpdateStatusLoop(int nTime);                  // 0x005d5650
};

class CSWGuiManager {
public:
    void Draw(float fDelta);        // 0x0040cc50
};

class CClientExoApp {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual CNetLayer* GetNetLayer();                  // 0x10
    CClientExoApp();                // 0x005ee2f0
    int IsMoviePlaying();           // 0x005edb40
    CSWGuiManager* GetGuiManager(); // 0x005eda40
    ~CClientExoApp();               // 0x005ee360
    void CreateCaptureEventFile(CExoString& sFile);    // 0x005ed820
    void setUpEventPlayBack(CExoString& sFile);        // 0x005ed830
    void setCaptureEventRate(int nRate);               // 0x005ed840
    void SetInputActive(int bActive, int bClear);      // 0x005eda10
    class CClientUnknown700* GetUnknown700();          // 0x005ed700

    char pad[4];
};

class CClientUnknown700 {
public:
    float GetValue();               // 0x0061d590
};

class CServerExoApp {
public:
    CServerExoApp();                // 0x004aec00
    ~CServerExoApp();               // 0x004aec70
    void ShutDown();                // 0x005ed310 (folded empty forwarder)
    int Initialize();               // 0x004ae850
    int StartServices(int n);       // 0x004ae8f0
    void MainLoop();                // 0x004ae860
    void StopServices();            // 0x004ae900
    int GetPauseState(unsigned char nMask);             // 0x004ae980
    void SetPauseState(unsigned char nState, int bPause); // 0x004ae9a0

    char pad[8];
};

class CObjectTableManager {
public:
    CObjectTableManager();          // 0x004ae5d0
    ~CObjectTableManager();         // 0x004ae640

    char pad[0x184];
};

class CAppManager {
public:
    void* m_pLargeTempBuffer;       // 0x00
    CClientExoApp* m_pClientExoApp; // 0x04
    CServerExoApp* m_pServerExoApp; // 0x08
    CObjectTableManager* m_pObjectTableManagerServer; // 0x0c
    CObjectTableManager* m_pObjectTableManagerClient; // 0x10
    CSWReentrantServerStats* m_pReentrantServerStats; // 0x14
    unsigned long m_nStartTime;     // 0x18
    int m_bPaused;                  // 0x1c

    CAppManager();
    ~CAppManager();
    CObjectTableManager* GetObjectTableManager(int nWhich);
    void DoSaveGameScreenShot(CExoString& sName);
    void AllocLargeTempBuffer();
    void DestroyServer();
    void CreateServer(int nMode);
};

extern CAppManager* g_pAppManager;  // 0x007a39fc

// FUNCTION: SWKOTOR 0x00401060
CObjectTableManager* CAppManager::GetObjectTableManager(int nWhich)
{
    if (nWhich == 0) {
        return m_pObjectTableManagerServer;
    }
    if (nWhich == 1) {
        return m_pObjectTableManagerClient;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00401080
void CAppManager::DoSaveGameScreenShot(CExoString& sName)
{
    SaveScreenShot(sName.CStr(), 0x100, 1, 1);
}

// FUNCTION: SWKOTOR 0x004010a0
void CAppManager::AllocLargeTempBuffer()
{
    if (m_pLargeTempBuffer == 0) {
        m_pLargeTempBuffer = new char[0x40000];
    }
}

// FUNCTION: SWKOTOR 0x00401430
const char* GetNotEnabledText()
{
    return "NOT ENABLED";
}

// FUNCTION: SWKOTOR 0x00401590
int __stdcall ReturnZero4(int a, int b, int c, int d)
{
    return 0;
}

int g_nMouseTrails;                 // 0x007a3ca8
int g_bScreenSaverActive;           // 0x007a3cac

// FUNCTION: SWKOTOR 0x004015b0
void StoreMouseTrailsState()
{
    int nTrails = 0;
    if (SystemParametersInfoA(0x5e, 0, &nTrails, 0) && nTrails >= 1) {
        g_nMouseTrails = nTrails;
        SystemParametersInfoA(0x5d, 0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x004015f0
void handleMouseTrails()
{
    if (g_nMouseTrails >= 1) {
        SystemParametersInfoA(0x5d, g_nMouseTrails, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x00401610
void StoreScreenSaverState()
{
    int bActive = 0;
    if (SystemParametersInfoA(0x10, 0, &bActive, 0) && bActive >= 1) {
        g_bScreenSaverActive = bActive;
        SystemParametersInfoA(0x11, 0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x00401650
void handleScreenSaver()
{
    if (g_bScreenSaverActive >= 1) {
        SystemParametersInfoA(0x11, g_bScreenSaverActive, 0, 0);
    }
}

extern unsigned int g_aGLBlendFactors[]; // 0x0073f270

// FUNCTION: SWKOTOR 0x00401670
void _glBlendFunc(int nSrc, int nDst)
{
    glBlendFunc(g_aGLBlendFactors[nSrc], g_aGLBlendFactors[nDst]);
}

// ---------------------------------------------------------------------------
// CAppManager lifecycle (0x00401130-0x00401520)

extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();
extern "C" __declspec(dllimport) int __stdcall SetWindowTextA(void* hWnd, const char* lpString);

struct tagMSG {
    void* hwnd;
    unsigned int message;
    unsigned int wParam;
    long lParam;
    unsigned long time;
    long pt_x;
    long pt_y;
};

extern "C" __declspec(dllimport) int __stdcall PeekMessageA(tagMSG* lpMsg, void* hWnd, unsigned int wMsgFilterMin,
                                                            unsigned int wMsgFilterMax, unsigned int wRemoveMsg);
extern "C" __declspec(dllimport) int __stdcall GetMessageA(tagMSG* lpMsg, void* hWnd, unsigned int wMsgFilterMin,
                                                           unsigned int wMsgFilterMax);
extern "C" __declspec(dllimport) int __stdcall TranslateMessage(const tagMSG* lpMsg);
extern "C" __declspec(dllimport) long __stdcall DispatchMessageA(const tagMSG* lpMsg);

class Vector {
public:
    float x, y, z;
    Vector();                       // 0x00409e70
};

void UseVectors(Vector* p);

void NewVectors()
{
    Vector av[4];
    UseVectors(av);
}

// FUNCTION: SWKOTOR 0x00401130 SYMBOL: ??_H@YGXPAXIHP6EPAX0@Z@Z

// FUNCTION: SWKOTOR 0x00401160
CAppManager::CAppManager()
{
    m_pClientExoApp = 0;
    m_pServerExoApp = 0;
    m_nStartTime = GetTickCount();
    m_pClientExoApp = new CClientExoApp;
    m_pReentrantServerStats = new CSWReentrantServerStats;
    m_pObjectTableManagerServer = new CObjectTableManager;
    m_pObjectTableManagerClient = new CObjectTableManager;
    m_pLargeTempBuffer = 0;
    AllocLargeTempBuffer();
}

// FUNCTION: SWKOTOR 0x00401280
void CAppManager::DestroyServer()
{
    if (m_pServerExoApp) {
        m_pServerExoApp->ShutDown();
        m_pServerExoApp->StopServices();
        delete m_pServerExoApp;
        m_pServerExoApp = 0;
    }
    UpdateWindowTitle();
}

// FUNCTION: SWKOTOR 0x004012c0
CAppManager::~CAppManager()
{
    if (m_pClientExoApp) {
        delete m_pClientExoApp;
        m_pClientExoApp = 0;
    }
    DestroyServer();
    if (m_pObjectTableManagerServer) {
        delete m_pObjectTableManagerServer;
        m_pObjectTableManagerServer = 0;
    }
    if (m_pObjectTableManagerClient) {
        delete m_pObjectTableManagerClient;
        m_pObjectTableManagerClient = 0;
    }
    if (m_pReentrantServerStats) {
        delete m_pReentrantServerStats;
        m_pReentrantServerStats = 0;
    }
    if (m_pLargeTempBuffer) {
        delete[] (char*)m_pLargeTempBuffer;
        m_pLargeTempBuffer = 0;
    }
}

// FUNCTION: SWKOTOR 0x00401380
void CAppManager::CreateServer(int nMode)
{
    if (m_pServerExoApp) {
        DestroyServer();
    }
    m_pServerExoApp = new CServerExoApp;
    m_pServerExoApp->StartServices(nMode);
    m_pServerExoApp->Initialize();
}

extern void* g_hWindow;             // 0x007a39d4
extern CExoString g_sWindowTitle;   // 0x007a3cdc
extern CExoString g_sWindowSubTitle; // 0x007a3ccc
extern int g_bQuitRequested;        // 0x007a39b8

// FUNCTION: SWKOTOR 0x00401440
void __cdecl UpdateWindowTitle()
{
    CExoString sTitle = g_sWindowTitle + " " + g_sWindowSubTitle;
    CExoString sText(g_sWindowSubTitle);
    SetWindowTextA(g_hWindow, sText.CStr());
}

// FUNCTION: SWKOTOR 0x00401510
void __cdecl PumpMessages()
{
    tagMSG msg;
    while (PeekMessageA(&msg, 0, 0, 0, 0)) {
        if (GetMessageA(&msg, 0, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        } else {
            g_bQuitRequested = 1;
        }
    }
}

// ---------------------------------------------------------------------------
// Start-up / activation helpers (0x00401690-0x00401ef0)

extern "C" int __cdecl sscanf(const char* s, const char* fmt, ...);
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);
extern "C" long __cdecl atol(const char* s);
extern "C" __declspec(dllimport) void __stdcall glClearColor(float r, float g, float b, float a);
extern "C" __declspec(dllimport) void __stdcall glEnable(unsigned int cap);
extern "C" __declspec(dllimport) long __stdcall SendMessageA(void* hWnd, unsigned int msg, unsigned int wParam,
                                                             long lParam);

// FUNCTION: SWKOTOR 0x00401690
void __cdecl SetClearColorFromString(const char* sColor)
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    sscanf(sColor, "%f %f %f %f", &r, &g, &b, &a);
    glClearColor(r, g, b, a);
}

inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

// FUNCTION: SWKOTOR 0x00401700
void __cdecl ParseEventCaptureArgs(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-ecf", 4) == 0) {
            i++;
            CExoString sFile("capture");
            if (i < argc) {
                sFile = argv[i];
            }
            GetClientExoApp()->CreateCaptureEventFile(sFile);
        } else if (strncmp(argv[i], "-epf", 4) == 0) {
            i++;
            CExoString sFile("capture");
            if (i < argc) {
                sFile = argv[i];
            }
            GetClientExoApp()->setUpEventPlayBack(sFile);
        } else if (strncmp(argv[i], "-epr", 4) == 0) {
            i++;
            int nRate = 30;
            if (i < argc) {
                nRate = atol(argv[i]);
            }
            GetClientExoApp()->setCaptureEventRate(nRate);
        } else if (strncmp(argv[i], "-epm", 4) == 0) {
        }
    }
}

extern unsigned int g_nGLCap0;      // 0x0078d454
extern unsigned int g_nGLCap1;      // 0x0078d474
extern unsigned int g_nGLCap2;      // 0x0078d45c
extern unsigned int g_nGLCap3;      // 0x0078d458
extern unsigned int g_nGLCap4;      // 0x0078d448
extern unsigned int g_nGLCap5;      // 0x0078d44c

// FUNCTION: SWKOTOR 0x00401bb0
void __cdecl InitGLState()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(g_nGLCap0);
    glEnable(g_nGLCap1);
    glEnable(g_nGLCap2);
    glEnable(g_nGLCap3);
    glEnable(g_nGLCap4);
    _glBlendFunc(0, 1);
    glEnable(g_nGLCap5);
}

class CExoSound {
public:
    void SetSoundMode(int nMode);   // 0x005d5e80
    void Render();                  // 0x005d5dd0
};

class CRenderWindow {
public:
    virtual void Slot00();
    virtual int IsFullScreen();     // 0x04
    virtual void Slot08();
    virtual void Slot0c();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1c();
    virtual void Slot20();
    virtual void SetGamma(float f, int bApply); // 0x24
};

extern int g_bAppActive;            // 0x007a3a38
extern CExoSound* g_pExoSound;      // 0x007a39ec
extern int g_nSoundModeInactive;    // 0x0074c600
extern int g_nSoundModeActive;      // 0x0074c604
extern CRenderWindow* g_pRenderWindow; // 0x007a39f8
extern int g_bWindowed;             // 0x007a3a30
extern int g_nActivateState;        // 0x007a3a2c
extern void* g_hMainWindow;         // 0x007a39d8

// FUNCTION: SWKOTOR 0x00401d90
void __cdecl AppDeactivate()
{
    if (g_bAppActive) {
        if (g_pAppManager->m_pServerExoApp) {
            int bPaused = g_pAppManager->m_pServerExoApp->GetPauseState(2);
            g_pAppManager->m_bPaused = bPaused;
            if (!bPaused) {
                g_pAppManager->m_pServerExoApp->SetPauseState(2, 1);
            }
        }
        g_pAppManager->m_pClientExoApp->SetInputActive(0, 0);
        if (g_pExoSound) {
            g_pExoSound->SetSoundMode(g_nSoundModeInactive);
        }
        g_bAppActive = 0;
    }
}

// FUNCTION: SWKOTOR 0x00401e00
void __cdecl AppActivate()
{
    if (!g_bAppActive) {
        g_bAppActive = 1;
        if (g_pAppManager->m_pServerExoApp && !g_pAppManager->m_bPaused) {
            g_pAppManager->m_pServerExoApp->SetPauseState(2, 0);
        }
        g_pAppManager->m_pClientExoApp->SetInputActive(1, 0);
        if (g_pExoSound) {
            g_pExoSound->SetSoundMode(g_nSoundModeActive);
        }
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        g_pRenderWindow->SetGamma(pClient->GetUnknown700()->GetValue(), 0);
    }
}

// FUNCTION: SWKOTOR 0x00401e90
void __cdecl OnAppActivated(int nActive, int nMinimized)
{
    g_pAppManager->m_pClientExoApp->SetInputActive(1, 0);
    if (g_bWindowed) {
        if (g_pRenderWindow) {
            if (nActive == 1) {
                g_nActivateState = (nMinimized == 1) ? 2 : 1;
                return;
            }
            g_nActivateState = g_pRenderWindow->IsFullScreen() ? 1 : 2;
        }
    } else {
        SendMessageA(g_hMainWindow, 0x1c, 0, 0);
    }
}

// ---------------------------------------------------------------------------
// Main loop frame (0x00401c10)

extern "C" __declspec(dllimport) void __stdcall glClear(unsigned int mask);
extern "C" __declspec(dllimport) void* __stdcall GetDC(void* hWnd);
extern "C" __declspec(dllimport) int __stdcall ReleaseDC(void* hWnd, void* hDC);
extern "C" __declspec(dllimport) int __stdcall SwapBuffers(void* hDC);

void __cdecl AurStartupWait();      // 0x0044ed90
void __cdecl AurEnableHeadLight();  // 0x0044da60
void __cdecl AurDisableHeadLight(); // 0x0044da80
void __cdecl AurPostString(const char* s, int x, int y, float fLife); // 0x0044d490
void __cdecl AurRenderStrings();    // 0x0044d6a0

extern int g_bHeadLight;            // 0x007a3c50
extern int g_bShowFrameInfo;        // 0x007a3a40
extern char g_szFrameInfo[];        // 0x007a3a4c
extern int g_nNetStatusTime;        // 0x0074c538

// FUNCTION: SWKOTOR 0x00401c10
void __cdecl RunFrame(float fDelta, int bRunServer, int bNoRender)
{
    AurStartupWait();
    if (!bNoRender && !g_pAppManager->m_pClientExoApp->IsMoviePlaying()) {
        glClear(0x4500);
        if (g_bHeadLight) {
            AurEnableHeadLight();
        } else {
            AurDisableHeadLight();
        }
    }
    GetClientExoApp()->GetGuiManager()->Draw(fDelta);
    if (!bNoRender && !g_pAppManager->m_pClientExoApp->IsMoviePlaying()) {
        if (g_bShowFrameInfo) {
            AurPostString(">", 0, 0, 5.0f);
            AurPostString(g_szFrameInfo, 2, 0, 5.0f);
        }
        AurRenderStrings();
        void* hDC = GetDC(g_hMainWindow);
        SwapBuffers(hDC);
        ReleaseDC(g_hMainWindow, hDC);
    }
    PumpMessages();
    if (bRunServer == 1 && g_pAppManager) {
        if (g_pAppManager->m_pServerExoApp) {
            g_pAppManager->m_pServerExoApp->MainLoop();
            PumpMessages();
            if (g_pAppManager->m_pClientExoApp) {
                g_pAppManager->m_pClientExoApp->GetNetLayer()->ProcessReceivedFrames(0);
            }
        }
        if (g_pAppManager->m_pClientExoApp) {
            g_pAppManager->m_pClientExoApp->GetNetLayer()->ProcessReceivedFrames(0);
        }
    }
    if (g_pExoSound) {
        g_pExoSound->Render();
    }
    if (g_pAppManager && g_pAppManager->m_pClientExoApp && g_pAppManager->m_pClientExoApp->GetNetLayer()) {
        g_pAppManager->m_pClientExoApp->GetNetLayer()->UpdateStatusLoop(g_nNetStatusTime);
    }
    g_pAppManager->m_pClientExoApp->GetGuiManager()->Draw(0.0f);
}
