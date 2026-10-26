// CSWVirtualMachineCommands: item commands that need the full CSWSItem.
#include "b/vm_commands.h"
#include "b/sws_message.h"
#include "b/sws_item.h"

// FUNCTION: SWKOTOR 0x0053C220
int CSWVirtualMachineCommands::ExecuteCommandGetItemStackSize(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    int nStackSize;
    if (pItem) {
        nStackSize = pItem->m_nStackCount;
    } else {
        nStackSize = -1;
    }
    return g_pVirtualMachine->StackPushInteger(nStackSize) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00547970
int CSWVirtualMachineCommands::ExecuteCommandChangeItemCost(int nCommandId, int nParameters)
{
    float fCostMultiplier = 1.0f;
    CExoString sItemTemplate;
    if (!g_pVirtualMachine->StackPopString(&sItemTemplate)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fCostMultiplier)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CResRef cResRef(sItemTemplate.CStr());
    CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
    if (pItem && pItem->LoadFromTemplate(cResRef)) {
        pItem->GetBaseItem()->SetCostMultiplier(fCostMultiplier);
    }
    return 0;
}

class CSWSStore {
public:
    int AcquireItem(CSWSItem* pItem);                       // 0x005C7610
};

class CSWGuiInGame {
public:
    void ShowFeedbackMessage(int nType, unsigned long nValue); // 0x0062EEB0 (name guessed)
};

extern const unsigned long OBJECT_INVALID;                  // 0x00746644

// FUNCTION: SWKOTOR 0x0052F570
int CSWVirtualMachineCommands::ExecuteCommandCreateItemOnObject(int nCommandId, int nParameters)
{
    CExoString sItemTemplate;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopString(&sItemTemplate)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nStackSize = 1;
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&nStackSize)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidItem = OBJECT_INVALID;
    CGameObject* pTarget;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID) {
        CResRef cResRef(sItemTemplate.CStr());
        CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
        if (pItem->LoadFromTemplate(cResRef)) {
            pItem->m_bDroppable = 1;
            int nBaseItem = pItem->m_nBaseItem;
            CSWBaseItem* pBaseItem = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
            if (pBaseItem && nStackSize > pBaseItem->m_nStackSize) {
                nStackSize = pBaseItem->m_nStackSize;
            }
            if (nStackSize >= 1) {
                pItem->m_nStackCount = (unsigned short)nStackSize;
                if (pTarget->AsSWSCreature()) {
                    pTarget->AsSWSCreature()->AcquireItem(&pItem, INVALID_OBJECT_ID, INVALID_OBJECT_ID, 1);
                    if (pTarget->AsSWSCreature()->m_bInParty) {
                        g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowFeedbackMessage(7, 0);
                    }
                } else if (pTarget->AsSWSPlaceable()) {
                    pTarget->AsSWSPlaceable()->AcquireItem(&pItem, INVALID_OBJECT_ID, 1);
                } else if (pTarget->AsSWSStore()) {
                    pTarget->AsSWSStore()->AcquireItem(pItem);
                } else {
                    if (pItem) {
                        delete pItem;
                    }
                    pItem = NULL;
                }
                if (pItem) {
                    oidItem = pItem->m_idSelf;
                }
            } else if (pItem) {
                delete pItem;
            }
        } else if (pItem) {
            delete pItem;
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005369B0
int CSWVirtualMachineCommands::ExecuteCommandGetBaseItemType(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = 0x100;
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        nResult = pItem->m_nBaseItem;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
