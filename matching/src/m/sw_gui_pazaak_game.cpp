// Pazaak game panel CSWGuiPazaakGame (vtable 0x00753358,
// 0x0067D4B0-0x0067FFA0); its game sequence, Draw and constructor follow at
// 0x00680030-0x006808A0.

#include "m/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x0067D4B0
void CSWGuiPazaakGame::RefreshDisplay()
{
    int bPlayerStanding = m_pPazaak->m_cPlayer.m_bStanding;
    int bNpcStanding = m_pPazaak->m_cAI.m_bStanding;
    int bPlayerChanged = 0;
    int bNpcChanged = 0;
    if (m_nState >= 10) {
        bPlayerStanding = 0;
        bNpcStanding = 0;
    }
    int i;
    for (i = 0; i < 9; i++) {
        if (m_aPlayerBoard[i].GetCard() != m_pPazaak->m_cPlayer.GetBoardCard(i)) {
            m_aPlayerBoard[i].SetCard(m_pPazaak->m_cPlayer.m_aBoard[i]);
            bPlayerChanged = 1;
        }
        m_aPlayerBoard[i].m_cBorder.m_cParams.m_vColor =
            bPlayerStanding ? Vector(0.667f, 0.667f, 0.667f) : Vector(1.0f, 1.0f, 1.0f);
        if (m_aNpcBoard[i].GetCard() != m_pPazaak->m_cAI.GetBoardCard(i)) {
            m_aNpcBoard[i].SetCard(m_pPazaak->m_cAI.m_aBoard[i]);
            bNpcChanged = 1;
        }
        m_aNpcBoard[i].m_cBorder.m_cParams.m_vColor =
            bNpcStanding ? Vector(0.667f, 0.667f, 0.667f) : Vector(1.0f, 1.0f, 1.0f);
    }
    int bAnyFlip = 0;
    int bAnyHand = 0;
    for (i = 0; i < 4; i++) {
        CPazaakCard* pCard = &m_pPazaak->m_cPlayer.m_aHand[i];
        int bFlip = pCard->m_nValue >= 12 && pCard->m_nValue < 18;
        bAnyFlip = bAnyFlip || bFlip;
        if (m_aPlayerHand[i].m_cCard.m_nValue != pCard->m_nValue ||
            m_aPlayerHand[i].m_cCard.m_bFlag != pCard->m_bFlag) {
            m_aPlayerHand[i].SetCard(*pCard);
            m_aFlip[i].SetEnabled(bFlip);
            m_aFlip[i].m_bFlag1 = bFlip;
        }
        m_aPlayerHand[i].m_cBorder.m_cParams.m_vColor =
            m_nState != 3 ? Vector(0.667f, 0.667f, 0.667f) : Vector(1.0f, 1.0f, 1.0f);
        if (m_aPlayerHand[i].m_cCard.m_nValue >= 0) {
            bAnyHand = 1;
        }
        CPazaakCard cNpc = m_pPazaak->m_cAI.m_aHand[i];
        if (!g_bPazaakShowOpponentHand) {
            if (cNpc.m_nValue >= 0) {
                cNpc.m_nValue = -2;
                cNpc.m_bFlag = 0;
            }
            if (m_aNpcHand[i].m_cCard.m_nValue != cNpc.m_nValue || m_aNpcHand[i].m_cCard.m_bFlag != cNpc.m_bFlag) {
                m_aNpcHand[i].SetCard(cNpc);
            }
        }
        else if (m_aNpcHand[i].m_cCard.m_nValue != cNpc.m_nValue ||
                 m_aNpcHand[i].m_cCard.m_bFlag != cNpc.m_bFlag) {
            m_aNpcHand[i].SetCard(m_pPazaak->m_cAI.m_aHand[i]);
        }
    }
    m_lblFlipIcon.m_bFlag1 = bAnyFlip;
    m_lblFlipLegend.m_bFlag1 = bAnyFlip;
    if (bPlayerChanged) {
        CExoString sTotal(m_pPazaak->m_cPlayer.GetTotal());
        m_lblPlayerTotal.m_cText.m_cParams.SetText(sTotal);
    }
    if (bNpcChanged) {
        CExoString sTotal(m_pPazaak->m_cAI.GetTotal());
        m_lblNpcTotal.m_cText.m_cParams.SetText(sTotal);
    }
    if (m_nState == 10) {
        int nSet = m_pPazaak->m_cPlayer.m_nSetsWon - 1;
        if (nSet >= 0 && nSet < 3) {
            CResRef cMark("lbl_winmark02");
            m_aPlayerScore[nSet].m_cBorder.m_cParams.SetFillImage(cMark, 0);
        }
        nSet = m_pPazaak->m_cAI.m_nSetsWon - 1;
        if (nSet >= 0 && nSet < 3) {
            CResRef cMark("lbl_winmark02");
            m_aNpcScore[nSet].m_cBorder.m_cParams.SetFillImage(cMark, 0);
        }
    }
    m_btnPlay.SetEnabled(m_nState == 3 && m_pActiveControl && m_nHandMode == 0 && bAnyHand);
    m_btnFlip.SetEnabled(bAnyHand && m_nHandMode == 0);
    int bTurn = !m_pPazaak->m_cPlayer.m_bStanding && (m_nState == 3 || m_nState == 4) && m_nHandMode == 0;
    m_btnEndTurn.SetEnabled(bTurn);
    m_btnStand.SetEnabled(bTurn);
}

