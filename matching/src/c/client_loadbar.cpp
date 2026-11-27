#include "shared/reentrant_server_stats.h"

#include "shared/app_manager.h"

struct CClientExoAppInternalLoadBar {
    char pad_0x0[0x18c];
    int m_nLoadBarState;             // +0x18c
};

#include "shared/globals.h"

// FUNCTION: SWKOTOR 0x005F29F0
void __fastcall ShutDownLoadGameLoadBar(CClientExoAppInternalLoadBar* pInternal, void*)
{
    if (pInternal->m_nLoadBarState == 0)
        return;

    g_pAppManager->m_pReentrantServerStats->m_n14 = 0;
    CSWReentrantServerStats* pPad = g_pAppManager->m_pReentrantServerStats;
    pPad->m_bActive = 0;
    pPad->m_nKind = 0;
    pInternal->m_nLoadBarState = 0;
}

// FUNCTION: SWKOTOR 0x005F2AC0
unsigned long __fastcall ServerToClientObjectId(void*, void*, unsigned long oid)
{
    if (oid != 0x7f000000)
        return oid | 0x80000000;
    return 0x7f000000;
}
