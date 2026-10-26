#ifndef A_SWS_CREATURE_FOLLOW_STATES_H
#define A_SWS_CREATURE_FOLLOW_STATES_H
// Batch-local follow state declarations copied from the exact 00512D00
// sibling and checked against 00512A60/00512B40/00512C20.

class Vector {
public:
    float x, y, z;
};

inline Vector operator-(const Vector &a, const Vector &b)
{
    Vector r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

inline float MagnitudeSquared(const Vector &v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

extern const unsigned long ACTION_IN_PROGRESS;  // 0x00745DBC
extern const unsigned long ACTION_COMPLETE;     // 0x00745DC0

class CSWFollowPath {
public:
    Vector *GetFollowPoint(int n);              // 0x00634CC0
    char pad_0x0[0xfa4];
    int m_nCurrent;                             // 0xfa4
};

struct CFollowNode {
    char pad_0x0[0x4];
    int m_nState;                               // 0x04
    char pad_0x8[0x10];
    Vector m_vStart;                            // 0x18
    int m_nStart;                               // 0x24
    Vector m_vEnd;                              // 0x28
    int m_nEnd;                                 // 0x34
};

class CSWSCreature {
public:
    unsigned long FollowLeaderOffsetWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
    unsigned long FollowLeaderPathWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
    unsigned long FollowLeaderToOffsetWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
};


#endif
