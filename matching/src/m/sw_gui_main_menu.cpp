// The title-screen main menu CSWGuiMainMenu (vtable 0x00752F70) and its 3D
// scene view CSWGui3DSceneView (vtable 0x00752E30), 0x0067AC10-0x0067CA4F.
// Handler names follow the playable reference (src/generated/class-choice
// c9d0373605f94236_mainmenu.h, 46fde490a32f5ba2_hover_path.h);
// ClearModuleNames, OnModuleSelected, PopulateModules and ShowWarpList are
// descriptive.

#include "m/sw_gui_main_menu.h"

// FUNCTION: SWKOTOR 0x0067AC10
void CSWGuiMainMenu::Draw(float fDelta)
{
    CSWGuiPanel::Draw(fDelta);
    if (m_nDrawCount > 4) {
        m_bActive = 1;
    }
    else {
        m_nDrawCount++;
    }
}

// FUNCTION: SWKOTOR 0x0067AC50
CSWGui3DSceneView::~CSWGui3DSceneView()
{
}

// FUNCTION: SWKOTOR 0x0067ACB0
void CSWGui3DSceneView::Draw(float fDelta)
{
    m_cScene.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x0067ACC0 SYMBOL: ??_GCSWGui3DSceneView@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067ACE0
void CSWGuiMainMenu::LoadFromLayout(CSWGuiManager* pManager)
{
    {
        CResRef cLayout("mainmenu");
        StartLoadFromLayout(cLayout);
    }
    m_nFlags |= 1;
    {
        CExoString sName("LB_MODULES");
        InitControl(&m_lbModules, sName, 1);
    }
    {
        CExoString sName("BTN_EXIT");
        InitControl(&m_btnExit, sName, 1);
    }
    {
        CExoString sName("BTN_WARP");
        InitControl(&m_btnWarp, sName, 1);
    }
    {
        CExoString sName("LBL_3DVIEW");
        InitControl(&m_lbl3DView, sName, 1);
    }
    {
        CExoString sName("BTN_NEWGAME");
        InitControl(&m_btnNewGame, sName, 1);
    }
    {
        CExoString sName("BTN_LOADGAME");
        InitControl(&m_btnLoadGame, sName, 1);
    }
    {
        CExoString sName("BTN_MOVIES");
        InitControl(&m_btnMovies, sName, 1);
    }
    {
        CExoString sName("BTN_OPTIONS");
        InitControl(&m_btnOptions, sName, 1);
    }
    {
        CExoString sName("LBL_NEWCONTENT");
        InitControl(&m_lblNewContent, sName, 1);
    }
    {
        CExoString sName("LBL_GAMELOGO");
        InitControl(&m_alblLogo[0], sName, 1);
    }
    {
        CExoString sName("LBL_MENUBG");
        InitControl(&m_alblLogo[1], sName, 1);
    }
    UnloadResource();
}

// FUNCTION: SWKOTOR 0x0067AFB0
void CSWGuiMainMenu::OnNewGamePicked(CSWGuiControl* pControl)
{
    if (IsMarkedForDelete() || !m_bActive || !pControl->m_n4c) {
        return;
    }
    CSWPartyTable::ResetCurrentSessionStartTime();
    CExoIni cIni;
    CExoString sUnused;
    CExoString sModule("END_M01AA");
    {
        CExoString sDir("MODULES:");
        g_pExoResMan->AddResourceDirectory(sDir);
    }
    if (!g_pExoResMan->Exists(sModule, 0x7db, 0) && !g_pExoResMan->Exists(sModule, 0xbba, 0)) {
        sModule = "END_M01AA";
    }
    {
        CExoString sDir("MODULES:");
        g_pExoResMan->RemoveResourceDirectory(sDir);
    }
    m_pManager->AddPanel(new CSWGuiClassSelection(m_pManager, sModule), 2, 1);
    g_pExoSound->SetSoundMode(g_nSoundModeBack);
    MarkForDelete();
}

// FUNCTION: SWKOTOR 0x0067B1A0
void CSWGuiMainMenu::OnLoadSaveGame(CSWGuiControl* pControl)
{
    if (IsMarkedForDelete() || !m_bActive || !pControl->m_n4c) {
        return;
    }
    CSWGuiPanel* pPanel = new CSWGuiSaveLoad(m_pManager, 0, 1);
    m_pManager->AddPanel(pPanel, 2, 1);
    if (!pPanel->IsMarkedForDelete()) {
        MarkForDelete();
    }
}

// FUNCTION: SWKOTOR 0x0067B250
void CSWGuiMainMenu::OnMoviesPicked(CSWGuiControl* pControl)
{
    if (IsMarkedForDelete() || !m_bActive || !pControl->m_n4c) {
        return;
    }
    g_pAppManager->m_pClientExoApp->BusyWaitMenuMusic();
    m_pManager->AddPanel(new CSWGuiTitleMovies(m_pManager), 3, 1);
}

// FUNCTION: SWKOTOR 0x0067B2F0
void CSWGuiMainMenu::OnOptionsPicked(CSWGuiControl* pControl)
{
    if (IsMarkedForDelete() || !m_bActive || !pControl->m_n4c) {
        return;
    }
    m_pManager->AddPanel(new CSWGuiOptionsMain(m_pManager), 3, 1);
}

// FUNCTION: SWKOTOR 0x0067B380
void CSWGuiMainMenu::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            if (m_nState == 2) {
                m_lbModules.SetVisible(0);
                m_lbl3DView.SetVisible(1);
                m_btnLoadGame.SetVisible(1);
                m_btnMovies.SetVisible(1);
                m_btnNewGame.SetVisible(1);
                m_btnOptions.SetVisible(1);
                m_btnExit.SetVisible(1);
                m_btnWarp.SetVisible(1);
                m_alblLogo[0].SetVisible(1);
                m_alblLogo[1].SetVisible(1);
                SetActiveControl(&m_btnNewGame, 0);
                m_nState = 1;
            }
            break;
        case 0x2d:
            m_pManager->PlayGuiSound(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0067B450
void CSWGuiMainMenu::OnEnterButton(CSWGuiControl* pControl)
{
    ((CSWGuiButton*)pControl)->m_cText.m_cParams.SetColor(g_vAbilityColorGranted);
}

// FUNCTION: SWKOTOR 0x0067B470
void CSWGuiMainMenu::OnExitButton(CSWGuiControl* pControl)
{
    ((CSWGuiButton*)pControl)->m_cText.m_cParams.SetColor(g_vPazaakButtonHilight);
}

// FUNCTION: SWKOTOR 0x0067B490
void CSWGuiMainMenu::LaunchDashboard(CSWGuiControl* pControl)
{
    g_pAppManager->m_pClientExoApp->ExitProgram();
}

// FUNCTION: SWKOTOR 0x0067B4A0
void CSWGuiMainMenu::OnQuitButtonPressed(CSWGuiControl* pControl)
{
    if (m_bActive) {
        if (m_nState == 2) {
            HandleInputEvent(0x2e, 1);
        }
        else {
            g_pAppManager->m_pClientExoApp->ExitProgram();
        }
    }
}

// FUNCTION: SWKOTOR 0x0067B4D0 SYMBOL: ??1?$CExoArrayList@UCSWGuiModuleEntry@@@@QAE@XZ
template class CExoArrayList<CSWGuiModuleEntry>;

// FUNCTION: SWKOTOR 0x0067B500
CSWGuiMainMenu::~CSWGuiMainMenu()
{
    m_lbModules.ClearControls();
    CResRef cModule("MAINMENU");
    if (g_pExoResMan->Exists(cModule, 0xbba, 0)) {
        g_pExoResMan->m_nFlags |= 2;
    }
}

// FUNCTION: SWKOTOR 0x0067B910 SYMBOL: ??_GCSWGuiMainMenu@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067B930
void CSWGuiMainMenu::ClearModuleNames()
{
    while (m_lstModuleNames.num > 0) {
        m_lstModuleNames.DelIndex(0);
    }
    m_lstModuleNames.Clear();
    m_lstModuleNames.num = 0;
}

// FUNCTION: SWKOTOR 0x0067C3F0
void CSWGuiMainMenu::ShowWarpList()
{
    if (IsMarkedForDelete() || !m_bActive) {
        return;
    }
    if (m_lbModules.m_nControls == 0) {
        PopulateModules();
    }
    m_lbModules.SetVisible(1);
    m_lbl3DView.SetVisible(0);
    m_btnLoadGame.SetVisible(0);
    m_btnMovies.SetVisible(0);
    m_btnNewGame.SetVisible(0);
    m_btnOptions.SetVisible(0);
    m_btnExit.SetVisible(0);
    m_btnWarp.SetVisible(0);
    m_alblLogo[0].SetVisible(0);
    m_alblLogo[1].SetVisible(0);
    SetActiveControl(&m_lbModules, 0);
    m_lbModules.SetActiveControl(m_lbModules.GetControl(0), 0);
    m_nState = 2;
}

// FUNCTION: SWKOTOR 0x0067C4B0
void CSWGuiMainMenu::OnWarpButtonPressed(CSWGuiControl* pControl)
{
    ShowWarpList();
}

// FUNCTION: SWKOTOR 0x0067B9D0
void CSWGuiMainMenu::OnModuleSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
    CExoString sCommand;
    CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
    CSWPartyTable::ResetCurrentSessionStartTime();
    g_pAppManager->CreateServer(0);
    g_pAppManager->m_pClientExoApp->GetWorldTimer()->PauseWorldTimer();
    g_pAppManager->m_pServerExoApp->GetWorldTimer()->PauseWorldTimer();
    pApp->GetNetLayer()->StartConnectToSession(0, pApp->GetClientName(), "", 10, g_nConnectionType);
    pApp->GetMessage()->SendPlayerToServer_01_00();
    sCommand.Format("%c%s.%s %s", 's', "Module", "Load", m_lstModuleNames.element[m_lbModules.m_nSelected].CStr());
    pApp->GetNetLayer()->SendMessageToPlayer(PLAYERID_SERVER, (unsigned char*)sCommand.CStr(), sCommand.GetLength(), 0);
    {
        CExoString sName("test");
        pApp->SetPlayerCharacterName(sName);
    }
    pApp->SetInputClass(0, 1);
    pApp->DisableInput();
    pApp->SetLoadMusic(pApp->GetLoadMusicByModuleName(m_lstModuleNames.element[m_lbModules.m_nSelected]));
    pApp->SetLoadScreenByModuleName(m_lstModuleNames.element[m_lbModules.m_nSelected]);
    pApp->ShowLoadScreen(1, 1, 0);
    pApp->SetLoadScreenHint(pApp->GetNextLoadScreenHintSTRREF(m_lstModuleNames.element[m_lbModules.m_nSelected], 0));
    ClearModuleNames();
    m_lbModules.ClearControls();
    MarkForDelete();
    g_pAppManager->m_pClientExoApp->GetInGameGui()->AdjustHideCount(1);
    }
}

