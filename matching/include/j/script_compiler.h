// Worker J matching headers: CScriptCompiler / CScriptCompilerInternal and
// the entry classes whose destructors live at 0x005D1120-0x005D14D0.
#ifndef J_SCRIPT_COMPILER_H
#define J_SCRIPT_COMPILER_H

#include "j/res.h"

class CScriptCompilerVarStackEntry {
public:
    ~CScriptCompilerVarStackEntry();

    CExoString m_sVarName;                          // 0x00
    char pad_0x8[0xc];
    CExoString m_sVarStructureName;                 // 0x14
};

class CScriptCompilerStructureFieldEntry {
public:
    ~CScriptCompilerStructureFieldEntry();

    int m_n0;                                       // 0x00
    CExoString m_psVarName;                         // 0x04
    CExoString m_psStructureName;                   // 0x0c
    int m_n14;                                      // 0x14
};

class CScriptCompilerStructureEntry {
public:
    ~CScriptCompilerStructureEntry();               // 0x0059EA30

    char pad_0x0[0x14];
};

class CScriptCompilerIdListEntry {
public:
    ~CScriptCompilerIdListEntry();

    CExoString m_psIdentifier;                      // 0x00
    char pad_0x8[0x10];
    CExoString m_psValue;                           // 0x18
    CExoString m_psStructureName;                   // 0x20
    char pad_0x28[0x24];
    int* m_pnParameterTypes;                        // 0x4c
    CExoString* m_psParameterStructureNames;        // 0x50
    int* m_pbOptionalParameters;                    // 0x54
    int* m_pnOptionalParameterIntegerData;          // 0x58
    float* m_pfOptionalParameterFloatData;          // 0x5c
    CExoString* m_psOptionalParameterStringData;    // 0x60
    void* m_pOptionalParameterObjectData;           // 0x64
    float* m_pfOptionalParameterVectorData;         // 0x68
    char pad_0x6c[0x10];
};

class CScriptCompilerIncludeFileStackEntry {
public:
    ~CScriptCompilerIncludeFileStackEntry();

    CExoString m_sCompiledScriptName;               // 0x00
    CResHelper<CResNSS, 2009> m_cSourceScript;      // 0x08
    char pad_0x24[0x10];
};

class CScriptCompilerInternal {
public:
    ~CScriptCompilerInternal();
    void ShutDown();

    char pad_0x0[0x8];
    CExoString m_s8;                                // 0x08
    CExoString m_s10;                               // 0x10
    char pad_0x18[0x210];
    char* m_p228;                                   // 0x228
    char pad_0x22c[0x20];
    CScriptCompilerIdListEntry* m_pIdentifierList;  // 0x24c
    char pad_0x250[0x10];
    CScriptCompilerIncludeFileStackEntry m_pcIncludeFileStack[16]; // 0x260
    char pad_0x5a0[0x4];
    CScriptCompilerVarStackEntry* m_pcVarStackList; // 0x5a4
    char pad_0x5a8[0x8];
    CExoString m_s5b0;                              // 0x5b0
    CScriptCompilerStructureEntry* m_pcStructList;  // 0x5b8
    CScriptCompilerStructureFieldEntry* m_pcStructFieldList; // 0x5bc
    char pad_0x5c0[0x30];
    CExoString m_s5f0;                              // 0x5f0
    char pad_0x5f8[0x14];
    int* m_p60c;                                    // 0x60c
    CExoString* m_p610;                             // 0x610
    char pad_0x614[0xc];
    CExoString m_s620;                              // 0x620
    char pad_0x628[0x4];
    CExoString m_s62c;                              // 0x62c
    char pad_0x634[0x274];
    CExoString m_as8a8[32];                         // 0x8a8
};

class CScriptCompiler {
public:
    ~CScriptCompiler();

    CScriptCompilerInternal* m_pInternal;           // 0x00
};

#endif
