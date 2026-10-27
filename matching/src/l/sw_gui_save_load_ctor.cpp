// Save/load screen constructors, the panel destructor and the scalar
// deleting destructors emitted with their vtables.
#define L_RADIO_BUTTON_INLINE
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006CB0A0 SYMBOL: ??_GCSWGuiSaveGameEditBox@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x006CB0C0 SYMBOL: ??_GCSWGuiSaveLoad@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x006CBA70 SYMBOL: ??_GCSWGuiSaveLoadEntry@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x006CC040 SYMBOL: ??_GCSWGuiSaveNamePanel@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006CA110
CSWGuiSaveLoad::~CSWGuiSaveLoad()
{
    {
        CExoString sAlias("TEMP:");
        g_pExoResMan->RemoveResourceDirectory(sAlias);
    }
    if (!m_sTempDirectory.IsEmpty()) {
        g_pExoResMan->RemoveResourceDirectory(m_sTempDirectory);
    }
}

// FUNCTION: SWKOTOR 0x006CAE70
CSWGuiSaveNamePanel::CSWGuiSaveNamePanel(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    CSWGuiButton* pOk = &m_btnOk;
    CSWGuiButton* pCancel = &m_btnCancel;
    CSWGuiSaveGameEditBox* pName = &m_ebName;
    m_pSaveLoad = 0;
    StartLoadFromLayout("savename");
    InitControl(pOk, "BTN_OK", 1);
    InitControl(pCancel, "BTN_CANCEL", 1);
    InitControl(pName, "EDITBOX", 1);
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    pOk->m_bNavigable = 0;
    pCancel->m_bNavigable = 0;
    UnloadResource();
    CenterPanel();
    pOk->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveNamePanel::HandleOkButton);
    pCancel->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveNamePanel::HandleCancelButton);
    m_ebName.m_cText.m_nMaxLength = 0x10;
}

// FUNCTION: SWKOTOR 0x006CB940
CSWGuiSaveLoadEntry::CSWGuiSaveLoadEntry()
{
    m_bSavedGame = 0;
    m_bCorrupt = 0;
    m_bFlag2 = 0;
    m_bRebootAutoSave = 0;
    m_bPCAutoSave = 0;
    m_bFlag6 = 0;
    m_bCheatUsed = 0;
    m_nGameplayHint = 0;
    m_nStoryHint = 0;
    m_nGameNumber = 0;
    m_nTimePlayed = 0;
    m_nBlocksNeeded = 0;
    m_sSaveName = "Bad";
}

// FUNCTION: SWKOTOR 0x006CC680
CSWGuiSaveLoad::CSWGuiSaveLoad(CSWGuiManager* pManager, int bSave, int bFromMainMenu)
    : CSWGuiPanel(pManager)
{
    CSWGuiButton* pSaveLoad = &m_btnSaveLoad;
    CSWGuiButton* pDelete = &m_btnDelete;
    StartLoadFromLayout("saveload");
    InitControl(&m_lblPanelName, "LBL_PANELNAME", 1);
    InitControl(&m_lblScreenShot, "LBL_SCREENSHOT", 1);
    InitControl(&m_lblPlanetName, "LBL_PLANETNAME", 1);
    InitControl(&m_lblAreaName, "LBL_AREANAME", 1);
    InitControl(&m_aPartyMembers[0], "LBL_PM1", 1);
    InitControl(&m_aPartyMembers[1], "LBL_PM2", 1);
    InitControl(&m_aPartyMembers[2], "LBL_PM3", 1);
    InitControl(&m_lbGames, "LB_GAMES", 1);
    InitControl(pSaveLoad, "BTN_SAVELOAD", 1);
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(pDelete, "BTN_DELETE", 1);
    UnloadResource();
    m_sTempDirectory = "";
    m_lbGames.SetListFlag0(1);
    pSaveLoad->m_bNavigable = 0;
    pSaveLoad->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::v50);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    pDelete->m_bNavigable = 0;
    pDelete->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::v58);
    pDelete->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::PromptDeleteGame);
    if (bSave) {
        pSaveLoad->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::HandleSaveButton);
    } else {
        pSaveLoad->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::VerifyLoadGame);
    }
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbGames.m_pProtoItem->IsSelectable();
    pProto->m_cBorderParams.SetColor(g_vDifficultyExitColor);
    pProto->m_cHilightParams.SetColor(g_vAbilityColorGranted);
    pProto->m_cHilightParams.SetFlags(1, 1, 0);
    m_bSave = bSave;
    m_bFromMainMenu = bFromMainMenu;
    m_nNextGameNumber = 2;
    if (m_bSave) {
        m_btnSaveLoad.m_cTextParams.SetStrRef(0x633);
        m_lblPanelName.m_cText.SetStrRef(0x634);
    } else {
        m_btnSaveLoad.m_cTextParams.SetStrRef(0x635);
        m_lblPanelName.m_cText.SetStrRef(0x631);
    }
    PopulateGameList();
    {
        CExoString sAlias("TEMP:");
        g_pExoResMan->AddResourceDirectory(sAlias);
    }
    CSWGuiPanel::SetActiveControl(&m_lbGames, 0);
}
