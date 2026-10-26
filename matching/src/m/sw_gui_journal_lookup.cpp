// Journal panel entry selection.
class CSWCJournalEntry;
class CSWCJournal {
public:
    int GetDoneCount();                              // 0x00675DD0
    int GetActiveCount();                            // 0x005A4C50
    CSWCJournalEntry* GetDoneEntryAt(int index);      // 0x00676340
    CSWCJournalEntry* GetActiveEntryAt(int index);    // 0x00676310
};
class CGuiInGame {
public:
    char pad_0x00[0xbc4];
    unsigned m_bJournalDone : 1;
};
class CClientExoApp {
public:
    CSWCJournal* GetQuestJournal();                  // 0x005ED320
    CGuiInGame* GetInGameGui();                      // 0x005ED690
};
class CAppManager {
public:
    char pad_0x00[4];
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;                   // 0x007A39FC
class CSWGuiInGameJournal {
public:
    CSWCJournalEntry* GetQuestEntryAt(int index);
};

// FUNCTION: SWKOTOR 0x00644960
CSWCJournalEntry* CSWGuiInGameJournal::GetQuestEntryAt(int index)
{
    CSWCJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone) {
        if (index >= journal->GetDoneCount()) return 0;
        return journal->GetDoneEntryAt(index);
    }
    if (index >= journal->GetActiveCount()) return 0;
    return journal->GetActiveEntryAt(index);
}
