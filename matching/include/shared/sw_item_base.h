// sw_item_base.h -- CSWItemBase
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_ignore_s8.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_ITEM_BASE_H
#define SHARED_SW_ITEM_BASE_H


class CSWItemBase {
public:
    virtual void v00();
    char pad_0x4[0x8];
    unsigned long m_nBaseItem;                      // 0x0c
};

#endif
