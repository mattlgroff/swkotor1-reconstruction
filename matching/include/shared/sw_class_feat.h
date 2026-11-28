// sw_class_feat.h -- CSWClassFeat
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/class_getters.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_CLASS_FEAT_H
#define SHARED_SW_CLASS_FEAT_H


struct CSWClassFeat {
    unsigned short m_nFeat;          // +0x0
    unsigned char m_nLevel;          // +0x2
    unsigned char m_nFlags;          // +0x3
    int m_nGrantedLevel;             // +0x4
};

#endif
