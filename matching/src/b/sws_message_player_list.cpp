// Player list records.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

class CNetLayerPlayerInfo {
public:
    int m_bPlayerInUse;                                     // 0x0
    CExoString m_sPlayerName;                               // 0x4
};

// FUNCTION: SWKOTOR 0x00568030
int CSWSMessage::SendServerToPlayerPlayerList_Add(unsigned long nPlayerId, CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nAddId = pPlayer->m_nPlayerID;
    unsigned long oidObject = pPlayer->GetGameObject()->m_idSelf;
    CExoString sPlayerName(
        g_pAppManager->GetServerExoApp()->GetNetLayer()->GetPlayerInfo(nAddId)->m_sPlayerName);
    unsigned long oidCharacter = INVALID_OBJECT_ID;
    CSWSCreature* pPlayerCreature = pPlayer->GetGameObject()->AsSWSCreature();
    if (pPlayerCreature && pPlayerCreature->GetStats()->m_bIsPC == 1) {
        oidCharacter = pPlayerCreature->GetId();
    }
    CExoLocString sFirstName;
    CExoLocString sLastName;
    CResRef cPortrait;
    int bHasCharacter = 0;
    CSWSCreature* pCreature =
        g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCharacter);
    if (pCreature) {
        bHasCharacter = 1;
        sFirstName = pCreature->GetStats()->m_lsFirstName;
        sLastName = pCreature->GetStats()->m_lsLastName;
        cPortrait = pCreature->GetStats()->GetPortrait();
    }
    CreateWriteMessage(0x100, 0xffffffff, 1);
    WriteDWORD(nAddId, 0x20);
    WriteOBJECTIDServer(oidObject);
    WriteCExoString(sPlayerName, 0x20);
    WriteBOOL(bHasCharacter);
    if (bHasCharacter == 1) {
        WriteOBJECTIDServer(oidCharacter);
        WriteCExoLocStringServer(sFirstName, 0);
        WriteCExoLocStringServer(sLastName, 0);
        unsigned short nPortraitId = pCreature->GetPortraitId();
        WriteWORD(nPortraitId, 0x10);
        if (nPortraitId >= 0xfffe) {
            WriteCResRef(cPortrait, 0x10);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xa, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FE40
int CSWSMessage::SendServerToPlayerPlayerList_All(CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x200, 0xffffffff, 1);
    CExoLinkedList<CSWSClient>* pPlayerList = g_pAppManager->GetServerExoApp()->GetPlayerList();
    if (!pPlayerList) {
        return 0;
    }
    unsigned char nPlayers = 0;
    void* pos = pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pOther = (CSWSPlayer*)pPlayerList->GetAtPos(pos);
        if (pOther->GetGameObject()) {
            nPlayers++;
        }
        pPlayerList->GetNext(pos);
    }
    WriteBYTE(nPlayers, 8);
    pos = pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pOther = (CSWSPlayer*)pPlayerList->GetAtPos(pos);
        unsigned long nAddId = pOther->m_nPlayerID;
        CGameObject* pObject = pOther->GetGameObject();
        if (pObject) {
            unsigned long oidObject = pObject->m_idSelf;
            CExoString sPlayerName(
                g_pAppManager->GetServerExoApp()->GetNetLayer()->GetPlayerInfo(nAddId)->m_sPlayerName);
            unsigned long oidCharacter = INVALID_OBJECT_ID;
            CSWSCreature* pPlayerCreature = pOther->GetGameObject()->AsSWSCreature();
            if (pPlayerCreature && pPlayerCreature->GetStats()->m_bIsPC == 1) {
                oidCharacter = pPlayerCreature->GetId();
            }
            CExoLocString sFirstName;
            CExoLocString sLastName;
            CResRef cPortrait;
            int bHasCharacter = 0;
            CSWSCreature* pCreature =
                g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCharacter);
            if (pCreature) {
                bHasCharacter = 1;
                sFirstName = pCreature->GetStats()->m_lsFirstName;
                sLastName = pCreature->GetStats()->m_lsLastName;
                cPortrait = pCreature->GetStats()->GetPortrait();
            }
            WriteDWORD(nAddId, 0x20);
            WriteOBJECTIDServer(oidObject);
            WriteCExoString(sPlayerName, 0x20);
            WriteBOOL(bHasCharacter);
            if (bHasCharacter == 1) {
                WriteOBJECTIDServer(oidCharacter);
                WriteCExoLocStringServer(sFirstName, 0);
                WriteCExoLocStringServer(sLastName, 0);
                unsigned short nPortraitId = pCreature->GetPortraitId();
                WriteWORD(nPortraitId, 0x10);
                if (nPortraitId >= 0xfffe) {
                    WriteCResRef(cPortrait, 0x10);
                }
            }
        }
        pPlayerList->GetNext(pos);
    }
    if (nPlayers != 0 && GetWriteMessage(&pMessage, &nSize)) {
        unsigned long nPlayerId = pPlayer->m_nPlayerID;
        return SendServerToPlayerMessage(nPlayerId, 0xa, 1, pMessage, nSize);
    }
    return 0;
}
