// CSWSItemPropertyHandler apply/remove handlers (0x004E5000-0x004EAFFF).
// Behaviour reference: src/batches/b004e6*.cpp, b004ea_*.cpp, b004e5_*.cpp.
#include "a/item_property_handler.h"

// FUNCTION: SWKOTOR 0x004E6A30
int CSWSItemPropertyHandler::ApplyTrueSeeing(CSWSItem* pItem, CSWItemProperty* pProperty,
                                             CSWSCreature* pCreature, unsigned long nSlot,
                                             int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_TRUESEEING);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7C00
int CSWSItemPropertyHandler::ApplyRegeneration(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot,
                                               int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    int nAmount = pProperty->m_nCostTableValue;
    if (nAmount <= 0) {
        return 0;
    }
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetEffectType(EFFECT_TYPE_REGENERATE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetInteger(0, nAmount);
    pEffect->SetInteger(1, 6000);
    pEffect->SetInteger(4, pProperty->m_nPropertyName);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7190
int CSWSItemPropertyHandler::ApplyDamageReduction(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                  CSWSCreature* pCreature, unsigned long nSlot,
                                                  int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_REDUCTION);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    int nValue;
    {
        CExoString sColumn("Amount");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(6)->GetINTEntry(nRow, sColumn, &nValue);
    }
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(1, pProperty->m_nSubType + 1);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E72B0
int CSWSItemPropertyHandler::ApplyDamageResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                   CSWSCreature* pCreature, unsigned long nSlot,
                                                   int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_RESISTANCE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    int nValue;
    {
        CExoString sColumn("Amount");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(7)->GetINTEntry(nRow, sColumn, &nValue);
    }
    pEffect->SetInteger(0, 1 << (unsigned char)pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E73D0
int CSWSItemPropertyHandler::ApplyDamageVulnerability(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature, unsigned long nSlot,
                                                      int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    unsigned short nDamageType = pProperty->m_nSubType;
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_VULNERABILITY);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x16)->GetINTEntry(nRow, sColumn, &nValue);
    }
    pEffect->SetInteger(0, 1 << nDamageType);
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9230
int CSWSItemPropertyHandler::ApplyImprovedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature, unsigned long nSlot,
                                                      int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue != 0) {
        ApplyChangedSavingThrow(pItem, nValue, pProperty, pCreature, bLoadingGame);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E92E0
int CSWSItemPropertyHandler::ApplyReducedSavingThrows(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature, unsigned long nSlot,
                                                      int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue != 0) {
        ApplyChangedSavingThrow(pItem, nValue, pProperty, pCreature, bLoadingGame);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9700
int CSWSItemPropertyHandler::RemoveBonusFeat(CSWSItem* pItem, CSWItemProperty* pProperty,
                                             CSWSCreature* pCreature, unsigned long nSlot)
{
    if (pCreature->m_pStats == 0) {
        return 0;
    }
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_BONUS_FEAT &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            unsigned short nFeat = pProperty->m_nSubType;
            if (pEffect->GetInteger(0) == nFeat &&
                pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
                pCreature->RemoveEffectById(pEffect->m_nID);
                break;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9BB0
int CSWSItemPropertyHandler::RemoveDamageReduction(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Amount");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(6)->GetINTEntry(nRow, sColumn, &nValue);
    }
    for (int i = pCreature->m_pStats->m_nDamageReductionEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_REDUCTION;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_REDUCTION &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            unsigned short nType = pProperty->m_nSubType;
            if (pEffect->GetInteger(1) == nType + 1) {
                pCreature->RemoveEffectById(pEffect->m_nID);
                break;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9CF0
int CSWSItemPropertyHandler::RemoveDamageResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    unsigned short nDamageType = pProperty->m_nSubType;
    int nValue;
    {
        CExoString sColumn("Amount");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(7)->GetINTEntry(nRow, sColumn, &nValue);
    }
    for (int i = pCreature->m_pStats->m_nDamageResistanceEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_RESISTANCE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_RESISTANCE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == 1 << nDamageType &&
            pEffect->GetInteger(1) == nValue) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9E30
int CSWSItemPropertyHandler::RemoveDamageVulnerability(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    unsigned short nDamageType = pProperty->m_nSubType;
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x16)->GetINTEntry(nRow, sColumn, &nValue);
    }
    for (int i = 0;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_VULNERABILITY;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_VULNERABILITY &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == 1 << nDamageType &&
            pEffect->GetInteger(1) == nValue) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA890
int CSWSItemPropertyHandler::RemoveImprovedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                       CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue != 0) {
        RemoveChangedSavingThrow(nValue, pItem, pProperty, pCreature);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA940
int CSWSItemPropertyHandler::RemoveReducedSavingThrow(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue != 0) {
        RemoveChangedSavingThrow(nValue, pItem, pProperty, pCreature);
    }
    return 0;
}
// FUNCTION: SWKOTOR 0x004E6390
int CSWSItemPropertyHandler::ApplyAbilityBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot,
                                               int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_ABILITY_INCREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    switch (pProperty->m_nSubType) {
    case 0: pEffect->SetInteger(0, ABILITY_STRENGTH); break;
    case 1: pEffect->SetInteger(0, ABILITY_DEXTERITY); break;
    case 2: pEffect->SetInteger(0, ABILITY_CONSTITUTION); break;
    case 3: pEffect->SetInteger(0, ABILITY_INTELLIGENCE); break;
    case 4: pEffect->SetInteger(0, ABILITY_WISDOM); break;
    case 5: pEffect->SetInteger(0, ABILITY_CHARISMA); break;
    }
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E74F0
int CSWSItemPropertyHandler::ApplyDecreasedAbilityScore(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                        CSWSCreature* pCreature, unsigned long nSlot,
                                                        int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x15)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_ABILITY_DECREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    switch (pProperty->m_nSubType) {
    case 0: pEffect->SetInteger(0, ABILITY_STRENGTH); break;
    case 1: pEffect->SetInteger(0, ABILITY_DEXTERITY); break;
    case 2: pEffect->SetInteger(0, ABILITY_CONSTITUTION); break;
    case 3: pEffect->SetInteger(0, ABILITY_INTELLIGENCE); break;
    case 4: pEffect->SetInteger(0, ABILITY_WISDOM); break;
    case 5: pEffect->SetInteger(0, ABILITY_CHARISMA); break;
    }
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E6510
int CSWSItemPropertyHandler::ApplyACBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                          CSWSCreature* pCreature, unsigned long nSlot,
                                          int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_AC_INCREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nACEnchantmentType);
    pEffect->SetInteger(1, nValue);
    pEffect->SetInteger(5, AC_DEFLECTION_BONUS_VS_DAMAGE);
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (pProperty->m_nPropertyName == 2) {
        int nACType = pProperty->m_nSubType;
        if (nACType > 0 && nACType <= 3) {
            pEffect->SetInteger(4, nACType);
        }
    }
    else if (pProperty->m_nPropertyName == 3) {
        pEffect->SetInteger(5, pProperty->m_nSubType);
    }
    else if (pProperty->m_nPropertyName == 4) {
        pEffect->SetInteger(2, pProperty->m_nSubType);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7670
int CSWSItemPropertyHandler::ApplyDecreasedAC(CSWSItem* pItem, CSWItemProperty* pProperty,
                                              CSWSCreature* pCreature, unsigned long nSlot,
                                              int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_AC_DECREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    pEffect->SetInteger(5, AC_DEFLECTION_BONUS_VS_DAMAGE);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E77C0
int CSWSItemPropertyHandler::ApplyDecreasedSkillModifier(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                         CSWSCreature* pCreature, unsigned long nSlot,
                                                         int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x15)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_SKILL_DECREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E8DB0
int CSWSItemPropertyHandler::RemoveAbilityBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    for (int i = pCreature->m_pStats->m_nAbilityEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_ABILITY_INCREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ABILITY_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(1) == nValue) {
            // Subtypes above 5 match on the value alone.
            switch (pProperty->m_nSubType) {
            case 0: if (pEffect->GetInteger(0) != ABILITY_STRENGTH) continue; break;
            case 1: if (pEffect->GetInteger(0) != ABILITY_DEXTERITY) continue; break;
            case 2: if (pEffect->GetInteger(0) != ABILITY_CONSTITUTION) continue; break;
            case 3: if (pEffect->GetInteger(0) != ABILITY_INTELLIGENCE) continue; break;
            case 4: if (pEffect->GetInteger(0) != ABILITY_WISDOM) continue; break;
            case 5: if (pEffect->GetInteger(0) != ABILITY_CHARISMA) continue; break;
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9F60
int CSWSItemPropertyHandler::RemoveDecreasedAbilityScore(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x15)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    for (int i = pCreature->m_pStats->m_nAbilityEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_ABILITY_DECREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ABILITY_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(1) == nValue) {
            // Subtypes above 5 match on the value alone.
            switch (pProperty->m_nSubType) {
            case 0: if (pEffect->GetInteger(0) != ABILITY_STRENGTH) continue; break;
            case 1: if (pEffect->GetInteger(0) != ABILITY_DEXTERITY) continue; break;
            case 2: if (pEffect->GetInteger(0) != ABILITY_CONSTITUTION) continue; break;
            case 3: if (pEffect->GetInteger(0) != ABILITY_INTELLIGENCE) continue; break;
            case 4: if (pEffect->GetInteger(0) != ABILITY_WISDOM) continue; break;
            case 5: if (pEffect->GetInteger(0) != ABILITY_CHARISMA) continue; break;
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// NONMATCH (23 insns): ebx/ebp swapped for pProperty and the index, and the
// subtype range check is a signed int compare on movzx(di) in the original
// (test eax,eax / jle; cmp eax,3 / jg) where we get 16-bit test di / jbe.
// FUNCTION: SWKOTOR 0x004E8F70
int CSWSItemPropertyHandler::RemoveACBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                           CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(1)->GetINTEntry(nRow, sColumn, &nValue);
    }
    int nACType = g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nACEnchantmentType;
    for (int i = pCreature->m_pStats->m_nACEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_AC_INCREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_AC_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nACType &&
            pEffect->GetInteger(1) == nValue) {
            if (pProperty->m_nPropertyName == 2) {
                unsigned short nSub = pProperty->m_nSubType;
                if (nSub > 0 && nSub <= 3 && pEffect->GetInteger(4) != nSub) {
                    continue;
                }
            }
            else if (pProperty->m_nPropertyName == 4) {
                unsigned short nSub = pProperty->m_nSubType;
                if (pEffect->GetInteger(2) != nSub) {
                    continue;
                }
            }
            else if (pProperty->m_nPropertyName == 3) {
                unsigned short nSub = pProperty->m_nSubType;
                if (pEffect->GetInteger(5) != nSub) {
                    continue;
                }
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA130
int CSWSItemPropertyHandler::RemoveDecreasedAC(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    for (int i = pCreature->m_pStats->m_nACEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_AC_DECREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_AC_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            unsigned short nSub = pProperty->m_nSubType;
            if (pEffect->GetInteger(0) == nSub && pEffect->GetInteger(1) == nValue) {
                unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
                if (pEffect->GetInteger(2) == nRacialType) {
                    pCreature->RemoveEffectById(pEffect->m_nID);
                    break;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA290
int CSWSItemPropertyHandler::RemoveDecreasedSkillModifier(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                          CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x15)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    for (int i = pCreature->m_pStats->m_nSkillEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_SKILL_DECREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_SKILL_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            unsigned short nSub = pProperty->m_nSubType;
            if (pEffect->GetInteger(0) == nSub && pEffect->GetInteger(1) == nValue) {
                unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
                if (pEffect->GetInteger(2) == nRacialType) {
                    pCreature->RemoveEffectById(pEffect->m_nID);
                    break;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E66B0
int CSWSItemPropertyHandler::ApplyImprovedForceResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                          CSWSCreature* pCreature, unsigned long nSlot,
                                                          int bLoadingGame)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0xb)->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_FORCE_RESISTANCE_INCREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9110
int CSWSItemPropertyHandler::RemoveImprovedForceResistance(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                           CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0xb)->GetINTEntry(nRow, sColumn, &nValue);
    }
    for (int i = pCreature->m_pStats->m_nForceResistanceEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_FORCE_RESISTANCE_INCREASE;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_FORCE_RESISTANCE_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E67D0
int CSWSItemPropertyHandler::ApplyChangedSavingThrow(CSWSItem* pItem, int nValue,
                                                     CSWItemProperty* pProperty,
                                                     CSWSCreature* pCreature, int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    if (nValue >= 0) {
        pEffect->SetEffectType(EFFECT_TYPE_SAVING_THROW_INCREASE);
    }
    else {
        pEffect->SetEffectType(EFFECT_TYPE_SAVING_THROW_DECREASE);
        nValue = -nValue;
    }
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    switch (pProperty->m_nSubType) {
    case 1: pEffect->SetInteger(1, 1); break;
    case 2: pEffect->SetInteger(1, 2); break;
    case 3: pEffect->SetInteger(1, 3); break;
    }
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(2, 0);
    pEffect->SetInteger(3, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9390
int CSWSItemPropertyHandler::RemoveChangedSavingThrow(int nValue, CSWSItem* pItem,
                                                      CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature)
{
    int nSaveType = pProperty->m_nSubType;
    for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if ((pEffect->m_nType == EFFECT_TYPE_SAVING_THROW_INCREASE ||
             pEffect->m_nType == EFFECT_TYPE_SAVING_THROW_DECREASE) &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue &&
            pEffect->GetInteger(1) == 0) {
            unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
            if (pEffect->GetInteger(3) == nRacialType) {
                int nType = pCreature->m_appliedEffects.element[i]->GetInteger(1);
                if ((nType == 1 && nSaveType == 1) || (nType == 3 && nSaveType == 3) ||
                    (nType == 2 && nSaveType == 2)) {
                    pCreature->RemoveEffectById(pCreature->m_appliedEffects.element[i]->m_nID);
                    break;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E60C0
int CSWSItemPropertyHandler::ApplyAttackPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                CSWSCreature* pCreature, unsigned long nSlot,
                                                int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_ATTACK_DECREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pEffect->SetCreator(oidItem);
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (nSlot == 0x10) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_ONHAND);
    }
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pEffect, 0);
        pOffHand->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E6F60
int CSWSItemPropertyHandler::ApplyDamagePenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                CSWSCreature* pCreature, unsigned long nSlot,
                                                int bLoadingGame)
{
    g_pRules->GetBaseItem(pItem->m_nBaseItem);  // result unused
    CGameEffect* pEffect = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_DECREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pEffect->SetCreator(oidItem);
    if (nSlot == 0x10) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pEffect->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_ONHAND);
    }
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(1, pItem->GetDamageFlags());
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pEffect, 0);
        pOffHand->SetInteger(5, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E83A0
int CSWSItemPropertyHandler::RemoveEnhancementPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                      CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    int i = pCreature->m_pStats->m_nAttackEffectStart;
    while (i < pCreature->m_appliedEffects.num &&
           pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_DECREASE) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ATTACK_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
            if (pEffect->GetInteger(2) == nRacialType) {
                if (nSlot == 0x10) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x20) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x4000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW1) { i++; continue; }
                }
                else if (nSlot == 0x8000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW2) { i++; continue; }
                }
                else if (nSlot == 0x10000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW3) { i++; continue; }
                }
                else if (nSlot == 8) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_ONHAND) { i++; continue; }
                }
                else {
                    i++;
                    continue;
                }
                pCreature->RemoveEffectById(pEffect->m_nID);
                i = pCreature->m_pStats->m_nAttackEffectStart;
                continue;
            }
        }
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
            if (pEffect->GetInteger(2) == nRacialType) {
                if (nSlot == 0x10) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x20) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x4000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW1) { i++; continue; }
                }
                else if (nSlot == 0x8000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW2) { i++; continue; }
                }
                else if (nSlot == 0x10000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW3) { i++; continue; }
                }
                else if (nSlot == 8) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_ONHAND) { i++; continue; }
                }
                else {
                    i++;
                    continue;
                }
                if (pEffect->GetInteger(1) & pItem->GetDamageFlags()) {
                    pCreature->RemoveEffectById(pEffect->m_nID);
                    break;
                }
            }
        }
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E8BC0
int CSWSItemPropertyHandler::RemoveAttackPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                 CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    int i = pCreature->m_pStats->m_nAttackEffectStart;
    while (i < pCreature->m_appliedEffects.num &&
           pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_DECREASE) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ATTACK_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
            if (pEffect->GetInteger(2) == nRacialType) {
                if (nSlot == 0x10) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x20) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x4000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW1) { i++; continue; }
                }
                else if (nSlot == 0x8000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW2) { i++; continue; }
                }
                else if (nSlot == 0x10000) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW3) { i++; continue; }
                }
                else if (nSlot == 8) {
                    if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_ONHAND) { i++; continue; }
                }
                else {
                    i++;
                    continue;
                }
                pCreature->RemoveEffectById(pEffect->m_nID);
                break;
            }
        }
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9990
int CSWSItemPropertyHandler::RemoveDamagePenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                 CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    int i = pCreature->m_pStats->m_nAttackEffectStart;
    while (i < pCreature->m_appliedEffects.num &&
           pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_DECREASE) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_DECREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
            if (pEffect->GetInteger(2) == nRacialType) {
                if (nSlot == 0x10) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x20) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON) { i++; continue; }
                }
                else if (nSlot == 0x4000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW1) { i++; continue; }
                }
                else if (nSlot == 0x8000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW2) { i++; continue; }
                }
                else if (nSlot == 0x10000) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW3) { i++; continue; }
                }
                else if (nSlot == 8) {
                    if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_ONHAND) { i++; continue; }
                }
                else {
                    i++;
                    continue;
                }
                if ((pEffect->GetInteger(1) & pItem->GetDamageFlags()) == 0) {
                    i++;
                    continue;
                }
                pCreature->RemoveEffectById(pEffect->m_nID);
                break;
            }
        }
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E8950
int CSWSItemPropertyHandler::RemoveAttackBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        C2DA* pTable = g_pRules->Get2DArrays()->m_pIPRPEnhancementTable;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    int i = pCreature->m_pStats->m_nAttackEffectStart;
    while (i < pCreature->m_appliedEffects.num &&
           pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_ATTACK_INCREASE) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ATTACK_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            if (nSlot == 0x10) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x20) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x4000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW1) { i++; continue; }
            }
            else if (nSlot == 0x8000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW2) { i++; continue; }
            }
            else if (nSlot == 0x10000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW3) { i++; continue; }
            }
            else if (nSlot == 8) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_ONHAND) { i++; continue; }
            }
            else {
                { i++; continue; }
            }
            if (pProperty->m_nPropertyName == 0x26 || pProperty->m_nPropertyName == 0x27) {
                unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
                if (pEffect->GetInteger(2) != nRacialType) { i++; continue; }
            }
            if (pProperty->m_nPropertyName == 0x27) {
                if (pProperty->m_nSubType == 1) {
                    if (pEffect->GetInteger(3) != 1) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 2) {
                    if (pEffect->GetInteger(4) != 2) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 3) {
                    if (pEffect->GetInteger(4) != 3) { i++; continue; }
                }
            }
            else if (pProperty->m_nPropertyName == 0x28) {
                unsigned short nSub = pProperty->m_nSubType;
                if (pEffect->GetInteger(2) != nSub) { i++; continue; }
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7F40
int CSWSItemPropertyHandler::RemoveEnhancementBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                    CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        C2DA* pTable = g_pRules->Get2DArrays()->m_pIPRPEnhancementTable;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    int i = pCreature->m_pStats->m_nAttackEffectStart;
    while (i < pCreature->m_appliedEffects.num &&
           pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_DAMAGE_INCREASE) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_ATTACK_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            if (nSlot == 0x10) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x20) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x4000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW1) { i++; continue; }
            }
            else if (nSlot == 0x8000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW2) { i++; continue; }
            }
            else if (nSlot == 0x10000) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_CLAW3) { i++; continue; }
            }
            else if (nSlot == 8) {
                if (pEffect->GetInteger(1) != ITEM_SLOT_ATTACK_TYPE_ONHAND) { i++; continue; }
            }
            else {
                i++;
                continue;
            }
            if (pProperty->m_nPropertyName == 5 || pProperty->m_nPropertyName == 6) {
                unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
                if (pEffect->GetInteger(2) != nRacialType) { i++; continue; }
            }
            if (pProperty->m_nPropertyName == 6) {
                if (pProperty->m_nSubType == 1) {
                    if (pEffect->GetInteger(3) != 1) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 2) {
                    if (pEffect->GetInteger(4) != 2) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 3) {
                    if (pEffect->GetInteger(4) != 3) { i++; continue; }
                }
            }
            else if (pProperty->m_nPropertyName == 7) {
                unsigned short nSub = pProperty->m_nSubType;
                if (pEffect->GetInteger(2) != nSub) { i++; continue; }
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            i = pCreature->m_pStats->m_nAttackEffectStart;
            continue;
        }
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_INCREASE &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetInteger(0) == nValue) {
            if (nSlot == 0x10) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x20) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON) { i++; continue; }
            }
            else if (nSlot == 0x4000) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW1) { i++; continue; }
            }
            else if (nSlot == 0x8000) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW2) { i++; continue; }
            }
            else if (nSlot == 0x10000) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_CLAW3) { i++; continue; }
            }
            else if (nSlot == 8) {
                if (pEffect->GetInteger(5) != ITEM_SLOT_DAMAGE_TYPE_ONHAND) { i++; continue; }
            }
            else {
                i++;
                continue;
            }
            if ((pEffect->GetInteger(1) & pItem->GetDamageFlags()) == 0) { i++; continue; }
            if (pProperty->m_nPropertyName == 5 || pProperty->m_nPropertyName == 6) {
                unsigned char nRacialType = g_pRules->m_nDifficultyRacialType;
                if (pEffect->GetInteger(2) != nRacialType) { i++; continue; }
            }
            if (pProperty->m_nPropertyName == 6) {
                if (pProperty->m_nSubType == 1) {
                    if (pEffect->GetInteger(3) != 1) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 2) {
                    if (pEffect->GetInteger(4) != 2) { i++; continue; }
                }
                else if (pProperty->m_nSubType == 3) {
                    if (pEffect->GetInteger(4) != 3) { i++; continue; }
                }
            }
            else if (pProperty->m_nPropertyName == 7) {
                unsigned short nSub = pProperty->m_nSubType;
                if (pEffect->GetInteger(2) != nSub) { i++; continue; }
            }
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
        i++;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E5E80
