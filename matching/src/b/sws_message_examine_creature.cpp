// Creature examine data.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C

// FUNCTION: SWKOTOR 0x0056EBE0
int CSWSMessage::SendServerToPlayerExamineGui_CreatureData(CSWSPlayer* pPlayer, unsigned long oid)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    g_pAppManager->GetServerExoApp()->GetNetLayer()->GetPlayerInfo(nPlayerId);
    CGameObject* pObject = NULL;
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CExoLocString sDescription;
    if (pArray->GetGameObject(oid, &pObject) != GAME_OBJECT_VALID) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    sDescription = pCreature->GetStats()->m_lsDescription;
    nSize = 8;
    pPlayer->GetLastUpdateObject(oid);
    CreateWriteMessage(nSize, nPlayerId, 1);
    WriteOBJECTIDServer(pCreature->GetId());
    WriteCExoLocStringServer(sDescription, 0);
    WriteBOOL(1);
    WriteCExoLocStringServer(*pCreature->GetFirstName(), 0);
    WriteCExoLocStringServer(*pCreature->GetLastName(), 0);
    unsigned short nPortraitId = pCreature->GetPortraitId();
    WriteWORD(nPortraitId, 0x10);
    if (nPortraitId >= 0xfffe) {
        WriteCResRef(pCreature->GetStats()->m_cPortrait, 0x10);
    }
    WriteBYTE(pCreature->GetDamageLevel(), 8);
    WriteBYTE(pPlayer->GetGameObject()->AsSWSCreature()->GetAIStateReputation(pCreature->GetId()), 8);
    float fLevel;
    CSWSCreatureStats* pStats = pCreature->GetStats();
    if (pStats->m_bIsPC) {
        fLevel = (float)pStats->GetLevel(0);
    } else {
        fLevel = pStats->m_fChallengeRating;
    }
    WriteFLOAT(fLevel, 1.0f, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x1b, 3, pMessage, nSize);
}

