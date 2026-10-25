// Compact CSWCItem accessors.
#include "m/sw_c_item.h"

// FUNCTION: SWKOTOR 0x00646600
void CSWCItem::SetEquippedBy(CSWCCreature* pCreature)
{
    if (pCreature) {
        m_oidEquippedBy = pCreature->m_oidSelf;
    }
    else {
        m_oidEquippedBy = 0x7f000000;
    }
}