int CSWSItemPropertyHandler::ApplyAttackBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                              CSWSCreature* pCreature, unsigned long nSlot,
                                              int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        C2DA* pTable = g_pRules->Get2DArrays()->m_pIPRPEnhancementTable;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_ATTACK_INCREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pEffect->SetCreator(oidItem);
    pEffect->SetInteger(0, nValue);
    if (nSlot == 0x10) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pEffect->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_ONHAND);
    }
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (pProperty->m_nPropertyName == 0x27) {
        if (pProperty->m_nSubType == 1) {
            pEffect->SetInteger(3, 1);
        }
        else if (pProperty->m_nSubType == 2) {
            pEffect->SetInteger(4, 2);
        }
        else if (pProperty->m_nSubType == 3) {
            pEffect->SetInteger(4, 3);
        }
    }
    else if (pProperty->m_nPropertyName == 0x28) {
        unsigned short nSub = pProperty->m_nSubType;
        pEffect->SetInteger(2, nSub);
    }
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pEffect, 0);
        pOffHand->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E5C60
int CSWSItemPropertyHandler::ApplyDamageBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                              CSWSCreature* pCreature, unsigned long nSlot,
                                              int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    int nValue = pProperty->m_nCostTableValue;
    if (nValue <= 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_INCREASE);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pEffect->SetCreator(oidItem);
    pEffect->SetInteger(0, nValue);
    if (pProperty->m_nPropertyName == 0xb) {
        unsigned short nDamageType = pProperty->m_nSubType;
        pEffect->SetInteger(1, 1 << nDamageType);
    }
    else {
        pEffect->SetInteger(1, 1 << pProperty->m_nParam1Value);
    }
    if (nSlot == 0x10) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pEffect->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_ONHAND);
    }
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (pProperty->m_nPropertyName == 0xc) {
        unsigned short nSub = pProperty->m_nSubType;
        if (nSub == 1) {
            pEffect->SetInteger(3, 1);
        }
        else if (nSub == 2) {
            pEffect->SetInteger(4, 2);
        }
        else if (nSub == 3) {
            pEffect->SetInteger(4, 3);
        }
    }
    else if (pProperty->m_nPropertyName == 0xd) {
        pEffect->SetInteger(2, pProperty->m_nSubType);
    }
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pEffect, 0);
        pOffHand->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// NONMATCH (63 insns, all one register swap): ebx/ebp exchanged between the
