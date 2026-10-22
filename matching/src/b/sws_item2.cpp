// CSWSItem / CItemRepository part B (0x00556000-0x00560A00).
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/sw_rules.h"
#include "b/sws_player.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x0055CFA0
int CSWSItem::SaveContainerItems(CResGFF* pGFF, CResStruct* pStruct)
{
    CResList cList;
    CResStruct cStruct;
    pGFF->AddList(&cList, pStruct, "ItemList");
    for (unsigned long i = 0; i < (unsigned long)m_pRepository->m_aItems.num; i++) {
        pGFF->AddListElement(&cStruct, &cList, i);
        m_pRepository->GetItemAt(i)->SaveItem(pGFF, &cStruct);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0055D2C0
void CItemRepository::AddItemAt(CSWSItem* pItem, int nPos)
{
    if (m_bReadOnly) {
        return;
    }
    if (nPos >= 0 && nPos < m_aItems.num) {
        m_aItems.InsertAt(pItem->m_idSelf, nPos);
        return;
    }
    m_aItems.Add(pItem->m_idSelf);
}

// FUNCTION: SWKOTOR 0x0055DC10
void CSWSItem::CloseContainerForAll()
{
    int nBaseItem = m_nBaseItem;
    CSWBaseItem* pBase = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem);
    if (pBase && pBase->m_bContainer) {
        CExoLinkedList<CSWSClient>* pList = g_pAppManager->GetServerExoApp()->GetPlayerList();
        void* pPos = pList->GetHeadPos();
        while (pPos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pList->GetAtPos(pPos);
            CGameObject* pObject;
            if (pPlayer && (pObject = pPlayer->GetGameObject()) != NULL &&
                pPlayer->m_pObjectRef->m_oid == m_idSelf) {
                CloseInventory(pObject->m_idSelf, 1);
            }
            pList->GetNext(pPos);
        }
    }
}

// FUNCTION: SWKOTOR 0x005561A0
// NONMATCH: m_bContainer test in ecx (eax), and the event call loads the
// app pointer before its arguments.
void CSWSItem::OpenInventory(unsigned long nOpener)
{
    if (!m_bInventoryOpen) {
        int nBaseItem = m_nBaseItem;
        if (g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem)->m_bContainer != 0) {
            CGameObject* pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(nOpener);
            if (pPlayer) {
                unsigned long nCount = m_pRepository->m_aItems.num;
                unsigned long oid = m_idSelf;
                int bHasItems = nCount > 0;
                g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerOpenInventory(
                    pPlayer, oid, bHasItems);
            }
            CScriptEvent* pEvent = new CScriptEvent();
            pEvent->m_nType = 0x16;
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, nOpener, m_idSelf, EVENT_DELAY_INVENTORY, pEvent);
            m_bInventoryOpen = 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x0055D040
// NONMATCH: loop matches; the 8 use bytes live in eax/edx (ecx/eax here)
// and bytes 6-7 are read as one word in the original.
void CSWSItem::UpdateUsedActiveProperties()
{
    unsigned char aUses[8];
    unsigned char nUseable = 0;
    memset(aUses, 0, sizeof(aUses));
    for (int i = 0; i < m_nActiveProperties; i++) {
        CSWItemProperty* pProperty = GetActiveProperty(i);
        if (pProperty->m_nPropertyName == 10) {
            switch (pProperty->m_nCostValue) {
            case 1:
                if (pProperty->m_bUseable) {
                    if (i < 8) {
                        aUses[i] = (unsigned char)GetUsedActivePropertyUsesLeft((unsigned char)i);
                    }
                    nUseable |= 1 << i;
                }
                break;
            case 2: case 3: case 4: case 5: case 6:
                if (m_nCharges > 0) {
                    if (i < 8) {
                        aUses[i] = (unsigned char)GetUsedActivePropertyUsesLeft((unsigned char)i);
                    }
                    nUseable |= 1 << i;
                }
                break;
            case 8: case 9: case 10: case 11: case 12:
                if (pProperty->m_nUsesLeft > 0) {
                    if (i < 8) {
                        aUses[i] = (unsigned char)GetUsedActivePropertyUsesLeft((unsigned char)i);
                    }
                    nUseable |= 1 << i;
                }
                break;
            case 7: case 13: case 14: case 15: case 16: case 17: case 18:
                nUseable |= 1 << i;
                break;
            }
        }
    }
    unsigned char nChanged = 0;
    if (aUses[0] != m_aUsesLeft[0]) nChanged |= 1;
    if (aUses[1] != m_aUsesLeft[1]) nChanged |= 2;
    if (aUses[2] != m_aUsesLeft[2]) nChanged |= 4;
    if (aUses[3] != m_aUsesLeft[3]) nChanged |= 8;
    if (aUses[4] != m_aUsesLeft[4]) nChanged |= 0x10;
    if (aUses[5] != m_aUsesLeft[5]) nChanged |= 0x20;
    if (aUses[6] != m_aUsesLeft[6]) nChanged |= 0x40;
    if (aUses[7] != m_aUsesLeft[7]) nChanged |= 0x80;
    if (nUseable != m_nUseableProperties || nChanged) {
        m_nUseableProperties = nUseable;
        m_aUsesLeft[0] = aUses[0];
        m_aUsesLeft[1] = aUses[1];
        m_aUsesLeft[2] = aUses[2];
        m_aUsesLeft[3] = aUses[3];
        m_aUsesLeft[4] = aUses[4];
        m_aUsesLeft[5] = aUses[5];
        m_aUsesLeft[6] = aUses[6];
        m_aUsesLeft[7] = aUses[7];
        CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
        CGameObject* pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(m_oidPossessor);
        if (pPlayer) {
            pMessage->SendServerToPlayerItemUses(pPlayer, m_idSelf, m_nUseableProperties,
                                                 nChanged, m_aUsesLeft);
        }
    }
}

// FUNCTION: SWKOTOR 0x0055DCA0
int CSWSItem::AcquireItem(CSWSItem** ppItem, unsigned long oidFrom, int nArg)
{
    g_pAppManager->GetServerExoApp()->GetObjectArray();
    CSWSItem* pContainer = NULL;
    if ((*ppItem)->m_oidPossessor != oidFrom) {
        pContainer = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID((*ppItem)->m_oidPossessor);
        if (!pContainer) {
            return 0;
        }
        if (pContainer->m_oidPossessor != oidFrom) {
            return 0;
        }
        oidFrom = (*ppItem)->m_oidPossessor;
    }
    CItemRepository* pRepository = m_pRepository;
    CGameObject* pPossessor = g_pAppManager->GetServerExoApp()->GetGameObject(oidFrom);
    if (pPossessor) {
        if (pPossessor->m_nObjectType == 5) {
            pPossessor->AsSWSCreature()->RemoveItem(*ppItem, 1, nArg, 0);
        }
        else if (pPossessor->m_nObjectType == 9) {
            pPossessor->AsSWSPlaceable()->RemoveItem(*ppItem);
        }
        else if (pPossessor->m_nObjectType == 6 && pPossessor->AsSWSItem()->m_pRepository) {
            CSWSItem* pOther = pPossessor->AsSWSItem();
            pOther->m_pRepository->RemoveItem(*ppItem);
        }
    }
    else if (pContainer && (*ppItem)->GetBaseItemFromRules()->m_bContainer &&
             pContainer->GetBaseItemFromRules2()->m_bContainer) {
        return 0;
    }
    int nResult = pRepository->AddItem(ppItem, 1, 0, 1);
    if (nResult == 1) {
        if (!pPossessor && *ppItem) {
            (*ppItem)->RemoveFromArea();
        }
        if (*ppItem) {
            (*ppItem)->SetPossessor(m_idSelf, 1, nArg, 0x7f000000);
        }
    }
    return nResult;
}
