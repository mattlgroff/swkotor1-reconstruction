#include "m/sw_c_message_handlers.h"

// NONMATCH: shared-case call scheduling differs; retained second source shape (17 diff insns).
// FUNCTION: SWKOTOR 0x00652B00
int CSWCMessage::HandleServerToPlayerGuiQuickbar_SetButton(unsigned char slot, int all)
{
    unsigned char end;
    unsigned char start;
    if (all) { start = 0; end = 0x24; }
    else { start = slot; end = slot + 1; }
    for (unsigned char index = start; index < end; ++index) {
        switch (ReadBYTE(8)) {
        case 2:
            ReadBYTE(8); ReadDWORD(0x20); ReadBYTE(8); ReadBYTE(8);
            break;
        case 0x2c:
            ReadDWORD(0x20); ReadBYTE(8);
            break;
        case 0x12: {
            CExoString first, second;
            first = ReadCExoString(0x20);
            second = ReadCExoString(0x20);
            if (MessageReadOverflow(1)) return 0;
            continue;
        }
        case 3: case 4: case 8: case 0xa: case 0x25: case 0x2a:
            ReadDWORD(0x20);
            break;
        default: continue;
        }
        if (MessageReadOverflow(1)) return 0;
    }
    return 1;
}
