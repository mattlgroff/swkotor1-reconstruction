// VM item stack-size handler.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"

class CSWCCMessageData {
public:
    CSWCCMessageData();                                      // 0x004D69F0
    void SetInteger(int nIndex, int nValue);                 // 0x004D76B0
    void SetObjectID(int nIndex, unsigned long oidValue);    // 0x004D7780
    char pad_0x0[0x34];
};
class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

// NONMATCH (17): register choices for the base-item and size values.
// FUNCTION: SWKOTOR 0x00546630
int CSWVirtualMachineCommands::ExecuteCommandSetItemStackSize(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int nSize = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (!g_pVirtualMachine->StackPopInteger(&nSize)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        int nBaseItem = pItem->m_nBaseItem;
        unsigned short nMax = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem)->m_nStackSize;
        if (nSize > nMax) nSize = nMax;
        if (nSize < 1) nSize = 1;
        int nOldSize = pItem->m_nStackCount;
        if (nSize != nOldSize) {
            pItem->m_nStackCount = nSize;
            CSWSCreature* pOwner = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(pItem->m_oidPossessor);
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pItem->m_idSelf);
            pData->SetInteger(0, 1);
            if (nSize > nOldSize) {
                pOwner->SendFeedbackMessage(0x32, pData);
                g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(7, 0);
            } else {
                pOwner->SendFeedbackMessage(0x33, pData);
                g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(8, 0);
            }
        }
    }
    return 0;
}
