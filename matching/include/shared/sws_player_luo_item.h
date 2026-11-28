// sws_player_luo_item.h -- CSWSPlayerLUOItem
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/b/sws_message_repository_update.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_PLAYER_LUO_ITEM_H
#define SHARED_SWS_PLAYER_LUO_ITEM_H


struct CSWSPlayerLUOItem {
    unsigned long m_oidItem;                                // 0x0
    unsigned long m_nStackSize;                             // 0x4
    unsigned long m_nCost;                                  // 0x8
};

#endif
