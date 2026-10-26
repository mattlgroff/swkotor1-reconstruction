// CSWSObject dialog pacing: IsDialogDelay (0x004CD2D0) and UpdateDialog
// (0x004CD580).
#include "i/sws_object.h"

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    int AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDay,
                      unsigned long* pTime);                        // 0x004ADF50
    int CompareWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                          unsigned long nTime2);                    // 0x004ADFA0
};

class CGuiInGame {
public:
    int IsFadingInDialog();                         // 0x0062ABB0
    int IsAnimationPlayingInDialog();               // 0x0062D570
    void UpdateDialogLipSync();                     // 0x0062FA30
    char pad_0x0[0xc0];
    void* m_pc0;                                    // 0xc0 (placeholder name)
};

class CSWCModule {
public:
    int IsCameraAnimated();                         // 0x006412B0
};

// FUNCTION: SWKOTOR 0x004CD2D0
int CSWSObject::IsDialogDelay()
{
    unsigned long nTime;
    unsigned long nDay;
    unsigned long oidOwner = m_pDialog->GetOwnerInline();
    if (!g_pAppManager->m_pClientExoApp->GetCanSendDialog(m_idSelf, oidOwner, (int)m_pDialog)) {
        return 1;
    }
    unsigned long nFlags = m_nDialogDelayFlags;
    int bDelay = 0;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    if (nFlags & 2) {
        bDelay = g_pAppManager->m_pClientExoApp->IsSoundPlayingInDialog();
    }
    if (nFlags & 4) {
        bDelay |= g_pAppManager->m_pClientExoApp->GetInGameGui()->IsAnimationPlayingInDialog();
    }
    if (nFlags & 1) {
        bDelay |= g_pAppManager->m_pClientExoApp->GetModule()->IsCameraAnimated();
        g_pAppManager->m_pClientExoApp->GetInGameGui();
        g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    }
    if (nFlags & 8) {
        bDelay |= g_pAppManager->m_pClientExoApp->GetInGameGui()->IsFadingInDialog();
    }
    if (m_n1f4 && m_pDialog->m_bSkippable) {
        m_bStopSoundPlaying = 1;
        g_pAppManager->m_pClientExoApp->StopSoundPlayingInDialog();
        GetServerExoApp()->GetWorldTimer()->AddWorldTimes(nDay, nTime, 0, 1,
                                                          (unsigned long*)&m_n38,
                                                          (unsigned long*)&m_n3c);
        return 0;
    }
    if (bDelay) {
        return 1;
    }
    return GetServerExoApp()->GetWorldTimer()->CompareWorldTimes(nDay, nTime, m_n38, m_n3c) == -1;
}

// FUNCTION: SWKOTOR 0x004CD580
int CSWSObject::UpdateDialog()
{
    if (!IsDialogOwnerInline()) {
        return 0;
    }
    g_pAppManager->m_pClientExoApp->GetInGameGui()->UpdateDialogLipSync();
    m_pDialog->ClearAllPlayerHostileActions();
    if (m_n50) {
        return 1;
    }
    if (!IsDialogDelay() && m_n48) {
        if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pc0 || !m_bStopSoundPlaying ||
            !g_pAppManager->m_pClientExoApp->IsSoundPlayingInDialog()) {
            int nEntry = m_n4c;
            m_n48 = 0;
            m_bStopSoundPlaying = 0;
            if (nEntry != -1) {
                return SendDialogEntry(nEntry);
            }
            return SendDialogReplies();
        }
    }
    else if (m_pDialog->m_p60 && !g_pAppManager->m_pClientExoApp->IsSoundPlayingInDialog()) {
        StopDialog();
    }
    return 1;
}
