// app_manager.h -- CAppManager, owner of the client and server applications
//
// One declaration for every unit (CODE_RULES.md, R1). The layout and the
// out-of-line methods follow the unit that defines them (d/app_misc.cpp);
// the Mac symbol tables name the same seven methods. The constructor fills
// +0x14 with a CSWReentrantServerStats.
#ifndef SHARED_APP_MANAGER_H
#define SHARED_APP_MANAGER_H

class CClientExoApp;
class CServerExoApp;
class CObjectTableManager;
class CSWReentrantServerStats;
class CExoString;

class CAppManager
{
public:
    CAppManager();                                          // 0x00401160
    ~CAppManager();                                         // 0x004012C0
    CObjectTableManager* GetObjectTableManager(int nWhich); // 0x00401060
    void DoSaveGameScreenShot(CExoString& sName);           // 0x00401080
    void AllocLargeTempBuffer();                            // 0x004010A0
    void DestroyServer();                                   // 0x00401280
    void CreateServer(int nMode);                           // 0x00401380

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
    CSWReentrantServerStats* GetReentrantServerStats() { return m_pReentrantServerStats; } // descriptive
    char* GetLargeTempBuffer()
    {
        if (!m_pLargeTempBuffer) {
            AllocLargeTempBuffer();
        }
        return m_pLargeTempBuffer;
    }

    char* m_pLargeTempBuffer;                               // +0x00
    CClientExoApp* m_pClientExoApp;                         // +0x04
    CServerExoApp* m_pServerExoApp;                         // +0x08
    CObjectTableManager* m_pObjectTableManagerServer;       // +0x0C
    CObjectTableManager* m_pObjectTableManagerClient;       // +0x10
    CSWReentrantServerStats* m_pReentrantServerStats;       // +0x14
    unsigned long m_nStartTime;                             // +0x18
    int m_bPaused;                                          // +0x1C
};

#endif
