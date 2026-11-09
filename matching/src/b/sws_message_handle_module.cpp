// Player-to-server module message: module start, save, load and run.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00524800
int CSWSMessage::HandlePlayerToServerModuleMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    CExoString sName;
    CExoString sModule;
    CExoString sSaveName;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    int bResult = 1;
    if (nMinor == 2) {
        if (pPlayer->m_n24 == 1 && g_pAppManager->GetServerExoApp()->GetServerMode() == 2) {
            pPlayer->m_n7C = 1;
            g_pAppManager->GetServerExoApp()->InitiateModuleForPlayer(pPlayer);
            bResult &= SendServerToPlayerPlayerList_Add(PLAYER_ID_ALL_PLAYERS, pPlayer);
            bResult &= SendServerToPlayerPlayerList_All(pPlayer);
        }
        return bResult;
    } else if (nMinor == 4) {
        unsigned long nSlot = ReadDWORD(0x20);
        sName = ReadCExoString(0x20);
        sSaveName = ReadCExoString(0x20);
        g_pAppManager->GetServerExoApp()->SetSaveGameName(sSaveName);
        if (!MessageReadUnderflow(1) && pPlayer->GetIsSaveAllowed()) {
            return g_pAppManager->GetServerExoApp()->SaveGame(nSlot, sName, pPlayer, 0);
        }
        return 0;
    } else if (nMinor == 5) {
        unsigned long nSlot = ReadDWORD(0x20);
        sName = ReadCExoString(0x20);
        sModule = ReadCExoString(0x20);
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        if (!pPlayer->GetIsSaveAllowed()) {
            return 0;
        }
        if (g_pAppManager->GetServerExoApp()->GetLoadingModule() == 1) {
            return 0;
        }
        g_pAppManager->GetServerExoApp()->SetLoadingModule(1);
        SendServerToPlayerModule_Loading(pPlayer);
        return g_pAppManager->GetServerExoApp()->LoadGame(nSlot, sName, sModule, pPlayer);
    } else if (nMinor == 7) {
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        g_pAppManager->GetServerExoApp()->RunModule();
        return 1;
    }
    return 0;
}
