// follow_node.h -- CFollowNode
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/a/sws_creature_follow_states.h); the other copies' members fall on its members.
#ifndef SHARED_FOLLOW_NODE_H
#define SHARED_FOLLOW_NODE_H

#include "shared/vector.h"

struct CFollowNode {
    char pad_0x0[0x4];
    int m_nState;                               // 0x04
    char pad_0x8[0x10];
    Vector m_vStart;                            // 0x18
    int m_nStart;                               // 0x24
    Vector m_vEnd;                              // 0x28
    int m_nEnd;                                 // 0x34
    char pad_38[0x5c-0x38];
    Vector m_vTeleport;
};

#endif
