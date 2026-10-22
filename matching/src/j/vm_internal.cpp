// CVirtualMachineStack, CVirtualMachineScript and the
// CVirtualMachineInternal stack push / pop helpers, 0x005D15A0-0x005D28E0.
#include <string.h>
#include "j/vm.h"
#include "j/res_helper.h"

// FUNCTION: SWKOTOR 0x005D15A0
void CVirtualMachineStack::AddToTopOfStack(int nType)
{
    if (m_nStackPointer == m_nTotalSize) {
        m_nTotalSize += 256;
        int* pNewNodes = new int[m_nTotalSize];
        char* pNewTypes = new char[m_nTotalSize];
        if (m_nStackPointer) {
            memcpy(pNewNodes, m_pStackNodes, m_nStackPointer * 4);
            memcpy(pNewTypes, m_pStackTypes, m_nStackPointer);
        }
        if (m_pStackTypes)
            delete[] m_pStackTypes;
        if (m_pStackNodes)
            delete[] m_pStackNodes;
        m_pStackNodes = pNewNodes;
        m_pStackTypes = pNewTypes;
    }
    if (m_nStackPointer < 0)
        return;
    m_pStackTypes[m_nStackPointer] = (char)nType;
    if (nType == 5) {
        m_pStackNodes[m_nStackPointer] = 0;
        m_nStackPointer++;
    }
    else if (nType >= 0x10 && nType <= 0x19) {
        if (m_pVMachine) {
            m_pStackNodes[m_nStackPointer] = 0;
            m_nStackPointer++;
        }
    }
    else if (nType == 3) {
        m_pStackNodes[m_nStackPointer] = 0;
        m_nStackPointer++;
    }
    else if (nType == 4) {
        float fZero = 0.0f;
        m_pStackNodes[m_nStackPointer] = *(int*)&fZero;
        m_nStackPointer++;
    }
    else if (nType == 6) {
        m_pStackNodes[m_nStackPointer] = 0x7f000000;
        m_nStackPointer++;
    }
}

// FUNCTION: SWKOTOR 0x005D16C0
void CVirtualMachineStack::ModifyIntegerAtLocation(int nLocation, int nValue)
{
    if (nLocation >= 0 && m_pVMachine && m_pStackTypes[nLocation] == 3)
        m_pStackNodes[nLocation] += nValue;
}

// FUNCTION: SWKOTOR 0x005D16F0
void CVirtualMachineInternal::InitializeScript(CVirtualMachineScript* pScript, char* pData, int nSize)
{
    pScript->m_nLoadedFromSave = 0;
    pScript->m_pStack = 0;
    pScript->m_nStackSize = 0;
    pScript->m_nInstructPtr = 13;
    pScript->m_nSecondaryInstructPtr = 0;
    pScript->m_nCodeSize = nSize;
    pScript->m_pCode = new char[nSize];
    memcpy(pScript->m_pCode, pData, nSize);
    pScript->m_n24 = 0;
}

// FUNCTION: SWKOTOR 0x005D1740
CVirtualMachineScript::CVirtualMachineScript()
{
    m_sScriptName = "";
    m_pStack = 0;
    m_nStackSize = 0;
    m_nInstructPtr = 0;
    m_nSecondaryInstructPtr = 0;
    m_pCode = 0;
    m_nCodeSize = 0;
    m_nLoadedFromSave = 0;
    m_n24 = 0;
}

// FUNCTION: SWKOTOR 0x005D17B0
int CVirtualMachineInternal::PopReturnValue(int* pValue)
{
    if (--m_nReturnStackDepth < 0) {
        m_nReturnStackDepth = 0;
        return 0;
    }
    *pValue = m_pReturnStack[m_nReturnStackDepth];
    return 1;
}

// FUNCTION: SWKOTOR 0x005D17F0
int CVirtualMachineInternal::PushReturnValue(int nValue)
{
    m_pReturnStack[m_nReturnStackDepth] = nValue;
    m_nReturnStackDepth++;
    return m_nReturnStackDepth < 0x80;
}

