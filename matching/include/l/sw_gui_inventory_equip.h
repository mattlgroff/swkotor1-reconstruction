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
    CSWGuiExtent GetInnerExtent(); // 0x00415240
    char pad_4[0x10];
    CSWGuiBorderParams m_cParams; // 0x14
    char pad_0x68[0xc];
};
class CSWGuiText {
public:
    CSWGuiText(); // 0x00417280
    ~CSWGuiText(); // 0x00415FF0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent&);
    virtual void v08();
    virtual void Draw(float);
    char pad_4[0x14];
    CSWGuiTextParams m_cParams; // 0x18
};
class CSWGuiRadioButton : public CSWGuiButton {
public:
#ifdef L_RADIO_BUTTON_INLINE
    // The save/load entry inlines both (0x006B66F0 / 0x006BE360 bodies).
    CSWGuiRadioButton() {}
    virtual ~CSWGuiRadioButton() {}
#else
    CSWGuiRadioButton(); // 0x006B66F0
    virtual ~CSWGuiRadioButton(); // 0x006BE360
#endif
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
#ifdef L_GUI_CONTROL_DRAW_SLOT
    void Draw(float fDelta);                        // 0x38 (0x006B4D40)
#endif
    virtual void v9C();
    virtual void Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder, CSWGuiBorder* pHilight,
                            int nWidth, int bPulse);    // 0xa0 (0x006B4DB0)
    void SetCanUse(unsigned char nState);           // 0x006B5060
    void SetItem(unsigned long oidItem, int a, int b);  // 0x006B6710
    unsigned long m_oidItem; // 0x1c4
    CSWGuiBorder m_icon; // 0x1c8
    CSWGuiBorder m_iconHighlight; // 0x23c
    CSWGuiBorder m_iconOverlay; // 0x2b0
    CSWGuiText m_quantity; // 0x324
    union {
        unsigned long m_nItemFlags; // 0x394
        struct {
            unsigned long m_bPulse : 1;             //       bit 0
        };
    };
    unsigned char m_n398;
};
class L_ENTRY_VTABLE CSWGuiStoreItemEntry : public CSWGuiButton {
public:
    CSWGuiStoreItemEntry();
    __declspec(noinline) virtual ~CSWGuiStoreItemEntry();
    void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x006B5270)
    virtual void v9C();
    virtual void Initialize(CSWGuiText* pText, CSWGuiBorder* pBorder,
                            CSWGuiBorder* pHilight, int nWidth);    // 0xa0 (0x006B53F0)
    void SetItem(unsigned long oidItem, int bUnused); // 0x006B7270
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
    virtual void HandleInputEvent(int nEvent, int bDown); // 0x3c (0x006B92F0)
    virtual void OnPanelRemoved(); // 0x4c (0x006B5250)
    void SetupGiveMode(int bKeepSelection); // 0x006B8410
    void Update(int nUnused); // 0x006B8770
    virtual void OnPanelAdded(); // 0x48 (0x006B51D0)
    void SetContainer(unsigned long oidContainer, int nMode); // 0x006B8130
    void GiveItem(CSWGuiControl* pControl); // 0x006B7170
    unsigned long m_oidContainer;
    unsigned char m_nMode; // 0x68
    char pad_69[3];
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
    CExoArrayList()
    {
        m_nCount = 0;
        m_nAllocated = 0;
        m_pArray = 0;
    }
    ~CExoArrayList() { delete[] m_pArray; m_pArray = 0; }
    void Allocate(int nSize);                       // 0x005E03C0 (folded)
    void SetSize(int nSize)
    {
        if (nSize == 0) {
            T* pArray = m_pArray;
            m_nCount = 0;
            if (pArray) {
                delete[] pArray;
                m_pArray = 0;
                m_nAllocated = 0;
            }
        } else {
            Allocate(nSize);
        }
        m_nCount = nSize;
    }
    void DelIndex(int nIndex);                      // 0x006857A0 (folded)
    void AddUnique(T t);                            // 0x006449D0 (unsigned long instance)
    int IndexOf(T t)
    {
        for (int i = 0; i < m_nCount; i++) {
            if (m_pArray[i] == t) {
                return i;
            }
        }
        return -1;
    }
    void Add(T t)
    {
        if (m_nCount == m_nAllocated) {
            Allocate(m_nAllocated ? m_nAllocated * 2 : 0x10);
        }
        m_pArray[m_nCount] = t;
        m_nCount++;
    }
    __forceinline void InsertInline(T t, int nPos)
    {
        int nIndex = m_nCount;
        AddInline(t);
        for (; nIndex > nPos; nIndex--) {
            m_pArray[nIndex] = m_pArray[nIndex - 1];
        }
        m_pArray[nPos] = t;
    }
    __forceinline void AddInline(T t)
    {
        if (m_nCount == m_nAllocated) {
            int nSize = m_nAllocated ? m_nAllocated * 2 : 0x10;
            T* pOld = m_pArray;
            m_nAllocated = nSize;
            m_pArray = new T[nSize];
            for (int i = 0; i < m_nCount; i++) {
                m_pArray[i] = pOld[i];
            }
            if (pOld) {
                delete[] pOld;
            }
        }
        m_pArray[m_nCount] = t;
        m_nCount++;
    }
};
#ifdef L_INVENTORY_PANEL_DTORS
#define L_INVENTORY_PANEL_VTABLE __declspec(novtable)
#else
#define L_INVENTORY_PANEL_VTABLE
#endif
class L_INVENTORY_PANEL_VTABLE CSWGuiInGameEquip : public CSWGuiPanel {
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
    void ShowDescription(int bShow); // 0x006B7680
    void OnSlotSelected(CSWGuiControl* pControl); // 0x006B8EB0 (descriptive)
    int ItemExists(unsigned long oidItem); // 0x006B66A0
    int EquipItem(unsigned long oidItem, unsigned long nSlot, int bClearActions); // 0x006B5760
    int EquipItem(class CSWSItem* pCopy, unsigned long nSlot, int bClearActions); // 0x006B5890
    CExoString GetIcon(int nSlot); // 0x006B5600
    void SetDescription(const CExoString& sText); // 0x006B56B0
    class CSWSItem* CopyItem(unsigned long oidItem); // 0x006B65E0
    void UnequipItem(unsigned long oidItem, int bClearActions); // 0x006B5910
    CSWGuiInGameItemEntry* AddItemEntryToList(int nIndex); // 0x006B91C0
    void ShowCantEquipMessage(int bShow, int nStrRef); // 0x006B59F0
    void OnItemSelected(CSWGuiControl* pControl); // 0x006B7920 (descriptive)
    void OnOKPressed(CSWGuiControl* pControl); // 0x006B9160
    void OnSwitchLeft(CSWGuiControl* pControl); // 0x006B60F0
    void OnChangeCharacter(CSWGuiControl* pControl); // 0x006BA820
    void OnSwitchRight(CSWGuiControl* pControl); // 0x006B6370
    int GetSlotItemIdIndex(int nSlotMask); // 0x006B4C90
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
class L_INVENTORY_PANEL_VTABLE CSWGuiInGameInventory : public CSWGuiPanel {
public:
    CSWGuiInGameInventory();
    __declspec(noinline) virtual ~CSWGuiInGameInventory();
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    virtual void Draw(float fDelta);
    virtual void HandleInputEvent(int nEvent, int bDown); // 0x3c (0x006B3ED0)
    void UpdatePortraits(); // 0x006B2DC0
    void PopulateItemListBox(); // 0x006B4810
    void UpdatePlayerStats(); // 0x006B28C0
    void SetNextFilter(); // 0x006B2A80
    void OnChangeCharacter(CSWGuiControl* pControl); // 0x006B2EC0
    void OnSwitchLeft(CSWGuiControl* pControl); // 0x006B2FD0
    void OnControlSelected(CSWGuiControl* pControl); // 0x006B25E0
    void OnControlEntered(CSWGuiControl* pControl); // 0x006B3D10
    void OnCantEquip(CSWGuiControl* pControl); // 0x006B26E0 (descriptive)
    void OnNotUseable(CSWGuiControl* pControl); // 0x006B2740 (descriptive)
    void OnNotUseableOnHawk(CSWGuiControl* pControl); // 0x006B27A0 (descriptive)
    void OnFullHealth(CSWGuiControl* pControl); // 0x006B2800 (descriptive)
    void OnSquadFullHealth(CSWGuiControl* pControl); // 0x006B2860 (descriptive)
    void OnSwitchRight(CSWGuiControl* pControl); // 0x006B3250
    int CheckFilter(class CSWSItem* pItem, class CSWSCreature* pCreature, int bEquipped); // 0x006B2C90
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
    unsigned long m_oidUsedItem; // 0x1dd8
    short m_nSavedTop; // 0x1ddc
    short m_nSavedSelected; // 0x1dde
    unsigned char m_nSelected; // 0x1de0
    char pad_1de1[3];
    union {
        unsigned long m_nInventoryFlags; // 0x1de4
        struct {
            unsigned long m_bInvFlag0 : 1;
            unsigned long m_bInvFlag1 : 1;
            unsigned long m_bInvFlag2 : 1;
        };
    };
};
#endif
