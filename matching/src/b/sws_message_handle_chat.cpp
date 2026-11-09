// Player-to-server chat message.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00520A10
int CSWSMessage::HandlePlayerToServerChatMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    CExoString sText;
    CExoString sName;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    unsigned long oidTarget;
    if (nMinor == 4) {
        if (!ReadBOOL()) {
            oidTarget = ReadDWORD(0x20);
        } else {
            sName = ReadCExoString(0x20);
            oidTarget = g_pAppManager->GetServerExoApp()->ResolvePlayerByFirstName(sName);
        }
    } else {
        oidTarget = 0xffffffff;
    }
    sText = ReadCExoString(0x20);
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject) {
            unsigned long oidSpeaker = pObject->m_idSelf;
            int nResult;
            switch (nMinor) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 6:
            case 14:
                nResult = SendServerToPlayerChatMessage(nMinor, oidSpeaker, sText, oidTarget, sName);
                break;
            default:
                nResult = 0;
                break;
            }
            return nResult;
        }
    }
    return 0;
}
