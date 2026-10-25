// CClientExoAppInternal small target and mouse-state helpers.

#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x005F1460
void CClientExoAppInternal::ClearCachedCreatures(int b)
{
    if (reinterpret_cast<unsigned long>(m_pCachedPlayerCreature) == (unsigned long)b) {
        m_oidCachedPlayer = OBJECT_INVALID;
        m_pCachedPlayerCreature = 0;
    }
}

// FUNCTION: SWKOTOR 0x005F4A20
void CClientExoAppInternal::SetLastTarget(unsigned long oidTarget, int bFlag)
{
    unsigned long nTarget = oidTarget;
    volatile unsigned long& nTargetSlot = *reinterpret_cast<volatile unsigned long*>(reinterpret_cast<char*>(this) + 0x2b4);
    unsigned long nOldTarget = nTargetSlot;
    unsigned long bChanged = nOldTarget == nTarget ? 0 : 0xFFFFFFFF;
    nTargetSlot = nTarget;
    unsigned long& nFlags = *reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(this) + 0x37c);
    unsigned long nOldFlags = nFlags;
    unsigned long nChange = bChanged & (unsigned long)bFlag;
    nFlags = nOldFlags ^ ((nChange ^ nOldFlags) & 1);
}

// FUNCTION: SWKOTOR 0x005F4EA0
void CClientExoAppInternal::RestoreMousePositionTo(int x, int y)
{
    m_nMousePositionRestoreX = (unsigned long)x;
    m_nMousePositionRestoreY = (unsigned long)y;
}
