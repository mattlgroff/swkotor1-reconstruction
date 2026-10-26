// CGuiInGame::ShowSWInGameGui (guiingame.o). Standalone layout.
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    CExoString(const char* s); // 0x005E5A90
    ~CExoString(); // 0x005E5C20
};
class CVirtualMachine { public: int RunScript(CExoString* sScript,unsigned long oid,int bArg); /* 0x005D0FC0 */ };
extern CVirtualMachine* g_pVirtualMachine; // 0x007A3A00
class CExoSound { public: void SetSoundMode(int nMode); /* 0x005D5E80 */ };
extern CExoSound* g_pExoSound; // 0x007A39EC
extern int g_nGuiSoundMode; // 0x0074C5FC
class CSWSCreatureStats { public: int CanLevelUp(); /* 0x005A6810 */ };
class CSWSCreature { public: char pad_0x0[0xa74]; CSWSCreatureStats* m_pStats; };
class CSWCCreature { public: CSWSCreature* GetServerCreature(); /* 0x0060FB20 */ };
class CSWParty { public: CSWCCreature* GetPlayerCharacter(); /* 0x00635460 */ };
class CSWCObject;
class CSWCMessage { public: int SendPlayerToServerInput_TogglePauseRequest(); /* 0x00677800 */ };
class CSWGuiPanel;
class CSWGuiManager {
public:
    void PlayGuiSound(char nSound); // 0x0040A140
    void AddPanel(CSWGuiPanel* pPanel,int nLayer,int bArg); // 0x0040BC70
    void SendPanelToBack(CSWGuiPanel* pPanel); // 0x0040BD20
};
class CClientExoApp {
public:
    CSWGuiManager* GetSWGuiManager(); // 0x005EDA40
    CSWCMessage* GetSWCMessage(); // 0x005ED6F0
    CSWParty* GetSWParty(); // 0x005ED8B0
    void IndicateObject(CSWCObject* pObject,int bArg); // 0x005EDB70
};
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CSWGuiInGameMenu { public: void SetActivePanel(int nPanel,int bArg); /* 0x00624BD0 */ };
class CSWGuiBarkBubble { public: void Pause(); /* 0x006A9BB0 */ };
class CGuiInGame {
public:
    char pad_0x0[8]; CSWGuiInGameMenu* m_pMenu; CSWGuiPanel* m_apPanels[8];
    int m_nCurrentPanel; int m_bShown;
    char pad_0x34[4]; CSWGuiManager* m_pGuiManager;
    char pad_0x3c[0x10]; CSWGuiBarkBubble* m_pBarkBubble;
    char pad_0x50[0xb8]; int m_bInitialized;
    char pad_0x10c[0xa2c]; int m_bPaused;
    void SetSWGuiStatus(int nStatus,int bArg); // 0x0062AA00
    void UpdateCreatedInGameGUI(int nOldPanel,int nNewPanel); // 0x0062B1E0
    int ShowSWInGameGui(int nPanel);
};

// FUNCTION: SWKOTOR 0x0062C9B0
int CGuiInGame::ShowSWInGameGui(int nPanel)
{
    if (m_bInitialized) {
        CSWGuiManager* pManager = g_pAppManager->GetClientExoApp()->GetSWGuiManager();
        if (pManager) {
            g_pExoSound->SetSoundMode(g_nGuiSoundMode);
            g_pAppManager->GetClientExoApp()->IndicateObject(0,0);
            SetSWGuiStatus(3,1);
            CExoString sScript("k_sup_guiopen");
            g_pVirtualMachine->RunScript(&sScript,0,0);
            if (nPanel >= 0 && nPanel < 8) {
                m_nCurrentPanel = nPanel;
            } else if (g_pAppManager->GetClientExoApp()->GetSWParty()->GetPlayerCharacter()) {
                if (g_pAppManager->GetClientExoApp()->GetSWParty()->GetPlayerCharacter()->GetServerCreature()->m_pStats->CanLevelUp())
                    m_nCurrentPanel = 2;
            }
            UpdateCreatedInGameGUI(-1,m_nCurrentPanel);
            pManager->AddPanel((CSWGuiPanel*)m_pMenu,0,1);
            pManager->AddPanel(m_apPanels[m_nCurrentPanel],2,1);
            pManager->SendPanelToBack(m_apPanels[m_nCurrentPanel]);
            m_pMenu->SetActivePanel(m_nCurrentPanel,0);
            m_bShown = 1;
            if (!m_bPaused) g_pAppManager->GetClientExoApp()->GetSWCMessage()->SendPlayerToServerInput_TogglePauseRequest();
            m_pBarkBubble->Pause();
            if (g_pVirtualMachine) {
                CExoString sChange("k_pend_screenchg");
                g_pVirtualMachine->RunScript(&sChange,0x7f000000,0);
            }
            m_pGuiManager->PlayGuiSound(4);
        }
    }
    return m_bShown;
}
