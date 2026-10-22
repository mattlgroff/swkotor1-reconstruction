// CObjectTableManager (0x004AE5D0-0x004AE650).
#include "i/world_timer.h"
#include <string.h>

// FUNCTION: SWKOTOR 0x004AE5D0
CObjectTableManager::CObjectTableManager()
{
    m_nTables = 0;
    memset(m_pTables, 0, sizeof(m_pTables));
}

// FUNCTION: SWKOTOR 0x004AE5F0 SYMBOL: ?ClearAll@CObjectTableManager@@QAEXXZ
void CObjectTableManager::ClearAll()
{
    for (unsigned long i = 0; i < m_nTables; i++) {
        CObjectTableEntry* pEntry = m_pTables[i];
        if (pEntry) {
            delete pEntry;
            m_pTables[i] = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x004AE640
CObjectTableManager::~CObjectTableManager()
{
    ClearAll();
}

// FUNCTION: SWKOTOR 0x004AE650
int CObjectTableManager::RemovePlayer(unsigned long nPlayer)
{
    if (nPlayer == 0xffffffff) {
        return 0;
    }
    if (nPlayer >= 0x60) {
        return 0;
    }
    if (m_pTables[nPlayer] == 0) {
        return 0;
    }
    {
        CObjectTableEntry* pEntry = m_pTables[nPlayer];
        delete pEntry;
    }
    m_pTables[nPlayer] = 0;
    return 1;
}