// {EH state -1, nSlot} and {pProperty, pOffHand, pCreature} webs; every other
// byte matches. Same swap in ApplyEnhancementPenalty.
// FUNCTION: SWKOTOR 0x004E5490
int CSWSItemPropertyHandler::ApplyEnhancementBonus(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                   CSWSCreature* pCreature, unsigned long nSlot,
                                                   int bLoadingGame)
{
    g_pRules->GetBaseItem(pItem->m_nBaseItem);  // result unused
    CGameEffect* pAttack = new CGameEffect(1);
    CGameEffect* pDamage = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        C2DA* pTable = g_pRules->Get2DArrays()->m_pIPRPEnhancementTable;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    if (nValue == 0) {
        return 0;
    }
    pAttack->SetEffectType(EFFECT_TYPE_ATTACK_INCREASE);
    pAttack->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pAttack->SetCreator(oidItem);
    pAttack->SetInteger(0, nValue);
    if (nSlot == 0x10) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_ONHAND);
    }
    pAttack->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (pProperty->m_nPropertyName == 6) {
        if (pProperty->m_nSubType == 1) {
            pAttack->SetInteger(3, 1);
        }
        else if (pProperty->m_nSubType == 2) {
            pAttack->SetInteger(4, 2);
        }
        else if (pProperty->m_nSubType == 3) {
            pAttack->SetInteger(4, 3);
        }
    }
    else if (pProperty->m_nPropertyName == 7) {
        unsigned short nSub = pProperty->m_nSubType;
        pAttack->SetInteger(2, nSub);
    }
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pAttack, 0);
        pOffHand->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pAttack, bLoadingGame, 0);

    pDamage->SetEffectType(EFFECT_TYPE_DAMAGE_INCREASE);
    pDamage->SetSubType(SUBTYPE_EQUIPPED);
    oidItem = pItem->m_idSelf;
    pDamage->SetCreator(oidItem);
    if (nSlot == 0x10) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_ONHAND);
    }
    pDamage->SetInteger(0, nValue);
    pDamage->SetInteger(1, pItem->GetDamageFlags());
    pDamage->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (pProperty->m_nPropertyName == 6) {
        if (pProperty->m_nSubType == 1) {
            pDamage->SetInteger(3, 1);
        }
        else if (pProperty->m_nSubType == 2) {
            pDamage->SetInteger(4, 2);
        }
        else if (pProperty->m_nSubType == 3) {
            pDamage->SetInteger(4, 3);
        }
    }
    else if (pProperty->m_nPropertyName == 7) {
        unsigned short nSub = pProperty->m_nSubType;
        pDamage->SetInteger(2, nSub);
    }
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pDamage, 0);
        pOffHand->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pDamage, bLoadingGame, 0);
    return 0;
}

