// CSWSMessage::WriteRepositoryUpdate: compares an item repository against
// the player's last-update inventory list and writes add/delete/update
// records ('G', type, 'A'/'D'/'U').
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sws_item.h"

extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635

struct CExoLinkedListNode;

struct CSWSPlayerLUOItem {
    unsigned long m_oidItem;                                // 0x0
    unsigned long m_nStackSize;                             // 0x4
    unsigned long m_nCost;                                  // 0x8
};

class CSWSPlayerLUOInventory {
public:
    CSWSPlayerLUOItem* ItemListGetUpdateItem(CExoLinkedListNode* pos,
                                             unsigned char nList); // 0x005CA4D0
    void ItemListGetTail(CExoLinkedListNode** pPos, unsigned char nList); // 0x005CA520
    void ItemListGetPrev(CExoLinkedListNode*& pos, unsigned char nList); // 0x005CA570
    void ItemListRemove(CExoLinkedListNode* pos, unsigned char nList); // 0x005CA5C0
    void ItemListAddHead(unsigned long oidItem, unsigned char nList,
                         unsigned long nStackSize, unsigned long nCost); // 0x005CA630
};

// FUNCTION: SWKOTOR 0x0056E360
void CSWSMessage::WriteRepositoryUpdate(CSWSPlayer* pPlayer, CSWSObject* pObject,
                                        CItemRepository* pRepository,
                                        CSWSPlayerLUOInventory* pInventory,
                                        unsigned char nList, char cType)
{
    CExoLinkedListNode* pos = NULL;
    CSWSPlayerLUOItem* pUpdate = NULL;
    int i = pRepository->m_aItems.num - 1;
    unsigned long oidItem;
    pInventory->ItemListGetTail(&pos, nList);
    while (i >= 0 || pos != NULL) {
        CSWSItem* pItem = NULL;
        if (i >= 0) {
            oidItem = pRepository->GetItemId(i);
            pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
        }
        if (pos != NULL) {
            pUpdate = pInventory->ItemListGetUpdateItem(pos, nList);
        }
        if (i >= 0) {
            if (pos != NULL) {
                if (oidItem != pUpdate->m_oidItem) {
                    WriteBYTE('G', 8);
                    WriteBYTE(cType, 8);
                    WriteBYTE('D', 8);
                    WriteOBJECTIDServer(pUpdate->m_oidItem);
                    CExoLinkedListNode* posRemove = pos;
                    pInventory->ItemListGetPrev(pos, nList);
                    pInventory->ItemListRemove(posRemove, nList);
                } else if (pItem->m_nStackCount == pUpdate->m_nStackSize &&
                           pItem->GetCost() == pUpdate->m_nCost) {
                    i--;
                    pInventory->ItemListGetPrev(pos, nList);
                } else {
                    WriteBYTE('G', 8);
                    WriteBYTE(cType, 8);
                    WriteBYTE('U', 8);
                    WriteOBJECTIDServer(pItem->GetId());
                    WriteDWORD(pItem->m_nStackCount, 0x20);
                    WriteDWORD(pItem->GetCost(), 0x20);
                    pUpdate->m_nStackSize = pItem->m_nStackCount;
                    pUpdate->m_nCost = pItem->GetCost();
                    i--;
                    pInventory->ItemListGetPrev(pos, nList);
                }
            } else {
                WriteBYTE('G', 8);
                WriteBYTE(cType, 8);
                WriteBYTE('A', 8);
                WriteOBJECTIDServer(oidItem);
                if (pItem->m_nObjectType == OBJECT_TYPE_ITEM) {
                    AddItemAppearanceToMessage(pItem);
                    AddActiveItemPropertiesToMessage(pItem, pObject->AsSWSCreature());
                }
                int nStack = pItem->m_nStackCount;
                unsigned long nCost = pItem->GetCost();
                pInventory->ItemListAddHead(oidItem, nList, nStack, nCost);
                i--;
            }
        } else {
            WriteBYTE('G', 8);
            WriteBYTE(cType, 8);
            WriteBYTE('D', 8);
            WriteOBJECTIDServer(pUpdate->m_oidItem);
            CExoLinkedListNode* posRemove = pos;
            pInventory->ItemListGetPrev(pos, nList);
            pInventory->ItemListRemove(posRemove, nList);
        }
    }
}