// FUNCTION: SWKOTOR 0x0067DA30
void CSWGuiPazaakGame::ReconnectSidedeckButtons()
{
    int bActiveEmpty = 0;
    int nPrev = -1;
    int i;
    for (i = 3; i >= 0; i--) {
        m_aPlayerHand[i].SetNavigation(0, 0);
        m_aPlayerHand[i].SetNavigation(2, 0);
        if (m_aPlayerHand[i].GetCard().m_nValue >= 0) {
            nPrev = i;
        }
    }
    for (i = 3; i >= 0; i--) {
        if (m_aPlayerHand[i].GetCard().m_nValue < 0) {
            if (m_pActiveControl == &m_aPlayerHand[i]) {
                bActiveEmpty = 1;
            }
            continue;
        }
        if (nPrev >= 0) {
            m_aPlayerHand[i].SetNavigation(3, &m_aPlayerHand[nPrev]);
            m_aPlayerHand[nPrev].SetNavigation(1, &m_aPlayerHand[i]);
        }
        nPrev = i;
    }
    if (bActiveEmpty && m_nHandMode) {
        if (nPrev >= 0) {
            SetActiveControl(&m_aPlayerHand[nPrev], 0);
        }
        else {
            SetActiveControl(0, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0067DB50
void CSWGuiPazaakGame::HandleQuitDialog(CSWGuiControl* pControl)
{
    if (!g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox->m_bAccepted) {
        return;
    }
    MarkForDelete();
    g_pAppManager->m_pClientExoApp->EndPazaakGame(0, m_pPazaak->m_nWager, 0, m_pPazaak->m_oidOpponent);
}

// FUNCTION: SWKOTOR 0x0067DBB0
void CSWGuiPazaakGame::SetHandEnabled(int bEnabled)
{
    int nLast = -1;
    for (int i = 3; i >= 0; i--) {
        if (m_aPlayerHand[i].GetCard().m_nValue >= 0) {
            m_aPlayerHand[i].m_bCanSelect = bEnabled;
            nLast = i;
        }
        else {
            m_aPlayerHand[i].m_bCanSelect = 0;
        }
    }
    int bButtons = bEnabled == 0;
    m_btnEndTurn.SetEnabled(bButtons);
    m_btnStand.SetEnabled(bButtons);
    m_btnPlay.SetEnabled(bButtons);
    m_btnFlip.SetEnabled(bButtons);
    if (bEnabled) {
        if (nLast >= 0) {
            SetActiveControl(&m_aPlayerHand[nLast], 0);
        }
    }
    else {
        SetActiveControl(&m_btnEndTurn, 0);
    }
    RefreshDisplay();
}

// FUNCTION: SWKOTOR 0x0067DC90
void CSWGuiPazaakGame::HandleXButton(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    m_nHandMode = 1;
    SetHandEnabled(1);
}

// FUNCTION: SWKOTOR 0x0067DCC0
void CSWGuiPazaakGame::HandleBlackButton(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    m_nHandMode = 2;
    SetHandEnabled(1);
}

// FUNCTION: SWKOTOR 0x0067DCF0
void CSWGuiPazaakGame::HandleFlipHandCard()
{
    CSWGuiControl* pActive = m_pActiveControl;
    if (!pActive) {
        return;
    }
    int nId = pActive->m_nId;
    if (nId < 0 || nId >= 4 || pActive != &m_aPlayerHand[nId]) {
        return;
    }
    CPazaakCard cCard = m_pPazaak->m_cPlayer.m_aHand[nId];
    if (cCard.m_nValue < 12 || cCard.m_nValue >= 18) {
        return;
    }
    m_btnFlip.SetColor(g_vPazaakFlashColor);
    m_pPazaak->m_cPlayer.m_aHand[nId].m_bFlag = !m_pPazaak->m_cPlayer.m_aHand[nId].m_bFlag;
    m_pManager->PlayGuiSound(0);
    RefreshDisplay();
}

// FUNCTION: SWKOTOR 0x0067DD80
void CSWGuiPazaakGame::HandleHandCardRightClick(CSWGuiControl* pControl)
{
    if (!pControl) {
        return;
    }
    CSWGuiControl* pSaved = m_pActiveControl;
    m_pActiveControl = pControl;
    HandleFlipHandCard();
    m_pActiveControl = pSaved;
}

// FUNCTION: SWKOTOR 0x0067DDA0
void CSWGuiPazaakGame::HandleFlipButton(CSWGuiControl* pControl)
{
    int nId = pControl->m_nId;
    CSWGuiControl* pSaved = m_pActiveControl;
    m_pActiveControl = &m_aPlayerHand[nId];
    HandleFlipHandCard();
    m_pActiveControl = pSaved;
}

// FUNCTION: SWKOTOR 0x0067E540
CSWGuiPazaakGame::~CSWGuiPazaakGame()
{
    if (m_pPazaak) {
        delete m_pPazaak;
        m_pPazaak = 0;
    }
}

// FUNCTION: SWKOTOR 0x0067E770
void CSWGuiPazaakGame::StartNewGame()
{
    m_pPazaak->ClearGameBoard();
    m_nState = 0;
    RefreshDisplay();
    m_nTutorialShown &= ~0x10;
    if (!(m_nTutorialShown & 8)) {
        m_nTutorialShown |= 8;
        unsigned int aHelp[2];
        aHelp[0] = 0x96e7;
        aHelp[1] = 0x96e8;
        m_cTutorial.ShowMultipleHelp(aHelp, 2);
    }
    m_nHandMode = 0;
    SetHandEnabled(0);
    SetActiveControl(&m_btnEndTurn, 0);
}

// FUNCTION: SWKOTOR 0x0067E8F0
void CSWGuiPazaakGame::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || nEvent == 0x2e)) {
        if (m_nHandMode) {
            m_nHandMode = 0;
            SetHandEnabled(0);
        }
        else {
            m_pManager->PlayGuiSound(0);
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(1);
            pBox->SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))
                                             &CSWGuiPazaakGame::HandleQuitDialog, 0);
            pBox->SetMessageStrRef(0xa5b9);
            m_pManager->AddPanel(pBox, 1, 1);
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0067E9A0
void CSWGuiPazaakGame::HandleEndDialog(CSWGuiControl* pControl)
{
    if (m_pPazaak->m_cPlayer.m_nSetsWon < 3 && m_pPazaak->m_cAI.m_nSetsWon < 3) {
        m_cTutorial.ShowHelp(0x96f8, -1, 0x96e1);
        m_nState = 11;
        return;
    }
    m_cTutorial.ShowHelp(0x96f9, -1, 0x96e2);
    m_nState = 12;
}

// FUNCTION: SWKOTOR 0x0067EA10
void CSWGuiPazaakGame::HandleTutorialClose(CSWGuiControl* pControl)
{
    CSWGuiTutorial* pTutorial = &m_cTutorial;
    unsigned int nMessage;
    if (pTutorial->m_nMessage < pTutorial->m_lstMessages.num) {
        nMessage = pTutorial->m_lstMessages.element[pTutorial->m_nMessage];
    }
    else {
        nMessage = 0;
    }
    if (m_cTutorial.m_bAccepted) {
        switch (nMessage) {
        case 0x96e9: {
            unsigned int aHelp[5];
            aHelp[0] = 0x96ea;
            aHelp[1] = 0x96eb;
            aHelp[2] = 0x96ec;
            aHelp[3] = 0x96f3;
            aHelp[4] = 0x96f4;
            pTutorial->ShowMultipleHelp(aHelp, 5);
            pTutorial->PlaceAbove(&m_aPlayerHand[0]);
            break;
        }
        case 0x96ec:
        case 0x96f4: {
            unsigned int aHelp[3];
            aHelp[0] = 0x96ed;
            aHelp[1] = 0x96ee;
            aHelp[2] = 0x96ef;
            pTutorial->ShowMultipleHelp(aHelp, 3);
            pTutorial->PlaceAbove(&m_btnEndTurn);
            break;
        }
        case 0x96f0:
            m_pPazaak->m_cPlayer.m_bStanding = 1;
        case 0x96f1:
            m_nState = 5;
            m_fTimer = 0.4f;
            RefreshDisplay();
            break;
        case 0x96f2: {
            m_pPazaak->UsePlayerSidedeckCard(m_nPendingCard);
            m_nPendingCard = -1;
            CResRef cSound(m_aSounds[2]);
            g_pExoSound->PlayOneShotSound(cSound, 0xb, 0, 0, 0.0f, 0.0f);
            m_nState = 4;
            RefreshDisplay();
            ReconnectSidedeckButtons();
            break;
        }
        case 0x96f9: {
            MarkForDelete();
            CSWPazaak* pPazaak = m_pPazaak;
            g_pAppManager->m_pClientExoApp->EndPazaakGame(pPazaak->m_cPlayer.m_nSetsWon > pPazaak->m_cAI.m_nSetsWon,
                                                          pPazaak->m_nWager, 0, pPazaak->m_oidOpponent);
            break;
        }
        }
    }
    else if (nMessage >= 0x96f8 && nMessage <= 0x96f9) {
        m_nState = 12;
    }
}

// FUNCTION: SWKOTOR 0x0067EC20
void CSWGuiPazaakGame::HandleContinue(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    if (m_pPazaak->m_cPlayer.m_bStanding) {
        return;
    }
    if (m_nState != 3 && m_nState != 4) {
        return;
    }
    if (m_nHandMode) {
        return;
    }
    m_btnEndTurn.Flash(g_vPazaakFlashColor);
    m_pManager->PlayGuiSound(0);
    if (m_cTutorial.m_bEnabled && GetPazaak()->m_cPlayer.GetTotal() >= 16) {
        m_cTutorial.ShowHelp(0x96f1, 0x7e4c, 0x96e2);
        return;
    }
    m_nState = 5;
    m_fTimer = 0.4f;
    RefreshDisplay();
}

// FUNCTION: SWKOTOR 0x0067ED00
void CSWGuiPazaakGame::HandleStand(CSWGuiControl* pControl)
{
    if (pControl && !pControl->m_n4c) {
        return;
    }
    if (m_pPazaak->m_cPlayer.m_bStanding) {
        return;
    }
    if (m_nState != 3 && m_nState != 4) {
        return;
    }
    m_btnStand.Flash(g_vPazaakFlashColor);
    m_pManager->PlayGuiSound(0);
    if (m_cTutorial.m_bEnabled && GetPazaak()->m_cPlayer.GetTotal() <= 13) {
        m_cTutorial.ShowHelp(0x96f0, 0x7e4d, 0x96e2);
        return;
    }
    m_pPazaak->m_cPlayer.m_bStanding = 1;
    m_nState = 5;
    m_fTimer = 0.4f;
    RefreshDisplay();
}

// FUNCTION: SWKOTOR 0x0067EDE0
void CSWGuiPazaakGame::HandlePlayHandCard(int nIndex)
{
    if (m_nState != 3) {
        return;
    }
    if (nIndex < 0 || nIndex >= 4) {
        return;
    }
    if (!m_pPazaak->CanUsePlayerSidedeckCard(nIndex)) {
        return;
    }
    m_btnPlay.Flash(g_vPazaakFlashColor);
    if (m_cTutorial.m_bEnabled && GetPazaak()->m_cPlayer.GetTotal() <= 12) {
        m_nPendingCard = nIndex;
        m_pManager->PlayGuiSound(0);
        m_cTutorial.ShowHelp(0x96f2, 0x7e4a, 0x96e2);
        return;
    }
    m_pPazaak->UsePlayerSidedeckCard(nIndex);
    CResRef cSound(m_aSounds[2]);
    g_pExoSound->PlayOneShotSound(cSound, 0xb, 0, 0, 0.0f, 0.0f);
    m_nState = 4;
    m_fTimer = 0.4f;
    RefreshDisplay();
    ReconnectSidedeckButtons();
}

// FUNCTION: SWKOTOR 0x0067EF00
void CSWGuiPazaakGame::HandleHandCardDrop(CSWGuiControl* pControl)
{
    CSWGuiControl* pTarget = g_pGuiManager->m_pMouseOverControl;
    CSWGuiControl* pSaved = m_pActiveControl;
    if (!pTarget) {
        return;
    }
    if (pTarget == &m_btnFlip) {
        m_pActiveControl = pControl;
        HandleFlipHandCard();
        m_pActiveControl = pSaved;
        return;
    }
    if (m_nState != 3) {
        return;
    }
    int bHit = pTarget == &m_btnPlay;
    if (!bHit) {
        for (int i = 0; i < 9; i++) {
            if (pTarget == &m_aPlayerBoard[i]) {
                bHit = 1;
                break;
            }
        }
    }
    if (!bHit) {
        return;
    }
    HandlePlayHandCard(pControl->m_nId);
}

// FUNCTION: SWKOTOR 0x0067EF80
void CSWGuiPazaakGame::HandleHandCardDoubleClick(CSWGuiControl* pControl)
{
    if (!pControl || m_nState != 3) {
        return;
    }
    HandlePlayHandCard(pControl->m_nId);
}

// FUNCTION: SWKOTOR 0x0067FF80 SYMBOL: ??_GCSWGuiPazaakGame@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067FFA0
void CSWGuiPazaakGame::OnPanelAdded()
{
    StartNewGame();
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CSWSCreature* pCreature = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureId());
    m_lblPlayerName.m_cText.m_cParams.SetText(pCreature->GetStats()->GetFullName());
    ReconnectSidedeckButtons();
    CSWGuiPanel::OnPanelAdded();
}
