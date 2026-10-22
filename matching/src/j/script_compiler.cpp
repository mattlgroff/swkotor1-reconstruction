// CScriptCompiler destructors and CScriptCompilerInternal::ShutDown
// (0x005D1120-0x005D14D0).
#include "j/script_compiler.h"

// FUNCTION: SWKOTOR 0x005D1120
CScriptCompiler::~CScriptCompiler()
{
    if (m_pInternal) {
        delete m_pInternal;
    }
}

// FUNCTION: SWKOTOR 0x005D1140
CScriptCompilerVarStackEntry::~CScriptCompilerVarStackEntry()
{
}

// FUNCTION: SWKOTOR 0x005D1190
CScriptCompilerStructureFieldEntry::~CScriptCompilerStructureFieldEntry()
{
}

// FUNCTION: SWKOTOR 0x005D1260
CScriptCompilerIdListEntry::~CScriptCompilerIdListEntry()
{
    if (m_pnParameterTypes) {
        delete[] m_pnParameterTypes;
    }
    if (m_psParameterStructureNames) {
        delete[] m_psParameterStructureNames;
    }
    if (m_pbOptionalParameters) {
        delete[] m_pbOptionalParameters;
    }
    if (m_pnOptionalParameterIntegerData) {
        delete[] m_pnOptionalParameterIntegerData;
    }
    if (m_pfOptionalParameterFloatData) {
        delete[] m_pfOptionalParameterFloatData;
    }
    if (m_psOptionalParameterStringData) {
        delete[] m_psOptionalParameterStringData;
    }
    if (m_pOptionalParameterObjectData) {
        delete[] (char*)m_pOptionalParameterObjectData;
    }
    if (m_pfOptionalParameterVectorData) {
        delete[] m_pfOptionalParameterVectorData;
    }
}

// FUNCTION: SWKOTOR 0x005D1370
CScriptCompilerIncludeFileStackEntry::~CScriptCompilerIncludeFileStackEntry()
{
}

// FUNCTION: SWKOTOR 0x005D13C0
void CScriptCompilerInternal::ShutDown()
{
    if (m_p228) {
        delete[] m_p228;
    }
    if (m_pIdentifierList) {
        delete[] m_pIdentifierList;
    }
    if (m_pcVarStackList) {
        delete[] m_pcVarStackList;
    }
    if (m_pcStructList) {
        delete[] m_pcStructList;
        m_pcStructList = 0;
    }
    if (m_pcStructFieldList) {
        delete[] m_pcStructFieldList;
        m_pcStructFieldList = 0;
    }
    if (m_p610) {
        delete[] m_p610;
        m_p610 = 0;
    }
    if (m_p60c) {
        delete[] m_p60c;
        m_p60c = 0;
    }
}

// FUNCTION: SWKOTOR 0x005D14D0
CScriptCompilerInternal::~CScriptCompilerInternal()
{
    ShutDown();
}
