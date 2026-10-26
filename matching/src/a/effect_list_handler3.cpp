// CSWSEffectListHandler handlers in 0x004DA050-0x004DF510 that have
// no functions.csv rows yet (table entries stored by the handler ctor at
// 0x004E4Bxx-0x004E52xx). Names are placeholders by member offset.
#include "a/effect_list_handler.h"

#include <math.h>

// FUNCTION: SWKOTOR 0x004DA2C0
int CSWSEffectListHandler::OnApplyStats11C(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        if (pEffect) {
            if (pObject->AsSWSCreature()) {
                pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
            }
            unsigned char nValue = pStats->m_n11C;
            pStats->Set11C(nValue + pEffect->GetInteger(0));
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DADC0
int CSWSEffectListHandler::OnRemoveAIValue(CSWSObject* pObject, CGameEffect* pEffect)
{
    pObject->SetAIValue(pEffect->GetInteger(0));
    return 1;
}

// FUNCTION: SWKOTOR 0x004DB090
int CSWSEffectListHandler::OnApplyLinkedVisual(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    if (pObject->m_nObjectType == g_nObjectTypeA || pObject->m_nObjectType == g_nObjectTypeB ||
        pObject->m_nObjectType == g_nObjectTypeC) {
        CGameEffect* pVisual = new CGameEffect(pEffect);
        pVisual->m_nType = 0x1e;
        pVisual->SetObjectID(0, pEffect->GetObjectID(0));
        pVisual->SetInteger(0, pEffect->GetInteger(0));
        pVisual->SetInteger(1, pEffect->GetInteger(1));
        pVisual->SetInteger(2, pEffect->GetInteger(2));
        pObject->ApplyEffect(pVisual, bLoadingGame, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DB210
// NONMATCH (14): pStats / nValue come out in ebx / edi instead of edi / ebx.
int CSWSEffectListHandler::OnApplyForceResistanceDecrease(CSWSObject* pObject,
                                                          CGameEffect* pEffect, int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        unsigned long oidCreator = pEffect->m_oidCreator;
        if (pStats->GetEffectImmunity(0x1a,
                GetServerExoApp()->GetCreatureByGameObjectID(oidCreator))) {
            return 1;
        }
        int nValue = pEffect->GetInteger(0);
        if (nValue < 0 || pObject->m_bF8) {
            return 1;
        }
        if (nValue > 0x80) {
            nValue = 0x80;
        }
        if (pObject->AsSWSCreature()) {
            pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
        }
        if (pStats->m_nForceResistanceDecrease <= nValue) {
            pStats->SetForceResistanceDecrease(nValue);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DB8A0
int CSWSEffectListHandler::OnApplyLink(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    pEffect->UpdateLinked();
    if (pEffect->m_pLinkLeft) {
        pObject->ApplyEffect(pEffect->m_pLinkLeft, bLoadingGame, 0);
    }
    if (pEffect->m_pLinkRight) {
        pObject->ApplyEffect(pEffect->m_pLinkRight, bLoadingGame, 0);
    }
    pEffect->ClearLinked(0, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004DBA30
int CSWSEffectListHandler::OnRemove8CC(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->m_b8CC = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DBCC0
int CSWSEffectListHandler::OnRemove8D0(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->m_b8D0 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DBD90
int CSWSEffectListHandler::OnApply9A4(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CSWSCombatRound* pTarget = pCreature->m_pCombatRound;
        int nCurrent = pTarget->m_nAdditionalAttacks;
        if (nCurrent > 2) {
            return 1;
        }
        pTarget->m_nAdditionalAttacks = nCurrent + pEffect->GetInteger(0);
        pTarget->m_nAdditionalAttacks = pTarget->m_nAdditionalAttacks > 2 ? 2 : pTarget->m_nAdditionalAttacks;
        pTarget->m_nAdditionalAttacks = pTarget->m_nAdditionalAttacks < 0 ? 0 : pTarget->m_nAdditionalAttacks;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DBE00
int CSWSEffectListHandler::OnRemove9A4(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CSWSCombatRound* pTarget = pCreature->m_pCombatRound;
        int nCurrent = pTarget->m_nAdditionalAttacks;
        pTarget->m_nAdditionalAttacks = nCurrent - pEffect->GetInteger(0);
        pTarget->m_nAdditionalAttacks = pTarget->m_nAdditionalAttacks > 2 ? 2 : pTarget->m_nAdditionalAttacks;
        pTarget->m_nAdditionalAttacks = pTarget->m_nAdditionalAttacks < 0 ? 0 : pTarget->m_nAdditionalAttacks;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DDF20
int CSWSEffectListHandler::OnRemoveCreatureOnly(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    return 1;
}

// FUNCTION: SWKOTOR 0x004DE5B0
int CSWSEffectListHandler::OnApplyA9C(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->m_bA9C = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DE5D0
int CSWSEffectListHandler::OnRemoveA9C(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->m_bA9C = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DE5F0
int CSWSEffectListHandler::OnApplyCreatureOnly(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    return 0;
}

// FUNCTION: SWKOTOR 0x004DECE0
int CSWSEffectListHandler::OnRemoveA04(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->m_bA04 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DF410
int CSWSEffectListHandler::OnRemoveNothing(CSWSObject* pObject, CGameEffect* pEffect)
{
    return 1;
}

// FUNCTION: SWKOTOR 0x004DCA80
int CSWSEffectListHandler::OnApplyA10(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    unsigned char nValue = pEffect->GetInteger(0);
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_nA10 = nValue;
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DCAC0
// NONMATCH (2): `mov ecx, edi` / `push esi` swapped before the final call.
int CSWSEffectListHandler::OnRemove4EE150(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        if (!pCreature->GetIsInConversation() || pCreature->m_b9D4 || !pCreature->m_nEC) {
            if (pEffect->GetInteger(1) == 0) {
                pEffect->SetInteger(1, 1);
                pEffect->SetDurationType(g_nDurationTemporary);
                pEffect->m_fDuration = 0.0f;
            }
            pCreature->UnDisguise(pEffect);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DCC90
int CSWSEffectListHandler::OnRemoveServerFlags(CSWSObject* pObject, CGameEffect* pEffect)
{
    GetServerExoApp()->Fn4AE960(1);
    GetServerExoApp()->Fn4AE9C0(pObject->m_idSelf, 1);
    return 1;
}

// FUNCTION: SWKOTOR 0x004DCCC0
int CSWSEffectListHandler::OnApplyRulesEntry(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (!pObject->AsSWSCreature()) {
        return 1;
    }
    return g_pRules->GetEntryB0(pEffect->GetInteger(0)) == NULL;
}

// FUNCTION: SWKOTOR 0x004DD470
int CSWSEffectListHandler::OnApplyCommandable(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    if (pObject->m_nObjectType == g_nObjectTypeA) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->SetCommandable(1);
        pCreature->ClearActions4ED060(0);
        return 1;
    }
    if (pObject->m_nObjectType == g_nObjectTypeB) {
        unsigned long oidSelf = pObject->m_idSelf;
        unsigned long oidCreator = pEffect->m_oidCreator;
        unsigned long nEvent = g_nEventRemoveAoE;
        GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0x7d0, oidCreator, oidSelf,
                                                                  nEvent, NULL);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DD4F0
int CSWSEffectListHandler::OnApplyCommandable2(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    if (pObject->m_nObjectType == g_nObjectTypeA) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->SetCommandable(1);
        pCreature->ClearActions4ECFF0();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DDE40
int CSWSEffectListHandler::OnApplyHitPoints126(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        if (pStats) {
            short nValue = pStats->m_nHitPoints126;
            pStats->m_nHitPoints126 = nValue + pEffect->GetInteger(0);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DDE80
int CSWSEffectListHandler::OnRemoveHitPoints126(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        if (pStats) {
            short nValue = pStats->m_nHitPoints126;
            pStats->m_nHitPoints126 = nValue - pEffect->GetInteger(0);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DDEC0
int CSWSEffectListHandler::OnApplyHitPoints124(CSWSObject* pObject, CGameEffect* pEffect,
                                               int bLoadingGame)
{
    int nAmount = pEffect->GetInteger(0);
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        pStats->m_nHitPoints124 = (pStats->GetCurrentHitPoints() - nAmount) < 0
                                      ? 0
                                      : (pStats->GetCurrentHitPoints() - nAmount);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DE4D0
int CSWSEffectListHandler::OnApplyDamage124(CSWSObject* pObject, CGameEffect* pEffect,
                                            int bLoadingGame)
{
    int nValue = 0;
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature && pCreature->m_pStats) {
        int nAmount = pEffect->GetInteger(0);
        if (pCreature->m_pStats->GetCurrentHitPoints() > nAmount) {
            nValue = pCreature->m_pStats->GetCurrentHitPoints() - nAmount;
        }
        pCreature->m_pStats->m_nHitPoints124 = nValue;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DE540
int CSWSEffectListHandler::OnApplyHeal124(CSWSObject* pObject, CGameEffect* pEffect,
                                          int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature && pCreature->m_pStats) {
        int nAmount = pEffect->GetInteger(0);
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        int nValue = (short)(pStats->m_nHitPoints126 + pStats->m_nHitPoints124) + nAmount;
        if (nValue > pCreature->GetMaxHitPoints()) {
            nValue = pCreature->GetMaxHitPoints();
        }
        pCreature->m_pStats->m_nHitPoints124 = nValue;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DBCE0
int CSWSEffectListHandler::OnApplyVisualCopy(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CGameEffect* pVisual = new CGameEffect(pEffect);
        pVisual->m_nType = 0x1e;
        pVisual->SetInteger(0, pEffect->GetInteger(0));
        pObject->AsSWSCreature()->ApplyEffect(pVisual, bLoadingGame, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DCFF0
int CSWSEffectListHandler::OnApplyPair2F49(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        CGameEffect* pFirst = new CGameEffect(pEffect);
        pFirst->m_nType = 0x2f;
        pFirst->SetInteger(0, 2);
        pFirst->SetInteger(1, g_pRules->m_nAA);
        pCreature->ApplyEffect(pFirst, bLoadingGame, 0);
        CGameEffect* pSecond = new CGameEffect(pEffect);
        pSecond->m_nType = 0x49;
        pSecond->SetInteger(0, 8);
        pCreature->ApplyEffect(pSecond, bLoadingGame, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DD9D0
int CSWSEffectListHandler::OnApplyTemporary54(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        if (pEffect->GetDurationType() == g_nDurationInstant) {
            CGameEffect* pNew = new CGameEffect(1);
            pNew->SetCreator(pEffect->m_oidCreator);
            pNew->m_nType = 0x54;
            pNew->SetDurationType(g_nDurationTemporary);
            pNew->m_fDuration = 6.0f;
            pObject->ApplyEffect(pNew, bLoadingGame, 0);
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DE400
int CSWSEffectListHandler::OnRemoveApplyVisual17(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        pCreature->Fn4F34A0(0x7f000000, 0);
        CGameEffect* pNew = new CGameEffect(1);
        pNew->SetDurationType(g_nDurationTemporary);
        pNew->m_fDuration = 2.55f;
        pNew->SetCreator(pNew->m_oidCreator);
        pNew->m_nType = 0x17;
        pNew->SetInteger(0, -0x15f);
        pCreature->ApplyEffect(pNew, 0, 0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DB8F0
int CSWSEffectListHandler::OnApplyPair1C30(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CGameEffect* pFirst = new CGameEffect(pEffect);
        pFirst->m_nType = 0x1c;
        pFirst->SetInteger(0, 0x96);
        pObject->ApplyEffect(pFirst, bLoadingGame, 0);
        CGameEffect* pSecond = new CGameEffect(pEffect);
        pSecond->SetInteger(0, 0);
        pSecond->SetInteger(1, 4);
        pSecond->SetInteger(2, g_pRules->m_nAA);
        pSecond->SetInteger(5, 0x4007);
        pSecond->m_nType = 0x30;
        pObject->ApplyEffect(pSecond, bLoadingGame, 0);
        pCreature->m_b8D0 = 0;
        pCreature->m_b8CC = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DC3B0
// NONMATCH (13): pObject / pEffect come out in ebx / edi instead of edi / ebx.
// Without the pCreature local the registers are right but a third
// AsSWSCreature call appears.
int CSWSEffectListHandler::OnApplyPair3119(CSWSObject* pObject, CGameEffect* pEffect,
                                           int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (!pCreature->m_bF8) {
            pCreature->m_b8E4 = 1;
            CGameEffect* pFirst = new CGameEffect(pEffect);
            pFirst->m_nType = 0x31;
            pFirst->SetInteger(0, 0);
            pFirst->SetInteger(1, pEffect->GetInteger(0));
            pFirst->SetInteger(2, g_pRules->m_nAA);
            pFirst->SetInteger(5, 0x4007);
            pObject->ApplyEffect(pFirst, bLoadingGame, 0);
            CGameEffect* pSecond = new CGameEffect(pEffect);
            pSecond->m_nType = 0x19;
            pSecond->SetInteger(0, 0x1e);
            pObject->ApplyEffect(pSecond, bLoadingGame, 0);
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC660
// NONMATCH (19): eax/ecx/edx choices for the +0x6c test, the duration-type
// compare and the subtype update differ; the instructions are otherwise identical.
int CSWSEffectListHandler::OnApplyFloat39(CSWSObject* pObject, CGameEffect* pEffect,
                                          int bLoadingGame)
{
    if (pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_pStats->m_b6C) {
        if (pEffect->GetDurationType() == g_nDurationInstant && pEffect->GetFloat(0) >= 0.1f) {
            CGameEffect* pNew = new CGameEffect(1);
            pNew->SetCreator(pEffect->m_oidCreator);
            pNew->m_nType = 0x39;
            pNew->SetDurationType(g_nDurationTemporary);
            pNew->SetFloat(0, pEffect->GetFloat(0));
            pNew->m_fDuration = (float)fabs(6.0f / pEffect->GetFloat(0));
            pObject->ApplyEffect(pNew, bLoadingGame, 0);
        }
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DD140
int CSWSEffectListHandler::OnApplyArea(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    Vector vPosition(pEffect->GetFloat(0), pEffect->GetFloat(1), pEffect->GetFloat(2));
    unsigned long oidArea = pEffect->GetObjectID(0);
    if (!GetServerExoApp()->GetAreaByGameObjectID(oidArea)) {
        return 1;
    }
    if (pObject->m_nObjectType == g_nObjectTypeA) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->SetCommandable(1);
        pCreature->Fn4FD960();
        pCreature->m_oid310 = oidArea;
        pCreature->m_v314 = pObject->AsSWSCreature()->m_vPosition;
        pCreature->m_n320 = 0;
        pCreature->m_pCombatRound->SetPauseTimer(0, 1);
        pCreature->m_pCombatRound->SetRoundPaused(1, pObject->m_idSelf);
        pCreature->ClearActions4ED060(1);
        return 0;
    }
    if (pObject->m_nObjectType == g_nObjectTypeB) {
        unsigned long nEvent = g_nEvent4;
        unsigned long oidSelf = pObject->m_idSelf;
        unsigned long oidCreator = pEffect->m_oidCreator;
        GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0x7d0, oidCreator, oidSelf,
                                                                  nEvent, NULL);
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D9C50
int CSWSEffectListHandler::OnRemoveCombatPause(CSWSObject* pObject, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature && pEffect) {
        if (pCreature->m_pCombatRound->m_bRoundPaused) {
            pCreature->m_pCombatRound->SetRoundPaused(0, 0x7f000000);
            pCreature->m_pCombatRound->SetPauseTimer(0, 0);
        }
        if (!pCreature->GetIsInConversation() && !pCreature->GetIsPossessed() &&
            !pCreature->GetDeadTemp()) {
            pCreature->SendFeedbackMessage(0x2711);
            pObject->m_nE8 = 1;
            return 1;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC200
// NONMATCH (35): same instructions, but pCreature/pData, pEffect/zero and
// pCreator/pCopy sit in a rotated set of edi/ebx/esi.
int CSWSEffectListHandler::OnApplyImmunityFeedback(CSWSObject* pObject, CGameEffect* pEffect,
                                                   int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CSWSCreature* pCreator =
            g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator);
        if (pCreature->m_pStats->GetEffectImmunity(0xb, pCreator)) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, pObject->m_idSelf);
            if (pCreator) {
                CSWCCMessageData* pCopy = new CSWCCMessageData;
                pData->CopyTo(pCopy);
                pCreator->SendFeedbackData(0x91, pCopy);
            }
            pObject->AsSWSCreature()->SendFeedbackData(0x91, pData);
            return 1;
        }
        if (pCreature->m_bF8) {
            return 1;
        }
        CGameEffect* pNew = new CGameEffect(pEffect);
        pNew->m_nType = 0x17;
        pNew->SetInteger(0, -9);
        pCreature->ApplyEffect(pNew, bLoadingGame, 0);
        pCreature->m_b8C8 = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DDF40
// NONMATCH (15): eax/edx choices for the target id and the subtype update.
int CSWSEffectListHandler::OnApplyTargetEvent(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        CServerExoApp* pServer = GetServerExoApp();
        CSWSCreature* pTarget = pServer->GetCreatureByGameObjectID(pEffect->GetObjectID(0));
        if (pTarget) {
            pCreature->Fn4F34A0(pTarget->m_idSelf, 0);
            CGameEffect* pEvent = new CGameEffect(pEffect);
            pEvent->m_nType = 0x67;
            pEvent->SetDurationType(g_nDurationTemporary);
            unsigned long oidTarget = pTarget->m_idSelf;
            pEvent->SetObjectID(0, oidTarget);
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 300, pCreature->m_idSelf, pCreature->m_idSelf, g_nEvent7441C8, pEvent);
            CGameEffect* pVisual = new CGameEffect(pEffect);
            pVisual->m_nType = 0x1e;
            pVisual->SetDurationType(g_nDurationInstant);
            pVisual->SetInteger(0, 0x3ea);
            pCreature->ApplyEffect(pVisual, 0, 0);
        }
    }
    return 0;
}
