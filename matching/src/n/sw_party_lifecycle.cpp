// CSWParty construction and reset (swparty.o).
#include "n/sw_party.h"

// FUNCTION: SWKOTOR 0x00637890
void CSWParty::ResetAllFollowData(const Vector& vPosition, float fFacing, int bFormation)
{
    Reset(vPosition, fFacing, bFormation);

    m_aMembers[0].m_nFollowState = -1;
    m_aMembers[0].m_vPathPoint = vPosition;
    m_aMembers[0].m_nPathIndex = 0;
    m_aMembers[0].m_vFollowPoint = vPosition;
    m_aMembers[0].m_nFollowPointIndex = 0;

    m_aMembers[1].m_nFollowState = -1;
    m_aMembers[1].m_vPathPoint = vPosition;
    m_aMembers[1].m_nPathIndex = 0;
    m_aMembers[1].m_vFollowPoint = vPosition;
    m_aMembers[1].m_nFollowPointIndex = 0;

    m_aMembers[2].m_nFollowState = -1;
    m_aMembers[2].m_vPathPoint = vPosition;
    m_aMembers[2].m_nPathIndex = 0;
    m_aMembers[2].m_vFollowPoint = vPosition;
    m_aMembers[2].m_nFollowPointIndex = 0;

    for (int i = 0; i < 3; ++i) {
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->ClientToServerObjectID(m_aMembers[i].m_oid));
        if (pCreature) {
            pCreature->m_pPathfindInformation->m_n210 = 0;
            pCreature->m_pPathfindInformation->m_oidBlocker = 0x7F000000;
            if (!g_pAppManager->GetServerExoApp()->TestSafeLocation(pCreature->m_oidArea, pCreature->m_vPosition)) {
                Vector vSafe;
                vSafe.x = 0.0f;
                vSafe.y = 0.0f;
                vSafe.z = 0.0f;
                g_pAppManager->GetServerExoApp()->ComputeSafeLocation(pCreature->m_vPosition, 5.0f, pCreature->m_oidArea, 1, &vSafe);
                pCreature->SetPosition(vSafe, 1, 1, 0);
            }
        }
    }
}
