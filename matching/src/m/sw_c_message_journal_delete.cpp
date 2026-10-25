// CSWCMessage handlers for world-journal deletion updates.

class CSWCWorldJournal {
public:
    void DeleteEntry(unsigned long nIndex);             // 0x00676F00
    void DeleteEntryStrRef(unsigned long nStrRef);      // 0x00676E60
    void DeleteAllEntries();                            // 0x00676FA0
};

class CClientExoApp {
public:
    CSWCWorldJournal* GetWorldJournal();                // 0x005ED330
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                     // +0x04
};

extern CAppManager* g_pAppManager;                     // 0x007A39FC

class CSWCMessage {
public:
    unsigned char ReadBYTE(int nBits);                  // 0x004D6720
    unsigned long ReadDWORD(int nBits);                 // 0x004D6780
    int MessageReadOverflow(int bFlag);                 // 0x004D6230
    int MessageReadUnderflow(int bFlag);                // 0x004D6240

    int HandleServerToPlayerJournal_Type3();
    int HandleServerToPlayerJournal_Type4();
    int HandleServerToPlayerJournal_Type5();
};

// FUNCTION: SWKOTOR 0x00652290 SYMBOL: ?HandleServerToPlayerJournal_Type3@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerJournal_Type3()
{
    CSWCWorldJournal* pJournal = g_pAppManager->m_pClientExoApp->GetWorldJournal();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    pJournal->DeleteEntry(ReadDWORD(0x20));
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006522E0 SYMBOL: ?HandleServerToPlayerJournal_Type4@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerJournal_Type4()
{
    CSWCWorldJournal* pJournal = g_pAppManager->m_pClientExoApp->GetWorldJournal();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    pJournal->DeleteEntryStrRef(ReadDWORD(0x20));
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00652330 SYMBOL: ?HandleServerToPlayerJournal_Type5@CSWCMessage@@QAEHXZ
int CSWCMessage::HandleServerToPlayerJournal_Type5()
{
    CSWCWorldJournal* pJournal = g_pAppManager->m_pClientExoApp->GetWorldJournal();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    ReadBYTE(8);
    pJournal->DeleteAllEntries();
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}
