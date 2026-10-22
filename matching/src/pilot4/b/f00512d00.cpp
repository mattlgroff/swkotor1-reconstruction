// Pilot 4 arm B: 0x00512D00, creature follow-point path check.

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
    unsigned long AIActionCheckFollow(CFollowNode *pNode, CSWFollowPath *pPath,
                                      int nArg3, int nArg4);
};

// FUNCTION: SWKOTOR 0x00512D00
unsigned long CSWSCreature::AIActionCheckFollow(CFollowNode *pNode, CSWFollowPath *pPath,
                                                int nArg3, int nArg4)
{
    int nStart = pNode->m_nStart;
    int nEnd = pNode->m_nEnd;
    int nCurrent = pPath->m_nCurrent;
    int i = nStart;
    while (i != nCurrent) {
        if (i == nEnd) {
            if (nEnd == nStart) {
                Vector v = *pPath->GetFollowPoint(nStart);
                Vector d1 = v - pNode->m_vStart;
                Vector d2 = v - pNode->m_vEnd;
                if (!(MagnitudeSquared(d1) > MagnitudeSquared(d2))) {
                    return ACTION_COMPLETE;
                }
            }
            pNode->m_nState = 2;
            return ACTION_IN_PROGRESS;
        }
        i++;
        if (i >= 100) {
            i = 0;
        }
    }
    return ACTION_COMPLETE;
}
