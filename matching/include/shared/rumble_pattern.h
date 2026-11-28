// rumble_pattern.h -- CRumblePattern
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/k/sw_c_rumble_pattern.cpp); the other copies' members fall on its members.
#ifndef SHARED_RUMBLE_PATTERN_H
#define SHARED_RUMBLE_PATTERN_H

#include "shared/rumble_channel.h"

class CRumblePattern {
public:
    CRumbleChannel m_cLeft;                         // 0x00
    CRumbleChannel m_cRight;                        // 0x10
    int m_bExclusive;                               // 0x20

    int GetIntensity(float fTime, float* pfLeft, float* pfRight); // 0x0068FDD0
};

#endif
