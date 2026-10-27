// CServerExoAppInternal service start/stop.
#include "i/server_exo_app.h"

class CWorldTimer {
public:
    CWorldTimer();                                                  // 0x004AE4C0
    ~CWorldTimer();                                                 // 0x004015A0 (shared empty body)
    char m_pad0[0x44];
};

class CServerAIMaster {
public:
    CServerAIMaster();                                              // 0x004B0780
    void SetExoAppInternal(CServerExoAppInternal* p);               // 0x00604790 (shared first-dword setter)
    void ClearEventQueue();                                         // 0x004B11C0
    ~CServerAIMaster();                                             // 0x004B1200
    char m_pad0[0x90];
};

class CFactionManager {
public:
    CFactionManager();                                              // 0x0052B8E0
    ~CFactionManager();                                             // 0x0052B950
    char m_pad0[0x28];
};

class CVirtualMachine {
public:
    CVirtualMachine();                                              // 0x005D0F40
    void SetCommandImplementer(CSWVirtualMachineCommands* p);      // 0x005D0FF0
    ~CVirtualMachine();                                             // 0x005D0FB0
    char m_pad0[4];
};
extern CVirtualMachine* g_pVirtualMachine;                          // 0x007A3A00

class CScriptCompiler {
public:
    ~CScriptCompiler();                                             // 0x005D1120
};
extern CScriptCompiler* g_pScriptCompiler;                          // 0x007A3A04 (placeholder name)

class CTlkTable {
public:
    CTlkTable();                                                    // 0x0041D8D0
    int OpenFile(const CExoString& sFile);                          // 0x0041E5A0 (placeholder name)
    void ClearCustomTokens();                                       // 0x0041DCF0
    ~CTlkTable();                                                   // 0x0041E100
    char m_pad0[0x58];
};
extern CTlkTable* g_pTlkTable;                                      // 0x007A3A08

extern unsigned char g_nAppMode;                                    // 0x007A39DC

class CSWSRules : public CSWRules {
public:
    CSWSRules();                                                    // 0x0057A1F0
};

class CObjectTableManager {
public:
    int RemovePlayer(unsigned long nPlayerId);                      // 0x004AE650
};

class CSWParty {
public:
    void ClearParty();                                              // 0x00634A40
};

class CClientExoApp {
public:
    CSWParty* GetParty();                                           // 0x005ED8B0
    int GetSoundPausedByCombat();                                   // 0x005EE2D0
};

class CExoSound {
public:
    void SetMusicState(int nState);                                 // 0x005D5E80
};
extern CExoSound* g_pExoSound;                                      // 0x007A39EC
extern const int MUSIC_STATE_NORMAL;                                // 0x0074C5EC

class CAppManager {
public:
    CObjectTableManager* GetObjectTableManager(int nTable);         // 0x00401060
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                                 // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

// FUNCTION: SWKOTOR 0x004B7C60
void CServerExoAppInternal::StopServices()
{
    CExoString sTimestamp;
    g_pExoBase->m_pcExoDebug->GetCurrentTimestamp(sTimestamp);
    sTimestamp.CStr();

    Shutdown(0, 0);

    if (m_pGameObjArray) {
        delete m_pGameObjArray;
        m_pGameObjArray = NULL;
    }
    if (m_pServerAIMaster) {
        delete m_pServerAIMaster;
        m_pServerAIMaster = NULL;
    }
    if (m_pServerMessage) {
        delete m_pServerMessage;
        m_pServerMessage = NULL;
    }
    if (m_pServerInfo) {
        delete m_pServerInfo;
        m_pServerInfo = NULL;
    }
    if (m_pNetLayer) {
        m_pNetLayer->EndServerMode();
        m_pNetLayer->ShutDown();
        delete m_pNetLayer;
        m_pNetLayer = NULL;
    }
    if (g_pVirtualMachine) {
        delete g_pVirtualMachine;
        g_pVirtualMachine = NULL;
    }
    if (m_pVirtualMachineCommands) {
        delete m_pVirtualMachineCommands;
        m_pVirtualMachineCommands = NULL;
    }
    if (g_pScriptCompiler) {
        delete g_pScriptCompiler;
        g_pScriptCompiler = NULL;
    }

    {
        CExoString sDir("TEMP:");
        g_pExoResMan->NukeDirectory(sDir, 1, 1);
    }
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->NukeDirectory(sDir, 1, 1);
    }
    {
        CExoString sDir("GAMEINPROGRESS:");
        g_pExoResMan->NukeDirectory(sDir, 1, 1);
    }

