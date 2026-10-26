// Equipment eligibility helpers adjacent to creature AI actions. Names without
// a defined-symbol record are descriptive names from the playable references.
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x00513E30
unsigned char CSWSCreature::CanUnEquipWeapon(CSWSItem* pItem)
{
    CSWSItem* pRight = m_pInventory->GetItemInSlot(0x10);
    CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
    if (pRight && pRight->m_n14 == pItem->m_n14 && pLeft) {
        int nBaseItem = pLeft->m_nBaseItem;
        if (g_pRules->GetBaseItem(nBaseItem)->m_nWeaponWield) {
            CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(m_idSelf);
            if (pPlayer) {
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[4] = 0x7f000000;
                pPlayer->m_pStoreGUI->m_pStore->m_aSlotItems[5] = 0x7f000000;
            }
            return 2;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00510E30
int CSWSCreature::CheckProficiencies(CSWSItem* pItem, unsigned long)
{
    int nBaseItem = pItem->m_nBaseItem;
    if (!g_pRules->GetBaseItem(nBaseItem)->m_nEquipableSlots) return 0;
    if (m_pStats->HasFeat(0x5d)) return 1;
    for (int i = 0; i < g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nRequiredFeatCount; i++) {
        if (!m_pStats->HasFeat(g_pRules->GetBaseItem(pItem->m_nBaseItem)->GetRequiredFeat(i)))
            return 0;
    }
    return 1;
}

// NONMATCH: 23 diff: activation-byte AL/CL and mask/loop scheduling tie.
// FUNCTION: SWKOTOR 0x005145A0
int CSWSCreature::CheckItemAlignmentRestrictions(CSWSItem* pItem)
{
    if (!pItem->GetPropertyByTypeExists(0x2b, 0)) return 1;
    if (m_pStats->HasFeat(0x5d)) return 1;
    short nAlignment = m_pStats->GetSimpleAlignmentGoodEvil();
    for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
        unsigned char nActivation = pItem->GetPassiveProperty(i)->m_nActivation;
        if (nActivation != 0xff) {
            unsigned long nMask = 1 << nActivation;
            if (!(nMask & pItem->m_nPropertyActivationMask)) continue;
        }
        {
            if (pItem->GetPassiveProperty(i)->m_nPropertyName == 0x2b &&
                pItem->GetPassiveProperty(i)->m_nSubType != nAlignment)
                return 0;
        }
    }
    return 1;
}

// NONMATCH: 7 diff: activation-byte AL/CL and mask scheduling tie.
// FUNCTION: SWKOTOR 0x00514650
int CSWSCreature::CheckItemFeatRestrictions(CSWSItem* pItem)
{
    if (!pItem->GetPropertyByTypeExists(0x39, 0)) return 1;
    CSWSCreatureStats* pStats = m_pStats;
    if (pStats->HasFeat(0x5d)) return 1;
    for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
        unsigned char nActivation = pItem->GetPassiveProperty(i)->m_nActivation;
        if (nActivation != 0xff) {
            unsigned long nMask = 1 << nActivation;
            if (!(nMask & pItem->m_nPropertyActivationMask)) continue;
        }
        {
            if (pItem->GetPassiveProperty(i)->m_nPropertyName == 0x39 &&
                m_pStats->GetFeatRemainingUses(pItem->GetPassiveProperty(i)->m_nSubType) <= 0)
                return 0;
        }
    }
    return 1;
}

// NONMATCH: 17 diff: activation-byte and nested class-loop register tie.
// FUNCTION: SWKOTOR 0x00518210
int CSWSCreature::CheckItemClassRestrictions(CSWSItem* pItem)
{
    if (!pItem->GetPropertyByTypeExists(0x2c, 0)) return 1;
    CSWSCreatureStats* pStats = m_pStats;
    if (pStats->HasFeat(0x5d)) return 1;
    for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
        unsigned char nActivation = pItem->GetPassiveProperty(i)->m_nActivation;
        if (nActivation != 0xff) {
            unsigned long nMask = 1 << nActivation;
            if (!(nMask & pItem->m_nPropertyActivationMask)) continue;
        }
        {
            if (pItem->GetPassiveProperty(i)->m_nPropertyName == 0x2c) {
                unsigned short nClass = pItem->GetPassiveProperty(i)->m_nSubType;
                for (int j = 0; j < m_pStats->m_nNumClasses; j++) {
                    if (nClass == (unsigned short)m_pStats->GetClass(j)) return 1;
                }
            }
        }
    }
    return 0;
}
