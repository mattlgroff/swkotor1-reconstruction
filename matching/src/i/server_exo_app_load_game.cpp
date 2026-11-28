// CServerExoAppInternal::LoadGame.
#include "i/server_exo_app.h"

class CClientExoApp {
public:
    void SetLoadBarProgress(int nProgress, int bShow);              // 0x005EDD40
    void SetTutorialFlag(int nFlag, int nValue);                    // 0x005EDFD0
};

#include "shared/app_manager.h"
#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x004BA640
int CServerExoAppInternal::LoadGame(unsigned long nSlot, CExoString& sName, CExoString& sModule,
                                    int bLoadFromSave)
{
    CExoString sDirectory;
    CExoString sSaveName;
    CExoString sUnused1;
    CExoString sUnused2;

    g_pAppManager->m_pClientExoApp->SetLoadBarProgress(10, 1);
    g_pAppManager->m_pClientExoApp->SetTutorialFlag(0, 10);
    g_pAppManager->m_pClientExoApp->SetTutorialFlag(1, 20);
    g_pAppManager->m_pClientExoApp->SetTutorialFlag(2, 23);
    g_pAppManager->m_pClientExoApp->SetTutorialFlag(3, 23);
    g_pAppManager->m_pClientExoApp->SetTutorialFlag(4, 23);

    if (nSlot != 0xffffffff) {
        sSaveName.Format("%06d - %s", nSlot, sName.CStr());
    } else {
        sSaveName = sName;
    }
    sDirectory = "SAVES:";
    sDirectory = sDirectory + sSaveName;
    g_pExoResMan->AddResourceDirectory(sDirectory);
    m_c1b770.LoadTableInfo();
    m_cGlobalVariableTable.Load();
    g_pExoResMan->RemoveResourceDirectory(sDirectory);
    m_c1b770.m_n10c = 1;
    m_nAutoSaveElapsed = 0;
    m_bAutoSaveDue = 0;
    return LoadModule(sModule, bLoadFromSave);
}
