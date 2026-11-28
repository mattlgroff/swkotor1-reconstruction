// sws_player_luo_sorted_object_list.h -- CSWSPlayerLUOSortedObjectList
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/b/sws_message_sort_objects.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_PLAYER_LUO_SORTED_OBJECT_LIST_H
#define SHARED_SWS_PLAYER_LUO_SORTED_OBJECT_LIST_H


class CSWSPlayerLUOSortedObjectList {
public:
    CSWSPlayerLUOSortedObjectList();                        // 0x00566D80

    unsigned long m_oidObject;                              // 0x0
    float m_fDistance;                                      // 0x4
    unsigned long m_nCategory;                              // 0x8
};

#endif
