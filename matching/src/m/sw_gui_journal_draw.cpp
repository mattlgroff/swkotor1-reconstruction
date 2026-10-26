// Journal panel draw refreshes changed lists before drawing.
class CSWCJournal {
public:
    char pad_0x00[0x1c];
    int m_bDoneChanged;
    char pad_0x20[0x1c];
    int m_bActiveChanged;
};
class CGuiInGame { public: char pad_0x00[0xbc4]; unsigned m_bJournalDone : 1; };
class CClientExoApp {
public:
    CSWCJournal* GetQuestJournal(); // 0x005ED320
    CGuiInGame* GetInGameGui();     // 0x005ED690
};
class CAppManager { public: char pad_0x00[4]; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;  // 0x007A39FC
class CSWGuiPanel { public: void Draw(float delta); }; // 0x0040B760
class CSWGuiInGameJournal : public CSWGuiPanel {
public:
    void PopulateItemListBox();     // 0x00645330
    void Draw(float delta);
};

// FUNCTION: SWKOTOR 0x00645670
void CSWGuiInGameJournal::Draw(float delta)
{
    CSWCJournal* journal = g_pAppManager->m_pClientExoApp->GetQuestJournal();
    if ((!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone && journal->m_bActiveChanged) ||
        (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bJournalDone && journal->m_bDoneChanged)) {
        PopulateItemListBox();
    }
    CSWGuiPanel::Draw(delta);
}
