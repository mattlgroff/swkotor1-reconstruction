// Party member values cached in the player's party last-update entry.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00567CB0
void CSWSMessage::StoreValuesInLastUpdatePartyObject(CSWSCreature* pPlayerCreature,
                                                     CLastUpdatePartyObject* pLast,
                                                     CSWSCreature* pCreature,
                                                     unsigned long nFlags)
{
    if (nFlags & 0x40) {
        pLast->m_nPartyState = pCreature->m_nPartyState;
        pLast->m_n1E = pCreature->m_n9F2;
        pLast->m_n20 = pCreature->m_n9FC;
        pLast->m_n24 = pCreature->m_n9F4;
        pLast->m_nReputation = pPlayerCreature->GetAIStateReputation(pCreature->GetId());
        pLast->m_n34 = pCreature->m_n4E0;
        pLast->m_bPlayerCharacter = pCreature->m_bPlayerCharacter;
    }
    if (nFlags & 0x40000) {
        pLast->m_oidArea = pCreature->m_oidArea;
    }
    if (nFlags & 0x200) {
        pLast->m_fX = pCreature->m_vPosition.x;
        pLast->m_fY = pCreature->m_vPosition.y;
    }
    if (nFlags & 0x400) {
        pLast->m_n2A = pCreature->m_nE0;
        pLast->m_nCurrentHitPoints = pCreature->GetCurrentHitPoints(1);
        pLast->m_n2C = pCreature->m_nE4;
        pLast->m_nMaxHitPoints = pCreature->GetCreatureSize(1);
    }
    if (nFlags & 0x80000) {
        pLast->m_nMaxForcePointsBase = pCreature->m_pStats->m_nMaxForcePointsBase;
        pLast->m_nForcePointsBonus = pCreature->m_pStats->m_nForcePointsBonus;
        pLast->m_nCurrentForcePoints = pCreature->m_pStats->m_nCurrentForcePoints;
        pLast->m_nMaxForcePoints = pCreature->GetMaxForcePoints();
    }
    if (nFlags & 0x4000) {
        pLast->m_bIsPC = pCreature->m_bIsPC;
        pLast->m_b50 = ((unsigned char)pCreature->m_nPartyState & 0x40) == 0x40;
        pLast->m_bSingletonParty = pCreature->GetFaction()->GetSingletonParty();
        unsigned long oidCreature = pCreature->GetId();
        pLast->m_bLeader = pCreature->GetFaction()->GetLeader() == oidCreature;
        pLast->m_n5C = pCreature->m_n930;
        pLast->m_n64 = pCreature->m_n9E4;
        pLast->m_bPoisoned = pCreature->m_bPoisoned;
    }
    if (nFlags & 0x20) {
        pLast->m_nPortraitId = pCreature->GetPortraitId();
        pLast->m_cPortrait = pCreature->GetPortrait();
    }
}
