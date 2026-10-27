// CSWVirtualMachineCommands::ExecuteCommandDestroyObject (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"
#include "b/party_table.h"

class CSWCCreature;
class CScriptEvent;

// sws_item.h cannot be combined with party_table.h.
class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                          unsigned long nObject, unsigned long nType,
                          CScriptEvent* pEvent);            // 0x004B08D0
};

extern const unsigned long EVENT_DESTROY_OBJECT;           // 0x007441E0 (11)

class CSWParty {
public:
    CSWCCreature* GetPC();                                  // 0x00635460
    int RemoveCharacter(unsigned long oidCreature);         // 0x006349F0

    int m_nCount;                                           // 0x00
};

// NONMATCH (25): temp register choice for the app pointer loads (eax/ecx/edx).
// FUNCTION: SWKOTOR 0x0052FF20
int CSWVirtualMachineCommands::ExecuteCommandDestroyObject(int nCommandId, int nParameters)
{
    unsigned long oidDestroy;
    float fDelay = 0.0f;
    int bNoFade = 0;
    float fDelayUntilFade = 0.0f;
    if (!g_pVirtualMachine->StackPopObject(&oidDestroy)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopFloat(&fDelay)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&bNoFade)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopFloat(&fDelayUntilFade)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetSWParty()->GetPC();
    if (pPC && g_pAppManager->GetServerExoApp()->ClientToServerObjectID(pPC->m_idSelf) == oidDestroy) {
        unsigned long oidOld = pPC->m_idSelf;
        unsigned char i = 0;
        if (g_pAppManager->m_pClientExoApp->GetSWParty()->m_nCount > 0) {
            do {
                g_pAppManager->m_pClientExoApp->ChangeCharacter(-2, 1);
                CSWCCreature* pNext = g_pAppManager->m_pClientExoApp->GetSWParty()->GetPC();
                if (pNext &&
                    g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(
                        g_pAppManager->GetServerExoApp()->ClientToServerObjectID(pNext->m_idSelf))) {
                    g_pAppManager->m_pClientExoApp->SetPlayerCreature(pNext->m_idSelf);
                    g_pAppManager->m_pClientExoApp->AttachPlayerGameObjectToArea();
                    break;
                }
                i++;
            } while (i < g_pAppManager->m_pClientExoApp->GetSWParty()->m_nCount);
        }
        g_pAppManager->m_pClientExoApp->GetSWParty()->RemoveCharacter(oidOld);
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CGameObject* pObject = pServer->GetGameObject(oidDestroy);
    if (pObject && pObject->AsSWSObject()) {
        int nFadeTime = (int)(fDelayUntilFade * 1000.0f);
        pObject->AsSWSObject()->SetDeleteNoFade(bNoFade);
        pObject->AsSWSObject()->SetFadeOutTime(nFadeTime);
    }
    pServer->GetPartyTable()->ClearNPCObjectId(pServer->GetPartyTable()->GetNPCIndexOfObject(oidDestroy));
    pServer->GetServerAIMaster()->AddEventDeltaTime(
        0, (int)(fDelay * 1000.0f), m_oidObjectRunScript, oidDestroy, EVENT_DESTROY_OBJECT, (CScriptEvent*)1);
    return 0;
}
