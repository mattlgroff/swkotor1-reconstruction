// CSWSCreature::RunEquip / RunUnequip (nwscreature.cpp).
#include "a/sws_creature.h"

// NONMATCH: 165 diff: pRepository kept in a register (original spills it with the party table) and player/message register swap.
// FUNCTION: SWKOTOR 0x00501DE0
int CSWSCreature::RunEquip(unsigned long oidItem, unsigned long nSlot, int nValue)
{
    int bUnequipped = 0;
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    CItemRepository* pRepository = 0;
    CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        if (pItem->m_oidPossessor == m_idSelf) {
            pRepository = GetItemRepository(1);
        } else {
            CSWSItem* pContainer = GetServerExoApp()->GetItemByGameObjectID(pItem->m_oidPossessor);
            if (pContainer && pContainer->m_oidPossessor == m_idSelf) {
                pRepository = pContainer->m_pRepository;
            } else {
                CClientExoApp* pClient = GetClientExoApp();
                CSWPartyTable* pPartyTable = GetServerExoApp()->GetPartyTable();
                CSWParty* pParty = pClient->GetSWParty();
                if (!pParty->GetIsCharacterInParty(pClient->ServerToClientObjectId(m_idSelf)) &&
                    !pPartyTable->GetIsNPCObject(m_idSelf)) {
                    if (pPlayer && pMessage)
                        pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
                    return 0;
                }
                if (pParty->GetIsCharacterInParty(pClient->ServerToClientObjectId(pItem->m_oidPossessor)) ||
                    pPartyTable->GetIsNPCObject(pItem->m_oidPossessor)) {
                    pRepository = GetItemRepository(1);
                } else {
                    CSWSItem* pOwner = GetServerExoApp()->GetItemByGameObjectID(pItem->m_oidPossessor);
                    if (!pOwner || !pPartyTable->GetIsNPCObject(pOwner->m_oidPossessor)) {
                        if (pPlayer && pMessage)
                            pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
                        return 0;
                    }
                    pRepository = pOwner->m_pRepository;
                }
            }
        }
        unsigned char nResult = CanEquipItem(pItem, &nSlot, 1, 0, 1);
        if (nResult == 1) {
            if (pItem->m_nStackSize > 1) pItem = pItem->SplitItem(1);
            if (pRepository->GetItemInRepository(pItem, 0) == 1) pRepository->RemoveItem(pItem);
            if (pItem->m_oidPossessor != m_idSelf) pItem->SetPossessor(m_idSelf, 0, 0, 0x7f000000);
            EquipItem(nSlot, pItem, 1, 0, nValue);
            unsigned long nEquipped = m_pInventory->GetSlotFromItem(pItem);
            if (nEquipped == 0x10) m_nMainHandValue = nValue;
            else if (nEquipped == 0x20) m_nOffHandValue = nValue;
        } else {
            CSWSItem* pHeld;
            if (nResult == 2) {
                pHeld = m_pInventory->GetItemInSlot(nSlot);
                if (!pHeld) goto Cancel;
                if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nStackSize > 1) {
                    if (pHeld->CompareItem(pItem)) goto Cancel;
                    UnequipItem(pHeld);
                    if (!GetItemRepository(1)->AddItem(&pHeld, 1, 0, 0)) {
                        RemoveItem(pHeld, 0, 1, 0);
                        pHeld->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                    }
                } else {
                    UnequipItem(pHeld);
                    if (!GetItemRepository(1)->AddItem(&pHeld, 1, 0, 0)) {
                        RemoveItem(pHeld, 0, 1, 0);
                        pHeld->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                    }
                }
            } else if (nResult == 3) {
                pHeld = m_pInventory->GetItemInSlot(0x10);
                if (pHeld) {
                    UnequipItem(pHeld);
                    if (!GetItemRepository(1)->AddItem(&pHeld, 1, 0, 0)) {
                        RemoveItem(pHeld, 0, 1, 0);
                        pHeld->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                    }
                    bUnequipped = 1;
                }
                pHeld = m_pInventory->GetItemInSlot(0x20);
                if (pHeld) {
                    UnequipItem(pHeld);
                    if (!GetItemRepository(1)->AddItem(&pHeld, 1, 0, 0)) {
                        RemoveItem(pHeld, 0, 1, 0);
                        pHeld->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                    }
                } else if (bUnequipped != 1) {
                    goto Cancel;
                }
            } else {
                goto Cancel;
            }
            if (pItem->m_nStackSize > 1) pItem = pItem->SplitItem(1);
            if (pRepository->GetItemInRepository(pItem, 0) == 1) pRepository->RemoveItem(pItem);
            if (pItem->m_oidPossessor != m_idSelf) pItem->SetPossessor(m_idSelf, 0, 0, 0x7f000000);
            EquipItem(nSlot, pItem, 1, 0, 0);
            SetAAxValue(m_pInventory->GetSlotFromItem(pItem), nValue);
        }
        CSWSItem* pRight = m_pInventory->GetItemInSlot(0x10);
        CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
        unsigned char nMode = m_nCombatMode;
        if (nMode == 5) {
            if (pRight || pLeft) SetCombatMode(0, 1);
        } else if (nMode == 6) {
            if (!pRight || g_pRules->GetBaseItem(pRight->m_nBaseItem)->m_n1A <= 0) SetCombatMode(0, 1);
        } else if (nMode == 2 || nMode == 3 || nMode == 1) {
            if (pRight && g_pRules->GetBaseItem(pRight->m_nBaseItem)->m_n1A > 0) SetCombatMode(0, 1);
        }
        return 1;
    }
