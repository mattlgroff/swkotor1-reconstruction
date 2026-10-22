// Tutorial message box CSWGuiTutorialBox (vtable 0x00755CD0,
// 0x006AA040-0x006AA8A0): pauses the game while shown, pages through the
// "Message%i" columns of tutorial.2da and on close resumes and performs the
// action that triggered the tutorial.

#include "l/sw_gui_tutorial_box.h"

// FUNCTION: SWKOTOR 0x006AA040
void CSWGuiTutorialBox::SetMessageStrRef(unsigned long nStrRef)
{
    CExoString sMessage = g_pTlkTable->GetSimpleString(nStrRef);
    g_pTlkTable->ParseTokens(&sMessage);
    CSWGuiMessageBox::SetMessage(sMessage);
}

// FUNCTION: SWKOTOR 0x006AA0D0
void CSWGuiTutorialBox::SetActionData(unsigned long a, unsigned long b, unsigned long c)
{
    m_oidCreature = a;
    m_nData = b;
    m_nAction = c;
}

// FUNCTION: SWKOTOR 0x006AA100
CSWGuiTutorialBox::CSWGuiTutorialBox(CSWGuiManager* pManager)
    : CSWGuiMessageBox(pManager)
{
    m_oidCreature = 0x7f000000;
    m_nData = 0x7f000000;
    m_nAction = 0;
    m_bWasPaused = 0;
    m_nReason = 0xff;
    m_nMessage = 0;
    m_lblMessage.m_cText.SetBaseFont("fnt_d16x16");
    m_nOpenSound = 0xf;
}

