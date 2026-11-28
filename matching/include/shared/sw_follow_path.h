// sw_follow_path.h -- CSWFollowPath
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/a/sws_creature_follow_states.h); the other copies' members fall on its members.
#ifndef SHARED_SW_FOLLOW_PATH_H
#define SHARED_SW_FOLLOW_PATH_H

#include "shared/vector.h"

struct NPartyVector;

class CSWFollowPath {
public:
    Vector *GetFollowPoint(int n);              // 0x00634CC0
    char pad_0x0[0xfa4];
    int m_nCurrent;                             // 0xfa4

    // declared by other copies
    int GetFollowPoint(float fDistance, NPartyVector* pPoint, unsigned long* pPointIndex, int nSide);
};

#endif