Cancel:
    if (pMessage && pPlayer)
        pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
    return 0;
}

// NONMATCH: 67 diff: player is not spilled and the message/repository-id registers rotate (edi/ebp).
// FUNCTION: SWKOTOR 0x005023A0
int CSWSCreature::RunUnequip(unsigned long oidItem, unsigned long oidRepository, int bCombine, CSWSItem** ppItem,
                             int nValue)
{
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    if (ppItem) *ppItem = 0;
    CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem && pItem->m_oidPossessor == m_idSelf && m_pInventory->Fn5A4950(pItem) == 1) {
        CItemRepository* pRepository;
        if (oidRepository == 0x7f000000) {
            pRepository = GetItemRepository(1);
        } else {
            CSWSItem* pContainer = GetServerExoApp()->GetItemByGameObjectID(oidRepository);
            if (pContainer && pContainer->m_oidPossessor == m_idSelf) pRepository = pContainer->m_pRepository;
            else pRepository = 0;
        }
        if (!pRepository) {
            if (pPlayer && pMessage)
                pMessage->SendServerToPlayerInventory_UnequipCancel(pPlayer->m_nPlayerID, oidItem);
            return 0;
        }
        {
            unsigned char nResult = CanUnEquipWeapon(pItem);
            if (nResult == 1) {
                SetAAxValue(m_pInventory->GetSlotFromItem(pItem), nValue);
                UnequipItem(pItem);
                if (!pRepository->AddItem(&pItem, bCombine, 0, 0))
                    pItem->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                if (ppItem) *ppItem = pItem;
                pItem->SetPossessor(oidRepository == 0x7f000000 ? m_idSelf : oidRepository, 0, 0, 0x7f000000);
                if (pPlayer && pMessage) {
                    pMessage->SendServerToPlayerInventory_Unequip(pPlayer->m_nPlayerID, oidItem, nValue);
                    return 1;
                }
            } else if (nResult == 2) {
                SetAAxValue(m_pInventory->GetSlotFromItem(pItem), nValue);
                UnequipItem(pItem);
                if (!pRepository->AddItem(&pItem, bCombine, 0, 0))
                    pItem->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z + 0.2f, 1);
                CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
                if (pLeft) {
                    UnequipItem(pLeft);
                    EquipItem(0x10, pLeft, 1, 0, 0);
                }
                if (ppItem) *ppItem = pItem;
                pItem->SetPossessor(oidRepository == 0x7f000000 ? m_idSelf : oidRepository, 0, 0, 0x7f000000);
                if (pPlayer && pMessage) {
                    pMessage->SendServerToPlayerInventory_Unequip(pPlayer->m_nPlayerID, pLeft->m_n14, nValue);
                    pMessage->SendServerToPlayerInventory_Unequip(pPlayer->m_nPlayerID, pItem->m_n14, nValue);
                    return 1;
                }
            }
        }
    }
    if (pPlayer && pMessage) pMessage->SendServerToPlayerInventory_UnequipCancel(pPlayer->m_nPlayerID, oidItem);
    return 0;
}
