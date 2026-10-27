// CSWVirtualMachineCommands::ExecuteCommandBeginConversation (round s10).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/server_exo_app.h"
#include "b/vector.h"

class CSWGuiInGame {
public:
    void SetGlobalDialogState(int nState);                  // 0x0062EC60

    char pad_0x0[0xb4];
    int m_nB4;                                              // 0xb4 (name guessed)
};

// CSWSObject dialog fields (CSWSObject stays data-free in sws_creature.h).
struct CSWSObjectDialogView {
    char pad_0x0[0x17c];
    unsigned long m_oidLastSpeaker;                         // 0x17c (name guessed)
    int m_bPrivateConversation;                             // 0x180 (name guessed)
    CExoString m_sDialogResRef;                             // 0x184 (name guessed)
};

// NONMATCH (87): structure matches; cl rotates edi/esi/ebx between the
// target copy, the speaker copy and the player (GetId() trims the rest).
// FUNCTION: SWKOTOR 0x0052E9F0
int CSWVirtualMachineCommands::ExecuteCommandBeginConversation(int nCommandId, int nParameters)
{
    CGameObject* pSelf;
    CGameObject* pTarget;
    unsigned long oidTarget = INVALID_OBJECT_ID;
    int bResult = 0;
    CExoString sDialogResRef;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    Vector vSelfOrientation(0.0f, 0.0f, 0.0f);
    Vector vTargetOrientation(0.0f, 0.0f, 0.0f);
    if (nParameters > 0) {
        if (!g_pVirtualMachine->StackPopString(&sDialogResRef) ||
            !g_pVirtualMachine->StackPopObject(&oidTarget)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    int nGuiState = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nB4;
    unsigned long oidConversation = g_pAppManager->GetServerExoApp()->GetActionConversation();
    g_pAppManager->GetServerExoApp()->SetActionConversation(0);
    if ((oidConversation || !nGuiState) && m_bValidObjectRunScript == 1 &&
        pObjectArray->GetGameObject(m_oidObjectRunScript, &pSelf) == GAME_OBJECT_VALID) {
        if (oidTarget == INVALID_OBJECT_ID) {
            oidTarget = ((CSWSObjectDialogView*)pSelf)->m_oidLastSpeaker;
        }
        CSWSObject* pTargetObject;
        CSWSObject* pSelfObject;
        if (pObjectArray->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID &&
            (pTargetObject = (CSWSObject*)pTarget) != NULL &&
            (pSelfObject = (CSWSObject*)pSelf) != NULL) {
            unsigned long oidPlayer = g_pAppManager->GetServerExoApp()->GetPlayerCreatureId();
            CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
            if (pTarget->AsSWSCreature() && pPlayer) {
                if (pTarget->GetId() != pPlayer->m_oid38) {
                    pTarget->AsSWSCreature()->ClearAllActions(1);
                }
                vTargetOrientation = pTargetObject->AsSWSCreature()->m_vOrientation;
                CLastUpdateObject* pLastUpdate = pPlayer->GetLastUpdateObject(pTargetObject->GetId());
                if (pLastUpdate) {
                    pTarget->AsSWSCreature()->SetDialogDelay7C(10000);
                    pTarget->AsSWSCreature()->ClearUpdateFlag(0);
                    pLastUpdate->m_nDialogDelay = 10000;
                }
            }
            if (pSelf->AsSWSCreature() && pPlayer) {
                if (pSelf->GetId() != pPlayer->m_oid38) {
                    pSelf->AsSWSCreature()->ClearAllActions(1);
                }
                vSelfOrientation = pSelfObject->AsSWSCreature()->m_vOrientation;
                CLastUpdateObject* pLastUpdate = pPlayer->GetLastUpdateObject(pSelfObject->GetId());
                if (pLastUpdate) {
                    pSelf->AsSWSCreature()->SetDialogDelay7C(10000);
                    pSelf->AsSWSCreature()->ClearUpdateFlag(0);
                    pLastUpdate->m_nDialogDelay = 10000;
                }
            }
            if (sDialogResRef.IsEmpty()) {
                sDialogResRef = ((CSWSObjectDialogView*)pSelfObject)->m_sDialogResRef;
            }
            int bPrivate = ((CSWSObjectDialogView*)pSelfObject)->m_bPrivateConversation;
            g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(pTargetObject->m_idSelf);
            unsigned long oidPlayerCreature = pPlayer->m_oid38;
            int bStarted;
            if (oidTarget == oidPlayerCreature) {
                bStarted = pSelfObject->StartDialog(oidTarget, sDialogResRef, bPrivate);
            } else if (m_oidObjectRunScript == oidPlayerCreature) {
                bStarted = pTargetObject->StartDialog(m_oidObjectRunScript, sDialogResRef, bPrivate);
            } else if (pSelfObject->AsSWSCreature() && pSelfObject->AsSWSCreature()->m_bInParty) {
                if (pTargetObject->StartDialog(m_oidObjectRunScript, sDialogResRef, bPrivate)) {
                    bResult = 1;
                    goto push_result;
                }
                if (pTargetObject->AsSWSCreature()) {
                    pTargetObject->AsSWSCreature()->SetLockOrientationToObject(INVALID_OBJECT_ID, 0);
                    pTargetObject->AsSWSCreature()->SetOrientation(vTargetOrientation);
                }
                goto restore_self;
            } else {
                bStarted = pSelfObject->StartDialog(oidTarget, sDialogResRef, bPrivate);
            }
            if (bStarted) {
                bResult = 1;
                goto push_result;
            }
            if (pTargetObject->AsSWSCreature()) {
                pTargetObject->AsSWSCreature()->SetLockOrientationToObject(INVALID_OBJECT_ID, 0);
                pTargetObject->AsSWSCreature()->SetOrientation(vTargetOrientation);
            }
restore_self:
            if (pSelfObject->AsSWSCreature()) {
                pSelfObject->AsSWSCreature()->SetLockOrientationToObject(INVALID_OBJECT_ID, 0);
                pSelfObject->AsSWSCreature()->SetOrientation(vSelfOrientation);
            }
        }
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->SetGlobalDialogState(0);
push_result:
    if (!g_pVirtualMachine->StackPushInteger(bResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
