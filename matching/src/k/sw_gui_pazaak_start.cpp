// CSWGuiPazaakStart side-deck selection (swguipazaak.o): moving a card
// between the available and chosen lists, input events and the start
// dialog callback.
#include "k/sw_gui_pazaak.h"

// FUNCTION: SWKOTOR 0x006807E0
void CSWGuiPazaakStart::HandleMoveCard(CSWGuiControl* pControl)
{
    if (!pControl || !pControl->m_n4c) {
        return;
    }
    int nId = pControl->m_nId;
    m_pManager->PlayGuiSound(0);
    CSWGuiControl* pActive = m_pActiveControl;
    int nResult = 0;
    if (IsChosenCard(pActive, pActive->m_nId)) {
        if (&m_aChosen[nId] == pActive) {
            nResult = RemoveChosenCard(nId);
        }
    }
    else if (IsOtherCardActiveControl() && &m_aAvail[nId] == pActive) {
        nResult = AddChosenCard(nId, -1);
    }
    if (nResult) {
        m_pManager->PlayGuiSound(0);
    }
    ShowDisabledButtons();
}

// FUNCTION: SWKOTOR 0x006816F0
void CSWGuiPazaakStart::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        int nCode = nEvent;
        if (nEvent == 0xba) {
            if (IsChosenCard(m_pActiveControl, m_pActiveControl->m_nId)) {
                nCode = 0x35;
            }
            else {
                nCode = 0x36;
            }
        }
        switch (nCode) {
        case 0x28:
        case 0x2e: {
            m_pManager->PlayGuiSound(0);
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(1);
            pBox->SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))
                                             &CSWGuiPazaakStart::HandleQuitDialog, 0);
            pBox->SetMessageStrRef(0xa5b8);
            m_pManager->AddPanel(pBox, 1, 1);
            break;
        }
        case 0x2a:
            HandleMoveCard(m_pActiveControl);
            break;
        case 0x35:
            if (IsSidedeckCardActiveControl()) {
                m_pLastChosen = m_pActiveControl;
            }
            if (!IsOtherCardActiveControl()) {
                SetActiveControl(m_pLastAvail, 0);
                m_btnSwitch.m_cText.m_cParams.SetStrRef(0x96c9);
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x36:
            if (IsOtherCardActiveControl()) {
                m_pLastAvail = m_pActiveControl;
            }
            if (!IsSidedeckCardActiveControl()) {
                SetActiveControl(m_pLastChosen, 0);
                m_btnSwitch.m_cText.m_cParams.SetStrRef(0x96ca);
                m_pManager->PlayGuiSound(1);
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// NONMATCH (9): register/schedule ties only; the original loads the pazaak
// pointer into EAX for ChooseSidedeck and the tutorial flag into EDI for the
// game constructor. Member, accessor and local-pointer shapes do not move it.
// FUNCTION: SWKOTOR 0x00681890
void CSWGuiPazaakStart::HandleStartDialog(CSWGuiControl* pControl)
{
    if (!m_cMessageBox.m_bAccepted) {
        return;
    }
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    for (int i = 0; i < 10; i++) {
        pTable->m_aPazaakSideList[i] = m_aChosenCards[i].m_nValue;
    }
    m_pPazaak->GetPlayer()->ChooseSidedeck(m_aChosenCards);
    m_pManager->PopModalPanel();
    MarkForDelete();
    CSWGuiPazaakGame* pGame =
        new CSWGuiPazaakGame(m_pManager, m_pPazaak, m_cTutorial.m_bEnabled, m_pPazaak->m_oidOpponent);
    m_pPazaak = 0;
    m_pManager->AddPanel(pGame, 3, 1);
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x006819E0
void CSWGuiPazaakStart::HandleAcceptButton(CSWGuiControl* pControl)
{
    if (!pControl || !pControl->m_n4c) {
        return;
    }
    m_pManager->PlayGuiSound(0);
    if (m_bAllChosen) {
        m_cMessageBox.SetMessageStrRef(0x7e42);
        m_cMessageBox.SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))
                                                 &CSWGuiPazaakStart::HandleStartDialog, 0);
        m_pManager->AddPanel(&m_cMessageBox, 1, 1);
        return;
    }
    CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
    pBox->SetAllowCancel(0);
    pBox->SetCloseCallback(0, 0, 0);
    pBox->SetMessageStrRef(0x96e6);
    m_pManager->AddPanel(pBox, 1, 1);
}
