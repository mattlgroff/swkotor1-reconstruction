// End-game message carries a temporary localized string which is discarded.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    ~CExoString();                                  // 0x005E5C20
};

class CSWCMessage {
public:
    void ReadCExoString(CExoString& sText, int nBits); // 0x004D67E0
    int HandleServerToPlayerModule_Type14();
};

// FUNCTION: SWKOTOR 0x00652AD0 SYMBOL: ?HandleServerToPlayerModule_Type14@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerModule_Type14()
{
    char storage[sizeof(CExoString)];
    CExoString* pText = (CExoString*)storage;
    ReadCExoString(*pText, 0x20);
    pText->~CExoString();
    return 1;
}
