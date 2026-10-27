// CSWVirtualMachineCommands::ExecuteCommandVersusAlignmentEffect (round s9):
// VersusAlignmentEffect (0x163), VersusRacialTypeEffect (0x164) and
// VersusTrapEffect (0x165) store the versus values in effect-type specific
// integer slots.
#include "b/vm_commands.h"
#include "b/game_effect.h"

extern const int EFFECT_TYPE_ATTACK_INCREASE;               // 0x007466A4 (10)
extern const int EFFECT_TYPE_ATTACK_DECREASE;               // 0x007466A8 (11)
extern const int EFFECT_TYPE_DAMAGE_INCREASE;               // 0x007466B0 (13)
extern const int EFFECT_TYPE_DAMAGE_DECREASE;               // 0x007466B4 (14)
extern const int EFFECT_TYPE_0746778;                       // 0x00746778 (63)
extern const int EFFECT_TYPE_0746738;                       // 0x00746738 (47)
extern const int EFFECT_TYPE_CONCEALMENT;                   // 0x007467AC (76)
extern const int EFFECT_TYPE_DAMAGE_RESISTANCE;             // 0x00746684 (2)
extern const int EFFECT_TYPE_DAMAGE_REDUCTION;              // 0x007466AC (12)
extern const int EFFECT_TYPE_DAMAGE_IMMUNITY;               // 0x007466BC (16)
extern const int EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE;      // 0x007466C0 (17)
extern const int EFFECT_TYPE_IMMUNITY;                      // 0x007466D4 (22)
extern const int EFFECT_TYPE_AC_INCREASE;                   // 0x0074673C (48)
extern const int EFFECT_TYPE_AC_DECREASE;                   // 0x00746740 (49)
extern const int EFFECT_TYPE_FORCE_RESISTANCE_INCREASE;     // 0x00746700 (33)
extern const int EFFECT_TYPE_FORCE_RESISTANCE_DECREASE;     // 0x00746704 (34)
extern const int EFFECT_TYPE_SAVING_THROW_INCREASE;         // 0x007466E4 (26)
extern const int EFFECT_TYPE_SAVING_THROW_DECREASE;         // 0x007466E8 (27)
extern const int EFFECT_TYPE_SKILL_INCREASE;                // 0x00746758 (55)
extern const int EFFECT_TYPE_SKILL_DECREASE;                // 0x0074675C (56)
extern const unsigned short RACIAL_TYPE_COUNT;               // 0x0074BF18 (name guessed)

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
        } else if (nType == EFFECT_TYPE_DAMAGE_REDUCTION) {
        } else if (nType == EFFECT_TYPE_DAMAGE_IMMUNITY) {
        } else if (nType == EFFECT_TYPE_DAMAGE_IMMUNITY_INCREASE) {
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
        } else if (nType == EFFECT_TYPE_FORCE_RESISTANCE_DECREASE) {
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
