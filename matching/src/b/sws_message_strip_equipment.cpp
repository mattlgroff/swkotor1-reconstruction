// Broadcast the removal of a creature's equipped-item appearance.
#include "b/sws_message.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00570180
int CSWSMessage::SendServerToAllPlayersCreatureUpdate_StripEquippedItems(unsigned long oidCreature)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
    void* pPos = pPlayers->GetHeadPos();
    while (pPos) {
        CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pPos);
        if (pPlayer->GetLastUpdateObject(oidCreature)) {
            CreateWriteMessage(4, 0xffffffff, 1);
            WriteOBJECTIDServer(oidCreature);
            if (GetWriteMessage(&pMessage, &nSize)) {
                SendServerToPlayerMessage(pPlayer->m_nPlayerID, 5, 6, pMessage, nSize);
            }
        }
        pPlayers->GetNext(pPos);
    }
    return 1;
}
