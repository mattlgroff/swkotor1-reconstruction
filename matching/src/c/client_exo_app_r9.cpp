// CClientExoApp facade accessors, worker C r9 (unnamed FUN_ entries in
// reverse/functions.csv, 0x005ED360 and 0x005EE020-0x005EE110).

#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x005ED360
void CClientExoApp::CallUnknown5FCD10(int a, int b, const CExoString& s)
{
    if (s == "") {
        m_pcInternal->Unknown5FCD10(a, b, "");
    } else {
        m_pcInternal->Unknown5FCD10(a, b, s);
    }
}

// FUNCTION: SWKOTOR 0x005EE020
CClientResRefSlot* CClientExoApp::GetUnknown418(int n)
{
    return &m_pcInternal->m_aUnknown418[n];
}

// FUNCTION: SWKOTOR 0x005EE0A0
void CClientExoApp::SetModuleResourcesLoadedFlag(unsigned char n)
{
    m_pcInternal->m_bModuleResourcesLoaded |= n;
}

// FUNCTION: SWKOTOR 0x005EE0D0
int CClientExoApp::GetModuleResourcesLoadedFlag(unsigned char n)
{
    return (m_pcInternal->m_bModuleResourcesLoaded & n) > 0;
}

// FUNCTION: SWKOTOR 0x005EE0F0
int CClientExoApp::IsUnknown378Positive()
{
    if (m_pcInternal->m_fUnknown378 > 0.0f) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005EE110
int CClientExoApp::GetPausedByCombatSet()
{
    return m_pcInternal->m_bPausedByCombatSet;
}

extern "C" int __cdecl atoi(const char* s);

// Console command handler: fixed frame rate for all client and server timers.
// FUNCTION: SWKOTOR 0x005EE4A0
const char* SetFramesPerSecond(const char* sArgs)
{
    int nRate = atoi(sArgs);
    if (nRate) {
        g_pAppManager->m_pClientExoApp->GetWorldTimer()->SetFixedSnapshotRate(1, nRate);
        g_pAppManager->m_pClientExoApp->GetPauseTimer()->SetFixedSnapshotRate(1, nRate);
        g_pAppManager->m_pClientExoApp->GetAnimationTimer()->SetFixedSnapshotRate(1, nRate);
        g_pAppManager->m_pServerExoApp->GetWorldTimer()->SetFixedSnapshotRate(1, nRate);
        g_pAppManager->m_pServerExoApp->GetPauseTimer()->SetFixedSnapshotRate(1, nRate);
        return "Frame rate set.";
    }
    g_pAppManager->m_pClientExoApp->GetWorldTimer()->SetFixedSnapshotRate(0, 0);
    g_pAppManager->m_pClientExoApp->GetPauseTimer()->SetFixedSnapshotRate(0, 0);
    g_pAppManager->m_pClientExoApp->GetAnimationTimer()->SetFixedSnapshotRate(0, 0);
    g_pAppManager->m_pServerExoApp->GetWorldTimer()->SetFixedSnapshotRate(0, 0);
    g_pAppManager->m_pServerExoApp->GetPauseTimer()->SetFixedSnapshotRate(0, 0);
    return "Fixed frame rate";
}
