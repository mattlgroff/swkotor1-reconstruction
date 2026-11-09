// Worker A round s9: CSWSEffectListHandler dispatch virtuals (vtable 0x00744274 slots 0x8/0xc).
// Neither start has a functions.csv row.
#include "a/effect_list_handler.h"

// FUNCTION: SWKOTOR 0x004D8320
int CSWSEffectListHandler::OnApplyEffect(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    int nType = pEffect->m_nType;
    if (nType > 0 && nType < 0x6e && m_pApplyFunctions[nType]) {
        return (this->*m_pApplyFunctions[nType])(pObject, pEffect, bLoadingGame);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8360
int CSWSEffectListHandler::OnRemoveEffect(CSWSObject* pObject, CGameEffect* pEffect)
{
    int nType = pEffect->m_nType;
    if (pEffect->m_nType < 0x6e && m_pRemoveFunctions[nType]) {
        return (this->*m_pRemoveFunctions[nType])(pObject, pEffect);
    }
    return 1;
}
