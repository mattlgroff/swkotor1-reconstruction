// Options slider, sound-quality and anti-alias stepping family.
#include "l/sw_gui_options_in_game.h"
extern int AurTestAntiAliasingMode(int nSamples); // 0x0044F140
class CExoSound { public: int GetMaxSoundQuality(); }; // 0x005D6460
extern CExoSound* g_pExoSound; // 0x007A39EC
extern Vector g_vAbilityColorGranted; // 0x007A23C0
extern Vector g_vDifficultyExitColor; // 0x007A23B4

// FUNCTION: SWKOTOR 0x006DED90
void CSWGuiOptionsGraphics::OnExitSlider(CSWGuiControl* pControl)
{
    pControl->GetSlider()->m_cThumbParams.SetHighlighted(0);
}

// FUNCTION: SWKOTOR 0x006DFAA0
void CSWGuiOptionsSound::OnEnterSlider(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
    pControl->GetSlider()->m_cThumbParams.SetHighlighted(1);
}

// FUNCTION: SWKOTOR 0x006E0150
void CSWGuiOptionsGraphics::OnEnterSlider(CSWGuiControl* pControl)
{
    if (pControl == &m_control_64) SetDescription(0xa546);
    pControl->GetSlider()->m_cThumbParams.SetHighlighted(1);
}

// FUNCTION: SWKOTOR 0x006DE200
void CSWGuiOptionsMouse::OnEnterSlider(CSWGuiControl* pControl)
{
    if (pControl == &m_control_94C) SetDescription(0xbb8d);
    pControl->GetSlider()->m_cThumbParams.SetHighlighted(1);
}

// FUNCTION: SWKOTOR 0x006DFE60
void CSWGuiOptionsMouse::OnEnterRevMouseButtons(CSWGuiControl* pControl)
{
    SetDescription(0xbd42);
}

// FUNCTION: SWKOTOR 0x006DFEE0
void CSWGuiOptionsMouse::OnMoveMouseSenSlider(CSWGuiControl* pControl)
{
    pControl->GetSlider();
    int value = pControl->GetSlider()->m_nValue;
    g_pAppManager->GetClientExoApp()->GetOptions()->m_nMouseSensitivity = (unsigned char)value;
}

