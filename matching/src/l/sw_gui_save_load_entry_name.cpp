// Save/load list entry caption: quick save / autosave labels or the
// numbered "Game N - hh:mm - name" string built from dialog.tlk tokens.
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006C9780
void CSWGuiSaveLoadEntry::SetName(const CExoString& sName)
{
    m_sDisplayName = sName;
    CExoString sUnused;
    if (m_nGameNumber == 1) {
        m_sDisplayName = g_pTlkTable->GetSimpleString(0x639);
    } else if (m_nGameNumber == 0) {
        m_sDisplayName = g_pTlkTable->GetSimpleString(0xbb77);
    }
    CExoString sText;
    int nHours = m_nTimePlayed / 3600;
    int nMinutes = m_nTimePlayed / 60 % 60;
    if (m_nGameNumber == 0 || m_nGameNumber == 1) {
        g_pTlkTable->SetCustomToken(0, nHours);
        g_pTlkTable->SetCustomToken(1, nMinutes);
        sText = g_pTlkTable->GetSimpleString(0xbc4d);
        g_pTlkTable->ParseTokens(&sText);
        m_sDisplayName = m_sDisplayName + sText;
    } else {
        g_pTlkTable->SetCustomToken(0, m_nGameNumber - 1);
        g_pTlkTable->SetCustomToken(1, nHours);
        g_pTlkTable->SetCustomToken(2, nMinutes);
        g_pTlkTable->SetCustomToken(3, m_sSaveName);
        sText = g_pTlkTable->GetSimpleString(0xbd87);
        g_pTlkTable->ParseTokens(&sText);
        m_sDisplayName = sText;
    }
    m_cTextParams.SetText(m_sDisplayName);
}
