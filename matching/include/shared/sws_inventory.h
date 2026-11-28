// sws_inventory.h -- CSWSInventory
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/a/sws_creature.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_INVENTORY_H
#define SHARED_SWS_INVENTORY_H


class CSWSItem;

class CSWSInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);   // 0x005A4C20
    unsigned long GetSlotFromItem(CSWSItem* pItem); // 0x005A4980
    int RemoveItem(CSWSItem* pItem);                // 0x005A4BA0
    int Fn5A4950(CSWSItem* pItem);                  // 0x005A4950 (placeholder name)
    int GetItemInInventory(CSWSItem* pItem);        // 0x005A4950
    void PutItemInSlot(unsigned long nSlot, CSWSItem* pItem); // 0x005A4BE0
};

#endif
