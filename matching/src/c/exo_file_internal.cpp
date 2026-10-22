// CExoFileInternal leaves (b005e3_exo_file_wrappers).

#include <stdio.h>
#include "c/exo_base.h"

typedef void* HANDLE;
extern "C" __declspec(dllimport) unsigned long __stdcall ResumeThread(HANDLE hThread);

class CExoCriticalSectionInternal;

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    void EnterCriticalSection();                    // 0x005E9300
    void LeaveCriticalSection();                    // 0x005E9310
};

class CExoFileThread {
public:
    HANDLE m_hThread;                               // 0x00
    unsigned long m_nThreadId;                      // 0x04
    CExoCriticalSection m_cCriticalSection;         // 0x08
    unsigned long m_n0c;                            // 0x0c
    int m_bExit;                                    // 0x10
    FILE* m_pFile;                                  // 0x14
    unsigned long m_nOffset;                        // 0x18
    unsigned long m_nSize;                          // 0x1c
    unsigned long m_n20;                            // 0x20
    void* m_pBuffer;                                // 0x24
};

extern CExoFileThread g_cExoFileThread;             // 0x008328B0

class CExoFileInternal {
public:
    FILE* m_pFile;                                  // 0x00
    char pad_0x4[0x10];

    long GetOffset();
    long GetSize();
    unsigned long Read(CExoString& sOut, unsigned long nCount);
    void ReadAsync(void* pBuffer, unsigned long nOffset, unsigned long nSize);
};

// FUNCTION: SWKOTOR 0x005EB970
long CExoFileInternal::GetOffset()
{
    if (m_pFile) {
        return ftell(m_pFile);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EB990
long CExoFileInternal::GetSize()
{
    if (m_pFile) {
        fflush(m_pFile);
        long nPosition = ftell(m_pFile);
        if (nPosition >= 0) {
            fseek(m_pFile, 0, SEEK_END);
            long nSize = ftell(m_pFile);
            fseek(m_pFile, nPosition, SEEK_SET);
            return nSize;
        }
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x005EB9F0
unsigned long CExoFileInternal::Read(CExoString& sOut, unsigned long nCount)
{
    if (m_pFile) {
        char* pBuffer = new char[nCount + 1];
        unsigned long nRead = fread(pBuffer, 1, nCount, m_pFile);
        pBuffer[nRead] = 0;
        sOut = pBuffer;
        delete[] pBuffer;
        return nRead;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EBBF0
void CExoFileInternal::ReadAsync(void* pBuffer, unsigned long nOffset, unsigned long nSize)
{
    FILE* pFile = m_pFile;
    g_cExoFileThread.m_cCriticalSection.EnterCriticalSection();
    g_cExoFileThread.m_pBuffer = pBuffer;
    g_cExoFileThread.m_n0c = 0;
    g_cExoFileThread.m_pFile = pFile;
    g_cExoFileThread.m_nOffset = nOffset;
    g_cExoFileThread.m_nSize = nSize;
    ResumeThread(g_cExoFileThread.m_hThread);
    g_cExoFileThread.m_cCriticalSection.LeaveCriticalSection();
}
