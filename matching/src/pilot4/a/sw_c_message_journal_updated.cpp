// Pilot 4 arm A: CSWCMessage::HandleServerToPlayerJournalMessage_Updated.
#include "pilot4/a/p4a_message.h"

// FUNCTION: SWKOTOR 0x006526E0
int CSWCMessage::HandleServerToPlayerJournalUpdated()
{
    g_pAppManager->m_pClientExoApp->GetSomething();
    CExoString sText;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    sText = ReadCExoLocStringClient(CResRef(), 0);
    int a = ReadBOOL();
    int b = ReadBOOL();
    g_pAppManager->GetClientExoApp()->JournalUpdated(a, b, sText);
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}
