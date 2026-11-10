// CSWSMessage::TestPartyUpdateDifferences (round s13): finds or creates the
// party last-update entry of a creature and sets the update flags for the
// values that changed since the last update.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include <stdlib.h>

// The player's +0x10 list holds party entries (declared with the object
// entry type in the shared header).
inline CExoLinkedList<CLastUpdatePartyObject>* GetPartyUpdateList(CSWSPlayer* pPlayer)
{
    return (CExoLinkedList<CLastUpdatePartyObject>*)pPlayer->m_pList10;
}

inline float DistanceSquared(const Vector& a, const Vector& b)
{
    float fDX = a.x - b.x;
    float fDY = a.y - b.y;
    float fDZ = a.z - b.z;
    return fDX * fDX + fDY * fDY + fDZ * fDZ;
}

// The original advances the party list position with the +0x0c object
// list's GetNext (same node walk, different list object).
// NONMATCH (6): the original stores *pFlags = 0 after loading the list head
// and builds &pos before loading the +0x0c list for GetNext.
// FUNCTION: SWKOTOR 0x0056E850
void CSWSMessage::TestPartyUpdateDifferences(CSWSPlayer* pPlayer, CSWSCreature* pCreature,
                                             CLastUpdatePartyObject** ppLast,
                                             unsigned long* pFlags)
{
    CSWSCreature* pPlayerCreature = pPlayer->GetGameObject()->AsSWSCreature();
    *pFlags = 0;
    void* pos = GetPartyUpdateList(pPlayer)->GetHeadPos();
    while (pos) {
        *ppLast = GetPartyUpdateList(pPlayer)->GetAtPos(pos);
        if ((*ppLast)->m_oidCreature == pCreature->GetId()) {
            (*ppLast)->m_bUpdated = 1;
            *pFlags = 0;
            pCreature->Fn4F1460();
            unsigned char nReputation;
            if (pPlayerCreature) {
                nReputation = pPlayerCreature->GetAIStateReputation(pCreature->GetId());
            }
            else {
                nReputation = 2;
            }
            if ((*ppLast)->m_nPartyState != pCreature->m_nPartyState ||
                (*ppLast)->m_n1E != pCreature->m_n9F2 || (*ppLast)->m_n20 != pCreature->m_n9FC ||
                (*ppLast)->m_n24 != pCreature->m_n9F4 || (*ppLast)->m_nReputation != nReputation ||
                (*ppLast)->m_n34 != pCreature->m_n4E0 ||
                (*ppLast)->m_bPlayerCharacter != pCreature->m_bPlayerCharacter) {
                *pFlags |= 0x40;
            }
            if (pCreature->m_oidArea != (*ppLast)->m_oidArea) {
                *pFlags |= 0x40200;
            }
            if (pCreature->m_oidArea == pPlayerCreature->m_oidArea && abs(1) + abs(0) <= 3) {
                if (DistanceSquared((*ppLast)->m_vPosition, pCreature->m_vPosition) >= 4.0f) {
                    *pFlags |= 0x200;
                }
            }
            if ((*ppLast)->m_nCurrentHitPoints != pCreature->GetCurrentHitPoints(1) ||
                (*ppLast)->m_n2A != pCreature->m_nE0 || (*ppLast)->m_n2C != pCreature->m_nE4 ||
                (*ppLast)->m_nMaxHitPoints != pCreature->GetCreatureSize(1)) {
                *pFlags |= 0x400;
            }
            if ((*ppLast)->m_nForcePointsBonus != pCreature->m_pStats->m_nForcePointsBonus ||
                (*ppLast)->m_nMaxForcePointsBase != pCreature->m_pStats->m_nMaxForcePointsBase ||
                (*ppLast)->m_nCurrentForcePoints != pCreature->m_pStats->m_nCurrentForcePoints ||
                (*ppLast)->m_nMaxForcePoints != pCreature->GetMaxForcePoints()) {
                *pFlags |= 0x80000;
            }
            unsigned long oidCreature;
            if (pCreature->AsSWSCreature()->m_bIsPC != (*ppLast)->m_bIsPC ||
                (((unsigned char)pCreature->AsSWSCreature()->m_nPartyState & 0x40) == 0x40) !=
                    (*ppLast)->m_b50 ||
                (oidCreature = pCreature->m_idSelf,
                 (pCreature->AsSWSCreature()->GetFaction()->GetLeader() == oidCreature) !=
                     (*ppLast)->m_bLeader) ||
                pCreature->GetFaction()->GetSingletonParty() != (*ppLast)->m_bSingletonParty ||
                pCreature->AsSWSCreature()->GetN930() != (*ppLast)->m_n5C ||
                pCreature->m_bPoisoned != (*ppLast)->m_bPoisoned ||
                pCreature->m_n9E4 != (*ppLast)->m_n64) {
                *pFlags |= 0x4000;
            }
            if ((*ppLast)->m_nPortraitId != pCreature->GetPortraitId() ||
                (*ppLast)->m_cPortrait != pCreature->GetPortrait()) {
                *pFlags |= 0x20;
            }
            return;
        }
        pPlayer->m_pLastUpdateObjects->m_pList->GetNext(pos);
    }
    *ppLast = new CLastUpdatePartyObject;
    (*ppLast)->m_bUpdated = 1;
    (*ppLast)->m_oidCreature = pCreature->m_idSelf;
    *pFlags = 0x56660;
    GetPartyUpdateList(pPlayer)->AddHead(*ppLast);
}
