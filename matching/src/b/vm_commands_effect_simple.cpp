// CSWVirtualMachineCommands::ExecuteCommandEffectAssuredHit (round s9): the
// shared handler of the simple effect constructors (EffectAssuredHit,
// EffectEntangle, EffectDeath, EffectKnockdown, the set-state effects,
// EffectRegenerate, EffectHaste, EffectDisguise, EffectTimeStop, ...).
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/server_exo_app.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const unsigned short EFFECT_TYPE_HASTE;              // 0x00746680 (1)
extern const unsigned short EFFECT_TYPE_REGENERATE;         // 0x00746698 (7)
extern const unsigned short EFFECT_TYPE_ENTANGLE;           // 0x007466C4 (18)
extern const unsigned short EFFECT_TYPE_DEATH;              // 0x007466C8 (19)
extern const unsigned short EFFECT_TYPE_KNOCKDOWN;          // 0x007466CC (20)
extern const unsigned short EFFECT_TYPE_DISPELMAGICALL;     // 0x00746748 (51)
extern const unsigned short EFFECT_TYPE_DISPELMAGICBEST;    // 0x0074674C (52)
extern const unsigned short EFFECT_TYPE_DISGUISE;           // 0x00746774 (62)
extern const unsigned short EFFECT_TYPE_TIMESTOP;           // 0x0074677C (64)
extern const unsigned short EFFECT_TYPE_ASSUREDHIT;         // 0x0074680C (100)
extern const unsigned short EFFECT_TYPE_ASSUREDDEFLECTION;  // 0x00746818 (103)
extern const unsigned short EFFECT_TYPE_FORCE_RESISTED;     // 0x0074681C (104)
extern const unsigned short EFFECT_TYPE_FORCE_FIZZLE;       // 0x00746820 (105)

// NONMATCH (100): case-local stack packing. The original packs every case
// local into the dead argument and new-temporary slots (8-byte frame) and
// merges the two dispel pops; cl gives DispelMagicBest its own slot.
// Cases are in the original block order.
// FUNCTION: SWKOTOR 0x005313F0
int CSWVirtualMachineCommands::ExecuteCommandEffectAssuredHit(int nCommandId, int nParameters)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->m_nType = EFFECT_TYPE_INVALIDEFFECT;
    switch (nCommandId) {
    case 0x82:
        pEffect->m_nType = EFFECT_TYPE_ENTANGLE;
        break;
    case 0x85: {
        int bSpectacularDeath = 0;
        int bDisplayFeedback = 1;
        if (nParameters >= 1 && !g_pVirtualMachine->StackPopInteger(&bSpectacularDeath)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bDisplayFeedback)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        pEffect->m_nType = EFFECT_TYPE_DEATH;
        pEffect->SetInteger(0, bSpectacularDeath);
        pEffect->SetInteger(1, bDisplayFeedback);
        break;
    }
    case 0x86:
        pEffect->m_nType = EFFECT_TYPE_KNOCKDOWN;
        break;
    case 0x94:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 5);
        break;
    case 0x9a:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 6);
        break;
    case 0x9d:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 1);
        break;
    case 0x9e:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 2);
        break;
    case 0xa1:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 4);
        break;
    case 0xa4: {
        int nAmount;
        float fInterval;
        if (!g_pVirtualMachine->StackPopInteger(&nAmount)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (!g_pVirtualMachine->StackPopFloat(&fInterval)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        int nIntervalMs = (int)(fInterval * 1000.0f);
        pEffect->m_nType = EFFECT_TYPE_REGENERATE;
        pEffect->SetInteger(0, nAmount);
        pEffect->SetInteger(1, nIntervalMs);
        break;
    }
    case 0x33:
        pEffect->m_nType = EFFECT_TYPE_ASSUREDHIT;
        break;
    case 0xfc: {
        int nAttackCount = 0;
        if (!g_pVirtualMachine->StackPopInteger(&nAttackCount)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        pEffect->m_nType = EFFECT_TYPE_ASSUREDDEFLECTION;
        pEffect->SetInteger(0, nAttackCount);
        break;
    }
    case 0x10e:
        pEffect->m_nType = EFFECT_TYPE_HASTE;
        break;
    case 0x1cc: {
        pEffect->m_nType = EFFECT_TYPE_DISPELMAGICALL;
        int nCasterLevel;
        if (!g_pVirtualMachine->StackPopInteger(&nCasterLevel)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        break;
    }
    case 0x1cf: {
        int nDisguiseAppearance;
        if (!g_pVirtualMachine->StackPopInteger(&nDisguiseAppearance)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        pEffect->m_nType = EFFECT_TYPE_DISGUISE;
        pEffect->SetInteger(0, nDisguiseAppearance);
        pEffect->SetInteger(1, 0);
        break;
    }
    case 0x192: {
        pEffect->m_nType = EFFECT_TYPE_FORCE_RESISTED;
        unsigned long oidCaster = INVALID_OBJECT_ID;
        if (!g_pVirtualMachine->StackPopObject(&oidCaster)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        pEffect->SetObjectID(0, oidCaster);
        break;
    }
    case 0x1a4:
        pEffect->m_nType = EFFECT_TYPE_FORCE_FIZZLE;
        break;
    case 0x1d3:
        pEffect->m_nType = EFFECT_TYPE_TIMESTOP;
        break;
    case 0x2f2:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 8);
        pEffect->SetInteger(1, 1);
        break;
    case 0x1d9: {
        pEffect->m_nType = EFFECT_TYPE_DISPELMAGICBEST;
        int nCasterLevel;
        if (!g_pVirtualMachine->StackPopInteger(&nCasterLevel)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        break;
    }
    case 0x2f3:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 5);
        pEffect->SetInteger(1, 1);
        break;
    case 0x2f4:
        pEffect->m_nType = EFFECT_TYPE_SETSTATE;
        pEffect->SetInteger(0, 4);
        pEffect->SetInteger(1, 1);
        break;
    }
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
