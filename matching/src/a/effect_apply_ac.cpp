#include "a/effect_list_handler.h"

// NONMATCH (80): compiler register allocation and early return block placement.
// FUNCTION: SWKOTOR 0x004D8D80
int CSWSEffectListHandler::OnApplyACIncrease(CSWSObject* pObject, CGameEffect* pEffect, int)
{
    if (!pObject->AsSWSCreature()) return 0;
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pEffect) return 0;
    int nKind = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int n3 = pEffect->GetInteger(3);
    int n4 = pEffect->GetInteger(4);
    pEffect->GetInteger(5);
    if (nAmount < 1) return 1;
    if (nRace != g_pRules->m_nAA || n3 || n4 || (unsigned)nKind > 4) return 0;
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    switch (nKind) {
    case 0: pStats->m_nACBonus[8] += (unsigned char)nAmount; break;
    case 1:
        if ((signed char)pStats->m_nACBonus[6] > 0) pCreature->SendFeedbackData(0xbd, 0);
        if (nAmount > (signed char)pStats->m_nACBonus[6]) pStats->m_nACBonus[6] = (unsigned char)nAmount;
        break;
    case 2:
        if ((signed char)pStats->m_nACBonus[0] > 0) pCreature->SendFeedbackData(0xbe, 0);
        if (nAmount > (signed char)pStats->m_nACBonus[0]) pStats->m_nACBonus[0] = (unsigned char)nAmount;
        break;
    case 3:
        if ((signed char)pStats->m_nACBonus[4] > 0) pCreature->SendFeedbackData(0xbf, 0);
        if (nAmount > (signed char)pStats->m_nACBonus[4]) pStats->m_nACBonus[4] = (unsigned char)nAmount;
        break;
    case 4:
        if ((signed char)pStats->m_nACBonus[2] > 0) pCreature->SendFeedbackData(0xc0, 0);
        if (nAmount > (signed char)pStats->m_nACBonus[2]) pStats->m_nACBonus[2] = (unsigned char)nAmount;
        break;
    }
    return 0;
}

// NONMATCH (72): compiler register allocation and early return block placement.
// FUNCTION: SWKOTOR 0x004D8F80
int CSWSEffectListHandler::OnApplyACDecrease(CSWSObject* pObject, CGameEffect* pEffect, int)
{
    if (!pObject->AsSWSCreature()) return 0;
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature->m_pStats->GetEffectImmunity(0x17, GetServerExoApp()->GetCreatureByGameObjectID(pEffect->m_oidCreator))) return 1;
    int nKind = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int n3 = pEffect->GetInteger(3);
    int n4 = pEffect->GetInteger(4);
    pEffect->GetInteger(5);
    if (nAmount < 1) return 1;
    if (pCreature->m_bF8) return 1;
    if (nRace != g_pRules->m_nAA || n3 || n4 || (unsigned)nKind > 4) return 0;
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    switch (nKind) {
    case 0: pStats->m_nACBonus[9] += (unsigned char)nAmount; break;
    case 1: if (nAmount > (signed char)pStats->m_nACBonus[7]) pStats->m_nACBonus[7] = (unsigned char)nAmount; break;
    case 2: if (nAmount > (signed char)pStats->m_nACBonus[1]) pStats->m_nACBonus[1] = (unsigned char)nAmount; break;
    case 3: if (nAmount > (signed char)pStats->m_nACBonus[5]) pStats->m_nACBonus[5] = (unsigned char)nAmount; break;
    case 4: if (nAmount > (signed char)pStats->m_nACBonus[3]) pStats->m_nACBonus[3] = (unsigned char)nAmount; break;
    }
    return 0;
}
