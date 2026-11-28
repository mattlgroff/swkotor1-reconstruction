#ifndef A_SWS_CREATURE_FOLLOW_STATES_H
#define A_SWS_CREATURE_FOLLOW_STATES_H
// Batch-local follow state declarations copied from the exact 00512D00
// sibling and checked against 00512A60/00512B40/00512C20.

#include "shared/vector.h"

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

#include "shared/globals.h"

#include "shared/sw_follow_path.h"

#include "shared/follow_node.h"

class CPathfindInformation;
class CSWSArea { public: int TestSafePoint(const Vector* p, CPathfindInformation* pPath); }; // 0x004BE5E0, descriptive name

class CSWSCreature {
public:
    unsigned long FollowLeaderTeleportForward(CFollowNode*, CSWFollowPath*, float*, int);
    CSWSArea* GetArea(); // 0x004CB120
    unsigned long FollowLeaderOffsetWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
    unsigned long FollowLeaderPathWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
    unsigned long FollowLeaderToOffsetWaitDest(CFollowNode*, CSWFollowPath*, float*, int);
    char pad_0[0x340];
    CPathfindInformation* m_pPathData;
};


#endif
