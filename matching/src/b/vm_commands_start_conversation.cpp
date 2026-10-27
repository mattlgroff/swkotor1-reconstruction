// CSWVirtualMachineCommands::ExecuteCommandActionStartConversation (round s8).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"

class CSWGuiInGame {
public:
    void AddToDialogNonDependantList(const CExoString& sName); // 0x00632300
    void SetGlobalDialogState(int nState);                  // 0x0062EC60

    char pad_0x0[0xb4];
    int m_nB4;                                              // 0xb4 (name guessed)
    char pad_0xb8[0xbd8 - 0xb8];
    int m_bUseLeader;                                       // 0xbd8 (name guessed)
};

class CSWParty {
public:
    CSWCCreature* GetCharacter(int nIndex);                 // 0x006346C0
};

class CServerAIMaster {
public:
    int SetAILevel(CSWSObject* pObject, int nLevel);        // 0x004B08A0
};

// Every pop failure returns -2000 (the stack-overflow code), as in the original.
// FUNCTION: SWKOTOR 0x0052D5B0
int CSWVirtualMachineCommands::ExecuteCommandActionStartConversation(int nCommandId, int nParameters)
{
    CExoString sDialogResRef("");
    int bPrivateConversation = 0;
    int bAlways = 1;
    int nConversationType = 1;
    int bIgnoreStartRange = 0;
    CSWSObject* pObject;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    unsigned long oidObjectToConverse;
    if (!g_pVirtualMachine->StackPopObject(&oidObjectToConverse)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopString(&sDialogResRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&bPrivateConversation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopInteger(&nConversationType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nParameters >= 5 && !g_pVirtualMachine->StackPopInteger(&bIgnoreStartRange)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    bIgnoreStartRange = 1;
    CExoString sNameToIgnore;
    if (nParameters > 5) {
        for (int i = 0; i < 6; i++) {
            if (!g_pVirtualMachine->StackPopString(&sNameToIgnore)) {
                return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
            }
            g_pAppManager->GetClientExoApp()->GetInGameGui()->AddToDialogNonDependantList(sNameToIgnore);
        }
    }
    int bUseLeader = 0;
    if (nParameters >= 12 && !g_pVirtualMachine->StackPopInteger(&bUseLeader)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (bUseLeader == 0) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bUseLeader = 0;
    } else {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bUseLeader = 1;
    }
    CSWCCreature* pCharacter = g_pAppManager->GetClientExoApp()->GetSWParty()->GetCharacter(0);
    if (pCharacter) {
        pCharacter->ClearAllActions();
        unsigned long oidCharacter = pCharacter->m_idSelf;
        oidCharacter = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidCharacter);
        CGameObject* pServerObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidCharacter);
        if (pServerObject && pServerObject->AsSWSCreature()) {
            pServerObject->AsSWSCreature()->ClearAllActions(1);
        }
    }
    if (!g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nB4 && m_bValidObjectRunScript == 1) {
        if (pObjectArray->GetGameObject(m_oidObjectRunScript, (CGameObject**)&pObject) == GAME_OBJECT_VALID &&
            pObject->m_bCommandable) {
            if (!pObject->m_nAILevel) {
                g_pAppManager->GetServerExoApp()->GetServerAIMaster()->SetAILevel(pObject, 1);
            }
            g_pAppManager->GetClientExoApp()->GetInGameGui()->SetGlobalDialogState(1);
            unsigned long oidNone = INVALID_OBJECT_ID;
            pObject->AddAction(0x18, 0xffff, 3, &oidObjectToConverse, 4, &sDialogResRef, 1, &bPrivateConversation,
                               1, &bAlways, 1, &bIgnoreStartRange, 3, &oidNone);
        }
    }
    return 0;
}
