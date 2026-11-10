// Worker L matching headers: the plot-item screen CSWGuiQuestItem
// (SWGuiQuestItem.o, layout "QuestItem", vtable 0x00757C20, 0xA70 bytes,
// 0x006D2460-0x006D2C20).
#ifndef L_SW_GUI_QUEST_ITEM_H
#define L_SW_GUI_QUEST_ITEM_H

#include "l/sw_gui_inventory.h"

class CSWGuiQuestItem : public CSWGuiPanel {
public:
    CSWGuiQuestItem(CSWGuiManager* pManager);       // 0x006D26C0
    virtual ~CSWGuiQuestItem();                     // 0x006D28F0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006D24D0)
    virtual void OnPanelAdded();                    // 0x48 (0x006D2C20)

    void OnControlEntered(CSWGuiControl* pControl); // 0x006D2580
    void PopulateItemListBox();                     // 0x006D29F0

    CExoLinkedList<CSWGuiInGameItemEntry> m_lstEntries;     // 0x64
    CSWGuiListBox m_lbDescription;                  // 0x68 LB_ITEM_DESCRIPTION
    CSWGuiLabel m_lblDescription;                   // 0x348 (description item)
    CSWGuiListBox m_lbItems;                        // 0x488 LB_ITEMS
    CSWGuiLabel m_lblTitle;                         // 0x768 LBL_TITLE
    CSWGuiButton m_btnBack;                         // 0x8a8 BTN_BACK
    CSWGuiControl* m_pSelected;                     // 0xa6c
};

#endif
