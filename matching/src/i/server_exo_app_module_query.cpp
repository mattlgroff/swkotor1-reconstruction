// CServerExoAppInternal::GetModulePreviouslyVisited: finds which resource
// type (saved module 0xBC1 / 0x809, NWM 0x80E, module 0x7DB / 0xBBA) holds
// the named module.
#include "i/server_exo_app.h"

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                                          // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CExoInput {
public:
    void UnpauseRumble();                                           // 0x005DF580
};
extern CExoInput* g_pExoInput;                                      // 0x007A39E4

extern unsigned char g_nLiveFolderCount;                            // 0x0073ECD2

// FUNCTION: SWKOTOR 0x004B51A0
int CServerExoAppInternal::GetModulePreviouslyVisited(CExoString& sModule)
{
    if (sModule.GetLength() < 1) {
        return 1;
    }
    CExoString sUnused1;
    CExoString sUnused2;
    unsigned short nType = 0xffff;
    int bResult = 0;

    if (IncludeModuleInSave(sModule)) {
        GetServerExoApp()->FUN_004aee70()->AddGameInProgress();
        if (g_pExoResMan->Exists(sModule, 0xbc1, NULL)) {
            nType = 0xbc1;
            bResult = 1;
        } else if (g_pExoResMan->Exists(sModule, 0x809, NULL)) {
            nType = 0x809;
            bResult = 1;
        }
        GetServerExoApp()->FUN_004aee70()->RemoveGameInProgress();
    }

    if (nType == 0xffff) {
        {
            CExoString sDir("NWMFILES:");
            g_pExoResMan->AddResourceDirectory(sDir);
        }
        if (g_pExoResMan->Exists(sModule, 0x80e, NULL)) {
            nType = 0x80e;
            bResult = 0;
        }
        {
            CExoString sDir("NWMFILES:");
            g_pExoResMan->RemoveResourceDirectory(sDir);
        }
    }

    if (nType == 0xffff) {
        {
            CExoString sDir("MODULES:");
            g_pExoResMan->AddResourceDirectory(sDir);
        }
        if (g_pExoResMan->Exists(sModule, 0x7db, NULL)) {
            nType = 0x7db;
            bResult = 0;
        } else if (g_pExoResMan->Exists(sModule, 0xbba, NULL)) {
            nType = 0xbba;
            bResult = 0;
        }
        {
            CExoString sDir("MODULES:");
            g_pExoResMan->RemoveResourceDirectory(sDir);
        }
    }

    if (nType == 0xffff) {
        CExoString sLive;
        for (int i = 1; i < g_nLiveFolderCount; i++) {
            sLive.Format("LIVE%d", i);
            if (g_pExoBase->m_pcExoAliasList->GetAliasPath(sLive, 0)->GetLength()) {
                sLive = sLive + ":MODULES";
                g_pExoResMan->AddResourceDirectory(sLive);
                if (g_pExoResMan->Exists(sModule, 0x7db, NULL)) {
                    nType = 0x7db;
                    bResult = 0;
                } else if (nType == 0xffff &&
                           g_pExoResMan->Exists(sModule, 0xbba, NULL)) {
                    nType = 0xbba;
                    bResult = 0;
                }
                g_pExoResMan->RemoveResourceDirectory(sLive);
                if (nType != 0xffff) {
                    break;
                }
            }
        }
    }

    g_pExoInput->UnpauseRumble();
    if (nType == 0xffff) {
        return 0;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004B41C0
CExoString CServerExoAppInternal::GetPlayerListString()
{
    CExoString sList;
    CExoString sName;

    sList = sList + "ServerAdmins: " + (int)m_pSysAdminList->GetCount();
    CExoLinkedListPosition pos = m_pSysAdminList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pSysAdminList->GetAtPos(pos);
        sName = GetServerExoApp()->GetNetLayer()->GetPlayerInfo(pPlayer->GetPlayerID())
                    ->GetPlayerName().CStr();
        sList = sList + "\n" + sName;
        sList = sList + " [ServerAdmin]";
        m_pSysAdminList->GetNext(pos);
    }
    sList = sList + "\n";

    sList = sList + "Players: " + (int)m_pPlayerList->GetCount();
    pos = m_pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = m_pPlayerList->GetAtPos(pos);
        sName = GetServerExoApp()->GetNetLayer()->GetPlayerInfo(pPlayer->GetPlayerID())
                    ->GetPlayerName().CStr();
        sList = sList + "\n" + sName;
        sList = sList + " [Player]";
        m_pPlayerList->GetNext(pos);
    }
    sList = sList + "\n";

    sList = sList + "Total: " +
            (int)(m_pSysAdminList->GetCount() + m_pPlayerList->GetCount());
    return sList;
}
