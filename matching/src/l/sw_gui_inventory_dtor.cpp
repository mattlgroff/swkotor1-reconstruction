// CSWGuiInGameInventory destructor (vtable 0x007564E0, 0x1DE8 bytes). This
// object inlines ~CSWGuiLabel, so it uses the title-movies drawable family
// (real CSWGuiBorder / CSWGuiText label members) instead of the padded
// l/sw_gui_inventory_equip.h layout.
#include "l/sw_gui_title_movies.h"

class CSWGuiInGameItemEntry {
public:
    virtual ~CSWGuiInGameItemEntry();               // 0x006B7FA0
};

class CSWGuiInGameInventory : public CSWGuiPanel {
public:
    __declspec(noinline) virtual ~CSWGuiInGameInventory();

    CSWGuiLabel m_label64;                          // 0x64
    CSWGuiLabel m_label1a4;                         // 0x1a4
    CSWGuiLabel m_label2e4;                         // 0x2e4
    CSWGuiLabel m_label424;                         // 0x424
    CSWGuiListBox m_items;                          // 0x564
    CSWGuiListBox m_description;                    // 0x844
    CSWGuiLabel m_labelb24;                         // 0xb24
    CSWGuiLabel m_labelc64;                         // 0xc64
    CSWGuiLabel m_labelda4;                         // 0xda4
    CSWGuiLabel m_labelee4;                         // 0xee4
    CSWGuiLabel m_label1024;                        // 0x1024
    CSWGuiButton m_button1164;                      // 0x1164
    CSWGuiButton m_button1328;                      // 0x1328
    CSWGuiButton m_button14ec;                      // 0x14ec
    CSWGuiButton m_button16b0;                      // 0x16b0
    CSWGuiButton m_button1874;                      // 0x1874
    CSWGuiButton m_button1a38;                      // 0x1a38
    CSWGuiButton m_button1bfc;                      // 0x1bfc
    CExoArrayList<CSWGuiInGameItemEntry*> m_entries;    // 0x1dc0
    CExoArrayList<unsigned long> m_newItems;        // 0x1dcc
    char pad_0x1dd8[0x1de8 - 0x1dd8];
};

// FUNCTION: SWKOTOR 0x006B40C0
CSWGuiInGameInventory::~CSWGuiInGameInventory()
{
    m_description.ClearControls();
    m_items.ClearControls();
    for (int i = 0; i < m_entries.num; i++) {
        if (m_entries.element[i]) {
            delete m_entries.element[i];
        }
        m_entries.element[i] = 0;
    }
    m_entries.num = 0;
}
