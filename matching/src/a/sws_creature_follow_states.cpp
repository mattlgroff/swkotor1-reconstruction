#include "a/sws_creature_follow_states.h"

// Descriptive reference names; the unused third argument is a speed output
// pointer in neighboring follow-state methods (four stack args, RET 0x10).
// FUNCTION: SWKOTOR 0x00512A60
unsigned long CSWSCreature::FollowLeaderOffsetWaitDest(CFollowNode *pNode, CSWFollowPath *pPath,
                                                float* pSpeed, int nArg4)
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
            pNode->m_nState = 1;
            return ACTION_IN_PROGRESS;
        }
        i++;
        if (i >= 100) {
            i = 0;
        }
    }
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x00512B40
unsigned long CSWSCreature::FollowLeaderPathWaitDest(CFollowNode *pNode, CSWFollowPath *pPath,
                                                float* pSpeed, int nArg4)
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
            pNode->m_nState = 3;
            return ACTION_IN_PROGRESS;
        }
        i++;
        if (i >= 100) {
            i = 0;
        }
    }
    return ACTION_COMPLETE;
}

// FUNCTION: SWKOTOR 0x00512C20
unsigned long CSWSCreature::FollowLeaderToOffsetWaitDest(CFollowNode *pNode, CSWFollowPath *pPath,
                                                float* pSpeed, int nArg4)
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
            pNode->m_nState = 4;
            return ACTION_IN_PROGRESS;
        }
        i++;
        if (i >= 100) {
            i = 0;
        }
    }
    return ACTION_COMPLETE;
}
