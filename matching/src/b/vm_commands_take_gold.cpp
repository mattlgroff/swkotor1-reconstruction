// CSWVirtualMachineCommands::ExecuteCommandTakeGoldFromCreature (round s8).
#define B_SWS_ITEM_OPAQUE_PROPERTY_GETTERS
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"

extern const char g_szCreditsTemplate[];                    // 0x007471C0 (credits item template)

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

// The original inlines the base item lookup; plain `inline` exceeds cl's budget here.
static __forceinline CSWBaseItem* GetContainerBaseItem(CSWSItem* pItem)
{
    int nBaseItem = pItem->m_nBaseItem;
    return g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
}

// FUNCTION: SWKOTOR 0x00544970
int CSWVirtualMachineCommands::ExecuteCommandTakeGoldFromCreature(int nCommandId, int nParameters)
{
    int nAmount;
    unsigned long oidCreature;
    int bDestroy;
    if (!g_pVirtualMachine->StackPopInteger(&nAmount)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bDestroy)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nAmount <= 0) {
        return 0;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (!pCreature) {
        return 0;
    }
    if (nAmount > pCreature->GetGold()) {
        nAmount = pCreature->GetGold();
    }
    if (nAmount <= 0) {
        return 0;
    }
    CSWSObject* pReceiver = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript)->AsSWSObject();
    if (!pReceiver) {
        return 0;
    }
    if (pReceiver->m_nObjectType == 5) {
        pCreature->RemoveGold(nAmount, 1);
        if (!bDestroy) {
            CSWSCreature* pTarget = pReceiver->AsSWSCreature();
            if (pTarget) {
                pTarget->AddGold(nAmount, 1);
            }
        }
    } else if (pReceiver->m_nObjectType == 9) {
        pCreature->RemoveGold(nAmount, 1);
        if (!bDestroy) {
            CSWSPlaceable* pPlaceable = pReceiver->AsSWSPlaceable();
            if (pPlaceable && pPlaceable->m_bHasInventory && pPlaceable->m_pItemRepository) {
                CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
                CResRef cTemplate(g_szCreditsTemplate);
                pItem->LoadFromTemplate(cTemplate);
                pItem->m_nStackCount = nAmount;
                pPlaceable->AcquireItem(&pItem, INVALID_OBJECT_ID, 1);
            }
        }
    } else if (pReceiver->m_nObjectType == 6) {
        pCreature->RemoveGold(nAmount, 1);
        if (!bDestroy) {
            CSWSItem* pContainer = pReceiver->AsSWSItem();
            if (pContainer && GetContainerBaseItem(pContainer)->m_bContainer) {
                CItemRepository* pRepository = pContainer->m_pRepository;
                if (pRepository) {
                    CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
                    CResRef cTemplate(g_szCreditsTemplate);
                    pItem->LoadFromTemplate(cTemplate);
                    pItem->m_nStackCount = nAmount;
                    pRepository->AddItem(&pItem, 1, 0, 1);
                }
            }
        }
    } else {
        pCreature->RemoveGold(nAmount, 1);
    }
    if (pCreature->m_bInParty) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(1, -nAmount);
    }
    return 0;
}
