// CSWVirtualMachineCommands::ExecuteCommandActionMoveToObject (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/res_gff.h"

// ActionMoveToObject (22) and ActionForceMoveToObject (0x17f) share this handler.
// FUNCTION: SWKOTOR 0x0053FB00
int CSWVirtualMachineCommands::ExecuteCommandActionMoveToObject(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    int bRun = 0;
    float fUseRange = 0.5f;
    float fRange = 1.0f;
    float fTimeout = 0.0f;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bRun)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopFloat(&fRange)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId == 0x17f) {
        if (nParameters >= 4) {
            if (!g_pVirtualMachine->StackPopFloat(&fTimeout)) {
                return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
            }
        } else {
            fTimeout = 30.0f;
        }
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        CGameObject* pTarget;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID &&
            pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_bCommandable) {
            Vector vUsePoint(0.0f, 0.0f, 0.0f);
            pObject->AsSWSCreature()->GetUseRange(oidTarget, vUsePoint, fUseRange, 0);
            if (!(fRange > fUseRange)) {
                fRange = fUseRange;
            }
            Vector vPosition = pTarget->m_vPosition;
            CSWSArea* pArea = ((CSWSObject*)pTarget)->GetArea();
            CResRef cResRef;
            if (pArea) {
                unsigned long oidArea = pArea->m_idSelf;
                nCommandId = 1;
                Vector vFacing(0.0f, 0.0f, 0.0f);
                pObject->AsSWSCreature()->AddMoveToPointAction(0xffff, vPosition, oidArea, oidTarget, bRun,
                    fRange, fTimeout, 0, 0, 0, 0, 0, 0, vFacing, 0, 0, 0);
                ((CSWSObject*)pObject)->AddAction(0x11, 0xfffe, 3, &oidTarget, 1, &bRun, 2, &fRange, 2, &fRange, 1, &nCommandId);
                if (pObject->AsSWSPlaceable() && pObject->AsSWSPlaceable()->m_bPartyInteract) {
                    ((CSWSObject*)pObject)->AddAction(0x13, 0xfffe, 3, &oidTarget);
                }
            }
        }
    }
    return 0;
}
