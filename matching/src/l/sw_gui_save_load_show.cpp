// CSWGuiSaveLoad::ShowGame: fills the info pane (planet, area, party
// portraits, screenshot) for the highlighted save and recolours the buttons.
#include "l/sw_gui_save_load.h"

extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vUpgradeAvailableColor;             // 0x007A2378 (descriptive)
extern Vector g_vKeyMapLockedColor;                 // 0x007A2384
extern Vector g_vKeyMapLockedTextColor;             // 0x007A2408

// FUNCTION: SWKOTOR 0x006C89D0
void CSWGuiSaveLoad::ShowGame(CSWGuiControl* pControl)
{
    if (!pControl->m_n4c) {
        return;
    }
    if (!pControl->IsSelectable()) {
        return;
    }
    CSWGuiSaveLoadEntry* pEntry = (CSWGuiSaveLoadEntry*)pControl;
    if (pEntry->m_bSavedGame && !pEntry->m_bCorrupt) {
        (&m_btnDelete)->SetEnabled(1);
        int nPos = pEntry->m_sAreaName.Find("-", 0);
        if (nPos == -1) {
            m_lblPlanetName.m_cText.SetText(pEntry->m_sAreaName);
            m_lblAreaName.m_cText.SetText("");
        } else {
            CExoString sPlanet = pEntry->m_sAreaName.Left(nPos - 1);
            CExoString sArea = pEntry->m_sAreaName.Right(pEntry->m_sAreaName.GetLength() - 2 - nPos);
            m_lblPlanetName.m_cText.SetText(sPlanet);
            m_lblAreaName.m_cText.SetText(sArea);
        }
        for (int i = 0; i < 3; i++) {
            m_aPartyMembers[i].SetFillImage(pEntry->GetPlayerPortrait(i));
        }
        if (!pEntry->m_bRebootAutoSave && !pEntry->m_bPCAutoSave) {
            m_lblScreenShot.SetFillImage("");
            if (!m_sTempDirectory.IsEmpty()) {
                g_pExoResMan->RemoveResourceDirectory(m_sTempDirectory);
            }
            m_sTempDirectory = pEntry->GetGameDirectory();
            g_pExoResMan->AddResourceDirectory(m_sTempDirectory);
            m_lblScreenShot.SetFillImage("screen");
        } else {
            m_lblScreenShot.SetFillImage(pEntry->m_sScreenShot);
        }
        m_lblScreenShot.m_cText.SetText("");
        if (pEntry->m_bCheatUsed) {
            m_lblScreenShot.m_cText.SetText("Cheat Used");
            m_lblScreenShot.m_cText.SetColor(g_vAbilityColorGranted);
            m_lblScreenShot.m_bVisible = 1;
        }
    } else {
        m_lblPlanetName.m_cText.SetText("");
        m_lblAreaName.m_cText.SetText("");
        for (int i = 0; i < 3; i++) {
            m_aPartyMembers[i].SetFillImage(CResRef());
        }
        m_lblScreenShot.SetFillImage(CResRef());
        m_lblScreenShot.m_cText.SetStrRef(0x636);
        (&m_btnDelete)->SetEnabled(0);
    }
    if (!m_bSave) {
        m_btnSaveLoad.m_cTextParams.SetColor(pEntry->m_bMissingContent ? g_vKeyMapLockedTextColor : g_vDifficultyExitColor);
        m_btnSaveLoad.m_cBorderParams.SetColor(pEntry->m_bMissingContent ? g_vKeyMapLockedColor : g_vUpgradeAvailableColor);
    } else {
        m_btnSaveLoad.m_cTextParams.SetColor(g_vDifficultyExitColor);
        m_btnSaveLoad.m_cBorderParams.SetColor(g_vUpgradeAvailableColor);
    }
    m_btnDelete.m_cTextParams.SetColor(pEntry->m_bSavedGame ? g_vDifficultyExitColor : g_vKeyMapLockedTextColor);
    m_btnDelete.m_cBorderParams.SetColor(pEntry->m_bSavedGame ? g_vUpgradeAvailableColor : g_vKeyMapLockedColor);
}