// FUNCTION: SWKOTOR 0x006AA1A0
int CSWGuiTutorialBox::HasTutorial(unsigned char nReason)
{
    int nStrRef;
    if (g_pRules->Get2DArrays()->GetTutorialTable()->GetINTEntry(nReason, "Message0", &nStrRef) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006AA230
void CSWGuiTutorialBox::OnCloseNothing(CSWGuiControl* pControl)
{
    RestorePause();
}

// FUNCTION: SWKOTOR 0x006AA260
void CSWGuiTutorialBox::OnCloseSkill(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    RestorePause();
    if (pCreature) {
        pCreature->PerformSkill((unsigned short)m_nAction, pCreature);
    }
}

// FUNCTION: SWKOTOR 0x006AA2C0
void CSWGuiTutorialBox::OnCloseSpell(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    RestorePause();
    if (pCreature) {
        Vector vTarget;
        vTarget.x = 0.0f;
        vTarget.y = 0.0f;
        vTarget.z = 0.0f;
        pCreature->PerformCastSpell(m_nAction, pCreature->m_oid, vTarget, (unsigned char)m_nData, 0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x006AA350
void CSWGuiTutorialBox::OnCloseFeat(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_nData);
    RestorePause();
    if (pCreature) {
        pCreature->PerformFeat(m_nAction, pTarget);
    }
}

// FUNCTION: SWKOTOR 0x006AA3D0
void CSWGuiTutorialBox::OnCloseUseItem(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    RestorePause();
    if (pCreature) {
        pCreature->PerformUseItem(m_nData, 0, 0x7f000000, pCreature->m_vPosition);
    }
}

// FUNCTION: SWKOTOR 0x006AA440
void CSWGuiTutorialBox::OnCloseFlourish(CSWGuiControl* pControl)
{
    RestorePause();
    g_pAppManager->GetClientExoApp()->PlayerFlourishWeapons();
}

// FUNCTION: SWKOTOR 0x006AA480
void CSWGuiTutorialBox::OnCloseOpen(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    RestorePause();
    if (pCreature) {
        if (m_nAction) {
            CSWCDoor* pDoor = g_pAppManager->GetClientExoApp()->GetDoorByGameObjectID(m_nData);
            if (pDoor) {
                pDoor->ActionMenuOpen(0, pCreature);
            }
        } else {
            CSWCPlaceable* pPlaceable = g_pAppManager->GetClientExoApp()->GetPlaceableByGameObjectID(m_nData);
            if (pPlaceable) {
                pPlaceable->ActionMenuOpen(0, pCreature);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006AA520
void CSWGuiTutorialBox::OnCloseAttack(CSWGuiControl* pControl)
{
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
    RestorePause();
    if (pCreature) {
        CSWCCreature* pTarget = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_nData);
        if (pTarget) {
            pTarget->ActionMenuAttack(0, pCreature);
        }
    }
}

// FUNCTION: SWKOTOR 0x006AA590
void CSWGuiTutorialBox::OnCloseGui(CSWGuiControl* pControl)
{
    RestorePause();
    g_pAppManager->GetClientExoApp()->GetInGameGui();
}

// Identical to OnCloseGui; the linker folded the two (/OPT:ICF).
void CSWGuiTutorialBox::OnCloseGuiB(CSWGuiControl* pControl)
{
    RestorePause();
    g_pAppManager->GetClientExoApp()->GetInGameGui();
}

// FUNCTION: SWKOTOR 0x006AA5D0
void CSWGuiTutorialBox::OnClosePartySelection(CSWGuiControl* pControl)
{
    RestorePause();
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pPartySelection->Show();
}

// FUNCTION: SWKOTOR 0x006AA620
void CSWGuiTutorialBox::OnCloseMap(CSWGuiControl* pControl)
{
    RestorePause();
    g_pAppManager->GetClientExoApp()->GetInGameGui()->GetMapPanel(6, 0)->OnXButton();
}

// FUNCTION: SWKOTOR 0x006AA670
int CSWGuiTutorialBox::ShowNextMessage()
{
    CExoString sColumn;
    CExoString sIcon;
    int nStrRef = 0;
    sColumn.Format("Message%i", m_nMessage);
    m_nMessage++;
    if (g_pRules->Get2DArrays()->GetTutorialTable()->GetINTEntry(m_nReason, sColumn, &nStrRef)) {
        int nNext;
        sColumn.Format("Message%i", m_nMessage);
        if (g_pRules->Get2DArrays()->GetTutorialTable()->GetINTEntry(m_nReason, sColumn, &nNext)) {
            SetOkStrRef(0x96df);
        } else {
            SetOkStrRef(0x62c);
        }
        if (g_pRules->Get2DArrays()->GetTutorialTable()->GetCExoStringEntry(m_nReason, "Icon", &sIcon)) {
            SetImage(sIcon);
        }
        SetAllowCancel(0);
        SetMessageStrRef(nStrRef);
    }
    return nStrRef;
}

// FUNCTION: SWKOTOR 0x006AA900
int CSWGuiTutorialBox::ShowTutorial(unsigned char nReason)
{
    m_nReason = nReason;
    m_nMessage = 0;
    if (!ShowNextMessage()) {
        return 0;
    }
    switch (nReason) {
    case 0:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseSkill, 0);
        return 1;
    case 1:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseFeat, 0);
        return 1;
    case 2:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseUseItem, 0);
        return 1;
    case 3:
    case 4:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseSpell, 0);
        return 1;
    case 0x1f:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseFlourish, 0);
        return 1;
    case 0x21:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseOpen, 0);
        return 1;
    case 0x22:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseAttack, 0);
        return 1;
    case 0x1d:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseGui, 0);
        return 1;
    case 0x1e:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseGuiB, 0);
        return 1;
    case 0x20:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnClosePartySelection, 0);
        return 1;
    case 0x27:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseMap, 0);
        return 1;
    default:
        SetCloseCallback(this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiTutorialBox::OnCloseNothing, 0);
        return 1;
    }
}

// FUNCTION: SWKOTOR 0x006AA830 SYMBOL: ??_GCSWGuiTutorialBox@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006AA850 SYMBOL: ??1CSWGuiTutorialBox@@UAE@XZ

// FUNCTION: SWKOTOR 0x006AA860
void CSWGuiTutorialBox::OnPanelAdded()
{
    CSWGuiMessageBox::OnPanelAdded();
    m_bWasPaused = g_pAppManager->GetClientExoApp()->GetPausedByCombat();
    if (!m_bWasPaused) {
        g_pAppManager->GetClientExoApp()->SetPausedByCombat(1, 2, 0);
    }
}

// FUNCTION: SWKOTOR 0x006AA8A0
void CSWGuiTutorialBox::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0x1f6:
            if (ShowNextMessage()) {
                return;
            }
            break;
        }
    }
    CSWGuiMessageBox::HandleInputEvent(nEvent, bDown);
}
