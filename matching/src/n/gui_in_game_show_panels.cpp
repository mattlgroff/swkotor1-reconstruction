// CGuiInGame modal panel openers: galaxy map, store and upgrade screen
// (guiingame.o).
#include "n/gui_in_game_dialog.h"

// FUNCTION: SWKOTOR 0x0062D040
void CGuiInGame::ShowGalaxyMapGui(int nPlanet)
{
    if (!m_bInitialized || m_bShowPanels)
        return;
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_nGlobalDialogState)
        return;
    if (!g_pAppManager->m_pClientExoApp->GetPausedByCombat())
        g_pAppManager->m_pClientExoApp->SetPausedByCombat(1, 0, 0);
    g_pAppManager->m_pClientExoApp->IndicateObject(0, 0);
    g_pExoSound->SetSoundMode(g_n74C5FC);
    SetSWGuiStatus(3, 1);
    {
        CResRef rSound("");
        CExoString sText("");
        m_pBarkBubble->SetBark(0x7F000000, sText, rSound, 0);
    }
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    if (m_pGalaxyMap == 0)
        m_pGalaxyMap = new CSWGuiInGameGalaxyMap(m_pGuiManager);
    g_pAppManager->GetServerExoApp()->GetPartyTable()->SetSelectedPlanet(nPlanet);
    m_pGuiManager->AddPanel(m_pGalaxyMap, 2, 1);
    m_pGuiManager->PlayGuiSound(4);
}

// FUNCTION: SWKOTOR 0x0062E310
void CGuiInGame::ShowStoreGui(unsigned long oidStore, unsigned long oidCustomer)
{
    if (!m_bInitialized || m_bShowPanels || m_nGlobalDialogState)
        return;
    if (!g_pAppManager->m_pClientExoApp->GetPausedByCombat())
        g_pAppManager->m_pClientExoApp->SetPausedByCombat(1, 0, 0);
    g_pAppManager->m_pClientExoApp->IndicateObject(0, 0);
    SetSWGuiStatus(3, 1);
    g_pExoSound->SetSoundMode(g_n74C5FC);
    {
        CResRef rSound("");
        CExoString sText("");
        m_pBarkBubble->SetBark(0x7F000000, sText, rSound, 0);
    }
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    if (m_pStore == 0)
        m_pStore = new CSWGuiStore(m_pGuiManager);
    m_pStore->m_oidStore = oidStore;
    m_pStore->m_oidCustomer = oidCustomer;
    m_pGuiManager->AddPanel(m_pStore, 3, 1);
    m_pGuiManager->PlayGuiSound(4);
}

// FUNCTION: SWKOTOR 0x0062E760
void CGuiInGame::ShowUpgradeScreen(unsigned long oidItem)
{
    if (!m_bInitialized || !m_pGuiManager || m_pUpgrade)
        return;
    if (oidItem != 0x7F000000 && !g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(oidItem))
        return;
    g_pExoSound->SetSoundMode(g_n74C5FC);
    if (!m_bPaused)
        g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServerInput_TogglePauseRequest();
    m_pUpgrade = new CSWGuiUpgradeSelection(m_pGuiManager);
    m_pUpgrade->m_oidItem = oidItem;
    m_pGuiManager->AddPanel(m_pUpgrade, 3, 1);
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
}

// FUNCTION: SWKOTOR 0x0062DC00
int CGuiInGame::ShowLevelUpGUI(int bFlag)
{
    if (!m_bInitialized)
        return 0;
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bLevelUpAvailable)
        return 0;
    if (bFlag) {
        if (!m_bPaused)
            g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServerInput_TogglePauseRequest();
        g_pExoSound->SetSoundMode(g_n74C5FC);
    }
    else {
        if (g_pAppManager->m_pClientExoApp->GetCharacterChangeInProgress())
            return 0;
    }
    if (m_pCharacter == 0)
        m_pCharacter = new CSWGuiInGameCharacter(m_pGuiManager);
    m_pCharacter->ShowLevelUpGUI(bFlag);
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    return 1;
}

// FUNCTION: SWKOTOR 0x0062DD20
void CGuiInGame::ShowPartySelection(const CExoString& sExitScript, int bFromScript, int nForced1, int nForced2)
{
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    if (!m_bInitialized)
        return;
    if (pServer->GetPartyTable()->m_nControlledNPC != -1)
        return;
    if (!pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId()))
        return;
    if (!pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId())->GetArea())
        return;
    if (pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId())->GetArea()->m_bUnescapable && !bFromScript)
        return;
    if (m_pGuiManager->PanelExists(m_pPartySelection))
        return;
    if (bFromScript) {
        if (!m_bPaused)
            g_pAppManager->m_pClientExoApp->GetSWCMessage()->SendPlayerToServerInput_TogglePauseRequest();
        m_pGuiManager->PlayGuiSound(4);
        g_pExoSound->SetSoundMode(g_n74C5FC);
    }
    if (m_pPartySelection == 0)
        m_pPartySelection = new CSWGuiPartySelection(m_pGuiManager);
    m_pPartySelection->m_sExitScript = sExitScript;
    m_pPartySelection->m_bFromScript = bFromScript;
    m_pPartySelection->SetForcedNPC(nForced1, nForced2);
    m_pPartySelection->m_nMode = bFromScript ? 0 : 2;
    m_pGuiManager->AddPanel(m_pPartySelection, 3, 1);
    g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
}
