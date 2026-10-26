#include "m/sw_c_message_handlers.h"

// FUNCTION: SWKOTOR 0x006632A0
int CSWCMessage::HandleServerToPlayerJournal_Type6()
{
    CSWCQuestJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    SJournalEntry entry;
    if (MessageReadOverflow(1)) return 0;
    entry.m_sName.AddString(g_pAppManager->m_pClientExoApp->GetClientLanguage(), ReadCExoLocStringClient(CResRef(), 0), 0);
    entry.m_sText.AddString(g_pAppManager->m_pClientExoApp->GetClientLanguage(), ReadCExoLocStringClient(CResRef(), 0), 0);
    entry.m_nCalendarDay = ReadDWORD(0x20);
    entry.m_nTimeOfDay = ReadDWORD(0x20);
    entry.m_sPlotID = ReadCExoString(0x20);
    entry.m_nState = ReadDWORD(0x20);
    entry.m_nPriority = ReadDWORD(0x20);
    entry.m_nPictureIndex = ReadWORD(0x10);
    entry.m_bQuestCompleted = ReadBOOL();
    entry.m_bUpdated = 1;
    entry.m_nPlanetID = ReadDWORD(0x20);
    entry.m_n34 = ReadDWORD(0x20);
    entry.m_nQuestXP = ReadDWORD(0x20);
    journal->Add(entry, 0x8000);
    if (MessageReadUnderflow(1)) return 0;
    return 1;
}

// NONMATCH: flag test/copy scheduling differs; two ordinary source shapes tried.
// FUNCTION: SWKOTOR 0x00652380
int CSWCMessage::HandleServerToPlayerJournal_Type9()
{
    CSWCQuestJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    SJournalEntry entry;
    if (MessageReadOverflow(1)) {
        journal->CallFullUpdateComplete();
        return 0;
    }
    if (ReadBOOL()) {
        journal->DisableSort();
        int full = ReadBOOL();
        if (full) journal->ClearAll();
        int count = ReadDWORD(0x20);
        if (count > 0) do {
            unsigned short flags = ReadWORD(0x10);
            entry.m_sPlotID = ReadCExoString(0x20);
            if (flags & 0x4000) journal->Remove(entry.m_sPlotID);
            else {
                if (full || ((short)flags < 0) || (flags & 0x1)) entry.m_sName.AddString(g_pAppManager->m_pClientExoApp->GetClientLanguage(), ReadCExoLocStringClient(CResRef(), 0), 0);
                if (full || ((short)flags < 0) || (flags & 0x2)) entry.m_sText.AddString(g_pAppManager->m_pClientExoApp->GetClientLanguage(), ReadCExoLocStringClient(CResRef(), 0), 0);
                if (full || ((short)flags < 0) || (flags & 0x4)) entry.m_nCalendarDay = ReadDWORD(0x20);
                if (full || ((short)flags < 0) || (flags & 0x8)) entry.m_nTimeOfDay = ReadDWORD(0x20);
                if (full || ((short)flags < 0) || (flags & 0x10)) entry.m_nState = ReadDWORD(0x20);
                if (full || ((short)flags < 0) || (flags & 0x20)) entry.m_nPriority = ReadDWORD(0x20);
                if (full || ((short)flags < 0) || (flags & 0x40)) entry.m_nPictureIndex = ReadWORD(0x10);
                if (full || ((short)flags < 0) || (flags & 0x80)) entry.m_bQuestCompleted = ReadBOOL();
                if (full || (flags & 0x8200)) entry.m_nPlanetID = ReadDWORD(0x20);
                if (full || (flags & 0x8400)) entry.m_n34 = ReadDWORD(0x20);
                if (full || (flags & 0x8800)) entry.m_nQuestXP = ReadDWORD(0x20);
                entry.m_bUpdated = (flags & 0x100) == 0;
                journal->Add(entry, flags);
            }
        } while (--count);
        journal->EnableSort();
    }
    journal->CallFullUpdateComplete();
    if (MessageReadUnderflow(1)) return 0;
    return 1;
}
