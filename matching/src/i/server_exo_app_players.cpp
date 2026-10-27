// CServerExoAppInternal::PlayerListChange: keeps the player and system
// administrator lists in step with net-layer connections.
#include "i/server_exo_app.h"

class CObjectTableManager {
public:
    int RemovePlayer(unsigned long nPlayerId);                      // 0x004AE650
};

class CAppManager {
public:
    CObjectTableManager* GetObjectTableManager(int nTable);         // 0x00401060
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC

extern const unsigned long PLAYER_ID_ALL_PLAYERS;                   // 0x0074C52C
extern const unsigned long PLAYER_ID_SERVER_ADMIN;                  // 0x0074C534

// NONMATCH: the original has an aligned (and esp, -8) frame with both
// player-name copies in separate slots; same calls and flow otherwise.
// FUNCTION: SWKOTOR 0x004B78F0
void CServerExoAppInternal::PlayerListChange(unsigned long nPlayerId, int bEnter, int bUnused)
{
    CExoString sTimestamp;
    g_pExoBase->m_pcExoDebug->GetCurrentTimestamp(sTimestamp);

    if (bEnter == 1) {
        CExoString sName = m_pNetLayer->GetPlayerInfo(nPlayerId)->m_sPlayerName;
        sName.CStr();
        if (m_pNetLayer->GetPlayerInfo(nPlayerId)->m_bServerAdminPrivileges == 1) {
            CSWSSysAdmin* pAdmin = new CSWSSysAdmin(nPlayerId);
            pAdmin->m_nLanguage = 0;
            m_pSysAdminList->AddHead((CSWSPlayer*)pAdmin);
            sTimestamp.CStr();
            sName.CStr();
        }
        if (m_pNetLayer->GetPlayerInfo(nPlayerId)->m_bGameMasterPrivileges == 1) {
            CSWSPlayer* pPlayer = new CSWSPlayer(nPlayerId);
            pPlayer->m_nLanguage = 0;
            m_pPlayerList->AddHead(pPlayer);
            sTimestamp.CStr();
            sName.CStr();
        }
        m_pServerMessage->SendServerToServerAdminPlayerList(PLAYER_ID_SERVER_ADMIN);
    } else if (bEnter == 0) {
        CExoString sName = m_pNetLayer->GetPlayerInfo(nPlayerId)->m_sPlayerName;
        int bPrimaryLeft = 0;
        if (m_pNetLayer->GetPlayerInfo(nPlayerId)->m_bServerAdminPrivileges == 1) {
            CExoLinkedListPosition pos = m_pSysAdminList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pAdmin = m_pSysAdminList->GetAtPos(pos);
                if (pAdmin->m_nPlayerID == nPlayerId) {
                    m_pSysAdminList->Remove(pos);
                    delete pAdmin;
                    break;
                }
                m_pSysAdminList->GetNext(pos);
            }
            sTimestamp.CStr();
            sName.CStr();
        }
        if (m_pNetLayer->GetPlayerInfo(nPlayerId)->m_bGameMasterPrivileges == 1) {
            CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
            while (pos) {
                CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
                if (pPlayer->m_nPlayerID == nPlayerId) {
                    if (m_posPC == pos) {
                        m_pPlayerList->GetNext(m_posPC);
                    }
                    m_pPlayerList->Remove(pos);
                    m_pServerMessage->SendServerToPlayerPlayerList_Delete(PLAYER_ID_ALL_PLAYERS,
                                                                          pPlayer);
                    bPrimaryLeft = pPlayer->m_bIsPrimaryPlayer;
                    delete pPlayer;
                    break;
                }
                m_pPlayerList->GetNext(pos);
            }
            g_pAppManager->GetObjectTableManager(1)->RemovePlayer(nPlayerId);
            sTimestamp.CStr();
            sName.CStr();
            if (bPrimaryLeft) {
                Shutdown(1, 0);
            }
        }
        if (m_lShutdownPlayers.Contains(nPlayerId)) {
            m_lShutdownPlayers.Remove(nPlayerId);
            if (m_lShutdownPlayers.num == 0) {
                m_nShutdownRemaining = 1;
            }
        }
    }
}
