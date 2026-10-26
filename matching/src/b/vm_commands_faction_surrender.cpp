// VM faction surrender: move matching creatures in the current area.
#define B_OUT_OF_LINE_FACTION_MANAGER
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/faction_manager.h"

// NONMATCH (10): saved-register push/pop timing.
// FUNCTION: SWKOTOR 0x00546340
int CSWVirtualMachineCommands::ExecuteCommandSurrenderByFaction(int nCommandId, int nParameters)
{
    int nFrom;
    int nTo;
    if (!g_pVirtualMachine->StackPopInteger(&nFrom)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (!g_pVirtualMachine->StackPopInteger(&nTo)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    CFactionManager* pManager = g_pAppManager->GetServerExoApp()->GetFactionManager();
    CGameObjectArray* pObjects = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (!m_bValidObjectRunScript) return 0;
    CGameObject* pObject;
    if (pObjects->GetGameObject(m_oidObjectRunScript, &pObject) != GAME_OBJECT_VALID) return 0;
    CSWSArea* pArea;
    if (pObject->AsSWSObject()) pArea = pObject->AsSWSObject()->GetArea();
    else if (pObject->AsSWSArea()) pArea = pObject->AsSWSArea();
    else return 0;
    if (pArea && pManager) {
        CFaction* pFaction = pManager->GetFaction(nTo);
        unsigned long oidObject;
        if (pArea->GetFirstObjectIdInArea(&oidObject) == 1) {
            do {
                CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidObject);
                if (pCreature) {
                    pCreature->m_sTag.CStr();
                    if (pCreature->m_pStats->m_nFactionId == nFrom) {
                        pFaction->AddMember(oidObject, 0);
                        if (nCommandId == 0x2e0) {
                            pCreature->SetCombatState(0, 1);
                            pCreature->ClearAllActions(1);
                            pCreature->PacifyCreature();
                        } else {
                            pCreature->ClearVisibleList();
                            pCreature->UpdateVisibleList(0);
                        }
                    }
                }
            } while (pArea->GetNextObjectIdInArea(&oidObject) == 1);
        }
    }
    return 0;
}
