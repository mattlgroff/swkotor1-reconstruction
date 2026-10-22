// CSWSEffectListHandler handlers 0x004DA050-0x004DF510 (behavioural
// reference: src/batches/b004d_effect_list_handlers.cpp, b004d8_effect_handlers.cpp,
// b004d42_combat_round_lifecycle.cpp, src/unknown/sub_004d*). Handlers ignore ECX.
#include "a/effect_list_handler.h"

// FUNCTION: SWKOTOR 0x004DA050
int CSWSEffectListHandler::OnApplySetAIState(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect) {
            unsigned short nFlags = pCreature->m_nAIStateFlags;
            pCreature->m_nAIStateFlags = nFlags & pEffect->GetInteger(0);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DA260
int CSWSEffectListHandler::OnApplyEnemyAttackBonus(CSWSObject* pObject, CGameEffect* pEffect,
                                                   int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        if (pObject->AsSWSCreature()->m_bF8) {
            return 1;
        }
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        if (pEffect) {
            unsigned char nBonus = pStats->m_nEnemyAttackBonus;
            pStats->m_nEnemyAttackBonus = nBonus + pEffect->GetInteger(0);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DA910
int CSWSEffectListHandler::OnApplyMovementSpeedIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                        int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect->GetInteger(0) < 100) {
            pEffect->SetInteger(0, pEffect->GetInteger(0) + 100);
        }
        float fScale = pEffect->GetInteger(0) * 0.01f;
        pCreature->SetMovementRate(pCreature->GetMovementRate() * fScale);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DA990
int CSWSEffectListHandler::OnApplyMovementSpeedDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                        int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_pStats->GetEffectImmunity(0x18,
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator))) {
            return 1;
        }
        if (pEffect->GetInteger(0) >= 100 || pCreature->m_bF8) {
            return 1;
        }
        float fScale = 1.0f - pEffect->GetInteger(0) * 0.01f;
        pCreature->SetMovementRate(pCreature->GetMovementRate() * fScale);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DB010
int CSWSEffectListHandler::OnRemoveAreaOfEffect(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        unsigned long oidAoE = pEffect->GetObjectID(0);
        CGameObject* pAoE;
        if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidAoE, &pAoE) ==
                g_nGameObjectFound &&
            pAoE) {
            GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, oidAoE, oidAoE, g_nEventRemoveAoE, NULL);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DB180
