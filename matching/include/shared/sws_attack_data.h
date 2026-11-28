// sws_attack_data.h -- CSWSAttackData
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/a/effect_list_handler7.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_ATTACK_DATA_H
#define SHARED_SWS_ATTACK_DATA_H


struct CSWSAttackData {
    char pad_0x0[0x10];
    short  m_nAttackDelay;                       // 0x10
    char pad_0x12[0x2];
    short  m_nReactionDelay;                     // 0x14
    char pad_0x16[0x24];
    unsigned char  m_nWeaponAttackType;          // 0x3a
    char pad_0x3b[0xf2];
    unsigned char m_nAttackResult;              // 0x12d (placeholder name)
    char pad_0x12e[0x13c - 0x12e];
    int m_bEffectFeedback;                      // 0x13c (placeholder name)
};

#endif
