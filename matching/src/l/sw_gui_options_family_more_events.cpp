// Remaining related options panel-opening and toggle events.
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006DE240
void CSWGuiOptionsMain::OnGameplayOpt(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiInGameGameplay* pPanel = new CSWGuiInGameGameplay(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006DE2C0
void CSWGuiOptionsMain::OnAutopauseOpt(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiInGameAutoPause* pPanel = new CSWGuiInGameAutoPause(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E2DF0
void CSWGuiOptionsMain::OnFeedbackOpt(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsFeedback* pPanel = new CSWGuiOptionsFeedback(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E3D80
void CSWGuiOptionsMain::OnGraphicsOpt(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsGraphics* pPanel = new CSWGuiOptionsGraphics(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E3E00
void CSWGuiOptionsMain::OnSoundOpt(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsSound* pPanel = new CSWGuiOptionsSound(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E2D70
void CSWGuiOptionsSound::OnAdvanced(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsSoundAdvanced* pPanel = new CSWGuiOptionsSoundAdvanced(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E2CF0
void CSWGuiOptionsGraphics::OnAdvanced(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsGraphicsAdvanced* pPanel = new CSWGuiOptionsGraphicsAdvanced(m_pManager, this);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E64A0
void CSWGuiInGameGameplay::OnMouse(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsMouse* pPanel = new CSWGuiOptionsMouse(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E6420
void CSWGuiInGameGameplay::OnKeyMap(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiInGameOptKeyMappings* pPanel = new CSWGuiInGameOptKeyMappings(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006DE550
void CSWGuiOptionsFeedback::OnHide(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 0) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback0 = value;
    }
}

// FUNCTION: SWKOTOR 0x006DE5A0
void CSWGuiOptionsFeedback::OnTutorial(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 1) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback1 = value;
    }
}

// FUNCTION: SWKOTOR 0x006DE5F0
void CSWGuiOptionsFeedback::OnSubtitles(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 2) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback2 = value;
    }
}

// FUNCTION: SWKOTOR 0x006DE640
void CSWGuiOptionsFeedback::OnMiniMap(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 3) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback3 = value;
        g_pAppManager->GetClientExoApp()->GetInGameGui()->InitializeMap();
    }
}

// FUNCTION: SWKOTOR 0x006DE6A0
void CSWGuiOptionsFeedback::OnFloaty(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 4) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback4 = value;
    }
}

// FUNCTION: SWKOTOR 0x006DE6F0
void CSWGuiOptionsFeedback::OnSummary(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags >> 5) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bFeedback5 = value;
    }
}

// FUNCTION: SWKOTOR 0x006DE740
void CSWGuiOptionsFeedback::OnHideInGameGUI(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        if (pOptions) {
            unsigned long value = ~(pOptions->m_nFlags >> 9);
        pOptions->m_bFeedback9 = value;
        }
    }
}

// FUNCTION: SWKOTOR 0x006DEAA0
void CSWGuiOptionsFeedback::OnEnableToolTips(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        if (pOptions) {
            unsigned long value = ~(pOptions->m_nFlags >> 10);
        pOptions->m_bFeedback10 = value;
        }
    }
}

// FUNCTION: SWKOTOR 0x006DE780
void CSWGuiOptionsFeedback::OnUseSmallFont(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        if (pOptions) {
            pOptions->SetLargeFont(~(pOptions->m_nFlags >> 8) & 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x006E62A0
void CSWGuiInGameGameplay::OnAutoLevelUp(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nGameplayFlags >> 0) & 1;
        g_pAppManager->GetClientExoApp()->GetOptions()->SetAutoLevelUpNPCs(value);
    }
}

// FUNCTION: SWKOTOR 0x006E62E0
void CSWGuiInGameGameplay::OnAutoSave(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nGameplayFlags >> 2) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bGame2 = value;
    }
}

// FUNCTION: SWKOTOR 0x006E6330
void CSWGuiInGameGameplay::OnInvertCam(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nGameplayFlags >> 1) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bGame1 = value;
    }
}

// FUNCTION: SWKOTOR 0x006E6380
void CSWGuiInGameGameplay::OnReverse(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nGameplayFlags >> 3) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bGame3 = value;
    }
}

// FUNCTION: SWKOTOR 0x006E63D0
void CSWGuiInGameGameplay::OnCombatMovement(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = ~(g_pAppManager->GetClientExoApp()->GetOptions()->m_nGameplayFlags >> 4) & 1;
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        pOptions->m_bGame4 = value;
    }
}
