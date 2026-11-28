// sw_base_item_data.h -- CSWBaseItemData
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_action_menu.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_BASE_ITEM_DATA_H
#define SHARED_SW_BASE_ITEM_DATA_H


class CSWBaseItemData {
public:
    char pad_0x0[0x78];
    unsigned char  m_nDropSound;                 // 0x78
    char pad_0x79[0x13];
    unsigned char  m_nWeaponSoundRow;            // 0x8c
    char pad_0x8d[0x1f];
    unsigned char m_nCategory;                      // 0xac (6: usable on hostiles)
};

#endif
