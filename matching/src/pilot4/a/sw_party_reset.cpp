// Pilot 4 arm A: CSWParty::Reset.
// NONMATCH (68 diff insns): stack layout and instruction shape match; register
// allocation differs (original: this=ebp, pPosition=esi, server=ebx, creature=edi;
// ours: this=esi, pPosition=ebx, server=ebp) plus x87 scheduling in the unary minus.
#include "pilot4/a/p4a_party.h"

// FUNCTION: SWKOTOR 0x00637630
void CSWParty::Reset(Vector* pPosition, float fYaw, int bFormation)
{
    m_cFollow.Reset(pPosition, fYaw);
    if (bFormation) {
        Quaternion q(g_vUp, fYaw * 0.017453292f);
        Vector vOffset(0.0f, 1.0f, 0.0f);
        vOffset = -(q * vOffset);
        vOffset *= 1.5f;
        Vector vOut(0.0f, 0.0f, 0.0f);
        CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
        unsigned long oid = pServer->ClientToServerObjectID(m_oidLeader);
        CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(oid);
        if (pCreature) {
            if (pServer->GetAreaByGameObjectID(pCreature->m_oidArea)
                    ->ComputeSafeLocation(vOffset + *pPosition, 2.0f, pCreature->m_n340, 1, &vOut)) {
                m_vMember1 = vOut;
                m_nMember1E0 = 0;
                m_cFollow.ProcessPoint(&vOut, fYaw, 0.5f);
            }
            vOffset += vOffset;
            if (pServer->GetAreaByGameObjectID(pCreature->m_oidArea)
                    ->ComputeSafeLocation(vOffset + *pPosition, 2.0f, pCreature->m_n340, 1, &vOut)) {
                m_vMember2 = vOut;
                m_nMember2168 = 0;
                m_cFollow.ProcessPoint(&vOut, fYaw, 0.5f);
            }
        }
        m_cFollow.ProcessPoint(pPosition, fYaw, 0.5f);
    }
}
