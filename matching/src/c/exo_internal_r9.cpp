// Exo pimpl leftovers (b005e4/b005e9, r9): ~CExoIniInternal,
// CExoFileInternal ctor/dtor, ~CExoAliasListInternal, CExoTimersInternal
// ctor, CExoDebugInternal ctor / GetCurrentTimestamp / CloseLogFiles.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

union LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; };
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* p);   // IAT 0x0073D1AC

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* CStr() const;                             // 0x005E5670
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoLinkedListInternal {
public:
    void* pHead;                                    // 0x00
    void* pTail;                                    // 0x04
    unsigned long nCount;                           // 0x08

    ~CExoLinkedListInternal();                      // 0x005E9AB0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    ~CExoLinkedList()
    {
        if (m_pcExoLinkedListInternal) {
            delete m_pcExoLinkedListInternal;
        }
    }
};

class CFileSection;

class CExoIniInternal {
public:
    CExoLinkedList<CFileSection> m_lstSections;     // 0x00
    CExoString m_sFileName;                         // 0x04

    ~CExoIniInternal();
    void Destroy();                                 // 0x005EA650
};

// FUNCTION: SWKOTOR 0x005EB500
CExoIniInternal::~CExoIniInternal()
{
    Destroy();
}

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sFileName, unsigned short nResType);   // 0x005E68A0
};

class CExoBase {
public:
    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    void* m_pcExoDebug;                             // 0x08
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CExoFileInternal {
public:
    FILE* m_pFile;                                  // 0x00
    CExoString m_sFileName;                         // 0x04
    CExoString m_sMode;                             // 0x0c

    CExoFileInternal(const CExoString& sFileName, unsigned short nResType, const CExoString& sMode);
    ~CExoFileInternal();
};

// FUNCTION: SWKOTOR 0x005EB840
CExoFileInternal::CExoFileInternal(const CExoString& sFileName, unsigned short nResType, const CExoString& sMode)
{
    m_pFile = 0;
    m_sMode = sMode;
    m_sFileName = g_pExoBase->m_pcExoAliasList->ResolveFileName(sFileName, nResType);
    m_pFile = fopen(m_sFileName.CStr(), sMode.CStr());
}

// FUNCTION: SWKOTOR 0x005EB900
CExoFileInternal::~CExoFileInternal()
{
    if (m_pFile) {
        fclose(m_pFile);
        m_pFile = 0;
    }
}

class CExoAlias {
public:
    CExoString m_sAlias;                            // 0x00
    CExoString m_sPath;                             // 0x08
    CExoAlias* m_pNext;                             // 0x10
};

class CExoAliasListInternal {
public:
    CExoString m_sUnknown0;                         // 0x00
    CExoAlias* m_pAliases;                          // 0x08

    ~CExoAliasListInternal();
};

// FUNCTION: SWKOTOR 0x005EC0E0
CExoAliasListInternal::~CExoAliasListInternal()
{
    CExoAlias* pAlias = m_pAliases;
    while (pAlias) {
        CExoAlias* pNext = pAlias->m_pNext;
        delete pAlias;
        pAlias = pNext;
    }
}

class CExoTimersInternal {
public:
    int m_bHighResolution;                          // 0x00
    int m_bLowFrequency;                            // 0x04
    LARGE_INTEGER m_liFrequency;                    // 0x08
    __int64 m_nTicksPerMicrosecond;                 // 0x10
    double m_fMicrosecondsPerTick;                  // 0x18

    CExoTimersInternal();
};

// FUNCTION: SWKOTOR 0x005EC160
CExoTimersInternal::CExoTimersInternal()
{
    LARGE_INTEGER liFrequency;
    m_bHighResolution = QueryPerformanceFrequency(&liFrequency);
    if (m_bHighResolution == 1) {
        m_liFrequency = liFrequency;
        m_bLowFrequency = 0;
        if ((unsigned __int64)liFrequency.QuadPart / 1000000 < 200) {
            m_bLowFrequency = 1;
            m_fMicrosecondsPerTick = 1.0 / ((double)liFrequency.QuadPart * 0.000001);
        } else {
            m_fMicrosecondsPerTick = 1.0 / ((double)liFrequency.QuadPart * 0.000001);
            m_nTicksPerMicrosecond = liFrequency.QuadPart / 1000000;
        }
    }
}

class CExoFile {
public:
    ~CExoFile();                                    // 0x005E69E0
};

class CExoDebugInternal {
public:
    int m_bFilesOpen;                               // 0x00
    CExoFile* m_pLogFile;                           // 0x04
    CExoFile* m_pScriptLogFile;                     // 0x08
    CExoFile* m_pErrorLogFile;                      // 0x0c
    CExoFile* m_pWarningLogFile;                    // 0x10
    CExoFile* m_pFile14;                            // 0x14
    CExoFile* m_pFile18;                            // 0x18

    CExoDebugInternal();
    void GetCurrentTimestamp(CExoString& s);
    void CloseLogFiles();
};

// FUNCTION: SWKOTOR 0x005EC290
CExoDebugInternal::CExoDebugInternal()
{
    m_pScriptLogFile = 0;
    m_pLogFile = 0;
    m_pErrorLogFile = 0;
    m_pWarningLogFile = 0;
    m_bFilesOpen = 0;
}

// FUNCTION: SWKOTOR 0x005EC560
void CExoDebugInternal::GetCurrentTimestamp(CExoString& s)
{
    time_t t;
    char buf[20];
    time(&t);
    sprintf(buf, "%.19s", ctime(&t));
    s.Format(buf);
}

// FUNCTION: SWKOTOR 0x005EC5C0
void CExoDebugInternal::CloseLogFiles()
{
    if (m_bFilesOpen) {
        if (m_pLogFile) {
            delete m_pLogFile;
            m_pLogFile = 0;
        }
        if (m_pErrorLogFile) {
            delete m_pErrorLogFile;
            m_pErrorLogFile = 0;
        }
        if (m_pWarningLogFile) {
            delete m_pWarningLogFile;
            m_pWarningLogFile = 0;
        }
        if (m_pFile14) {
            delete m_pFile14;
            m_pFile14 = 0;
        }
        if (m_pFile18) {
            delete m_pFile18;
            m_pFile18 = 0;
        }
        if (m_pScriptLogFile) {
            delete m_pScriptLogFile;
            m_pScriptLogFile = 0;
        }
        m_bFilesOpen = 0;
    }
}