    if (g_nAppMode == 2) {
        if (g_pRules) {
            delete g_pRules;
            g_pRules = NULL;
        }
        {
            CExoString sDir("HD0:chitin");
            g_pExoResMan->RemoveFixedKeyTableFile(sDir);
        }
        {
            CExoString sDir("OVERRIDE:");
            g_pExoResMan->RemoveResourceDirectory(sDir);
        }
        {
            CExoString sDir("ERRORTEX:");
            g_pExoResMan->RemoveResourceDirectory(sDir);
        }
        {
            CExoString sDir("SERVERVAULT:");
            g_pExoResMan->RemoveResourceDirectory(sDir);
        }
        if (g_pTlkTable) {
            delete g_pTlkTable;
            g_pTlkTable = NULL;
        }
        if (g_pExoResMan) {
            delete g_pExoResMan;
            g_pExoResMan = NULL;
        }
    }

    if (m_pWorldTimer) {
        delete m_pWorldTimer;
        m_pWorldTimer = NULL;
    }
    if (m_pTimestopTimer) {
        delete m_pTimestopTimer;
        m_pTimestopTimer = NULL;
    }
    if (m_pPauseTimer) {
        delete m_pPauseTimer;
        m_pPauseTimer = NULL;
    }
    if (m_pFactionManager) {
        delete m_pFactionManager;
        m_pFactionManager = NULL;
    }
}

// FUNCTION: SWKOTOR 0x004B63E0
void CServerExoAppInternal::StartServices(int nMode)
{
    CExoString sTimestamp;
    g_pExoBase->m_pcExoDebug->GetCurrentTimestamp(sTimestamp);
    sTimestamp.CStr();

    m_pFactionManager = new CFactionManager;
    m_pWorldTimer = new CWorldTimer;
    m_pTimestopTimer = new CWorldTimer;
    m_pPauseTimer = new CWorldTimer;
    m_pGameObjArray = new CGameObjectArray(1);
    if (g_nAppMode == 2) {
        g_pExoResMan = new CExoResMan;
    }
    m_pNetLayer = new CNetLayer;
    if (!m_pServerInfo) {
        m_pServerInfo = new CServerInfo;
    }
    m_pServerMessage = new CSWSMessage;
    m_pServerAIMaster = new CServerAIMaster;
    m_pServerAIMaster->SetExoAppInternal(this);

    if (g_nAppMode == 2) {
        {
            CExoString sDir("OVERRIDE:");
            g_pExoResMan->AddResourceDirectory(sDir);
        }
        {
            CExoString sDir("ERRORTEX:");
            g_pExoResMan->AddResourceDirectory(sDir);
        }
        {
            CExoString sDir("HD0:chitin");
            g_pExoResMan->AddFixedKeyTableFile(sDir);
        }
        {
            CExoString sDir("SERVERVAULT:");
            g_pExoResMan->AddResourceDirectory(sDir);
        }
    }
    {
        CExoString sDir("HD0:TEMP");
        g_pExoResMan->RemoveFile(sDir, 0x56);
    }
    {
        CExoString sDir("TEMP:");
        g_pExoResMan->CreateDirectory2(sDir);
    }
    {
        CExoString sDir("GAMEINPROGRESS:");
        g_pExoResMan->CreateDirectory2(sDir);
    }
    if (!g_pTlkTable) {
        g_pTlkTable = new CTlkTable;
        {
            CExoString sFile("HD0:dialog");
            g_pTlkTable->OpenFile(sFile);
        }
    }

    g_pVirtualMachine = new CVirtualMachine;
    m_pVirtualMachineCommands = new CSWVirtualMachineCommands;
    g_pVirtualMachine->SetCommandImplementer(m_pVirtualMachineCommands);

    if (g_nAppMode != 2) {
        CExoString sLog("swserver");
        g_pExoBase->m_pcExoDebug->OpenLogFiles(sLog);
    }

    m_nShutdownDelay = 180000000;
    m_pNetLayer->Initialize(GetServerExoApp());
    GetServerInfoFromIniFile();
    if (g_nAppMode == 2) {
        g_pRules = new CSWSRules;
    } else {
        DumpServerOptionsToLog();
    }

    m_nThrottleLast = 0;
    m_nThrottleRemaining = 0;
    m_n100cc = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    m_n100d0 = 0;
    m_nAutoSaveElapsed = 0;
    m_bAutoSaveDue = 0;
}

