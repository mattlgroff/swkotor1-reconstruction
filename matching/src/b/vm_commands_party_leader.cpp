// CSWVirtualMachineCommands::ExecuteCommandSetPartyLeader (round s8): the
// client party's member list and leader slot.
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/sws_creature.h"
#include "b/party_table.h"

extern const unsigned short DURATION_TYPE_INSTANT;          // 0x007473B8 (0)

class CSWCCreatureServerView {
public:
    CSWSCreature* AsServerCreature();                       // 0x0060FB20
};

// Client party member entry (0x88 bytes).
struct CSWPartyMemberEntry {
    char pad_0x0[0x20];
    unsigned long m_oidCreature;                            // 0x20 (client id)
    char pad_0x24[0x88 - 0x24];
};

class CSWParty {
public:
    void SetLeader(int nIndex);                             // 0x00635480 (name guessed)
    int GetCount() { return m_nCount; }
    unsigned long GetMemberId(int nIndex) { return m_aMembers[nIndex].m_oidCreature; }

    int m_nCount;                                           // 0x00
    CSWPartyMemberEntry m_aMembers[1];                      // 0x04
};

// FUNCTION: SWKOTOR 0x00545A70
int CSWVirtualMachineCommands::ExecuteCommandSetPartyLeader(int nCommandId, int nParameters)
{
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    int bResult = 0;
    CSWParty* pParty = pClient->GetSWParty();
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nNPC == -1) {
        for (int i = 0; i < pParty->GetCount(); i++) {
            CSWCCreatureServerView* pMember =
                (CSWCCreatureServerView*)g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(pParty->GetMemberId(i));
            if (pMember && pMember->AsServerCreature()) {
                CSWSCreature* pCreature = pMember->AsServerCreature();
                if (pCreature->m_bInParty == 1 && pCreature->m_bIsPC == 1 &&
                    (pCreature->GetObjectState94() == 1 || pCreature->GetDeadTemp() == 1)) {
                    CGameEffect* pEffect = new CGameEffect(1);
                    pEffect->m_nType = EFFECT_TYPE_RESURRECTION;
                    pEffect->SetDurationType(DURATION_TYPE_INSTANT);
                    pCreature->ApplyEffect(pEffect, 0, 0);
                    pCreature->m_bF0 = 1;
                }
            }
        }
        pParty->SetLeader(-3);
        bResult = 1;
    } else {
        unsigned long oidNPC = pClient->ServerToClientObjectId(pServer->GetPartyTable()->GetNPCObject(nNPC, 0, 1));
        for (int i = 0; i < pParty->m_nCount; i++) {
            if (pParty->m_aMembers[i].m_oidCreature == oidNPC) {
                pParty->SetLeader(i);
                bResult = 1;
                break;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
