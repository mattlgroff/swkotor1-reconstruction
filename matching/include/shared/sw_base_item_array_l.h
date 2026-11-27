// sw_base_item_array_l.h -- CSWBaseItemArrayL
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/l/sw_gui_debug_menu.h); the other copies' members fall on its members.
#ifndef SHARED_SW_BASE_ITEM_ARRAY_L_H
#define SHARED_SW_BASE_ITEM_ARRAY_L_H


class CSWBaseItem;

class CSWBaseItemArrayL {
public:
    int m_nNumBaseItems;                            // 0x00

    // declared by other copies
    CSWBaseItem* GetBaseItem(int nBaseItem);
};

#endif
