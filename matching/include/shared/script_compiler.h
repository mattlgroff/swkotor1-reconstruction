// script_compiler.h -- CScriptCompiler
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/j/script_compiler.h); the other copies' members fall on its members.
#ifndef SHARED_SCRIPT_COMPILER_H
#define SHARED_SCRIPT_COMPILER_H


class CScriptCompilerInternal;

class CScriptCompiler {
public:
    ~CScriptCompiler();

    CScriptCompilerInternal* m_pInternal;           // 0x00
};

#endif
