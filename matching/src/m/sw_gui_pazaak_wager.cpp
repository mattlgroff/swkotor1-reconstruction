// Pazaak wager popup CSWGuiWagerPopup (vtable 0x007534C8,
// 0x0067CB40-0x0067F650): picks the wager before a side-deck is chosen.

#include "m/sw_gui_pazaak.h"

// Folded with the identical step-button handlers of other panels.
// FUNCTION: SWKOTOR 0x0067CB40
void CSWGuiWagerPopup::OnLessButton(CSWGuiControl* pControl)
{
    HandleInputEvent(0x2f, 1);
}

// FUNCTION: SWKOTOR 0x0067CB50
void CSWGuiWagerPopup::OnMoreButton(CSWGuiControl* pControl)
{
    HandleInputEvent(0x30, 1);
}

// FUNCTION: SWKOTOR 0x0067CB60
void CSWGuiWagerPopup::OnWagerButton(CSWGuiControl* pControl)
{
    HandleInputEvent(0x2d, 1);
}

// FUNCTION: SWKOTOR 0x0067D340
void CSWGuiWagerPopup::UpdateWagerText()
{
    CExoString sWager(m_nWager);
    m_lblWagerVal.m_cText.m_cParams.SetText(sWager);
}

// FUNCTION: SWKOTOR 0x0067E150
void CSWGuiWagerPopup::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x2d:
            m_pStart->HandleWagerExit(m_nWager);
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            m_pManager->RemovePanel(this);
            break;
        case 0x28:
        case 0x2e: {
            m_pManager->PlayGuiSound(0);
            CSWGuiMessageBox* pBox = g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pMessageBox;
            pBox->SetAllowCancel(1);
            pBox->SetCloseCallback(m_pStart, (void (CSWGuiPanel::*)(CSWGuiControl*))
                                                 &CSWGuiPazaakStart::HandleQuitDialog, 0);
            pBox->SetMessageStrRef(0xa5b8);
            m_pManager->AddPanel(pBox, 1, 1);
            break;
        }
        case 0x2f:
        case 0x32:
        case 0x3a:
        case 0x3b:
        case 0x3e:
        case 0x3f:
            if (m_nWager > 1) {
                m_pManager->PlayGuiSound(1);
                m_nWager--;
                UpdateWagerText();
            }
            break;
        case 0x30:
        case 0x31:
        case 0x39:
        case 0x3c:
        case 0x3d:
        case 0x40:
            if (m_nWager < m_nMaxWager) {
                m_pManager->PlayGuiSound(1);
                m_nWager++;
                UpdateWagerText();
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x0067F000
CSWGuiWagerPopup::CSWGuiWagerPopup(CSWGuiManager* pManager, CSWGuiPazaakStart* pStart, int nMaxWager)
    : CSWGuiPanel(pManager)
{
    m_pStart = pStart;
    m_nMaxWager = nMaxWager;
    m_nWager = nMaxWager;
    {
        CResRef cLayout("pazaakwager");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_TITLE");
        InitControl(&m_lblTitle, sName, 1);
    }
    {
        CExoString sName("LBL_MAXIMUM");
        InitControl(&m_lblMaximum, sName, 1);
    }
    {
        CExoString sName("LBL_WAGERVAL");
        InitControl(&m_lblWagerVal, sName, 1);
    }
    {
        CExoString sName("BTN_LESS");
        InitControl(&m_btnLess, sName, 1);
    }
    {
        CExoString sName("BTN_MORE");
        InitControl(&m_btnMore, sName, 1);
    }
    {
        CExoString sName("BTN_WAGER");
        InitControl(&m_btnWager, sName, 1);
    }
    {
        CExoString sName("BTN_QUIT");
        InitControl(&m_btnQuit, sName, 1);
    }
    {
        CExoString sName("LBL_BG");
        InitControl(&m_lblBG, sName, 1);
    }
    UnloadResource();
    m_btnLess.AddEvent(0x16256, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiWagerPopup::OnLessButton);
    m_btnMore.AddEvent(0x16256, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiWagerPopup::OnMoreButton);
    m_btnWager.AddEvent(0x27, this, (void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiWagerPopup::OnWagerButton);
    m_btnQuit.AddEvent(0x27, this, &CSWGuiPanel::OnBButtonPressed);
    m_btnLess.m_fRepeatDelay = 1.0f / 15.0f;
    m_btnMore.m_fRepeatDelay = 1.0f / 15.0f;
    m_btnLess.SetNavigable(0);
    m_btnMore.SetNavigable(0);
    m_btnWager.SetNavigable(0);
    m_btnQuit.SetNavigable(0);

    CSWSCreature* pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
        g_pAppManager->m_pServerExoApp->ClientToServerObjectID(
            g_pAppManager->m_pClientExoApp->GetParty()->GetPC()->m_idSelf));
    int nGold = 0;
    if (pCreature) {
        nGold = pCreature->GetGold();
    }
    UpdateWagerText();
    CExoString sText = g_pTlkTable->GetSimpleString(0x7e41) + " " + m_nMaxWager + "\n" +
                       g_pTlkTable->GetSimpleString(0x96c8) + " " + nGold;
    m_lblMaximum.m_cText.m_cParams.SetText(sText);
    m_nFlags |= 0x60;
}

// FUNCTION: SWKOTOR 0x0067F630 SYMBOL: ??_GCSWGuiWagerPopup@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0067F650 SYMBOL: ??1CSWGuiWagerPopup@@UAE@XZ

// CSWGuiSpeedButton (vtable 0x007533C8): the wager popup's auto-repeat
// buttons. The popup constructor emits its compiler-generated destructors.

// Folded with every other button subclass whose destructor only runs
// ~CSWGuiButton; the linker kept this copy.
// FUNCTION: SWKOTOR 0x00644A30 SYMBOL: ??1CSWGuiSpeedButton@@UAE@XZ

// FUNCTION: SWKOTOR 0x0067E800
void CSWGuiSpeedButton::HandleLMouseDown()
{
    m_bLeftButton = 1;
    CSWGuiControl::HandleLMouseDown();
    HandleInputEvent(0x16256, 1);
    m_fTimer = m_fInitialDelay;
}

// FUNCTION: SWKOTOR 0x0067E830
void CSWGuiSpeedButton::HandleRMouseDown()
{
    m_bLeftButton = 0;
    CSWGuiControl::HandleRMouseDown();
    HandleInputEvent(0x16257, 1);
    m_fTimer = m_fInitialDelay;
}

// FUNCTION: SWKOTOR 0x0067E860
void CSWGuiSpeedButton::Draw(float fDelta)
{
    CSWGuiButton::Draw(fDelta);
    if (g_pGuiManager->m_pCaptureControl != this) {
        return;
    }
    m_fTimer -= fDelta;
    while (m_fTimer < 0.0f) {
        m_fTimer += m_fRepeatDelay;
        if (m_bLeftButton) {
            HandleInputEvent(0x16256, 1);
        }
        else {
            HandleInputEvent(0x16257, 1);
        }
    }
}

// Shared with CSWGuiActivatedButton's table slot (identical bodies folded).
// FUNCTION: SWKOTOR 0x0067EFA0 SYMBOL: ??_GCSWGuiSpeedButton@@UAEPAXI@Z
