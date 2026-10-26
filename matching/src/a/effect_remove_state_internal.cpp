#include "a/effect_list_handler.h"

class CVirtualMachine { public: int RunScript(CExoString*, unsigned long, int); };
extern CVirtualMachine* g_pVirtualMachine; // 0x007A3A00
struct CCreatureScriptView { char pad_0x00[0x260]; CExoString m_sScript; };

// FUNCTION: SWKOTOR 0x004DA870
int CSWSEffectListHandler::OnRemoveSetStateInternal(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pEffect || !pCreature) return 1;
    switch (pEffect->GetInteger(0)) {
    case 1: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
        g_pVirtualMachine->RunScript(&((CCreatureScriptView*)pCreature)->m_sScript, pCreature->m_idSelf, 1);
    case 12: case 13:
        pCreature->UpdateAmbientAnimationState();
    }
    if (!pCreature->m_b9D4) {
        pCreature->ClearVisibleList();
        pCreature->UpdateVisibleList(0);
    }
    return 1;
}
