// CExoFileInternal retrying Read/Write (s5).

#include <stdio.h>
#include "c/exo_base.h"

extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long nMilliseconds);   // IAT 0x0073D1E4

// Descriptive name: the file-retry log message buffer at 0x008328D8.
extern CExoString g_sExoFileRetryLog;

class CExoFileInternal {
public:
    FILE* m_pFile;                                  // 0x00
    CExoString m_sFileName;                         // 0x04
    CExoString m_sMode;                             // 0x0c

    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount);
    unsigned long Write(const void* pBuffer, unsigned long nSize, unsigned long nCount);
};

// NONMATCH (16 diff insns, Read and Write alike): after the bottom loop
// test our build threads the known "transfer complete" edge straight into
// the recovered message (and the first equality test past the bRetried
// check); the original re-tests both. Early-return and break/goto forms
// are worse (pushes hoisted above the null test, loop-head padding).
// FUNCTION: SWKOTOR 0x005EBA40
unsigned long CExoFileInternal::Read(void* pBuffer, unsigned long nSize, unsigned long nCount)
{
    int nRetries = 0;
    if (m_pFile) {
        int bRetried = 0;
        long nPosition = ftell(m_pFile);
        unsigned long nRead = fread(pBuffer, nSize, nCount, m_pFile) * nSize;
        unsigned long nRequested = nSize * nCount;
        while (nRead != nRequested && ferror(m_pFile) && nRetries < 10) {
            bRetried = 1;
            g_sExoFileRetryLog.Format("%sFAILED READ (%s) - ATTEMPT %d - Asked for %d bytes got %d bytes\n",
                                      m_sFileName.CStr(), g_sExoFileRetryLog.CStr(), nRetries, nRequested, nRead);
            Sleep(500);
            if (m_pFile) {
                fclose(m_pFile);
            } else {
                g_sExoFileRetryLog.Format("%sFile handle was null\n", g_sExoFileRetryLog.CStr(), nRetries);
            }
            m_pFile = fopen(m_sFileName.CStr(), m_sMode.CStr());
            if (m_pFile) {
                fseek(m_pFile, nPosition, SEEK_SET);
                nRead = fread(pBuffer, nSize, nCount, m_pFile) * nSize;
            } else {
                g_sExoFileRetryLog.Format("%sFile handle failed to reopen\n", g_sExoFileRetryLog.CStr(), nRetries);
            }
            nRetries++;
        }
        if (nRead != nRequested) {
            g_sExoFileRetryLog.Format("%sFile write failed.  Giving up\n", g_sExoFileRetryLog.CStr());
        } else if (bRetried) {
            g_sExoFileRetryLog.Format("%sFile write recovered.\n", g_sExoFileRetryLog.CStr());
        }
        return nRead;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EBC60
unsigned long CExoFileInternal::Write(const void* pBuffer, unsigned long nSize, unsigned long nCount)
{
    int nRetries = 0;
    if (m_pFile) {
        int bRetried = 0;
        long nPosition = ftell(m_pFile);
        unsigned long nWritten = fwrite(pBuffer, nSize, nCount, m_pFile) * nSize;
        unsigned long nRequested = nSize * nCount;
        while (nWritten != nRequested && ferror(m_pFile) && nRetries < 10) {
            bRetried = 1;
            g_sExoFileRetryLog.Format("%sFAILED WRITE (%s) - ATTEMPT %d - Tried to write %d bytes at position %d, actually wrote %d bytes\n",
                                      m_sFileName.CStr(), g_sExoFileRetryLog.CStr(), nRetries, nRequested,
                                      nPosition, nWritten);
            Sleep(500);
            if (m_pFile) {
                fclose(m_pFile);
            } else {
                g_sExoFileRetryLog.Format("%sFile handle was null\n", g_sExoFileRetryLog.CStr());
            }
            m_pFile = fopen(m_sFileName.CStr(), m_sMode.CStr());
            if (m_pFile) {
                fseek(m_pFile, nPosition, SEEK_SET);
                nWritten = fwrite(pBuffer, nSize, nCount, m_pFile) * nSize;
            } else {
                g_sExoFileRetryLog.Format("%sFile handle failed to reopen\n", g_sExoFileRetryLog.CStr());
            }
            nRetries++;
        }
        if (nWritten != nRequested) {
            g_sExoFileRetryLog.Format("%sFile write failed.  Giving up\n", g_sExoFileRetryLog.CStr());
        } else if (bRetried) {
            g_sExoFileRetryLog.Format("%sFile write recovered.\n", g_sExoFileRetryLog.CStr());
        }
        return nWritten;
    }
    return 0;
}
