// Worker A round s7: CSWSCreature::EvaluateLock (nwscreature.cpp).
#include "a/sws_creature.h"

extern const unsigned long EVENT_0x7441F4;      // 0x007441F4 (placeholder name)

// FUNCTION: SWKOTOR 0x004F0B20
int CSWSCreature::EvaluateLock(CGameObject* pObject, int bLock)
{
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    if (pObject->AsSWSDoor()) {
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        if (pDoor->m_bLocked == bLock) {
            return 1;
        }
        if (pDoor->m_sKeyName.IsEmpty()) {
            return 0;
        }
        unsigned long oidKey = GetItemRepository(1)->FindItemWithTag(pDoor->m_sKeyName);
        CSWSItem* pKey = GetServerExoApp()->GetItemByGameObjectID(oidKey);
        if (pKey) {
            if (pDoor->m_bAutoRemoveKey) {
                pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pKey->m_n14, EVENT_0x7441F4, NULL);
            }
            pDoor->m_bLocked = bLock;
        }
        else {
            int bFound = 0;
            for (int i = 0; i < 18; i++) {
                CSWSItem* pItem = GetInventory()->GetItemInSlot(1 << i);
                if (pItem && pItem->m_sTag == pDoor->m_sKeyName) {
                    if (pDoor->m_bAutoRemoveKey) {
                        pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pItem->m_n14, EVENT_0x7441F4, NULL);
                    }
                    pDoor->m_bLocked = bLock;
                    bFound = 1;
                    break;
                }
            }
            if (!bFound) {
                return 0;
            }
        }
    }
    else if (pObject->AsSWSPlaceable()) {
        CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
        if (pPlaceable->m_bLocked == bLock) {
            return 1;
        }
        if (pPlaceable->m_sKeyName.IsEmpty()) {
            return 0;
        }
        unsigned long oidKey = GetItemRepository(1)->FindItemWithTag(pPlaceable->m_sKeyName);
        CSWSItem* pKey = GetServerExoApp()->GetItemByGameObjectID(oidKey);
        if (pKey) {
            if (pPlaceable->m_bAutoRemoveKey) {
                pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pKey->m_n14, EVENT_0x7441F4, NULL);
            }
            pPlaceable->m_bLocked = bLock;
        }
        else {
            int bFound = 0;
            for (int i = 0; i < 18; i++) {
                CSWSItem* pItem = GetInventory()->GetItemInSlot(1 << i);
                if (pItem && pItem->m_sTag == pPlaceable->m_sKeyName) {
                    if (pPlaceable->m_bAutoRemoveKey) {
                        pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pItem->m_n14, EVENT_0x7441F4, NULL);
                    }
                    pPlaceable->m_bLocked = bLock;
                    bFound = 1;
                    break;
                }
            }
            if (!bFound) {
                return 0;
            }
        }
    }
    else {
        return 0;
    }
    SendFeedbackMessage(0x10, NULL);
    return 1;
}
