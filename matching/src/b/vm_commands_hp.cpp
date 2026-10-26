// CSWVirtualMachineCommands: hit dice, hit/force points, inventory disturb
// and time-of-day queries (0x00539570-0x00539DDB).
#include "b/vm_commands.h"
#include "b/sws_creature.h"

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)
extern const int IMMUNITY_TYPE_MAX;                         // 0x00746978 (0x21)

// Inventory-disturb fields of CSWSObject (CSWSObject itself stays data-free
// in sws_creature.h).
struct CSWSObjectDisturbView {
    char pad_0x0[0x174];
    unsigned long m_oidInventoryDisturbItem;                // 0x174
    int m_nInventoryDisturbType;                            // 0x178
};

// FUNCTION: SWKOTOR 0x00539570
int CSWVirtualMachineCommands::ExecuteCommandGetHitDice(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    int nHitDice = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature) {
            CSWSCreatureStats* pStats = pCreature->m_pStats;
            nHitDice = pStats->GetLevel(0);
        }
    }
    return g_pVirtualMachine->StackPushInteger(nHitDice) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetMaxHitPoints (vtable slot 0x98, GetMaxHitPoints in buckets a and k).
// FUNCTION: SWKOTOR 0x00539610
int CSWVirtualMachineCommands::ExecuteCommandGetCurrentHitPoints(int nCommandId, int nParameters)
{
    int nHitPoints = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        if (nCommandId == 0x31) {
            if (pObject->AsSWSCreature()) {
                nHitPoints = pObject->AsSWSCreature()->GetCurrentHitPoints(0);
            } else {
                nHitPoints = pObject->AsSWSObject()->GetCurrentHitPoints(0);
            }
        } else {
            if (pObject->AsSWSCreature()) {
                nHitPoints = pObject->AsSWSCreature()->GetCreatureSize(1);
            } else {
                nHitPoints = pObject->AsSWSObject()->GetCreatureSize(1);
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nHitPoints) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005396F0
int CSWVirtualMachineCommands::ExecuteCommandGetCurrentForcePoints(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidObject);
    int nForcePoints = 0;
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        nForcePoints = (short)(pStats->m_nCurrentForcePoints + pStats->m_nForcePointsBonus);
    }
    return g_pVirtualMachine->StackPushInteger(nForcePoints) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539760
int CSWVirtualMachineCommands::ExecuteCommandGetMaxForcePoints(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidObject);
    int nForcePoints = 0;
    if (pCreature) {
        nForcePoints = pCreature->GetMaxForcePoints();
    }
    return g_pVirtualMachine->StackPushInteger(nForcePoints) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// The original reports every item as identified.
// FUNCTION: SWKOTOR 0x005397C0
int CSWVirtualMachineCommands::ExecuteCommandGetIdentified(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bIdentified = 0;
    if (g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem)) {
        bIdentified = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bIdentified) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539820
int CSWVirtualMachineCommands::ExecuteCommandGetInventoryDisturbItem(int nCommandId, int nParameters)
{
    unsigned long oidItem = OBJECT_INVALID_ID;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject) {
            oidItem = ((CSWSObjectDisturbView*)pSWSObject)->m_oidInventoryDisturbItem;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidItem) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539870
int CSWVirtualMachineCommands::ExecuteCommandGetInventoryDisturbType(int nCommandId, int nParameters)
{
    int nType = 0;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject) {
            nType = ((CSWSObjectDisturbView*)pSWSObject)->m_nInventoryDisturbType;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nType) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005398C0
int CSWVirtualMachineCommands::ExecuteCommandGetIsDawn(int nCommandId, int nParameters)
{
    int bResult = 0;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule && pModule->m_nTimeOfDay == 3) {
        bResult = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539900
int CSWVirtualMachineCommands::ExecuteCommandGetIsDay(int nCommandId, int nParameters)
{
    int bResult = 0;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule && pModule->m_nTimeOfDay == 1) {
        bResult = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539DE0
int CSWVirtualMachineCommands::ExecuteCommandGetIsNight(int nCommandId, int nParameters)
{
    int bResult = 0;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule && pModule->m_nTimeOfDay == 2) {
        bResult = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539C10
int CSWVirtualMachineCommands::ExecuteCommandGetIsEncounterCreature(int nCommandId, int nParameters)
{
    int bResult = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature && pCreature->m_oidEncounter != 0x7f000000) {
        bResult = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539C80
int CSWVirtualMachineCommands::ExecuteCommandGetIsImmune(int nCommandId, int nParameters)
{
    int bImmune = 0;
    unsigned long oidCreature;
    int nImmunityType;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nImmunityType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidVersus = 0x7f000000;
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopObject(&oidVersus)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pVersus = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidVersus);
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && nImmunityType > 0 && nImmunityType < IMMUNITY_TYPE_MAX) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        bImmune = pStats->GetEffectImmunity((unsigned char)nImmunityType, pVersus);
    }
    if (!g_pVirtualMachine->StackPushInteger(bImmune)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00539D80
int CSWVirtualMachineCommands::ExecuteCommandGetIsInCombat(int nCommandId, int nParameters)
{
    int bInCombat = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        bInCombat = pCreature->m_n4E0;
    }
    return g_pVirtualMachine->StackPushInteger(bInCombat) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
