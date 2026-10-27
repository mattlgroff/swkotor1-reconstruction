// CSWVirtualMachineCommands::ExecuteCommandGetFirstEffect (round s8):
// GetFirstEffect and GetNextEffect share the per-object effect iterator.
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/sws_creature.h"
#include "b/exo_array_list.h"

extern const int EFFECT_TYPE_ICON;                          // 0x00746788 (read as a dword here)

struct CSWSObjectEffectIteratorView {
    char pad_0x0[0x124];
    CExoArrayList<CGameEffect*> m_appliedEffects;           // 0x124
    char pad_0x130[0x1e8 - 0x130];
    unsigned short m_nEffectIterator;                       // 0x1e8 (name guessed)
};

// NONMATCH (41): the original keeps the applied flag in ebx; cl homes it
// on the stack across the allocation's EH region.
// FUNCTION: SWKOTOR 0x00549180
int CSWVirtualMachineCommands::ExecuteCommandGetFirstEffect(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidCreature);
    int bApplied = 1;
    CGameEffect* pEffect;
    CSWSObjectEffectIteratorView* pTarget;
    if (pObject && (pTarget = (CSWSObjectEffectIteratorView*)pObject->AsSWSObject()) != NULL) {
        if (nCommandId == 0x55) {
            pTarget->m_nEffectIterator = 0;
        }
        int i = pTarget->m_nEffectIterator;
        int nCount = pTarget->m_appliedEffects.num;
        for (; i < nCount; i++) {
            CGameEffect* pApplied = pTarget->m_appliedEffects.element[i];
            if (pApplied->m_n20 && pApplied->m_nType != EFFECT_TYPE_ICON &&
                (pApplied->GetDurationType() == 2 || pApplied->GetDurationType() == 1)) {
                break;
            }
        }
        if (i < nCount) {
            pEffect = pTarget->m_appliedEffects.element[i];
            pTarget->m_nEffectIterator = i + 1;
        } else {
            bApplied = 0;
            pTarget->m_nEffectIterator = 0;
            pEffect = new CGameEffect(1);
            pEffect->m_nType = EFFECT_TYPE_INVALIDEFFECT;
        }
        if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
        if (!bApplied && pEffect) {
            delete pEffect;
        }
    } else {
        pEffect = new CGameEffect(bApplied);
        pEffect->m_nType = EFFECT_TYPE_INVALIDEFFECT;
        if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pEffect)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
        delete pEffect;
    }
    return 0;
}
