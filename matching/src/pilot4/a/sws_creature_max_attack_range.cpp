// Pilot 4 arm A: CSWSCreature::MaxAttackRange.
#include "pilot4/a/p4a_fb0f0.h"

// FUNCTION: SWKOTOR 0x004FB0F0
float CSWSCreature::MaxAttackRange(unsigned long oTarget, int a2, int bRangedOnly)
{
    CSWSItem* pItem = m_pInventory->GetItemInSlot(0x10);
    if (pItem && pItem->GetBaseItem()->IsRangedWeapon() > 0) {
        if (bRangedOnly) {
            return 20.0f;
        }
        float fRange = pItem->GetBaseItem()->m_fMaxRange;
        if (fRange == 0.0f) {
            return 30.0f;
        }
        return fRange;
    }
    return DesiredAttackRange(oTarget, a2) + 0.5f;
}
