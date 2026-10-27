// Deliver quick chat only to players in the same area and hearing range.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const float AI_SOUND_RANGE;                           // 0x00747444, 1000.0f

static inline Vector VectorDelta(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

// NONMATCH (24): esi/edi swap (bResult/pPlayer) and the GetNext this load order.
// FUNCTION: SWKOTOR 0x00571EC0
int CSWSMessage::SendServerToPlayerQuickChatMessage(unsigned long oidSpeaker, unsigned short nQuickChat)
{
    int bResult = 1;
    CSWSArea* pArea = 0;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pPlayers) return 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSpeaker, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        pArea = pObject->AsSWSCreature()->GetArea();
        vPosition = pObject->AsSWSCreature()->m_vPosition;
    } else {
        bResult = 0;
    }
    void* pos = pPlayers->GetHeadPos();
    while (pos && bResult == 1) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pos);
        if (pPlayer->GetGameObject()) {
            CGameObject* pPlayerObject = pPlayer->GetGameObject();
            Vector vDelta = VectorDelta(pPlayerObject->m_vPosition, vPosition);
            if (pArea == ((CSWSObject*)pPlayer->GetGameObject())->GetArea()) {
                if (vDelta.z*vDelta.z + vDelta.y*vDelta.y + vDelta.x*vDelta.x <= AI_SOUND_RANGE * AI_SOUND_RANGE) {
                    bResult = SendServerToPlayerQuickChat(pPlayer->m_nPlayerID, oidSpeaker, nQuickChat) & 1;
                }
            }
        }
        pPlayers->GetNext(pos);
    }
    return bResult;
}
