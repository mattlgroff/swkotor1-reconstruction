// VM SetEffectIcon: copy and link an icon effect with the original effect.
#include "b/vm_commands.h"
#include "b/game_effect.h"

extern const unsigned short EFFECT_TYPE_ICON;               // 0x00746788
extern const unsigned short EFFECT_TYPE_LINK;               // 0x0074671C

// FUNCTION: SWKOTOR 0x005475A0
int CSWVirtualMachineCommands::ExecuteCommandSetEffectIcon(int nCommandId, int nParameters)
{
    CGameEffect* pEffect;
    int nIcon;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, (void**)&pEffect))
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    CGameEffect* pResult = pEffect;
    if (!g_pVirtualMachine->StackPopInteger(&nIcon)) {
        if (pResult) delete pResult;
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pIcon = new CGameEffect(pResult);
    if (pIcon) {
        pIcon->m_nType = EFFECT_TYPE_ICON;
        pIcon->m_n20 = 1;
        pIcon->SetCreator(m_oidObjectRunScript);
        pIcon->m_n20 = 1;
        pIcon->SetInteger(0, nIcon);
        CGameEffect* pLink = new CGameEffect(pResult);
        if (pLink) {
            pLink->m_nType = EFFECT_TYPE_LINK;
            pLink->m_n20 = 1;
            pLink->SetCreator(m_oidObjectRunScript);
            pLink->SetLinked(pIcon, pResult);
            pResult = pLink;
        } else {
            delete pIcon;
        }
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_EFFECT, pResult))
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    if (pResult) delete pResult;
    return 0;
}
