// the CVirtualMachine facade (+0x00 internal), 0x005D0F40-0x005D1110.
// CVirtualMachineInternal is defined in vm_internal.cpp so nothing gets
// inlined here.
#include "j/vm.h"

// FUNCTION: SWKOTOR 0x005D0F40 SYMBOL: ??0CVirtualMachine@@QAE@XZ
CVirtualMachine::CVirtualMachine()
{
    m_pInternal = new CVirtualMachineInternal();
}

// FUNCTION: SWKOTOR 0x005D0FB0 SYMBOL: ??1CVirtualMachine@@QAE@XZ
CVirtualMachine::~CVirtualMachine()
{
    if (m_pInternal)
        delete m_pInternal;
}

// FUNCTION: SWKOTOR 0x005D0FC0
int CVirtualMachine::RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid)
{
    return m_pInternal->RunScript(sScriptName, oid, bOidValid);
}

// FUNCTION: SWKOTOR 0x005D0FD0
int CVirtualMachine::RunScriptSituation(void* pScript, unsigned long oid, int bOidValid)
{
    return m_pInternal->RunScriptSituation(pScript, oid, bOidValid);
}

// FUNCTION: SWKOTOR 0x005D0FE0
int CVirtualMachine::GetRunScriptReturnValue(int* nParameterType, void** pParameter)
{
    return m_pInternal->GetRunScriptReturnValue(nParameterType, pParameter);
}

// FUNCTION: SWKOTOR 0x005D0FF0
void CVirtualMachine::SetCommandImplementer(CVirtualMachineCmdImplementer* pImplementer)
{
    m_pInternal->SetCommandImplementer(pImplementer);
}

// FUNCTION: SWKOTOR 0x005D1000
int CVirtualMachine::StackPopInteger(int* nValue)
{
    return m_pInternal->StackPopInteger(nValue);
}

// FUNCTION: SWKOTOR 0x005D1010
int CVirtualMachine::StackPushInteger(int nValue)
{
    return m_pInternal->StackPushInteger(nValue);
}

// FUNCTION: SWKOTOR 0x005D1020
int CVirtualMachine::StackPopFloat(float* fValue)
{
    return m_pInternal->StackPopFloat(fValue);
}

// FUNCTION: SWKOTOR 0x005D1030
int CVirtualMachine::StackPushFloat(float fValue)
{
    return m_pInternal->StackPushFloat(fValue);
}

// FUNCTION: SWKOTOR 0x005D1040
int CVirtualMachine::StackPopVector(Vector* vValue)
{
    return m_pInternal->StackPopVector(vValue);
}

// FUNCTION: SWKOTOR 0x005D1050
int CVirtualMachine::StackPushVector(Vector vValue)
{
    return m_pInternal->StackPushVector(vValue);
}

// FUNCTION: SWKOTOR 0x005D1080
int CVirtualMachine::StackPopString(CExoString* sValue)
{
    return m_pInternal->StackPopString(sValue);
}

// FUNCTION: SWKOTOR 0x005D1090
int CVirtualMachine::StackPushString(const CExoString& sValue)
{
    return m_pInternal->StackPushString(sValue);
}

// FUNCTION: SWKOTOR 0x005D10A0
int CVirtualMachine::StackPopEngineStructure(int nType, void** pValue)
{
    return m_pInternal->StackPopEngineStructure(nType, pValue);
}

// FUNCTION: SWKOTOR 0x005D10B0
int CVirtualMachine::StackPushEngineStructure(int nType, void* pValue)
{
    return m_pInternal->StackPushEngineStructure(nType, pValue);
}

// FUNCTION: SWKOTOR 0x005D10C0
int CVirtualMachine::StackPopObject(unsigned long* oidValue)
{
    return m_pInternal->StackPopObject(oidValue);
}

// FUNCTION: SWKOTOR 0x005D10D0
int CVirtualMachine::StackPushObject(unsigned long oidValue)
{
    return m_pInternal->StackPushObject(oidValue);
}

// FUNCTION: SWKOTOR 0x005D10E0
int CVirtualMachine::StackPopCommand(CVirtualMachineScript** pScript)
{
    return m_pInternal->StackPopCommand(pScript);
}

// FUNCTION: SWKOTOR 0x005D10F0
int CVirtualMachine::SaveScriptSituation(void* pScript, CResGFF* pGFF, CResStruct* pStruct)
{
    return m_pInternal->SaveScriptSituation(pScript, pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x005D1100
int CVirtualMachine::LoadScriptSituation(void** pScript, CResGFF* pGFF, CResStruct* pStruct)
{
    return m_pInternal->LoadScriptSituation(pScript, pGFF, pStruct);
}

// FUNCTION: SWKOTOR 0x005D1110
void CVirtualMachine::DeleteScriptSituation(void* pScript)
{
    m_pInternal->DeleteScriptSituation(pScript);
}
