// Controller pause, tutorial gating and fade visibility share the GUI state.
class CExoInput { public: int GetMainDeviceLost(); int GetActive(); void SetActive(int); };
extern CExoInput* g_pExoInput;
class CWorldTimer { public: void GetWorldTime(unsigned long*,unsigned long*); };
class CClientOptions { public: char pad_0x0[0x14]; unsigned long m_nFlags; };
class CClientExoApp {
public: CWorldTimer* GetWorldTimer(); CClientOptions* GetOptions(); void* GetMiniGame();
    int SetPausedByCombat(int,int,int); int GetPazaakIsRunning();
};
class CAppManager { public: char pad_0x0[4]; CClientExoApp* m_pClient; void* m_pServer; };
extern CAppManager* g_pAppManager;
class CSWGuiTutorialBox { public: int HasTutorial(unsigned char); };
class CSWGuiPanel { public: char pad_0x0[0x44]; unsigned long m_nFlags; };
class CSWGuiMainInterface { public: char pad_0x0[0x5d04]; union { unsigned long m_nFlags; struct { unsigned long m_bOther:1,m_bMinimapHidden:1,m_nRest:30; }; }; };
class CSWGuiManager { public: int PanelExists(CSWGuiPanel*); };
class CGuiInGame {
public:
    char pad_0x0[8]; CSWGuiPanel* m_pMainMenu;
    char pad_0xc[0x2c]; CSWGuiManager* m_pGuiManager;
    char pad_0x3c[0x3c]; CSWGuiPanel* m_pOptions;
    char pad_0x7c[4]; CSWGuiPanel* m_pMenu; CSWGuiPanel* m_pMessages;
    char pad_0x88[8]; CSWGuiMainInterface* m_pMainInterface;
    char pad_0x94[0xc]; CSWGuiTutorialBox* m_pTutorial;
    char pad_0xa4[0x10]; int m_bDialogScrollFinished;
    char pad_0xb8[0x20]; unsigned long m_nInputPause; int m_bControllerPaused,m_bOtherPause;
    char pad_0xe4[0x24]; int m_bFadeOverride;
    char pad_0x10c[0xba8-0x10c]; unsigned char m_tutorialShown[6];
    char pad_0xbae[0xbcc-0xbae]; unsigned long m_nNoClickDay,m_nNoClickTime,m_nNoClickDuration;
    char pad_0xbd8[0x30]; int m_bSavedInputActive;
    void EnterControllerPauseState(); void UpdateControllerPauseState(); void EndControllerPauseState();
    int IsMinimapHidden(); int TutorialReasonWillShowWindow(unsigned char reason);
    void MarkNoClickEvent(unsigned long duration); int GetFadeOverridePanelVisible();
};
// FUNCTION: SWKOTOR 0x0062F780
void CGuiInGame::UpdateControllerPauseState() {
    if(g_pExoInput->GetMainDeviceLost() && m_nInputPause<=0 && !m_bControllerPaused) {
        m_bSavedInputActive=g_pExoInput->GetActive(); g_pExoInput->SetActive(1);
        EnterControllerPauseState(); m_bControllerPaused=1;
    }
}
// FUNCTION: SWKOTOR 0x0062F7E0
void CGuiInGame::EndControllerPauseState() {
    m_bControllerPaused=0;
    if(m_bSavedInputActive==1) g_pExoInput->SetActive(1); else g_pExoInput->SetActive(0);
    if(g_pAppManager && g_pAppManager->m_pServer && !m_bOtherPause) g_pAppManager->m_pClient->SetPausedByCombat(0,0,1);
}
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x0062F910
int CGuiInGame::IsMinimapHidden() {
    if((g_pAppManager->m_pClient->GetOptions()->m_nFlags&8) && !g_pAppManager->m_pClient->GetMiniGame()) {
        if(m_pMainInterface) return m_pMainInterface->m_bMinimapHidden; return 1;
    } return 0;
}
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x0062F420
int CGuiInGame::TutorialReasonWillShowWindow(unsigned char reason) {
    if(g_pAppManager->m_pClient->GetOptions()->m_nFlags&2) {
        if(reason<43) {
            unsigned char byte=reason/8;
            unsigned char shift=(unsigned char)(reason-(unsigned char)(byte*8));
            unsigned long shown=(1 << shift) & m_tutorialShown[byte];
            if(shown) return 0;
        }
        if(!m_bDialogScrollFinished && m_pTutorial->HasTutorial(reason)) return 1;
    }
    return 0;
}
// FUNCTION: SWKOTOR 0x0062F960
void CGuiInGame::MarkNoClickEvent(unsigned long duration) {
    CClientExoApp* app=g_pAppManager->m_pClient; app->GetWorldTimer()->GetWorldTime(&m_nNoClickDay,&m_nNoClickTime); m_nNoClickDuration=duration;
}
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x0062FAD0
int CGuiInGame::GetFadeOverridePanelVisible() {
    if(m_bFadeOverride && m_pGuiManager) {
        if((m_pMenu && m_pGuiManager->PanelExists(m_pMenu) && (m_pMenu->m_nFlags&4)) ||
           (m_pOptions && m_pGuiManager->PanelExists(m_pOptions) && (m_pOptions->m_nFlags&4)) ||
           (m_pMessages && m_pGuiManager->PanelExists(m_pMessages) && (m_pMessages->m_nFlags&4)) ||
           (m_pMainMenu && m_pGuiManager->PanelExists(m_pMainMenu) && (m_pMainMenu->m_nFlags&4)) ||
           g_pAppManager->m_pClient->GetPazaakIsRunning()) return 1;
        return 0;
    }
    return 0;
}
class CSWGuiFade {
public: char pad_0x0[0x64]; float m_fDuration,m_fStart,m_fCurrent;
    int IsFadeDone();
};
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x006244B0
int CSWGuiFade::IsFadeDone() { if(m_fCurrent!=0.0f && !(m_fStart+m_fDuration<m_fCurrent)) return 0; return 1; }
