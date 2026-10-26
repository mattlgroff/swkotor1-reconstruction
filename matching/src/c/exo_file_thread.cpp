// CExoFileThread constructor / destructor (b005e8_exo_rand_client_misc).

typedef void* HANDLE;

extern "C" {
__declspec(dllimport) HANDLE __stdcall CreateThread(void* pAttributes, unsigned long nStackSize,
    unsigned long (__stdcall* pStart)(void*), void* pParameter, unsigned long nFlags,
    unsigned long* pThreadId);
__declspec(dllimport) int __stdcall SetThreadPriority(HANDLE hThread, int nPriority);
__declspec(dllimport) unsigned long __stdcall ResumeThread(HANDLE hThread);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(HANDLE h, unsigned long nMs);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE h);
__declspec(dllimport) unsigned long __stdcall SuspendThread(HANDLE hThread);
__declspec(dllimport) __declspec(noreturn) void __stdcall ExitThread(unsigned long nExitCode);
}

#include <stdio.h>

class CExoCriticalSectionInternal;

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    CExoCriticalSection();                          // 0x005E9290
    ~CExoCriticalSection();                         // 0x005E9320
    void EnterCriticalSection();                    // 0x005E9300
    void LeaveCriticalSection();                    // 0x005E9310
};

unsigned long __stdcall ExoFileThreadProc(void* pParameter);   // 0x005EBE10

class CExoFileThread {
public:
    HANDLE m_hThread;                               // 0x00
    unsigned long m_nThreadId;                      // 0x04
    CExoCriticalSection m_cCriticalSection;         // 0x08
    int m_bDone;                                    // 0x0c
    int m_bExit;                                    // 0x10
    FILE* m_pFile;                                  // 0x14
    unsigned long m_nElementSize;                   // 0x18
    unsigned long m_nCount;                         // 0x1c
    unsigned long m_nWritten;                       // 0x20
    void* m_pBuffer;                                // 0x24

    CExoFileThread();
    ~CExoFileThread();
};

extern CExoFileThread g_cExoFileThread;             // 0x008328B0

// Write-behind worker: each wake-up writes the queued buffer, then suspends
// itself until the next request. A real start missing from functions.csv.
// FUNCTION: SWKOTOR 0x005EBE10
unsigned long __stdcall ExoFileThreadProc(void* pParameter)
{
    while (!g_cExoFileThread.m_bExit) {
        g_cExoFileThread.m_cCriticalSection.EnterCriticalSection();
        g_cExoFileThread.m_cCriticalSection.EnterCriticalSection();
        g_cExoFileThread.m_nWritten = 0;
        if (g_cExoFileThread.m_pFile) {
            g_cExoFileThread.m_nWritten = fwrite(g_cExoFileThread.m_pBuffer, g_cExoFileThread.m_nElementSize,
                                                 g_cExoFileThread.m_nCount, g_cExoFileThread.m_pFile)
                                          * g_cExoFileThread.m_nElementSize;
        }
        g_cExoFileThread.m_bDone = 1;
        g_cExoFileThread.m_pFile = 0;
        g_cExoFileThread.m_nCount = 0;
        g_cExoFileThread.m_nElementSize = 0;
        g_cExoFileThread.m_pBuffer = 0;
        g_cExoFileThread.m_cCriticalSection.LeaveCriticalSection();
        g_cExoFileThread.m_cCriticalSection.LeaveCriticalSection();
        SuspendThread(g_cExoFileThread.m_hThread);
    }
    ExitThread(1);
}

// FUNCTION: SWKOTOR 0x005EBED0
CExoFileThread::CExoFileThread()
{
    m_bDone = 0;
    m_bExit = 0;
    m_pFile = 0;
    m_nWritten = 0;
    m_nCount = 0;
    m_nElementSize = 0;
    m_pBuffer = 0;
    unsigned long nParameter = 0;
    m_hThread = CreateThread(0, 0x10000, ExoFileThreadProc, &nParameter, 4, &m_nThreadId);
    SetThreadPriority(m_hThread, -2);
}

// FUNCTION: SWKOTOR 0x005EB570
CExoFileThread::~CExoFileThread()
{
    m_bExit = 1;
    ResumeThread(m_hThread);
    if (m_hThread != (HANDLE)-1) {
        WaitForSingleObject(m_hThread, 0xffffffff);
        CloseHandle(m_hThread);
    }
}
