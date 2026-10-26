// CSWVirtualMachineCommands: world journal entries (module update plus a
// message to every player), 0x005487F0-0x00548BE0.
#include "b/vm_commands.h"
#include "b/sws_player.h"
#include "b/sws_message.h"
#include "b/sws_creature.h"

// FUNCTION: SWKOTOR 0x00548640
int CSWVirtualMachineCommands::ExecuteCommandAddJournalWorldEntry(int nCommandId, int nParameters)
{
    CExoString sEntry;
    CExoString sTitle;
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    int nIndex;
    if (!g_pVirtualMachine->StackPopInteger(&nIndex)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (!g_pVirtualMachine->StackPopString(&sEntry)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (!g_pVirtualMachine->StackPopString(&sTitle)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    unsigned long nCalendarDay;
    unsigned long nTimeOfDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    if (pModule) {
        pModule->AddWorldJournalEntry(nIndex, sEntry, sTitle, nCalendarDay, nTimeOfDay);
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pList) {
        void* pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalAddWorld(pPlayer, nIndex, sEntry, sTitle, nCalendarDay, nTimeOfDay);
            }
            pList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005487F0
int CSWVirtualMachineCommands::ExecuteCommandAddJournalWorldEntryStrref(int nCommandId, int nParameters)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    int nStrref;
    int nTitleStrref;
    if (!g_pVirtualMachine->StackPopInteger(&nStrref)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nTitleStrref)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long nCalendarDay;
    unsigned long nTimeOfDay;
    g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nCalendarDay, &nTimeOfDay);
    if (pModule) {
        pModule->AddWorldJournalEntryStrref(nStrref, nTitleStrref, nCalendarDay, nTimeOfDay);
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pList) {
        void* pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalAddWorldStrref(pPlayer, nStrref, nTitleStrref, nCalendarDay, nTimeOfDay);
            }
            pList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00548A80
int CSWVirtualMachineCommands::ExecuteCommandDeleteJournalWorldAllEntries(int nCommandId, int nParameters)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        pModule->DeleteWorldJournalAllEntries();
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pList) {
        void* pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalDeleteWorldAll(pPlayer);
            }
            pList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00548B20
int CSWVirtualMachineCommands::ExecuteCommandDeleteJournalWorldEntry(int nCommandId, int nParameters)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    int nIndex;
    if (!g_pVirtualMachine->StackPopInteger(&nIndex)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pModule) {
        pModule->DeleteWorldJournalEntry(nIndex);
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pList) {
        void* pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalDeleteWorld(pPlayer, nIndex);
            }
            pList->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00548BE0
int CSWVirtualMachineCommands::ExecuteCommandDeleteJournalWorldEntryStrref(int nCommandId, int nParameters)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    int nStrref;
    if (!g_pVirtualMachine->StackPopInteger(&nStrref)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pModule) {
        pModule->DeleteWorldJournalEntryStrref(nStrref);
    }
    CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (pList) {
        void* pos = pList->GetHeadPos();
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
            if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                pMessage->SendServerToPlayerJournalDeleteWorldStrref(pPlayer, nStrref);
            }
            pList->GetNext(pos);
        }
    }
    return 0;
}

// Waypoint map note state (the rest of CSWSWaypoint is unused here).
class CSWSWaypoint {
public:
    char pad_0x0[0x228];
    int m_bHasMapNote;                                      // 0x228
    int m_bMapNoteEnabled;                                  // 0x22c
};

// FUNCTION: SWKOTOR 0x0054B460
int CSWVirtualMachineCommands::ExecuteCommandSetMapPinEnabled(int nCommandId, int nParameters)
{
    int bEnabled;
    unsigned long oidMapPin;
    if (!g_pVirtualMachine->StackPopObject(&oidMapPin)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bEnabled)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSWaypoint* pWaypoint = g_pAppManager->GetServerExoApp()->GetWaypointByGameObjectID(oidMapPin);
    if (pWaypoint && pWaypoint->m_bHasMapNote) {
        pWaypoint->m_bMapNoteEnabled = bEnabled;
        CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
        CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
        if (pList) {
            void* pos = pList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pos);
                if (pPlayer && pPlayer->GetGameObject() && pMessage) {
                    pMessage->SendServerToPlayerMapPinEnabled(pPlayer, oidMapPin, bEnabled);
                }
                pList->GetNext(pos);
            }
        }
    }
    return 0;
}