// NONMATCH (58 insns, all one register swap): ebx/ebp exchanged between the
// {EH state -1, nSlot} and {pItem, pOffHand, pCreature} webs.
// FUNCTION: SWKOTOR 0x004E58C0
int CSWSItemPropertyHandler::ApplyEnhancementPenalty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                     CSWSCreature* pCreature, unsigned long nSlot,
                                                     int bLoadingGame)
{
    g_pRules->GetBaseItem(pItem->m_nBaseItem);  // result unused
    CGameEffect* pAttack = new CGameEffect(1);
    CGameEffect* pDamage = new CGameEffect(1);
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        g_pRules->Get2DArrays()->GetIPRPCostTable(0x14)->GetINTEntry(nRow, sColumn, &nValue);
    }
    nValue = -nValue;
    if (nValue == 0) {
        return 0;
    }
    pAttack->SetEffectType(EFFECT_TYPE_ATTACK_DECREASE);
    pAttack->SetSubType(SUBTYPE_EQUIPPED);
    unsigned long oidItem = pItem->m_idSelf;
    pAttack->SetCreator(oidItem);
    pAttack->SetInteger(0, nValue);
    pAttack->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (nSlot == 0x10) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pAttack->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_ONHAND);
    }
    unsigned char nWield = g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield;
    if (nWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pAttack, 0);
        pOffHand->SetInteger(1, ITEM_SLOT_ATTACK_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pAttack, bLoadingGame, 0);

    pDamage->SetEffectType(EFFECT_TYPE_DAMAGE_DECREASE);
    pDamage->SetSubType(SUBTYPE_EQUIPPED);
    oidItem = pItem->m_idSelf;
    pDamage->SetCreator(oidItem);
    if (nSlot == 0x10) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_RIGHT_WEAPON);
    }
    else if (nSlot == 0x20) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
    }
    else if (nSlot == 0x4000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW1);
    }
    else if (nSlot == 0x8000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW2);
    }
    else if (nSlot == 0x10000) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_CLAW3);
    }
    else if (nSlot == 8) {
        pDamage->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_ONHAND);
    }
    pDamage->SetInteger(0, nValue);
    pDamage->SetInteger(1, pItem->GetDamageFlags());
    pDamage->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
        CGameEffect* pOffHand = new CGameEffect(0);
        pOffHand->CopyEffect(pDamage, 0);
        pOffHand->SetInteger(5, ITEM_SLOT_DAMAGE_TYPE_LEFT_WEAPON);
        pCreature->ApplyEffect(pOffHand, bLoadingGame, 0);
    }
    pCreature->ApplyEffect(pDamage, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E68E0
int CSWSItemPropertyHandler::ApplySpecialWalk(CSWSItem* pItem, CSWItemProperty* pProperty,
                                              CSWSCreature* pCreature, unsigned long nSlot,
                                              int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetSubTypeCategory(8);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetEffectType(EFFECT_TYPE_SPECIALWALK);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);

    pEffect = new CGameEffect(1);
    pEffect->SetSubTypeCategory(8);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetEffectType(EFFECT_TYPE_SPECIALWALK_END);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E6AD0
int CSWSItemPropertyHandler::ApplyFreedomOfMovement(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                    CSWSCreature* pCreature, unsigned long nSlot,
                                                    int bLoadingGame)
{
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetInteger(0, IMMUNITY_TYPE_PARALYSIS);
    pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);

    pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetInteger(0, IMMUNITY_TYPE_ENTANGLE);
    pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);

    pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetInteger(0, IMMUNITY_TYPE_SLOW);
    pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);

    pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pEffect->SetInteger(0, IMMUNITY_TYPE_MOVEMENT_SPEED_DECREASE);
    pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E6E20
