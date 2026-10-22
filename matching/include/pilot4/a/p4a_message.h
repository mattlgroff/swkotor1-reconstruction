// Pilot 4 arm A: CNWMessage / CSWMessage / CSWCMessage subset.
#ifndef P4A_MESSAGE_H
#define P4A_MESSAGE_H

class CExoString {
public:
    CExoString();                                       // 0x005E5A90
    ~CExoString();                                      // 0x005E5C20
    CExoString& operator=(const CExoString& s);         // 0x005E5C50

    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                          // 0x00405F70

    char m_resRef[16];
};

class CClientExoApp {
public:
    void* GetSomething();                               // 0x005ED330
    void JournalUpdated(int a, int b, const CExoString& sText); // 0x005ED340
};

class CAppManager {
public:
    void* m_pUnknown00;                                 // 0x00
    CClientExoApp* m_pClientExoApp;                     // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                      // 0x007A39FC

struct STR_RES {
    CExoString sString;                                 // 0x00
    CResRef cSoundResRef;                               // 0x08
    float fSoundLength;                                 // 0x18
};

class CNWMessage {
public:
    virtual ~CNWMessage();

    int ReadBOOL();                                     // 0x004D66F0
    int MessageReadOverflow(int bFlag);                 // 0x004D6230
    int MessageReadUnderflow(int bFlag);                // 0x004D6240
    unsigned long ReadDWORD(int nBits);                 // 0x004D6780
};

class CSWMessage : public CNWMessage {
};

class CSWCMessage : public CSWMessage {
public:
    CExoString ReadCExoLocStringClient(CResRef& cSound, unsigned long* pLength); // 0x00692660
    int HandleServerToPlayerJournalUpdated();                       // 0x006526E0
    int HandlePlayerToServerShutDownServer(unsigned char nMinor);   // 0x00654970
};

#endif
