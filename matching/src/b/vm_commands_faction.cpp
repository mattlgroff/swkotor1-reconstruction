// CSWVirtualMachineCommands: GetFaction* queries (0x005380F0-0x00538BD0).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/faction_manager.h"

#define INVALID_OBJECT_ID 0x7f000000

static inline CFactionManager* GetServerFactionManager()
{
    CServerExoAppInternal* pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (!pInternal) {
        return NULL;
    }
    return pInternal->m_pFactionManager;
}

// FUNCTION: SWKOTOR 0x005380F0
int CSWVirtualMachineCommands::ExecuteCommandGetFactionAverageGoodEvilAlignment(int nCommandId, int nParameters)
{
    int nResult = -1;
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetAverageGoodEvilAlignment();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00538190
int CSWVirtualMachineCommands::ExecuteCommandGetFactionAverageLevel(int nCommandId, int nParameters)
{
    int nResult = -1;
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetAverageLevel();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00538230
int CSWVirtualMachineCommands::ExecuteCommandGetFactionAverageReputation(int nCommandId, int nParameters)
{
    unsigned long oidSource;
    unsigned long oidTarget;
    int nResult = -1;
    if (!g_pVirtualMachine->StackPopObject(&oidSource)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSource);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetAverageReputation(oidTarget);
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005382E0
int CSWVirtualMachineCommands::ExecuteCommandGetFactionAverageXP(int nCommandId, int nParameters)
{
    int nResult = -1;
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetAverageXP();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00538380
int CSWVirtualMachineCommands::ExecuteCommandGetFactionBestAC(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetBestAC(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538450
int CSWVirtualMachineCommands::ExecuteCommandGetFactionEqual(int nCommandId, int nParameters)
{
    unsigned long oidFirst;
    unsigned long oidSecond;
    int nFirstFaction;
    int nSecondFaction;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidFirst)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidSecond)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidFirst, &nFirstFaction) == 1 &&
        g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidSecond, &nSecondFaction) == 1 &&
        nFirstFaction == nSecondFaction) {
        nResult = 1;
    }
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538500
int CSWVirtualMachineCommands::ExecuteCommandGetFactionGold(int nCommandId, int nParameters)
{
    int nResult = -1;
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetGold();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005385A0
int CSWVirtualMachineCommands::ExecuteCommandGetFactionLeastDamagedMember(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetLeastDamagedMember(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538670
int CSWVirtualMachineCommands::ExecuteCommandGetFactionLeader(int nCommandId, int nParameters)
{
    unsigned long oidMemberOfFaction;
    unsigned long oidLeader = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidMemberOfFaction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidMemberOfFaction);
    if (pCreature && pCreature->GetFaction()) {
        oidLeader = pCreature->GetFaction()->GetLeader();
    }
    return g_pVirtualMachine->StackPushObject(oidLeader) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetNextFactionMember (0x17d).
// NONMATCH: the original keeps g_pAppManager in ecx and reloads the server
// pointer for GetFactionOfObject; ours keeps the server pointer in ecx.
// FUNCTION: SWKOTOR 0x005386F0
int CSWVirtualMachineCommands::ExecuteCommandGetFirstFactionMember(int nCommandId, int nParameters)
{
    unsigned long oidMemberOfFaction = INVALID_OBJECT_ID;
    int bPCOnly = 1;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidMemberOfFaction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bPCOnly)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidMemberOfFaction, &pObject) == GAME_OBJECT_VALID) {
        CFactionManager* pFactionManager = GetServerFactionManager();
        int nFactionId;
        if (pFactionManager && g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidMemberOfFaction, &nFactionId) == 1) {
            CSWSFaction* pFaction = (CSWSFaction*)pFactionManager->GetFaction(nFactionId);
            if (pFaction) {
                if (nCommandId == 0x17c) {
                    oidResult = pFaction->GetFactionMember(0, bPCOnly);
                } else if (nCommandId == 0x17d) {
                    oidResult = pFaction->GetFactionMember(pFaction->m_nCurrentMember, bPCOnly);
                }
            }
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538800
int CSWVirtualMachineCommands::ExecuteCommandGetFactionMostDamagedMember(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetMostDamagedMember(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005388D0
int CSWVirtualMachineCommands::ExecuteCommandGetFactionMostFrequentClass(int nCommandId, int nParameters)
{
    int nResult = -1;
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            nResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetMostFrequentClass();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00538970
int CSWVirtualMachineCommands::ExecuteCommandGetFactionStrongestMember(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetStrongestMember(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538A40
int CSWVirtualMachineCommands::ExecuteCommandGetFactionWeakestMember(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetWeakestMember(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00538B10
int CSWVirtualMachineCommands::ExecuteCommandGetFactionWorstAC(int nCommandId, int nParameters)
{
    unsigned long oidFactionMember = INVALID_OBJECT_ID;
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMustBeVisible = 1;
    if (nParameters == 2 && !g_pVirtualMachine->StackPopInteger(&bMustBeVisible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidFactionMember);
    if (pCreature) {
        int nFactionId = pCreature->m_pStats->m_nFactionId;
        CFactionManager* pFactionManager = GetServerFactionManager();
        if (pFactionManager) {
            oidResult = ((CSWSFaction*)pFactionManager->GetFaction(nFactionId))->GetWorstAC(oidFactionMember, bMustBeVisible);
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
