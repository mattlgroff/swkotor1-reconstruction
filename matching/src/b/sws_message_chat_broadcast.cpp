// Area-limited chat broadcasts: string-reference and multi-language chat (round s9).
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const float AI_SOUND_RANGE;                           // 0x00747444, 1000.0f
extern const float AI_WHISPER_RANGE;                         // 0x00747448 (name guessed)

static inline Vector VectorDelta(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

// Talk volume 9 goes to every player; 8 (talk) and 10 (whisper) only to
// players in the speaker's area within hearing range.
// NONMATCH (11): load order of this/arguments for the per-player send and GetNext.
// FUNCTION: SWKOTOR 0x00571760
int CSWSMessage::ChatStrRefMessage(unsigned long nTalkVolume, unsigned long oidSpeaker, unsigned long nStrRef)
{
    int bResult = 1;
    CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pPlayers) {
        return 0;
    }
    unsigned char nVolume = (unsigned char)nTalkVolume;
    switch (nVolume) {
    case 9:
        bResult = SendServerToPlayerChat_StrRef(PLAYER_ID_ALL_PLAYERS, oidSpeaker, nVolume, nStrRef) & 1;
        break;
    case 8:
    case 10: {
        void* pos = 0;
        Vector vPosition(0.0f, 0.0f, 0.0f);
        float fRange;
        if (nVolume == 8) {
            fRange = AI_SOUND_RANGE;
        } else {
            fRange = AI_WHISPER_RANGE;
        }
        CSWSArea* pArea;
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSpeaker, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSObject()) {
            pArea = pObject->AsSWSObject()->GetArea();
            vPosition = pObject->AsSWSObject()->m_vPosition;
            pos = pPlayers->GetHeadPos();
        }
        while (pos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pos);
            if (pPlayer->GetGameObject()) {
                Vector vDelta = VectorDelta(pPlayer->GetGameObject()->m_vPosition, vPosition);
                if (pArea == ((CSWSObject*)pPlayer->GetGameObject())->GetArea()) {
                    if (vDelta.z * vDelta.z + vDelta.y * vDelta.y + vDelta.x * vDelta.x <= fRange * fRange) {
                        bResult &= SendServerToPlayerChat_StrRef(pPlayer->m_nPlayerID, oidSpeaker, nVolume, nStrRef);
                    }
                }
            }
            pPlayers->GetNext(pos);
        }
        break;
    }
    default:
        bResult = 0;
        break;
    }
    return bResult;
}

// Talk (8) only: every player in the speaker's area within hearing range, or
// with bRestrictToList only the listed players.
// NONMATCH (16): load order of this/arguments for the helper call.
// FUNCTION: SWKOTOR 0x00571570
int CSWSMessage::SendServerToPlayerChatMultiLangMessage(unsigned char nChatType, unsigned long oidSpeaker,
                                                        CExoLocString& sMessage, unsigned long oidTarget,
                                                        unsigned char nGender, unsigned long* pPlayerIds,
                                                        unsigned long nPlayerIds, int bRestrictToList,
                                                        const CResRef& cSound, unsigned long oidSound,
                                                        int nValue)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (nChatType != 8) {
        return 0;
    }
    CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pPlayers) {
        return 0;
    }
    CGameObject* pObject;
    CSWSArea* pArea;
    int bResult;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSpeaker, &pObject) == GAME_OBJECT_VALID) {
        CSWSObject* pSpeaker = (CSWSObject*)pObject;
        pArea = pSpeaker->GetArea();
        vPosition = pSpeaker->m_vPosition;
        bResult = 1;
    } else {
        bResult = 0;
    }
    void* pos = pPlayers->GetHeadPos();
    while (pos && bResult == 1) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pos);
        if (pPlayer->GetGameObject()) {
            int bListed = 0;
            int bSkip = bRestrictToList;
            for (unsigned long i = 0; i < nPlayerIds; i++) {
                if (pPlayerIds[i] == pPlayer->m_nPlayerID) {
                    bListed = 1;
                    bSkip = 0;
                    break;
                }
            }
            Vector vDelta = VectorDelta(pPlayer->GetGameObject()->m_vPosition, vPosition);
            if (!bSkip && pArea == ((CSWSObject*)pPlayer->GetGameObject())->GetArea()) {
                if (vDelta.z * vDelta.z + vDelta.y * vDelta.y + vDelta.x * vDelta.x <= AI_SOUND_RANGE * AI_SOUND_RANGE) {
                    bResult = SendServerToPlayerChatMultiLang_Helper(pPlayer->m_nPlayerID, 8, oidSpeaker, sMessage,
                                                                     oidTarget, nGender, bListed, cSound, oidSound,
                                                                     nValue);
                }
            }
        }
        pPlayers->GetNext(pos);
    }
    return bResult;
}
