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
}

class CExoCriticalSectionInternal;

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    CExoCriticalSection();                          // 0x005E9290
    ~CExoCriticalSection();                         // 0x005E9320
};

unsigned long __stdcall ExoFileThreadProc(void* pParameter);   // 0x005EBE10

class CExoFileThread {
public:
    HANDLE m_hThread;                               // 0x00
    unsigned long m_nThreadId;                      // 0x04
    CExoCriticalSection m_cCriticalSection;         // 0x08
    unsigned long m_n0c;                            // 0x0c
    int m_bExit;                                    // 0x10
    void* m_pFile;                                  // 0x14
    unsigned long m_nOffset;                        // 0x18
    unsigned long m_nSize;                          // 0x1c
    unsigned long m_n20;                            // 0x20
    void* m_pBuffer;                                // 0x24

    CExoFileThread();
    ~CExoFileThread();
};

// FUNCTION: SWKOTOR 0x005EBED0
CExoFileThread::CExoFileThread()
{
    m_n0c = 0;
    m_bExit = 0;
    m_pFile = 0;
    m_n20 = 0;
    m_nSize = 0;
    m_nOffset = 0;
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
