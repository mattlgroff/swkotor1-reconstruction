// CExoMoviePlayerInternal leaves (Bink playback window) and the
// neighbouring GL / vector helpers at 0x00404ae0-0x00404f7f.
// Layout partly guessed from the accesses here.

extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void* hWnd);
extern "C" __declspec(dllimport) void __stdcall glOrtho(double left, double right, double bottom, double top,
                                                       double zNear, double zFar);
extern "C" __declspec(dllimport) int __stdcall BinkGetSummary(void* hBink, void* pSummary);
extern "C" __declspec(dllimport) int __stdcall BinkPause(void* hBink, int bPause);
extern "C" __declspec(dllimport) void __stdcall BinkClose(void* hBink);
extern "C" __declspec(dllimport) void __stdcall BinkBufferClose(void* hBuffer);
extern "C" __declspec(dllimport) int __stdcall BinkBufferCheckWinPos(void* hBuffer, int* pX, int* pY);

struct BINKSUMMARY {
    unsigned long aData[31];        // 0x7c bytes
};

struct MOVIE_RECT {
    int nLeft;                      // 0x00
    int nTop;                       // 0x04
    int nX;                         // 0x08
    int nY;                         // 0x0c
};

struct BINK {
    unsigned long Width;            // 0x00
    unsigned long Height;           // 0x04
    unsigned long Frames;           // 0x08
    unsigned long FrameNum;         // 0x0c
    char pad_0x10[0x24];
    char FrameRects[4];             // 0x34
};

struct BINKBUFFER {
    unsigned long Width;            // 0x00
    unsigned long Height;           // 0x04
    char pad_0x8[0x8];
    unsigned long SurfaceType;      // 0x10
    void* Buffer;                   // 0x14
    long BufferPitch;               // 0x18
};

extern "C" __declspec(dllimport) int __stdcall BinkDoFrame(BINK* hBink);
extern "C" __declspec(dllimport) void __stdcall BinkNextFrame(BINK* hBink);
extern "C" __declspec(dllimport) int __stdcall BinkWait(BINK* hBink);
extern "C" __declspec(dllimport) int __stdcall BinkCopyToBuffer(BINK* hBink, void* pDest, long nDestPitch,
                                                                unsigned long nDestHeight, unsigned long nDestX,
                                                                unsigned long nDestY, unsigned long nFlags);
extern "C" __declspec(dllimport) int __stdcall BinkGetRects(BINK* hBink, unsigned long nFlags);
extern "C" __declspec(dllimport) int __stdcall BinkBufferLock(BINKBUFFER* hBuffer);
extern "C" __declspec(dllimport) int __stdcall BinkBufferUnlock(BINKBUFFER* hBuffer);
extern "C" __declspec(dllimport) void __stdcall BinkBufferBlit(BINKBUFFER* hBuffer, void* pRects, unsigned long nRects);
extern "C" __declspec(dllimport) int __stdcall BinkBufferSetOffset(BINKBUFFER* hBuffer, int nX, int nY);

struct tagMSG {
    void* hwnd;
    unsigned int message;
    unsigned int wParam;
    long lParam;
    unsigned long time;
    long pt_x;
    long pt_y;
};

struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
};

struct tagWINDOWINFO {
    unsigned long cbSize;           // 0x00
    tagRECT rcWindow;               // 0x04
    tagRECT rcClient;               // 0x14
    unsigned long dwStyle;          // 0x24
    unsigned long dwExStyle;        // 0x28
    unsigned long dwWindowStatus;   // 0x2c
    unsigned int cxWindowBorders;   // 0x30
    unsigned int cyWindowBorders;   // 0x34
    unsigned short atomWindowType;  // 0x38
    unsigned short wCreatorVersion; // 0x3a
};

extern "C" __declspec(dllimport) int __stdcall PeekMessageA(tagMSG* lpMsg, void* hWnd, unsigned int wMsgFilterMin,
                                                            unsigned int wMsgFilterMax, unsigned int wRemoveMsg);
extern "C" __declspec(dllimport) int __stdcall GetMessageA(tagMSG* lpMsg, void* hWnd, unsigned int wMsgFilterMin,
                                                           unsigned int wMsgFilterMax);
