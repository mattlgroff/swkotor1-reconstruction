// CSWVirtualMachineCommands: the GetLast* event-query commands
// (0x0053A880-0x0053B730).
#include "b/vm_commands.h"
#include "b/sws_creature.h"

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT;      // 0x0074663A

// Event bookkeeping on every server object (CSWSObject keeps its members
// out of the shared header, see worker-b notes).
struct CSWSObjectLastView {
    char pad_0x0[0xf8];
    int m_bDead;                                            // 0xf8 (name guessed)
    char pad_0xfc[0x154 - 0xfc];
    unsigned long m_oidLastKiller;                          // 0x154 (name guessed)
    unsigned long m_oidLastHostileActor;                    // 0x158 (name guessed)
    unsigned long m_oidLastAttacker;                        // 0x15c (name guessed)
    unsigned long m_oidLastDamager;                         // 0x160 (name guessed)
    unsigned long m_oidLastDisturbed;                       // 0x164 (name guessed)
};

class CSWSDoor : public CSWSObject {
public:
    char pad_0xf4[0x314 - 0xf4];
    unsigned long m_oidLastLocked;                          // 0x314 (name guessed)
    unsigned long m_oidLastUnlocked;                        // 0x318 (name guessed)
    unsigned long m_oidLastOpenedBy;                        // 0x31c (name guessed)
    unsigned long m_oidLastClosedBy;                        // 0x320 (name guessed)
    char pad_0x324[0x328 - 0x324];
    unsigned long m_oidLastDisarmed;                        // 0x328 (name guessed)
};

class CSWSPlaceable : public CSWSObject {
public:
    char pad_0xf4[0x350 - 0xf4];
    unsigned long m_oidLastOpenedBy;                        // 0x350 (name guessed)
    unsigned long m_oidLastClosedBy;                        // 0x354 (name guessed)
    unsigned long m_oidLastUsedBy;                          // 0x358 (name guessed)
    char pad_0x35c[0x360 - 0x35c];
    unsigned long m_oidLastDisarmed;                        // 0x360 (name guessed)
    unsigned long m_oidLastLocked;                          // 0x364 (name guessed)
    unsigned long m_oidLastUnlocked;                        // 0x368 (name guessed)
};

// Vtable slot 0x40 (object type 7).
class CSWSAreaOfEffectObject : public CSWSObject {
public:
    char pad_0xf4[0x2a0 - 0xf4];
    unsigned long m_oidExitingObject;                       // 0x2a0 (name guessed)
    unsigned long m_oidLastDisarmed;                        // 0x2a4 (name guessed)
};

// Vtable slot 0x50 (object type 0xb).
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x248 - 0xf4];
    unsigned long m_oidCreator;                             // 0x248
    char pad_0x24c[0x254 - 0x24c];
    unsigned long m_oidExitingObject;                       // 0x254 (name guessed)
};

// Vtable slot 0x5c (object type 0xd).
struct CGameObject5CExitView {
    char pad_0x0[0x2e4];
    unsigned long m_oidExitingObject;                       // 0x2e4 (name guessed)
};

