// CExoDebug / CExoDebugInternal forwarders (b005e0_exo_foundation_leaves).

#include "c/exo_base.h"

class CExoFile {
public:
    int Flush();                                    // 0x005E6A20
    int Write(const CExoString& s);                 // 0x005E6A70
};

class CExoDebugInternal {
public:
    int m_bFilesOpen;                               // 0x00
    CExoFile* m_pLogFile;                           // 0x04
    CExoFile* m_pScriptLogFile;                     // 0x08
    char pad_0xc[0x10];

    CExoDebugInternal();                            // 0x005EC290
    ~CExoDebugInternal();                           // 0x005EC680
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005EC2B0
    void GetCurrentTimestamp(CExoString& s);        // 0x005EC560
    void WriteToScriptLogFile(const CExoString& s);
    void FlushScriptLogFile();
    void CloseLogFiles();                           // 0x005EC5C0
    void OpenLogFiles(const CExoString& s);                // 0x0060E760
};

class CExoDebug {
public:
    CExoString m_sUnknown0;                         // 0x00
    CExoDebugInternal* m_pcExoDebugInternal;        // 0x08
    int m_nUnknownC;                                // 0x0c

    CExoDebug();
    ~CExoDebug();

    void FlushScriptLogFile();
    void OpenLogFiles(const CExoString& s);
    void Warning(int nLine, char* sFile, char* sMessage);
    void GetCurrentTimestamp(CExoString& s);
    void WriteToScriptLogFile(const CExoString& s);
};

// FUNCTION: SWKOTOR 0x005E93E0
void CExoDebug::FlushScriptLogFile()
{
    if (m_pcExoDebugInternal) {
        m_pcExoDebugInternal->FlushScriptLogFile();
    }
}

// FUNCTION: SWKOTOR 0x005E93F0
void CExoDebug::OpenLogFiles(const CExoString& s)
{
    if (m_pcExoDebugInternal) {
        m_pcExoDebugInternal->OpenLogFiles(s);
    }
}

// FUNCTION: SWKOTOR 0x005E9400
void CExoDebug::Warning(int nLine, char* sFile, char* sMessage)
{
    if (m_pcExoDebugInternal) {
        m_pcExoDebugInternal->Warning(nLine, sFile, sMessage);
    }
}

// FUNCTION: SWKOTOR 0x005E9410
void CExoDebug::GetCurrentTimestamp(CExoString& s)
{
    if (m_pcExoDebugInternal) {
        m_pcExoDebugInternal->GetCurrentTimestamp(s);
    }
}

// FUNCTION: SWKOTOR 0x005E9420
void CExoDebug::WriteToScriptLogFile(const CExoString& s)
{
    if (m_pcExoDebugInternal) {
        m_pcExoDebugInternal->WriteToScriptLogFile(s);
    }
}

// FUNCTION: SWKOTOR 0x005E9370
CExoDebug::CExoDebug()
{
    m_pcExoDebugInternal = new CExoDebugInternal();
    m_nUnknownC = 0;
}

// FUNCTION: SWKOTOR 0x005E9430
CExoDebug::~CExoDebug()
{
    if (m_pcExoDebugInternal) {
        delete m_pcExoDebugInternal;
        m_pcExoDebugInternal = 0;
    }
}
