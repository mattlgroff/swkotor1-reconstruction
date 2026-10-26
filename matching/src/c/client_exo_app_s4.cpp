// CClientExoApp virtual slots (CBaseExoApp interface) and two leftover
// facade accessors, worker c round s4. All are real function starts that
// functions.csv lacks. Names are the playable source's.

#include "c/client_exo_app.h"

extern float g_fFPS;                                // 0x007A22B4

extern "C" __declspec(dllimport) void __stdcall PostQuitMessage(int nExitCode);

struct CGuiQuitPanel {
    char pad_0x0[0x64];
    unsigned char m_nFlags;                         // 0x64
};

class CGuiInGame {
public:
    char pad_0x0[0x98];
    CGuiQuitPanel* m_pQuitPanel;                    // 0x98
};

// FUNCTION: SWKOTOR 0x005ED520
CConnectionLib* CClientExoApp::GetConnectionLib()
{
    return m_pcInternal->m_pConnectionLib;
}

// FUNCTION: SWKOTOR 0x005ED6E0
CNetLayer* CClientExoApp::GetNetLayer()
{
    return m_pcInternal->m_pNetLayer;
}

// FUNCTION: SWKOTOR 0x005ED790
int CClientExoApp::HandleMessage(unsigned long nPlayerId, unsigned char* pData, unsigned long nSize, int bRaw)
{
    return m_pcInternal->HandleMessage(nPlayerId, pData, nSize, bRaw);
}

// FUNCTION: SWKOTOR 0x005ED7A0
int CClientExoApp::ContinueMessageProcessing()
{
    return m_pcInternal->ContinueMessageProcessing();
}

// FUNCTION: SWKOTOR 0x005ED890
void CClientExoApp::ShutDownToMainMenu()
{
    m_pcInternal->ShutDownToMainMenu();
}

// FUNCTION: SWKOTOR 0x005ED8A0
void CClientExoApp::SetApplicationIdsMatch(int bMatch)
{
    m_pcInternal->m_bApplicationIdsMatch = bMatch;
}

// FUNCTION: SWKOTOR 0x005ED990
unsigned long CClientExoApp::GetApplicationId()
{
    return g_pAppManager->m_nApplicationId;
}

// FUNCTION: SWKOTOR 0x005EE350
float CClientExoApp::GetFPS()
{
    return g_fFPS;
}

// Exit-game confirmation from the in-game quit panel: a real start missing
// from functions.csv, emitted right after StopLoadMusic.
// FUNCTION: SWKOTOR 0x005F1BD0
void CClientExoAppInternal::HandleExitGameButton(CSWGuiControl* pControl)
{
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_pQuitPanel->m_nFlags & 1) {
        m_bUnknown250 = 1;
        m_bUnknown254 = 1;
        PostQuitMessage(0);
    }
}
