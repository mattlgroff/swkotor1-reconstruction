// CSWSEffectListHandler remove/apply handlers in 0x004E1A70-0x004E478B
// (functions.csv rows plus the untabled FUN_004e2fe0 and the
// CExoArrayList<unsigned short>::Remove helper they call).
#include "a/effect_list_handler.h"

// FUNCTION: SWKOTOR 0x004E1A70
// NONMATCH (5): original caches the 0x17 type constant in ebp for the word compare.
int CSWSEffectListHandler::OnRemoveSetAIState(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_nAIStateFlags) {
            unsigned short nFlags = 0xffff;
            if (pCreature->m_appliedEffects.num > 0) {
                for (int i = pCreature->m_pStats->m_nAIStateStart;
                     i < pCreature->m_appliedEffects.num; i++) {
                    CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
                    if (pOther->m_nType != 0x17) {
                        break;
                    }
                    if (pOther != pEffect) {
                        nFlags &= pOther->GetInteger(0);
                    }
                }
            }
            pCreature->m_nAIStateFlags = nFlags;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E1B00
int CSWSEffectListHandler::OnRemoveEnemyAttackBonus(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int i = 0;
        int nTotal = 0;
        if (pEffect) {
            for (; i < pCreature->m_appliedEffects.num; i++) {
                CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
                if (pOther->m_nType > 0x18) {
                    break;
                }
                if (pOther->m_nType == 0x18 && pOther != pEffect) {
                    nTotal += pOther->GetInteger(0);
                }
            }
        }
        pCreature->m_pStats->m_nEnemyAttackBonus = nTotal;
    }
    return 1;
}
// FUNCTION: SWKOTOR 0x004E1B80
// NONMATCH (3): push edi placement (loop index hoisted before the pEffect test).
int CSWSEffectListHandler::OnRemoveArcaneSpellFailure(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nTotal = 0;
        if (pEffect) {
            for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
                CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
                if (pOther->m_nType > 0x19) {
                    break;
                }
                if (pOther->m_nType == 0x19 && pOther != pEffect) {
                    nTotal += pOther->GetInteger(0);
                }
            }
        }
        if (pObject->AsSWSCreature()) {
            pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
        }
        pCreature->m_pStats->Set11C(nTotal);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E2FE0
int CSWSEffectListHandler::OnRemove8E8(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CExoArrayList<CGameEffect*>* pEffects = &pObject->AsSWSCreature()->m_appliedEffects;
        int bFound = 0;
        if (pEffects->num > 0) {
            for (int i = 0; i < pEffects->num; i++) {
                CGameEffect* pOther = pEffects->element[i];
                if (pOther->m_nType > 0x3b) {
                    break;
                }
                if (pOther->m_nType == 0x3b && pOther != pEffect) {
                    bFound = 1;
                    break;
                }
            }
        }
        pObject->AsSWSCreature()->m_b8E8 = bFound;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E3050
int CSWSEffectListHandler::OnRemoveSpecialWalkAnimation(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
        unsigned char nAnimation = 0;
        if (pEffects) {
            if (pEffects->num > 0) {
                for (int i = 0; i < pEffects->num; i++) {
                    CGameEffect* pOther = pEffects->element[i];
                    if (pOther != pEffect) {
                        if (pOther->m_nType == 0x3a) {
                            nAnimation = (unsigned char)pOther->GetInteger(0);
                            break;
                        }
                        if (pOther->m_nType > 0x3a) {
                            break;
                        }
                    }
                }
            }
        }
        pCreature->m_nA10 = nAnimation;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E3760
int CSWSEffectListHandler::OnRemoveBlindness(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        unsigned char n8EC = pCreature->m_n8EC;
        int nFlags = n8EC & ~pEffect->GetInteger(0);
        for (int i = pCreature->m_pStats->m_nFlags8ECStart; i < pCreature->m_appliedEffects.num;
             i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x49) {
                break;
            }
            if (pOther->m_nType == 0x49 && pOther != pEffect) {
                nFlags |= pOther->GetInteger(0);
            }
        }
        pCreature->m_n8EC = (unsigned char)nFlags;
    }
    return 1;
}
// FUNCTION: SWKOTOR 0x004E3B20 SYMBOL: ?Remove@?$CExoArrayList@G@@QAEXG@Z
template void CExoArrayList<unsigned short>::Remove(unsigned short t);

// FUNCTION: SWKOTOR 0x004E3BE0
int CSWSEffectListHandler::OnApplyInvisibility(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect) {
            CServerExoAppInternal2* pInternal = GetServerExoApp()->GetInternal6B0();
            pInternal->m_lInvisible.AddInvisible(pCreature->m_idSelf);
        }
    }
    return 0;
}
// FUNCTION: SWKOTOR 0x004E4750
int CSWSEffectListHandler::OnRemoveBonusFeat(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        int nFeat = pEffect->GetInteger(0);
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->m_pStats->m_lstBonusFeats.Remove((unsigned short)nFeat);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E32A0
int CSWSEffectListHandler::OnRemove8ECBit0(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nFlags = pCreature->m_n8EC & ~1;
        int nCount = pCreature->m_appliedEffects.num;
        for (int i = pCreature->m_pStats->m_nFlag8ECBit0Start; i < nCount; i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x46) {
                break;
            }
            if (pOther->m_nType == 0x46 && pOther != pEffect) {
                nFlags |= 1;
            }
        }
        pCreature->m_n8EC = (unsigned char)nFlags;
    }
    return 1;
}
