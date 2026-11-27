// CServerExoAppInternal load-error / pause-toggle / module-load
// entry points (0x004BA5B0-0x004BA918).
#include "i/server_exo_app.h"

#include "shared/reentrant_server_stats.h"

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
#include "shared/globals.h"

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    void ResetDriveAcceleration();                  // 0x005EDBA0
    void DisableInput();                            // 0x005EDBE0
};

#include "shared/app_manager.h"


void PrintModuleName(const char* sName);            // 0x00401430 (placeholder name)

// FUNCTION: SWKOTOR 0x004BA5B0
void CServerExoAppInternal::DealWithLoadGameError(unsigned long nError)
{
    CSWReentrantServerStats* pState = g_pAppManager->m_pReentrantServerStats;
    UnloadModule();
    pState->m_nLoadError = nError;
    pState->m_n14 = 1;
    if (g_nAppMode == 2) {
        pState->m_n14 = 0;
        pState->m_bActive = 0;
        pState->m_nKind = 0;
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
