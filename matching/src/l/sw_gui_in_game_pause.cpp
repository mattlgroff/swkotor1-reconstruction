// CSWGuiInGamePause (layout "pause", vtable 0x00756DC8): the pause banner
// with the pause reason, the "press to unpause" hint and an unpause
// button. The destructor is in sw_gui_panel_dtors_novtable.cpp.
#include "l/sw_gui_inventory_equip.h"

class CSWGuiInGamePause : public CSWGuiPanel {
public:
    CSWGuiInGamePause(CSWGuiManager* pManager);     // 0x006C03B0
    virtual ~CSWGuiInGamePause();                   // 0x006C05A0
    virtual void Draw(float fDelta);                // 0x34 (0x006C00B0)
    virtual void OnPanelAdded();                    // 0x48 (0x006C02F0)
    void SetPauseReason(unsigned char nReason);     // 0x006C00C0
    void OnUnpause(CSWGuiControl* pControl);        // 0x006C0350 (descriptive)

    CSWGuiLabel m_lblReason;                        // 0x64
    CSWGuiLabel m_lblPress;                         // 0x1a4
    CSWGuiButton m_btnUnpause;                      // 0x2e4
    int m_nSavedTop;                                // 0x4a8 (descriptive)
    int m_nSavedLeft;                               // 0x4ac (descriptive)
};

// FUNCTION: SWKOTOR 0x006C00B0
void CSWGuiInGamePause::Draw(float fDelta)
{
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006C00C0
void CSWGuiInGamePause::SetPauseReason(unsigned char nReason)
{
    int bShowPress = 1;
    unsigned long nStrRef;
    switch (nReason) {
    case 1:
        nStrRef = 0xbc54;
        bShowPress = 0;
        break;
    case 5:
        nStrRef = 0xa5c0;
        break;
    case 7:
        nStrRef = 0xa5f2;
        break;
    case 8:
        nStrRef = 0xa5f1;
        break;
    case 9:
        nStrRef = 0xa59d;
        break;
    case 10:
        nStrRef = 0xbd27;
        break;
    case 11:
        nStrRef = 0xbfde;
        bShowPress = 0;
        break;
    default:
        nStrRef = 0x5e4;
        break;
    }
    CExoString sReason = g_pAppManager->m_pClientExoApp->GetGUIString(nStrRef);
    m_lblReason.m_cText.SetText(sReason);
    CSWGuiZeroExtent cExtent;
    CSWGuiZeroExtent cButton;
    int nHeight = m_lblReason.m_cTextBox.GetHeight();
    static_cast<CSWGuiExtent&>(cExtent) = m_lblReason.m_extent;
    cExtent.height = nHeight;
    m_lblReason.SetExtent(cExtent);
    cButton = cExtent;
    int nTop = cExtent.top + nHeight + 2;
    if (bShowPress) {
        ShowControl(&m_lblPress);
        int nPressHeight = m_lblPress.m_cTextBox.GetHeight();
        static_cast<CSWGuiExtent&>(cExtent) = m_lblPress.m_extent;
        cExtent.height = nPressHeight;
        cExtent.top = nTop;
        m_lblPress.SetExtent(cExtent);
    } else {
        HideControl(&m_lblPress);
    }
    CSWGuiExtent cPanel = m_extent;
    cPanel.height = cExtent.top + cExtent.height + 5;
    SetExtent(cPanel);
    cButton.height = cPanel.height;
    m_btnUnpause.SetExtent(cButton);
}

// FUNCTION: SWKOTOR 0x006C02F0
void CSWGuiInGamePause::OnPanelAdded()
{
    if (g_pAppManager->m_pClientExoApp->GetMiniGame()) {
        m_extent.left = (m_pManager->m_nWidth - m_extent.width) / 2;
        m_extent.top = (m_pManager->m_nHeight - m_extent.height) / 2;
    } else {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetupPauseGuiExtent(&m_extent);
    }
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006C0350
void CSWGuiInGamePause::OnUnpause(CSWGuiControl* pControl)
{
    if (pControl->IsActivated()) {
        int bPaused = g_pAppManager->m_pClientExoApp->GetPausedByCombat();
        g_pAppManager->m_pClientExoApp->SetPausedByCombat(!bPaused, 4, 0);
        if (g_pAppManager->m_pClientExoApp->GetAutoPaused()) {
            g_pAppManager->m_pClientExoApp->SetAutoPaused(0, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x006C03B0
CSWGuiInGamePause::CSWGuiInGamePause(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("pause");
    InitControl(&m_lblReason, "LBL_PAUSEREASON", 1);
    InitControl(&m_lblPress, "LBL_PRESS", 1);
    InitControl(&m_btnUnpause, "BTN_UNPAUSE", 1);
    m_nFlags |= 1;
    UnloadResource();
    m_nSavedTop = m_extent.top;
    m_nSavedLeft = m_extent.left;
    CSWGuiTextParams& rPress = m_lblPress.m_cText;
    CExoString sPress(rPress.m_sText);
    g_pTlkTable->ParseTokens(&sPress);
    rPress.SetText(sPress);
    m_lblReason.m_cText.m_bHighlighted = 1;
    rPress.m_bHighlighted = 1;
    CSWGuiButton* pButton = &m_btnUnpause;
    pButton->m_bNavigable = 0;
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::v50);
    pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiInGamePause::OnUnpause);
}
