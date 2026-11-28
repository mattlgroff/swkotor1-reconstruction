// CSWVirtualMachineCommands::ExecuteCommandVersusAlignmentEffect (round s9):
// VersusAlignmentEffect (0x163), VersusRacialTypeEffect (0x164) and
// VersusTrapEffect (0x165) store the versus values in effect-type specific
// integer slots.
#include "b/vm_commands.h"
#include "b/game_effect.h"

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x00545470
int CSWVirtualMachineCommands::ExecuteCommandVersusAlignmentEffect(int nCommandId, int nParameters)
{
    CGameEffect* pEffect;
    int nA;
    int nB;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (nParameters > 1) {
        if (!g_pVirtualMachine->StackPopInteger(&nA)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    if (nParameters > 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nB)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    int bValid = 1;
    if (nCommandId == 0x163) {
        if (nA <= -1 || nA >= 4) {
            bValid = 0;
        }
    } else if (nCommandId == 0x164) {
        if (nA < 0 || nA > RACIAL_TYPE_COUNT) {
            bValid = 0;
        }
    }
    if (pEffect && bValid) {
        int nType = pEffect->m_nType;
        if (nType == EFFECT_TYPE_ATTACK_INCREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_ATTACK_DECREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_DAMAGE_INCREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_DAMAGE_DECREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_0746778) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nB);
                break;
            case 0x164:
                pEffect->SetInteger(1, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_0746738) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nB);
                break;
            case 0x164:
                pEffect->SetInteger(1, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_CONCEALMENT) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(2, nA);
                pEffect->SetInteger(3, nB);
                break;
            case 0x164:
                pEffect->SetInteger(1, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_DAMAGE_RESISTANCE) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_DAMAGE_REDUCTION) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_DAMAGE_IMMUNITY) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_IMMUNITY) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(2, nA);
                pEffect->SetInteger(3, nB);
                break;
            case 0x164:
                pEffect->SetInteger(1, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_AC_INCREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_AC_DECREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_FORCE_RESISTANCE_INCREASE) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_FORCE_RESISTANCE_DECREASE) {
            // NOTE: the original tests this type and does nothing
        } else if (nType == EFFECT_TYPE_SAVING_THROW_INCREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(4, nA);
                pEffect->SetInteger(5, nB);
                break;
            case 0x164:
                pEffect->SetInteger(3, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_SAVING_THROW_DECREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(4, nA);
                pEffect->SetInteger(5, nB);
                break;
            case 0x164:
                pEffect->SetInteger(3, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_SKILL_INCREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        } else if (nType == EFFECT_TYPE_SKILL_DECREASE) {
            switch (nCommandId) {
            case 0x163:
                pEffect->SetInteger(3, nA);
                pEffect->SetInteger(4, nB);
                break;
            case 0x164:
                pEffect->SetInteger(2, nA);
                break;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pEffect;
    return 0;
}
