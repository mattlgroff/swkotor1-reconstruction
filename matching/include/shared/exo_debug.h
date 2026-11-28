// exo_debug.h -- CExoDebug
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_debug.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_DEBUG_H
#define SHARED_EXO_DEBUG_H

#include "shared/exo_debug_internal.h"
#include "shared/exo_string.h"

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

    // declared by other copies
    void WriteToScriptLogFile(CExoString& sText);
};

#endif
