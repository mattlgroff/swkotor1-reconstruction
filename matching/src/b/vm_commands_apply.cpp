// CSWVirtualMachineCommands: ApplyEffectAtLocation / ApplyEffectToObject
// (0x0052E430-0x0052E720).
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/sws_creature.h"
#include "b/global_variable_table.h"

extern const int EFFECT_TYPE_LINK;                          // 0x0074671C

// CSWSObject +0x148 (sws_creature.h keeps CSWSObject data-free, as CSWSItem and
// CSWSCreature declare their own members from +0xf4).
struct CSWSObjectLinkView {
    char pad_0x0[0x148];
    int m_bApplyingLink;                                    // 0x148 (name guessed)
};

// NONMATCH (23): effect / location stack slots are swapped (and so ebx/edi);
// no declaration order changes them.
// FUNCTION: SWKOTOR 0x0052E430
int CSWVirtualMachineCommands::ExecuteCommandApplyEffectAtLocation(int nCommandId, int nParameters)
{
    int nDurationType;
    void* pEffectData;
    void* pLocationData;
    float fDuration;
    if (!g_pVirtualMachine->StackPopInteger(&nDurationType)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, &pEffectData)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, &pLocationData)) {
        if (pEffectData) {
            delete pEffectData;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopFloat(&fDuration)) {
        if (pLocationData) {
            delete pLocationData;
            pLocationData = NULL;
        }
        if (pEffectData) {
            delete pEffectData;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = (CGameEffect*)pEffectData;
    CScriptLocation* pLocation = (CScriptLocation*)pLocationData;
    switch (nDurationType) {
    case 0:
        pEffect->SetDurationType(0);
        break;
    case 1:
        pEffect->SetDurationType(1);
        pEffect->m_fDuration = fDuration;
        break;
    default:
        pEffect->SetDurationType(2);
        break;
    }
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    CSWSCreature* pPlayer = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    CSWSArea* pArea;
    if (pPlayer && (pArea = pPlayer->GetArea()) != NULL) {
        pArea->ApplyEffect(pEffect, *(Vector*)pLocation->m_vPosition, *(Vector*)pLocation->m_vOrientation);
    } else {
        delete pEffect;
    }
    if (pLocation) {
        delete pLocation;
    }
    return 0;
}

// NONMATCH: the original reloads pEffect straight into esi; ours copies it
// through eax (register allocation).
// FUNCTION: SWKOTOR 0x0052E570
int CSWVirtualMachineCommands::ExecuteCommandApplyEffectToObject(int nCommandId, int nParameters)
{
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    int nDurationType;
    void* pEffectData;
    unsigned long oidTarget;
    float fDuration;
    if (!g_pVirtualMachine->StackPopInteger(&nDurationType) ||
        !g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_EFFECT, &pEffectData) ||
        !g_pVirtualMachine->StackPopObject(&oidTarget) ||
        !g_pVirtualMachine->StackPopFloat(&fDuration)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameEffect* pEffect = (CGameEffect*)pEffectData;
    switch (nDurationType) {
    case 0:
        pEffect->SetDurationType(0);
        break;
    case 1:
        pEffect->SetDurationType(1);
        pEffect->m_fDuration = fDuration;
        break;
    default:
        pEffect->SetDurationType(2);
        break;
    }
    CGameObject* pTarget;
    if (pObjectArray->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID && pTarget) {
        CGameObject* pCreator;
        if (pObjectArray->GetGameObject(pEffect->m_oidCreator, &pCreator) == GAME_OBJECT_VALID && pCreator &&
            pCreator->m_nObjectType == 5 && pCreator->AsSWSCreature()->m_n964) {
            pEffect->m_n24 = pCreator->AsSWSCreature()->m_n968;
        }
        if (pTarget->AsSWSObject()) {
            if (pEffect->m_nType == EFFECT_TYPE_LINK) {
                ((CSWSObjectLinkView*)pTarget->AsSWSObject())->m_bApplyingLink = 1;
            }
            pTarget->AsSWSObject()->ApplyEffect(pEffect, 0, 1);
            ((CSWSObjectLinkView*)pTarget->AsSWSObject())->m_bApplyingLink = 0;
        }
    } else {
        delete pEffect;
    }
    return 0;
}
