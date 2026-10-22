// CSWSPlayer GUI state helpers (0x00560B30-0x00560CD0).
// Header names: CSWSPlayerInventoryGUI = functions.csv CSWSPlayerCharSheetGUI (player +0x50),
// CSWSPlayerStoreGUI = functions.csv CSWSPlayerInventoryGUI (player +0x54).
#include "b/sws_player.h"

// FUNCTION: SWKOTOR 0x00560B30
CSWSPlayerInventoryGUI::CSWSPlayerInventoryGUI()
{
    m_oidCreature = 0x7f000000;
    m_pItems = new CSWSPlayerInventoryItems;
    m_nDisplayMode = 0xff;
    m_nC = 1;
}

// FUNCTION: SWKOTOR 0x00560BA0
void CSWSPlayerInventoryGUI::SetCreatureDisplayed(unsigned long oidCreature)
{
    if (m_oidCreature != oidCreature && m_pItems) {
        delete m_pItems;
        m_pItems = new CSWSPlayerInventoryItems;
    }
    m_oidCreature = oidCreature;
}

// FUNCTION: SWKOTOR 0x00560C20
CSWSPlayerStoreGUI::CSWSPlayerStoreGUI()
{
    m_bOpen = 0;
    m_pStore = new CSWSPlayerStore;
}

// FUNCTION: SWKOTOR 0x00560C90
void CSWSPlayerStoreGUI::SetOpen(int bOpen)
{
    if (!bOpen && m_bOpen) {
        m_pStore->ClearItems();
        m_pStore->ClearRepository();
    }
    m_bOpen = bOpen;
}

// FUNCTION: SWKOTOR 0x00560CC0
int CSWSPlayer::GetIsSaveAllowed()
{
    return m_n3C != 0;
}

// FUNCTION: SWKOTOR 0x00560CD0
void CSWSPlayer::SetAreaTransitionBMP(int nMode, const CExoString& sBMP)
{
    if (nMode == 1) {
        m_s18 = sBMP;
    }
    m_n14 = nMode;
}
