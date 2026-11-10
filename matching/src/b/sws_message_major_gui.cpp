// CSWSMessage::WriteGameObjUpdate_MajorGUIPanels: player sheet, open
// container and equipped-slot updates for one player.
#define B_EXO_ARRAY_LIST_OPAQUE_ADD
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sws_item.h"
#include "b/server_info.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638

class CSWSPlayerLUOInventory;

class CSWGuiInGame {
public:
    int GetPartyAccessPanelUp(int bArg);                    // 0x0062B4D0
    int GetCurrentEquipScreenNPC();                         // 0x0062FEA0
};

class CSWPartyTable {
public:
    unsigned long GetNPCObject(int nNPC, int bCreate, int bRevive); // 0x00564700
};

// The player's store GUI holds the last-update inventory (CSWSPlayerStore
// here is the class the Mac map calls CSWSPlayerLUOInventory).
inline CSWSPlayerLUOInventory* GetLUOInventory(CSWSPlayer* pPlayer)
{
    return (CSWSPlayerLUOInventory*)pPlayer->m_pStoreGUI->m_pStore;
}

// NONMATCH (65): scratch registers from the open-container branch on
// (repository in edx, g_pAppManager in eax); the -1 flag store follows
// the dead AsSWSCreature call in the original.
// FUNCTION: SWKOTOR 0x00577670
void CSWSMessage::WriteGameObjUpdate_MajorGUIPanels(CSWSPlayer* pPlayer)
{
    CExoArrayList<unsigned long> aSlots;
    CGameObject* pObject = pPlayer->GetGameObject();
    if (pObject != NULL && pObject->AsSWSCreature() != NULL) {
        CSWSPlayerLastUpdateObject* pPlayerLast = pPlayer->m_pLastUpdatePlayer;
        CLastUpdateObject* pLast = pPlayer->GetLastUpdateObject(pObject->GetId());
        if (pLast != NULL) {
            unsigned long nFlags = 0;
            if (pPlayerLast == NULL) {
                pPlayerLast = pPlayer->CreateNewPlayerLastUpdateObject();
                ComputeLastUpdate_GuiKnownSpells(pObject->AsSWSCreature(), pPlayerLast);
                nFlags = 0xffffffff;
                pObject->AsSWSCreature();
            } else {
                TestPlayerUpdateDifferences(pPlayer, pPlayerLast, pLast, nFlags);
                if (nFlags == 0) goto done;
            }
            WriteGameObjUpdate_PlayerUpdate(pPlayer, pPlayerLast, pLast, nFlags);
            StoreValuesInLastPlayerUpdateObject(pPlayer, pPlayerLast, pLast, nFlags);
        }
    done:
        int bOpen = pPlayer->m_pStoreGUI->m_bOpen;
        if (bOpen == 1) {
            unsigned long oidContainer = pPlayer->m_pObjectRef->m_oid;
            if (oidContainer != OBJECT_INVALID_ID) {
                CGameObject* pContainer;
                CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
                if (pArray->GetGameObject(oidContainer, &pContainer) == GAME_OBJECT_VALID) {
                    CItemRepository* pRepository;
                    if (pContainer->m_nObjectType == OBJECT_TYPE_ITEM) {
                        pRepository = pContainer->AsSWSItem()->m_pRepository;
                    } else {
                        pRepository = (CItemRepository*)pContainer;
                    }
                    if (pContainer->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                        pRepository = pContainer->AsSWSPlaceable()->m_pItemRepository;
                    }
                    WriteRepositoryUpdate(pPlayer, (CSWSObject*)pObject, pRepository,
                                          GetLUOInventory(pPlayer), 0, 'C');
                }
            } else {
                int bSwapped = 0;
                unsigned long oidSaved = pPlayer->m_oidNWSObject;
                CGameObject* pSaved = pObject;
                if (g_pAppManager->GetServerExoApp()->GetServerInfo()->m_bSpecialModule &&
                    g_pAppManager->GetClientExoApp()->GetInGameGui()->GetPartyAccessPanelUp(1)) {
                    int nNPC = g_pAppManager->GetClientExoApp()->GetInGameGui()->GetCurrentEquipScreenNPC();
                    unsigned long oidNPC =
                        g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCObject(nNPC, 0, 1);
                    pObject = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidNPC);
                    if (pObject != NULL) {
                        bSwapped = 1;
                        pPlayer->m_oidNWSObject = oidNPC;
                    } else {
                        pObject = pSaved;
                    }
                }
                for (int nSlot = 0; nSlot < 18; nSlot++) {
                    if (ComputeInventoryUpdateRequired(pPlayer, nSlot) == 1) {
                        UpdateLastUpdateInventory(pPlayer, nSlot);
                        unsigned long nMask = 1 << nSlot;
                        if (pObject->AsSWSCreature()->GetInventory()->GetItemInSlot(nMask) == NULL) {
                            WriteBYTE('G', 8);
                            WriteBYTE('I', 8);
                            WriteBYTE('D', 8);
                            WriteDWORD(nMask, 0x20);
                        } else {
                            aSlots.Add(nMask);
                        }
                    }
                }
                for (int i = 0; i < aSlots.num; i++) {
                    WriteBYTE('G', 8);
                    WriteBYTE('I', 8);
                    WriteBYTE('A', 8);
                    WriteDWORD(aSlots[i], 0x20);
                    CSWSItem* pItem = pObject->AsSWSCreature()->GetInventory()->GetItemInSlot(aSlots[i]);
                    WriteOBJECTIDServer(pItem->GetId());
                    AddItemAppearanceToMessage(pItem);
                    AddActiveItemPropertiesToMessage(pItem, pObject->AsSWSCreature());
                }
                if (bSwapped) {
                    pObject = pSaved;
                    pPlayer->m_oidNWSObject = oidSaved;
                }
            }
            WriteRepositoryUpdate(pPlayer, (CSWSObject*)pObject,
                                  pObject->AsSWSCreature()->GetItemRepository(1),
                                  GetLUOInventory(pPlayer), 1, 'R');
        }
    }
}
