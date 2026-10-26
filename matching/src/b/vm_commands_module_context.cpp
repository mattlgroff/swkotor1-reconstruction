// VM module context: item activation and activation target location.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/global_variable_table.h"

struct ModuleActivationView {
    char pad_0x0[0x178];
    unsigned long m_oidActivatedItem;
    unsigned long m_oidActivator;
    char pad_0x180[4];
    unsigned long m_oidTarget;
    Vector m_vTargetPosition;
};

// NONMATCH (22): register choices and branch layout.
// FUNCTION: SWKOTOR 0x0053F220
int CSWVirtualMachineCommands::ExecuteCommandGetItemActivated(int nCommandId, int nParameters)
{
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    ModuleActivationView* pActivation = (ModuleActivationView*)pModule;
    if (nCommandId == 0x1b9) {
        CScriptLocation cLocation;
        if (pActivation) {
            CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(pActivation->m_oidActivator);
            Vector& vPosition = *(Vector*)cLocation.m_vPosition;
            Vector& vDirection = *(Vector*)cLocation.m_vOrientation;
            vPosition = pActivation->m_vTargetPosition;
            if (pCreature && !(pCreature->m_vPosition == vPosition)) {
                Vector vDifference(vPosition.x - pCreature->m_vPosition.x,
                                   vPosition.y - pCreature->m_vPosition.y,
                                   vPosition.z - pCreature->m_vPosition.z);
                vDirection = Normalize(vDifference);
            }
        }
        if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, &cLocation)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else {
        unsigned long oidResult = 0x7f000000;
        if (pActivation) {
            if (nCommandId == 0x1b7) oidResult = pActivation->m_oidActivatedItem;
            else if (nCommandId == 0x1b8) oidResult = pActivation->m_oidActivator;
            else if (nCommandId == 0x1ba) oidResult = pActivation->m_oidTarget;
        }
        if (!g_pVirtualMachine->StackPushObject(oidResult)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    return 0;
}
