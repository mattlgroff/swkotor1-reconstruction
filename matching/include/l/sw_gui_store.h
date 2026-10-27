// Worker L matching headers: the in-game store screen CSWGuiStore (layout
// "store", vtable 0x00756E38, 0x2280 bytes, 0x006C0610-0x006C2270) and its
// item lines CSWGuiStoreItemEntry (sw_gui_inventory_equip.h).
#ifndef L_SW_GUI_STORE_H
#define L_SW_GUI_STORE_H

#include "l/sw_gui_upgrade.h"

class CSWSObject;

// Sort key read from the base item (+0x30) when the store lists group items.
class CSWBaseItem {
public:
    char pad_0x0[0x30];
    unsigned char m_nInventorySortOrder;            // 0x30 (descriptive)
};

// The item's CSWItem base sits at offset 0 of CSWSItem.
class CSWItem {
public:
    CSWBaseItem* GetBaseItem();                     // 0x005B4790
};

class CSWSCreatureStats {
public:
    unsigned char GetLevel(int bIncludeNegative);   // 0x005A5FD0
};

class CSWTlkTable {
public:
    CExoString Fetch(unsigned long nStrRef, CSWSObject* pObject, int a, int b);   // 0x005EE390
};

// Server store object; markups are percentages of the item cost.
class CSWSStore {
public:
    int SellItem(CSWSItem* pItem, CSWSCreature* pBuyer);    // 0x005C6F70 (descriptive)
    int AcquireItem(CSWSItem* pItem);               // 0x005C7610 (descriptive)
    char pad_0x0[0x230];
    CItemRepository* m_pItems;                      // 0x230
    char pad_0x234[0x244 - 0x234];
    unsigned long m_nMarkDown;                      // 0x244
    unsigned long m_nMarkUp;                        // 0x248
    unsigned long m_nMarkDownBonus;                 // 0x24c (descriptive)
    unsigned long m_nMarkUpBonus;                   // 0x250 (descriptive)
    char pad_0x254[0x2];
    unsigned char m_nBuySellFlag;                   // 0x256 (1 buy, 2 sell, 3 both)
};

// vtable 0x00756E38; layout "store".
class CSWGuiStore : public CSWGuiPanel {
public:
    CSWGuiStore(CSWGuiManager* pManager);           // 0x006C1C00
    virtual ~CSWGuiStore();                         // 0x006C1470
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006C2190)
    virtual void OnPanelAdded();                    // 0x48 (0x006C2170)
    virtual void OnPanelRemoved();                  // 0x4c (0x006C1650)

    void PopulateStore();                           // 0x006C0610
    void SetDescription(const CExoString& sText);   // 0x006C0690
    unsigned long GetItemBuyValue(CSWSItem* pItem); // 0x006C0790
    unsigned long GetItemSellValue(CSWSItem* pItem);    // 0x006C07F0
    void PopulateInventoryItemListBox();            // 0x006C0850
    void OnControlEntered(CSWGuiControl* pControl); // 0x006C0AA0
    void BuyItem(CSWSItem* pItem);                  // 0x006C0BE0
    void BuyItemCallback(CSWGuiControl* pControl);  // 0x006C0D20
    void SellItem(CSWSItem* pItem);                 // 0x006C0D80
    void SellItemCallback(CSWGuiControl* pControl); // 0x006C0EE0
    void OnControlInvAButton(CSWGuiControl* pControl);  // 0x006C0F40
    void OnControlStoreAButton(CSWGuiControl* pControl);    // 0x006C1130
    void DeleteItemEntries();                       // 0x006C1330
    void ShowSellGUI();                             // 0x006C13C0
    int CreateItemEntry(int* pIndex, CExoArrayList<CSWGuiControl*>* pList, unsigned long oidItem,
                        int bInventory);            // 0x006C1660
    void PopulateStoreItemListBox();                // 0x006C1840
    void ShowBuyGUI();                              // 0x006C1A50
    void OnControlXButton(CSWGuiControl* pControl); // 0x006C1B00
    void InitializeStoreType();                     // 0x006C1B50

    CSWSStore* GetStore();

    int m_n64;                                      // 0x64
    CExoArrayList<CSWGuiStoreItemEntry*> m_lstStoreEntries; // 0x68
    CExoArrayList<CSWGuiStoreItemEntry*> m_lstInvEntries;   // 0x74
    CSWGuiLabel m_lbl80;                            // 0x80
    CSWGuiLabel m_lblTitle;                         // 0x1c0
    CSWGuiLabel m_lbl300;                           // 0x300
    CSWGuiLabel m_lbl440;                           // 0x440
    CSWGuiLabel m_lbl580;                           // 0x580
    CSWGuiLabel m_lblShopTitle;                     // 0x6c0
    CSWGuiLabel m_lblInvTitle;                      // 0x800
    CSWGuiLabel m_lblDescription;                   // 0x940
    CSWGuiLabel m_lblCost;                          // 0xa80
    CSWGuiLabel m_lblCostValue;                     // 0xbc0
    CSWGuiLabel m_lblStock;                         // 0xd00
    CSWGuiLabel m_lblStockValue;                    // 0xe40
    CSWGuiLabel m_lblScrollBar;                     // 0xf80 (descriptive)
    CSWGuiLabel m_lblCredits;                       // 0x10c0
    CSWGuiLabel m_lblCreditsValue;                  // 0x1200
    CSWGuiLabel m_lblBuySell;                       // 0x1340
    CSWGuiListBox m_lbShopItems;                    // 0x1480
    CSWGuiListBox m_lbInvItems;                     // 0x1760
    CSWGuiListBox m_lbDescription;                  // 0x1a40
    CSWGuiButton m_btnCancel;                       // 0x1d20
    CSWGuiButton m_btnExamine;                      // 0x1ee4
    CSWGuiButton m_btnAccept;                       // 0x20a8
    int m_n226c;                                    // 0x226c
    unsigned long m_nGold;                          // 0x2270
    int m_n2274;                                    // 0x2274
    unsigned long m_oidStore;                       // 0x2278
    unsigned long m_oidCustomer;                    // 0x227c
};

#endif // L_SW_GUI_STORE_H
