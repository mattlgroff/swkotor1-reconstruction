// CSWGuiInGameEquip destructor (vtable 0x007569A0, 0x42BC bytes). Like the
// inventory destructor this object inlines ~CSWGuiLabel, so it uses the
// title-movies drawable family instead of l/sw_gui_inventory_equip.h.
#include "l/sw_gui_title_movies.h"

class CSWGuiInGameItemEntry {
public:
    virtual ~CSWGuiInGameItemEntry();               // 0x006B7FA0
};

class CSWCCreature;

class CSWGuiInGameEquip : public CSWGuiPanel {
public:
    __declspec(noinline) virtual ~CSWGuiInGameEquip();

    CSWCCreature* m_pCreature;                      // 0x64
    CSWGuiButton m_slots[9];                        // 0x68
    CSWGuiLabel m_slotLabels[9];                    // 0x104c
    CExoArrayList<CSWGuiInGameItemEntry*> m_entries;    // 0x1b8c
    CSWGuiLabel m_label1b98;                        // 0x1b98
    CSWGuiLabel m_label1cd8;                        // 0x1cd8
    CSWGuiLabel m_label1e18;                        // 0x1e18
    CSWGuiLabel m_label1f58;                        // 0x1f58
    CSWGuiLabel m_label2098;                        // 0x2098
    CSWGuiLabel m_label21d8;                        // 0x21d8
    CSWGuiLabel m_label2318;                        // 0x2318
    CSWGuiLabel m_label2458;                        // 0x2458
    CSWGuiLabel m_label2598;                        // 0x2598
    CSWGuiLabel m_label26d8;                        // 0x26d8
    CSWGuiLabel m_label2818;                        // 0x2818
    CSWGuiLabel m_label2958;                        // 0x2958
    CSWGuiLabel m_label2a98;                        // 0x2a98
    CSWGuiLabel m_label2bd8;                        // 0x2bd8
    CSWGuiLabel m_label2d18;                        // 0x2d18
    CSWGuiLabel m_label2e58;                        // 0x2e58
    CSWGuiLabel m_label2f98;                        // 0x2f98
    CSWGuiListBox m_items;                          // 0x30d8
    CSWGuiListBox m_description;                    // 0x33b8
    CSWGuiButton m_button3698;                      // 0x3698
    CSWGuiButton m_button385c;                      // 0x385c
    CSWGuiButton m_button3a20;                      // 0x3a20
    CSWGuiButton m_button3be4;                      // 0x3be4
    CSWGuiButton m_button3da8;                      // 0x3da8
    CSWGuiButton m_button3f6c;                      // 0x3f6c
    CSWGuiLabel m_label4130;                        // 0x4130
    char pad_0x4270[0x42b0 - 0x4270];
    CSWGuiControl* m_pControl42b0;                  // 0x42b0
    int m_n42b4;                                    // 0x42b4
    CSWGuiControl* m_pControl42b8;                  // 0x42b8
};

// FUNCTION: SWKOTOR 0x006B8840
CSWGuiInGameEquip::~CSWGuiInGameEquip()
{
    m_description.ClearControls();
    m_items.ClearControls();
    for (int i = 0; i < m_entries.num; i++) {
        if (m_entries.element[i]) {
            delete m_entries.element[i];
        }
        m_entries.element[i] = 0;
    }
    m_entries.SetSize(0);
    if (m_pControl42b0) {
        delete m_pControl42b0;
        m_pControl42b0 = 0;
    }
    if (m_pControl42b8) {
        delete m_pControl42b8;
        m_pControl42b8 = 0;
    }
}
