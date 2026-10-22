// CSWGuiInGameMessages (b0063 client gui panels).

#define OBJECT_INVALID 0x7f000000

class CGuiInGame {
public:
    int UpdateMessageGUI();                         // 0x0062AD60

    char pad_0x0[0xbc8];
    unsigned char m_bDialogMessages;                // 0xbc8
};

class CClientExoApp {
public:
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    void ShowTutorialWindow(int n, unsigned long a, unsigned long b, int c);   // 0x005EDF40
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiPanel {
public:
    void OnPanelAdded();                            // 0x0040B870
};

class CSWGuiInGameMessages : public CSWGuiPanel {
public:
    void OnPanelAdded();
    void ShowFeedback();                            // 0x00624DF0
    void ShowDialog();                              // 0x00624F70
};

// FUNCTION: SWKOTOR 0x00626D90
void CSWGuiInGameMessages::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    g_pAppManager->GetClientExoApp()->GetGuiInGame()->UpdateMessageGUI();
    if (!g_pAppManager->GetClientExoApp()->GetGuiInGame()->m_bDialogMessages) {
        ShowFeedback();
    } else {
        ShowDialog();
    }
    g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0xc, OBJECT_INVALID, OBJECT_INVALID, 0);
}
