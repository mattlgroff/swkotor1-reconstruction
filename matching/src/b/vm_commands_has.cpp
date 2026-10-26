// CSWVirtualMachineCommands: GetHas* queries (0x00539170-0x0053956C).
#include "b/vm_commands.h"
#include "b/sws_creature.h"

class CSWSPlaceable : public CSWSObject {
public:
    char pad_0xf4[0x324 - 0xf4];
    int m_bHasInventory;                                    // 0x324
};

class CSWSItem {
public:
    char pad_0x0[0x26c];
    class CItemRepository* m_pItemRepository;               // 0x26c (containers only)
};

// FUNCTION: SWKOTOR 0x00539170
int CSWVirtualMachineCommands::ExecuteCommandGetHasFeat(int nCommandId, int nParameters)
{
    int bHasFeat = 0;
    int nFeat;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nFeat)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        bHasFeat = pStats->GetFeatRemainingUses((unsigned short)nFeat) > 0;
    }
    if (!g_pVirtualMachine->StackPushInteger(bHasFeat)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00539230
int CSWVirtualMachineCommands::ExecuteCommandGetHasFeatEffect(int nCommandId, int nParameters)
{
    int bHasEffect = 0;
    int nFeat;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopInteger(&nFeat)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSObject()) {
        bHasEffect = ((CSWSObject*)pObject)->GetHasFeatEffectApplied((unsigned short)nFeat);
    }
    if (!g_pVirtualMachine->StackPushInteger(bHasEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005392D0
int CSWVirtualMachineCommands::ExecuteCommandGetHasInventory(int nCommandId, int nParameters)
{
    int bHasInventory = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        if (pObject->AsSWSCreature() || pObject->AsSWSStore()) {
            bHasInventory = 1;
        } else if (pObject->AsSWSPlaceable()) {
            bHasInventory = pObject->AsSWSPlaceable()->m_bHasInventory;
        } else if (pObject->AsSWSItem() && pObject->AsSWSItem()->m_pItemRepository) {
            bHasInventory = 1;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bHasInventory) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539380
int CSWVirtualMachineCommands::ExecuteCommandGetHasSkill(int nCommandId, int nParameters)
{
    int bHasSkill = 0;
    int nSkill;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nSkill)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        if (pStats->GetCanUseSkill((unsigned char)nSkill) == 1) {
            bHasSkill = 1;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(bHasSkill)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00539440
int CSWVirtualMachineCommands::ExecuteCommandGetHasSpell(int nCommandId, int nParameters)
{
    int bHasSpell = 0;
    int nSpell;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        bHasSpell = pStats->HasSpell(0, nSpell, 1);
    }
    if (!g_pVirtualMachine->StackPushInteger(bHasSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005394D0
int CSWVirtualMachineCommands::ExecuteCommandGetHasSpellEffect(int nCommandId, int nParameters)
{
    int bHasEffect = 0;
    int nSpell;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopInteger(&nSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSObject()) {
        bHasEffect = ((CSWSObject*)pObject)->GetHasSpellEffectApplied(nSpell);
    }
    if (!g_pVirtualMachine->StackPushInteger(bHasEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
