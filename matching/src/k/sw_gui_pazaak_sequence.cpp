// CSWGuiPazaakGame::DoGameSequence: the per-frame state machine of a pazaak
// match (turn hand-over, AI turns, set and match results).
#include "k/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x00680030
int CSWGuiPazaakGame::DoGameSequence()
{
    int bContinue = 0;
    switch (m_nState) {
    case 0:
        m_nState = 1;
        bContinue = 1;
        break;
    case 1:
        if (m_pPazaak->m_cPlayer.m_bStanding) {
            m_nState = 5;
            bContinue = 1;
            break;
        }
        m_lblPlayerTurn.SetFlag1(1);
        m_lblNpcTurn.SetFlag1(0);
        g_pExoSound->PlayOneShotSound(m_aSounds[0], 0xb, 0, 0, 0.0f, 0.0f);
        m_nState = 2;
        m_fTimer = 0.4f;
        break;
    case 2: {
        m_pPazaak->DrawPlayerCard();
        g_pExoSound->PlayOneShotSound(m_aSounds[1], 0xb, 0, 0, 0.0f, 0.0f);
        if (!(m_nTutorialShown & 2)) {
            m_nTutorialShown |= 2;
            m_cTutorial.ShowHelp(0x96e9, -1, 0x96e2);
            m_cTutorial.PlaceRightOf(&m_aPlayerBoard[0]);
        }
        if (m_pPazaak->m_cPlayer.m_bStanding) {
            m_btnStand.SetColor(g_vPazaakFlashColor);
            m_fTimer = 0.4f;
        }
        if (GetPazaak()->m_cPlayer.GetTotal() > 20) {
            g_pExoSound->PlayOneShotSound(m_aSounds[3], 0xb, 0, 0, 0.0f, 0.0f);
        }
        m_nState = 3;
        CSWGuiControl* pFocus = GetFocusControl();
        SetActiveControl(0, 0);
        SetActiveControl(pFocus, 0);
        RefreshDisplay();
        break;
    }
    case 3:
    case 4:
        if (m_pPazaak->m_cPlayer.m_bStanding) {
            m_nState = 5;
            bContinue = 1;
        }
        break;
    case 5: {
        int nTotal = GetPazaak()->m_cPlayer.GetTotal();
        if (nTotal > 20) {
            m_cTutorial.ShowHelp(0x96f7, -1, 0x96e2);
        }
        else if (nTotal == 20 && !(m_nTutorialShown & 0x10)) {
            m_nTutorialShown |= 0x10;
            m_cTutorial.ShowHelp(0x96f6, -1, 0x96e2);
        }
        if (m_pPazaak->DetermineWinner()) {
            m_nState = 8;
            bContinue = 1;
            break;
        }
        m_nState = 6;
    }
    case 6:
        if (m_pPazaak->m_cAI.m_bStanding) {
            m_nState = 8;
            bContinue = 1;
            break;
        }
        m_lblPlayerTurn.SetFlag1(0);
        m_lblNpcTurn.SetFlag1(1);
        g_pExoSound->PlayOneShotSound(m_aSounds[0], 0xb, 0, 0, 0.0f, 0.0f);
        m_fTimer = 0.4f;
        m_nState = 7;
        if (!(m_nTutorialShown & 4)) {
            m_nTutorialShown |= 4;
            m_cTutorial.ShowHelp(0x96f5, -1, 0x96e2);
        }
        break;
    case 7: {
        int nAction = m_pPazaak->DoAITurn();
        switch (nAction) {
        case 1: {
            g_pExoSound->PlayOneShotSound(m_aSounds[1], 0xb, 0, 0, 0.0f, 0.0f);
            if (GetPazaak()->m_cAI.GetTotal() > 20) {
        g_pExoSound->PlayOneShotSound(m_aSounds[3], 0xb, 0, 0, 0.0f, 0.0f);
            }
            break;
        }
        case 2: {
            m_btnPlay.SetColor(g_vAbilityColorAvailable);
            g_pExoSound->PlayOneShotSound(m_aSounds[2], 0xb, 0, 0, 0.0f, 0.0f);
            break;
        }
        case 3:
            m_btnStand.SetColor(g_vAbilityColorAvailable);
            m_pManager->PlayGuiSound(0);
            break;
        case 4:
            m_btnEndTurn.SetColor(g_vAbilityColorAvailable);
            m_pManager->PlayGuiSound(0);
            break;
        }
        if (nAction == 1 || nAction == 2) {
            m_fTimer = 0.8f;
            if (GetPazaak()->m_cAI.GetTotal() == 20 && !(m_nTutorialShown & 0x10)) {
                m_nTutorialShown |= 0x10;
                m_cTutorial.ShowHelp(0x96f6, -1, 0x96e2);
            }
        }
        if (nAction) {
            RefreshDisplay();
            break;
        }
        m_fTimer = 0.4f;
        m_nState = 8;
    }
    case 8: {
        int nWinner = m_pPazaak->DetermineWinner();
        if (nWinner) {
            m_cMessageBox.SetAllowCancel(0);
            if (nWinner == 1) {
                m_pPazaak->m_cPlayer.m_nSetsWon++;
                if (m_pPazaak->m_cPlayer.m_nSetsWon >= 3) {
                    m_cMessageBox.SetMessageStrRef(0x7e50);
                    m_szEndSound = "mgs_winmatch";
                }
                else {
                    m_cMessageBox.SetMessageStrRef(0x7e4e);
                    m_szEndSound = "mgs_winset";
                }
            }
            else if (nWinner == 2) {
                m_pPazaak->m_cAI.m_nSetsWon++;
                if (m_pPazaak->m_cAI.m_nSetsWon >= 3) {
                    m_cMessageBox.SetMessageStrRef(0x7e51);
                    m_szEndSound = "mgs_losematch";
                }
                else {
                    m_cMessageBox.SetMessageStrRef(0x7e4f);
                    m_szEndSound = "mgs_loseset";
                }
            }
            else {
                m_cMessageBox.SetMessageStrRef(0x7e52);
            }
            m_nState = 9;
        }
        else {
            m_nState = 1;
        }
        RefreshDisplay();
        break;
    }
    case 9:
        if (m_szEndSound) {
            g_pExoSound->PlayOneShotSound(m_szEndSound, 0xb, 0, 0, 0.0f, 0.0f);
            m_szEndSound = 0;
        }
        m_cMessageBox.SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))
                                       &CSWGuiPazaakGame::HandleEndDialog, 0);
        m_pManager->AddPanel(&m_cMessageBox, 1, 1);
        m_nState = 10;
        RefreshDisplay();
        break;
    case 10:
        break;
    case 11:
        StartNewGame();
        break;
    case 12: {
        MarkForDelete();
        CSWPazaak* pPazaak = m_pPazaak;
        g_pAppManager->GetClientExoApp()->EndPazaakGame(
            pPazaak->m_cPlayer.m_nSetsWon > pPazaak->m_cAI.m_nSetsWon, pPazaak->m_nWager, 0,
            pPazaak->m_oidOpponent);
        break;
    }
    }
    return bContinue;
}
