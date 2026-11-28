// swc_item_property.h -- CSWCItemProperty
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_c_item_object.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_ITEM_PROPERTY_H
#define SHARED_SWC_ITEM_PROPERTY_H


class CSWCItemProperty {
public:
    unsigned short m_nPropertyType;
    unsigned short m_nSubtype;
    unsigned short  m_n04;                       // 0x4
    unsigned short  m_nCostValue;                // 0x6
    unsigned char  m_nParam1Value;               // 0x8
    char pad_0x9[0x3];
    int  m_bActive;                              // 0xc
    unsigned char  m_nUsesLeft;                  // 0x10
    unsigned char  m_nUpgradeType;               // 0x11
    char pad_0x12[0xa];
};

#endif
