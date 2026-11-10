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

// NONMATCH (8 insns): the original compares the old target from a register
// and reads the flag word once; same statements otherwise.
// FUNCTION: SWKOTOR 0x005F4A20
void CClientExoAppInternal::SetLastTarget(unsigned long oidTarget, int bFlag)
{
    int bChanged = (m_oidLastTarget != oidTarget) ? bFlag : 0;
    m_oidLastTarget = oidTarget;
    m_bUnknown37c_0 = bChanged;
}

// FUNCTION: SWKOTOR 0x005F4EA0
void CClientExoAppInternal::RestoreMousePositionTo(int x, int y)
{
    m_nMousePositionRestoreX = (unsigned long)x;
    m_nMousePositionRestoreY = (unsigned long)y;
}
