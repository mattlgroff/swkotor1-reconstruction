// CSWVirtualMachineCommands: small effect builders, script events and dice.
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/party_table.h"
#include "b/global_variable_table.h"

// FUNCTION: SWKOTOR 0x00531EF0
int CSWVirtualMachineCommands::ExecuteCommandEffectDamageImmunity(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE;
    int nDamageType;
    int nPercent;
    if (!g_pVirtualMachine->StackPopInteger(&nDamageType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nPercent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nPercent <= 100) {
        if (nPercent < -100) {
            nPercent = -100;
        }
    } else {
        nPercent = 100;
    }
    if (nDamageType < 0 || nDamageType > DAMAGE_TYPE_MAX) {
        nDamageType = DAMAGE_TYPE_DEFAULT;
    }
    pEffect->SetSubType_Supernatural();
    pEffect->SetCreator(m_oidObjectRunScript);
    pEffect->SetInteger(0, nDamageType);
    pEffect->SetInteger(1, nPercent);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00533660
int CSWVirtualMachineCommands::ExecuteCommandEffectResurrection(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_RESURRECTION;
    pEffect->SetSubType_Supernatural();
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

// FUNCTION: SWKOTOR 0x00534810
int CSWVirtualMachineCommands::ExecuteCommandEffectDroidStun(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SETSTATE;
    pEffect->SetInteger(0, 3);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005345E0
int CSWVirtualMachineCommands::ExecuteCommandEffectTrueSeeing(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_TRUESEEING;
    pEffect->SetCreator(m_oidObjectRunScript);
    pEffect->SetSubType_Supernatural();
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005346B0
int CSWVirtualMachineCommands::ExecuteCommandEffectVisualEffect(int nCommandId, int nParameters)
{
    int nVisualEffect;
    int bMissEffect = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nVisualEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&bMissEffect)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_VISUALEFFECT;
    pEffect->SetSubType_Supernatural();
    pEffect->SetInteger(0, nVisualEffect);
    pEffect->SetInteger(2, bMissEffect);
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

// FUNCTION: SWKOTOR 0x005348C0
int CSWVirtualMachineCommands::ExecuteCommandEffectForceJump(int nCommandId, int nParameters)
{
    unsigned long oidTarget = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nAdvanced = 0;
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nAdvanced)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_FORCE_JUMP;
    pEffect->SetObjectID(0, oidTarget);
    pEffect->SetInteger(0, nAdvanced);
    pEffect->SetCreator(m_oidObjectRunScript);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005349F0
int CSWVirtualMachineCommands::ExecuteCommandEffectForcePushed(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_FORCE_PUSHED;
    pEffect->SetCreator(m_oidObjectRunScript);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00534E90
int CSWVirtualMachineCommands::ExecuteCommandEffectHorrified(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SETSTATE;
    pEffect->SetInteger(0, 8);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005351E0
int CSWVirtualMachineCommands::ExecuteCommandEffectChoke(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SETSTATE;
    pEffect->SetInteger(0, 7);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535290
int CSWVirtualMachineCommands::ExecuteCommandEffectBodyFuel(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_BODY_FUEL;
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535340
int CSWVirtualMachineCommands::ExecuteCommandEffectPsychicStatic(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_PSYCHIC_STATIC;
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005358C0
int CSWVirtualMachineCommands::ExecuteCommandEventConversation(int nCommandId, int nParameters)
{
    CScriptEvent* pEvent = new CScriptEvent;
    pEvent->m_nType = 7;
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EVENT, pEvent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEvent;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535960
int CSWVirtualMachineCommands::ExecuteCommandEventSpellCastAt(int nCommandId, int nParameters)
{
    CScriptEvent* pEvent = new CScriptEvent;
    unsigned long oidCaster;
    int nSpell;
    int bHarmful = 1;
    if (!g_pVirtualMachine->StackPopObject(&oidCaster)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 2) {
        if (!g_pVirtualMachine->StackPopInteger(&bHarmful)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    pEvent->m_nType = 2;
    pEvent->SetInteger(0, nSpell);
    pEvent->SetInteger(1, bHarmful);
    pEvent->SetObjectID(0, oidCaster);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EVENT, pEvent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEvent;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535A90
int CSWVirtualMachineCommands::ExecuteCommandEventUserDefined(int nCommandId, int nParameters)
{
    CScriptEvent* pEvent = new CScriptEvent;
    int nEventNumber;
    if (!g_pVirtualMachine->StackPopInteger(&nEventNumber)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    pEvent->m_nType = 11;
    pEvent->SetInteger(0, nEventNumber);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EVENT, pEvent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEvent;
    return 0;
}

// FUNCTION: SWKOTOR 0x005420B0
int CSWVirtualMachineCommands::ExecuteCommandRollDice(int nCommandId, int nParameters)
{
    int nNumDice = 1;
    int nDieType;
    switch (nCommandId) {
    case 0x5f: nDieType = 2; break;
    case 0x60: nDieType = 3; break;
    case 0x61: nDieType = 4; break;
    case 0x62: nDieType = 6; break;
    case 0x63: nDieType = 8; break;
    case 0x64: nDieType = 10; break;
    case 0x65: nDieType = 12; break;
    case 0x66: nDieType = 20; break;
    case 0x67: nDieType = 100; break;
    }
    if (nParameters == 1) {
        if (!g_pVirtualMachine->StackPopInteger(&nNumDice)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    if (nNumDice <= 1) {
        nNumDice = 1;
    }
    int nResult = g_pRules->RollDice((unsigned char)nNumDice, (unsigned char)nDieType);
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x005421A0
int CSWVirtualMachineCommands::ExecuteCommandSaveNPCState(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SaveMember(nNPC, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x005474F0
int CSWVirtualMachineCommands::ExecuteCommandGetDifficultyModifier(int nCommandId, int nParameters)
{
    float fModifier = 1.0f;
    int nDifficulty = g_pAppManager->GetClientExoApp()->GetOptions()->m_nDifficulty;
    g_pRules->Get2DArrays()->GetDifficultyOptions()->GetFLOATEntry(nDifficulty, "MULTIPLIER", &fModifier);
    return g_pVirtualMachine->StackPushFloat(fModifier) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00531260
int CSWVirtualMachineCommands::ExecuteCommandEffectBeam(int nCommandId, int nParameters)
{
    int nBeamVisualEffect;
    int bMissEffect = 0;
    int nBodyPart;
    unsigned long oidEffector;
    if (!g_pVirtualMachine->StackPopInteger(&nBeamVisualEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidEffector)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nBodyPart)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bMissEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    if (!IsValidBeamVisualEffect(nBeamVisualEffect)) {
        pEffect->m_nType = EFFECT_TYPE_INVALIDEFFECT;
    } else {
        pEffect->m_nType = EFFECT_TYPE_BEAM;
        pEffect->SetCreator(m_oidObjectRunScript);
        pEffect->SetInteger(0, nBeamVisualEffect);
        pEffect->SetInteger(1, nBodyPart);
        pEffect->SetInteger(2, bMissEffect);
        pEffect->SetObjectID(0, oidEffector);
        pEffect->SetSubType_Supernatural();
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00533760
int CSWVirtualMachineCommands::ExecuteCommandEffectSavingThrowDecrease(int nCommandId, int nParameters)
{
    int nSave;
    int nSaveType = 0;
    int nValue;
    CGameObject* pObject;
    if (!g_pVirtualMachine->StackPopInteger(&nSave)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nSave < 0 || nSave >= 4) {
        nSave = 0;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nSaveType)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SAVING_THROW_DECREASE;
    pEffect->SetSubType_Supernatural();
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(1, nSave);
    pEffect->SetInteger(2, nSaveType);
    pEffect->SetInteger(3, g_pRules->m_nAA);
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

// FUNCTION: SWKOTOR 0x00533910
int CSWVirtualMachineCommands::ExecuteCommandEffectSavingThrowIncrease(int nCommandId, int nParameters)
{
    int nSave;
    int nSaveType = 0;
    int nValue;
    CGameObject* pObject;
    if (!g_pVirtualMachine->StackPopInteger(&nSave)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nSave < 0 || nSave >= 4) {
        nSave = 0;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nSaveType)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SAVING_THROW_INCREASE;
    pEffect->SetSubType_Supernatural();
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(1, nSave);
    pEffect->SetInteger(2, nSaveType);
    pEffect->SetInteger(3, g_pRules->m_nAA);
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

// FUNCTION: SWKOTOR 0x00534AB0
int CSWVirtualMachineCommands::ExecuteCommandEffectForcePushTargeted(int nCommandId, int nParameters)
{
    CScriptLocation* pLocation = NULL;
    int nAnimation = 0;
    if (!g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAnimation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_FORCE_PUSHED;
    pEffect->SetCreator(m_oidObjectRunScript);
    pEffect->SetInteger(0, 1);
    pEffect->SetInteger(1, nAnimation);
    pEffect->SetFloat(0, pLocation->m_vPosition[0]);
    pEffect->SetFloat(1, pLocation->m_vPosition[1]);
    pEffect->SetFloat(2, pLocation->m_vPosition[2]);
    delete pLocation;
    pLocation = NULL;
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// Effect type words used only here.
extern const unsigned short EFFECT_TYPE_2F;                 // 0x00746738 (0x2F)
extern const unsigned short EFFECT_TYPE_5B;                 // 0x007467E4 (0x5B)
extern const unsigned short EFFECT_TYPE_5C;                 // 0x007467E8 (0x5C)
extern const unsigned short EFFECT_TYPE_5D;                 // 0x007467EC (0x5D)
extern const unsigned short EFFECT_TYPE_5F;                 // 0x007467F4 (0x5F)
extern const unsigned short EFFECT_TYPE_60;                 // 0x007467F8 (0x60)
extern const unsigned short EFFECT_TYPE_LIGHTSABERTHROW;    // 0x00746808 (0x64)

// CSWSItem's possessor id (+0x9c); party_table.h only has a stub CSWSItem.
struct CSWSItemPossessorView {
    char pad_0x0[0x9c];
    unsigned long m_oidPossessor;                           // 0x9c
};

class CScriptTalent {
public:
    CScriptTalent();                                        // 0x004CA9E0
    ~CScriptTalent();                                       // 0x004015A0 (shared empty function)

    int m_nType;                                            // 0x00
    int m_nId;                                              // 0x04
    unsigned char m_nMultiClass;                            // 0x08
    unsigned long m_oidItem;                                // 0x0c
    int m_nItemPropertyIndex;                               // 0x10
    unsigned char m_nCasterLevel;                           // 0x14
    unsigned char m_nMetaType;                              // 0x15
};

// FUNCTION: SWKOTOR 0x00532E40
int CSWVirtualMachineCommands::ExecuteCommandEffectType2F(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    if (nValue != 1 && nValue != 2 && nValue != 4) {
        pEffect->m_nType = EFFECT_TYPE_INVALIDEFFECT;
    } else {
        pEffect->SetCreator(m_oidObjectRunScript);
        pEffect->m_nType = EFFECT_TYPE_2F;
        pEffect->m_nSubType = (pEffect->m_nSubType & 0xffef) | 0x8;
        pEffect->SetInteger(0, nValue);
        pEffect->SetInteger(1, g_pRules->m_nAA);
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00534CD0
int CSWVirtualMachineCommands::ExecuteCommandEffectType5C(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_5C;
    pEffect->SetInteger(0, nValue);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00534DB0
int CSWVirtualMachineCommands::ExecuteCommandEffectType5D(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_5D;
    pEffect->SetInteger(0, nValue);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00534F40
int CSWVirtualMachineCommands::ExecuteCommandEffectType5F(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_5F;
    pEffect->SetInteger(0, nValue);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535020
int CSWVirtualMachineCommands::ExecuteCommandEffectType60(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_60;
    pEffect->SetInteger(0, nValue);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535100
int CSWVirtualMachineCommands::ExecuteCommandEffectType5B(int nCommandId, int nParameters)
{
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_5B;
    pEffect->SetInteger(0, nValue);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x005353F0
int CSWVirtualMachineCommands::ExecuteCommandEffectLightsaberThrow(int nCommandId, int nParameters)
{
    unsigned long oidTarget1 = INVALID_OBJECT_ID;
    unsigned long oidTarget2 = INVALID_OBJECT_ID;
    unsigned long oidTarget3 = INVALID_OBJECT_ID;
    int nAdvancedDamage = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget1)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopObject(&oidTarget2)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopObject(&oidTarget3)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopInteger(&nAdvancedDamage)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_LIGHTSABERTHROW;
    pEffect->SetObjectID(0, oidTarget1);
    pEffect->SetObjectID(1, oidTarget2);
    pEffect->SetObjectID(2, oidTarget3);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00535710
int CSWVirtualMachineCommands::ExecuteCommandEventActivateItem(int nCommandId, int nParameters)
{
    CScriptEvent* pEvent = new CScriptEvent;
    unsigned long oidPossessor = INVALID_OBJECT_ID;
    unsigned long oidItem;
    CScriptLocation* pLocation = NULL;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidItem) ||
        !g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation) ||
        !g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItemPossessorView* pItem = (CSWSItemPossessorView*)g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        oidPossessor = pItem->m_oidPossessor;
    }
    pEvent->m_nType = 0x12;
    pEvent->SetObjectID(0, oidItem);
    pEvent->SetObjectID(1, m_oidObjectRunScript);
    pEvent->SetObjectID(2, oidPossessor);
    pEvent->SetObjectID(3, oidTarget);
    pEvent->SetFloat(0, pLocation->m_vPosition[0]);
    pEvent->SetFloat(1, pLocation->m_vPosition[1]);
    pEvent->SetFloat(2, pLocation->m_vPosition[2]);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EVENT, pEvent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEvent;
    if (pLocation) {
        delete pLocation;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00548360
void CSWVirtualMachineCommands::DestroyGameDefinedStructure(int nType, void* pStructure)
{
    if (nType == 0) {
        if ((CGameEffect*)pStructure) {
            delete (CGameEffect*)pStructure;
        }
    } else if (nType == 1) {
        if ((CScriptEvent*)pStructure) {
            delete (CScriptEvent*)pStructure;
        }
    } else if (nType == 2) {
        if ((CScriptLocation*)pStructure) {
            delete (CScriptLocation*)pStructure;
        }
    } else if (nType == 3) {
        if ((CScriptTalent*)pStructure) {
            delete (CScriptTalent*)pStructure;
        }
    }
}

// FUNCTION: SWKOTOR 0x00534C20
int CSWVirtualMachineCommands::ExecuteCommandEffectWhirlWind(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_SETSTATE;
    pEffect->SetInteger(0, 10);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00544C70
int CSWVirtualMachineCommands::ExecuteCommandTalent(int nCommandId, int nParameters)
{
    CScriptTalent* pTalent = new CScriptTalent;
    int nId;
    if (!g_pVirtualMachine->StackPopInteger(&nId)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId == 0x12d) {
        pTalent->m_nType = 0;
    } else if (nCommandId == 0x12e) {
        pTalent->m_nType = 1;
    } else if (nCommandId == 0x12f) {
        pTalent->m_nType = 2;
    }
    pTalent->m_nId = nId;
    if (!g_pVirtualMachine->StackPushEngineStructure(3, pTalent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pTalent;
    return 0;
}

extern const unsigned short EFFECT_TYPE_IMMUNITY;           // 0x007466D4 (0x16)
extern const int IMMUNITY_TYPE_MAX;                         // 0x00746978 (0x21)

#define B_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define B_MIN(a, b) (((a) < (b)) ? (a) : (b))

// FUNCTION: SWKOTOR 0x00532D00
int CSWVirtualMachineCommands::ExecuteCommandEffectImmunity(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_IMMUNITY;
    pEffect->SetCreator(m_oidObjectRunScript);
    pEffect->m_nSubType = (pEffect->m_nSubType & 0xffef) | 0x8;
    int nImmunityType;
    if (!g_pVirtualMachine->StackPopInteger(&nImmunityType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    nImmunityType = B_MAX(0, B_MIN(IMMUNITY_TYPE_MAX, nImmunityType));
    pEffect->SetInteger(0, nImmunityType);
    pEffect->SetInteger(1, g_pRules->m_nAA);
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}

// FUNCTION: SWKOTOR 0x00544D70
int CSWVirtualMachineCommands::ExecuteCommandGetTalentValue(int nCommandId, int nParameters)
{
    CScriptTalent* pTalent;
    if (!g_pVirtualMachine->StackPopEngineStructure(3, (void**)&pTalent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bSuccess = 1;
    int nValue = 0;
    switch (nCommandId) {
    case 0x16a:
        bSuccess = g_pVirtualMachine->StackPushInteger(pTalent->m_nType);
        break;
    case 0x16b:
        bSuccess = g_pVirtualMachine->StackPushInteger(pTalent->m_nId);
        break;
    case 0x2df:
        if (pTalent->m_nType == 0) {
            int nSpell = pTalent->m_nId;
            CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
            if (pSpell) {
                nValue = pSpell->m_n124;
            }
        } else if (pTalent->m_nType == 1) {
            CSWFeat* pFeat = g_pRules->GetFeat((unsigned short)pTalent->m_nId);
            if (pFeat) {
                nValue = pFeat->m_n10;
            }
        }
        bSuccess = g_pVirtualMachine->StackPushInteger(nValue);
        break;
    }
    if (pTalent) {
        delete pTalent;
    }
    return bSuccess ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

extern const int EFFECT_TYPE_INVALIDEFFECT_DWORD;           // 0x0074667C (0, compared as a dword)
extern const unsigned short DURATION_TYPE_7473B8;           // 0x007473B8
extern const unsigned short DURATION_TYPE_7473BC;           // 0x007473BC
extern const unsigned short DURATION_TYPE_7473C0;           // 0x007473C0
extern const unsigned short DURATION_TYPE_7473C4;           // 0x007473C4
extern const unsigned short DURATION_TYPE_7473C8;           // 0x007473C8

// FUNCTION: SWKOTOR 0x0053EE00
int CSWVirtualMachineCommands::ExecuteCommandGetIsEffectValid(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = NULL;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bValid = 1;
    if (pEffect == NULL || pEffect->m_nType == EFFECT_TYPE_INVALIDEFFECT_DWORD || pEffect->m_n20 == 0) {
        bValid = 0;
    }
        if (pEffect->m_nDurationType != DURATION_TYPE_7473B8 && pEffect->m_nDurationType != DURATION_TYPE_7473C0 &&
        pEffect->m_nDurationType != DURATION_TYPE_7473C4 && pEffect->m_nDurationType != DURATION_TYPE_7473BC &&
        pEffect->m_nDurationType != DURATION_TYPE_7473C8) {
        bValid = 0;
    }
    if (!g_pVirtualMachine->StackPushInteger(bValid)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}