extern "C" __declspec(dllimport) int __stdcall TranslateMessage(const tagMSG* lpMsg);
extern "C" __declspec(dllimport) long __stdcall DispatchMessageA(const tagMSG* lpMsg);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long nMilliseconds);
extern "C" __declspec(dllimport) int __stdcall GetWindowInfo(void* hWnd, tagWINDOWINFO* pwi);
extern "C" __declspec(dllimport) int __stdcall MoveWindow(void* hWnd, int x, int y, int w, int h, int bRepaint);
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
extern "C" __declspec(dllimport) void* __stdcall GetDC(void* hWnd);
extern "C" __declspec(dllimport) int __stdcall ReleaseDC(void* hWnd, void* hDC);
extern "C" __declspec(dllimport) int __stdcall PatBlt(void* hDC, int x, int y, int w, int h, unsigned long nRop);
extern "C" __declspec(dllimport) int __stdcall ShowWindow(void* hWnd, int nCmdShow);
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* hThread, int nPriority);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* hEvent);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* pAttr, int bManualReset, int bInitialState,
                                                              const char* sName);
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* pAttr, unsigned long nStackSize,
                                                              unsigned long (__stdcall* pfnStart)(void*),
                                                              void* pParam, unsigned long nFlags,
                                                              unsigned long* pThreadId);

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                   // 0x005b3190
    ~CExoString();                  // 0x005e5c20
    CExoString& operator=(const char* s);           // 0x005e5140
    CExoString& operator=(const CExoString& s);     // 0x005e5c50
    CExoString operator+(const CExoString& s) const; // 0x005e5d10
    char* CStr() const;                             // 0x005e5670
    void Format(const char* sFormat, ...);          // 0x005e5680
    int operator!=(const char* s) const;            // 0x005e5390
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0);
    ~CExoArrayList();
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);
    void Clear(int bFree);
    void SetSize(int nSize);                        // 0x00405950 (CExoString)
    T& operator[](int i) { return element[i]; }
};

class CExoCriticalSection {
public:
    CExoCriticalSection();          // 0x005e9290
    ~CExoCriticalSection();
    void EnterCriticalSection();    // 0x005e9300
    void LeaveCriticalSection();    // 0x005e9310

    void* m_pcExoCriticalSectionInternal;
};

class CExoDebug {
public:
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005e9400
};

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sName, unsigned short nResType); // 0x005e68a0
};

class CExoBase {
public:
    char pad_0x0[8];
    CExoDebug* m_pcExoDebug;        // 0x08
    CExoAliasList* m_pcExoAliasList; // 0x0c
};

extern CExoBase* g_pExoBase;        // 0x007a39e0

class CSWGuiFadePanel;

class CSWGuiManager {
public:
    void AddPanel(CSWGuiFadePanel* pPanel, int nFlags, int bActive);    // 0x0040bc70
    int RemovePanel(CSWGuiFadePanel* pPanel);                           // 0x0040c830
};

class CClientOptions;

class CClientExoAppInternal {
public:
    char pad_0x0[0x4bc];
    int m_bEvilEndingHackInProgress; // 0x4bc
};

class CClientExoApp {
public:
    void* m_p00;
    CClientExoAppInternal* m_pInternal; // 0x04
    CSWGuiManager* GetGuiManager(); // 0x005eda40
    CClientOptions* GetClientOptions(); // 0x005ed700
};

class CAppManager {
public:
    void* m_pLargeTempBuffer;       // 0x00
    CClientExoApp* m_pClientExoApp; // 0x04
};

extern CAppManager* g_pAppManager;  // 0x007a39fc
extern void* g_hMainWindow;         // 0x007a39d8
extern int g_nScreenWidth;          // 0x0078d1d4
extern int g_nScreenHeight;         // 0x0078d1d8

void __cdecl RunFrame(float fDelta, int bRunServer, int bNoRender);    // 0x00401c10

class CExoMoviePlayerInternal {
public:
    CExoMoviePlayerInternal();
    ~CExoMoviePlayerInternal();

