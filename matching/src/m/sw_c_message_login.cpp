// CSWCMessage::HandleServerToPlayerLogin (nwchandlemessage.o); the name
// follows the playable reference (recovered_client_handle_login).

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
};

class CClientExoApp {
public:
    void EmptyForwarder();                          // 0x005ED310
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern int g_bModuleLoadEnabled;                    // 0x008328FC

class CSWCMessage {
public:
    int MessageReadOverflow(int bError);            // 0x004D6230
    int MessageReadUnderflow(int bError);           // 0x004D6240
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    int HandleServerToPlayerLogin(unsigned char nMinor);
};

// FUNCTION: SWKOTOR 0x0064EC00
// NONMATCH (19 diff insns): instructions agree; only the frame layout differs
// (the original keeps a 4-byte hole below the byte array and the loop trip
// count between it and the strings). Declaration order/sizes did not move it.
int CSWCMessage::HandleServerToPlayerLogin(unsigned char nMinor)
{
    CExoString sPlayerName;
    CExoString sPassword;
    unsigned char anTypes[4];
    unsigned long anIds[3];
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nMinor) {
    case 5:
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        g_bModuleLoadEnabled = 1;
        return 1;
    case 6:
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        return 1;
    case 10: {
        unsigned char nCount = ReadBYTE(8);
        for (unsigned char i = 0; i < nCount; i++) {
            anIds[i] = ReadDWORD(32);
            anTypes[i] = ReadBYTE(8);
        }
        ReadDWORD(32);
        return 1;
    }
    case 0x10:
        g_pAppManager->m_pClientExoApp->EmptyForwarder();
        return 1;
    case 0x12:
        ReadDWORD(32);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        return 1;
    }
    return 0;
}
