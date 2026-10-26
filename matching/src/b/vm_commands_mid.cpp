// CSWVirtualMachineCommands: mid-size handlers (round s7).
#include "b/vm_commands.h"
#include "b/exo_string.h"
#include "b/sws_creature.h"
#include "b/server_exo_app.h"

extern int g_bDebugActions;                                 // 0x0083281C (descriptive)

// CSWSObject +0xfc (sws_creature.h keeps CSWSObject data-free).
struct CSWSObjectActionView {
    char pad_0x0[0xfc];
    CExoLinkedList<CSWSObjectActionNode> m_lQueuedActions;  // 0xfc
};

// Name generator (nwnamegen.o); 0x24 bytes, used on the stack.
class CSWNameGen {
public:
    CSWNameGen();                                   // 0x00711290
    ~CSWNameGen();                                  // 0x007112E0
    void UnloadNameTable();                              // 0x00711090 (descriptive)
    void LoadNameTable(const CExoString& sTable);       // 0x00711340 (descriptive)
    CExoString GetRandomName(unsigned char nType);  // 0x00711390
    char pad_0x00[0x24];
};

// FUNCTION: SWKOTOR 0x00541A30
int CSWVirtualMachineCommands::ExecuteCommandRandomName(int nCommandId, int nParameters)
{
    CSWNameGen cNameGen;
    CExoString sName;
    {
        CExoString sTable("names");
        cNameGen.LoadNameTable(sTable);
    }
    sName = cNameGen.GetRandomName(0);
    cNameGen.UnloadNameTable();
    if (!g_pVirtualMachine->StackPushString(sName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// NONMATCH (10): the original keeps the list internal in eax and the head
// node in ecx; ours swaps them (declaration orders tried).
// FUNCTION: SWKOTOR 0x00549040
int CSWVirtualMachineCommands::ExecuteCommandGetCurrentAction(int nCommandId, int nParameters)
{
    int nAction = 0xffff;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject && pObject->AsSWSObject()) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        CExoLinkedList<CSWSObjectActionNode>* pList = &((CSWSObjectActionView*)pSWSObject)->m_lQueuedActions;
        if (pList) {
            CSWSObjectActionNode* pNode = pList->GetHead();
            void* pPos = pList->GetHeadPos();
            if (pNode) {
                unsigned short nGroup = pNode->m_n6C;
                do {
                    if (pNode->m_n6C != nGroup) {
                        break;
                    }
                    if (pSWSObject->GetAcceptableAction(pNode->m_nActionId)) {
                        nAction = pSWSObject->GetAQActionIDByID((unsigned short)pNode->m_nActionId, 0);
                    }
                    pNode = pList->GetNext(pPos);
                } while (pNode);
            } else {
                nAction = 0xfffe;
            }
        }
    }
    if (g_bDebugActions) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
        if (pCreature && pCreature->m_bInParty && pCreature->m_bCommandable == 1) {
            pCreature->m_sTag.CStr();
        }
    }
    return g_pVirtualMachine->StackPushInteger(nAction) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
