// CServerExoAppInternal load-error / pause-toggle / module-load
// entry points (0x004BA5B0-0x004BA918).
#include "i/server_exo_app.h"

struct CAppManager14 {
    int m_n0;                                       // +0x00
    int m_n4;                                       // +0x04
    char m_pad08[0x14 - 0x08];
    int m_bLoadError;                               // +0x14 (placeholder name)
    char m_pad18[0x38 - 0x18];
    unsigned long m_nLoadError;                     // +0x38 (placeholder name)
};

class CWorldTimer {
public:
    void SetSnapshotTime();                         // 0x004ADBD0
    void PauseWorldTimer();                         // 0x004ADFF0
};

class CSWSMessage2 {
public:
    void SendServerToPlayerLoadBar_EndStallEvent(int nArg, unsigned long nError);  // 0x0056C980
    void SendServerToPlayerModule_StartStartNewModule();                           // 0x0056CBF0
};

class CExoInput {
public:
    void ClearEvents();                             // 0x005DF4E0
};
extern CExoInput* g_pExoInput;                      // 0x007A39E4

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    void ResetDriveAcceleration();                  // 0x005EDBA0
    void DisableInput();                            // 0x005EDBE0
};

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
    char m_pad0c[0x14 - 0x0c];
    CAppManager14* m_p14;                           // +0x14

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern unsigned char g_nAppMode;                    // 0x007A39DC (placeholder name)
extern int g_nDriveForward;                         // 0x008338F0 (placeholder name)
extern int g_nDriveTurn;                            // 0x008338F4 (placeholder name)
extern int g_nDriveStrafe;                          // 0x008338F8 (placeholder name)

void PrintModuleName(const char* sName);            // 0x00401430 (placeholder name)

// FUNCTION: SWKOTOR 0x004BA5B0
void CServerExoAppInternal::DealWithLoadGameError(unsigned long nError)
{
    CAppManager14* pState = g_pAppManager->m_p14;
    UnloadModule();
    pState->m_nLoadError = nError;
    pState->m_bLoadError = 1;
    if (g_nAppMode == 2) {
        pState->m_bLoadError = 0;
        pState->m_n0 = 0;
        pState->m_n4 = 0;
    }
    m_nServerMode = 0;
    m_bLoadingModule = 0;
    ((CSWSMessage2*)g_pAppManager->GetServerExoApp()->FUN_004aedb0())
        ->SendServerToPlayerLoadBar_EndStallEvent(1, nError);
}

// FUNCTION: SWKOTOR 0x004BA610
void CServerExoAppInternal::TogglePauseState(unsigned char nState)
{
    SetPauseState(nState, (unsigned char)(m_nPauseState & nState) <= (unsigned char)0);
}

// FUNCTION: SWKOTOR 0x004BA820
int CServerExoAppInternal::ExecuteLoadModule(CExoString& sModule)
{
    if (GetModule() && !GetModule()->GetModuleNamePtr()->IsEmpty()) {
        PrintModuleName(GetModule()->GetModuleNamePtr()->CStr());
    }
    else {
        PrintModuleName("");
    }
    ((CSWSMessage2*)g_pAppManager->GetServerExoApp()->FUN_004aedb0())
        ->SendServerToPlayerModule_StartStartNewModule();
    g_pExoInput->ClearEvents();
    g_nDriveForward = 0;
    g_nDriveTurn = 0;
    g_nDriveStrafe = 0;
    g_pAppManager->m_pClientExoApp->ResetDriveAcceleration();
    g_pAppManager->m_pClientExoApp->GetWorldTimer()->PauseWorldTimer();
    ((CWorldTimer*)g_pAppManager->GetServerExoApp()->FUN_004aede0())->PauseWorldTimer();
    g_pAppManager->m_pClientExoApp->GetWorldTimer()->SetSnapshotTime();
    ((CWorldTimer*)g_pAppManager->GetServerExoApp()->FUN_004aede0())->SetSnapshotTime();
    g_pAppManager->m_pClientExoApp->DisableInput();
    return LoadModule(sModule, 0);
}
