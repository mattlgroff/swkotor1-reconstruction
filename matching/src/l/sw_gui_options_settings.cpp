// Options panels: description text, settings transfer to CClientOptions and
// the sound/graphics slider handlers (swguioptions.o).
#include "l/sw_gui_options_in_game.h"

extern int AurTestAntiAliasingMode(int nSamples); // 0x0044F140
extern int AurGetMaxAnisotropy(); // 0x0044F090

class CExoSound {
public:
    void SetMusicVolume(float fVolume, int bFade);  // 0x005D5C50
    void SetSoundEffectVolume(float fVolume);       // 0x005D5CD0
    void SetDialogVolume(float fVolume);            // 0x005D5D50
};
extern CExoSound* g_pExoSound; // 0x007A39EC

class CRenderWindow {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20();
    virtual void SetGamma(float fGamma, int bApply); // 0x24
};
extern CRenderWindow* g_pRenderWindow; // 0x007A39F8

extern Vector g_vDifficultyExitColor; // 0x007A23B4 (enabled text)
extern Vector g_vKeyMapLockedTextColor; // 0x007A2408 (disabled text)

// FUNCTION: SWKOTOR 0x006DDC80
void CSWGuiOptionsSound::SetDescription(unsigned long nStrRef)
{
    m_control_C44.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_C44.m_cText.m_sText;
    m_control_EC4.ClearControls();
    CSWGuiExtent extent = m_control_C44.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_C44.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_C44.m_cTextBox.GetLineHeight();
    }
    m_control_C44.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_C44;
    m_control_EC4.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_EC4.SetActiveControl(&m_control_C44, 0);
}

// FUNCTION: SWKOTOR 0x006DDE50
void CSWGuiOptionsSoundAdvanced::SetDescription(unsigned long nStrRef)
{
    m_control_868.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_868.m_cText.m_sText;
    m_control_AE8.ClearControls();
    CSWGuiExtent extent = m_control_868.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_868.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_868.m_cTextBox.GetLineHeight();
    }
    m_control_868.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_868;
    m_control_AE8.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_AE8.SetActiveControl(&m_control_868, 0);
}

// FUNCTION: SWKOTOR 0x006DE340
void CSWGuiOptionsMain::SetDescription(unsigned long nStrRef)
{
    m_control_AFC.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_AFC.m_cText.m_sText;
    m_control_F40.ClearControls();
    CSWGuiExtent extent = m_control_AFC.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_AFC.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_AFC.m_cTextBox.GetLineHeight();
    }
    m_control_AFC.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_AFC;
    m_control_F40.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_F40.SetActiveControl(&m_control_AFC, 0);
}

// FUNCTION: SWKOTOR 0x006DE9B0
void CSWGuiOptionsFeedback::SetDescription(unsigned long nStrRef)
{
    m_control_344.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_344.m_cText.m_sText;
    m_control_5C4.ClearControls();
    CSWGuiExtent extent = m_control_344.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_344.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_344.m_cTextBox.GetLineHeight();
    }
    m_control_344.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_344;
    m_control_5C4.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_5C4.SetActiveControl(&m_control_344, 0);
}

// FUNCTION: SWKOTOR 0x006DEE40
void CSWGuiOptionsGraphics::SetDescription(unsigned long nStrRef)
{
    m_control_35C.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_35C.m_cText.m_sText;
    m_control_5DC.ClearControls();
    CSWGuiExtent extent = m_control_35C.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_35C.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_35C.m_cTextBox.GetLineHeight();
    }
    m_control_35C.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_35C;
    m_control_5DC.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_5DC.SetActiveControl(&m_control_35C, 0);
}

// The mouse panel's identical SetDescription folds into this body.
// FUNCTION: SWKOTOR 0x006DF390
void CSWGuiOptionsGraphicsAdvanced::SetDescription(unsigned long nStrRef)
{
    m_control_64.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_64.m_cText.m_sText;
    m_control_2E4.ClearControls();
    CSWGuiExtent extent = m_control_64.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_64.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_64.m_cTextBox.GetLineHeight();
    }
    m_control_64.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_64;
    m_control_2E4.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_2E4.SetActiveControl(&m_control_64, 0);
}

// FUNCTION: SWKOTOR 0x006DDF40
void CSWGuiOptionsSoundAdvanced::EnableEAXControls(unsigned long bEnabled)
{
    m_control_31C.SetEnabled(bEnabled);
    m_control_31C.m_cTextParams.SetColor(bEnabled ? g_vDifficultyExitColor : g_vKeyMapLockedTextColor);
    m_control_4E0.SetEnabled(bEnabled);
    m_control_6A4.SetEnabled(bEnabled);
}

// FUNCTION: SWKOTOR 0x006DDB90
void CSWGuiOptionsSound::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x2d:
            m_pManager->PlayGuiSound(0);
            break;
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            break;
        case 0x39:
            m_control_EC4.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_control_EC4.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DED30
