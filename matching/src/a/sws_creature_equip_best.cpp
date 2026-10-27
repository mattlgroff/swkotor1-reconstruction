// CSWSCreature best-equipment selection (nwscreature.cpp).
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x00501250
int CSWSCreature::EquipMostDamagingRangedWeapon(unsigned long oidVersus)
{
    CSWSCreature* pVersus = 0;
    CSWSItem* pCurrent = 0;
    int nBestModifier = -30;
    CSWSItem* pBest = 0;
    unsigned long nSlot;
    if (!GetItemRepository(1)) return 0;
    CGameObject* pTarget = GetServerExoApp()->GetGameObject(oidVersus);
    if (pTarget && pTarget->AsSWSObject()) {
        CGameObject* pObject = reinterpret_cast<CGameObject*>(pTarget->AsSWSObject());
        if (pObject->AsSWSCreature()) pVersus = pObject->AsSWSCreature();
    }
    pCurrent = m_pInventory->GetItemInSlot(0x10);
    if (pCurrent) {
        CSWBaseItem* pBase = g_pRules->GetBaseItem(pCurrent->m_nBaseItem);
        if (pBase && pBase->m_n1A > 0) {
            pBest = pCurrent;
            nBestModifier = m_pStats->GetAttackModifierVersus(pVersus);
        }
        if (pCurrent) {
            UnequipItem(pCurrent);
            GetItemRepository(1)->AddItem(&pCurrent, 0, 0, 1);
            CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
            if (pLeft) {
                UnequipItem(pLeft);
                GetItemRepository(1)->AddItem(&pLeft, 0, 0, 1);
            }
        }
    }
    for (unsigned long i = 0; i < GetItemRepository(1)->m_nItems; i++) {
        CSWSItem* pItem = GetItemRepository(1)->ItemListGetItem(i);
        if (!pItem) continue;
        CSWBaseItem* pBase = g_pRules->GetBaseItem(pItem->m_nBaseItem);
        if (!pBase || !pBase->m_nWeaponWield) continue;
        nSlot = 0x10;
        if (!CanEquipItem(pItem, &nSlot, 0, 0, 1) || pBase->m_n1A <= 0) continue;
        EquipItem(nSlot, pItem, 1, 0, 0);
        int nModifier = m_pStats->GetAttackModifierVersus(pVersus);
        if (nModifier > nBestModifier) {
            nBestModifier = nModifier;
            pBest = pItem;
        }
        UnequipItem(pItem);
    }
    if (pCurrent) {
        GetItemRepository(1)->RemoveItem(pCurrent);
        EquipItem(0x10, pCurrent, 1, 0, 0);
    }
    if (!pBest || pCurrent == pBest) return 0;
    AddEquipItemActions(pBest, 0x10, 0, 0, 0, 0, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x00500E50
int CSWSCreature::EquipMostDamagingMeleeWeapon(unsigned long oidVersus, int bOffHand, int bRecursed)
{
    CGameObject* pVersusObject = 0;
    CSWSCreature* pVersus = 0;
    CSWSItem* pCurrent;
    unsigned long nSlot;
    unsigned long nEquipSlot;
    if (!GetItemRepository(1)) return 0;
    CGameObject* pTarget = GetServerExoApp()->GetGameObject(oidVersus);
    if (pTarget && pTarget->AsSWSObject()) {
        pVersusObject = reinterpret_cast<CGameObject*>(pTarget->AsSWSObject());
        if (pVersusObject->AsSWSCreature()) pVersus = pVersusObject->AsSWSCreature();
    }
    if (!bOffHand) {
        int nType = m_pCombatRound->GetWeaponAttackType();
        if (nType == 3) nSlot = 0x4000;
        else if (nType == 4) nSlot = 0x8000;
        else nSlot = nType == 5 ? 0x10000 : 0x10;
    } else {
        nSlot = 0x20;
    }
    pCurrent = m_pInventory->GetItemInSlot(nSlot);
    if (pCurrent && g_pRules->GetBaseItem(pCurrent->m_nBaseItem)->m_n1A > 0) {
        UnequipItem(pCurrent);
        GetItemRepository(1)->AddItem(&pCurrent, 0, 0, 1);
        pCurrent = 0;
        if (nSlot == 0x10) {
            CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
            if (pLeft) {
                UnequipItem(pLeft);
                GetItemRepository(1)->AddItem(&pLeft, 0, 0, 1);
            }
        }
    }
    if (bOffHand == 1 && !pCurrent) {
        CSWSItem* pMain = m_pInventory->GetItemInSlot(0x10);
        if (pMain && g_pRules->GetBaseItem(pMain->m_nBaseItem)->m_nWeaponType == 3) {
            pCurrent = pMain;
            nSlot = 0x10;
        }
    }
    if (pCurrent) {
        UnequipItem(pCurrent);
        GetItemRepository(1)->AddItem(&pCurrent, 0, 0, 1);
    }
    CSWSItem* pBest = 0;
    m_pStats->GetSaveVsDamage(reinterpret_cast<CSWSCreature*>(pVersusObject), 0, 0, 0, 1, 1);
    GetWeaponDamageBonus(pVersus, 0);
    int nBestDamage = 0;
    for (unsigned long i = 0; i < GetItemRepository(1)->m_nItems; i++) {
        CSWSItem* pItem = GetItemRepository(1)->ItemListGetItem(i);
        if (!pItem) continue;
        CSWBaseItem* pBase = g_pRules->GetBaseItem(pItem->m_nBaseItem);
        if (!pBase || !pBase->m_nWeaponWield) continue;
        nEquipSlot = nSlot;
        int nCreatureSize = m_n4F8;
        int nDiff = g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_n1B - nCreatureSize;
        int nSize = (nDiff < -2 || nDiff > 1) ? -10 : nDiff;
        if (!(!bOffHand && CanEquipItem(pItem, &nEquipSlot, 0, 0, 1) ||
              bOffHand && CanEquipItem(pItem, &nEquipSlot, 0, 0, 1) == 1)) continue;
        if (pBase->m_n1A > 0 || !pBase->m_nWeaponWield) continue;
        if (bOffHand && (nSize >= 1 || nSize == -10)) continue;
        EquipItem(nEquipSlot, pItem, 1, 0, 0);
        int nDamage = GetStats()->GetSaveVsDamage(reinterpret_cast<CSWSCreature*>(pVersusObject), bOffHand, 0, 0, 1, 1);
        nDamage += GetWeaponDamageBonus(pVersus, bOffHand);
        if (nDamage >= nBestDamage) {
            nBestDamage = nDamage;
            pBest = pItem;
        }
        UnequipItem(pItem);
    }
    if (pCurrent) {
        GetItemRepository(1)->RemoveItem(pCurrent);
        EquipItem(nSlot, pCurrent, 1, 0, 0);
    }
    if (!pBest) {
        if (pCurrent && oidVersus != 0x7f000000 && !bRecursed) {
            EquipMostDamagingMeleeWeapon(0x7f000000, bOffHand, 1);
            return 1;
        }
        return 1;
    }
    if (pCurrent && pCurrent->m_n14 == pBest->m_n14) return 1;
    AddEquipItemActions(pBest, nSlot, 0, 0, 0, 0, 0);
    return 1;
}
