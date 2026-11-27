#include "shared/exo_string.h"

class CSWPartyTable {
public:
    char pad_0x0[0x190];
    int m_bSoloMode;                 // +0x190
    void SetSoloMode(int bSolo, int bSet);
};

class CServerExoApp {
public:
    CSWPartyTable* GetPartyTable();  // 0x004AEE70
};

#include "shared/app_manager.h"

class CVirtualMachine {
public:
    int RunScript(CExoString* pScript, unsigned long oid, int b); // 0x005D0FC0
};

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x005F2A20
void __fastcall TogglePartyFollow(void*, void*)
{
    CExoString sScript("k_sup_solo");
    int nSolo = g_pAppManager->m_pServerExoApp->GetPartyTable()->m_bSoloMode;
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    pServer->GetPartyTable()->SetSoloMode(nSolo == 0 ? 1 : 0, 1);
    g_pVirtualMachine->RunScript(&sScript, 0x7f000000, 0);
}
