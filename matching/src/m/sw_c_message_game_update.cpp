#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x0066A4A0
int CSWCMessage::HandleServerToPlayerGameObjectUpdate(unsigned char minor)
{
    if (MessageReadOverflow(1)) return 0;
    switch (minor) {
    case 1:
        while (MoreDataToRead()) {
            unsigned char kind = ReadBYTE(8);
            if (MessageReadOverflow(1)) return 0;
            switch (kind) {
            case 'A': HandleServerToPlayerUpdate_Add(); break;
            case 'U': HandleServerToPlayerUpdate_Update(); break;
            case 'D': HandleServerToPlayerUpdate_Delete(); break;
            case 'P': HandleServerToPlayerUpdate_Appearance(); break;
            case 'I': HandleServerToPlayerUpdate_PlayerInfo(); break;
            case 'G': HandleServerToPlayerUpdate_GuiElements(); break;
            case 'W': HandleServerToPlayerUpdate_LoadProgress(); break;
            default: return 0;
            }
        }
        return 1;
    case 2: {
        unsigned long id = ReadOBJECTIDClient();
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
        return HandleServerToPlayerCreatureUpdate_ControlObject(id);
    }
    case 3: return HandleServerToPlayerCreatureUpdate_VisEffect();
    case 6: return HandleServerToPlayerCreatureUpdate_StripEquippedItems();
    case 7: return HandleServerToPlayerItemUpdate_DestroyItem();
    case 8: return HandleServerToPlayerItemScript_Add();
    default: return 0;
    }
}