int CSWSEffectListHandler::OnApplyForceResistanceIncrease(CSWSObject* pObject,
                                                          CGameEffect* pEffect, int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        int nValue = pEffect->GetInteger(0);
        if (nValue > 0x80) {
            nValue = 0x80;
        } else if (nValue < 0) {
            return 1;
        }
        if (pObject->AsSWSCreature()) {
            pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
        }
        if (nValue > pStats->GetForceResistance()) {
            pStats->SetForceResistance(nValue);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DB2C0
int CSWSEffectListHandler::OnApplyRegenerate(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        unsigned long nDay;
        unsigned long nTime;
        GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
        pEffect->SetInteger(2, nDay);
        pEffect->SetInteger(3, nTime);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DB810
int CSWSEffectListHandler::OnRemove9E0(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_n9E0 == 1 && pCreature->m_nEffectID9E8 == pEffect->m_nID) {
            pCreature->m_n9E0 = 0;
            pCreature->m_nEffectID9E8 = (unsigned __int64)-1;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DB870
int CSWSEffectListHandler::OnRemove9E4(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_b9E4 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC380
int CSWSEffectListHandler::OnRemove8C8(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_b8C8 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC500
int CSWSEffectListHandler::OnRemove8E4(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_b8E4 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DC530
int CSWSEffectListHandler::OnApply8E8(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        if (pObject->AsSWSCreature()->m_bF8) {
            return 1;
        }
        pObject->AsSWSCreature()->m_b8E8 = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DC580
int CSWSEffectListHandler::OnApplySkillIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                int bLoadingGame)
{
    int nSkill = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int nAlign = pEffect->GetInteger(3);
    if (nAmount < 0) {
        return 1;
    }
    return pObject->AsSWSCreature() == NULL;
}

// FUNCTION: SWKOTOR 0x004DC5D0
int CSWSEffectListHandler::OnApplySkillDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                int bLoadingGame)
{
    int nSkill = pEffect->GetInteger(0);
    int nAmount = pEffect->GetInteger(1);
    int nRace = pEffect->GetInteger(2);
    int nAlign = pEffect->GetInteger(3);
    if (nAmount < 0 || pObject->m_bF8 || !pObject->AsSWSCreature()) {
        return 1;
    }
    unsigned long oidCreator = pEffect->m_oidCreator;
    CSWSCreature* pCreator = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidCreator);
    CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
    return pStats->GetEffectImmunity(0x1b, pCreator) != 0;
}

// FUNCTION: SWKOTOR 0x004DCD00
int CSWSEffectListHandler::OnApply8EC(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        pCreature->m_n8EC |= 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DCFC0
int CSWSEffectListHandler::OnApplyMissChance(CSWSObject* pObject, CGameEffect* pEffect,
                                             int bLoadingGame)
{
    if (pEffect->GetInteger(0) > 100 || pEffect->GetInteger(0) < 1) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DD100
int CSWSEffectListHandler::OnApplyStats6C(CSWSObject* pObject, CGameEffect* pEffect,
                                          int bLoadingGame)
{
    if (pObject && pObject->AsSWSCreature() && !pObject->AsSWSCreature()->m_pStats->m_b6C) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DD970
int CSWSEffectListHandler::OnRemoveNegativeLevel(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nClass = pEffect->GetInteger(1);
        int nLevels = pCreature->m_pStats->GetClassNegativeLevels(nClass);
        CSWSCreatureStats* pStats = pCreature->m_pStats;
        pStats->SetClassNegativeLevels(nClass, nLevels - pEffect->GetInteger(0));
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DDD50
int CSWSEffectListHandler::OnApplyVampiricRegeneration(CSWSObject* pObject, CGameEffect* pEffect,
                                                       int bLoadingGame)
{
    CSWSCreature* pCreator =
        g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator);
    if (pCreator) {
        int nAmount = pEffect->GetInteger(0);
        int nMax = pEffect->GetInteger(1);
        CGameEffect* pHeal = new CGameEffect(1);
        pHeal->m_nType = 0x27;
        pHeal->SetDurationType(g_nDurationInstant);
        pHeal->SetCreator(pEffect->m_oidCreator);
        if (nMax < nAmount) {
            nAmount = nMax;
        }
        pHeal->SetInteger(0, nAmount);
        pCreator->ApplyEffect(pHeal, bLoadingGame, 0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DED00
int CSWSEffectListHandler::OnApplyForceResisted(CSWSObject* pObject, CGameEffect* pEffect,
                                                int bLoadingGame)
{
    CGameEffect* pVisual = new CGameEffect(1);
    if (pVisual) {
        pVisual->m_nType = 0x1e;
        pVisual->SetDurationType(g_nDurationInstant);
        pVisual->SetCreator(pObject->m_idSelf);
        pVisual->SetInteger(0, 0xfc5);
        pVisual->SetObjectID(0, pEffect->GetObjectID(0));
        pObject->ApplyEffect(pVisual, bLoadingGame, 0);
        pObject->SendFeedbackMessage(0x27a1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DEDE0
int CSWSEffectListHandler::OnApplyForceFizzle(CSWSObject* pObject, CGameEffect* pEffect,
                                              int bLoadingGame)
{
    CGameEffect* pVisual = new CGameEffect(1);
    if (pVisual) {
        pVisual->m_nType = 0x1e;
        pVisual->SetDurationType(g_nDurationInstant);
        pVisual->SetInteger(0, 0xfc4);
        pVisual->SetCreator(pObject->m_idSelf);
        pObject->ApplyEffect(pVisual, bLoadingGame, 0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004DF420
// NONMATCH (11): p2DA / nRow come out in ebx / edi instead of edi / ebx.
int CSWSEffectListHandler::IsEffectToIgnoreOnDeath(CGameEffect* pEffect)
{
    C2DAs* p2DAs = g_pRules->m_p2DArrays;
    C2DA* p2DA = p2DAs->m_pIgnoreOnDeath;
    for (int nRow = 0; nRow < p2DA->m_nNumRows; nRow++) {
        int nType;
        int bFound;
        {
            CExoString sColumn("EffectType");
            bFound = p2DA->GetINTEntry(nRow, sColumn, &nType);
        }
        if (bFound) {
            if (pEffect->GetSaveType() == nType) {
                return 1;
            }
            if (nType == 0x15 && pEffect->m_nType == 0x1e &&
                IsIgnoredVisualEffect(pEffect->GetInteger(0))) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004DF510
int CEffectIDList::CountMatches(unsigned __int64 nID)
{
    int nMatches = 0;
    for (int i = 0; i < num; i++) {
        if (element[i] == nID) {
            nMatches++;
        }
    }
    return nMatches;
}
