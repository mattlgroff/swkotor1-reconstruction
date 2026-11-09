// Player-to-server login message.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x005244E0
int CSWSMessage::HandlePlayerToServerLoginMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    g_pAppManager->GetServerExoApp()->GetModule();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nMinor == 2 || nMinor == 0xe || nMinor == 0x11) {
        CResRef cResRef = ReadCResRef(0x10);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        g_pAppManager->GetServerExoApp()->LoadCharacterStart(nMinor, pPlayer, cResRef, NULL, 0);
        return 1;
    } else if (nMinor == 0xf) {
        if (MessageReadUnderflow(1) || MessageReadOverflow(1)) {
            return 0;
        }
        if (pPlayer->m_n80 != -1) {
            g_pAppManager->GetServerExoApp()->LoadCharacterStart(0xf, pPlayer, "", NULL, 0);
            return 1;
        }
        SendServerToPlayerLogin_NeedCharacter(pPlayer->m_nPlayerID);
        return 1;
    } else if (nMinor == 1 || nMinor == 0x13) {
        unsigned long nSize = ReadDWORD(0x20);
        unsigned char* pData = ReadVOIDPtr(nSize);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        g_pAppManager->GetServerExoApp()->LoadCharacterStart(nMinor, pPlayer, "", pData, nSize);
        return 1;
    } else if (nMinor == 0xb) {
        int bArg = ReadBOOL();
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        g_pAppManager->GetServerExoApp()->LoadCharacterFinish(pPlayer, bArg);
        return 1;
    }
    return 0;
}
