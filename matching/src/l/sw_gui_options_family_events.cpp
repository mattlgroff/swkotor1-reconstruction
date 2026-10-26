// Options events: names follow established panel owners and callback registration.
#include "l/sw_gui_options_in_game.h"
extern int AurGetMaxAnisotropy(); // 0x0044F090

// FUNCTION: SWKOTOR 0x006E68B0
void CSWGuiInGameGameplay::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetGameDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E0100
void CSWGuiOptionsFeedback::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetFeedbackDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E0190
void CSWGuiOptionsGraphics::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetDisplayDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E0F50
void CSWGuiOptionsSound::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetSoundDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E12B0
void CSWGuiOptionsMouse::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetBrightnessDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E0470
void CSWGuiOptionsGraphicsAdvanced::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetGraphicsDefaults();
        SetUpOptions();
    }
}

// FUNCTION: SWKOTOR 0x006E1270
void CSWGuiOptionsSoundAdvanced::OnDefault(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetSoundQualityDefault();
        SetUpOptions();
        m_bChanged = 1;
    }
}

// FUNCTION: SWKOTOR 0x006E68A0
void CSWGuiInGameGameplay::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006E0130
void CSWGuiOptionsFeedback::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006E0140
void CSWGuiOptionsGraphics::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006E0460
void CSWGuiOptionsGraphicsAdvanced::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006E0010
void CSWGuiOptionsMain::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006E6170
void CSWGuiInGameGameplay::OnPanelAdded()
{
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006DDB80
void CSWGuiOptionsGraphicsAdvanced::OnDiscardChanges(CSWGuiControl* pControl)
{
    m_bDiscard = 1;
    HandleInputEvent28((int)pControl);
}

// FUNCTION: SWKOTOR 0x006DDFF0
void CSWGuiOptionsSoundAdvanced::OnDiscardChanges(CSWGuiControl* pControl)
{
    m_bDiscard = 1;
    HandleInputEvent28((int)pControl);
}

// FUNCTION: SWKOTOR 0x006DDB10
void CSWGuiOptionsGraphicsAdvanced::OnFrameBuffer(CSWGuiControl* pControl)
{
    unsigned long value = !m_bFrameBuffer;
    m_bChanged = 1;
    m_bFrameBuffer = value;
}

// FUNCTION: SWKOTOR 0x006DDB30
void CSWGuiOptionsGraphicsAdvanced::OnSoftShadows(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = !m_bSoftShadows;
        m_bChanged = 1;
        m_bSoftShadows = value;
    }
}

// FUNCTION: SWKOTOR 0x006DDB60
void CSWGuiOptionsGraphicsAdvanced::OnVSync(CSWGuiControl* pControl)
{
    m_bVSync = !m_bVSync;
}

// FUNCTION: SWKOTOR 0x006E04A0
void CSWGuiOptionsGraphicsAdvanced::OnTexQualLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && m_nTexQuality > 0) {
        --m_nTexQuality;
        SetTexQualText();
        m_pManager->PlayGuiSound(1);
    }
}

// FUNCTION: SWKOTOR 0x006E04E0
void CSWGuiOptionsGraphicsAdvanced::OnTexQualRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && m_nTexQuality <= m_nMaxTexQuality) {
        ++m_nTexQuality;
        SetTexQualText();
        m_pManager->PlayGuiSound(1);
    }
}

// FUNCTION: SWKOTOR 0x006E0680
void CSWGuiOptionsGraphicsAdvanced::OnAnisotropyLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && m_nAnisotropy > 0) {
        m_nAnisotropy /= 2;
        if (m_nAnisotropy <= 0) m_nAnisotropy = 1;
        SetAnisotropyText();
        m_pManager->PlayGuiSound(1);
    }
}

// FUNCTION: SWKOTOR 0x006E06D0
void CSWGuiOptionsGraphicsAdvanced::OnAnisotropyRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int value = m_nAnisotropy;
        if (value < AurGetMaxAnisotropy()) {
            m_nAnisotropy = value + value;
            SetAnisotropyText();
            m_pManager->PlayGuiSound(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x006DFBB0
void CSWGuiOptionsSoundAdvanced::OnEAXLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && m_nEAX > 0) {
        --m_nEAX;
        SetEAXText();
        m_pManager->PlayGuiSound(1);
        m_bChanged = 1;
    }
}

// FUNCTION: SWKOTOR 0x006DFC50
void CSWGuiOptionsSoundAdvanced::OnForceSoftware(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        unsigned long value = !m_bForceSoftware;
        m_bForceSoftware = value;
        EnableEAXControls(!value);
        m_bChanged = 1;
    }
}