void CSWGuiOptionsGraphics::OnMoveGammaSlider(CSWGuiControl* pControl)
{
    int nMax = pControl->GetSlider()->m_nMaxValue;
    int nValue = pControl->GetSlider()->m_nValue;
    g_pRenderWindow->SetGamma(2.0f - ((float)nValue / nMax) * 1.77f, 0);
    g_pAppManager->GetClientExoApp()->GetOptions()->m_nGamma = (unsigned char)nValue;
}

// FUNCTION: SWKOTOR 0x006DEFE0
void CSWGuiOptionsGraphicsAdvanced::SetNewOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        pOptions->SetAntiAliasing((unsigned char)m_nAntiAlias);
        pOptions->SetTexturePack((unsigned char)m_nTexQuality);
        pOptions->SetFrameBuffer(m_bFrameBuffer);
        pOptions->SetAnisotropy(m_nAnisotropy);
        pOptions->SetVSync(m_bVSync);
        pOptions->SetSoftShadows(m_bSoftShadows);
        if (m_bSoftShadows) {
            pOptions->SetShadows(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x006DF1C0
void CSWGuiOptionsGraphicsAdvanced::SetAntiAliasText()
{
    switch (m_nAntiAlias) {
    case 0:
        m_control_132C.m_cTextParams.SetStrRef(0xbb7c);
        break;
    case 2:
        m_control_132C.m_cTextParams.SetStrRef(0xbb7d);
        break;
    case 4:
        m_control_132C.m_cTextParams.SetStrRef(0xbb7f);
        break;
    case 6:
        m_control_132C.m_cTextParams.SetStrRef(0xbfe4);
        break;
    case 8:
        m_control_132C.m_cTextParams.SetStrRef(0xbfe5);
        break;
    }
    ShowControl(&m_control_14F0);
    ShowControl(&m_control_16B4);
    if (m_nAntiAlias == 0) {
        HideControl(&m_control_14F0);
    }
    int nMax = 0;
    if (AurTestAntiAliasingMode(2)) nMax = 2;
    if (AurTestAntiAliasingMode(4)) nMax = 4;
    if (AurTestAntiAliasingMode(6)) nMax = 6;
    if (AurTestAntiAliasingMode(8)) nMax = 8;
    if (m_nAntiAlias == 8 || m_nAntiAlias >= nMax) {
        HideControl(&m_control_16B4);
    }
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x006DF2C0
void CSWGuiOptionsGraphicsAdvanced::SetAnisotropyText()
{
    switch (m_nAnisotropy) {
    case 1:
        m_control_DE0.m_cTextParams.SetStrRef(0xbfb7);
        break;
    case 2:
        m_control_DE0.m_cTextParams.SetStrRef(0xbfb8);
        break;
    case 4:
        m_control_DE0.m_cTextParams.SetStrRef(0xbfb9);
        break;
    case 8:
        m_control_DE0.m_cTextParams.SetStrRef(0xbfba);
        break;
    case 16:
        m_control_DE0.m_cTextParams.SetStrRef(0xbfbb);
        break;
    }
    ShowControl(&m_control_FA4);
    ShowControl(&m_control_1168);
    if (m_nAnisotropy <= 1) {
        HideControl(&m_control_FA4);
    } else if (m_nAnisotropy >= AurGetMaxAnisotropy() || m_nAnisotropy >= 16) {
        HideControl(&m_control_1168);
    }
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x006DF9B0
void CSWGuiOptionsSound::OnMoveVolumeSlider(CSWGuiControl* pControl)
{
    int nMax = pControl->GetSlider()->m_nMaxValue;
    int nValue = pControl->GetSlider()->m_nValue;
    float fVolume = (float)nValue / nMax;
    if (pControl == &m_control_64) {
        g_pExoSound->SetMusicVolume(fVolume, 0);
        g_pAppManager->GetClientExoApp()->GetOptions()->m_nMusicVolume = (unsigned char)nValue;
    } else if (pControl == &m_control_21C) {
        g_pExoSound->SetDialogVolume(fVolume);
        g_pAppManager->GetClientExoApp()->GetOptions()->m_nVoiceVolume = (unsigned char)nValue;
    } else if (pControl == &m_control_3D4) {
        g_pExoSound->SetSoundEffectVolume(fVolume);
        g_pAppManager->GetClientExoApp()->GetOptions()->m_nSoundFXVolume = (unsigned char)nValue;
    } else if (pControl == &m_control_58C) {
        g_pAppManager->GetClientExoApp()->GetOptions()->m_nMovieVolume = (unsigned char)nValue;
    }
}

// FUNCTION: SWKOTOR 0x006DFB60
void CSWGuiOptionsSoundAdvanced::SetNewOptions()
{
    if (g_pAppManager->GetClientExoApp()->GetOptions()) {
        g_pAppManager->GetClientExoApp()->GetOptions()->SetSoundQuality((unsigned char)m_nEAX);
        g_pAppManager->GetClientExoApp()->GetOptions()->SetForceSoftware(m_bForceSoftware);
    }
}

// FUNCTION: SWKOTOR 0x006DFE70
void CSWGuiOptionsMouse::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_control_94C.GetSlider()->SetValue(pOptions->GetMouseSensitivity());
        m_control_C44.GetButtonToggle()->SetToggleState(pOptions->m_bReverseMouseButtons);
    }
}

// FUNCTION: SWKOTOR 0x006E7280
void CSWGuiInGameAutoPause::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_aCheckboxes[0].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause0);
        m_aCheckboxes[1].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause1);
        m_aCheckboxes[2].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause2);
        m_aCheckboxes[3].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause3);
        m_aCheckboxes[4].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause4);
        m_aCheckboxes[5].GetButtonToggle()->SetToggleState(pOptions->m_bAutoPause5);
    }
}

