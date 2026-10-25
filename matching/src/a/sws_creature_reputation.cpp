// CSWSCreature faction reputation helpers.
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x00513FF0
int CSWSCreature::GetCreatureReputation(unsigned long oidCreature, int nFaction)
{
    int nOtherFaction = nFaction;
    int nOwnFaction = m_pStats->m_nFactionId;
    CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (m_bA88 && pCreature && pCreature->m_bA88) {
        return 100;
    }
    if (m_bA88) {
        nOwnFaction = 0;
    }
    if (pCreature && pCreature->m_bA88) {
        nOtherFaction = 0;
    }
    CFactionManager* pFactionManager = GetServerExoApp()->m_pInternal->m_pFactionManager;
    return pFactionManager->GetNPCReputation(nOtherFaction, nOwnFaction);
}

// FUNCTION: SWKOTOR 0x00514070
void CSWSCreature::AdjustReputation(int nFaction, int nDelta)
{
    int nFactionId = nFaction;
    CFactionManager* pFactionManager = GetServerExoApp()->m_pInternal->m_pFactionManager;
    if (!pFactionManager->IsValidFactionId(nFactionId)) {
        return;
    }
    int nOwnFaction = m_pStats->m_nFactionId;
    int nReputation = pFactionManager->GetNPCReputation(nOwnFaction, nFactionId);
    if (m_pStats->m_nFactionId != nFactionId) {
        pFactionManager->SetNPCReputation(m_pStats->m_nFactionId, nFactionId, nReputation + nDelta);
    }
}
