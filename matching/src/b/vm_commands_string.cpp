// CSWVirtualMachineCommands: NWScript string commands.
#include "b/vm_commands.h"
#include "b/vm_effect.h"

// FUNCTION: SWKOTOR 0x00535C90
int CSWVirtualMachineCommands::ExecuteCommandFindSubString(int nCommandId, int nParameters)
{
    CExoString sString;
    CExoString sSubString;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sSubString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(sString.Find(sSubString, 0))) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053D720
int CSWVirtualMachineCommands::ExecuteCommandGetStringLeft(int nCommandId, int nParameters)
{
    CExoString sString;
    int nCount;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nCount)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult = sString.Left(nCount);
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053D860
int CSWVirtualMachineCommands::ExecuteCommandGetStringLength(int nCommandId, int nParameters)
{
    CExoString sString;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(sString.GetLength())) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053D920
int CSWVirtualMachineCommands::ExecuteCommandGetStringLowerCase(int nCommandId, int nParameters)
{
    CExoString sString;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult = sString.LowerCase();
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053DA40
int CSWVirtualMachineCommands::ExecuteCommandGetStringRight(int nCommandId, int nParameters)
{
    CExoString sString;
    int nCount;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nCount)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult = sString.Right(nCount);
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053DB80
int CSWVirtualMachineCommands::ExecuteCommandGetStringUpperCase(int nCommandId, int nParameters)
{
    CExoString sString;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult = sString.UpperCase();
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053DDA0
int CSWVirtualMachineCommands::ExecuteCommandGetSubString(int nCommandId, int nParameters)
{
    CExoString sString;
    int nStart;
    int nCount;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nStart)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nCount)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult = sString.SubString(nStart, nCount);
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053E960
int CSWVirtualMachineCommands::ExecuteCommandInsertString(int nCommandId, int nParameters)
{
    CExoString sString;
    CExoString sDestination;
    int nPosition;
    if (!g_pVirtualMachine->StackPopString(&sDestination)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nPosition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sDestination.Insert(sString, nPosition);
    if (!g_pVirtualMachine->StackPushString(sDestination)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053EAF0
int CSWVirtualMachineCommands::ExecuteCommandIntToHexString(int nCommandId, int nParameters)
{
    int nInteger;
    CExoString sResult;
    if (!g_pVirtualMachine->StackPopInteger(&nInteger)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sResult.Format("0x%08x", nInteger);
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

class CSWSExpression {
public:
    CSWSExpression();                                       // 0x0059D990
    ~CSWSExpression();                                      // 0x0059D9E0
    void SetExpression(CExoString sExpression);             // 0x0059DAB0 (name guessed)
    int TestExpression(CExoString& sText, int bFlag);       // 0x0059E170 (name guessed)

    char pad_0x0[0x20];
};

// FUNCTION: SWKOTOR 0x00544EF0
int CSWVirtualMachineCommands::ExecuteCommandTestStringAgainstPattern(int nCommandId, int nParameters)
{
    CExoString sPattern;
    CExoString sString;
    if (!g_pVirtualMachine->StackPopString(&sPattern) ||
        !g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSExpression cExpression;
    cExpression.SetExpression(sPattern);
    if (!g_pVirtualMachine->StackPushInteger(cExpression.TestExpression(sString, 1))) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
