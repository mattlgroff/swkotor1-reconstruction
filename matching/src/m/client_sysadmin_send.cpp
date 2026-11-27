// Send a system administration command from the client to the server.
#include <string.h>
#include "shared/exo_string.h"
class CNetLayer {
public:
    int SendMessageToPlayer(unsigned long player, unsigned char* message,
                            unsigned long size, unsigned long flags); // 0x005D4CB0
};
class CClientExoApp {
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual CNetLayer* GetNetLayer(); // vtable +0x10
};
#include "shared/app_manager.h"
#include "shared/globals.h"
class CClientMsgSysAdmin {
public:
    void SendSysAdminToServerMessage(const CExoString* text);
};

// NONMATCH (9): null-path block placement and memcpy register scheduling.
// The class/method name and void return are descriptive pending independent ABI evidence.
// FUNCTION: SWKOTOR 0x00675690
void CClientMsgSysAdmin::SendSysAdminToServerMessage(const CExoString* text)
{
    int length = text->GetLength() + 1;
    unsigned char* packet = new unsigned char[length];
    if (packet) {
        packet[0] = 's';
        memcpy(packet + 1, text->CStr(), text->GetLength());
        g_pAppManager->m_pClientExoApp->GetNetLayer()->SendMessageToPlayer(
            PLAYERID_SERVER, packet, length, 0);
    }
}
