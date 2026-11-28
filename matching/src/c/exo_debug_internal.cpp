// CExoDebugInternal leaves (b005e0_exo_foundation_leaves).

#include "c/exo_base.h"

class CExoFile {
public:
    int Flush();                                    // 0x005E6A20
    int Write(const CExoString& s);                 // 0x005E6A70
};

#include "shared/exo_debug_internal.h"

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
