struct CAppPad {
    int m_n00;
    int m_n04;
    char pad_0x8[0xc];
    int m_n14;
};

struct CAppManager {
    char pad_0x0[0x14];
    CAppPad* m_pPad;                  // +0x14
};

struct CClientExoAppInternalLoadBar {
    char pad_0x0[0x18c];
    int m_nLoadBarState;             // +0x18c
};

extern CAppManager* g_pAppManager;   // 0x007A39FC

// FUNCTION: SWKOTOR 0x005F29F0
void __fastcall ShutDownLoadGameLoadBar(CClientExoAppInternalLoadBar* pInternal, void*)
{
    if (pInternal->m_nLoadBarState == 0)
        return;

    g_pAppManager->m_pPad->m_n14 = 0;
    CAppPad* pPad = g_pAppManager->m_pPad;
    pPad->m_n00 = 0;
    pPad->m_n04 = 0;
    pInternal->m_nLoadBarState = 0;
}

// FUNCTION: SWKOTOR 0x005F2AC0
unsigned long __fastcall ServerToClientObjectId(void*, void*, unsigned long oid)
{
    if (oid != 0x7f000000)
        return oid | 0x80000000;
    return 0x7f000000;
}
