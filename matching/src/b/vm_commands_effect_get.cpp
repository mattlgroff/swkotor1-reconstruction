// CSWVirtualMachineCommands: GetEffect* queries (0x00537AA0-0x00537CD7).
#include "b/vm_commands.h"
#include "b/vm_effect.h"

#define INVALID_OBJECT_ID 0x7f000000

// FUNCTION: SWKOTOR 0x00537AA0
int CSWVirtualMachineCommands::ExecuteCommandGetEffectCreator(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    unsigned long oidCreator = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pEffect) {
        oidCreator = pEffect->m_oidCreator;
    }
    if (!g_pVirtualMachine->StackPushObject(oidCreator)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00537B20
int CSWVirtualMachineCommands::ExecuteCommandGetEffectDurationType(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(pEffect->GetDurationType())) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00537B90
int CSWVirtualMachineCommands::ExecuteCommandGetEffectSpellId(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nSpellId = pEffect->m_nSpellId;
    if (!g_pVirtualMachine->StackPushInteger(nSpellId)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00537C00
int CSWVirtualMachineCommands::ExecuteCommandGetEffectSubType(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(pEffect->m_nSubType & 0x18)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00537C70
int CSWVirtualMachineCommands::ExecuteCommandGetEffectType(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(pEffect->GetScriptEffectType())) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}
