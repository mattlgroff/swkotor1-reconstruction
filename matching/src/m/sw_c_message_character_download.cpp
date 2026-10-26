#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x00654700
int CSWCMessage::HandleServerToPlayerCharacterDownload(unsigned char minor)
{
    CExoString name, first, second;
    if (MessageReadOverflow(1)) return 0;
    switch (minor) {
    case 2: {
        first = ReadCExoLocStringClient(CResRef(), 0);
        second = ReadCExoLocStringClient(CResRef(), 0);
        unsigned long size = ReadDWORD(0x20);
        unsigned char* data = ReadVOIDPtr(size);
        g_pAppManager->m_pClientExoApp->SetCharacterFile(size, (const char*)data, 0);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
        break;
    }
    case 3: {
        name = ReadCExoString(0x20);
        unsigned long flag = ReadBOOL();
        CClientExoApp* client = g_pAppManager->m_pClientExoApp;
        client->SetCharacterFile(name.GetLength(), name.CStr(), flag);
        break;
    }
    default: return 0;
    }
    return 1;
}
