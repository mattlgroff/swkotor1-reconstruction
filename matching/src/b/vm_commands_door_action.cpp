// CSWVirtualMachineCommands::ExecuteCommandDoDoorAction (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_door.h"
#include "b/server_exo_app.h"

// Creature +0x340 points at the AI data whose +0xbc holds the door to ignore.
struct CSWSCreatureDoorAIData {
    char pad_0x0[0xbc];
    unsigned long m_oidIgnoreDoor;                          // 0xbc (name guessed)
};

struct CSWSCreatureDoorAIView {
    char pad_0x0[0x340];
    CSWSCreatureDoorAIData* m_pAIData;                      // 0x340 (name guessed)
};

// Door actions: 0 open, 1 unlock, 2 bash, 3 ignore, 4 knock (spell 0x5d).
// FUNCTION: SWKOTOR 0x00530360
int CSWVirtualMachineCommands::ExecuteCommandDoDoorAction(int nCommandId, int nParameters)
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    unsigned long oidDoor;
    int nDoorAction;
    if (!g_pVirtualMachine->StackPopObject(&oidDoor)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nDoorAction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidDoor);
    if (pObject && pObject->AsSWSObject() && pCreature) {
        CSWSObject* pDoor = pObject->AsSWSObject();
        switch (nDoorAction) {
        case 0:
            if (nCommandId == 0x152) {
                pCreature->AddActionToFront(0x14, 0xffff, 3, &oidDoor);
            } else {
                pCreature->AddActionToFront(0x28, 0xffff, 3, &oidDoor);
            }
            return 0;
        case 1:
            if (nCommandId == 0x152) {
                pCreature->AddActionToFront(0x14, 0xffff, 3, &oidDoor);
            } else {
                pCreature->AddActionToFront(0x28, 0xffff, 3, &oidDoor);
            }
            pCreature->AddActionToFront(0x26, 0xfffe, 3, &oidDoor);
            return 0;
        case 2:
            pCreature->AddAttackActions(oidDoor, 0, 0, 0, 0, 1, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
            return 0;
        case 3:
            if (nCommandId == 0x152) {
                ((CSWSCreatureDoorAIView*)pCreature)->m_pAIData->m_oidIgnoreDoor = oidDoor;
            }
            return 0;
        case 4:
            if (!((CSWSDoor*)pDoor)->m_bPlot) {
                for (unsigned char nClass = 0; nClass < pCreature->m_pStats->m_nNumMultiClasses; nClass++) {
                    if (pCreature->m_pStats->GetSpellUsesLeft(0x5d, nClass, 0, 0) > 0) {
                        pCreature->AddCastSpellActions(0x5d, nClass, 0, 0, 0, pDoor->m_vPosition, oidDoor, 0, 0, 1,
                                                       0, 0, 0, -1, 0xff, 0);
                        break;
                    }
                }
                return 0;
            }
            break;
        }
    }
    return 0;
}
