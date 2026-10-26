// the in-game options menu CSWGuiOptionsInGame (layout
// "optionsingame", vtable 0x00755DE0), 0x006AAAD0-0x006AB7B0.
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006AAAD0
void CSWGuiOptionsInGame::OnPanelRemoved()
{
    SetActiveControl(0, 0);
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006AAAF0
void CSWGuiOptionsInGame::OnLoadGame(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        g_pAppManager->GetClientExoApp()->SaveOptions();
        CSWGuiSaveLoad* pPanel = new CSWGuiSaveLoad(m_pManager, 0, 0);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAB80
void CSWGuiOptionsInGame::OnSaveGame(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        g_pAppManager->GetClientExoApp()->SaveOptions();
        CSWGuiSaveLoad* pPanel = new CSWGuiSaveLoad(m_pManager, 1, 0);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAC10
void CSWGuiOptionsInGame::OnGameplay(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        m_pManager->AddPanel(new CSWGuiInGameGameplay(m_pManager), 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAC90
void CSWGuiOptionsInGame::OnFeedback(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsFeedback* pPanel = new CSWGuiOptionsFeedback(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAD10
void CSWGuiOptionsInGame::OnAutoPause(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiInGameAutoPause* pPanel = new CSWGuiInGameAutoPause(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAD90
void CSWGuiOptionsInGame::OnGraphics(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsGraphics* pPanel = new CSWGuiOptionsGraphics(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAE10
void CSWGuiOptionsInGame::OnSound(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsSound* pPanel = new CSWGuiOptionsSound(m_pManager);
        m_pManager->AddPanel(pPanel, 3, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAE90
void CSWGuiOptionsInGame::OnQuitConfirmed(CSWGuiControl* pControl)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        g_pAppManager->GetClientExoApp()->SetUnknown1a8(1, 0, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AAEC0
void CSWGuiOptionsInGame::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0xdf:
            if (g_pAppManager->GetClientExoApp()->GetInGameGui()->HideSWInGameGui(0)) {
                g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
            }
        case 0x35:
        case 0x36:
            g_pAppManager->GetClientExoApp()->SaveOptions();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006AB000
void CSWGuiOptionsInGame::SetDescription(int nStrRef)
{
    m_lblDesc.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_lblDesc.m_cText.m_sText;
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDesc.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_lblDesc.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDesc.m_cTextBox.GetLineHeight();
    }
    m_lblDesc.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDesc;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDesc, 0);
    m_lblScroll.m_bVisible = m_lbDesc.IsOversized();
}

// FUNCTION: SWKOTOR 0x006AB140
void CSWGuiOptionsInGame::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
    CSWGuiControl* pControl = m_pManager->m_pMouseOverControl;
    if (pControl && pControl->IsSelectable()) {
        int i = GetButtonIndex(pControl);
        if (i >= 0 && i < 8) {
            SetActiveControl(&m_aButtons[i], 0);
        }
    }
    if (!GetFocusControl()) {
        SetActiveControl(&m_aButtons[0], 0);
    }
}

// FUNCTION: SWKOTOR 0x006AB1B0
void CSWGuiOptionsInGame::OnQuit(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        g_pAppManager->GetClientExoApp()->SaveOptions();
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(1);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnQuitConfirmed, 0);
        pBox->SetMessageStrRef(0xa56c);
        m_pManager->AddPanel(pBox, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006AB220
void CSWGuiOptionsInGame::OnButtonSelected(CSWGuiControl* pControl)
{
    int nIndex = -1;
    for (int i = 0; i < 8; i++) {
        if (nIndex != -1) {
            break;
        }
        if (pControl == &m_aButtons[i]) {
            nIndex = i;
        }
    }
    switch (nIndex) {
    case 0:
        SetDescription(0xa53c);
        break;
    case 1:
        SetDescription(0xa53d);
        break;
    case 2:
        SetDescription(0xbb86);
        break;
    case 3:
        SetDescription(0xbd99);
        break;
    case 4:
        SetDescription(0xbdbc);
        break;
    case 5:
        SetDescription(0xbb8a);
        break;
    case 6:
        SetDescription(0xbb8c);
        break;
    case 7:
        SetDescription(0xa53e);
        break;
    }
}

// FUNCTION: SWKOTOR 0x006AB2F0
CSWGuiOptionsInGame::CSWGuiOptionsInGame(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("optionsingame");
    InitControl(&m_aButtons[0], "BTN_LOADGAME", 1);
    InitControl(&m_aButtons[1], "BTN_SAVEGAME", 1);
    InitControl(&m_aButtons[2], "BTN_GAMEPLAY", 1);
    InitControl(&m_aButtons[3], "BTN_FEEDBACK", 1);
    InitControl(&m_aButtons[4], "BTN_AUTOPAUSE", 1);
    InitControl(&m_aButtons[5], "BTN_GRAPHICS", 1);
    InitControl(&m_aButtons[6], "BTN_SOUND", 1);
    InitControl(&m_aButtons[7], "BTN_QUIT", 1);
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_btnExit, "BTN_EXIT", 1);
    UnloadResource();
    m_lblDesc.CopyStyle(m_lbDesc.m_pProtoItem->m_extent, m_lbDesc.m_pProtoItem, 1.0f);
    m_aButtons[0].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnLoadGame);
    m_aButtons[1].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnSaveGame);
    m_aButtons[2].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnGameplay);
    m_aButtons[3].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnFeedback);
    m_aButtons[4].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnAutoPause);
    m_aButtons[5].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnGraphics);
    m_aButtons[6].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnSound);
    m_aButtons[7].AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnQuit);
    for (int i = 0; i < 8; i++) {
        m_aButtons[i].AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsInGame::OnButtonSelected);
        m_aButtons[i].m_cHilightParams.SetFlags(1, 1, 0);
    }
    CSWGuiButton* pExit = &m_btnExit;
    pExit->m_bNavigable = 0;
    pExit->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiPanel* pPanel = this;
    pPanel->SetActiveControl(&m_aButtons[0], 0);
}

// FUNCTION: SWKOTOR 0x006AB790 SYMBOL: ??_GCSWGuiOptionsInGame@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x006AB7B0 SYMBOL: ??1CSWGuiOptionsInGame@@UAE@XZ
