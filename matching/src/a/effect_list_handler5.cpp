// CSWSEffectListHandler effect-icon, true-seeing and bonus-feat handlers
// in 0x004E30D0-0x004E478B.
#include "a/effect_list_handler.h"

void operator delete(void* p);

// the stdlib.h macros (each argument may be evaluated twice)
#ifndef __max
#define __max(a, b) (((a) > (b)) ? (a) : (b))
#define __min(a, b) (((a) < (b)) ? (a) : (b))
#endif

// FUNCTION: SWKOTOR 0x004E30D0
// NONMATCH (46): the original saves ebx/ebp only around the effect scan and
// keeps the icon search loop unrotated (top test, jmp back).
int CSWSEffectListHandler::OnRemoveEffectIcon(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CExoArrayList<CEffectIcon*>* pIcons = &pObject->AsSWSCreature()->m_lstEffectIcons;
        unsigned short nIndex = pObject->AsSWSCreature()->m_pStats->m_nEffectIconStart;
        unsigned char nIcon = (unsigned char)pEffect->GetInteger(0);
        int bOther = 0;
        for (; nIndex < pObject->m_appliedEffects.num; nIndex++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[nIndex];
            if (pOther->m_nType != 0x43) {
                break;
            }
            if (pOther == pEffect) {
                continue;
            }
            if (pOther->GetInteger(0) == nIcon) {
                bOther = 1;
            }
        }
        for (unsigned short nSlot = (unsigned short)(pIcons->num - 1);; nSlot--) {
            if (pIcons->element[nSlot]->m_nIcon == nIcon) {
                if (!bOther) {
                    int i = nSlot;
                    delete pIcons->element[i];
                    pIcons->num--;
                    for (; i < pIcons->num; i++) {
                        pIcons->element[i] = pIcons->element[i + 1];
                    }
                }
                break;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E31D0
// (missing from functions.csv: the row for 0x004E30D0 spans both handlers)
int CSWSEffectListHandler::OnApplyDisguise(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 1;
    }
    if (!bLoadingGame && pCreature->GetIsInConversation()) {
        return 1;
    }
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (!pEffects) {
        return 1;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pOther = pEffects->element[i];
        if (pOther->m_nType == 0x3e && pOther->m_nID != pEffect->m_nID) {
            pEffects->element[i]->SetInteger(1, 1);
            pCreature->RemoveEffectById(pEffects->element[i]->m_nID);
            break;
        }
    }
    int nAppearance = pEffect->GetInteger(0);
    pCreature->Disguise(nAppearance, pEffect, bLoadingGame);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E35B0
// The second type test re-reads the list element: that keeps 0x49 in a
// register (mov eax,0x49; cmp word ptr [edi+8],ax) as in the original.
int CSWSEffectListHandler::OnApplyTrueSeeing(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->m_n8EC |= 4;
        for (int i = pCreature->m_pStats->m_nFlags8ECStart; i < pCreature->m_appliedEffects.num;
             i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > (unsigned short)0x49) {
                break;
            }
            if (pCreature->m_appliedEffects.element[i]->m_nType != (unsigned short)0x49) {
                continue;
            }
            if (pOther->GetInteger(0) == 8) {
                for (int j = 0; j < pCreature->m_appliedEffects.num; j++) {
                    CGameEffect* pDup = pCreature->m_appliedEffects.element[j];
                    if (pDup->m_nType > 0x4b) {
                        break;
                    }
                    if (pDup->m_nID == pOther->m_nID && pDup != pOther) {
                        pCreature->RemoveEffect(pCreature->m_appliedEffects.element[i]);
                        i = pCreature->m_pStats->m_nFlags8ECStart;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E3690
// NONMATCH (8): the original re-reads the list count and m_pStats after the
// first loop (edi/ebp swapped here).
int CSWSEffectListHandler::OnRemoveTrueSeeing(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nFlags = pCreature->m_n8EC & ~4;
        int nCount = pCreature->m_appliedEffects.num;
        for (int i = pCreature->m_pStats->m_nTrueSeeingStart; i < nCount; i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x48) {
                break;
            }
            if (pOther->m_nType == 0x48 && pOther != pEffect) {
                nFlags |= 2;
            }
        }
        pCreature->m_n8EC = (unsigned char)nFlags;
        for (int j = pCreature->m_pStats->m_nFlags8ECStart; j < pCreature->m_appliedEffects.num;
             j++) {
            CGameEffect* pBlind = pCreature->m_appliedEffects.element[j];
            if (pBlind->m_nType > 0x49) {
                break;
            }
            if (pCreature->m_appliedEffects.element[j]->m_nType == 0x49) {
                pCreature->RemoveEffect(pBlind);
                pCreature->ApplyEffect(pBlind, 0, 0);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E46E0
int CSWSEffectListHandler::OnApplyBonusFeat(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    int nFeat = pEffect->GetInteger(0);
    pObject->AsSWSCreature()->m_pStats->m_lstBonusFeats.Add((unsigned short)nFeat);
    return 0;
}

// FUNCTION: SWKOTOR 0x004DBA50
int CSWSEffectListHandler::OnApplySlow(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CGameEffect* pNew = new CGameEffect(pEffect);
        pNew->m_nType = 0x1d;
        pNew->SetInteger(0, 0x32);
        pObject->ApplyEffect(pNew, bLoadingGame, 0);
        pNew = new CGameEffect(pEffect);
        pNew->SetInteger(0, 0);
        pNew->SetInteger(1, 2);
        pNew->SetInteger(2, g_pRules->m_nAA);
        pNew->SetInteger(5, 0x4007);
        pNew->m_nType = 0x31;
        pObject->ApplyEffect(pNew, bLoadingGame, 0);
        pNew = new CGameEffect(pEffect);
        pNew->m_nType = 0x0b;
        pNew->SetInteger(0, 2);
        pNew->SetInteger(1, 0);
        pNew->SetInteger(2, g_pRules->m_nAA);
        pObject->ApplyEffect(pNew, bLoadingGame, 0);
        pNew = new CGameEffect(pEffect);
        pNew->SetInteger(0, 2);
        pNew->SetInteger(1, 2);
        pNew->SetInteger(3, g_pRules->m_nAA);
        pNew->m_nType = 0x1b;
        pObject->ApplyEffect(pNew, bLoadingGame, 0);
        pNew = new CGameEffect(pEffect);
        pNew->m_nType = 0x3b;
        pNew->SetInteger(0, 0);
        pObject->ApplyEffect(pNew, bLoadingGame, 0);
        pCreature->m_b8CC = 0;
        pCreature->m_b8D0 = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DCD30
// NONMATCH (10): the original places the kind != 8 path (bDeaf = 0, kind
// 0x10 test) out of line after the first allocation.
int CSWSEffectListHandler::OnApplyBlindness(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nKind = pEffect->GetInteger(0);
        if (!pCreature || (nKind != 0x10 && nKind != 8)) {
            return 1;
        }
        CSWSCreature* pCreator =
            GetServerExoApp()->GetCreatureByGameObjectID(pEffect->m_oidCreator);
        if (nKind == 0x10 && pCreature->m_pStats->GetEffectImmunity(7, pCreator)) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pObject->m_idSelf);
            if (pCreator) {
                CSWCCMessageData* pCopy = new CSWCCMessageData;
                pData->CopyTo(pCopy);
                pCreator->SendFeedbackData(0x8b, pCopy);
            }
            pObject->AsSWSCreature()->SendFeedbackData(0x8b, pData);
            return 1;
        }
        if (pCreature->m_bF8) {
            return 1;
        }
        int bDeaf;
        if (nKind == 8) {
            unsigned char nFlags = pCreature->m_n8EC;
            if (nFlags & 2) {
                return 0;
            }
            if (nFlags & 4) {
                return 0;
            }
            bDeaf = 1;
        } else {
            bDeaf = 0;
        }
        if (nKind == 8 || nKind == 0x10) {
            CGameEffect* pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x4b;
            pNew->SetInteger(0, 0x32);
            pNew->SetInteger(1, bDeaf);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x45;
            pNew->SetInteger(0, 4);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pNew = new CGameEffect(pEffect);
            pNew->m_nType = 0x1e;
            pNew->SetInteger(0, 0x138a);
            pCreature->ApplyEffect(pNew, bLoadingGame, 0);
            pCreature->m_n8EC |= (unsigned char)nKind;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E2650
// (the functions.csv row also spans the untabled OnApplyDisease at 0x004E2720)
int CSWSEffectListHandler::OnRemoveForceResistanceDecrease(CSWSObject* pObject,
                                                           CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        int nBest = 0;
        for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            unsigned short nType = pOther->m_nType;
            if (nType == 0x21 || nType == 0x22) {
                if (pOther == pEffect) {
                    continue;
                }
                if (nType == 0x22) {
                    pOther->GetInteger(0);
                    continue;
                }
                if (pOther->GetInteger(0) > nBest) {
                    nBest = pObject->m_appliedEffects.element[i]->GetInteger(0);
                }
            } else if (nType > 0x22) {
                break;
            }
        }
        if (pObject->AsSWSCreature()) {
            pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
        }
        pStats->SetForceResistanceDecrease(nBest);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E2A90
// NONMATCH (100): the original keeps pObject in ebp, bHaste in ebx and the
// creator in esi, and reuses ebp for the -1 EH state in the feedback block.
int CSWSEffectListHandler::OnApplyHasteOrSlow(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        unsigned short nEffectType = pEffect->m_nType;
        int bHaste = nEffectType == 1;
        CSWSCreature* pCreator =
            g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator);
        if (!bHaste && pObject->AsSWSCreature()->m_pStats->GetEffectImmunity(9, pCreator)) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pObject->m_idSelf);
            if (pCreator) {
                CSWCCMessageData* pCopy = new CSWCCMessageData;
                pData->CopyTo(pCopy);
                pCreator->SendFeedbackData(0x92, pCopy);
            }
            pObject->AsSWSCreature()->SendFeedbackData(0x92, pData);
            return 1;
        }
        int nCount = pObject->m_appliedEffects.num;
        int nNet = 0;
        int i;
        for (i = 0; i < nCount; i++) {
            int nType = pObject->m_appliedEffects.element[i]->m_nType;
            if (nType == 1) {
                nNet++;
            } else if (nType == 3) {
                nNet--;
            } else if (nType > 3) {
                break;
            }
        }
        int nAfter = __max(__min((bHaste ? 1 : -1) + nNet, 1), -1);
        nNet = __max(__min(nNet, 1), -1);
        if (nNet == nAfter) {
            return 0;
        }
        if (nNet > 0) {
            for (i = 0; i < nCount; i++) {
                unsigned short nType = pObject->m_appliedEffects.element[i]->m_nType;
                if (nType == 0x29) {
                    pObject->RemoveEffectById(pObject->m_appliedEffects.element[i]->m_nID);
                    break;
                }
                if (nType > 0x29) {
                    break;
                }
            }
            return 0;
        }
        if (nNet < 0) {
            for (i = 0; i < nCount; i++) {
                unsigned short nType = pObject->m_appliedEffects.element[i]->m_nType;
                if (nType == 0x2a) {
                    pObject->RemoveEffectById(pObject->m_appliedEffects.element[i]->m_nID);
                    break;
                }
                if (nType > 0x2a) {
                    break;
                }
            }
            return 0;
        }
        CGameEffect* pMarker = new CGameEffect(1);
        pMarker->SetDurationType(g_nDurationEquipped);
        pMarker->m_nSubType = (pMarker->m_nSubType & 0xffef) | 8;
        pMarker->m_nSpellId = pEffect->m_nSpellId;
        pMarker->m_bExpose = 0;
        pMarker->m_nType = (unsigned short)(0x29 + (nAfter <= 0));
        pObject->ApplyEffect(pMarker, bLoadingGame, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E2D50
// NONMATCH (7): pEffect lands in ecx instead of edx and the two zeroing
// xors are scheduled after the type compare.
int CSWSEffectListHandler::OnRemoveHasteOrSlow(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    pObject->AsSWSCreature()->m_b8CC = 0;
    pObject->AsSWSCreature()->m_b8D0 = 0;
    unsigned short nEffectType = pEffect->m_nType;
    int bHaste = nEffectType == 1;
    int nCount = pObject->m_appliedEffects.num;
    int nNet = 0;
    int i;
    for (i = 0; i < nCount; i++) {
        int nType = pObject->m_appliedEffects.element[i]->m_nType;
        if (nType == 1) {
            nNet++;
        } else if (nType == 3) {
            nNet--;
        } else if (nType > 3) {
            break;
        }
    }
    int nAfter = __max(__min((bHaste ? -1 : 1) + nNet, 1), -1);
    nNet = __max(__min(nNet, 1), -1);
    if (nNet == nAfter) {
        return 1;
    }
    if (nNet > 0) {
        for (i = 0; i < nCount; i++) {
            unsigned short nType = pObject->m_appliedEffects.element[i]->m_nType;
            if (nType == 0x29) {
                pObject->RemoveEffectById(pObject->m_appliedEffects.element[i]->m_nID);
                break;
            }
            if (nType > 0x29) {
                break;
            }
        }
        return 1;
    }
    if (nNet < 0) {
        for (i = 0; i < nCount; i++) {
            unsigned short nType = pObject->m_appliedEffects.element[i]->m_nType;
            if (nType == 0x2a) {
                pObject->RemoveEffectById(pObject->m_appliedEffects.element[i]->m_nID);
                break;
            }
            if (nType > 0x2a) {
                break;
            }
        }
        return 1;
    }
    CGameEffect* pMarker = new CGameEffect(1);
    pMarker->m_bExpose = 0;
    pMarker->SetDurationType(g_nDurationEquipped);
    pMarker->m_nSubType = (pMarker->m_nSubType & 0xffef) | 8;
    pMarker->m_nType = (unsigned short)(0x29 + (nAfter <= 0));
    pObject->ApplyEffect(pMarker, 0, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004E2F40
int CSWSEffectListHandler::OnApplyVision(CSWSObject* pObject, CGameEffect* pEffect,
                                         int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        pEffect->GetInteger(0);
        CGameEffect* pNew = new CGameEffect(pEffect);
        if (pNew) {
            pNew->m_nType = 0x1e;
            delete pNew;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E2490
// (untabled: the functions.csv row 0x004E1C20 spans it)
// The type is re-read through the list after the id test (cmp word ptr
// [ecx+8],0x1d first, reload afterwards).
int CSWSEffectListHandler::OnRemoveMovementSpeedChange(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        pObject->AsSWSCreature();
        if (pEffect) {
            float fRate = 1.0f;
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
                CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
                if (pOther->m_nType > 0x1d) {
                    break;
                }
                if (pOther->m_nID == pEffect->m_nID) {
                    continue;
                }
                unsigned short nType = pCreature->m_appliedEffects.element[i]->m_nType;
                if (nType == 0x1c) {
                    fRate += pOther->GetInteger(0) * 0.01f;
                } else if (nType == 0x1d) {
                    fRate -= pOther->GetInteger(0) * 0.01f;
                }
            }
            pCreature->SetMovementRate(fRate);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E2570
// (untabled: the functions.csv row 0x004E1C20 spans it)
int CSWSEffectListHandler::OnRemoveForceResistanceIncrease(CSWSObject* pObject,
                                                           CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        int nBest = 0;
        int nPenalty = 0;
        for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            if (pOther->m_nID == pEffect->m_nID) {
                continue;
            }
            unsigned short nType = pOther->m_nType;
            if (nType == 0x21 || nType == 0x22) {
                if (pOther->m_nType == 0x22) {
                    nPenalty = pOther->GetInteger(0);
                    continue;
                }
                if (pOther->GetInteger(0) > nBest) {
                    nBest = pObject->m_appliedEffects.element[i]->GetInteger(0);
                }
            } else if (nType > 0x22) {
                break;
            }
        }
        if (pObject->AsSWSCreature()) {
            pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
        }
        pStats->SetForceResistance((unsigned char)(nBest - nPenalty));
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E3320
int CSWSEffectListHandler::OnApplyUltraVision(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        CGameEffect* pNew = new CGameEffect(pEffect);
        pNew->m_nType = 0x45;
        pNew->SetInteger(0, 3);
        pCreature->ApplyEffect(pNew, bLoadingGame, 0);
        pCreature->m_n8EC |= 2;
        for (int i = pCreature->m_pStats->m_nFlags8ECStart; i < pCreature->m_appliedEffects.num;
             i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x49) {
                break;
            }
            if (pCreature->m_appliedEffects.element[i]->m_nType == 0x49 &&
                pOther->GetInteger(0) == 8) {
                int j = 0;
                while (j < pCreature->m_appliedEffects.num) {
                    CGameEffect* pDup = pCreature->m_appliedEffects.element[j];
                    if (pDup->m_nType > 0x4b) {
                        break;
                    }
                    if (pDup->m_nID == pOther->m_nID && pDup != pOther) {
                        pCreature->RemoveEffect(pDup);
                        i = pCreature->m_pStats->m_nFlags8ECStart;
                        j = 0;
                    } else {
                        j++;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E3480
// NONMATCH (33): as OnRemoveTrueSeeing: the second loop reloads the count
// and caches 0x49; the register sets of both loops differ.
int CSWSEffectListHandler::OnRemoveUltraVision(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nFlags = pCreature->m_n8EC & ~2;
        int nCount = pCreature->m_appliedEffects.num;
        for (int i = pCreature->m_pStats->m_nTrueSeeingStart; i < nCount; i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x47) {
                break;
            }
            if (pOther->m_nType == 0x47 && pOther != pEffect) {
                nFlags |= 2;
            }
        }
        pCreature->m_n8EC = (unsigned char)nFlags;
        for (int j = pCreature->m_pStats->m_nFlags8ECStart; j < pCreature->m_appliedEffects.num;
             j++) {
            CGameEffect* pBlind = pCreature->m_appliedEffects.element[j];
            if (pBlind->m_nType > 0x49) {
                break;
            }
            if (pBlind->m_nType == 0x49) {
                CGameEffect* pAgain = new CGameEffect(1);
                pAgain->CopyEffect(pBlind, 0);
                pCreature->RemoveEffect(pBlind);
                pCreature->ApplyEffect(pAgain, 0, 0);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E3C30
int CSWSEffectListHandler::OnRemoveInvisibility(CSWSObject* pObject, CGameEffect* pEffect)
{
    int bOther = 0;
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    int nCount = pCreature->m_appliedEffects.num;
    int i = pCreature->m_pStats->m_nInvisibilityStart;
    if (i < nCount) {
        for (; i < nCount; i++) {
            CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
            if (pOther->m_nType > 0x3f) {
                break;
            }
            if (pOther->m_nType == 0x2f || pOther->m_nType == 0x3f) {
                if (pEffect->m_nID != pOther->m_nID) {
                    bOther = 1;
                }
            }
        }
        if (bOther) {
            return 1;
        }
    }
    CServerExoAppInternal2* pInternal = GetServerExoApp()->GetInternal6B0();
    unsigned long oidSelf = pCreature->m_idSelf;
    if (pInternal->m_lInvisible.Contains(oidSelf)) {
        pInternal->m_lInvisible.Remove(oidSelf);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E14D0
// (untabled: the functions.csv row 0x004E13C0 spans it)
int CSWSEffectListHandler::OnRemoveACIncrease(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pEffect) {
        return 1;
    }
    int nKind = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int n3 = pEffect->GetInteger(3);
    int n4 = pEffect->GetInteger(4);
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    int i = pStats->m_nACStart;
    if (nRace != g_pRules->m_nAA || n3 != 0 || n4 != 0) {
        return 1;
    }
    if (nKind == 0) {
        pStats->m_nACBonus[8] -= (unsigned char)nAmount;
        return 1;
    }
    int nBest = 0;
    for (; i < pCreature->m_appliedEffects.num; i++) {
        CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
        if (pOther->m_nType != 0x30) {
            break;
        }
        if (pOther == pEffect) {
            continue;
        }
        if (pOther->GetInteger(0) != nKind) {
            continue;
        }
        unsigned char nAA = g_pRules->m_nAA;
        if (pOther->GetInteger(2) != nAA) {
            continue;
        }
        if (pOther->GetInteger(3) != 0) {
            continue;
        }
        if (pOther->GetInteger(4) != 0) {
            continue;
        }
        if (pOther->GetInteger(1) > nBest) {
            nBest = pOther->GetInteger(1);
        }
    }
    switch (nKind) {
    case 1:
        pCreature->m_pStats->m_nACBonus[6] = (unsigned char)nBest;
        break;
    case 4:
        pCreature->m_pStats->m_nACBonus[2] = (unsigned char)nBest;
        break;
    case 2:
        pCreature->m_pStats->m_nACBonus[0] = (unsigned char)nBest;
        break;
    case 3:
        pCreature->m_pStats->m_nACBonus[4] = (unsigned char)nBest;
        break;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E16E0
// (untabled: the functions.csv row 0x004E13C0 spans it)
int CSWSEffectListHandler::OnRemoveACDecrease(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pEffect) {
        return 1;
    }
    int nKind = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int n3 = pEffect->GetInteger(3);
    int n4 = pEffect->GetInteger(4);
    CSWSCreatureStats* pStats = pCreature->m_pStats;
    int i = pStats->m_nACStart;
    if (nRace != g_pRules->m_nAA || n3 != 0 || n4 != 0) {
        return 1;
    }
    if (nKind == 0) {
        pStats->m_nACBonus[9] -= (unsigned char)nAmount;
        return 1;
    }
    int nBest = 0;
    for (; i < pCreature->m_appliedEffects.num; i++) {
        CGameEffect* pOther = pCreature->m_appliedEffects.element[i];
        if (pOther->m_nType != 0x31) {
            break;
        }
        if (pOther == pEffect) {
            continue;
        }
        if (pOther->GetInteger(0) != nKind) {
            continue;
        }
        unsigned char nAA = g_pRules->m_nAA;
        if (pCreature->m_appliedEffects.element[i]->GetInteger(2) != nAA) {
            continue;
        }
        if (pCreature->m_appliedEffects.element[i]->GetInteger(1) > nBest) {
            nBest = pCreature->m_appliedEffects.element[i]->GetInteger(1);
        }
    }
    switch (nKind) {
    case 1:
        pCreature->m_pStats->m_nACBonus[7] = (unsigned char)nBest;
        break;
    case 2:
        pCreature->m_pStats->m_nACBonus[1] = (unsigned char)nBest;
        break;
    case 3:
        pCreature->m_pStats->m_nACBonus[5] = (unsigned char)nBest;
        break;
    case 4:
        pCreature->m_pStats->m_nACBonus[3] = (unsigned char)nBest;
        break;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E18D0
// (untabled: the functions.csv row 0x004E13C0 spans it)
int CSWSEffectListHandler::OnRemoveDamageImmunityIncrease(CSWSObject* pObject,
                                                          CGameEffect* pEffect)
{
    if (pEffect) {
        int nType = pEffect->GetInteger(0);
        int nAmount = pEffect->GetInteger(1);
        pObject->GetDamageImmunity(nType);
        int nTotal = 0;
        for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            if ((pOther->m_nType == 0x10 && pOther->GetInteger(0) == nType) ||
                (pOther->m_nType == 0x11 && pOther->GetInteger(0) == nType)) {
                if (pOther->m_nType == 0x10) {
                    nTotal += pOther->GetInteger(1);
                } else {
                    nTotal -= pOther->GetInteger(1);
                }
            }
        }
        pObject->SetDamageImmunity((unsigned short)nType, (char)(nTotal - nAmount));
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E19A0
// (untabled: the functions.csv row 0x004E13C0 spans it)
int CSWSEffectListHandler::OnRemoveDamageImmunityDecrease(CSWSObject* pObject,
                                                          CGameEffect* pEffect)
{
    if (pEffect) {
        int nType = pEffect->GetInteger(0);
        int nAmount = pEffect->GetInteger(1);
        pObject->GetDamageImmunityB0(nType);
        int nTotal = 0;
        for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            if ((pOther->m_nType == 0x10 && pOther->GetInteger(0) == nType) ||
                (pOther->m_nType == 0x11 && pOther->GetInteger(0) == nType)) {
                if (pOther->m_nType == 0x10) {
                    nTotal += pOther->GetInteger(1);
                } else {
                    nTotal -= pOther->GetInteger(1);
                }
            }
        }
        pObject->SetDamageImmunity((unsigned short)nType, (char)(nTotal - nAmount));
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004E22F0
// (untabled: the functions.csv row 0x004E1C20 spans it)
int CSWSEffectListHandler::OnRemoveSetState(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 1;
    }
    pObject->m_nE8 = 1;
    pCreature->m_n8ED = 0;
    if (pObject->m_appliedEffects.num > 0) {
        int i = 0;
        int nBest = 0;
        unsigned long oidCreator = 0x7f000000;
        for (; i < pObject->m_appliedEffects.num; i++) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            if (pOther->m_nType > 8) {
                break;
            }
            if (pOther->m_nType == 8 && pOther != pEffect && pOther->GetInteger(0) > nBest) {
                nBest = pObject->m_appliedEffects.element[i]->GetInteger(0);
                oidCreator = pObject->m_appliedEffects.element[i]->m_oidCreator;
            }
        }
        while (i < pObject->m_appliedEffects.num) {
            CGameEffect* pOther = pObject->m_appliedEffects.element[i];
            if (pOther->m_nType > 9) {
                break;
            }
            if (pOther->m_nType == 9) {
                pCreature->RemoveEffectById(pOther->GetID());
            } else {
                i++;
            }
        }
        if (nBest) {
            pCreature->m_n8ED = (unsigned char)nBest;
            pObject->m_nE8 = 0;
            CGameEffect* pNew = new CGameEffect(1);
            pNew->SetCreator(oidCreator);
            pNew->SetDurationType(g_nDurationPermanent);
            pNew->m_bExpose = 0;
            pNew->m_nType = 9;
            pNew->SetInteger(0, nBest);
            pCreature->ApplyEffect(pNew, 0, 0);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DCB40
// NONMATCH (22): register choice for the active-timer call arguments and the
// effect loop (count/index in eax/ecx swapped).
int CSWSEffectListHandler::OnApplyTimestop(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    if (GetServerExoApp()->GetActivePauseState() == 0) {
        GetServerExoApp()->AddToExclusionList(pObject->m_idSelf, 1);
        GetServerExoApp()->TogglePauseState(1);
    } else if (GetServerExoApp()->GetActivePauseState() == 1) {
        unsigned long nAddDay =
            GetServerExoApp()->GetWorldTimer()->GetCalendarDayFromSeconds(pEffect->GetDuration());
        unsigned long nAddTime =
            GetServerExoApp()->GetWorldTimer()->GetTimeOfDayFromSeconds(pEffect->GetDuration());
        unsigned long nDay;
        unsigned long nTime;
        GetServerExoApp()->GetActiveTimer(pObject->m_idSelf)->GetWorldTime(&nDay, &nTime);
        unsigned long nExpiryDay;
        unsigned long nExpiryTime;
        GetServerExoApp()->GetWorldTimer()->AddWorldTimes(nDay, nTime, nAddDay, nAddTime,
                                                          &nExpiryDay, &nExpiryTime);
        int i = 0;
        int nCount = pObject->m_appliedEffects.num;
        for (; i < nCount; i++) {
            if (pObject->m_appliedEffects.element[i]->m_nType == 0x40) {
                pObject->m_appliedEffects.element[i]->SetExpiryTime(nExpiryDay, nExpiryTime);
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E3B70
void CEffectIDList::AddUnique(unsigned __int64 nID)
{
    if (Contains(nID)) {
        return;
    }
    Add(nID);
}

// FUNCTION: SWKOTOR 0x004E40D0
// NONMATCH (78): the original keeps pObject in ebp from the prologue on and
// lays the locals out differently. (The functions.csv row also spans
// OnApplyDispelBestMagic at 0x004E43C0.)
int CSWSEffectListHandler::OnApplyDispelAllMagic(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CGameObject* pCaster;
    if (pArray->GetGameObject(pEffect->m_oidCreator, &pCaster) != g_nGameObjectFound) {
        return 1;
    }
    if (!pCaster) {
        return 1;
    }
    if (!pCaster->AsSWSCreature()) {
        return 1;
    }
    int nCasterLevel = pCaster->AsSWSCreature()->m_pStats->GetLevel(0);
    CEffectIDList lSeen;
    CSWCCMessageData* pMessage = new CSWCCMessageData;
    pMessage->SetObjectID(0, pObject->m_idSelf);
    int nDispelled = 0;
    for (int i = 0; i < pObject->m_appliedEffects.num; i++) {
        CGameEffect* pOther = pObject->m_appliedEffects.element[i];
        unsigned short nSubType = pOther->m_nSubType;
        if ((nSubType & 0x18) != 8) {
            continue;
        }
        unsigned short nDuration = nSubType & 7;
        if (nDuration != g_nDurationTemporary && nDuration != g_nDurationPermanent) {
            continue;
        }
        unsigned __int64 nID = pOther->m_nID;
        unsigned long nSpellId = pOther->m_nSpellId;
        if (lSeen.CountMatches(nID)) {
            continue;
        }
        CGameObject* pOwner;
        int nOwnerLevel;
        if (pArray->GetGameObject(pOther->m_oidCreator, &pOwner) == g_nGameObjectFound &&
            pOwner->AsSWSCreature()) {
            nOwnerLevel = pOwner->AsSWSCreature()->m_pStats->GetLevel(0);
        } else {
            nOwnerLevel = 0;
        }
        if (g_pRules->RollDice(1, 0x14) + nCasterLevel > nOwnerLevel + 11) {
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, pEffect->m_oidCreator, pObject->m_idSelf, g_nEventRemoveEffect, pOther);
            if (pOther->m_nSpellId != 0xffffffff) {
                nDispelled++;
                pMessage->SetInteger(nDispelled, nSpellId);
            }
        }
        lSeen.AddUnique(nID);
    }
    pMessage->SetInteger(0, nDispelled);
    if (nDispelled > 0) {
        CSWSClient* pClient = GetServerExoApp()->GetClientObjectByObjectID(pObject->m_idSelf);
        if (pClient) {
            GetServerExoApp()->GetNWSMessage()->SendServerToPlayerCCMessage(
                pClient->m_nPlayerId, 0xf, pMessage, NULL);
        }
        unsigned long oidCaster = pCaster->m_idSelf;
        if (oidCaster != pObject->m_idSelf) {
            CSWSClient* pCasterClient = GetServerExoApp()->GetClientObjectByObjectID(oidCaster);
            if (pCasterClient) {
                GetServerExoApp()->GetNWSMessage()->SendServerToPlayerCCMessage(
                    pCasterClient->m_nPlayerId, 0xf, pMessage, NULL);
            }
        }
    }
    delete pMessage;
    return 1;
}
