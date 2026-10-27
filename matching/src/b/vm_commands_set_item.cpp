// CSWVirtualMachineCommands: item, door and placeable setters
// (SetIdentified, SetLocked, SetPlaceableIllumination).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/sws_door.h"

// FUNCTION: SWKOTOR 0x00542D50
int CSWVirtualMachineCommands::ExecuteCommandSetIdentified(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int bIdentified;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bIdentified)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        pItem->SetItemFlag0(bIdentified);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543240
int CSWVirtualMachineCommands::ExecuteCommandSetLocked(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    int bLocked = 0;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bLocked)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->AsSWSDoor()) {
            pObject->AsSWSDoor()->SetLocked(bLocked);
        } else if (pObject->AsSWSPlaceable()) {
            pObject->AsSWSPlaceable()->SetLocked(bLocked);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00543360
int CSWVirtualMachineCommands::ExecuteCommandSetPlaceableIllumination(int nCommandId, int nParameters)
{
    unsigned long oidPlaceable;
    int bIlluminate;
    if (!g_pVirtualMachine->StackPopObject(&oidPlaceable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bIlluminate)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSPlaceable* pPlaceable = g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidPlaceable);
    if (pPlaceable) {
        pPlaceable->SetLightIsOn(bIlluminate);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005465D0
int CSWVirtualMachineCommands::ExecuteCommandGetItemStackSize(int nCommandId, int nParameters)
{
    int nStackSize = 0;
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        nStackSize = pItem->m_nStackCount;
    }
    return g_pVirtualMachine->StackPushInteger(nStackSize) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005469A0
int CSWVirtualMachineCommands::ExecuteCommandSetItemNonEquippable(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int bNonEquippable;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bNonEquippable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        pItem->m_bNonEquippable = bNonEquippable;
    }
    return 0;
}

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned long EVENT_SIGNAL_EVENT;              // 0x007441DC (name guessed)

// FUNCTION: SWKOTOR 0x005439D0
int CSWVirtualMachineCommands::ExecuteCommandSignalEvent(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    CScriptEvent* pEvent;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EVENT, (void**)&pEvent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CScriptEvent* pScriptEvent = pEvent;
    unsigned long oidCaller = 0x7f000000;
    CGameObject* pCaller;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pCaller) == GAME_OBJECT_VALID) {
        oidCaller = m_oidObjectRunScript;
    }
    CGameObject* pTarget;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID) {
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, oidCaller, oidTarget, EVENT_SIGNAL_EVENT, pScriptEvent);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545050
int CSWVirtualMachineCommands::ExecuteCommandActionUnequipItem(int nCommandId, int nParameters)
{
    int bInstant = 0;
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bInstant)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    CGameObject* pItemObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidItem, &pItemObject) == GAME_OBJECT_VALID) {
        pObject->AsSWSCreature()->AddUnequipActions(pItemObject->AsSWSItem(), OBJECT_INVALID_ID, 0, 0, bInstant);
    }
    return 0;
}

// NONMATCH: register choice only (the original builds &pItem and the
// base item index in eax, cl uses ecx/edx).
// FUNCTION: SWKOTOR 0x0053E830
int CSWVirtualMachineCommands::ExecuteCommandGiveItem(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    unsigned long oidGiveTo;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidGiveTo)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidGiveTo);
    if (pItem && pObject) {
        if (pObject->m_nObjectType == 5) {
            pObject->AsSWSCreature()->AcquireItem(&pItem, pItem->GetPossessor(), 0x7f000000, 0);
            return 0;
        }
        if (pObject->m_nObjectType == 9) {
            pObject->AsSWSPlaceable()->AcquireItem(&pItem, pItem->GetPossessor(), 0);
            return 0;
        }
        if (pObject->m_nObjectType == 6 &&
            g_pRules->m_pBaseItemArray->GetBaseItem(pObject->AsSWSItem()->m_nBaseItem)->m_bContainer) {
            pObject->AsSWSItem()->AcquireItem(&pItem, pItem->GetPossessor(), 0);
        }
    }
    return 0;
}

// Destroyable flags of any CSWSObject (+0xec..+0xf4); sws_creature.h keeps
// CSWSObject data-free.
struct CSWSObjectDestroyView {
    char pad_0x0[0xec];
    int m_bDestroyable;                                     // 0xec
    int m_bRaiseable;                                       // 0xf0
    int m_bSelectableWhenDead;                              // 0xf4
};

extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)
extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644

// FUNCTION: SWKOTOR 0x00542DC0
int CSWVirtualMachineCommands::ExecuteCommandSetIsDestroyable(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    int bRaiseable = 1;
    int bSelectable = 0;
    int bDestroyable;
    if (!g_pVirtualMachine->StackPopInteger(&bDestroyable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&bRaiseable)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (!g_pVirtualMachine->StackPopInteger(&bSelectable)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID
        && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        ((CSWSObjectDestroyView*)pObject)->m_bDestroyable = bDestroyable != 0;
        if (((CSWSObjectDestroyView*)pObject)->m_bDestroyable == 1) {
            // Vtable slot 0x94 checks whether this creature is dead.
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature && pCreature->GetObjectState94() && !pCreature->m_bInParty) {
                pCreature->SetLockOrientationToObject(OBJECT_INVALID_ID, 0);
                if (!pCreature->m_bIsPC) {
                    CServerAIMaster* pAIMaster = g_pAppManager->GetServerExoApp()->GetServerAIMaster();
                    pAIMaster->AddEventDeltaTime(0, 3000, pCreature->m_oid154, pCreature->m_idSelf, EVENT_DESTROY_OBJECT, NULL);
                }
            }
        }
        ((CSWSObjectDestroyView*)pObject)->m_bRaiseable = bRaiseable;
        ((CSWSObjectDestroyView*)pObject)->m_bSelectableWhenDead = bSelectable;
    }
    return 0;
}
