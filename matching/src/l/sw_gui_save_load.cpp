// Save/load screen (swguisaveload.o): list entry helpers, the save-name
// dialog callbacks and the save/load panel's prompts and callbacks.
#include "l/sw_gui_save_load.h"

void PlayDelayedSound(float fDelay, int a, int b); // 0x00401C10

// FUNCTION: SWKOTOR 0x006C8250
CExoString CSWGuiSaveLoadEntry::GetGameDirectory()
{
    CExoString sDirectory;
    sDirectory.Format("SAVES:%06d - %s", m_nGameNumber, m_sGameName);
    return sDirectory;
}

static char s_szEditChar[2];

// FUNCTION: SWKOTOR 0x006C82F0
void CSWGuiSaveGameEditBox::HandleChar(int nChar)
{
    switch (nChar) {
    case 8:
    case 0x7f:
        m_cText.RemoveLastChar();
        return;
    case 0x1b:
        if (m_pOwner) {
            CSWGuiPanel* pPanel = ((CSWGuiControl*)m_pOwner)->GetPanel();
            if (pPanel) {
                pPanel->HandleInputEvent(0x28, 1);
            }
        }
        return;
    case 0xa:
    case 0xd:
        if (m_pOwner) {
            CSWGuiPanel* pPanel = ((CSWGuiControl*)m_pOwner)->GetPanel();
            if (pPanel) {
                pPanel->HandleInputEvent(0x27, 1);
            }
        }
        return;
    }
    if (nChar >= 0x20) {
        s_szEditChar[0] = (char)nChar;
        s_szEditChar[1] = 0;
        m_cText.AddNewChar(s_szEditChar);
    }
}

// FUNCTION: SWKOTOR 0x006C8410
void CSWGuiSaveNamePanel::OnPanelAdded()
{
    m_ebName.SetFocus();
    CSWGuiPanel::OnPanelAdded();
    CSWGuiExtent extent = m_extent;
    g_pGuiManager->SaveMousePosition();
    g_pGuiManager->MoveMouseToPosition(extent.left, extent.top);
}

// FUNCTION: SWKOTOR 0x006C8460
void CSWGuiSaveNamePanel::OnPanelRemoved()
{
    CSWGuiPanel::OnPanelRemoved();
    g_pGuiManager->RestoreOldMousePosition();
}

// FUNCTION: SWKOTOR 0x006C8470
void CSWGuiSaveNamePanel::HandleCancelButton(CSWGuiControl*)
{
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    m_pManager->PopModalPanel();
}