// FUNCTION: SWKOTOR 0x006DFBF0
void CSWGuiOptionsSoundAdvanced::OnEAXRight(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int best;
        if (g_pExoSound->GetMaxSoundQuality() > 3) best = 3;
        else best = g_pExoSound->GetMaxSoundQuality();
        if (m_nEAX != best) {
            ++m_nEAX;
            SetEAXText();
            m_pManager->PlayGuiSound(1);
            m_bChanged = 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x006DDFA0
void CSWGuiOptionsSoundAdvanced::OnEnterOption(CSWGuiControl* pControl)
{
    if (pControl == &m_control_64) SetDescription(0xbdc1);
    else if (pControl == &m_control_31C || pControl == &m_control_4E0 || pControl == &m_control_6A4)
        SetDescription(0xbdc2);
}

// FUNCTION: SWKOTOR 0x006E1660
void CSWGuiOptionsGraphics::OnResolution(CSWGuiControl* pControl)
{
    int bClicked = pControl->m_n4c;
    if (bClicked) {
        CSWGuiOptionsResolution* pPanel = new CSWGuiOptionsResolution(m_pManager);
        pPanel->CenterPanel();
        m_pManager->AddPanel(pPanel, 1, 1);
    }
}

// FUNCTION: SWKOTOR 0x006E68E0
void CSWGuiInGameGameplay::OnDifficultyLeft(CSWGuiControl* pControl)
{
    if (pControl->m_n4c && m_nDifficulty > 0) {
        --m_nDifficulty;
        unsigned char value = (unsigned char)m_nDifficulty;
        g_pAppManager->GetClientExoApp()->GetOptions()->m_nDifficulty = (unsigned char)value;
        SetDifficultyText();
        m_pManager->PlayGuiSound(1);
    }
}

// FUNCTION: SWKOTOR 0x006E6710
void CSWGuiInGameGameplay::OnEnterDiff(CSWGuiControl* pControl)
{
    CSWGuiTextParams* pText = &m_control_CD4.m_cTextParams;
    pText->SetColor(g_vAbilityColorGranted);
    pText->m_nFlags |= 0x40;
    SetDifficultyText();
    SetDescription(0xa519);
}

// FUNCTION: SWKOTOR 0x006E6750
void CSWGuiInGameGameplay::OnExitDiff(CSWGuiControl* pControl)
{
    CSWGuiTextParams* pText = &m_control_CD4.m_cTextParams;
    pText->SetColor(g_vDifficultyExitColor);
    pText->m_nFlags &= ~0x40;
}

// FUNCTION: SWKOTOR 0x006E0520
void CSWGuiOptionsGraphicsAdvanced::OnAntiAliasLeft(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c || m_nAntiAlias <= 0) return;
    int found = 0;
    if (m_nAntiAlias >= 8) {
        m_nAntiAlias = 6;
        if (AurTestAntiAliasingMode(6)) found = 1;
    }
    if (m_nAntiAlias >= 6 && !found) {
        m_nAntiAlias = 4;
        if (AurTestAntiAliasingMode(4)) found = 1;
    }
    if (m_nAntiAlias >= 4) {
        if (!found) {
            m_nAntiAlias = 2;
            if (AurTestAntiAliasingMode(2)) goto updateText;
            m_nAntiAlias = 0;
        }
    } else if (!found) m_nAntiAlias = 0;
updateText:
    SetAntiAliasText();
    m_pManager->PlayGuiSound(1);
}

// FUNCTION: SWKOTOR 0x006E05E0
void CSWGuiOptionsGraphicsAdvanced::OnAntiAliasRight(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c || m_nAntiAlias == 8) return;
    int next;
    if (m_nAntiAlias <= 0) {
        next = 2;
        if (!AurTestAntiAliasingMode(next)) goto try4;
        goto found;
    }
    if (m_nAntiAlias <= 2) goto try4;
    if (m_nAntiAlias <= 4) goto try6;
    if (m_nAntiAlias <= 6) goto try8;
    goto done;
try4:
    next = 4;
    if (!AurTestAntiAliasingMode(next)) goto try6;
    goto found;
try6:
    next = 6;
    if (!AurTestAntiAliasingMode(next)) goto try8;
    goto found;
try8:
    next = 8;
    if (!AurTestAntiAliasingMode(next)) goto done;
found:
    m_nAntiAlias = next;
done:
    SetAntiAliasText();
    m_pManager->PlayGuiSound(1);
}

// FUNCTION: SWKOTOR 0x006E7570
void CSWGuiInGameAutoPause::OnDefault(CSWGuiControl* pControl)
{
    g_pAppManager->GetClientExoApp()->GetOptions()->SetControlDefaults();
    SetUpOptions();
}

// FUNCTION: SWKOTOR 0x006E7690
void CSWGuiInGameAutoPause::OnEnterSlot(CSWGuiControl* pControl)
{
    SetDescription(pControl->m_n58);
}

// FUNCTION: SWKOTOR 0x006DEDB0
void CSWGuiOptionsGraphics::OnShadows(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
        if (pOptions) {
            pOptions->SetShadows(!pOptions->m_bShadows);
            if (!pOptions->m_bShadows) pOptions->SetSoftShadows(0);
        }
    }
}

// FUNCTION: SWKOTOR 0x006DEE00
void CSWGuiOptionsGraphics::OnGrass(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int value = g_pAppManager->GetClientExoApp()->GetOptions()->m_bGrass;
        g_pAppManager->GetClientExoApp()->GetOptions()->SetGrass(!value);
    }
}
