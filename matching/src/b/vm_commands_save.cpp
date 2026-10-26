// CSWVirtualMachineCommands: saving throw, difficulty and gold queries
// (0x00538BE0-0x0053916A, 0x0053CE10, 0x0053E540).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/server_info.h"

#define INVALID_OBJECT_ID 0x7f000000

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634 (5)
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638 (9)
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639 (10)

class CSWSDoor : public CSWSObject {
public:
    char pad_0xf4[0x2bc - 0xf4];
    unsigned char m_nFortSave;                              // 0x2bc
    unsigned char m_nReflexSave;                            // 0x2bd
    unsigned char m_nWillSave;                              // 0x2be
};

class CItemRepository {
public:
    unsigned long FindItemWithBaseItemType(unsigned char nType, int nNth); // 0x00555E00
};

class CSWSPlaceable : public CSWSObject {
public:
    char pad_0xf4[0x314 - 0xf4];
    unsigned char m_nFortSave;                              // 0x314
    unsigned char m_nWillSave;                              // 0x315
    unsigned char m_nReflexSave;                            // 0x316
    char pad_0x317[0x36c - 0x317];
    CItemRepository* m_pItemRepository;                     // 0x36c
};

class CSWSItem {
public:
    unsigned long GetCost();                                // 0x00554000

    char pad_0x0[0x28c];
    int m_nStackCount;                                      // 0x28c
};

// FUNCTION: SWKOTOR 0x00538BE0
int CSWVirtualMachineCommands::ExecuteCommandGetFortitudeSavingThrow(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    int nSave = 0;
    if (pObject) {
        if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature && pCreature->m_pStats) {
                nSave = pCreature->m_pStats->GetFortSavingThrow(0);
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
            CSWSDoor* pDoor = pObject->AsSWSDoor();
            if (pDoor) {
                nSave = pDoor->m_nFortSave;
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
            if (pPlaceable) {
                nSave = pPlaceable->m_nFortSave;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nSave) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00538CA0
int CSWVirtualMachineCommands::ExecuteCommandGetGameDifficulty(int nCommandId, int nParameters)
{
    int nDifficulty = g_pAppManager->GetServerExoApp()->GetServerInfo()->m_nDifficulty;
    return g_pVirtualMachine->StackPushInteger(nDifficulty) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// The original swaps the two error codes here.
// FUNCTION: SWKOTOR 0x00538FC0
int CSWVirtualMachineCommands::ExecuteCommandGetGoingToBeAttackedBy(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    unsigned long oidAttacker = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pCreature) {
        oidAttacker = pCreature->m_oidGoingToBeAttackedBy;
    }
    return g_pVirtualMachine->StackPushObject(oidAttacker) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
}

// FUNCTION: SWKOTOR 0x00539030
int CSWVirtualMachineCommands::ExecuteCommandGetGold(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nGold = 0;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pCreature) {
        nGold = pCreature->GetGold();
    } else {
        CSWSPlaceable* pPlaceable = g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidTarget);
        if (pPlaceable && pPlaceable->m_pItemRepository) {
            unsigned long oidGold = pPlaceable->m_pItemRepository->FindItemWithBaseItemType(0x17, 0);
            CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidGold);
            if (pItem) {
                nGold = pItem->m_nStackCount;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nGold) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005390E0
int CSWVirtualMachineCommands::ExecuteCommandGetGoldPieceValue(int nCommandId, int nParameters)
{
    int nValue = 0;
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidItem, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSItem()) {
        nValue = pObject->AsSWSItem()->GetCost();
    }
    return g_pVirtualMachine->StackPushInteger(nValue) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053CE10
int CSWVirtualMachineCommands::ExecuteCommandGetReflexSavingThrow(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    int nSave = 0;
    if (pObject) {
        if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature && pCreature->m_pStats) {
                nSave = pCreature->m_pStats->GetReflexSavingThrow(0);
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
            CSWSDoor* pDoor = pObject->AsSWSDoor();
            if (pDoor) {
                nSave = pDoor->m_nReflexSave;
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
            if (pPlaceable) {
                nSave = pPlaceable->m_nReflexSave;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nSave) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E540
int CSWVirtualMachineCommands::ExecuteCommandGetWillSavingThrow(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    int nSave = 0;
    if (pObject) {
        if (pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature && pCreature->m_pStats) {
                nSave = pCreature->m_pStats->GetWillSavingThrow(0);
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
            CSWSDoor* pDoor = pObject->AsSWSDoor();
            if (pDoor) {
                nSave = pDoor->m_nWillSave;
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
            if (pPlaceable) {
                nSave = pPlaceable->m_nWillSave;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nSave) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