    int m_bQuit;                    // 0x00
    void* m_hEvent;                 // 0x04
    CExoArrayList<CExoString> m_lstMovies;  // 0x08
    int m_bAbort;                   // 0x14
    int m_n18;                      // 0x18
    int m_bDirty;                   // 0x1c
    int m_n20;                      // 0x20
    int m_bClosing;                 // 0x24
    CExoString m_sMovie;            // 0x28
    int m_nState;                   // 0x30
    int m_n34;                      // 0x34
    void* m_hThread;                // 0x38
    unsigned long m_nThreadId;      // 0x3c
    CExoCriticalSection m_csFade;   // 0x40
    class CSWGuiFadePanel* m_pFadePanel;    // 0x44
    BINK* m_hBink;                  // 0x48
    BINKBUFFER* m_hBinkBuffer;      // 0x4c
    void* m_hWnd;                   // 0x50
    char pad_0x54[0x0c];
    int m_n60;                      // 0x60
    int m_n64;                      // 0x64
    CExoString m_sMovieFile;        // 0x68
    int m_n70;                      // 0x70
    char pad_0x74[0x10];
    int m_nOffsetX;                 // 0x84
    int m_nOffsetY;                 // 0x88
    int m_n8c;                      // 0x8c
    CExoArrayList<int> m_lstSkippable;      // 0x90
    void* m_p9c;                    // 0x9c

    void CloseMovie();
    void SetMode(int n);
    void SetSkip(int n, int bForce);
    int GetState();
    void StopMovie();
    void CheckWindowPos(MOVIE_RECT* pRect);
    int PlayLoop(int n);
    void ShowFadePanel();
    void HideFadePanel();
    void UpdateWindowPos(MOVIE_RECT* pRect);
    void Resume();
    int OpenMovie(const CExoString& sMovie);                    // 0x004053e0
    void PlayMoviesAsync(CExoArrayList<CExoString>& lstMovies, int n, CExoArrayList<int>* pSkippable);
    static unsigned long __stdcall ThreadProc(void* pParam);   // 0x00405b30
};

void* g_hMovieWnd;                  // 0x007a3cf8

// FUNCTION: SWKOTOR 0x00404bb0
void CExoMoviePlayerInternal::CloseMovie()
{
    m_bClosing = 1;
    if (m_hBink) {
        BINKSUMMARY cSummary;
        BinkGetSummary(m_hBink, &cSummary);
        BinkPause(m_hBink, 1);
        BinkClose(m_hBink);
        m_hBink = 0;
        if (m_hBinkBuffer) {
            BinkBufferClose(m_hBinkBuffer);
            m_hBinkBuffer = 0;
        }
        DestroyWindow(m_hWnd);
        m_hWnd = 0;
        g_hMovieWnd = 0;
    }
    m_bClosing = 0;
}

// Called out of line by MovieWndProc.
#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x00404c20
void CExoMoviePlayerInternal::SetMode(int n)
{
    m_bDirty = 1;
    if (m_n20 == 0 || n == 1) {
        m_n20 = n;
    }
}

// FUNCTION: SWKOTOR 0x00404c40
void CExoMoviePlayerInternal::SetSkip(int n, int bForce)
{
    if (bForce || (m_nState == 1 && m_p9c)) {
        m_bAbort = 1;
    }
    m_n18 = n;
}

// FUNCTION: SWKOTOR 0x00404dd0
int CExoMoviePlayerInternal::GetState()
{
    return m_nState;
}

// FUNCTION: SWKOTOR 0x00404de0
void CExoMoviePlayerInternal::StopMovie()
{
    CloseMovie();
    m_nState = 0;
}

