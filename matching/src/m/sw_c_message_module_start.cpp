// CSWCMessage start-new-module handshake handlers.

class CClientExoApp {
public:
    void SetInStartNewModule(int bInProgress);       // 0x005ED630
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                  // +0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCMessage {
public:
    int ReadBOOL();                                  // 0x004D66F0
    int MessageReadOverflow(int nBits);              // 0x004D6230
    int MessageReadUnderflow(int nBits);             // 0x004D6240
    int SendPlayerToServer_02_0F();                  // 0x00678030

    int HandleServerToPlayerModule_Type11();
    int HandleServerToPlayerModule_Type12();
};

// FUNCTION: SWKOTOR 0x00652840 SYMBOL: ?HandleServerToPlayerModule_Type11@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerModule_Type11()
{
    g_pAppManager->m_pClientExoApp->SetInStartNewModule(1);
    return 1;
}

// FUNCTION: SWKOTOR 0x00652860 SYMBOL: ?HandleServerToPlayerModule_Type12@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerModule_Type12()
{
    int bStartNewModule = ReadBOOL();
    g_pAppManager->m_pClientExoApp->SetInStartNewModule(0);
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        if (bStartNewModule == 1) {
            SendPlayerToServer_02_0F();
        }
        return 1;
    }
    return 0;
}
