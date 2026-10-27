// CSWVirtualMachineCommands: Get* queries on creatures, areas and areas of
// effect (0x005361B0-0x00536A80).
#include "b/vm_commands.h"
#include "b/sws_creature.h"

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)
extern int ABILITY_STRENGTH;                                // 0x00746838
extern int ABILITY_DEXTERITY;                               // 0x0074683C
extern int ABILITY_CONSTITUTION;                            // 0x00746840
extern int ABILITY_INTELLIGENCE;                            // 0x00746844
extern int ABILITY_WISDOM;                                  // 0x00746848
extern int ABILITY_CHARISMA;                                // 0x0074684C

// Vtable slot 0x50 (object type 0xb).
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x248 - 0xf4];
    unsigned long m_oidCreator;                             // 0x248
};

// FUNCTION: SWKOTOR 0x005361B0
int CSWVirtualMachineCommands::ExecuteCommandGetAbilityScore(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nAbility;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAbility)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        if (nAbility == ABILITY_STRENGTH) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetSTRStat();
        } else if (nAbility == ABILITY_DEXTERITY) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetDEXStat();
        } else if (nAbility == ABILITY_CONSTITUTION) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetCONStat();
        } else if (nAbility == ABILITY_INTELLIGENCE) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetINTStat();
        } else if (nAbility == ABILITY_WISDOM) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetWISStat();
        } else if (nAbility == ABILITY_CHARISMA) {
            nResult = pObject->AsSWSCreature()->m_pStats->GetCHAStat();
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00536310
int CSWVirtualMachineCommands::ExecuteCommandGetAbilityModifier(int nCommandId, int nParameters)
{
    int nAbility;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nAbility)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = 0;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        switch (nAbility) {
        case 0:
            nResult = pCreature->m_pStats->m_nStrengthModifier;
            break;
        case 1:
            {
                CSWSCreatureStats* pStats = pCreature->m_pStats;
                nResult = pStats->GetDEXMod(0);
            }
            break;
        case 2:
            nResult = pCreature->m_pStats->m_nConstitutionModifier;
            break;
        case 3:
            nResult = pCreature->m_pStats->m_nIntelligenceModifier;
            break;
        case 4:
            nResult = pCreature->m_pStats->m_nWisdomModifier;
            break;
        case 5:
            nResult = pCreature->m_pStats->m_nCharismaModifier;
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00536410
int CSWVirtualMachineCommands::ExecuteCommandGetAC(int nCommandId, int nParameters)
{
    unsigned long oidObject = 0;
    int nForFutureUse;
    int nResult = -1;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&nForFutureUse)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        switch (pObject->m_nObjectType) {
        case 5:
            nResult = pObject->AsSWSCreature()->GetArmorClass();
            break;
        case 6:
        case 9:
        case 0xa:
            nResult = 0;
            break;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetAlignmentGoodEvil.
// FUNCTION: SWKOTOR 0x005364F0
int CSWVirtualMachineCommands::ExecuteCommandGetGoodEvilValue(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nResult = -1;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->GetStats();
        switch (nCommandId) {
        case 0x7d:
            nResult = pStats->m_nGoodEvil;
            break;
        case 0x7f:
            nResult = pStats->GetSimpleAlignmentGoodEvil();
            break;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005365A0
int CSWVirtualMachineCommands::ExecuteCommandGetAreaOfEffectCreator(int nCommandId, int nParameters)
{
    unsigned long oidAreaOfEffect;
    if (!g_pVirtualMachine->StackPopObject(&oidAreaOfEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidResult = INVALID_OBJECT_ID;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidAreaOfEffect, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSTrigger()) {
        oidResult = pObject->AsSWSTrigger()->m_oidCreator;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536630
int CSWVirtualMachineCommands::ExecuteCommandGetAppearanceType(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = 0;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        nResult = pCreature->m_pStats->m_nAppearanceType;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005366A0
int CSWVirtualMachineCommands::ExecuteCommandGetArea(int nCommandId, int nParameters)
{
    CExoString sUnused;
    unsigned long oidTarget;
    unsigned long oidResult = OBJECT_INVALID_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->m_pServerExoApp->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        if (pObject->m_nObjectType > OBJECT_TYPE_AREA) {
            CSWSArea* pArea = ((CSWSObject*)pObject)->GetArea();
            if (pArea) {
                oidResult = pArea->GetId();
            }
        } else if (pObject->m_nObjectType == OBJECT_TYPE_AREA) {
            oidResult = pObject->m_idSelf;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005367B0
int CSWVirtualMachineCommands::ExecuteCommandGetAreaUnescapable(int nCommandId, int nParameters)
{
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    int bUnescapable = pPlayer->GetArea()->m_bUnescapable;
    return g_pVirtualMachine->StackPushInteger(bUnescapable) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536800
int CSWVirtualMachineCommands::ExecuteCommandGetAttackTarget(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        oidResult = pCreature->m_oidAttackTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536860
int CSWVirtualMachineCommands::ExecuteCommandGetAttemptedAttackTarget(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        oidResult = pCreature->m_oidAttemptedAttackTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005368B0
int CSWVirtualMachineCommands::ExecuteCommandGetAttemptedSpellTarget(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        oidResult = pCreature->m_oidAttemptedSpellTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536900
int CSWVirtualMachineCommands::ExecuteCommandGetAttemptedMovementTarget(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        oidResult = pCreature->m_oidAttemptedMovementTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536950
int CSWVirtualMachineCommands::ExecuteCommandGetSpellTarget(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        oidResult = pCreature->m_oidSpellTarget;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536A10
int CSWVirtualMachineCommands::ExecuteCommandMusicBackgroundGetBattleTrack(int nCommandId, int nParameters)
{
    unsigned long oidArea;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (pArea) {
        nResult = pArea->m_pAmbientSound->m_nMusicBattle;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00536A80
int CSWVirtualMachineCommands::ExecuteCommandGetBlockingDoor(int nCommandId, int nParameters)
{
    unsigned long oidResult = INVALID_OBJECT_ID;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        oidResult = pCreature->m_oidBlockingDoor;
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
