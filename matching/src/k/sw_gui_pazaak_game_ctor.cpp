// CSWGuiPazaakGame constructor (pazaak board panel, 0x86E8 bytes, vtable
// 0x00753358).
#include <stdio.h>

#include "k/sw_gui_pazaak.h"

typedef void (CSWGuiPanel::*CSWGuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x006808A0
CSWGuiPazaakGame::CSWGuiPazaakGame(CSWGuiManager* pManager, CSWPazaak* pPazaak, int bTutorial,
                                   unsigned long oidOpponent)
    : CSWGuiPanel(pManager), m_cMessageBox(pManager), m_cTutorial(pManager)
{
    m_pPazaak = pPazaak;
    m_aSounds[0] = "mgs_startturn";
    m_aSounds[1] = "mgs_drawmain";
    m_aSounds[2] = "mgs_playside";
    m_aSounds[3] = "mgs_warnbust";
    m_nState = 0;
    m_fTimer = -1.0f;
    m_nTutorialShown = 0;
    m_nPendingCard = -1;
    m_szEndSound = 0;
    m_nHandMode = 0;
    m_cTutorial.SetCloseCallback(this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleTutorialClose, 0);
    m_cTutorial.m_bEnabled = bTutorial;
    m_cMessageBox.SetAllowCancel(0);
    {
        CResRef cLayout("pazaakgame");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_PLRNAME");
        InitControl(&m_lblPlayerName, sName, 1);
    }
    {
        CExoString sName("LBL_NPCNAME");
        InitControl(&m_lblNpcName, sName, 1);
    }
    {
        CExoString sName("LBL_PLRTOTAL");
        InitControl(&m_lblPlayerTotal, sName, 1);
    }
    {
        CExoString sName("LBL_NPCTOTAL");
        InitControl(&m_lblNpcTotal, sName, 1);
    }
    {
        CExoString sName("LBL_PLRSIDEDECK");
        InitControl(&m_lblPlayerSideDeck, sName, 1);
    }
    {
        CExoString sName("LBL_NPCSIDEDECK");
        InitControl(&m_lblNpcSideDeck, sName, 1);
    }
    {
        CExoString sName("LBL_PLRTURN");
        InitControl(&m_lblPlayerTurn, sName, 1);
    }
    {
        CExoString sName("LBL_NPCTURN");
        InitControl(&m_lblNpcTurn, sName, 1);
    }
    m_lblPlayerTurn.SetFlag1(0);
    m_lblNpcTurn.SetFlag1(0);
    {
        CExoString sName("LBL_PLRSCORE0");
        InitControl(&m_aPlayerScore[0], sName, 1);
    }
    {
        CExoString sName("LBL_PLRSCORE1");
        InitControl(&m_aPlayerScore[1], sName, 1);
    }
    {
        CExoString sName("LBL_PLRSCORE2");
        InitControl(&m_aPlayerScore[2], sName, 1);
    }
    {
        CExoString sName("LBL_NPCSCORE0");
        InitControl(&m_aNpcScore[0], sName, 1);
    }
    {
        CExoString sName("LBL_NPCSCORE1");
        InitControl(&m_aNpcScore[1], sName, 1);
    }
    {
        CExoString sName("LBL_NPCSCORE2");
        InitControl(&m_aNpcScore[2], sName, 1);
    }
    {
        CExoString sName("LBL_FLIPICON");
        InitControl(&m_lblFlipIcon, sName, 1);
    }
    {
        CExoString sName("LBL_FLIPLEGEND");
        InitControl(&m_lblFlipLegend, sName, 1);
    }
    int i;
    {
        char szPlayerButton[16] = "BTN_PLRx       ";
        char szNpcButton[16] = "BTN_NPCx       ";
        char szPlayerLabel[16] = "LBL_PLRx       ";
        char szNpcLabel[16] = "LBL_NPCx       ";
        for (i = 0; i < 9; i++) {
            sprintf(&szPlayerButton[7], "%d", i);
            sprintf(&szNpcButton[7], "%d", i);
            sprintf(&szPlayerLabel[7], "%d", i);
            sprintf(&szNpcLabel[7], "%d", i);
            InitControl(&m_aPlayerBoard[i], szPlayerButton, 1);
            InitControl(&m_aPlayerBoard[i].m_lblFace, szPlayerLabel, 1);
            InitControl(&m_aNpcBoard[i], szNpcButton, 1);
            InitControl(&m_aNpcBoard[i].m_lblFace, szNpcLabel, 1);
            m_aNpcBoard[i].SetFlag5(1);
            m_aPlayerBoard[i].SetEnabled(0);
            m_aPlayerBoard[i].m_bDraggable = 0;
        }
    }
    {
        char szPlayerSide[16] = "BTN_PLRSIDEx   ";
        char szNpcSide[16] = "BTN_NPCSIDEx   ";
        char szPlayerSideLabel[16] = "LBL_PLRSIDEx   ";
        char szNpcSideLabel[16] = "LBL_NPCSIDEx   ";
        char szFlip[16] = "BTN_FLIPx      ";
        for (i = 0; i < 4; i++) {
            sprintf(&szPlayerSide[11], "%d", i);
            sprintf(&szNpcSide[11], "%d", i);
            sprintf(&szPlayerSideLabel[11], "%d", i);
            sprintf(&szNpcSideLabel[11], "%d", i);
            sprintf(&szFlip[8], "%d", i);
            InitControl(&m_aPlayerHand[i], szPlayerSide, 1);
            InitControl(&m_aPlayerHand[i].m_lblFace, szPlayerSideLabel, 1);
            InitControl(&m_aFlip[i], szFlip, 1);
            m_aPlayerHand[i].m_nId = i;
            m_aFlip[i].m_nId = i;
            InitControl(&m_aNpcHand[i], szNpcSide, 1);
            InitControl(&m_aNpcHand[i].m_lblFace, szNpcSideLabel, 1);
            m_aPlayerHand[i].AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleHandCardAction);
            m_aPlayerHand[i].AddEvent(0x17feb, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleHandCardDrop);
            m_aPlayerHand[i].AddEvent(0x1f9, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleHandCardDoubleClick);
            m_aPlayerHand[i].AddEvent(0x44, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleHandCardRightClick);
            m_aNpcHand[i].SetFlag5(1);
            m_aFlip[i].AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleFlipButton);
        }
    }
    {
        CExoString sName("BTN_XTEXT");
        InitControl(&m_btnEndTurn, sName, 1);
    }
    {
        CExoString sName("BTN_YTEXT");
        InitControl(&m_btnStand, sName, 1);
    }
    UnloadResource();
    m_btnEndTurn.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleContinue);
    m_btnStand.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleStand);
    m_btnPlay.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleXButton);
    m_btnFlip.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiPazaakGame::HandleBlackButton);
    CSWSCreature* pOpponent = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidOpponent);
    if (pOpponent) {
        m_lblNpcName.m_cText.m_cParams.SetText(pOpponent->GetStats()->GetFullName());
    }
    if (m_btnEndTurn.m_cText.GetHeight() > m_btnEndTurn.m_extent.height) {
        m_btnEndTurn.m_cText.m_cParams.SetAlignment(10);
    }
    if (m_btnStand.m_cText.GetHeight() > m_btnStand.m_extent.height) {
        m_btnStand.m_cText.m_cParams.SetAlignment(10);
    }
    m_nResolution = 2;
}