// FUNCTION: SWKOTOR 0x006E73D0
void CSWGuiInGameAutoPause::SetNewOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        pOptions->m_bAutoPause0 = m_aCheckboxes[0].GetButtonToggle()->m_bSelected;
        pOptions->m_bAutoPause1 = m_aCheckboxes[1].GetButtonToggle()->m_bSelected;
        pOptions->m_bAutoPause2 = m_aCheckboxes[2].GetButtonToggle()->m_bSelected;
        pOptions->m_bAutoPause3 = m_aCheckboxes[3].GetButtonToggle()->m_bSelected;
        pOptions->m_bAutoPause4 = m_aCheckboxes[4].GetButtonToggle()->m_bSelected;
        pOptions->m_bAutoPause5 = m_aCheckboxes[5].GetButtonToggle()->m_bSelected;
    }
}

// FUNCTION: SWKOTOR 0x006E74C0
void CSWGuiInGameAutoPause::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
            SetNewOptions();
            m_pManager->PopModalPanel();
            m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
            break;
        case 0x2d:
            if (m_pControl1c && m_pControl1c->GetButtonToggle()) {
                m_pManager->PlayGuiSound(3);
                CSWGuiButtonToggle* pToggle = m_pControl1c->GetButtonToggle();
                m_pControl1c->GetButtonToggle()->SetSelected(~pToggle->m_nToggleFlags & 1);
            }
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E75A0
void CSWGuiInGameAutoPause::SetDescription(unsigned long nStrRef)
{
    m_lblDescription.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_lblDescription.m_cText.m_sText;
    m_lbDetails.ClearControls();
    CSWGuiExtent extent = m_lblDescription.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_lblDescription.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDescription.m_cTextBox.GetLineHeight();
    }
    m_lblDescription.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDescription;
    m_lbDetails.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDetails.SetActiveControl(&m_lblDescription, 0);
}

// FUNCTION: SWKOTOR 0x006E6620
void CSWGuiInGameGameplay::SetDescription(unsigned long nStrRef)
{
    m_control_64.m_cText.SetStrRef(nStrRef);
    CExoString sText = m_control_64.m_cText.m_sText;
    m_control_2E4.ClearControls();
    CSWGuiExtent extent = m_control_64.m_extent;
    if (sText.GetLength() > 0) {
        extent.height = m_control_64.m_cTextBox.GetHeight();
    } else {
        extent.height = m_control_64.m_cTextBox.GetLineHeight();
    }
    m_control_64.SetExtent(extent);
    CSWGuiControl* pLabel = &m_control_64;
    m_control_2E4.AddControls(&pLabel, 1, 0, 0, 0);
    m_control_2E4.SetActiveControl(&m_control_64, 0);
}

// FUNCTION: SWKOTOR 0x006DEF30
void CSWGuiOptionsGraphics::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_control_64.GetSlider()->SetValue(pOptions->GetGamma());
        OnMoveGammaSlider(&m_control_64);
        if (pOptions->m_bSoftShadows) {
            pOptions->SetShadows(1);
        }
        m_control_E08.GetButtonToggle()->SetToggleState(pOptions->m_bShadows);
        m_control_10BC.GetButtonToggle()->SetToggleState(pOptions->m_bGrass);
    }
}

// FUNCTION: SWKOTOR 0x006DFAD0
void CSWGuiOptionsSound::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_control_64.SetValue(pOptions->m_nMusicVolume);
        m_control_21C.SetValue(pOptions->m_nVoiceVolume);
        m_control_3D4.SetValue(pOptions->m_nSoundFXVolume);
        m_control_58C.SetValue(pOptions->m_nMovieVolume);
        OnMoveVolumeSlider(&m_control_64);
        OnMoveVolumeSlider(&m_control_21C);
        OnMoveVolumeSlider(&m_control_3D4);
        OnMoveVolumeSlider(&m_control_58C);
    }
}
