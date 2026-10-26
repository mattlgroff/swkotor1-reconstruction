#include "m/sw_c_message_handlers.h"
class CTlkTable { public: void SetCustomToken(int, const CExoString&); };
extern CTlkTable* g_pTlkTable;

// FUNCTION: SWKOTOR 0x0064BF30
int CSWCMessage::HandleServerToPlayerSetCustomToken(unsigned char minor)
{
    CExoString value;
    if (MessageReadOverflow(1)) return 0;
    if (minor == 1) {
        unsigned long number = ReadDWORD(0x20);
        value = ReadCExoString(0x20);
        g_pTlkTable->SetCustomToken(number, value);
    } else if (minor == 2) {
        unsigned long count = ReadDWORD(0x20);
        for (unsigned long left = count; left > 0; --left) {
            unsigned long number = ReadDWORD(0x20);
            value = ReadCExoString(0x20);
            g_pTlkTable->SetCustomToken(number, value);
        }
    } else return 0;
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
    return 1;
}