// FUNCTION: SWKOTOR 0x005D1820
int CVirtualMachineInternal::GetRunScriptReturnValue(int* nParameterType, void** pParameter)
{
    if (m_nReturnValueType != 3)
        return 0;
    *nParameterType = 3;
    *pParameter = (void*)m_nReturnValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1850
int CVirtualMachineInternal::StackPushInteger(int nValue)
{
    m_cRunTimeStack.AddToTopOfStack(3);
    m_cRunTimeStack.m_pStackNodes[m_cRunTimeStack.m_nStackPointer - 1] = nValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1880
int CVirtualMachineInternal::StackPushFloat(float fValue)
{
    m_cRunTimeStack.AddToTopOfStack(4);
    m_cRunTimeStack.m_pStackNodes[m_cRunTimeStack.m_nStackPointer - 1] = *(int*)&fValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D18B0
int CVirtualMachineInternal::StackPushVector(Vector vValue)
{
    StackPushFloat(vValue.x);
    StackPushFloat(vValue.y);
    StackPushFloat(vValue.z);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1930
int CVirtualMachineInternal::StackPushString(const CExoString& sValue)
{
    CExoString* pString = new CExoString(sValue.CStr());
    m_cRunTimeStack.AddToTopOfStack(5);
    m_cRunTimeStack.m_pStackNodes[m_cRunTimeStack.m_nStackPointer - 1] = (int)pString;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D19C0
int CVirtualMachineInternal::StackPushEngineStructure(int nType, void* pValue)
{
    if (!m_pCmdImplementer)
        return 0;
    void* pCopy = m_pCmdImplementer->CopyGameDefinedStructure(nType, pValue);
    m_cRunTimeStack.AddToTopOfStack(nType + 0x10);
    m_cRunTimeStack.m_pStackNodes[m_cRunTimeStack.m_nStackPointer - 1] = (int)pCopy;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1A10
int CVirtualMachineInternal::StackPushObject(unsigned long oidValue)
{
    m_cRunTimeStack.AddToTopOfStack(6);
    m_cRunTimeStack.m_pStackNodes[m_cRunTimeStack.m_nStackPointer - 1] = oidValue;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1A40
void CVirtualMachineInternal::SetCommandImplementer(CVirtualMachineCmdImplementer* pImplementer)
{
    m_pCmdImplementer = pImplementer;
    if (pImplementer)
        pImplementer->InitializeCommands();
}

// FUNCTION: SWKOTOR 0x005D2140
void CVirtualMachineStack::ClearStack()
{
    if (m_pStackTypes && m_pStackNodes) {
        for (int i = m_nStackPointer - 1; i >= 0; i--) {
            char nType = m_pStackTypes[i];
            if (nType == 5) {
                CExoString* pString = (CExoString*)m_pStackNodes[i];
                if (pString)
                    delete pString;
            }
            else if (nType >= 0x10 && nType <= 0x19) {
                m_pVMachine->m_pCmdImplementer->DestroyGameDefinedStructure(nType - 0x10, (void*)m_pStackNodes[i]);
            }
            m_pStackNodes[i] = 0;
        }
    }
    m_nBasePointer = 0;
    m_nStackPointer = 0;
    m_nTotalSize = 0;
    if (m_pStackTypes) {
        delete[] m_pStackTypes;
        m_pStackTypes = 0;
    }
    if (m_pStackNodes) {
        delete[] m_pStackNodes;
        m_pStackNodes = 0;
    }
}

// FUNCTION: SWKOTOR 0x005D28E0 SYMBOL: ??1CVirtualMachineStack@@QAE@XZ
CVirtualMachineStack::~CVirtualMachineStack()
{
    ClearStack();
}

// FUNCTION: SWKOTOR 0x005D21F0
void CVirtualMachineStack::SetStackPointer(int nStackPointer)
{
    if (nStackPointer < m_nStackPointer) {
        for (int i = m_nStackPointer - 1; i >= nStackPointer; i--) {
            char nType = m_pStackTypes[i];
            if (nType == 5) {
                CExoString* pString = (CExoString*)m_pStackNodes[i];
                if (pString)
                    delete pString;
            }
            else if (nType >= 0x10 && nType <= 0x19) {
                m_pVMachine->m_pCmdImplementer->DestroyGameDefinedStructure(nType - 0x10, (void*)m_pStackNodes[i]);
            }
        }
        m_nStackPointer = nStackPointer;
    }
}

// FUNCTION: SWKOTOR 0x005D24C0
int CVirtualMachineInternal::StackPopInteger(int* nValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != 3)
        return 0;
    *nValue = m_cRunTimeStack.m_pStackNodes[nSP];
    m_cRunTimeStack.SetStackPointer(nSP);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D2510
int CVirtualMachineInternal::StackPopFloat(float* fValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != 4)
        return 0;
    *fValue = *(float*)&m_cRunTimeStack.m_pStackNodes[nSP];
    m_cRunTimeStack.SetStackPointer(nSP);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D25E0
int CVirtualMachineInternal::StackPopString(CExoString* sValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != 5)
        return 0;
    *sValue = *(CExoString*)m_cRunTimeStack.m_pStackNodes[nSP];
    m_cRunTimeStack.SetStackPointer(nSP);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D2630
int CVirtualMachineInternal::StackPopEngineStructure(int nType, void** pValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != nType + 0x10)
        return 0;
    void* pStruct = (void*)m_cRunTimeStack.m_pStackNodes[nSP];
    if (!m_pCmdImplementer)
        return 0;
    *pValue = m_pCmdImplementer->CopyGameDefinedStructure(nType, pStruct);
    m_cRunTimeStack.SetStackPointer(nSP);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D26A0
int CVirtualMachineInternal::StackPopObject(unsigned long* oidValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != 6)
        return 0;
    *oidValue = m_cRunTimeStack.m_pStackNodes[nSP];
    m_cRunTimeStack.SetStackPointer(nSP);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D44F0
int CVirtualMachineInternal::DeleteScript(CVirtualMachineScript* pScript)
{
    if (pScript->m_pStack) {
        delete pScript->m_pStack;
        pScript->m_pStack = 0;
    }
    if (pScript->m_pCode) {
        delete[] pScript->m_pCode;
        pScript->m_pCode = 0;
    }
    pScript->m_nStackSize = 0;
    pScript->m_nInstructPtr = 13;
    pScript->m_nSecondaryInstructPtr = 0;
    pScript->m_nCodeSize = 0;
    pScript->m_n24 = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D4550 SYMBOL: ??1CVirtualMachineScript@@QAE@XZ
CVirtualMachineScript::~CVirtualMachineScript()
{
    if (m_pStack)
        delete m_pStack;
    m_pStack = 0;
    if (m_pCode) {
        delete[] m_pCode;
        m_pCode = 0;
    }
}

// FUNCTION: SWKOTOR 0x005D48E0
void CVirtualMachineInternal::DeleteScriptSituation(void* pScript)
{
    if (pScript)
        delete (CVirtualMachineScript*)pScript;
}

// FUNCTION: SWKOTOR 0x005D4900 SYMBOL: ??0CVirtualMachineInternal@@QAE@XZ
CVirtualMachineInternal::CVirtualMachineInternal()
{
    m_nRecursionLevel = -1;
    m_pCmdImplementer = 0;
    m_cRunTimeStack.ClearStack();
    m_cRunTimeStack.m_pVMachine = this;
}

// FUNCTION: SWKOTOR 0x005D49B0 SYMBOL: ??1CVirtualMachineInternal@@UAE@XZ
CVirtualMachineInternal::~CVirtualMachineInternal()
{
    for (int i = 0; i < 8; i++)
        DeleteScript(&m_pVirtualMachineScript[i]);
}

// FUNCTION: SWKOTOR 0x005D2260
int CVirtualMachineInternal::ReadScriptFile(CExoString* sScriptName)
{
    if (++m_nRecursionLevel >= 8) {
        m_nRecursionLevel--;
        return -0x5e;
    }
    if (!sScriptName) {
        CResRef cResRef("");
        SetResRef(cResRef, 0);
    }
    else {
        CResRef cResRef(*sScriptName);
        SetResRef(cResRef, 0);
    }
    if (!m_pRes->Demand()) {
        m_nRecursionLevel--;
        return -0x5f;
    }
    char* pData = m_pRes->GetNCSDataPtr();
    int nSize = m_pRes->GetNCSSize();
    m_pVirtualMachineScript[m_nRecursionLevel].m_sScriptName = *sScriptName;
    if (pData[0] == 'N' && pData[1] == 'C' && pData[2] == 'S' && pData[3] == ' '
        && pData[4] == 'V' && pData[6] == '.') {
        int nVersion = 0;
        if (pData[5] >= '1' && pData[5] <= '9')
            nVersion = (pData[5] - '0') * 10;
        if (pData[7] >= '1' && pData[7] <= '9')
            nVersion += pData[7] - '0';
        if (nVersion == 10 && pData[8] == 'B') {
            InitializeScript(&m_pVirtualMachineScript[m_nRecursionLevel], pData + 13, nSize - 13);
            m_pRes->Release();
            CResRef cResRef("");
            SetResRef(cResRef, 0);
            return 0;
        }
    }
    m_nRecursionLevel--;
    m_pRes->Release();
    return -0x60;
}

// FUNCTION: SWKOTOR 0x005D23E0
int CVirtualMachineInternal::SetUpScriptSituation(CVirtualMachineScript* pScript)
{
    int nLoadedFromSave = pScript->m_nLoadedFromSave;
    if (pScript->m_pCode == 0) {
        int nResult = ReadScriptFile(&pScript->m_sScriptName);
        if (nResult)
            return nResult;
        nLoadedFromSave = 0;
    }
    else {
        if (++m_nRecursionLevel >= 8) {
            m_nRecursionLevel--;
            return -0x5e;
        }
    }
    m_pVirtualMachineScript[m_nRecursionLevel].m_nLoadedFromSave = nLoadedFromSave;
    m_pVirtualMachineScript[m_nRecursionLevel].m_sScriptName = pScript->m_sScriptName;
    m_pVirtualMachineScript[m_nRecursionLevel].m_pStack = pScript->m_pStack;
    m_pVirtualMachineScript[m_nRecursionLevel].m_nStackSize = pScript->m_nStackSize;
    m_pVirtualMachineScript[m_nRecursionLevel].m_nInstructPtr = pScript->m_nInstructPtr;
    m_pVirtualMachineScript[m_nRecursionLevel].m_nSecondaryInstructPtr = pScript->m_nSecondaryInstructPtr;
    m_pVirtualMachineScript[m_nRecursionLevel].m_n24 = pScript->m_n24;
    if (pScript->m_pCode) {
        m_pVirtualMachineScript[m_nRecursionLevel].m_pCode = pScript->m_pCode;
        m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize = pScript->m_nCodeSize;
    }
    pScript->m_pStack = 0;
    pScript->m_pCode = 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D4270
int CVirtualMachineInternal::RunScriptFile(int nInstructionPointer)
{
    int nReturnStackDepth = m_nReturnStackDepth;
    int nStackPointer = m_cRunTimeStack.m_nStackPointer;
    m_nReturnValueType = 0;
    m_nReturnValue = 0;
    m_pReturnStack[m_nReturnStackDepth] = -1;
    m_nReturnStackDepth++;
    int nResult;
    {
        int nIP = nInstructionPointer;
        nResult = ExecuteCode(&nIP,
                              m_pVirtualMachineScript[m_nRecursionLevel].m_pCode,
                              m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize);
    }
    if (nResult < 0 || nReturnStackDepth != m_nReturnStackDepth) {
        CExoString sScriptName(m_pVirtualMachineScript[m_nRecursionLevel].m_sScriptName.CStr());
        m_pCmdImplementer->ReportError(&sScriptName, -nResult);
        if (m_cRunTimeStack.m_nStackPointer > nStackPointer)
            m_cRunTimeStack.SetStackPointer(nStackPointer);
        else
            m_cRunTimeStack.m_nStackPointer = nStackPointer;
        m_nReturnStackDepth = nReturnStackDepth;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005D45D0
int CVirtualMachineInternal::RunScript(CExoString* sScriptName, unsigned long oid, int bOidValid)
{
    if (sScriptName == 0 || sScriptName->IsEmpty() == 1)
        return 0;
    if (m_nRecursionLevel == -1) {
        m_cRunTimeStack.ClearStack();
        m_cRunTimeStack.m_pVMachine = this;
        m_n24 = 0;
        m_nReturnStackDepth = 0;
    }
    if (ReadScriptFile(sScriptName) == 0) {
        m_pVirtualMachineScript[m_nRecursionLevel].m_n24 = 0;
        m_bValidObjectRunScript[m_nRecursionLevel] = bOidValid;
        m_oidObjectRunScript[m_nRecursionLevel] = oid;
        if (m_pCmdImplementer) {
            m_pCmdImplementer->RunScriptStarted(sScriptName);
            m_pCmdImplementer->m_bValidObjectRunScript = m_bValidObjectRunScript[m_nRecursionLevel];
            m_pCmdImplementer->m_oidObjectRunScript = m_oidObjectRunScript[m_nRecursionLevel];
        }
        int nStackPointer = m_cRunTimeStack.m_nStackPointer;
        int nResult = RunScriptFile(0);
        DeleteScript(&m_pVirtualMachineScript[m_nRecursionLevel]);
        m_nRecursionLevel--;
        if (m_nRecursionLevel != -1 && m_pCmdImplementer) {
            m_pCmdImplementer->m_bValidObjectRunScript = m_bValidObjectRunScript[m_nRecursionLevel];
            m_pCmdImplementer->m_oidObjectRunScript = m_oidObjectRunScript[m_nRecursionLevel];
        }
        if (nResult == 0) {
            if (m_nRecursionLevel != -1)
                sScriptName->CStr();
            if (m_cRunTimeStack.m_nStackPointer == nStackPointer + 1) {
                if (m_cRunTimeStack.m_pStackTypes[nStackPointer] == 3) {
                    m_nReturnValueType = 3;
                    m_nReturnValue = m_cRunTimeStack.m_pStackNodes[nStackPointer];
                }
                m_cRunTimeStack.SetStackPointer(nStackPointer);
                if (m_nRecursionLevel == -1)
                    m_cRunTimeStack.ClearStack();
                return 1;
            }
            if (m_cRunTimeStack.m_nStackPointer == nStackPointer) {
                if (m_nRecursionLevel == -1)
                    m_cRunTimeStack.ClearStack();
                return 1;
            }
            m_cRunTimeStack.SetStackPointer(nStackPointer);
            if (m_nRecursionLevel == -1)
                m_cRunTimeStack.ClearStack();
            return 0;
        }
        else {
            if (m_nRecursionLevel != -1)
                sScriptName->CStr();
            return 0;
        }
    }
    if (m_nRecursionLevel != -1)
        sScriptName->CStr();
    return 0;
}

// FUNCTION: SWKOTOR 0x005D4AD0
int CVirtualMachineInternal::RunScriptSituation(void* pScriptSituation, unsigned long oid, int bOidValid)
{
    CVirtualMachineScript* pScript = (CVirtualMachineScript*)pScriptSituation;
    m_cRunTimeStack.ClearStack();
    m_cRunTimeStack.m_nStackPointer = pScript->m_pStack->m_nStackPointer;
    m_cRunTimeStack.m_nTotalSize = pScript->m_pStack->m_nTotalSize;
    m_cRunTimeStack.m_pVMachine = this;
    m_cRunTimeStack.CopyFromStack(pScript->m_pStack, 0, 0);
    if (pScript->m_pStack) {
        delete pScript->m_pStack;
        pScript->m_pStack = 0;
    }
    m_nReturnStackDepth = 0;
    m_n24 = 0;
    if (SetUpScriptSituation(pScript) == 0) {
        if (m_pVirtualMachineScript[m_nRecursionLevel].m_n24 == 0) {
            m_bValidObjectRunScript[m_nRecursionLevel] = bOidValid;
            m_oidObjectRunScript[m_nRecursionLevel] = oid;
            if (m_pCmdImplementer) {
                m_pCmdImplementer->m_bValidObjectRunScript = m_bValidObjectRunScript[m_nRecursionLevel];
                m_pCmdImplementer->m_oidObjectRunScript = m_oidObjectRunScript[m_nRecursionLevel];
            }
            int nResult = RunScriptFile(pScript->m_nInstructPtr);
            DeleteScript(&m_pVirtualMachineScript[m_nRecursionLevel]);
            m_nRecursionLevel--;
            delete pScript;
            return nResult == 0;
        }
        DeleteScript(&m_pVirtualMachineScript[m_nRecursionLevel]);
        m_nRecursionLevel--;
    }
    delete pScript;
    return 0;
}

// FUNCTION: SWKOTOR 0x005D1C40
void CVirtualMachineStack::AssignLocationToLocation(int nSrcLocation, int nDstLocation)
{
    if (nSrcLocation < 0 || nDstLocation < 0 || !m_pVMachine)
        return;
    if (nDstLocation == m_nStackPointer)
        AddToTopOfStack(m_pStackTypes[nSrcLocation]);
    char nType = m_pStackTypes[nSrcLocation];
    if (nType == 5) {
        CExoString* pOld = (CExoString*)m_pStackNodes[nDstLocation];
        if (pOld)
            delete pOld;
        CExoString* pSrc = (CExoString*)m_pStackNodes[nSrcLocation];
        CExoString* pNew = new CExoString();
        *pNew = *pSrc;
        m_pStackNodes[nDstLocation] = (int)pNew;
    }
    else if (nType >= 0x10 && nType <= 0x19) {
        void* pOld = (void*)m_pStackNodes[nDstLocation];
        if (pOld)
            m_pVMachine->m_pCmdImplementer->DestroyGameDefinedStructure(nType - 0x10, pOld);
        m_pStackNodes[nDstLocation] = (int)m_pVMachine->m_pCmdImplementer->CopyGameDefinedStructure(
            m_pStackTypes[nSrcLocation] - 0x10, (void*)m_pStackNodes[nSrcLocation]);
    }
    else {
        m_pStackNodes[nDstLocation] = m_pStackNodes[nSrcLocation];
    }
    m_pStackTypes[nDstLocation] = m_pStackTypes[nSrcLocation];
}

// FUNCTION: SWKOTOR 0x005D4380
int CVirtualMachineInternal::StackPopCommand(CVirtualMachineScript** ppScript)
{
    *ppScript = new CVirtualMachineScript();
    if (m_pVirtualMachineScript[m_nRecursionLevel].m_nLoadedFromSave == 0) {
        (*ppScript)->m_nCodeSize = 0;
        (*ppScript)->m_pCode = 0;
    }
    else {
        (*ppScript)->m_nCodeSize = m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize;
        (*ppScript)->m_pCode = new char[m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize];
        memcpy((*ppScript)->m_pCode, m_pVirtualMachineScript[m_nRecursionLevel].m_pCode,
               m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize);
    }
    (*ppScript)->m_n24 = m_pVirtualMachineScript[m_nRecursionLevel].m_n24;
    (*ppScript)->m_nLoadedFromSave = m_pVirtualMachineScript[m_nRecursionLevel].m_nLoadedFromSave;
    (*ppScript)->m_nSecondaryInstructPtr = 0;
    (*ppScript)->m_nInstructPtr = m_nCommandInstructPtr;
    (*ppScript)->m_nStackSize = m_cRunTimeStack.m_nStackPointer;
    (*ppScript)->m_pStack = new CVirtualMachineStack();
    (*ppScript)->m_pStack->m_pVMachine = this;
    (*ppScript)->m_pStack->CopyFromStack(&m_cRunTimeStack, m_nCommandStackSize, m_nCommandBasePointer);
    (*ppScript)->m_sScriptName = m_pVirtualMachineScript[m_nRecursionLevel].m_sScriptName;
    return 1;
}

// FUNCTION: SWKOTOR 0x005D28F0
void CVirtualMachineStack::CopyFromStack(CVirtualMachineStack* pStack, int nSizeOfStack, int nBasePointer)
{
    ClearStack();
    delete[] m_pStackTypes;
    delete[] m_pStackNodes;
    m_pStackTypes = 0;
    m_pStackNodes = 0;
    m_nTotalSize = 0;
    m_nBasePointer = 0;
    m_nStackPointer = 0;
    nSizeOfStack = nSizeOfStack >> 2;
    nBasePointer = nBasePointer >> 2;
    if (nSizeOfStack == 0 && nBasePointer == 0) {
        nSizeOfStack = pStack->m_nStackPointer - pStack->m_nBasePointer;
        nBasePointer = pStack->m_nBasePointer;
    }
    if (nBasePointer > 0x10000)
        nBasePointer = 0x10000;
    else if (nBasePointer < 0)
        nBasePointer = 0;
    if (pStack->m_nBasePointer < nBasePointer)
        nBasePointer = pStack->m_nBasePointer;
    if (nSizeOfStack > 0x10000)
        nSizeOfStack = 0x10000;
    else if (nSizeOfStack < 0)
        nSizeOfStack = 0;
    if (pStack->m_nBasePointer < nBasePointer)
        nBasePointer = pStack->m_nBasePointer;
    int nTotalSize = nSizeOfStack + nBasePointer;
    if (nTotalSize != 0) {
        nTotalSize += 0x10;
        m_pStackTypes = new char[nTotalSize];
        m_pStackNodes = new int[nTotalSize];
        m_nStackPointer = nSizeOfStack + nBasePointer;
        m_nTotalSize = nTotalSize;
        m_nBasePointer = nBasePointer;
        memcpy(m_pStackTypes + nBasePointer, pStack->m_pStackTypes + pStack->m_nStackPointer - nSizeOfStack, nSizeOfStack);
        memcpy(m_pStackTypes, pStack->m_pStackTypes + pStack->m_nBasePointer - nBasePointer, nBasePointer);
        for (int i = 0; i < nBasePointer; i++) {
            int nSrc = pStack->m_nBasePointer - nBasePointer + i;
            char nType = m_pStackTypes[i];
            if (nType == 5) {
                CExoString* pSrc = (CExoString*)pStack->m_pStackNodes[nSrc];
                CExoString* pNew = new CExoString();
                *pNew = *pSrc;
                m_pStackNodes[i] = (int)pNew;
            }
            else if (nType >= 0x10 && nType <= 0x19) {
                m_pStackNodes[i] = (int)m_pVMachine->m_pCmdImplementer->CopyGameDefinedStructure(
                    pStack->m_pStackTypes[nSrc] - 0x10, (void*)pStack->m_pStackNodes[nSrc]);
            }
            else {
                m_pStackNodes[i] = pStack->m_pStackNodes[nSrc];
            }
        }
        for (int j = 0; j < nSizeOfStack; j++) {
            int nSrc = pStack->m_nStackPointer - nSizeOfStack + j;
            char nType = m_pStackTypes[m_nBasePointer + j];
            if (nType == 5) {
                CExoString* pSrc = (CExoString*)pStack->m_pStackNodes[nSrc];
                CExoString* pNew = new CExoString();
                *pNew = *pSrc;
                m_pStackNodes[m_nBasePointer + j] = (int)pNew;
            }
            else if (nType >= 0x10 && nType <= 0x19) {
                m_pStackNodes[m_nBasePointer + j] = (int)m_pVMachine->m_pCmdImplementer->CopyGameDefinedStructure(
                    pStack->m_pStackTypes[nSrc] - 0x10, (void*)pStack->m_pStackNodes[nSrc]);
            }
            else {
                m_pStackNodes[m_nBasePointer + j] = pStack->m_pStackNodes[nSrc];
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005D1D80
int CVirtualMachineStack::SaveStack(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldINT(pStruct, m_nBasePointer, "BasePointer");
    pGFF->WriteFieldINT(pStruct, m_nStackPointer, "StackPointer");
    pGFF->WriteFieldINT(pStruct, m_nTotalSize, "TotalSize");
    if (m_nTotalSize > 0) {
        CResList cList;
        pGFF->AddList(&cList, pStruct, "Stack");
        for (int i = 0; i < m_nStackPointer; i++) {
            CResStruct cElement;
            pGFF->AddListElement(&cElement, &cList, i);
            pGFF->WriteFieldCHAR(&cElement, m_pStackTypes[i], "Type");
            switch (m_pStackTypes[i]) {
            case 3:
                pGFF->WriteFieldINT(&cElement, m_pStackNodes[i], "Value");
                break;
            case 4:
                pGFF->WriteFieldFLOAT(&cElement, *(float*)&m_pStackNodes[i], "Value");
                break;
            case 5:
                pGFF->WriteFieldCExoString(&cElement, *(CExoString*)m_pStackNodes[i], "Value");
                break;
            case 6:
                pGFF->WriteFieldDWORD(&cElement, m_pStackNodes[i], "Value");
                break;
            default:
                m_pVMachine->m_pCmdImplementer->SaveGameDefinedStructure(
                    m_pStackTypes[i] - 0x10, (void*)m_pStackNodes[i], pGFF, &cElement);
                break;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D47A0
int CVirtualMachineInternal::SaveScriptSituation(void* pScriptSituation, CResGFF* pGFF, CResStruct* pStruct)
{
    CVirtualMachineScript* pScript = (CVirtualMachineScript*)pScriptSituation;
    if (pScript->m_pCode && pScript->m_nCodeSize) {
        pGFF->WriteFieldINT(pStruct, pScript->m_nCodeSize, "CodeSize");
        pGFF->WriteFieldVOID(pStruct, pScript->m_pCode, pScript->m_nCodeSize, "Code");
    }
    else if (ReadScriptFile(&pScript->m_sScriptName) == 0) {
        pGFF->WriteFieldINT(pStruct, m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize, "CodeSize");
        pGFF->WriteFieldVOID(pStruct, m_pVirtualMachineScript[m_nRecursionLevel].m_pCode,
                             m_pVirtualMachineScript[m_nRecursionLevel].m_nCodeSize, "Code");
        DeleteScript(&m_pVirtualMachineScript[m_nRecursionLevel]);
        m_nRecursionLevel--;
    }
    else {
        pGFF->WriteFieldINT(pStruct, 0, "CodeSize");
    }
    pScript->m_n24 = 0;
    pGFF->WriteFieldDWORD(pStruct, pScript->m_n24, "LoadedFromSave");
    pGFF->WriteFieldINT(pStruct, pScript->m_nInstructPtr, "InstructionPtr");
    pGFF->WriteFieldINT(pStruct, pScript->m_nSecondaryInstructPtr, "SecondaryPtr");
    pGFF->WriteFieldCExoString(pStruct, pScript->m_sScriptName, "Name");
    pGFF->WriteFieldINT(pStruct, pScript->m_nStackSize, "StackSize");
    CResStruct cStack;
    pGFF->AddStructToStruct(&cStack, pStruct, "Stack", 0);
    pScript->m_pStack->SaveStack(pGFF, &cStack);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D1EF0
int CVirtualMachineStack::LoadStack(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    m_nBasePointer = pGFF->ReadFieldINT(pStruct, "BasePointer", bSuccess, 0);
    m_nStackPointer = pGFF->ReadFieldINT(pStruct, "StackPointer", bSuccess, 0);
    m_nTotalSize = pGFF->ReadFieldINT(pStruct, "TotalSize", bSuccess, 0);
    if (m_nTotalSize <= 0)
        return 1;
    m_pStackTypes = new char[m_nTotalSize];
    m_pStackNodes = new int[m_nTotalSize];
    CResList cList;
    pGFF->GetList(&cList, pStruct, "Stack");
    for (int i = 0; i < m_nStackPointer; i++) {
        CResStruct cElement;
        pGFF->GetListElement(&cElement, &cList, i);
        m_pStackTypes[i] = pGFF->ReadFieldCHAR(&cElement, "Type", bSuccess, 0);
        switch (m_pStackTypes[i]) {
        case 3:
            m_pStackNodes[i] = pGFF->ReadFieldINT(&cElement, "Value", bSuccess, 0);
            break;
        case 4: {
            float fValue = pGFF->ReadFieldFLOAT(&cElement, "Value", bSuccess, 0.0f);
            m_pStackNodes[i] = *(int*)&fValue;
            break;
        }
        case 5: {
            CExoString* pString = new CExoString();
            {
                CExoString sDefault("");
                *pString = pGFF->ReadFieldCExoString(&cElement, "Value", bSuccess, sDefault);
            }
            m_pStackNodes[i] = (int)pString;
            break;
        }
        case 6:
            m_pStackNodes[i] = pGFF->ReadFieldDWORD(&cElement, "Value", bSuccess, 0);
            break;
        default:
            m_pVMachine->m_pCmdImplementer->LoadGameDefinedStructure(
                m_pStackTypes[i] - 0x10, (void**)&m_pStackNodes[i], pGFF, &cElement);
            break;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005D26F0
int CVirtualMachineInternal::LoadScriptSituation(void** ppScript, CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    CVirtualMachineScript** ppSituation = (CVirtualMachineScript**)ppScript;
    *ppSituation = new CVirtualMachineScript();
    (*ppSituation)->m_nCodeSize = pGFF->ReadFieldINT(pStruct, "CodeSize", bSuccess, 0);
    if ((*ppSituation)->m_nCodeSize)
        (*ppSituation)->m_pCode = new char[(*ppSituation)->m_nCodeSize];
    else
        (*ppSituation)->m_pCode = 0;
    pGFF->ReadFieldVOID(pStruct, (*ppSituation)->m_pCode, (*ppSituation)->m_nCodeSize, "Code", bSuccess, 0);
    (*ppSituation)->m_n24 = pGFF->ReadFieldDWORD(pStruct, "LoadedFromSave", bSuccess, 0);
    if (!bSuccess)
        (*ppSituation)->m_n24 = 0;
    (*ppSituation)->m_nLoadedFromSave = 1;
    (*ppSituation)->m_nInstructPtr = pGFF->ReadFieldINT(pStruct, "InstructionPtr", bSuccess, 0);
    (*ppSituation)->m_nSecondaryInstructPtr = pGFF->ReadFieldINT(pStruct, "SecondaryPtr", bSuccess, 0);
    {
        CExoString sDefault("");
        (*ppSituation)->m_sScriptName = pGFF->ReadFieldCExoString(pStruct, "Name", bSuccess, sDefault);
    }
    (*ppSituation)->m_nStackSize = pGFF->ReadFieldINT(pStruct, "StackSize", bSuccess, 0);
    CResStruct cStack;
    pGFF->GetStructFromStruct(&cStack, pStruct, "Stack");
    (*ppSituation)->m_pStack = new CVirtualMachineStack();
    (*ppSituation)->m_pStack->m_pVMachine = this;
    (*ppSituation)->m_pStack->LoadStack(pGFF, &cStack);
    return 1;
}

// FUNCTION: SWKOTOR 0x005D2560
int CVirtualMachineInternal::StackPopVector(Vector* vValue)
{
    int nSP = m_cRunTimeStack.m_nStackPointer;
    if (nSP <= 0)
        return 0;
    nSP--;
    if (m_cRunTimeStack.m_pStackTypes[nSP] != 4)
        return 0;
    float fValue = *(float*)&m_cRunTimeStack.m_pStackNodes[nSP];
    m_cRunTimeStack.SetStackPointer(nSP);
    vValue->z = fValue;
    int nResult = StackPopFloat(&fValue);
    if (nResult == 1) {
        vValue->y = fValue;
        nResult = StackPopFloat(&fValue);
        if (nResult == 1)
            vValue->x = fValue;
    }
    return nResult;
}
