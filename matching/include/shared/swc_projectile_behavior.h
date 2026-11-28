// swc_projectile_behavior.h -- CSWCProjectileBehavior
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/l/sw_c_projectile.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_PROJECTILE_BEHAVIOR_H
#define SHARED_SWC_PROJECTILE_BEHAVIOR_H

#include "shared/vector.h"

struct CSWCProjectileBehavior {
    Vector m_vStart;                                // 0x00
    Vector m_vEnd;                                  // 0x0c
    unsigned long m_oidTarget;                      // 0x18
    int m_nFlags;                                // 0x1c
    unsigned long m_nDuration;                      // 0x20
    unsigned long m_nDelay;                         // 0x24
    float m_fSpeed;                                 // 0x28
    unsigned char m_nState;                         // 0x2c
    unsigned char m_nType;                          // 0x2d
    char pad_0x2e[2];
    unsigned long m_n30;                            // 0x30
};

#endif
