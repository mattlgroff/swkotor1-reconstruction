// CSWVirtualMachineCommands: EffectDamage, GetReflexAdjustedDamage and
// EffectAreaOfEffect (round s8).
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/sws_creature.h"

extern const unsigned short EFFECT_TYPE_DAMAGE;             // 0x00746714
extern const int DAMAGE_POWER_NORMAL;                       // 0x00746894 (0)
extern const int DAMAGE_POWER_MAX;                          // 0x007468AC (6, name guessed)
extern const unsigned short EFFECT_TYPE_AREA_OF_EFFECT;     // 0x007466F8
extern const unsigned short DURATION_TYPE_INSTANT;          // 0x007473B8 (0)
extern int g_nDamageIndex;                                  // 0x008327E0
extern const float g_fInvLog10Of2;                          // 0x00745FF8

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

extern "C" double __cdecl log10(double x);
#pragma intrinsic(log10)

// FUNCTION: SWKOTOR 0x00531AE0
int CSWVirtualMachineCommands::ExecuteCommandEffectDamage(int nCommandId, int nParameters)
{
    int nDamageType;
    int nDamageAmount;
    int nDamagePower;
    if (!g_pVirtualMachine->StackPopInteger(&nDamageAmount)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nDamageType)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        nDamageType = 3;
    }
    if (nParameters >= 3) {
        if (!g_pVirtualMachine->StackPopInteger(&nDamagePower)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        nDamagePower = DAMAGE_POWER_NORMAL;
    }
    if (nDamageAmount < 0 || nDamageAmount > 10000) {
        nDamageAmount = 1;
    }
    if (nDamageType < 0 || nDamageType > DAMAGE_TYPE_MAX) {
        nDamageType = DAMAGE_TYPE_DEFAULT;
    }
    if (nDamagePower < 0 || nDamagePower > DAMAGE_POWER_MAX) {
        nDamagePower = DAMAGE_POWER_NORMAL;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nSubType = (pEffect->m_nSubType & 0xffef) | 0x8;
    pEffect->m_nType = EFFECT_TYPE_DAMAGE;
    pEffect->SetNumIntegers(0x15);
    for (int i = 0; i < 15; i++) {
        pEffect->SetInteger(i, -1);
    }
    FloatToInt(&g_nDamageIndex, log10((double)nDamageType) * g_fInvLog10Of2);
    pEffect->SetInteger(g_nDamageIndex, nDamageAmount);
    pEffect->SetInteger(0xe, nDamageAmount);
    pEffect->SetInteger(0x10, 1000);
    pEffect->SetInteger(0x11, nDamageType);
    pEffect->SetInteger(0x12, nDamagePower);
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        pEffect->SetCreator(m_oidObjectRunScript);
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// NONMATCH (20): the two pop paths merge after the last push in the original.
// FUNCTION: SWKOTOR 0x0053CC30
int CSWVirtualMachineCommands::ExecuteCommandGetReflexAdjustedDamage(int nCommandId, int nParameters)
{
    unsigned long oidSaveVersus;
    unsigned long oidTarget;
    int nSaveType;
    int nDC;
    int nDamage;
    if (nParameters == 4) {
        if (!g_pVirtualMachine->StackPopObject(&oidTarget) || !g_pVirtualMachine->StackPopInteger(&nDamage) ||
            !g_pVirtualMachine->StackPopInteger(&nSaveType) || !g_pVirtualMachine->StackPopObject(&oidSaveVersus)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        if (!g_pVirtualMachine->StackPopInteger(&nDamage) || !g_pVirtualMachine->StackPopObject(&oidTarget) ||
            !g_pVirtualMachine->StackPopInteger(&nDC) || !g_pVirtualMachine->StackPopInteger(&nSaveType) ||
            !g_pVirtualMachine->StackPopObject(&oidSaveVersus)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pCreature && pCreature->SavingThrowRoll(2, (unsigned short)nDC, (unsigned short)nSaveType, oidSaveVersus, 1, 0, 0)) {
        nDamage = nDamage / 2;
        CGameEffect* pEffect = new CGameEffect(1);
        pEffect->SetDurationType(DURATION_TYPE_INSTANT);
        pEffect->SetCreator(oidSaveVersus);
        pEffect->m_nType = EFFECT_TYPE_VISUALEFFECT;
        pEffect->SetInteger(0, 4000);
        pCreature->ApplyEffect(pEffect, 0, 0);
    }
    return g_pVirtualMachine->StackPushInteger(nDamage) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00530CE0
int CSWVirtualMachineCommands::ExecuteCommandEffectAreaOfEffect(int nCommandId, int nParameters)
{
    CExoString sOnEnterScript;
    CExoString sHeartbeatScript;
    CExoString sOnExitScript;
    int nAreaEffectId;
    if (!g_pVirtualMachine->StackPopInteger(&nAreaEffectId) || !g_pVirtualMachine->StackPopString(&sOnEnterScript) ||
        !g_pVirtualMachine->StackPopString(&sHeartbeatScript) || !g_pVirtualMachine->StackPopString(&sOnExitScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_AREA_OF_EFFECT;
    pEffect->m_nSubType = (pEffect->m_nSubType & 0xffef) | 0x8;
    pEffect->SetInteger(0, nAreaEffectId);
    pEffect->SetFloat(0, 0.0f);
    pEffect->SetFloat(1, 0.0f);
    pEffect->SetFloat(2, 0.0f);
    pEffect->SetString(0, sOnEnterScript);
    pEffect->SetString(1, sHeartbeatScript);
    pEffect->SetString(2, sOnExitScript);
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        pEffect->SetCreator(m_oidObjectRunScript);
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}
