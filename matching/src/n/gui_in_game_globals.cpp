// CGuiInGame::InitializeGlobal and CGuiInGame::ResetInterfaceForSize
// (guiingame.o): (re)create the global message boxes / main interface.
// Standalone layout.
class CSWGuiManager;
class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
    void CenterPanel(); // 0x0040A600
};
class CSWGuiMessageBox : public CSWGuiPanel {
public:
    CSWGuiMessageBox(CSWGuiManager* pManager); // 0x00626DF0
    char pad_0x4[0x980];
};
// Derived message box with its own vtable (0x007513F8) and no new members.
class CSWGuiInGameMessageBox : public CSWGuiMessageBox {
public:
    CSWGuiInGameMessageBox(CSWGuiManager* pManager) : CSWGuiMessageBox(pManager) {}
};
class CSWGuiPanel24E8 : public CSWGuiPanel {
public:
    CSWGuiPanel24E8(CSWGuiManager* pManager); // 0x006CE7C0
    char pad_0x4[0x24e4];
};
class CSWGuiTutorialBox : public CSWGuiPanel {
public:
    CSWGuiTutorialBox(CSWGuiManager* pManager); // 0x006AA100
    char pad_0x4[0x994];
};
class CSWGuiMainInterface : public CSWGuiPanel {
public:
    CSWGuiMainInterface(CSWGuiManager* pManager); // 0x0068C100
    char pad_0x4[0xc8c8];
};
class CSWGuiDialogCinematic : public CSWGuiPanel {
public:
    CSWGuiDialogCinematic(CSWGuiManager* pManager); // 0x006A8B40
    char pad_0x4[0x1dfc];
};
class CClientExoApp { public: CSWGuiManager* GetSWGuiManager(); /* 0x005EDA40 */ };
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CGuiInGame {
public:
    char pad_0x0[0x38]; CSWGuiManager* m_pGuiManager;
    char pad_0x3c[4]; CSWGuiDialogCinematic* m_pDialogCinematic;
    char pad_0x44[0x10]; CSWGuiPanel* m_pPanel54; CSWGuiPanel* m_pPanel58; CSWGuiPanel* m_pPanel5C;
    char pad_0x60[0x10]; CSWGuiPanel* m_pPanel70; CSWGuiPanel* m_pPanel74;
    char pad_0x78[0x18]; CSWGuiMainInterface* m_pMainInterface;
    char pad_0x94[4]; CSWGuiMessageBox* m_pMessageBox; CSWGuiPanel24E8* m_pPanel9C;
    CSWGuiTutorialBox* m_pTutorialBox; CSWGuiInGameMessageBox* m_pMessageBoxA4;
    void InitializeGlobal();
    void ResetInterfaceForSize();
};

// FUNCTION: SWKOTOR 0x00632720
void CGuiInGame::InitializeGlobal()
{
    if (!m_pGuiManager) m_pGuiManager = g_pAppManager->GetClientExoApp()->GetSWGuiManager();
    m_pMessageBox = new CSWGuiMessageBox(m_pGuiManager);
    m_pPanel9C = new CSWGuiPanel24E8(m_pGuiManager);
    m_pTutorialBox = new CSWGuiTutorialBox(m_pGuiManager);
    m_pMessageBoxA4 = new CSWGuiInGameMessageBox(m_pGuiManager);
}

// FUNCTION: SWKOTOR 0x0062F5F0
void CGuiInGame::ResetInterfaceForSize()
{
    if (m_pMainInterface) {
        delete m_pMainInterface;
        m_pMainInterface = new CSWGuiMainInterface(g_pAppManager->GetClientExoApp()->GetSWGuiManager());
    }
    if (m_pDialogCinematic) {
        delete m_pDialogCinematic;
        m_pDialogCinematic = new CSWGuiDialogCinematic(g_pAppManager->GetClientExoApp()->GetSWGuiManager());
    }
    if (m_pPanel58) m_pPanel58->CenterPanel();
    if (m_pPanel70) m_pPanel70->CenterPanel();
    if (m_pPanel74) m_pPanel74->CenterPanel();
    if (m_pPanel5C) m_pPanel5C->CenterPanel();
    if (m_pMessageBox) {
        delete m_pMessageBox;
        m_pMessageBox = new CSWGuiMessageBox(g_pAppManager->GetClientExoApp()->GetSWGuiManager());
    }
    if (m_pPanel54) m_pPanel54->CenterPanel();
    if (m_pPanel9C) m_pPanel9C->CenterPanel();
    if (m_pTutorialBox) m_pTutorialBox->CenterPanel();
}
