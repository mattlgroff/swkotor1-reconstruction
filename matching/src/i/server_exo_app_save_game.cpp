// CServerExoAppInternal::SaveGame.
#include "i/server_exo_app.h"

class CAurBehavior;

class CSWCameraOnAStick {
public:
    void OrientTowardTarget();                                      // 0x00638E80
};

class CSWCModuleCamera {
public:
    virtual void m00(); virtual void m04(); virtual void m08(); virtual void m0c();
    virtual void m10(); virtual void m14(); virtual void m18(); virtual void m1c();
    virtual void m20(); virtual void m24(); virtual void m28(); virtual void m2c();
    virtual void m30(); virtual void m34(); virtual void m38(); virtual void m3c();
    virtual void m40(); virtual void m44(); virtual void m48(); virtual void m4c();
    virtual void m50(); virtual void m54(); virtual void m58(); virtual void m5c();
    virtual void m60(); virtual void m64(); virtual void m68(); virtual void m6c();
    virtual void m70(); virtual void m74(); virtual void m78(); virtual void m7c();
    virtual CSWCameraOnAStick* GetBehavior(int nType);              // +0x80
};

class CSWCModule {
public:
    char m_pad0[0x40];
    CSWCModuleCamera* m_pCamera;                                    // +0x40
    CSWCModuleCamera* GetCamera() { return m_pCamera; }
};

class CSWGuiManager {
public:
    void Draw(float fDelta);                                        // 0x0040CC50
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                                        // 0x005ED530
    CSWGuiManager* GetGuiManager();                                 // 0x005EDA40
};

// Save/load progress block hung off the app manager (placeholder name).
#include "shared/reentrant_server_stats.h"

#include "shared/app_manager.h"
#include "shared/globals.h"
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CExoSound {
public:
    void SetMusicState(int nState);                                 // 0x005D5E80
};

void PlayDelayedSound(float fDelay, int a, int b);                  // 0x00401C10

// FUNCTION: SWKOTOR 0x004B58A0
int CServerExoAppInternal::SaveGame(unsigned long nSlot, CExoString& sName, CSWSPlayer* pPlayer,
                                    int bUnused)
{
    CExoString sPath;
    CExoString sScreenShot;
    g_pExoSound->SetMusicState(SOUND_MODE_MOVIE);
    g_pExoResMan->CreateDirectory2("SAVES:");
    sPath.Format("SAVES:%06d - %s", nSlot, sName.CStr());

    unsigned __int64 nFreeBytes;
    int bGotFreeSpace = g_pExoResMan->GetFreeDiskSpace("SAVES:", &nFreeBytes);
    if (bGotFreeSpace == 1) {
        unsigned __int64 nNeeded = m_nEstimatedSaveSize;
        if (nNeeded > g_pExoBase->GetDirectorySize(sPath) + nFreeBytes) {
            GetServerMessage()->SendServerToPlayerSaveLoad_Status(pPlayer, 2, 1);
            return 0;
        }
    }
    if (!g_pExoResMan->CreateDirectory2(sPath)) {
        if (!g_pExoResMan->Leaf9460(sPath, 0, 0)) {
            return 0;
        }
    }
    sScreenShot = sPath + "\\" + "SAVEGAME";

    CSWReentrantServerStats* pStatus = g_pAppManager->GetReentrantServerStats();
    pStatus->m_bActive = 1;
    pStatus->m_nKind = 2;
    pStatus->m_n10 = 1;
    pStatus->m_nLoadError = 0;
    g_pAppManager->GetReentrantServerStats()->m_sDirectory = sPath;
    g_pAppManager->GetReentrantServerStats()->m_sSaveGame = sScreenShot;
    g_pAppManager->GetReentrantServerStats()->m_nStep = 0;
    g_pAppManager->GetReentrantServerStats()->m_nProgress = 0;
    CSWSMessage* pMessage = GetServerExoApp()->FUN_004aedb0();
    pMessage->SendServerToPlayerLoadBar_StartStallEvent(2);

    sPath = *g_pExoBase->m_pcExoAliasList->GetAliasPath("SAVES", 0);
    if (sPath != "") {
        CSWCameraOnAStick* pCamera =
            g_pAppManager->m_pClientExoApp->GetModule()->GetCamera()->GetBehavior(0x106a);
        if (pCamera) {
            pCamera->OrientTowardTarget();
        }
        sScreenShot.Format("%s%06d - %s%sScreen", sPath.CStr(), nSlot, sName.CStr(), "\\");
        g_pAppManager->DoSaveGameScreenShot(sScreenShot);
    }
    if (nSlot != 0) {
        g_pAppManager->m_pClientExoApp->GetGuiManager()->Draw(1.0f / 30.0f);
        PlayDelayedSound(1.0f / 30.0f, 0, 0);
    }
    m_nAutoSaveElapsed = 0;
    m_bAutoSaveDue = 0;
    return 1;
}
