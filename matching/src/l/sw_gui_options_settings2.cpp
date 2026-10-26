// Options panels: input handling, option-to-control transfer and the
// texture-pack / difficulty / EAX label helpers (swguioptions.o).
#include "l/sw_gui_options_in_game.h"

extern unsigned char AurCheckFrameBufferModificationsAvailable(); // 0x0045FAB0
extern unsigned char AurCheckSoftShadowsAvailable(); // 0x0045FA20 (descriptive name)

class CExoSound {
public:
    int GetMaxSoundQuality(); // 0x005D6460
};
extern CExoSound* g_pExoSound; // 0x007A39EC

// FUNCTION: SWKOTOR 0x006DDD70
void CSWGuiOptionsSoundAdvanced::SetEAXText()
{
    CExoString sText;
    switch (m_nEAX) {
    case 0:
        m_control_31C.m_cTextParams.SetStrRef(0xbdbd);
        break;
    case 2:
        m_control_31C.m_cTextParams.SetStrRef(0xbdbf);
        break;
    case 3:
        m_control_31C.m_cTextParams.SetStrRef(0xbdc0);
        break;
    default:
        m_control_31C.m_cTextParams.SetStrRef(0xbdbe);
        break;
    }
    ShowControl(&m_control_6A4);
    ShowControl(&m_control_4E0);
    int nMax;
    if (g_pExoSound->GetMaxSoundQuality() > 3) nMax = 3;
    else nMax = g_pExoSound->GetMaxSoundQuality();
    if (m_nEAX == 0) {
        HideControl(&m_control_6A4);
    } else if (m_nEAX == nMax) {
        HideControl(&m_control_4E0);
    }
}

// FUNCTION: SWKOTOR 0x006DE430
void CSWGuiOptionsFeedback::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
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
        case 0x39:
            m_control_5C4.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_control_5C4.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E6180
void CSWGuiInGameGameplay::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x28:
        case 0x2e:
            m_pManager->PlayGuiSound(0);
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
        case 0x39:
            m_control_2E4.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_control_2E4.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DFF10
void CSWGuiOptionsMain::HandleInputEvent(int nEvent, int bDown)
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
            g_pAppManager->GetClientExoApp()->SaveOptions();
            break;
        case 0x39:
            m_control_F40.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_control_F40.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006DE7C0
void CSWGuiOptionsFeedback::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_control_64.GetControl(0)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback0);
        m_control_64.GetControl(1)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback1);
        m_control_64.GetControl(2)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback2);
        m_control_64.GetControl(3)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback3);
        m_control_64.GetControl(4)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback4);
        m_control_64.GetControl(5)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback5);
        m_control_64.GetControl(6)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback9);
        m_control_64.GetControl(7)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback8);
        m_control_64.GetControl(8)->GetButtonToggle()->SetToggleState(pOptions->m_bFeedback10);
    }
}

// FUNCTION: SWKOTOR 0x006DFC90
void CSWGuiOptionsSoundAdvanced::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_nEAX = pOptions->m_nSoundQuality;
        SetEAXText();
        m_bForceSoftware = pOptions->m_bForceSoftware;
        m_control_64.GetButtonToggle()->SetToggleState(m_bForceSoftware);
        EnableEAXControls(pOptions->m_bForceSoftware == 0);
    }
    m_bChanged = 0;
}

// FUNCTION: SWKOTOR 0x006E6520
void CSWGuiInGameGameplay::SetDifficultyText()
{
    int nStrRef;
    if (g_pRules->Get2DArrays()->GetDifficultyTable()->GetINTEntry(m_nDifficulty, "NAME", &nStrRef)) {
        m_control_CD4.m_cTextParams.SetStrRef(nStrRef);
    }
    ShowControl(&m_control_105C);
    ShowControl(&m_control_E98);
    if (m_nDifficulty == 0) {
        HideControl(&m_control_105C);
    } else if (m_nDifficulty == g_pRules->Get2DArrays()->GetDifficultyTable()->m_nNumRows - 2) {
        HideControl(&m_control_E98);
    }
}

// FUNCTION: SWKOTOR 0x006E6770
void CSWGuiInGameGameplay::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        m_nDifficulty = pOptions->m_nDifficulty;
        SetDifficultyText();
        m_control_1220.GetButtonToggle()->SetToggleState(pOptions->m_bGame0);
        m_control_14D4.GetButtonToggle()->SetToggleState(pOptions->m_bGame1);
        m_control_1788.GetButtonToggle()->SetToggleState(pOptions->m_bGame2);
        m_control_1A3C.GetButtonToggle()->SetToggleState(pOptions->m_bGame3);
        m_control_1CF0.GetButtonToggle()->SetToggleState(pOptions->m_bGame4);
    }
}

// FUNCTION: SWKOTOR 0x006DF070
void CSWGuiOptionsGraphicsAdvanced::SetTexQualText()
{
    int nStrRef;
    C2DA* pTexPacks = new C2DA(CResRef("texpacks"), 0);
    if (pTexPacks) {
        if (!pTexPacks->Load2DArray()) {
            delete pTexPacks;
            return;
        }
        if (pTexPacks->m_nNumRows > 0) {
            if (pTexPacks->GetINTEntry(m_nTexQuality, "StrrefName", &nStrRef)) {
                m_control_1878.m_cTextParams.SetStrRef(nStrRef);
            }
        }
        ShowControl(&m_control_1A3C);
        ShowControl(&m_control_1C00);
        if (m_nTexQuality == 0) {
            HideControl(&m_control_1A3C);
        } else if (m_nTexQuality == 2) {
            HideControl(&m_control_1C00);
        }
        m_bChanged = 1;
        pTexPacks->Unload2DArray();
        delete pTexPacks;
    }
}

// FUNCTION: SWKOTOR 0x006DF480
void CSWGuiOptionsGraphicsAdvanced::SetUpOptions()
{
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions) {
        if (AurCheckFrameBufferModificationsAvailable()) {
            m_bFrameBuffer = pOptions->m_bFrameBuffer;
            m_control_5C4.GetButtonToggle()->SetToggleState(m_bFrameBuffer);
        } else {
            m_control_5C4.GetButtonToggle()->SetToggleState(0);
            m_control_5C4.SetEnabled(0);
            m_bFrameBuffer = 0;
        }
        m_nAnisotropy = pOptions->GetAnisotropy();
        if (AurCheckSoftShadowsAvailable()) {
            m_bSoftShadows = pOptions->m_bSoftShadows;
            m_control_878.GetButtonToggle()->SetToggleState(m_bSoftShadows);
        } else {
            m_control_878.GetButtonToggle()->SetToggleState(0);
            m_control_878.SetEnabled(0);
            m_bSoftShadows = 0;
        }
        m_bVSync = pOptions->m_bVSync;
        m_control_B2C.GetButtonToggle()->SetToggleState(m_bVSync);
        m_nAntiAlias = pOptions->m_nAntiAliasing;
        m_nTexQuality = pOptions->m_nTexturePack;
    }
    SetAntiAliasText();
    SetAnisotropyText();
    C2DA* pTexPacks = new C2DA(CResRef("texpacks"), 0);
    if (pTexPacks) {
        if (!pTexPacks->Load2DArray()) {
            delete pTexPacks;
            return;
        }
        m_nMaxTexQuality = pTexPacks->m_nNumRows - 1;
        pTexPacks->Unload2DArray();
        delete pTexPacks;
        SetTexQualText();
    }
}
