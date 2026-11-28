// exo_debug_internal.h -- CExoDebugInternal
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_debug.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_DEBUG_INTERNAL_H
#define SHARED_EXO_DEBUG_INTERNAL_H

#include "shared/exo_string.h"

class CExoFile;

class CExoDebugInternal {
public:
    int m_bFilesOpen;                               // 0x00
    CExoFile* m_pLogFile;                           // 0x04
    CExoFile* m_pScriptLogFile;                     // 0x08
    CExoFile * m_pErrorLogFile;                  // 0xc
    CExoFile * m_pWarningLogFile;                // 0x10
    CExoFile * m_pFile14;                        // 0x14
    CExoFile * m_pFile18;                        // 0x18

    CExoDebugInternal();                            // 0x005EC290
    ~CExoDebugInternal();                           // 0x005EC680
    void Warning(int nLine, char* sFile, char* sMessage);   // 0x005EC2B0
    void GetCurrentTimestamp(CExoString& s);        // 0x005EC560
    void WriteToScriptLogFile(const CExoString& s);
    void FlushScriptLogFile();
    void CloseLogFiles();                           // 0x005EC5C0
    void OpenLogFiles(const CExoString& s);                // 0x0060E760

    // declared by other copies
    void Warning(int nLine, char* sFile, const char* sMessage);
};

#endif
