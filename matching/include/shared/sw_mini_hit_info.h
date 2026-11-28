// sw_mini_hit_info.h -- CSWMiniHitInfo
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_mini_game_object.h); the other copies' members fall on its members.
#ifndef SHARED_SW_MINI_HIT_INFO_H
#define SHARED_SW_MINI_HIT_INFO_H

#include "shared/res_ref.h"

class CSWMiniHitInfo {
public:
    // Defined in m/sw_mg_gun_bank.cpp, whose constructors inline it.
    CSWMiniHitInfo();

    int m_nDamage;                                  // 0x00
    float m_fSpeed;                                 // 0x04
    float m_fRateOfFire;                            // 0x08
    float m_fLifespan;                              // 0x0c
    int m_nTargetMask;                              // 0x10
    int m_n14;                                      // 0x14
    CResRef m_cSound;                               // 0x18
};

#endif
