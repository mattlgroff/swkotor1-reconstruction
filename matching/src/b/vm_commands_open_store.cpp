// Open a store and queue its store-opened script event.
#include "b/vm_commands.h"
#include "b/sws_item.h"
#include "b/sws_player.h"

class CSWSStore {
public:
    char pad_0x00[0x24c];
    int m_nMarkdown;
    int m_nMarkup;
};
class CGuiInGame {
public:
    void ShowStoreGui(unsigned long oidStore, unsigned long oidPlayer); // 0x0062E310
};

// FUNCTION: SWKOTOR 0x00540300
int CSWVirtualMachineCommands::ExecuteCommandOpenStore(int nCommandId, int nParameters)
{
    int nMarkup = 0;
    int nMarkdown = 0;
    unsigned long oidStore;
    unsigned long oidPlayer;
    if (g_pVirtualMachine->StackPopObject(&oidStore) &&
        g_pVirtualMachine->StackPopObject(&oidPlayer) &&
        (nParameters <= 2 || (g_pVirtualMachine->StackPopInteger(&nMarkup) &&
                              g_pVirtualMachine->StackPopInteger(&nMarkdown)))) {
    if (nMarkup <= 100) {
        if (nMarkup < -100) nMarkup = -100;
    } else nMarkup = 100;
    if (nMarkdown <= 100) {
        if (nMarkdown < -100) nMarkdown = -100;
    } else nMarkdown = 100;
    CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidStore);
    if (pObject) {
        CSWSStore* pStore = pObject->AsSWSStore();
        if (pStore) {
            pStore->m_nMarkup = nMarkup;
            pStore->m_nMarkdown = nMarkdown;
            ((CGuiInGame*)g_pAppManager->GetClientExoApp()->GetInGameGui())->ShowStoreGui(oidStore, oidPlayer);
            CScriptEvent* pEvent = new CScriptEvent;
            pEvent->m_nType = 0x16;
            if (!pPlayer) {
                CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
                pServer->GetServerAIMaster()->AddEventDeltaTime(0, 0, pServer->GetPlayerCreatureId(), oidStore, EVENT_DELAY_INVENTORY, pEvent);
            } else {
                unsigned long oidCaller = pPlayer->GetGameObject()->m_idSelf;
                g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidCaller, oidStore, EVENT_DELAY_INVENTORY, pEvent);
            }
        }
    }
        return 0;
    }
    return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
}
