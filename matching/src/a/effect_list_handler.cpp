// CSWSEffectListHandler (0x004D82B0-0x004D99AF; behavioural
// reference: src/batches/b004d_effect_list_handlers.cpp, b004d8_effect_handlers.cpp,
// b004d42_combat_round_lifecycle.cpp). Handlers ignore ECX (this unused).
#include "a/effect_list_handler.h"

// FUNCTION: SWKOTOR 0x004D82B0
CSWSEffectListHandler::~CSWSEffectListHandler()
{
    if (m_pApplyFunctions) {
        delete[] m_pApplyFunctions;
        m_pApplyFunctions = NULL;
    }
    if (m_pRemoveFunctions) {
        delete[] m_pRemoveFunctions;
        m_pRemoveFunctions = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004D83A0
int CSWSEffectListHandler::OnApply8D4(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    int bResult = 1;
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect && !pCreature->m_b8D4) {
            pCreature->m_b8D4 = 1;
            bResult = 0;
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004D83F0
int CSWSEffectListHandler::OnRemove8D4(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_b8D4 = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8420
int CSWSEffectListHandler::OnApplyAssuredDeflection(CSWSObject* pObject, CGameEffect* pEffect,
                                                    int bLoadingGame)
{
    int bResult = 1;
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (!pCreature->m_bAssuredDeflection && !pCreature->m_bAssuredDeflectionApplied && pEffect) {
            pCreature->m_bAssuredDeflectionApplied = 1;
            if (pEffect->GetInteger(0)) {
                pCreature->m_bAssuredDeflection = 1;
            }
            bResult = 0;
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004D8490
int CSWSEffectListHandler::OnRemoveAssuredDeflection(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_bAssuredDeflectionApplied = 0;
        pObject->AsSWSCreature()->m_bAssuredDeflection = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D84D0
int CSWSEffectListHandler::OnApplyAbilityIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                  int bLoadingGame)
{
    if (pEffect == NULL) {
        return 1;
    }
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nAmount = pEffect->GetInteger(1);
        if (bLoadingGame || (!pCreature->GetIsInConversation() && !pCreature->GetIsPossessed() &&
                             !pCreature->GetDeadTemp())) {
            return nAmount < 1;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8550
int CSWSEffectListHandler::OnRemoveAbilityIncrease(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nAbility = pEffect->GetInteger(0);
        int nAmount = pEffect->GetInteger(1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8590
int CSWSEffectListHandler::OnApplyAbilityDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                  int bLoadingGame)
{
    if (pEffect == NULL) {
        return 1;
    }
    if (pObject->AsSWSCreature() == NULL) {
        return 1;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    CSWSCreatureStats* pStats;
    if (pCreature) {
        pStats = pCreature->m_pStats;
    }
    if (pStats->GetEffectImmunity(0x13,
            g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator))) {
        return 1;
    }
    int nAmount = pEffect->GetInteger(1);
    if (!bLoadingGame) {
        if (pCreature->GetIsInConversation() || pCreature->GetIsPossessed() ||
            pCreature->GetDeadTemp()) {
            return 1;
        }
    }
    if (nAmount >= 1 && !pCreature->m_bF8) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8650
int CSWSEffectListHandler::OnRemoveAbilityDecrease(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int nAbility = pEffect->GetInteger(0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D8CB0
int CSWSEffectListHandler::OnApplySavingThrowIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                      int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect) {
            if (pEffect->GetInteger(0) < 1) {
                return 1;
            }
            pCreature->m_bUpdateCombatInformation = 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D8D00
int CSWSEffectListHandler::OnApplySavingThrowDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                      int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature->m_pStats->GetEffectImmunity(0x19,
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pEffect->m_oidCreator))) {
            return 1;
        }
        if (pEffect->GetInteger(0) < 1 || pCreature->m_bF8) {
            return 1;
        }
        pCreature->m_bUpdateCombatInformation = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9120
int CSWSEffectListHandler::OnApplyAttackIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    if (pObject->AsSWSCreature() && pEffect) {
        if (pEffect->GetInteger(0) < 1) {
            return 1;
        }
        pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9170
int CSWSEffectListHandler::OnApplyAttackDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    if (pObject->AsSWSCreature() && pEffect) {
        if (pEffect->GetInteger(0) < 1 || pObject->m_bF8) {
            return 1;
        }
        unsigned long oidCreator = pEffect->m_oidCreator;
        CSWSCreature* pCreator = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidCreator);
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        if (pStats->GetEffectImmunity(0x14, pCreator)) {
            return 1;
        }
        pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9200
int CSWSEffectListHandler::OnApplyFlag344(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9230
int CSWSEffectListHandler::OnApplyDamageDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                 int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        unsigned long oidCreator = pEffect->m_oidCreator;
        CSWSCreature* pCreator = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidCreator);
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        if (pStats->GetEffectImmunity(0x15, pCreator)) {
            return 1;
        }
        pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D92A0
int CSWSEffectListHandler::OnRemoveFlag344(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject->AsSWSCreature()) {
        pObject->AsSWSCreature()->m_bUpdateCombatInformation = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004D92D0
int CSWSEffectListHandler::OnApplyTemporaryHitpoints(CSWSObject* pObject, CGameEffect* pEffect,
                                                     int bLoadingGame)
{
    if (pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pEffect) {
            short nCurrent = (short)pCreature->m_nTemporaryHitPoints;
            pCreature->m_nTemporaryHitPoints = pEffect->GetInteger(0) + nCurrent;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9620
int CSWSEffectListHandler::OnApplyDamageImmunityIncrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                         int bLoadingGame)
{
    if (pEffect) {
        int nType = pEffect->GetInteger(0);
        int nAmount = pEffect->GetInteger(1);
        if (nAmount < 0) {
            return 1;
        }
        nAmount += pObject->GetDamageImmunity(nType);
        pObject->SetDamageImmunity(nType, nAmount);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9680
int CSWSEffectListHandler::OnApplyDamageImmunityDecrease(CSWSObject* pObject, CGameEffect* pEffect,
                                                         int bLoadingGame)
{
    if (pEffect) {
        int nType = pEffect->GetInteger(0);
        int nAmount = pEffect->GetInteger(1);
        if (nAmount < 0 || pObject->m_bF8) {
            return 1;
        }
        if (pObject->AsSWSCreature()) {
            unsigned long oidCreator = pEffect->m_oidCreator;
            CSWSCreature* pCreator =
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(oidCreator);
            CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
            if (pStats->GetEffectImmunity(0x16, pCreator)) {
                return 1;
            }
        }
        nAmount = pObject->GetDamageImmunity(nType) - nAmount;
        pObject->SetDamageImmunity(nType, nAmount);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D9970
int CSWSEffectListHandler::OnRemoveEntangled(CSWSObject* pObject, CGameEffect* pEffect)
{
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (!pCreature->m_b9D4) {
            pCreature->ClearVisibleList();
            pCreature->UpdateVisibleList(0);
        }
    }
    return 1;
}
