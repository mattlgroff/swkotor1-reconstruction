// Per-tick party member update for one player.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C

// The player's +0x10 list holds party entries (declared with the object
// entry type in the shared header).
inline CExoLinkedList<CLastUpdatePartyObject>* GetPartyUpdateList(CSWSPlayer* pPlayer)
{
    return (CExoLinkedList<CLastUpdatePartyObject>*)pPlayer->m_pList10;
}

// NONMATCH (31): `this` (ebp) and the player object (ebx) are swapped, and
// the original loads the flags straight into ebx. Flag copy, declaration
// order, loop form and accessor variants all leave the same allocation.
// FUNCTION: SWKOTOR 0x005779F0
int CSWSMessage::WriteGameObjUpdate_PartyAIState(CSWSPlayer* pPlayer)
{
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CGameObject* pPlayerObject = pPlayer->GetGameObject();
    CSWSCreature* pPlayerCreature = pPlayerObject->AsSWSCreature();
    CSWSFaction* pFaction = pPlayerCreature->GetFaction();
    if (pFaction) {
        unsigned long* pMembers;
        unsigned long nMembers = pFaction->GetMemberList(&pMembers);
        for (unsigned long i = 0; i < nMembers; i++) {
            unsigned long oid = pMembers[i];
            if (oid != pPlayerObject->m_idSelf) {
                CGameObject* pObject;
                if (pObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
                    CSWSCreature* pCreature = pObject->AsSWSCreature();
                    if (pCreature->m_n39C != 1) {
                        CLastUpdatePartyObject* pLast = NULL;
                        unsigned long nFlags;
                        TestPartyUpdateDifferences(pPlayer, pCreature, &pLast, &nFlags);
                        unsigned long nUpdate = nFlags;
                        if (nUpdate) {
                            WriteGameObjUpdate_UpdateObject(pPlayer, pCreature, NULL, nUpdate, 0);
                        }
                        StoreValuesInLastUpdatePartyObject(pPlayerCreature, pLast, pCreature, nUpdate);
                    }
                }
            }
        }
    }
    void* pos = GetPartyUpdateList(pPlayer)->GetHeadPos();
    while (pos) {
        CLastUpdatePartyObject* pLast = GetPartyUpdateList(pPlayer)->GetAtPos(pos);
        if (pLast->m_bUpdated) {
            pLast->m_bUpdated = 0;
            GetPartyUpdateList(pPlayer)->GetNext(pos);
        } else {
            void* posRemove = pos;
            GetPartyUpdateList(pPlayer)->GetNext(pos);
            GetPartyUpdateList(pPlayer)->Remove(posRemove);
            delete pLast;
        }
    }
    return 1;
}
