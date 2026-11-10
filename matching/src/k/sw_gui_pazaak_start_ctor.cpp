// CSWGuiPazaakStart constructor (sidedeck selection panel, 0x8928 bytes,
// vtable 0x007532E8).
#include <stdio.h>

#include "k/sw_gui_pazaak.h"

typedef void (CSWGuiPanel::*CSWGuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x00681A90
CSWGuiPazaakStart::CSWGuiPazaakStart(CSWGuiManager* pManager, int nWager, int nDeck, int bTutorial,
                                     unsigned long oidOpponent)
    : CSWGuiPanel(pManager), m_cMessageBox(pManager), m_cTutorial(pManager)
{
    m_pWagerPopup = 0;
    m_bAllChosen = 0;
    m_pLastChosen = m_aChosen;
    m_pLastAvail = m_aAvail;
    int i;
    for (i = 0; i < 18; i++) {
        m_aAvailCount[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        m_aChosenCards[i] = CPazaakCard(-1, 0);
    }
    m_cTutorial.m_bEnabled = bTutorial;
    m_pPazaak = new CSWPazaak;
    m_pPazaak->m_nWager = nWager;
    m_pPazaak->m_oidOpponent = oidOpponent;
    if (nWager > 0) {
        GetPazaak()->m_nWager = 1;
        m_pWagerPopup = new CSWGuiWagerPopup(pManager, this, nWager);
    }
    CPazaakCard aCards[10];
    m_pPazaak->LoadInitialSideDeck(nDeck, aCards);
    GetPazaak()->m_cAI.ChooseSidedeck(aCards);
    {
        CResRef cLayout("pazaaksetup");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_TITLE");
        InitControl(&m_lblTitle, sName, 1);
    }
    {
        CExoString sName("BTN_ATEXT");
        InitControl(&m_btnAccept, sName, 1);
    }
    {
        CExoString sName("LBL_LTEXT");
        InitControl(&m_lblLText, sName, 1);
    }
    {
        CExoString sName("LBL_RTEXT");
        InitControl(&m_lblRText, sName, 1);
    }
    char szButton[16] = "BTN_CHOSENx    ";
    char szLabel[16] = "LBL_CHOSENx    ";
    for (i = 0; i < 10; i++) {
        sprintf(&szButton[10], "%d", i);
        sprintf(&szLabel[10], "%d", i);
        InitControl(&m_aChosen[i], szButton, 1);
        InitControl(&m_aChosen[i].m_lblFace, szLabel, 1);
        m_aChosen[i].m_nId = i;
        m_aChosen[i].AddEvent(0, this, (CSWGuiCallback)&CSWGuiPazaakStart::OnChosenCardEntered);
        m_aChosen[i].AddEvent(0x17feb, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleChosenCardDrop);
        m_aChosen[i].AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleMoveCard);
    }
    char szAvail[16] = "BTN_AVAILxx    ";
    char szAvailLabel[16] = "LBL_AVAILxx    ";
    char szAvailNum[16] = "LBL_AVAILNUMxx ";
    for (i = 0; i < 18; i++) {
        int nRow = i / 6;
        int nColumn = i % 6;
        sprintf(&szAvail[9], "%d%d", nRow, nColumn);
        sprintf(&szAvailLabel[9], "%d%d", nRow, nColumn);
        sprintf(&szAvailNum[12], "%d%d", nRow, nColumn);
        InitControl(&m_aAvail[i], szAvail, 1);
        InitControl(&m_aAvail[i].m_lblFace, szAvailLabel, 1);
        InitControl(&m_aAvailNum[i], szAvailNum, 1);
        m_aAvail[i].m_nId = i;
        m_aAvail[i].AddEvent(0, this, (CSWGuiCallback)&CSWGuiPazaakStart::OnChosenCardEntered);
        m_aAvail[i].AddEvent(0x17feb, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleAvailCardDrop);
        m_aAvail[i].AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleMoveCard);
        m_aAvailNum[i].SetFlag5(1);
    }
    UnloadResource();
    m_btnSwitch.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleMoveCard);
    m_btnAccept.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakStart::HandleAcceptButton);
    m_btnSwitch.SetNavigable(0);
    m_nResolution = 2;
}
