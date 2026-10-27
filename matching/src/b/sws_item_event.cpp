// CSWSItem::EventHandler (round s10). Not in functions.csv: the row for
// AddAttackModifierProperties (0x0055E930) spans it. Slot 0x78 of the
// CSWSItem vftable for CSWSObject (0x00748CC0), so `this` is the CSWSObject
// base at +0x10.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

class CSWSStore {
public:
    int RemoveItem(CSWSItem* pItem);                        // 0x005C6BA0
    int GetItemInInventory(unsigned long oidItem);          // 0x005C6BB0
};

extern const unsigned long EVENT_TIMED_EVENT;               // 0x007441B8
extern const unsigned long EVENT_0x7441F4;                  // 0x007441F4 (use one charge)
extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638
extern const unsigned char OBJECT_TYPE_STORE;               // 0x0074663D

// NONMATCH (14): only the possessor lookup differs; the original loads
// m_oidPossessor into eax before the app pointer into edx (direct
// m_pServerExoApp gives that order but swaps the registers).
// FUNCTION: SWKOTOR 0x0055EE10
void CSWSItem::EventHandler(unsigned long nEventId, unsigned long nCallerObjectId, void* pScript,
                            unsigned long nCalendarDay, unsigned long nTimeOfDay)
{
    if (nEventId == EVENT_TIMED_EVENT) {
        if (pScript) {
            g_pVirtualMachine->RunScriptSituation(pScript, m_idSelf, 1);
        }
        return;
    }
    if (nEventId == EVENT_0x7441F4) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidPossessor);
        if (m_nStackCount > 1) {
            m_nStackCount--;
        } else {
            g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                0, 0, m_idSelf, m_idSelf, EVENT_DESTROY_OBJECT, NULL);
        }
        if (pCreature && pCreature->m_pInventory->GetItemInInventory(this)) {
            pCreature->m_nTotalWeight -= GetBaseItemFromRules()->m_nWeight;
        }
        return;
    }
    if (nEventId == EVENT_DESTROY_OBJECT && m_bEC) {
        CloseContainerForAll();
        CGameObject* pPossessor = g_pAppManager->m_pServerExoApp->GetGameObject(m_oidPossessor);
        if (pPossessor) {
            if (pPossessor->m_nObjectType == OBJECT_TYPE_CREATURE) {
                CSWSCreature* pCreature = pPossessor->AsSWSCreature();
                pCreature->RemoveItem(this, 0, 1, 0);
                if (pCreature->m_bInParty && (int)pScript == 1) {
                    g_pAppManager->m_pClientExoApp->GetInGameGui()->UpdateStatus(8, 0);
                }
            } else if (pPossessor->m_nObjectType == OBJECT_TYPE_ITEM) {
                CSWSItem* pContainer = pPossessor->AsSWSItem();
                if (pContainer->m_pRepository->GetItemInRepository(this, 0) == 1) {
                    pContainer->m_pRepository->RemoveItem(this);
                }
                SetPossessor(INVALID_OBJECT_ID, 1, 1, INVALID_OBJECT_ID);
            } else if (pPossessor->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                CSWSPlaceable* pPlaceable = pPossessor->AsSWSPlaceable();
                if (pPlaceable->m_bHasInventory == 1 &&
                    pPlaceable->m_pItemRepository->GetItemInRepository(this, 0) == 1) {
                    pPlaceable->m_pItemRepository->RemoveItem(this);
                }
                SetPossessor(INVALID_OBJECT_ID, 1, 1, INVALID_OBJECT_ID);
            } else if (pPossessor->m_nObjectType == OBJECT_TYPE_STORE) {
                CSWSStore* pStore = pPossessor->AsSWSStore();
                if (pStore->GetItemInInventory(GetId()) == 1) {
                    pStore->RemoveItem(this);
                }
                SetPossessor(INVALID_OBJECT_ID, 1, 1, INVALID_OBJECT_ID);
            }
        }
        RemoveFromArea();
        delete this;
    }
}