// FUNCTION: SWKOTOR 0x0067B6C0
void CSWGuiMainMenu::OnPanelAdded()
{
    unsigned __int64 nFreeSpace;
    {
        CExoString sSaves("SAVES:");
        g_pExoResMan->GetFreeDiskSpace(sSaves, &nFreeSpace);
    }
    __int64 nFreeBlocks = nFreeSpace >> 14;
    __int64 nRequired = 3200;
    CExoString sDir;
    sDir.Format("SAVES:%06d - %s", 1, "AUTOSAVE");
    int bCreated = !g_pExoResMan->CreateDirectory2(sDir);
    if (!bCreated) {
        nRequired = 4800;
        g_pExoResMan->Leaf9480(sDir, 1, 1);
    }
    if (nRequired + 2 > nFreeBlocks) {
        float fMegabytes = (float)(int)((nRequired + 2 - nFreeBlocks) * 16384) * 1e-6f;
        CSWGuiMessageBox* pMessageBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
        CExoString sMessage;
        CExoString sSize;
        sSize.Format("%2.3f", fMegabytes);
        g_pTlkTable->SetCustomToken(0, sSize);
        sMessage = g_pTlkTable->GetSimpleString(0xbb78);
        g_pTlkTable->ParseTokens(&sMessage);
        pMessageBox->SetAllowCancel(0);
        pMessageBox->SetMessage(sMessage);
        pMessageBox->SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiMainMenu::LaunchDashboard, 0);
        m_pManager->AddPanel(pMessageBox, 1, 1);
    }
    else {
        g_pAppManager->m_pClientExoApp->ShowMouse(0xffff);
    }
}

typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x0067C4C0
CSWGuiMainMenu::CSWGuiMainMenu(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    {
        CResRef cModule("MAINMENU");
        if (g_pExoResMan->Exists(cModule, 0xbba, 0)) {
            CExoString sRim("RIMS:MAINMENU");
            g_pExoResMan->AddResourceImageFile(sRim);
        }
    }
    g_pExoResMan->m_nFlags &= ~2;
    LoadFromLayout(pManager);
    m_btnWarp.SetVisible(0);
    m_lblNewContent.SetVisible(0);
    m_lbModules.SetVisible(0);

    m_btnNewGame.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnNewGamePicked);
    m_btnLoadGame.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnLoadSaveGame);
    m_btnMovies.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnMoviesPicked);
    m_btnOptions.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnOptionsPicked);
    m_btnWarp.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnWarpButtonPressed);
    m_btnExit.AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnQuitButtonPressed);
    m_btnNewGame.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnNewGamePicked);
    m_btnLoadGame.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnLoadSaveGame);
    m_btnMovies.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnMoviesPicked);
    m_btnOptions.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnOptionsPicked);
    m_btnWarp.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnWarpButtonPressed);
    m_btnExit.AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnQuitButtonPressed);
    m_btnNewGame.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnLoadGame.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnMovies.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnOptions.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnWarp.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnExit.AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnEnterButton);
    m_btnNewGame.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);
    m_btnLoadGame.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);
    m_btnMovies.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);
    m_btnOptions.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);
    m_btnWarp.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);
    m_btnExit.AddEvent(1, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnExitButton);

    CAurCamera* pCamera = m_lbl3DView.m_cScene.m_pCamera;
    CAurScene* pScene = m_lbl3DView.m_cScene.m_pScene;
    if (g_bDo3DGui) {
        pScene->SetRoom("gui3D_room", Vector(0.0f, 0.0f, 0.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        CAurObject* pModel = m_lbl3DView.m_cScene.AddModel("mainmenu", -1);
        if (pModel) {
            pModel->PlayAnimation("default", 1.0f, 0, 0);
            pModel->SetAnimationSpeed(1.0f);
            pCamera->AttachToHook(pModel, "camerahook", 0);
            pCamera->SetFieldOfView(22.726f);
        }
    }
    SetActiveControl(&m_btnNewGame, 0);
    m_nState = 1;
    {
        CExoString sFile("HD0:GAMEINPROGRESS");
        g_pExoResMan->RemoveFile(sFile, 0x56);
    }
    {
        CExoString sDir("GAMEINPROGRESS:");
        g_pExoResMan->Leaf9460(sDir, 0, 0);
    }
    {
        CExoString sFile("HD0:CURRENTGAME");
        g_pExoResMan->RemoveFile(sFile, 0x56);
    }
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->Leaf9460(sDir, 0, 0);
    }
    {
        CExoString sDir("REBOOTDATA:");
        g_pExoResMan->Leaf9460(sDir, 0, 0);
    }
    g_pExoSound->SetSoundMode(g_nSoundModeBack);
    m_bActive = 0;
    m_nDrawCount = 0;
}
