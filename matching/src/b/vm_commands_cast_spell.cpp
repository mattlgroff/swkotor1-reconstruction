// CSWVirtualMachineCommands::ExecuteCommandActionCastSpellAtObject (round s10):
// ActionCastSpellAtObject (0x30), ActionCastSpellAtLocation (0xea),
// ActionCastFakeSpellAtObject (0x1f5) and ActionCastFakeSpellAtLocation (0x1f6).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/sw_rules.h"
#include "b/vector.h"

class CScriptLocation {
public:
    CScriptLocation();                                      // 0x004CA7A0
    ~CScriptLocation();                                     // 0x004015A0

    Vector m_vPosition;                                     // 0x00
    Vector m_vOrientation;                                  // 0x0c
};

class CSWClass {
public:
    static int IsForceUserClass(int nClass);                // 0x005BE4A0
};

// NONMATCH (51): cl keeps `this` in ebp and the fake flag in memory; the
// original spills `this` to [esp+0x30] and keeps the flag (later the -1
// constant) in ebp, which also changes how the EH-state resets merge.
// FUNCTION: SWKOTOR 0x0052EE50
int CSWVirtualMachineCommands::ExecuteCommandActionCastSpellAtObject(int nCommandId, int nParameters)
{
    int nProjectilePathType;
    int nSpell;
    unsigned long oidTarget;
    int bInstantSpell = 0;
    int bLocationTarget = 0;
    int nMetaMagic;
    int nDomainLevel;
    CScriptLocation* pLocation;
    int bCheat;
    int bFake = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    oidTarget = INVALID_OBJECT_ID;
    CScriptLocation lTarget;
    if (nCommandId != 0x30 && nCommandId != 0x1f5) {
        if (!g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        lTarget = *pLocation;
        bLocationTarget = 1;
        delete pLocation;
    } else if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId != 0x1f5 && nCommandId != 0x1f6) {
        if (!g_pVirtualMachine->StackPopInteger(&nMetaMagic) ||
            !g_pVirtualMachine->StackPopInteger(&bCheat)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nCommandId == 0x30) {
            if (!g_pVirtualMachine->StackPopInteger(&nDomainLevel)) {
                return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
            }
        } else {
            nDomainLevel = 0;
        }
    } else {
        bFake = 1;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nProjectilePathType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if ((nCommandId == 0x30 || nCommandId == 0xea) && nParameters >= 7 &&
        !g_pVirtualMachine->StackPopInteger(&bInstantSpell)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pRules->m_pSpellArray->m_nNumSpells < nSpell || nSpell < 0) {
        return 0;
    }
    switch (nProjectilePathType) {
    case 0:
        nProjectilePathType = 0;
        break;
    case 1:
        nProjectilePathType = 1;
        break;
    case 2:
        nProjectilePathType = 2;
        break;
    case 3:
        nProjectilePathType = 3;
        break;
    case 4:
        nProjectilePathType = 5;
        break;
    default:
        return 0;
    }
    CGameObject* pCaller = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (m_bValidObjectRunScript != 1 || !pCaller) {
        return 0;
    }
    if ((nCommandId == 0x30 || nCommandId == 0x1f5) &&
        !g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget)) {
        return 0;
    }
    CSWSCreature* pCreature = pCaller->AsSWSCreature();
    CSWSPlaceable* pPlaceable = pCaller->AsSWSPlaceable();
    if (bFake) {
        if (pCreature) {
            pCreature->AddCastSpellActions(nSpell, 0, 0, 0, 0, lTarget.m_vPosition, oidTarget, 0,
                                           bLocationTarget, 0, 1, (unsigned char)nProjectilePathType,
                                           bInstantSpell, -1, 0xff, 0);
        } else if (pPlaceable) {
            pPlaceable->AddCastFakeSpellActions(nSpell, 0, lTarget.m_vPosition, oidTarget, 1,
                                                (unsigned char)nProjectilePathType);
        }
        return 0;
    }
    if (pCreature) {
        unsigned char nClassType = 0xff;
        if (!pCreature->m_bCommandable) {
            return 0;
        }
        unsigned char nMultiClass;
        if (bCheat) {
            nMultiClass = 0xff;
            goto cast;
        }
        int i;
        for (i = 0; i < pCreature->GetStats()->m_nNumMultiClasses; i++) {
            if (CSWClass::IsForceUserClass(pCreature->GetStats()->GetClass(i)) &&
                pCreature->GetStats()->GetSpellUsesLeft(nSpell, (unsigned char)i, 0, 0)) {
                CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
                if (pSpell && pSpell->GetSpellLevel(pCreature->GetStats()->GetClass(i)) < 0xff) {
                    nMultiClass = (unsigned char)i;
                    if (nMultiClass != 2) {
                        goto cast;
                    }
                    break;
                }
            }
        }
        if (pCreature->GetStats()->HasReadySpell(nSpell) != 1) {
            return 0;
        }
        {
            unsigned char nMemorized = pCreature->GetStats()->GetMemorizedSpellClass(nSpell);
            if (nMemorized == -1) {
                return 0;
            }
            nMultiClass = 0xfe;
            nClassType = nMemorized;
        }
cast:
        nMetaMagic = 0;
        pCreature->AddCastSpellActions(nSpell, nMultiClass, nDomainLevel, 0, 0, lTarget.m_vPosition, oidTarget,
                                       0, bLocationTarget, 0, 0, (unsigned char)nProjectilePathType,
                                       bInstantSpell, -1, nClassType, 0);
        return 0;
    }
    if (pPlaceable && pPlaceable->m_bCommandable) {
        pPlaceable->AddCastFakeSpellActions(nSpell, nMetaMagic, lTarget.m_vPosition, oidTarget, 0,
                                            (unsigned char)nProjectilePathType);
    }
    return 0;
}
