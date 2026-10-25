#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x0061B6F0
void CClientOptions::SetCameraMode(unsigned char nMode)
{
    m_nCameraMode = nMode;
    if (nMode == 1 || nMode == 0) {
        m_nCameraModeCurrent = nMode;
    }
}

// FUNCTION: SWKOTOR 0x0061B710
void CClientOptions::SetFullScreenEnabled(int bEnabled)
{
    unsigned long nRequested = bEnabled;
    unsigned long nFlagsBefore = m_nFlags;
    if (nRequested == ((nFlagsBefore >> 3) & 1)) {
        return;
    }

    g_bReinitializeVideoMode = (nRequested != 0 ? 1 : 0) + 1;
    unsigned long nFlags = m_nFlags;
    nFlags ^= ((nRequested << 3) ^ nFlags) & 8;
    m_nFlags = nFlags;
}

// FUNCTION: SWKOTOR 0x0061B740
void CClientOptions::SetAutoLevelUpNPCs(int bEnabled)
{
    unsigned long nOldFlags = m_nGameplayFlags;
    unsigned long nChange = (nOldFlags ^ (unsigned long)bEnabled) & 1;
    m_nGameplayFlags ^= nChange;
    if ((m_nGameplayFlags & 1) == 0) {
        return;
    }

    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (pServer == 0) {
        return;
    }
    pServer->GetPartyTable()->AutoLevelUpNPCs();
}
