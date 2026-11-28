// sws_creature_appearance_info.h -- CSWSCreatureAppearanceInfo
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/b/sws_creature.h); the other copies' members fall on its members.
#ifndef SHARED_SWS_CREATURE_APPEARANCE_INFO_H
#define SHARED_SWS_CREATURE_APPEARANCE_INFO_H


struct CSWSCreatureAppearanceInfo {
    unsigned long m_aParts[4];                              // 0x0
    unsigned short m_nAppearance;                           // 0x10
    unsigned char m_n12;                                    // 0x12
    unsigned char m_n13;                                    // 0x13
    unsigned char m_n14;                                    // 0x14
    unsigned char m_n15;                                    // 0x15
    unsigned char m_n16;                                    // 0x16
    unsigned char m_n17;                                    // 0x17
    unsigned char m_n18;                                    // 0x18
    unsigned char m_n19;                                    // 0x19
    unsigned char m_n1A;                                    // 0x1a
    char pad_0x1b[0x1c - 0x1b];
    unsigned long m_n1C;                                    // 0x1c
    unsigned char m_n20;                                    // 0x20
    char pad_0x21[0x24 - 0x21];
};

#endif
