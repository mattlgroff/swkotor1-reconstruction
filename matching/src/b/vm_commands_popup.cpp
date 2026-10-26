// PopUpGUIPanel and PopUpDeathGUIPanel share this VM handler.
#include "b/vm_commands.h"
#include "b/sws_message.h"

// FUNCTION: SWKOTOR 0x00541320
int CSWVirtualMachineCommands::ExecuteCommandPopUpGUIPanel(int nCommandId, int nParameters)
{
    unsigned long oidPlayer = 0x7f000000;
    int nPanel = 0;
    int bRespawn = 1;
    int bWait = 1;
    int nStrref = 0;
    CExoString sText("");
    if (!g_pVirtualMachine->StackPopObject(&oidPlayer)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (nCommandId == 0x184) {
        if (!g_pVirtualMachine->StackPopInteger(&nPanel)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    } else if (nCommandId == 0x22a) {
        if (!g_pVirtualMachine->StackPopInteger(&bRespawn)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        if (!g_pVirtualMachine->StackPopInteger(&bWait)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        if (!g_pVirtualMachine->StackPopInteger(&nStrref)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        if (!g_pVirtualMachine->StackPopString(&sText)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (oidPlayer != 0x7f000000) {
        g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerPopUpGUIPanel(
            oidPlayer, nPanel, bRespawn != 0, bWait != 0, nStrref, sText);
    }
    return 0;
}
