// CSWVirtualMachineCommands: spell target location.
#include "b/vm_commands.h"
#include "b/global_variable_table.h"
#include "b/sws_creature.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x0053D400
int CSWVirtualMachineCommands::ExecuteCommandGetSpellTargetLocation(int nCommandId, int nParameters)
{
    CScriptLocation cLocation;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1) {
        if (pObjectArray->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
            CSWSObject* pSWSObject = pObject->AsSWSObject();
            if (pSWSObject && (pSWSObject->AsSWSCreature() || pSWSObject->AsSWSPlaceable())) {
                memcpy(&cLocation.m_vPosition[0], (char*)pSWSObject + 0x1b0, 12);
                memcpy(&cLocation.m_vOrientation[0], (char*)pSWSObject + 0x9c, 12);
            }
        }
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(2, &cLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