// FUNCTION: SWKOTOR 0x004B9240
int CServerExoAppInternal::UnloadModule()
{
    CGameObjectArray* pGameObjArray = m_pGameObjArray;
    CSWSModule* pModule = GetModule();
    CSWSMessage* pMessage = GetServerExoApp()->FUN_004aedb0();

    if (g_pAppManager->m_pClientExoApp &&
        g_pAppManager->m_pClientExoApp->GetSoundPausedByCombat()) {
        g_pExoSound->SetMusicState(MUSIC_STATE_NORMAL);
    }
    SetPauseState(2, 0);
    SetPauseState(1, 0);

    delete m_pExclusionList2;
    m_pExclusionList2 = new CExoLinkedList<unsigned long>;
    delete m_pExclusionList1;
    m_pExclusionList1 = new CExoLinkedList<unsigned long>;

    CExoLinkedListPosition pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos)->AsSWSPlayer();
        if (pPlayer) {
            g_pAppManager->GetObjectTableManager(1)->RemovePlayer(pPlayer->m_nPlayerID);
            pMessage->SendServerToPlayerModule_DumpPlayer(pPlayer);
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject) {
                CSWSCreature* pCreature = pObject->AsSWSCreature();
                if (pCreature) {
                    if (pCreature->GetArea()) {
                        pCreature->RemoveFromArea(0);
                    }
                    delete pCreature;
                    pPlayer->SetGameObject(NULL);
                }
            }
            pPlayer->ClearPlayerOnDestroyGame();
            pPlayer->m_bPendingAreaMove = 0;
            pPlayer->m_n7c = 0;
            pPlayer->m_oid34 = 0x7f000000;
            unsigned long nPlayerID = pPlayer->m_nPlayerID;
            m_pServerMessage->SendServerToPlayerGameObjUpdate_ObjControl(nPlayerID, 0x7f000000);
        }
        m_pPlayerList->GetNext(pos);
    }

    if (g_pAppManager->m_pClientExoApp && g_pAppManager->m_pClientExoApp->GetParty()) {
        g_pAppManager->m_pClientExoApp->GetParty()->ClearParty();
    }
    CSWPartyTable* pPartyTable = GetPartyTable();
    if (pPartyTable) {
        pPartyTable->ClearInventory();
    }
    g_pTlkTable->ClearCustomTokens();
    delete pModule;
    m_oidModule = 0x7f000000;
    delete m_pFactionManager;
    m_pFactionManager = NULL;
    m_pFactionManager = new CFactionManager;
    delete pGameObjArray;
    m_pGameObjArray = new CGameObjectArray(1);
    m_pServerAIMaster->ClearEventQueue();
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->NukeDirectory(sDir, 1, 1);
    }
    return 1;
}
