// CSWGuiLevelUpPanel constructor (layout "LEVELUPPNL"); the scalar deleting
// destructor is emitted with the vtable here.
#include "l/sw_gui_levelup_chargen.h"

// FUNCTION: SWKOTOR 0x006EE7D0
CSWGuiLevelUpPanel::CSWGuiLevelUpPanel(CSWGuiManager* pManager, unsigned long oidCreature)
    : CSWGuiPanel(pManager)
{
    CExoString sName;
    m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    m_pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oidCreature);
    m_bCanCancel = 0;
    int nNumber = 0;
    StartLoadFromLayout("LEVELUPPNL");
    InitControl(&m_lblBG, "LBL_BG", 1);
    for (int i = 0; i < 5; i++) {
        sName.Format("LBL_%d", i + 1);
        InitControl(&m_aHilights[i], sName, 1);
        m_aHilights[i].m_cHilight.m_cParams.SetFlags(1, 0, 1);
        sName.Format("BTN_STEPNAME%d", i + 1);
        InitControl(&m_aSteps[i], sName, 1);
        sName.Format("LBL_NUM%d", i + 1);
        InitControl(&m_aNumbers[i], sName, 1);
    }
    InitControl(&m_btnBack, "BTN_BACK", 1);
    InitControl(&m_btnCancel, "BTN_CANCEL", 1);
    UnloadResource();

    if (GetLevelUpStats()->GetTotalLevel() % 4 == 0) {
        EnableStep(0, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnSelectAbilitiesButton, nNumber);
    } else {
        DisableStep(0);
    }

    GetLevelUpStats()->m_nSkillPoints = 0;
    GetLevelUpStats()->AddSkillPointsPerLevel();
    int nSkillPoints = GetLevelUpStats()->m_nSkillPoints;
    if (nSkillPoints > 0) {
        EnableStep(1, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnSelectSkillsButton, nNumber);
    } else {
        DisableStep(1);
    }

    unsigned char nNormal = 0;
    unsigned char nBonus = 0;
    GetLevelUpStats()->GetPackageFeatsLeft(&nNormal, &nBonus);
    if (nBonus + nNormal > 0) {
        EnableStep(2, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnSelectFeatsButton, nNumber);
    } else {
        DisableStep(2);
        CExoArrayListL<int> lstGranted;
        GetLevelUpStats()->AddGrantedFeats(&lstGranted);
        if (lstGranted.num > 0) {
            CSWGuiSkillInfo* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pSkillInfo;
            pBox->SetSkills(lstGranted, 1);
            pBox->m_lblMessage.m_cText.SetStrRef(0xA512);
            m_pManager->AddPanel(pBox, 1, 1);
        }
    }

    int nSpellsLeft = GetLevelUpStats()->ComputeNumberKnownSpellsLeft();
    if (nSpellsLeft > 0) {
        EnableStep(3, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnSelectPowersButton, nNumber);
    } else {
        DisableStep(3);
    }

    EnableStep(4, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnSelectAcceptButton, nNumber);

    m_nFlags |= 0x60;
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pCancel = &m_btnCancel;
    pCancel->m_bNavigable = 0;
    pCancel->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiLevelUpPanel::OnCancelPressed);
}

// FUNCTION: SWKOTOR 0x006EEE70 SYMBOL: ??_GCSWGuiLevelUpPanel@@UAEPAXI@Z
// Emitted by the constructor above; the regular destructor is external.