// FUNCTION: SWKOTOR 0x00404f40
void CExoMoviePlayerInternal::CheckWindowPos(MOVIE_RECT* pRect)
{
    if (m_hBinkBuffer) {
        int nX = pRect->nX;
        int nY = pRect->nY;
        BinkBufferCheckWinPos(m_hBinkBuffer, &nX, &nY);
        pRect->nX = nX;
        pRect->nY = nY;
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x00404b60
void _glOrtho(double left, double right, double bottom, double top, double zNear, double zFar)
{
    glOrtho(left, right, bottom, top, zNear, zFar);
}

class Vector {
public:
    float x;
    float y;
    float z;
    Vector(float fX, float fY, float fZ);
};

// FUNCTION: SWKOTOR 0x00404ae0
Vector::Vector(float fX, float fY, float fZ)
{
    x = fX;
    y = fY;
    z = fZ;
}

// The fading panel created by the movie player (0x1b8 bytes, ctor 0x00624810).
class CSWGuiFadePanel {
public:
    CSWGuiFadePanel(CSWGuiManager* pManager);   // 0x00624810
    virtual ~CSWGuiFadePanel();

    char pad_0x4[0x48];
    float m_fAlpha;                 // 0x4c
    Vector m_vColor;                // 0x50
    char pad_0x5c[0x15c];

    void SetAlpha(float fAlpha);
};

#define FADE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define FADE_MAX(a, b) ((a) > (b) ? (a) : (b))

// FUNCTION: SWKOTOR 0x00404b00
void CSWGuiFadePanel::SetAlpha(float fAlpha)
{
    m_fAlpha = FADE_MAX(FADE_MIN(fAlpha, 1.0f), 0.0f);
}

// FUNCTION: SWKOTOR 0x00404c80
int CExoMoviePlayerInternal::PlayLoop(int n)
{
    tagMSG msg;
    BINKSUMMARY cSummary;
    if (m_hBink) {
        while (m_hBink->Frames != m_hBink->FrameNum) {
            BinkDoFrame(m_hBink);
            if (BinkBufferLock(m_hBinkBuffer)) {
                BinkCopyToBuffer(m_hBink, m_hBinkBuffer->Buffer, m_hBinkBuffer->BufferPitch, m_hBinkBuffer->Height,
                                 0, 0, m_hBinkBuffer->SurfaceType);
                BinkBufferUnlock(m_hBinkBuffer);
            }
            BinkBufferBlit(m_hBinkBuffer, m_hBink->FrameRects, BinkGetRects(m_hBink, m_hBinkBuffer->SurfaceType));
            BinkNextFrame(m_hBink);
            while (BinkWait(m_hBink) == 1) {
                Sleep(1);
            }
            if (m_bAbort == 1) {
                break;
            }
            while (PeekMessageA(&msg, 0, 0, 0, 0)) {
                if (!GetMessageA(&msg, 0, 0, 0)) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
        BinkGetSummary(m_hBink, &cSummary);
    }
    return 0;
}

inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

// FUNCTION: SWKOTOR 0x00404e00
void CExoMoviePlayerInternal::ShowFadePanel()
{
    m_csFade.EnterCriticalSection();
    if (!m_pFadePanel) {
        m_pFadePanel = new CSWGuiFadePanel(g_pAppManager->m_pClientExoApp->GetGuiManager());
        m_pFadePanel->m_vColor = Vector(0.0f, 0.0f, 0.0f);
    }
    GetClientExoApp()->GetGuiManager()->AddPanel(m_pFadePanel, 3, 1);
    m_pFadePanel->SetAlpha(1.0f);
    RunFrame(0.033333335f, 0, 1);
    m_csFade.LeaveCriticalSection();
}

// FUNCTION: SWKOTOR 0x00404f00
void CExoMoviePlayerInternal::HideFadePanel()
{
    m_csFade.EnterCriticalSection();
    if (m_pFadePanel) {
        m_pFadePanel->SetAlpha(0.0f);
        GetClientExoApp()->GetGuiManager()->RemovePanel(m_pFadePanel);
    }
    m_csFade.LeaveCriticalSection();
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x00404f80
void CExoMoviePlayerInternal::UpdateWindowPos(MOVIE_RECT* pRect)
{
    if (m_hBinkBuffer) {
        BinkBufferSetOffset(m_hBinkBuffer, m_nOffsetX, m_nOffsetY);
    }
    tagWINDOWINFO wi;
    wi.cbSize = sizeof(wi);
    if (GetWindowInfo(g_hMainWindow, &wi)) {
        if (!MoveWindow(m_hWnd, wi.rcClient.left, wi.rcClient.top, wi.rcClient.right - wi.rcClient.left,
                        wi.rcClient.bottom - wi.rcClient.top, 1)) {
            GetLastError();
        }
        void* hDC = GetDC(g_hMainWindow);
        PatBlt(hDC, 0, 0, g_nScreenWidth, g_nScreenHeight, 0x42);
        ReleaseDC(g_hMainWindow, hDC);
        ShowWindow(m_hWnd, 8);
    }
}
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x00405050
void CExoMoviePlayerInternal::Resume()
{
    SetThreadPriority(m_hThread, 0);
    SetThreadPriority(m_hThread, 0);
    m_nState = 1;
    SetEvent(m_hEvent);
}

// FUNCTION: SWKOTOR 0x00405d70
CExoMoviePlayerInternal::CExoMoviePlayerInternal()
{
    m_sMovie = "";
    m_nState = 0;
    m_bQuit = 0;
    m_hEvent = CreateEventA(0, 0, 0, 0);
    m_hThread = CreateThread(0, 0x8000, ThreadProc, this, 0, &m_nThreadId);
    m_n34 = 0;
    m_bAbort = 0;
    m_n18 = 0;
    m_bDirty = 0;
    m_n20 = 0;
    m_bClosing = 0;
    m_pFadePanel = 0;
    m_hBinkBuffer = 0;
    m_hBink = 0;
    m_n70 = 0;
    m_hWnd = 0;
}

// FUNCTION: SWKOTOR 0x00405e50
void CExoMoviePlayerInternal::PlayMoviesAsync(CExoArrayList<CExoString>& lstMovies, int n,
                                              CExoArrayList<int>* pSkippable)
{
    SetThreadPriority(m_hThread, 0);
    m_lstMovies = lstMovies;
    m_n8c = n;
    if (pSkippable) {
        m_lstSkippable = *pSkippable;
        if (m_lstMovies.num > m_lstSkippable.num) {
            g_pExoBase->m_pcExoDebug->Warning(0x653, "exomovieplayerinternal.cpp",
                                              "CExoMoviePlayerInternal::PlayMoviesAsync(): More movies specified "
                                              "than skippable flags.");
        }
    } else {
        m_lstSkippable.Clear(0);
    }
    m_nState = 1;
    SetEvent(m_hEvent);
}

extern "C" __declspec(dllimport) long __stdcall DefWindowProcA(void* hWnd, unsigned int nMsg, unsigned int wParam,
                                                               long lParam);
extern "C" __declspec(dllimport) long __stdcall SendMessageA(void* hWnd, unsigned int nMsg, unsigned int wParam,
                                                             long lParam);

class CClientOptions {
public:
    char pad_0x0[0x48];
    unsigned char m_nMovieVolume;   // 0x48
    char pad_0x49[0x54 - 0x49];
    int m_bSwapMouseButtons;        // 0x54
};

extern void* g_hWindow;             // 0x007a39d4
extern CExoMoviePlayerInternal* g_pMoviePlayer; // 0x007a3cf4

inline CClientOptions* GetClientOptions() { return g_pAppManager->m_pClientExoApp->GetClientOptions(); }

// NONMATCH: 23 insns; the original keeps g_pMoviePlayer in ecx across SetMode
// (no reload before SetSkip), i.e. cross-function register knowledge.
// FUNCTION: SWKOTOR 0x00405190
long __stdcall MovieWndProc(void* hWnd, unsigned int nMsg, unsigned int wParam, long lParam)
{
    switch (nMsg) {
    case 0x104:
        if (wParam == 0x73 && (lParam & 0x20000000) && g_hWindow) {
            SendMessageA(g_hWindow, 0x10, 0, 0);
        }
        break;
    case 0x100:
        if (wParam == 0x1b && g_pMoviePlayer) {
            g_pMoviePlayer->SetSkip(0, 0);
        }
        break;
    case 0x1c:
        if (wParam == 0 && g_pMoviePlayer && !g_pMoviePlayer->m_bClosing) {
            g_pMoviePlayer->SetMode(0);
            g_pMoviePlayer->SetSkip(1, 1);
        }
        break;
    case 0x105:
        if (wParam == 0x0d && (lParam & 0x20000000) && g_pMoviePlayer) {
            g_pMoviePlayer->SetMode(1);
            g_pMoviePlayer->SetSkip(1, 1);
            return 0;
        }
        break;
    case 0x202:
        if (g_pMoviePlayer && !GetClientOptions()->m_bSwapMouseButtons) {
            g_pMoviePlayer->SetSkip(0, 0);
        }
        return 0;
    case 0x205:
        if (g_pMoviePlayer && GetClientOptions()->m_bSwapMouseButtons) {
            g_pMoviePlayer->SetSkip(0, 0);
        }
        return 0;
    case 0x201:
        return 0;
    case 0x203:
        return 0;
    case 0x204:
        return 0;
    case 0x206:
        return 0;
    case 0x408:
        g_pMoviePlayer->UpdateWindowPos((MOVIE_RECT*)lParam);
        delete (MOVIE_RECT*)lParam;
        return 0;
    case 0x407:
        g_pMoviePlayer->CheckWindowPos((MOVIE_RECT*)lParam);
        delete (MOVIE_RECT*)lParam;
        return 0;
    }
    return DefWindowProcA(hWnd, nMsg, wParam, lParam);
}

extern "C" __declspec(dllimport) int __stdcall GetExitCodeThread(void* hThread, unsigned long* pExitCode);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* hObject);
extern "C" __declspec(dllimport) __declspec(noreturn) void __stdcall ExitThread(unsigned long nExitCode);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* hHandle,
                                                                             unsigned long nMilliseconds);
extern "C" __declspec(dllimport) int __stdcall PostMessageA(void* hWnd, unsigned int nMsg, unsigned int wParam,
                                                            long lParam);

class CRenderWindow {
public:
    virtual void Slot00();
    virtual int IsFullScreen();     // 0x04
};

extern CRenderWindow* g_pRenderWindow;              // 0x007a39f8
extern int g_bMovieResolutionLocked;                // 0x007a3cc0

// FUNCTION: SWKOTOR 0x00405a50
CExoMoviePlayerInternal::~CExoMoviePlayerInternal()
{
    m_bQuit = 1;
    SetEvent(m_hEvent);
    unsigned long nExitCode;
    do {
        GetExitCodeThread(m_hThread, &nExitCode);
    } while (nExitCode == 0x103);
    CloseHandle(m_hThread);
    CloseHandle(m_hEvent);
    if (m_pFadePanel) {
        delete m_pFadePanel;
        m_pFadePanel = 0;
    }
    CloseMovie();
}

// FUNCTION: SWKOTOR 0x00405b30
unsigned long __stdcall CExoMoviePlayerInternal::ThreadProc(void* pParam)
{
    CExoMoviePlayerInternal* pPlayer = (CExoMoviePlayerInternal*)pParam;
    g_pMoviePlayer = pPlayer;
    if (!pPlayer) {
        ExitThread(0);
    }
    WaitForSingleObject(pPlayer->m_hEvent, 0xffffffff);
    while (!pPlayer->m_bQuit) {
        for (int i = 0; i < pPlayer->m_lstMovies.num; i++) {
            pPlayer->m_lstMovies[i] != "";
        }
        pPlayer->m_p9c = (void*)1;
        for (int j = 0; j < pPlayer->m_lstMovies.num; j++) {
            CExoString& sMovie = pPlayer->m_lstMovies[j];
            if (sMovie != "") {
                if (pPlayer->m_lstSkippable.num > 0) {
                    if (j < pPlayer->m_lstSkippable.num) {
                        pPlayer->m_p9c = (void*)pPlayer->m_lstSkippable[j];
                    } else {
                        pPlayer->m_p9c = (void*)1;
                    }
                }
                if (pPlayer->OpenMovie(sMovie) == 1) {
                    pPlayer->PlayLoop(1);
                    Sleep(5);
                    if (!pPlayer->m_bDirty) {
                        pPlayer->m_lstMovies[j] = "";
                    }
                    pPlayer->StopMovie();
                }
                if (!pPlayer->m_p9c) {
                    pPlayer->m_bAbort = 0;
                } else if (pPlayer->m_bAbort == 1) {
                    if (pPlayer->m_n18) {
                        break;
                    }
                    pPlayer->m_bAbort = 0;
                }
            }
        }
        if (!pPlayer->m_bDirty) {
            pPlayer->m_lstMovies.SetSize(0);
        }
        pPlayer->m_bAbort = 0;
        pPlayer->m_n18 = 0;
        pPlayer->m_nState = 0;
        if (pPlayer->m_bDirty == 1) {
            g_bMovieResolutionLocked = 1;
            if (pPlayer->m_n20 == 1) {
                if (g_pRenderWindow->IsFullScreen() == 1) {
                    PostMessageA(g_hMainWindow, 0x409, 0, 0);
                } else {
                    PostMessageA(g_hMainWindow, 0x409, 1, 0);
                }
            } else {
                PostMessageA(g_hMainWindow, 0x1c, 0, 0);
            }
            pPlayer->m_n20 = 0;
            pPlayer->m_bDirty = 0;
        } else {
            g_bMovieResolutionLocked = 0;
            PostMessageA(g_hMainWindow, 0x40a, 0, 0);
        }
        WaitForSingleObject(pPlayer->m_hEvent, 0xffffffff);
    }
    ExitThread(0);
}

struct WNDCLASSA {
    unsigned int style;
    long (__stdcall* lpfnWndProc)(void* hWnd, unsigned int nMsg, unsigned int wParam, long lParam);
    int cbClsExtra;
    int cbWndExtra;
    void* hInstance;
    void* hIcon;
    void* hCursor;
    void* hbrBackground;
    const char* lpszMenuName;
    const char* lpszClassName;
};

extern "C" __declspec(dllimport) void* __stdcall LoadIconA(void* hInstance, const char* sName);
extern "C" __declspec(dllimport) void* __stdcall LoadCursorA(void* hInstance, const char* sName);
extern "C" __declspec(dllimport) unsigned short __stdcall RegisterClassA(const WNDCLASSA* pWndClass);
extern "C" __declspec(dllimport) unsigned long __stdcall FormatMessageA(unsigned long nFlags, const void* pSource,
                                                                       unsigned long nMessageId,
                                                                       unsigned long nLanguageId, char* pBuffer,
                                                                       unsigned long nSize, void* pArgs);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd, const char* sText, const char* sCaption,
                                                           unsigned int nType);
extern "C" __declspec(dllimport) void* __stdcall LocalFree(void* hMem);
extern "C" __declspec(dllimport) void* __stdcall CreateWindowExA(unsigned long nExStyle, const char* sClassName,
                                                                 const char* sWindowName, unsigned long nStyle,
                                                                 int x, int y, int nWidth, int nHeight,
                                                                 void* hWndParent, void* hMenu, void* hInstance,
                                                                 void* pParam);
extern "C" __declspec(dllimport) void* __stdcall SetCapture(void* hWnd);
extern "C" __declspec(dllimport) long __stdcall SetWindowLongA(void* hWnd, int nIndex, long nNewLong);
extern "C" __declspec(dllimport) int __stdcall SetWindowPos(void* hWnd, void* hWndInsertAfter, int x, int y,
                                                            int cx, int cy, unsigned int nFlags);
extern "C" __declspec(dllimport) int __stdcall AIL_quick_handles(void** pDig, void** pMdi, void** pDls);
extern "C" __declspec(dllimport) void* __stdcall BinkOpenMiles(unsigned long nParam);
extern "C" __declspec(dllimport) int __stdcall BinkSetSoundSystem(void* pOpen, unsigned long nParam);
extern "C" __declspec(dllimport) BINK* __stdcall BinkOpen(const char* sName, unsigned long nFlags);
extern "C" __declspec(dllimport) BINKBUFFER* __stdcall BinkBufferOpen(void* hWnd, unsigned long nWidth,
                                                                      unsigned long nHeight, unsigned long nFlags);
extern "C" __declspec(dllimport) void __stdcall BinkSetVolume(BINK* hBink, unsigned long nTrack, int nVolume);
extern "C" __declspec(dllimport) int __stdcall BinkBufferSetScale(BINKBUFFER* hBuffer, unsigned long nWidth,
                                                                  unsigned long nHeight);
extern "C" char* __cdecl strchr(const char* s, int c);

extern void* g_hInstance;           // 0x007a39c0
extern int g_bMovieWaitForFrame;    // 0x007a3c9c (descriptive)
int g_bMovieClassRegistered;        // 0x007a3cfc

// NONMATCH: 65 insns, body and calls line up; the original keeps the
// constant 1 in edi from the entry block (saves edi up front, ebp later) and
// has a frame 4 bytes larger, which shifts every stack offset.
// FUNCTION: SWKOTOR 0x004053e0
int CExoMoviePlayerInternal::OpenMovie(const CExoString& sMovie)
{
    WNDCLASSA wc;
    tagWINDOWINFO wi;
    char* pMessage;
    void* hDig;
    int nLeft;
    int nTop;
    int nRight;
    int nBottom;
    int nWidth;
    m_hWnd = 0;
    g_hMovieWnd = 0;
    if (!g_bMovieClassRegistered) {
        wc.style = 0;
        wc.lpfnWndProc = MovieWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = g_hInstance;
        wc.hIcon = LoadIconA(g_hInstance, (const char*)0x65);
        wc.hCursor = LoadCursorA(0, (const char*)0x7f00);
        wc.hbrBackground = 0;
        wc.lpszMenuName = 0;
        wc.lpszClassName = "SWMovieWindow";
        if (!RegisterClassA(&wc)) {
            FormatMessageA(0x1300, 0, GetLastError(), 0x400, (char*)&pMessage, 0, 0);
            MessageBoxA(0, pMessage, "Error", 0x40);
            LocalFree(pMessage);
            return 0;
        }
        g_bMovieClassRegistered = 1;
    }
    m_nState = 1;
    wi.cbSize = sizeof(wi);
    if (!GetWindowInfo(g_hMainWindow, &wi)) {
        return 0;
    }
    nLeft = wi.rcClient.left;
    nTop = wi.rcClient.top;
    nRight = wi.rcClient.right;
    nBottom = wi.rcClient.bottom;
    void* hWnd = CreateWindowExA(0, "SWMovieWindow", "SW Movie Player Window", 0x90000000, nLeft, nTop,
                                 nRight - nLeft, nBottom - nTop, g_hMainWindow, 0, g_hInstance, 0);
    m_hWnd = hWnd;
    g_hMovieWnd = hWnd;
    SetCapture(hWnd);
    g_pRenderWindow->IsFullScreen();
    SetWindowLongA(m_hWnd, -16, 0x82000000);
    nWidth = nRight - nLeft;
    SetWindowPos(m_hWnd, (void*)-1, nLeft, nTop, nWidth, nBottom - nTop, 0x121);
    m_hBink = 0;
    m_n60 = 0;
    m_n64 = 0;
    if (!strchr(sMovie.CStr(), '.')) {
        m_sMovieFile = sMovie;
        if (!strchr(m_sMovieFile.CStr(), ':')) {
            m_sMovieFile.Format("MOVIES:%s", m_sMovieFile.CStr());
        }
        m_sMovieFile = g_pExoBase->m_pcExoAliasList->ResolveFileName(m_sMovieFile, 0x80f);
    } else {
        m_sMovieFile = sMovie;
    }
    AIL_quick_handles(&hDig, 0, 0);
    BinkSetSoundSystem(BinkOpenMiles, (unsigned long)hDig);
    m_hBink = BinkOpen(m_sMovieFile.CStr(), 0x8000000);
    if (!m_hBink) {
        CExoString sError;
        sError = "CExoMoviePlayerInternal::CExoMoviePlayerInternal()  Can't open movie: ";
        sError = sError + m_sMovieFile;
        g_pExoBase->m_pcExoDebug->Warning(0x43b, "exomovieplayerinternal.cpp", sError.CStr());
        CloseMovie();
        m_nState = 0;
        return 0;
    }
    m_hBinkBuffer = 0;
    m_hBinkBuffer = BinkBufferOpen(m_hWnd, m_hBink->Width, m_hBink->Height, 0x5d000000);
    if (!m_hBinkBuffer) {
        BinkClose(m_hBink);
        m_nState = 0;
        return 0;
    }
    int nVolume = GetClientOptions()->m_nMovieVolume;
    if (g_pAppManager->m_pClientExoApp->m_pInternal->m_bEvilEndingHackInProgress) {
        nVolume = 0;
    }
    BinkSetVolume(m_hBink, 0, (nVolume << 15) / 100);
    if (nWidth != m_hBink->Width) {
        float fScale = (float)nWidth / (float)m_hBink->Width;
        int nScaledWidth = (int)(m_hBink->Width * fScale);
        int nScaledHeight = (int)(m_hBink->Height * fScale);
        BinkBufferSetScale(m_hBinkBuffer, nScaledWidth, nScaledHeight);
        m_nOffsetX = (nRight - nScaledWidth - nLeft) / 2;
        m_nOffsetY = (nBottom - nScaledHeight - nTop) / 2;
    } else {
        m_nOffsetX = (nRight - m_hBink->Width - nLeft) >> 1;
        m_nOffsetY = (nBottom - m_hBink->Height - nTop) >> 1;
    }
    BinkBufferSetOffset(m_hBinkBuffer, m_nOffsetX, m_nOffsetY);
    m_nState = 1;
    while (g_bMovieWaitForFrame == 1) {
        Sleep(1);
    }
    void* hDC = GetDC(g_hMainWindow);
    PatBlt(hDC, 0, 0, g_nScreenWidth, g_nScreenHeight, 0x42);
    ReleaseDC(g_hMainWindow, hDC);
    ShowWindow(m_hWnd, 8);
    return 1;
}
