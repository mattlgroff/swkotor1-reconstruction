#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x00651ED0
int CSWCMessage::HandleServerToPlayerJournal_Type7()
{
    CExoString tag;
    CSWCQuestJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    if (MessageReadOverflow(1)) return 0;
    tag = ReadCExoString(0x20);
    journal->Remove(tag);
    if (MessageReadUnderflow(1)) return 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00651FB0
int CSWCMessage::HandleServerToPlayerJournal_Type8()
{
    CSWCQuestJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    CExoString tag;
    if (MessageReadOverflow(1)) return 0;
    tag = ReadCExoString(0x20);
    journal->SetJournalPicture(tag, ReadDWORD(0x20));
    if (MessageReadUnderflow(1)) return 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006520A0
int CSWCMessage::HandleServerToPlayerJournal_Type1()
{
    CExoString first, second;
    CSWCWorldJournal* journal = g_pAppManager->m_pClientExoApp->GetWorldJournal();
    if (MessageReadOverflow(1)) return 0;
    unsigned long index = ReadDWORD(0x20);
    first = ReadCExoString(0x20);
    second = ReadCExoString(0x20);
    unsigned long day = ReadDWORD(0x20);
    unsigned long time = ReadDWORD(0x20);
    journal->AddEntry(index, first, second, day, time);
    if (MessageReadUnderflow(1)) return 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x00652210
int CSWCMessage::HandleServerToPlayerJournal_Type2()
{
    CSWCWorldJournal* journal = g_pAppManager->m_pClientExoApp->GetWorldJournal();
    if (MessageReadOverflow(1)) return 0;
    unsigned long first = ReadDWORD(0x20);
    unsigned long second = ReadDWORD(0x20);
    unsigned long day = ReadDWORD(0x20);
    unsigned long time = ReadDWORD(0x20);
    journal->AddEntryStrRef(first, second, day, time);
    return !MessageReadUnderflow(1);
}
