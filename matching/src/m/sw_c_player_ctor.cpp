// CSWCPlayer constructor (client player record, created by
// CSWCMessage::HandleServerToPlayerUpdate_PlayerInfo).

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    char m_resRef[16];
};

class CSWPortrait {
public:
    CSWPortrait();                                  // 0x00678EC0
    ~CSWPortrait();                                 // 0x004015A0
    CResRef m_cBaseResRef;
};

class CSWCPlayer {
public:
    CSWCPlayer(unsigned long nPlayerID);
    // Single vtable slot; the original entry is the shared `return 0`
    // stub 0x0063E7F0.
    virtual int Unknown00();

    unsigned long m_nPlayerID;                      // 0x04
    int m_n08;                                      // 0x08
    CExoString m_sPlayerName;                       // 0x0c
    CSWPortrait m_cPortrait;                        // 0x14
    unsigned short m_nPortraitId;                   // 0x24
    char pad_0x26[0x2];
    unsigned long m_oidControlled;                  // 0x28
    char pad_0x2c[0x4];
    CExoString m_s30;                               // 0x30
};

// FUNCTION: SWKOTOR 0x0064B460
CSWCPlayer::CSWCPlayer(unsigned long nPlayerID)
{
    m_nPlayerID = nPlayerID;
    m_n08 = 0;
    m_nPortraitId = 0xffff;
}
