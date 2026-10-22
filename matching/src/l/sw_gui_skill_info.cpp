// the skill info popup CSWGuiSkillInfo (layout "skillinfo",
// vtable 0x00757940: a list box of up to ten CSWGuiInGameSkillEntry rows,
// a message label and an OK button) and the examine message box
// CSWGuiExamineBox (vtable 0x007579C8).
#include "l/sw_gui_skill_info.h"

// FUNCTION: SWKOTOR 0x006CD320
CSWGuiSkillInfo::~CSWGuiSkillInfo()
{
    m_lbSkills.ClearControls();
}

// FUNCTION: SWKOTOR 0x006CD3C0
void CSWGuiSkillInfo::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            SetToDestroy(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006CDFA0 SYMBOL: ??_GCSWGuiSkillInfo@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006CDFC0
void CSWGuiSkillInfo::SetSkills(const CExoArrayListL<int>& lstSkills, int nType)
{
    CExoArrayListL<CSWGuiControl*> lstControls;
    for (int i = 0; i < lstSkills.num; i++) {
        ((CSWGuiInGameSkillEntryV*)&m_aEntries[i])->SetSkillI(lstSkills.element[i], nType);
        lstControls.Add(&m_aEntries[i]);
    }
    m_lbSkills.AddControls(lstControls, 0, 0, 0);
    SetActiveControl(&m_lbSkills, 0);
}

// FUNCTION: SWKOTOR 0x006CE7C0
// NONMATCH: the m_n2c1 byte store is scheduled before the vtable load (the
// original stores it right before the SetEnabled call).
CSWGuiSkillInfo::CSWGuiSkillInfo(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("skillinfo");
    InitControl(&m_lbSkills, "LB_SKILLS", 1);
    InitControl(&m_lblMessage, "LBL_MESSAGE", 1);
    InitControl(&m_btnOk, "BTN_OK", 1);
    UnloadResource();
    CSWGuiListBox* pList = &m_lbSkills;
    pList->m_nListFlags |= 0x200;
    m_lbSkills.m_n2c1 = 1;
    pList->SetEnabled(0);
    CSWGuiLabelHilight* pProto = m_lbSkills.m_pProtoItem->GetLabelHilight();
    int nWidth = pList->m_nAreaWidth - pList->m_nPadding * 2;
    for (int i = 0; i < 10; i++) {
        CSWGuiInGameSkillEntryV* pEntry = (CSWGuiInGameSkillEntryV*)&m_aEntries[i];
        pEntry->SetStyle(&pProto->m_cTextBox, &pProto->GetBorder(), &pProto->m_cHilight, nWidth);
    }
    CSWGuiButton* pOk = &m_btnOk;
    pOk->m_bNavigable = 0;
    pOk->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::v50);
    CenterPanel();
}

// FUNCTION: SWKOTOR 0x006CE9C0
CSWGuiExamineBox::CSWGuiExamineBox(CSWGuiManager* pManager)
    : CSWGuiMessageBox(pManager)
{
    GetOkButton().m_cTextParams.SetStrRef(0x62e);
    SetAllowCancel(0);
    m_n984 = 0;
}

// FUNCTION: SWKOTOR 0x006CEA30
void CSWGuiExamineBox::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            ((CGuiInGameL*)g_pAppManager->m_pClientExoApp->GetInGameGui())->HideExamineBox();
            return;
        }
    }
    CSWGuiMessageBox::HandleInputEvent(nEvent, bDown);
}

// Emits the vtable (and ??_G) of CSWGuiSkillInfo.
CSWGuiPanel* NewSkillInfo(CSWGuiManager* pManager)
{
    return new CSWGuiSkillInfo(pManager);
}