// FUNCTION: SWKOTOR 0x006C86D0
void CSWGuiSaveLoad::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || (nEvent > 0x2c && nEvent <= 0x2e))) {
        m_pManager->PlayGuiSound(0);
        m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        if (m_bFromMainMenu) {
            g_pAppManager->GetClientExoApp()->DisplayMainMenu(0);
        } else {
            m_pManager->PopModalPanel();
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006C8750
int CSWGuiSaveLoad::LoadBarCallback(int nCurrent, int nTotal, int nBase, int nRange)
{
    g_pAppManager->GetClientExoApp()->SetLoadBarProgress(
        nBase + (int)((float)nCurrent / nTotal * nRange), 1);
    PlayDelayedSound(0.033333335f, 0, 0);
    return 0;
}

// FUNCTION: SWKOTOR 0x006C8790
void CSWGuiSaveLoad::WriteGame(const CExoString& sName)
{
    CSWGuiSaveLoadEntry* pEntry = (CSWGuiSaveLoadEntry*)m_lbGames.GetSelectedControl();
    unsigned long nGame = pEntry->m_nGameNumber;
    if (nGame > 999) {
        if (m_nNextGameNumber > 999) {
            CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
            CExoString sMessage = g_pTlkTable->GetSimpleString(0xa5fa);
            g_pTlkTable->ParseTokens(&sMessage);
            pBox->SetAllowCancel(0);
            pBox->SetMessage(sMessage);
            m_pManager->AddPanel(pBox, 1, 1);
            return;
        }
        pEntry->m_nGameNumber = m_nNextGameNumber;
    }
    if (pEntry->m_bSavedGame) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        if (!pBox->m_bAccepted) {
            return;
        }
    }
    CExoString sGameName;
    sGameName.Format("Game%d", pEntry->m_nGameNumber - 1);
    g_pAppManager->GetClientExoApp()->SendSaveGameRequest(pEntry->m_nGameNumber, sGameName, sName);
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    m_pManager->PopModalPanel();
}

// FUNCTION: SWKOTOR 0x006C8900
void CSWGuiSaveLoad::DeleteGame(CSWGuiControl*)
{
    CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
    if (!pBox->m_bAccepted) {
        return;
    }
    CSWGuiSaveLoadEntry* pEntry = (CSWGuiSaveLoadEntry*)m_lbGames.GetSelectedControl();
    if (!pEntry) {
        return;
    }
    g_pExoResMan->NukeDirectory(pEntry->GetGameDirectory(), 1, 1);
    int nSelected = m_lbGames.m_nSelected;
    PopulateGameList();
    int nLast = m_lbGames.m_nControls - 1;
    m_lbGames.SetSelectedControl(nSelected < nLast ? nSelected : nLast, 0);
    SetActiveControl(0, 0);
    SetActiveControl(&m_lbGames, 0);
}

// FUNCTION: SWKOTOR 0x006CAA90
void CSWGuiSaveLoad::PromptDeleteGame(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(1);
    pBox->SetMessageStrRef(0x638);
    pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::DeleteGame, (int)pControl);
    m_pManager->AddPanel(pBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x006CB040
void CSWGuiSaveNamePanel::HandleInputEvent(int nEvent, int)
{
    switch (nEvent) {
    case 0x27:
    case 0x2d:
        HandleOkButton(0);
        return;
    case 0x28:
    case 0x2e:
        m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        m_pManager->PopModalPanel();
        break;
    }
}

// FUNCTION: SWKOTOR 0x006CB820
void CSWGuiSaveLoad::PromptForSaveName(CSWGuiControl* pControl)
{
    CExoString sName;
    if (!pControl) {
        return;
    }
    CSWGuiSaveLoadEntry* pEntry = (CSWGuiSaveLoadEntry*)pControl;
    if (pEntry->m_bSavedGame) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        if (!pBox->m_bAccepted) {
            return;
        }
        sName = pEntry->m_sSaveName;
    } else {
        sName = CExoString("");
    }
    CSWGuiSaveNamePanel* pPanel = new CSWGuiSaveNamePanel(m_pManager);
    pPanel->m_pSaveLoad = this;
    pPanel->m_ebName.m_cText.SetText(sName);
    g_pGuiManager->AddPanel(pPanel, 1, 1);
}

// FUNCTION: SWKOTOR 0x006CC0E0
void CSWGuiSaveLoad::VerifyLoadGame(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    CSWGuiControl* pEntry = m_lbGames.GetSelectedControl();
    if (!pEntry) {
        return;
    }
    if (m_bFromMainMenu) {
        LoadGame(pEntry);
        return;
    }
    CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(1);
    pBox->SetMessageStrRef(0x7d9b);
    pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::LoadGame, (int)pEntry);
    m_pManager->AddPanel(pBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x006C9A90
int CSWGuiSaveLoad::ExtractSaveGame(int nGameNumber, const CExoString& sName)
{
    CExoString sDirectory;
    CExoString sGame;
    CExoString sFile;
    CExoString sUnused;
    if (nGameNumber != -1) {
        sGame.Format("%06d - %s", nGameNumber, sName.CStr());
    } else {
        sGame = sName;
    }
    sDirectory = "SAVES:";
    sDirectory = sDirectory + sGame;
    g_pExoResMan->CreateDirectory2("FUTUREGAME:");
    g_pExoResMan->CleanDirectory("FUTUREGAME:", 0, 0);
    sFile = sDirectory + "\\" + "SAVEGAME";
    CERFFile* pERF = new CERFFile;
    pERF->ReadHeader(sFile);
    pERF->Read(sFile);
    pERF->ExportFilesFromERF(sFile, "FUTUREGAME:", -1, &CSWGuiSaveLoad::LoadBarCallback, 0,
                             g_pAppManager->GetClientExoApp()->GetTutorialFlag(0));
    delete pERF;
    return 1;
}

// FUNCTION: SWKOTOR 0x006CAAF0
int CSWGuiSaveLoad::PrepareGameInProgress(int nGameNumber, const CExoString& sName, int)
{
    if (ExtractSaveGame(nGameNumber, sName)) {
        CExoString sInProgress = g_pExoBase->m_pcExoAliasList->ResolveFileName("GAMEINPROGRESS:", 0xffff);
        CExoString sFuture = g_pExoBase->m_pcExoAliasList->ResolveFileName("FUTUREGAME:", 0xffff);
        g_pExoResMan->NukeDirectory("GAMEINPROGRESS:", 1, 1);
        sFuture = sFuture.Left(sFuture.GetLength() - 1);
        sInProgress = sInProgress.Left(sInProgress.GetLength() - 1);
        rename(sFuture.CStr(), sInProgress.CStr());
        return 1;
    }
    CExoString sGame;
    CExoString sDirectory;
    if (nGameNumber != -1) {
        sGame.Format("%06d - %s", nGameNumber, sName.CStr());
    } else {
        sGame = sName;
    }
    sDirectory = "SAVES:";
    sDirectory = sDirectory + sGame;
    g_pAppManager->GetClientExoApp()->HideLoadScreen(0, 1);
    CExoFile cFile(sDirectory + "\\" + "CORRUPT", 0, "wb+");
    cFile.Write("CORRUPT");
    cFile.Flush();
    return 0;
}
