// Pilot 4 arm A: CSWCMessage::HandlePlayerToServerShutDownServer.
#include "pilot4/a/p4a_message.h"

// FUNCTION: SWKOTOR 0x00654970
int CSWCMessage::HandlePlayerToServerShutDownServer(unsigned char nMinor)
{
    STR_RES sUnused;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    ReadDWORD(32);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}
