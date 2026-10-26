// CSWParty follow-path tests (swparty.o).
#include "n/sw_party.h"

// FUNCTION: SWKOTOR 0x00635350
int CSWParty::IsPartyTogether()
{
    unsigned long nPlayer = g_pAppManager->m_pClientExoApp->GetPlayerCreatureId();
    unsigned long nServerPlayer = g_pAppManager->m_pServerExoApp->ClientToServerObjectID(nPlayer);
    CSWSCreature* pLeader = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(nServerPlayer);
    if (pLeader == 0)
        return 0;
    pLeader->GetArea();
    Vector vLeader = pLeader->m_vPosition;
    for (int i = 0; i < m_nCount; ++i) {
        unsigned long nMember = g_pAppManager->m_pServerExoApp->ClientToServerObjectID(m_aMembers[i].m_oid);
        CSWSCreature* pMember = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(nMember);
        if (pMember == 0)
            continue;
        Vector vMember = pMember->m_vPosition;
        Vector vDiff = vLeader - vMember;
        if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z > 900.0f)
            return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00636840
int CSWParty::TestAndResetPath()
{
    int nHead = m_cFollow.m_nHead - 1;
    if (nHead < 0)
        nHead += 100;
    float fFacing = m_cFollow.GetPathData(nHead)->m_fFacing;
    Vector vForward(0.0f, 1.0f, 0.0f);
    Vector vAxis(0.0f, 0.0f, 1.0f);
    Vector vFacing = Quaternion(vAxis, DegreesToRadians(fFacing)) * vForward;
    int nClosest = m_cFollow.FindClosestPartyMemeber_Current();
    Vector vDirection = m_aMembers[nClosest].m_vDirection;
    if (vFacing.x * vDirection.x + vFacing.y * vDirection.y + vFacing.z * vDirection.z < 0.0f) {
        RecalculateFollowPoint();
        for (int i = 1; i <= 2; ++i) {
            if (GetCharacter(i)) {
                int& nState = m_aMembers[i].m_nFollowState;
                if (nState != -1 && nState != 0 && nState != 5 && nState != 6 && nState != 7) {
                    m_aMembers[i].m_nPathIndex = m_aMembers[i].m_nFollowPointIndex;
                    m_aMembers[i].m_vPathPoint = m_aMembers[i].m_vFollowPoint;
                    nState = 4;
                }
            }
        }
        InvertFormation();
        return 1;
    }
    return 0;
}
