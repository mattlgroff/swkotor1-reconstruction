// CClientExoApp facade accessors, worker C r5 (unnamed FUN_ getters in
// reverse/functions.csv). Kept apart from the internal members so the
// compiler does not inline them into their callers.

#include "c/client_exo_app.h"

// CClientExoApp facade accessors without a name in reverse/functions.csv.

// FUNCTION: SWKOTOR 0x005ED310
void CClientExoApp::EmptyForwarder()
{
    m_pcInternal->EmptyMethod();
}

// FUNCTION: SWKOTOR 0x005ED460
int CClientExoApp::GetUnknown88()
{
    return m_pcInternal->m_nUnknown88;
}

// FUNCTION: SWKOTOR 0x005ED4C0
int CClientExoApp::GetUnknown144()
{
    return m_pcInternal->m_nUnknown144;
}

// FUNCTION: SWKOTOR 0x005ED4F0
CWorldTimer* CClientExoApp::GetWorldTimer()
{
    return m_pcInternal->m_pWorldTimer;
}

// FUNCTION: SWKOTOR 0x005ED550
unsigned long CClientExoApp::GetPlayerCreatureId()
{
    return m_pcInternal->m_oidPlayer;
}

// FUNCTION: SWKOTOR 0x005ED570
CGameObjectArray* CClientExoApp::GetGameObjectArray()
{
    return m_pcInternal->m_pGameObjectArray;
}

// FUNCTION: SWKOTOR 0x005ED690
CGuiInGame* CClientExoApp::GetInGameGui()
{
    return m_pcInternal->m_pGuiInGame;
}

// FUNCTION: SWKOTOR 0x005ED6B0
void* CClientExoApp::GetUnknown0c()
{
    return m_pcInternal->m_pUnknown0c;
}

// FUNCTION: SWKOTOR 0x005ED6F0
CSWCSaveLoad* CClientExoApp::GetSaveLoad()
{
    return m_pcInternal->m_pSaveLoad;
}

// FUNCTION: SWKOTOR 0x005ED700
CClientUnknown700* CClientExoApp::GetUnknown700()
{
    return (CClientUnknown700*)m_pcInternal->m_pClientOptions;
}

// FUNCTION: SWKOTOR 0x005ED710
void* CClientExoApp::GetUnknown08()
{
    return m_pcInternal->m_pUnknown08;
}

// FUNCTION: SWKOTOR 0x005ED8B0
CSWParty* CClientExoApp::GetParty()
{
    return m_pcInternal->m_pParty;
}

// FUNCTION: SWKOTOR 0x005EDA40
CSWGuiManager* CClientExoApp::GetGuiManager()
{
    return m_pcInternal->m_pGuiManager;
}

// FUNCTION: SWKOTOR 0x005EDAF0
void* CClientExoApp::GetUnknown00()
{
    return m_pcInternal->m_pUnknown00;
}

// FUNCTION: SWKOTOR 0x005EDB80
int CClientExoApp::IsNotUnknown28c(int n)
{
    return n != m_pcInternal->m_nUnknown28c;
}

// FUNCTION: SWKOTOR 0x005EDDE0
void CClientExoApp::GetPazaakResult(int* pWon, int* pWager)
{
    CClientExoAppInternal* pInternal = m_pcInternal;
    *pWon = pInternal->m_bPazaakWon;
    *pWager = pInternal->m_nPazaakWager;
}

// FUNCTION: SWKOTOR 0x005EDE10
void CClientExoApp::SetUnknown1a8(int a, int b, int c)
{
    CClientExoAppInternal* pInternal = m_pcInternal;
    pInternal->m_nUnknown1a8 = a;
    pInternal->m_nUnknown374 = b;
    pInternal->m_nUnknown1ac = c;
}

// FUNCTION: SWKOTOR 0x005EDFD0
void CClientExoApp::SetTutorialFlag(int n, unsigned char b)
{
    m_pcInternal->m_pTutorialFlags[n] = b;
}

// FUNCTION: SWKOTOR 0x005EDFF0
unsigned char CClientExoApp::GetTutorialFlag(int n)
{
    return m_pcInternal->m_pTutorialFlags[n];
}
