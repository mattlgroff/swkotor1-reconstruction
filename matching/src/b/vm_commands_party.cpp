// CSWVirtualMachineCommands: party roster and alignment commands
// (0x0052DC40-0x0052E180).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/party_table.h"
#include "b/faction_manager.h"
#include <stdlib.h>

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0

    char pad_0x0[0x34];
};

// FUNCTION: SWKOTOR 0x0052DC40
int CSWVirtualMachineCommands::ExecuteCommandAddAvailableNPCByObject(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(pServer->GetPartyTable()->AddNPC(nNPC, pServer->GetCreatureByGameObjectID(oidCreature)))) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052DCD0
int CSWVirtualMachineCommands::ExecuteCommandAddAvailableNPCByTemplate(int nCommandId, int nParameters)
{
    CExoString sTemplate;
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sTemplate)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPushInteger(pServer->GetPartyTable()->AddNPC(nNPC, sTemplate))) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052DDC0
int CSWVirtualMachineCommands::ExecuteCommandAddMultiClass(int nCommandId, int nParameters)
{
    int nClassType;
    unsigned long oidSource;
    if (!g_pVirtualMachine->StackPopInteger(&nClassType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidSource)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSource);
    if (pCreature) {
        int nExperience = pCreature->m_pStats->m_nExperience;
        int nXP = pCreature->m_pStats->GetExpNeededForNextLevel() - nExperience;
        pCreature->m_pStats->AddClass((unsigned char)nClassType, 0);
        if (g_pAppManager->GetServerExoApp()->GetPartyTable()) {
            g_pAppManager->GetServerExoApp()->GetPartyTable()->DistributeExperience(nXP, 1);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052DE70
int CSWVirtualMachineCommands::ExecuteCommandAddPartyMember(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    int bResult = pServer->GetPartyTable()->AddMember(nNPC, oidCreature);
    if (bResult) {
        CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
        CSWCCreature* pClientCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(oidCreature));
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oidCreature);
        if (pClientCreature) {
            pClientCreature->SetPartyMember(1);
        }
        if (pCreature) {
            pCreature->SetInParty(1, 1);
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(bResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052DF40
int CSWVirtualMachineCommands::ExecuteCommandAddToParty(int nCommandId, int nParameters)
{
    unsigned long oidPC;
    unsigned long oidPartyLeader;
    if (!g_pVirtualMachine->StackPopObject(&oidPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidPartyLeader)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pLeader = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPartyLeader);
    CSWSCreature* pPC = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPC);
    if (pLeader && pLeader->m_bIsPC && pPC && pPC->m_bIsPC) {
        CSWSFaction* pFaction = pLeader->GetFaction();
        if (pFaction) {
            pFaction->AddMember(oidPC, 0);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052DFE0
int CSWVirtualMachineCommands::ExecuteCommandAdjustAlignment(int nCommandId, int nParameters)
{
    unsigned long oidSubject;
    int nAlignment;
    int nShift;
    if (!g_pVirtualMachine->StackPopObject(&oidSubject) ||
        !g_pVirtualMachine->StackPopInteger(&nAlignment) ||
        !g_pVirtualMachine->StackPopInteger(&nShift)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSubject);
    if (!pCreature) {
        return 0;
    }
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    pStats->GetSimpleAlignmentGoodEvil();
    short nGoodEvil = pStats->m_nGoodEvil;
    short nDelta = 0;
    switch (nAlignment) {
    case 1:
        if (nGoodEvil > 50) {
            nDelta = max(-nShift, 50 - nGoodEvil);
        } else {
            nDelta = min(nShift, 50 - nGoodEvil);
        }
        break;
    case 2:
        nDelta = nShift;
        break;
    case 3:
        nDelta = -nShift;
        break;
    }
    pStats->ModifyAlignment(nDelta);
    CSWCCMessageData* pData = new CSWCCMessageData;
    if (nAlignment != 1) {
        pData->SetInteger(0, nAlignment);
        pData->SetInteger(1, nShift);
    } else {
        if (nGoodEvil > 50) {
            pData->SetInteger(0, 3);
        } else {
            pData->SetInteger(0, 2);
        }
        pData->SetInteger(1, abs(nShift));
    }
    pCreature->SendFeedbackMessage(0x6c, pData);
    if (nDelta != 0 && pCreature->m_bInParty) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(4, nDelta);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052E1A0
int CSWVirtualMachineCommands::ExecuteCommandAdjustReputation(int nCommandId, int nParameters)
{
    unsigned long oidTarget = INVALID_OBJECT_ID;
    unsigned long oidSourceFactionMember = INVALID_OBJECT_ID;
    int nAdjustment;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidSourceFactionMember)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAdjustment)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pSource = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSourceFactionMember);
    if (pSource && pSource->m_nF8) {
        return 0;
    }
    CSWSCreature* pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        int nFaction;
        if (g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidSourceFactionMember, &nFaction) == 1) {
            pTarget->AdjustReputation(nFaction, nAdjustment);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052E270
int CSWVirtualMachineCommands::ExecuteCommandAmbientSound(int nCommandId, int nParameters)
{
    int nValue = 0;
    unsigned long oidArea;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        switch (nCommandId) {
        case 0x1b1:
            pArea->m_pAmbientSound->PlayAmbientSound(1);
            return 0;
        case 0x1b2:
            pArea->m_pAmbientSound->PlayAmbientSound(0);
            return 0;
        case 0x1b3:
            pArea->m_pAmbientSound->SetAmbientDayTrack(nValue);
            return 0;
        case 0x1b4:
            pArea->m_pAmbientSound->SetAmbientNightTrack(nValue);
            return 0;
        case 0x237:
            pArea->m_pAmbientSound->SetAmbientDayVolume(nValue);
            return 0;
        case 0x238:
            pArea->m_pAmbientSound->SetAmbientNightVolume(nValue);
            return 0;
        }
    }
    return 0;
}

// CSWSPlaceable +0x23c (sws_item.h's layout clashes with party_table.h).
struct CSWSPlaceableFactionView {
    char pad_0x0[0x23c];
    int m_nFactionId;                                       // 0x23c
};

// FUNCTION: SWKOTOR 0x0052F2F0
int CSWVirtualMachineCommands::ExecuteCommandChangeFaction(int nCommandId, int nParameters)
{
    unsigned long oidObjectToChangeFaction;
    int nFactionId;
    unsigned long oidMemberOfFactionToJoin;
    int nStandardFaction;
    if (!g_pVirtualMachine->StackPopObject(&oidObjectToChangeFaction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId == 0xad) {
        if (!g_pVirtualMachine->StackPopObject(&oidMemberOfFactionToJoin)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else if (nCommandId == 0x19c) {
        if (!g_pVirtualMachine->StackPopInteger(&nStandardFaction)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CServerExoAppInternal* pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (!pInternal) {
        return 0;
    }
    CFactionManager* pFactionManager = pInternal->m_pFactionManager;
    if (!pFactionManager) {
        return 0;
    }
    if (nCommandId == 0xad) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidObjectToChangeFaction);
        if (!pCreature || pCreature->m_pStats->m_bIsPC) {
            return 0;
        }
        if (!g_pAppManager->GetServerExoApp()->GetFactionOfObject(oidMemberOfFactionToJoin, &nFactionId)) {
            return 0;
        }
        CSWSCreature* pMember = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidMemberOfFactionToJoin);
        if (!pMember || pMember->m_pStats->m_bIsPC) {
            return 0;
        }
    } else if (nCommandId == 0x19c) {
        if (!pFactionManager->IsValidFactionId(nStandardFaction)) {
            return 0;
        }
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidObjectToChangeFaction);
        if (pCreature && pCreature->m_pStats->m_bIsPC) {
            return 0;
        }
        nFactionId = nStandardFaction;
    }
    CSWSPlaceableFactionView* pPlaceable =
        (CSWSPlaceableFactionView*)g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidObjectToChangeFaction);
    if (pPlaceable) {
        pPlaceable->m_nFactionId = nFactionId;
        return 0;
    }
    pFactionManager->GetFaction(nFactionId)->AddMember(oidObjectToChangeFaction, 0);
    return 0;
}
