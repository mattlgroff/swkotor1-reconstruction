#ifndef L_SW_GUI_INVENTORY_EQUIP_H
#define L_SW_GUI_INVENTORY_EQUIP_H
#include "l/sw_gui_options_in_game.h"

// Complete drawable sizes recovered from adjacent member construction.
class CSWGuiBorder {
public:
    CSWGuiBorder(); // 0x004167F0
    ~CSWGuiBorder(); // 0x004151C0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent&);
    virtual void v08();
    virtual void Draw(float);
    char pad_4[0x70];
};
class CSWGuiText {
public:
    CSWGuiText(); // 0x00417280
    ~CSWGuiText(); // 0x00415FF0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent&);
    virtual void v08();
    virtual void Draw(float);
    char pad_4[0x6c];
};
class CSWGuiRadioButton : public CSWGuiButton {
public:
    CSWGuiRadioButton(); // 0x006B66F0
    virtual ~CSWGuiRadioButton(); // 0x006BE360
};
#ifdef L_INVENTORY_ENTRY_DTORS
#define L_ENTRY_VTABLE __declspec(novtable)
#else
#define L_ENTRY_VTABLE
#endif
class L_ENTRY_VTABLE CSWGuiInGameItemEntry : public CSWGuiRadioButton {
public:
    CSWGuiInGameItemEntry();
    int GetNumItems();
    __declspec(noinline) virtual ~CSWGuiInGameItemEntry();
    unsigned long m_oidItem; // 0x1c4
    CSWGuiBorder m_icon; // 0x1c8
    CSWGuiBorder m_iconHighlight; // 0x23c
    CSWGuiBorder m_iconOverlay; // 0x2b0
    CSWGuiText m_quantity; // 0x324
    unsigned long m_nItemFlags; // 0x394
    unsigned char m_n398;
};
class L_ENTRY_VTABLE CSWGuiStoreItemEntry : public CSWGuiButton {
public:
    CSWGuiStoreItemEntry();
    __declspec(noinline) virtual ~CSWGuiStoreItemEntry();
    unsigned long m_oidItem;
    CSWGuiBorder m_icon;
    CSWGuiBorder m_iconHighlight;
    CSWGuiBorder m_iconOverlay;
    CSWGuiText m_quantity;
};
#ifdef L_CONTAINER_DTOR
class __declspec(novtable) CSWGuiContainer : public CSWGuiPanel {
#else
class CSWGuiContainer : public CSWGuiPanel {
#endif
public:
    CSWGuiContainer();
    __declspec(noinline) virtual ~CSWGuiContainer(); // 0x006B8050
    void SetupGiveMode(int bKeepSelection); // 0x006B8410
    void Update(int nUnused); // 0x006B8770
    unsigned long m_oidContainer;
    int m_nMode;
    unsigned long m_nContainerFlags;
    CSWGuiLabel m_label70;
    CSWGuiLabel m_label1b0;
    CSWGuiLabel m_label2f0;
    CSWGuiLabel m_label430;
    CSWGuiLabel m_label570;
    CSWGuiLabel m_label6b0;
    CSWGuiListBox m_items;
    CSWGuiButton m_ok;
    CSWGuiButton m_cancel;
    CSWGuiButton m_give;
};
template <class T> class CExoArrayList {
public:
    T* m_pArray;
    int m_nCount;
    int m_nAllocated;
    CExoArrayList() : m_pArray(0), m_nCount(0), m_nAllocated(0) {}
    ~CExoArrayList() { delete[] m_pArray; m_pArray = 0; }
};
class CSWGuiInGameEquip : public CSWGuiPanel {
public:
    CSWGuiInGameEquip();
    __declspec(noinline) virtual ~CSWGuiInGameEquip();
    virtual void Draw(float fDelta);
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    void UpdateInventory(); // 0x006B9970
    void SetCharacter(CSWCCreature*); // 0x006B7590
    void UpdatePortraits(int); // 0x006B5B60
    void OnEnterSlot(CSWGuiControl*); // 0x006B9470
    void CloseDescription(); // 0x006B7E30
    void SetDescriptionVisible(int); // descriptive, 0x006B7680
    CSWCCreature* m_pCreature; // 0x64
    CSWGuiButton m_slots[9]; // 0x68
    CSWGuiLabel m_slotLabels[9]; // 0x104c
    CExoArrayList<CSWGuiInGameItemEntry*> m_entries; // 0x1b8c
    CSWGuiLabel m_label1b98;
    CSWGuiLabel m_label1cd8;
    CSWGuiLabel m_label1e18;
    CSWGuiLabel m_label1f58;
    CSWGuiLabel m_label2098;
    CSWGuiLabel m_label21d8;
    CSWGuiLabel m_label2318;
    CSWGuiLabel m_label2458;
    CSWGuiLabel m_label2598;
    CSWGuiLabel m_label26d8;
    CSWGuiLabel m_label2818;
    CSWGuiLabel m_label2958;
    CSWGuiLabel m_label2a98;
    CSWGuiLabel m_label2bd8;
    CSWGuiLabel m_label2d18;
    CSWGuiLabel m_label2e58;
    CSWGuiLabel m_label2f98;
    CSWGuiListBox m_items; // 0x30d8
    CSWGuiListBox m_description; // 0x33b8
    CSWGuiButton m_button3698;
    CSWGuiButton m_button385c;
    CSWGuiButton m_button3a20;
    CSWGuiButton m_button3be4;
    CSWGuiButton m_button3da8;
    CSWGuiButton m_button3f6c;
    CSWGuiLabel m_label4130;
    unsigned long m_nEquipFlags; // 0x4270
    unsigned char m_nSelected;
    char pad_4275[3];
    unsigned long m_nSlot;
    unsigned long m_oidEquipped[9];
    CSWGuiControl* m_pSavedControl;
    unsigned long m_nCompareSlot;
    unsigned long m_oidCompareItem;
    int m_n42ac;
    CSWGuiControl* m_pControl42b0;
    int m_n42b4;
    CSWGuiControl* m_pControl42b8;
};
class CSWGuiInGameInventory : public CSWGuiPanel {
public:
    CSWGuiInGameInventory();
    __declspec(noinline) virtual ~CSWGuiInGameInventory();
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    virtual void Draw(float fDelta);
    void UpdatePortraits();
    void PopulateItemListBox();
    void UpdatePlayerStats();
    CSWGuiLabel m_label64;
    CSWGuiLabel m_label1a4;
    CSWGuiLabel m_label2e4;
    CSWGuiLabel m_label424;
    CSWGuiListBox m_items; // 0x564
    CSWGuiListBox m_description; // 0x844
    CSWGuiLabel m_labelb24;
    CSWGuiLabel m_labelc64;
    CSWGuiLabel m_labelda4;
    CSWGuiLabel m_labelee4;
    CSWGuiLabel m_label1024;
    CSWGuiButton m_button1164;
    CSWGuiButton m_button1328;
    CSWGuiButton m_button14ec;
    CSWGuiButton m_button16b0;
    CSWGuiButton m_button1874;
    CSWGuiButton m_button1a38;
    CSWGuiButton m_button1bfc;
    CExoArrayList<CSWGuiInGameItemEntry*> m_entries; // 0x1dc0
    CExoArrayList<unsigned long> m_newItems; // 0x1dcc
    unsigned long m_oidCreature;
    int m_n1ddc;
    unsigned char m_nSelected;
    char pad_1de1[3];
    unsigned long m_nInventoryFlags;
};
#endif