// FUNCTION: SWKOTOR 0x0053A880
int CSWVirtualMachineCommands::ExecuteCommandGetLastAttacker(int nCommandId, int nParameters)
{
    unsigned long oidAttackee = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidAttackee)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidAttackee);
    if (pObject && pObject->AsSWSObject()) {
        oidResult = ((CSWSObjectLastView*)pObject->AsSWSObject())->m_oidLastAttacker;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A900
int CSWVirtualMachineCommands::ExecuteCommandGetLastHostileTarget(int nCommandId, int nParameters)
{
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidAttacker)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidAttacker);
    if (pCreature) {
        oidResult = pCreature->m_oidLastHostileTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A970
int CSWVirtualMachineCommands::ExecuteCommandGetLastAttackAction(int nCommandId, int nParameters)
{
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidAttacker)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidAttacker);
    if (pCreature) {
        nResult = pCreature->m_nLastAttackAction;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A9E0
int CSWVirtualMachineCommands::ExecuteCommandGetLastAttackMode(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = INVALID_OBJECT_ID;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_n4E0) {
        switch (pCreature->m_nLastAttackMode) {
        case 1:
            nResult = 1;
            break;
        case 2:
            nResult = 2;
            break;
        case 3:
            nResult = 3;
            break;
        case 5:
            nResult = 4;
            break;
        case 6:
            nResult = 5;
            break;
        default:
            nResult = 0;
            break;
        }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053AAD0
int CSWVirtualMachineCommands::ExecuteCommandGetLastAttackResult(int nCommandId, int nParameters)
{
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidAttacker)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidAttacker);
    if (pCreature) {
        nResult = pCreature->m_nLastAttackResult;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053AB40
int CSWVirtualMachineCommands::ExecuteCommandGetWasForcePowerSuccessful(int nCommandId, int nParameters)
{
    unsigned long oidCreature = INVALID_OBJECT_ID;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        nResult = pCreature->m_bLastForcePowerSuccessful;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053ABB0
int CSWVirtualMachineCommands::ExecuteCommandGetLastAttackType(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_n4E0) {
        switch (pCreature->m_nLastAttackType) {
        case 0xb:
            nResult = 9;
            break;
        case 0x1e:
            nResult = 10;
            break;
        default:
            nResult = 0;
            break;
        }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053AC60
int CSWVirtualMachineCommands::ExecuteCommandGetLastClosedBy(int nCommandId, int nParameters)
{
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (pObject) {
        if (pObject->AsSWSPlaceable()) {
            oidResult = pObject->AsSWSPlaceable()->m_oidLastClosedBy;
        } else if (pObject->AsSWSDoor()) {
            oidResult = pObject->AsSWSDoor()->m_oidLastClosedBy;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053ACD0
int CSWVirtualMachineCommands::ExecuteCommandGetLastCombatFeatUsed(int nCommandId, int nParameters)
{
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidAttacker)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidAttacker);
    if (pCreature) {
        nResult = pCreature->m_nLastCombatFeatUsed;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// The original reads an uninitialised pointer when the caller is not a
// server object.
// FUNCTION: SWKOTOR 0x0053AD40
int CSWVirtualMachineCommands::ExecuteCommandGetLastDamager(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        oidResult = ((CSWSObjectLastView*)pObject)->m_oidLastDamager;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053ADB0
int CSWVirtualMachineCommands::ExecuteCommandGetLastDisarmed(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        if (pObject->m_nObjectType == 9) {
            oidResult = pObject->AsSWSPlaceable()->m_oidLastDisarmed;
        } else if (pObject->m_nObjectType == 7) {
            oidResult = pObject->AsSWSAreaOfEffectObject()->m_oidLastDisarmed;
        } else if (pObject->m_nObjectType == 0xa) {
            oidResult = pObject->AsSWSDoor()->m_oidLastDisarmed;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053AE50
int CSWVirtualMachineCommands::ExecuteCommandGetLastDisturbed(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        if (pObject->m_nObjectType == 5) {
            oidResult = pObject->AsSWSCreature()->m_oidLastDisturbed;
        } else if (pObject->m_nObjectType == 9) {
            oidResult = ((CSWSObjectLastView*)pObject->AsSWSPlaceable())->m_oidLastDisturbed;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053AED0
int CSWVirtualMachineCommands::ExecuteCommandGetLastForcePowerUsed(int nCommandId, int nParameters)
{
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidAttacker)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidAttacker);
    if (pCreature) {
        nResult = pCreature->m_nLastForcePowerUsed;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// NONMATCH (23): pObject/pCreature swap edi/ebx and ebx is pushed late.
// FUNCTION: SWKOTOR 0x0053AF40
int CSWVirtualMachineCommands::ExecuteCommandGetLastHostileActor(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    unsigned long oidCreature = INVALID_OBJECT_ID;
    CSWSCreature* pCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidCreature);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject() && (pObject = pGameObject->AsSWSObject()) != NULL) {
        CSWSObjectLastView* pView = (CSWSObjectLastView*)pObject;
        if (pView->m_bDead == 1) {
            if (pView->m_oidLastHostileActor != INVALID_OBJECT_ID) {
                ((CSWSCreature*)pObject)->SetLastHostileActor(INVALID_OBJECT_ID, 1);
            }
            oidResult = INVALID_OBJECT_ID;
        } else {
            oidResult = pView->m_oidLastHostileActor;
            CGameObject* pActor = g_pAppManager->GetServerExoApp()->GetGameObject(oidResult);
            pCreature = NULL;
            if (pActor) {
                if (pActor->m_nObjectType == OBJECT_TYPE_AREA_OF_EFFECT) {
                    oidResult = pActor->AsSWSTrigger()->m_oidCreator;
                    pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidResult);
                } else if (pActor->m_nObjectType == OBJECT_TYPE_CREATURE) {
                    pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidResult);
                }
            }
            if (!pCreature || pCreature->GetIsInConversation() == 1 || pCreature->GetIsPaused() == 1 ||
                pCreature->GetDeadTemp()) {
                ((CSWSCreature*)pObject)->SetLastHostileActor(INVALID_OBJECT_ID, 1);
                oidResult = INVALID_OBJECT_ID;
            }
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B080
int CSWVirtualMachineCommands::ExecuteCommandGetLastKiller(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject && pObject->AsSWSObject()) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject) {
            oidResult = ((CSWSObjectLastView*)pSWSObject)->m_oidLastKiller;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B0E0
int CSWVirtualMachineCommands::ExecuteCommandGetLastLocked(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        if (pObject->m_nObjectType == 0xa) {
            oidResult = pObject->AsSWSDoor()->m_oidLastLocked;
        } else if (pObject->m_nObjectType == 9) {
            oidResult = pObject->AsSWSPlaceable()->m_oidLastLocked;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B170
int CSWVirtualMachineCommands::ExecuteCommandGetLastOpenedBy(int nCommandId, int nParameters)
{
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (pObject) {
        if (pObject->AsSWSPlaceable()) {
            oidResult = pObject->AsSWSPlaceable()->m_oidLastOpenedBy;
        } else if (pObject->AsSWSDoor()) {
            oidResult = pObject->AsSWSDoor()->m_oidLastOpenedBy;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B1E0
int CSWVirtualMachineCommands::ExecuteCommandGetLastPazaakResult(int nCommandId, int nParameters)
{
    int nResult = 0;
    int nWager = 0;
    g_pAppManager->GetClientExoApp()->GetPazaakResult(&nResult, &nWager);
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B230
int CSWVirtualMachineCommands::ExecuteCommandGetLastPerceived(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject);
    unsigned long oidResult = OBJECT_INVALID_ID;
    if (pObject->AsSWSCreature()) {
        oidResult = pObject->AsSWSCreature()->m_oidLastPerceived;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetLastPerceptionInaudible, GetLastPerceptionSeen and
// GetLastPerceptionVanished.
// FUNCTION: SWKOTOR 0x0053B2A0
int CSWVirtualMachineCommands::ExecuteCommandGetLastPerception(int nCommandId, int nParameters)
{
    int nResult = 0;
    CGameObject* pObject;
    if (nCommandId == 0x101) {
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSCreature()) {
            nResult = pObject->AsSWSCreature()->m_bLastPerceptionHeard;
        }
    } else if (nCommandId == 0x102) {
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSCreature()) {
            nResult = pObject->AsSWSCreature()->m_bLastPerceptionInaudible;
        }
    } else if (nCommandId == 0x103) {
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSCreature()) {
            nResult = pObject->AsSWSCreature()->m_bLastPerceptionSeen;
        }
    } else if (nCommandId == 0x105) {
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSCreature()) {
            nResult = pObject->AsSWSCreature()->m_bLastPerceptionVanished;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B410
int CSWVirtualMachineCommands::ExecuteCommandGetLastSpeaker(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        oidResult = pObject->AsSWSCreature()->m_oidLastSpeaker;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B480
int CSWVirtualMachineCommands::ExecuteCommandGetLastTrapDetected(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pCreature) {
        oidResult = pCreature->m_oidLastTrapDetected;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B4E0
int CSWVirtualMachineCommands::ExecuteCommandGetLastUnlocked(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    CSWSObject* pObject;
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        if (pObject->m_nObjectType == 0xa) {
            oidResult = pObject->AsSWSDoor()->m_oidLastUnlocked;
        } else if (pObject->m_nObjectType == 9) {
            oidResult = pObject->AsSWSPlaceable()->m_oidLastUnlocked;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B570
int CSWVirtualMachineCommands::ExecuteCommandGetLastUsedBy(int nCommandId, int nParameters)
{
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (pObject->AsSWSPlaceable()) {
        oidResult = pObject->AsSWSPlaceable()->m_oidLastUsedBy;
    } else if (pObject->AsSWSDoor()) {
        oidResult = pObject->AsSWSDoor()->m_oidLastOpenedBy;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B5E0
int CSWVirtualMachineCommands::ExecuteCommandGetLastWeaponUsed(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (pCreature) {
        oidResult = pCreature->m_oidLastWeaponUsed;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B640
int CSWVirtualMachineCommands::ExecuteCommandGetExitingObject(int nCommandId, int nParameters)
{
    unsigned long oidResult = OBJECT_INVALID_ID;
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        switch (pObject->m_nObjectType) {
        case 7:
            {
                CSWSAreaOfEffectObject* p = pObject->AsSWSAreaOfEffectObject();
                if (p) {
                    oidResult = p->m_oidExitingObject;
                }
            }
            break;
        case 0xb:
            {
                CSWSTrigger* p = pObject->AsSWSTrigger();
                if (p) {
                    oidResult = p->m_oidExitingObject;
                }
            }
            break;
        case 3:
            {
                CSWSModule* p = pObject->AsSWSModule();
                if (p) {
                    oidResult = p->m_oidExitingObject;
                }
            }
            break;
        case 4:
            {
                CSWSArea* p = pObject->AsSWSArea();
                if (p) {
                    oidResult = p->m_oidExitingObject;
                }
            }
            break;
        case 0xd:
            {
                void* p = pObject->AsSWS5C();
                if (p) {
                    oidResult = ((CGameObject5CExitView*)p)->m_oidExitingObject;
                }
            }
            break;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B730
int CSWVirtualMachineCommands::ExecuteCommandGetLoadFromSaveGame(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetLoadFromSaveGame()) ?
        0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
