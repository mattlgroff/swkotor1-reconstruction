// CSWSMessage::DeleteLastUpdateObjectsForObject (round s9): write the delete
// record for one object and drop its last update object.
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sws_creature.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634 (5)
extern const unsigned char OBJECT_TYPE_ITEM;                // 0x00746635 (6)
extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636 (7)
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638 (9)

// FUNCTION: SWKOTOR 0x0056E190
void CSWSMessage::DeleteLastUpdateObjectsForObject(CSWSPlayer* pPlayer, unsigned long oidObject)
{
    CGameObject* pPlayerObject = pPlayer->GetGameObject();
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (!pObjectArray) {
        return;
    }
    void* pos = pPlayer->m_pLastUpdateObjects->GetHeadPos();
    while (pos) {
        CLastUpdateObject* pLastUpdate = pPlayer->m_pLastUpdateObjects->GetAtPos(pos);
        if (pLastUpdate->m_nId == oidObject && pLastUpdate->m_nId != pPlayerObject->m_idSelf) {
            WriteBYTE(0x44, 8);
            WriteBYTE(pLastUpdate->m_nObjectType, 8);
            WriteOBJECTIDServer(pLastUpdate->m_nId);
            if (pLastUpdate->m_nObjectType == OBJECT_TYPE_ITEM || pLastUpdate->m_nObjectType == OBJECT_TYPE_CREATURE ||
                pLastUpdate->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                int bOtherArea = 1;
                CGameObject* pObject;
                if (pObjectArray->GetGameObject(pLastUpdate->m_nId, &pObject) == GAME_OBJECT_VALID &&
                    pObject->AsSWSObject()) {
                    if (pObject->AsSWSObject()->GetArea() == ((CSWSObject*)pPlayer->GetGameObject())->GetArea()) {
                        bOtherArea = 0;
                    }
                }
                WriteBOOL(bOtherArea);
                if (pLastUpdate->m_nObjectType == OBJECT_TYPE_CREATURE || pLastUpdate->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                    WriteBOOL(pLastUpdate->m_b148);
                    WriteDWORD(pLastUpdate->m_n150, 0x20);
                    if (pLastUpdate->m_nObjectType == OBJECT_TYPE_CREATURE) {
                        WriteBOOL(pLastUpdate->m_b14C);
                    }
                }
            } else if (pLastUpdate->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                WriteBYTE(3, 8);
            }
            void* posRemove = pos;
            pPlayer->m_pLastUpdateObjects->GetNext(pos);
            pPlayer->m_pLastUpdateObjects->Remove(posRemove);
            delete pLastUpdate;
            return;
        }
        pLastUpdate->m_bUpdated = 0;
        pPlayer->m_pLastUpdateObjects->GetNext(pos);
    }
}

// Drop the last update objects of objects that are no longer in the
// player's area (an object in an area the player's creature just left is
// kept while the creature is between areas).
// NONMATCH (17): block placement only; the original emits the shared "keep"
// block after the delete record (if/else, continue and loop forms tried).
// FUNCTION: SWKOTOR 0x0056DF60
void CSWSMessage::DeleteLastUpdateObjectsInOtherAreas(CSWSPlayer* pPlayer)
{
    CGameObject* pPlayerObject = pPlayer->GetGameObject();
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (!pObjectArray) {
        return;
    }
    void* pos = pPlayer->m_pLastUpdateObjects->GetHeadPos();
    while (pos) {
        CLastUpdateObject* pLastUpdate = pPlayer->m_pLastUpdateObjects->GetAtPos(pos);
        if (pPlayerObject && pLastUpdate->m_nId == pPlayerObject->m_idSelf) {
            pLastUpdate->m_bUpdated = 0;
            pPlayer->m_pLastUpdateObjects->GetNext(pos);
            continue;
        }
        CGameObject* pObject;
        if (pObjectArray->GetGameObject(pLastUpdate->m_nId, &pObject) == GAME_OBJECT_VALID && pObject->AsSWSObject() &&
            pPlayerObject &&
            (pObject->AsSWSObject()->GetArea() == ((CSWSObject*)pPlayerObject)->GetArea() ||
             (pObject->AsSWSObject()->GetArea() && !((CSWSObject*)pPlayerObject)->GetArea() &&
              pPlayerObject->AsSWSCreature() &&
              pObject->AsSWSObject()->GetArea()->GetId() == pPlayerObject->AsSWSCreature()->m_oidLastArea))) {
            pPlayer->m_pLastUpdateObjects->GetNext(pos);
            continue;
        }
        WriteBYTE(0x44, 8);
        WriteBYTE(pLastUpdate->m_nObjectType, 8);
        WriteOBJECTIDServer(pLastUpdate->m_nId);
        if (pLastUpdate->m_nObjectType == OBJECT_TYPE_CREATURE || pLastUpdate->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            WriteBOOL(1);
            WriteBOOL(pLastUpdate->m_b148);
            WriteDWORD(pLastUpdate->m_n150, 0x20);
            if (pLastUpdate->m_nObjectType == OBJECT_TYPE_CREATURE) {
                WriteBOOL(pLastUpdate->m_b14C);
            }
        } else if (pLastUpdate->m_nObjectType == OBJECT_TYPE_ITEM) {
            WriteBOOL(1);
        } else if (pLastUpdate->m_nObjectType == OBJECT_TYPE_TRIGGER) {
            WriteBYTE((unsigned char)pLastUpdate->m_n150, 8);
        }
        void* posRemove = pos;
        pPlayer->m_pLastUpdateObjects->GetNext(pos);
        pPlayer->m_pLastUpdateObjects->Remove(posRemove);
        delete pLastUpdate;
    }
}