int CSWSItemPropertyHandler::ApplyDamageImmunity(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                 CSWSCreature* pCreature, unsigned long nSlot,
                                                 int bLoadingGame)
{
    unsigned char nTable = pProperty->m_nCostTable;
    C2DA* pTable = g_pRules->Get2DArrays()->GetIPRPCostTable(nTable);
    if (pTable == 0) {
        return 0;
    }
    unsigned short nDamageType = pProperty->m_nSubType;
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    int nDamageFlag = 1 << nDamageType;
    CGameEffect* pEffect = new CGameEffect(1);
    if (pEffect == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_DAMAGE_IMMUNITY);
    pEffect->SetSubType(3);
    pEffect->SetInteger(0, nDamageFlag);
    pEffect->SetInteger(1, nValue);
    pEffect->SetCreator(pItem->m_idSelf);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    return 0;
}

// Bytes identical (0 diff insns); matchcmp still reports a length mismatch
// because the COMDAT ends in the jump table and the next function
// (0x004E7B10) has no functions.csv row.
// FUNCTION: SWKOTOR 0x004E7900
int CSWSItemPropertyHandler::ApplyImmunity(CSWSItem* pItem, CSWItemProperty* pProperty,
                                           CSWSCreature* pCreature, unsigned long nSlot,
                                           int bLoadingGame)
{
    int nImmunity;
    int nSecondImmunity;
    int bSecond = 0;
    switch (pProperty->m_nSubType) {
    case 0: nImmunity = IMMUNITY_TYPE_1E; break;
    case 1:
        nSecondImmunity = IMMUNITY_TYPE_13;
        nImmunity = IMMUNITY_TYPE_1D;
        bSecond = 1;
        break;
    case 2: nImmunity = IMMUNITY_TYPE_01; break;
    case 3: nImmunity = IMMUNITY_TYPE_02; break;
    case 4: nImmunity = IMMUNITY_TYPE_03; break;
    case 5: nImmunity = IMMUNITY_TYPE_04; break;
    case 6: nImmunity = IMMUNITY_TYPE_1C; break;
    case 7: nImmunity = IMMUNITY_TYPE_PARALYSIS; break;
    case 8: nImmunity = IMMUNITY_TYPE_1F; break;
    case 9: nImmunity = IMMUNITY_TYPE_20; break;
    default: return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    if (pEffect == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
    pEffect->SetSubType(3);
    pEffect->SetInteger(0, nImmunity);
    pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
    {
        unsigned long oidItem = pItem->m_idSelf;
        pEffect->SetCreator(oidItem);
    }
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    if (bSecond) {
        pEffect = new CGameEffect(1);
        if (pEffect == 0) {
            return 0;
        }
        pEffect->SetEffectType(EFFECT_TYPE_IMMUNITY);
        pEffect->SetSubType(3);
        pEffect->SetInteger(0, nSecondImmunity);
        pEffect->SetInteger(1, g_pRules->m_nDifficultyRacialType);
        pEffect->SetCreator(pItem->m_idSelf);
        pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E94B0
int CSWSItemPropertyHandler::RemoveSpecialWalk(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot)
{
    for (int i = pCreature->m_pStats->m_nSpecialWalkEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_SPECIALWALK_END;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_SPECIALWALK &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            unsigned short nWalk = pProperty->m_nSubType;
            if (pEffect->GetInteger(0) == nWalk && pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
                pCreature->RemoveEffectById(pEffect->m_nID);
                continue;
            }
        }
        if (pEffect->m_nType == EFFECT_TYPE_SPECIALWALK_END &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA9F0
void CSWSItemPropertyHandler::InitializeItemProperties()
{
    m_pApplyFunctions = new ItemPropertyApplyFn[60];
    m_pRemoveFunctions = new ItemPropertyRemoveFn[60];
    for (int i = 0; i < 60; i++) {
        m_pApplyFunctions[i] = 0;
        m_pRemoveFunctions[i] = 0;
    }
    m_pApplyFunctions[0] = &CSWSItemPropertyHandler::ApplyAbilityBonus;
    m_pRemoveFunctions[0] = &CSWSItemPropertyHandler::RemoveAbilityBonus;
    m_pApplyFunctions[1] = &CSWSItemPropertyHandler::ApplyACBonus;
    m_pRemoveFunctions[1] = &CSWSItemPropertyHandler::RemoveACBonus;
    m_pApplyFunctions[2] = &CSWSItemPropertyHandler::ApplyACBonus;
    m_pRemoveFunctions[2] = &CSWSItemPropertyHandler::RemoveACBonus;
    m_pApplyFunctions[3] = &CSWSItemPropertyHandler::ApplyACBonus;
    m_pRemoveFunctions[3] = &CSWSItemPropertyHandler::RemoveACBonus;
    m_pApplyFunctions[4] = &CSWSItemPropertyHandler::ApplyACBonus;
    m_pRemoveFunctions[4] = &CSWSItemPropertyHandler::RemoveACBonus;
    m_pApplyFunctions[5] = &CSWSItemPropertyHandler::ApplyEnhancementBonus;
    m_pRemoveFunctions[5] = &CSWSItemPropertyHandler::RemoveEnhancementBonus;
    m_pApplyFunctions[6] = &CSWSItemPropertyHandler::ApplyEnhancementBonus;
    m_pRemoveFunctions[6] = &CSWSItemPropertyHandler::RemoveEnhancementBonus;
    m_pApplyFunctions[7] = &CSWSItemPropertyHandler::ApplyEnhancementBonus;
    m_pRemoveFunctions[7] = &CSWSItemPropertyHandler::RemoveEnhancementBonus;
    m_pApplyFunctions[8] = &CSWSItemPropertyHandler::ApplyEnhancementPenalty;
    m_pRemoveFunctions[8] = &CSWSItemPropertyHandler::RemoveEnhancementPenalty;
    m_pApplyFunctions[9] = &CSWSItemPropertyHandler::ApplyBonusFeat;
    m_pRemoveFunctions[9] = &CSWSItemPropertyHandler::RemoveBonusFeat;
    m_pApplyFunctions[11] = &CSWSItemPropertyHandler::ApplyDamageBonus;
    m_pRemoveFunctions[11] = &CSWSItemPropertyHandler::RemoveDamageBonus;
    m_pApplyFunctions[12] = &CSWSItemPropertyHandler::ApplyDamageBonus;
    m_pRemoveFunctions[12] = &CSWSItemPropertyHandler::RemoveDamageBonus;
    m_pApplyFunctions[13] = &CSWSItemPropertyHandler::ApplyDamageBonus;
    m_pRemoveFunctions[13] = &CSWSItemPropertyHandler::RemoveDamageBonus;
    m_pApplyFunctions[14] = &CSWSItemPropertyHandler::ApplyDamageImmunity;
    m_pRemoveFunctions[14] = &CSWSItemPropertyHandler::RemoveDamageImmunity;
    m_pApplyFunctions[15] = &CSWSItemPropertyHandler::ApplyDamagePenalty;
    m_pRemoveFunctions[15] = &CSWSItemPropertyHandler::RemoveDamagePenalty;
    m_pApplyFunctions[16] = &CSWSItemPropertyHandler::ApplyDamageReduction;
    m_pRemoveFunctions[16] = &CSWSItemPropertyHandler::RemoveDamageReduction;
    m_pApplyFunctions[17] = &CSWSItemPropertyHandler::ApplyDamageResistance;
    m_pRemoveFunctions[17] = &CSWSItemPropertyHandler::RemoveDamageResistance;
    m_pApplyFunctions[18] = &CSWSItemPropertyHandler::ApplyDamageVulnerability;
    m_pRemoveFunctions[18] = &CSWSItemPropertyHandler::RemoveDamageVulnerability;
    m_pApplyFunctions[19] = &CSWSItemPropertyHandler::ApplyDecreasedAbilityScore;
    m_pRemoveFunctions[19] = &CSWSItemPropertyHandler::RemoveDecreasedAbilityScore;
    m_pApplyFunctions[20] = &CSWSItemPropertyHandler::ApplyDecreasedAC;
    m_pRemoveFunctions[20] = &CSWSItemPropertyHandler::RemoveDecreasedAC;
    m_pApplyFunctions[21] = &CSWSItemPropertyHandler::ApplyDecreasedSkillModifier;
    m_pRemoveFunctions[21] = &CSWSItemPropertyHandler::RemoveDecreasedSkillModifier;
    m_pApplyFunctions[24] = &CSWSItemPropertyHandler::ApplyImmunity;
    m_pRemoveFunctions[24] = &CSWSItemPropertyHandler::RemoveImmunity;
    m_pApplyFunctions[25] = &CSWSItemPropertyHandler::ApplyImprovedForceResistance;
    m_pRemoveFunctions[25] = &CSWSItemPropertyHandler::RemoveImprovedForceResistance;
    m_pApplyFunctions[26] = &CSWSItemPropertyHandler::ApplyImprovedSavingThrow;
    m_pRemoveFunctions[26] = &CSWSItemPropertyHandler::RemoveImprovedSavingThrow;
    m_pApplyFunctions[27] = &CSWSItemPropertyHandler::ApplyImprovedSavingThrow;
    m_pRemoveFunctions[27] = &CSWSItemPropertyHandler::RemoveImprovedSavingThrow;
    m_pApplyFunctions[29] = &CSWSItemPropertyHandler::ApplyProperty29;
    m_pRemoveFunctions[29] = &CSWSItemPropertyHandler::RemoveNothing;
    m_pApplyFunctions[33] = &CSWSItemPropertyHandler::ApplyReducedSavingThrows;
    m_pRemoveFunctions[33] = &CSWSItemPropertyHandler::RemoveReducedSavingThrow;
    m_pApplyFunctions[34] = &CSWSItemPropertyHandler::ApplyReducedSavingThrows;
    m_pRemoveFunctions[34] = &CSWSItemPropertyHandler::RemoveReducedSavingThrow;
    m_pApplyFunctions[35] = &CSWSItemPropertyHandler::ApplyRegeneration;
    m_pRemoveFunctions[35] = &CSWSItemPropertyHandler::RemoveRegeneration;
    m_pApplyFunctions[54] = &CSWSItemPropertyHandler::ApplyRegeneration;
    m_pRemoveFunctions[54] = &CSWSItemPropertyHandler::RemoveRegeneration;
    m_pApplyFunctions[36] = &CSWSItemPropertyHandler::ApplyProperty36;
    m_pRemoveFunctions[36] = &CSWSItemPropertyHandler::RemoveProperty36;
    m_pApplyFunctions[38] = &CSWSItemPropertyHandler::ApplyAttackBonus;
    m_pRemoveFunctions[38] = &CSWSItemPropertyHandler::RemoveAttackBonus;
    m_pApplyFunctions[39] = &CSWSItemPropertyHandler::ApplyAttackBonus;
    m_pRemoveFunctions[39] = &CSWSItemPropertyHandler::RemoveAttackBonus;
    m_pApplyFunctions[40] = &CSWSItemPropertyHandler::ApplyAttackBonus;
    m_pRemoveFunctions[40] = &CSWSItemPropertyHandler::RemoveAttackBonus;
    m_pApplyFunctions[8] = &CSWSItemPropertyHandler::ApplyAttackPenalty;
    m_pRemoveFunctions[8] = &CSWSItemPropertyHandler::RemoveAttackPenalty;
    m_pApplyFunctions[47] = &CSWSItemPropertyHandler::ApplyTrueSeeing;
    m_pRemoveFunctions[47] = &CSWSItemPropertyHandler::RemoveTrueSeeing;
    m_pApplyFunctions[50] = &CSWSItemPropertyHandler::ApplyFreedomOfMovement;
    m_pRemoveFunctions[50] = &CSWSItemPropertyHandler::RemoveFreedomOfMovement;
    m_pApplyFunctions[52] = &CSWSItemPropertyHandler::ApplySpecialWalk;
    m_pRemoveFunctions[52] = &CSWSItemPropertyHandler::RemoveSpecialWalk;
    m_pApplyFunctions[55] = &CSWSItemPropertyHandler::ApplyProperty55;
    m_pRemoveFunctions[55] = &CSWSItemPropertyHandler::RemoveProperty55;
    m_pApplyFunctions[56] = &CSWSItemPropertyHandler::ApplyProperty56;
    m_pRemoveFunctions[56] = &CSWSItemPropertyHandler::RemoveProperty56;
    m_pApplyFunctions[59] = &CSWSItemPropertyHandler::ApplyProperty59;
    m_pRemoveFunctions[59] = &CSWSItemPropertyHandler::RemoveProperty59;
}

// Handlers below have no functions.csv row yet (tool compares our length).

// FUNCTION: SWKOTOR 0x004E62D0
int CSWSItemPropertyHandler::ApplyProperty29(CSWSItem* pItem, CSWItemProperty* pProperty,
                                             CSWSCreature* pCreature, unsigned long nSlot,
                                             int bLoadingGame)
{
    if (pItem->GetPropertyByTypeExists(0x1d, 0)) {
        CGameEffect* pEffect = new CGameEffect(1);
        pEffect->SetEffectType(EFFECT_TYPE_PROPERTY29);
        pEffect->SetSubType(SUBTYPE_EQUIPPED);
        pEffect->SetCreator(pItem->m_idSelf);
        pEffect->SetInteger(0, 5000);
        pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E6D50
int CSWSItemPropertyHandler::ApplyBonusFeat(CSWSItem* pItem, CSWItemProperty* pProperty,
                                            CSWSCreature* pCreature, unsigned long nSlot,
                                            int bLoadingGame)
{
    if (pCreature->m_pStats == 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    if (pEffect == 0) {
        return 0;
    }
    pEffect->SetEffectType(EFFECT_TYPE_BONUS_FEAT);
    pEffect->SetSubType(3);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetCreator(pItem->m_idSelf);
    pCreature->ApplyEffect(pEffect, bLoadingGame, bLoadingGame);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E95A0
int CSWSItemPropertyHandler::RemoveTrueSeeing(CSWSItem* pItem, CSWItemProperty* pProperty,
                                              CSWSCreature* pCreature, unsigned long nSlot)
{
    for (int i = 0; i < pCreature->m_appliedEffects.num; i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_TRUESEEING &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E9620
int CSWSItemPropertyHandler::RemoveFreedomOfMovement(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                     CSWSCreature* pCreature, unsigned long nSlot)
{
    for (int i = pCreature->m_pStats->m_nImmunityEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_IMMUNITY;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_IMMUNITY &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            (pEffect->GetInteger(0) == IMMUNITY_TYPE_PARALYSIS ||
             pEffect->GetInteger(0) == IMMUNITY_TYPE_ENTANGLE ||
             pEffect->GetInteger(0) == IMMUNITY_TYPE_SLOW ||
             pEffect->GetInteger(0) == IMMUNITY_TYPE_MOVEMENT_SPEED_DECREASE)) {
            pCreature->RemoveEffectById(pEffect->m_nID);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7B10
int CSWSItemPropertyHandler::ApplyProperty36(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame)
{
    int nValue = pProperty->m_nCostTableValue;
    if (nValue <= 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_PROPERTY36);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pEffect->SetInteger(2, g_pRules->m_nDifficultyRacialType);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7CE0
int CSWSItemPropertyHandler::ApplyProperty55(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame)
{
    int nValue = pProperty->m_nCostTableValue;
    if (nValue <= 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_PROPERTY55);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7DB0
int CSWSItemPropertyHandler::ApplyProperty56(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame)
{
    int nValue = pProperty->m_nCostTableValue;
    if (nValue <= 0) {
        return 0;
    }
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_PROPERTY56);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, pProperty->m_nSubType);
    pEffect->SetInteger(1, nValue);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004E7E80
int CSWSItemPropertyHandler::ApplyProperty59(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame)
{
    int nValue = pProperty->m_nSubType;
    CGameEffect* pEffect = new CGameEffect(1);
    pEffect->SetEffectType(EFFECT_TYPE_PROPERTY59);
    pEffect->SetSubType(SUBTYPE_EQUIPPED);
    pEffect->SetCreator(pItem->m_idSelf);
    pEffect->SetInteger(0, nValue);
    pEffect->SetInteger(1, 0);
    pCreature->ApplyEffect(pEffect, bLoadingGame, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA580
int CSWSItemPropertyHandler::RemoveProperty36(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    for (int i = pCreature->m_pStats->m_nAttackEffectStart;
         i < pCreature->m_appliedEffects.num &&
         pCreature->m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_PROPERTY36;
         i++) {
        CGameEffect* pEffect = pCreature->m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_PROPERTY36 &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            unsigned short nSub = pProperty->m_nSubType;
            if (pEffect->GetInteger(0) == nSub) {
                unsigned short nValue = pProperty->m_nCostTableValue;
                if (pEffect->GetInteger(1) == nValue) {
                    pCreature->RemoveEffectById(pEffect->m_nID);
                    break;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA650
int CSWSItemPropertyHandler::RemoveRegeneration(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue = pProperty->m_nCostTableValue;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_REGENERATE &&
            pEffect->GetInteger(0) == nValue &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA6E0
int CSWSItemPropertyHandler::RemoveProperty55(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue = pProperty->m_nCostTableValue;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_PROPERTY55 &&
            pEffect->GetInteger(0) == nValue &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA770
int CSWSItemPropertyHandler::RemoveProperty56(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue = pProperty->m_nCostTableValue;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_PROPERTY56 &&
            pEffect->GetInteger(0) == nValue &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA800
int CSWSItemPropertyHandler::RemoveProperty59(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    int nValue = pProperty->m_nSubType;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_PROPERTY59 &&
            pEffect->GetInteger(0) == nValue &&
            pEffect->m_oidCreator == pItem->m_idSelf &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E97A0
int CSWSItemPropertyHandler::RemoveDamageImmunity(CSWSItem* pItem, CSWItemProperty* pProperty,
        CSWSCreature* pCreature, unsigned long nSlot)
{
    unsigned short nDamageType = pProperty->m_nSubType;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    unsigned char nTable = pProperty->m_nCostTable;
    C2DA* pTable = g_pRules->Get2DArrays()->GetIPRPCostTable(nTable);
    if (pTable == 0) {
        return 0;
    }
    int nValue;
    {
        CExoString sColumn("Value");
        unsigned short nRow = pProperty->m_nCostTableValue;
        pTable->GetINTEntry(nRow, sColumn, &nValue);
    }
    int nFlag;
    switch (nDamageType) {
    case 0: nFlag = DAMAGE_FLAG_00; break;
    case 1: nFlag = DAMAGE_FLAG_01; break;
    case 2: nFlag = DAMAGE_FLAG_02; break;
    case 3: nFlag = DAMAGE_FLAG_03; break;
    case 4: nFlag = DAMAGE_FLAG_04; break;
    case 5: nFlag = DAMAGE_FLAG_05; break;
    case 6: nFlag = DAMAGE_FLAG_06; break;
    case 7: nFlag = DAMAGE_FLAG_07; break;
    case 8: nFlag = DAMAGE_FLAG_08; break;
    case 9: nFlag = DAMAGE_FLAG_09; break;
    case 10: nFlag = DAMAGE_FLAG_10; break;
    case 11: nFlag = DAMAGE_FLAG_11; break;
    case 12: nFlag = DAMAGE_FLAG_12; break;
    case 13: nFlag = DAMAGE_FLAG_13; break;
    default: return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_DAMAGE_IMMUNITY &&
            pEffect->GetInteger(0) == nFlag &&
            pEffect->GetInteger(1) == nValue &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EA3F0
int CSWSItemPropertyHandler::RemoveImmunity(CSWSItem* pItem, CSWItemProperty* pProperty,
                                            CSWSCreature* pCreature, unsigned long nSlot)
{
    int nImmunity;
    int nSecondImmunity;
    int bSecond = 0;
    CExoArrayList<CGameEffect*>* pEffects = &pCreature->m_appliedEffects;
    if (pEffects == 0) {
        return 0;
    }
    switch (pProperty->m_nSubType) {
    case 0: nImmunity = IMMUNITY_TYPE_1E; break;
    case 1:
        nSecondImmunity = IMMUNITY_TYPE_13;
        nImmunity = IMMUNITY_TYPE_1D;
        bSecond = 1;
        break;
    case 2: nImmunity = IMMUNITY_TYPE_01; break;
    case 3: nImmunity = IMMUNITY_TYPE_02; break;
    case 4: nImmunity = IMMUNITY_TYPE_03; break;
    case 5: nImmunity = IMMUNITY_TYPE_04; break;
    case 6: nImmunity = IMMUNITY_TYPE_1C; break;
    case 7: nImmunity = IMMUNITY_TYPE_PARALYSIS; break;
    case 8: nImmunity = IMMUNITY_TYPE_1F; break;
    case 9: nImmunity = IMMUNITY_TYPE_20; break;
    default: return 0;
    }
    for (int i = 0; i < pEffects->num; i++) {
        CGameEffect* pEffect = pEffects->element[i];
        if (pEffect->m_nType == EFFECT_TYPE_IMMUNITY &&
            pEffect->GetInteger(0) == nImmunity &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
        if (bSecond &&
            pEffect->m_nType == EFFECT_TYPE_IMMUNITY &&
            pEffect->GetInteger(0) == nSecondImmunity &&
            pEffect->GetSubType() == SUBTYPE_EQUIPPED &&
            pEffect->m_oidCreator == pItem->m_idSelf) {
            pCreature->RemoveEffectById(pEffect->m_nID);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E53A0
CSWSItemPropertyHandler::~CSWSItemPropertyHandler()
{
    if (m_pApplyFunctions) {
        delete[] m_pApplyFunctions;
        m_pApplyFunctions = 0;
    }
    if (m_pRemoveFunctions) {
        delete[] m_pRemoveFunctions;
        m_pRemoveFunctions = 0;
    }
}
