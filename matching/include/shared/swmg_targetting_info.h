// swmg_targetting_info.h -- CSWMGTargettingInfo
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_mini_game_object.h); the other copies' members fall on its members.
#ifndef SHARED_SWMG_TARGETTING_INFO_H
#define SHARED_SWMG_TARGETTING_INFO_H


class CSWMGTargettingInfo {
public:
    // Defined in m/sw_mg_gun_bank.cpp, whose constructors inline it.
    CSWMGTargettingInfo();

    float m_fHorizSpread;                           // 0x00
    float m_fSensingRadius;                         // 0x04
    float m_fVertSpread;                            // 0x08
    float m_fInaccuracy;                            // 0x0c
};

#endif
