// CExoDebugInternal leaves (b005e0_exo_foundation_leaves).

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

    ~CExoDebugInternal();
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005EC2B0
    void GetCurrentTimestamp(CExoString& s);        // 0x005EC560
    void WriteToScriptLogFile(const CExoString& s);
    void FlushScriptLogFile();
    void CloseLogFiles();                           // 0x005EC5C0
    void OpenLogFiles(const CExoString& s);                // 0x0060E760
};

// FUNCTION: SWKOTOR 0x005EC5A0
void CExoDebugInternal::WriteToScriptLogFile(const CExoString& s)
{
    if (m_bFilesOpen) {
        m_pScriptLogFile->Write(s);
    }
}

// FUNCTION: SWKOTOR 0x005EC5B0
void CExoDebugInternal::FlushScriptLogFile()
{
    if (m_bFilesOpen) {
        m_pScriptLogFile->Flush();
    }
}

// FUNCTION: SWKOTOR 0x005EC680
CExoDebugInternal::~CExoDebugInternal()
{
    CloseLogFiles();
}
