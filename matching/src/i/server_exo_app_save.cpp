// CServerExoAppInternal members that pass a CResRef temporary by
// value (built in place, so CResRef's copy constructor is declared here).
#define I_CRESREF_DECLARED_COPY
#include "i/server_exo_app.h"

// FUNCTION: SWKOTOR 0x004B20E0
int CServerExoAppInternal::IncludeModuleInSave(const CExoString& sModule)
{
    int nInclude;
    C2DA* p2DA = new C2DA(CResRef("modulesave"), 0);
    if (p2DA == NULL) {
        return 1;
    }
    if (!p2DA->Load2DArray()) {
        delete p2DA;
        return 1;
    }
    bool bFailed;
    {
        CExoString sColumn("IncludeInSave");
        bFailed = !p2DA->GetINTEntry(sModule, sColumn, &nInclude);
    }
    if (bFailed) {
        delete p2DA;
        return 1;
    }
    delete p2DA;
    return nInclude != 0;
}

class CGuiInGame {
public:
    void SetGlobalDialogState(int nState);                          // 0x0062EC60
    void CloseDialog();                                             // 0x006332B0
};

class CClientExoApp {
public:
    int GetNextLoadScreenHintSTRREF(CExoString& sModule, int nArg); // 0x005EE090
    void SetLoadScreenHint(int nStrRef);                            // 0x005EDF00
    void SetLoadScreenHintUpdateTimer(int nTime);                   // 0x005EE040
    void SetCanSendDialog(int bCan);                                // 0x005EDC80
    CGuiInGame* GetInGameGui();                                     // 0x005ED690
    void PlayLoadModuleMovies();                                    // 0x005EDB20
};

class CExoSound {
public:
    void StopAllOneShots();                                         // 0x005D5E70
};
extern CExoSound* g_pExoSound;                                      // 0x007A39EC

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                                 // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC

// FUNCTION: SWKOTOR 0x004BA920
int CServerExoAppInternal::StartNewModule(CExoString& sModule)
{
    if (GetLoadingModule() == 1) {
        return 0;
    }
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    pClient->SetLoadScreenHint(pClient->GetNextLoadScreenHintSTRREF(sModule, 0));
    g_pAppManager->m_pClientExoApp->SetLoadScreenHintUpdateTimer(0);
    g_pAppManager->m_pClientExoApp->SetCanSendDialog(0);
    g_pAppManager->m_pClientExoApp->GetInGameGui()->SetGlobalDialogState(0);
    g_pAppManager->m_pClientExoApp->GetInGameGui()->CloseDialog();
    g_pExoSound->StopAllOneShots();
    g_pAppManager->m_pClientExoApp->PlayLoadModuleMovies();

    g_pExoResMan->AddResourceDirectory("MODULES:");
    C2DA* p2DA = NULL;
    if (!g_pExoResMan->Exists(sModule, 0x7db, NULL) &&
        !g_pExoResMan->Exists(sModule, 0xbba, NULL)) {
        g_pExoResMan->RemoveResourceDirectory("MODULES:");
        return 0;
    }
    g_pExoResMan->RemoveResourceDirectory("MODULES:");

    m_bLoadingModule = 1;
    m_bDoingStartNewModule = 1;
    ForceRunAreaOnExitScripts();
    if (StorePlayerCharacters() && StoreCurrentModule()) {
        p2DA = new C2DA(CResRef("modulesave"), 0);
        if (p2DA && !p2DA->Load2DArray()) {
            delete p2DA;
            p2DA = NULL;
        }
        DoModuleEnterSaveCleanup(sModule, p2DA);
        if (GetModule()) {
            if (!p2DA || AutoSaveOnEnteringModule(sModule, p2DA)) {
                DoPCAutosave(sModule, p2DA);
            }
        }
        if (p2DA) {
            delete p2DA;
        }
        return ExecuteLoadModule(sModule);
    }
    m_bDoingStartNewModule = 0;
    return 0;
}
