// Worker A round s9: CSWSItemPropertyHandler dispatch virtuals (vtable 0x00744284 slots 0x8/0xc).
// Neither start has a functions.csv row.
#include "a/item_property_handler.h"

// FUNCTION: SWKOTOR 0x004E5410
int CSWSItemPropertyHandler::ApplyItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                               CSWSCreature* pCreature, unsigned long nSlot, int bLoadingGame)
{
    unsigned short nProperty = pProperty->m_nPropertyName;
    if (nProperty < 0x3c && m_pApplyFunctions[nProperty]) {
        return (this->*m_pApplyFunctions[nProperty])(pItem, pProperty, pCreature, nSlot, bLoadingGame);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004E5450
int CSWSItemPropertyHandler::RemoveItemProperty(CSWSItem* pItem, CSWItemProperty* pProperty,
                                                CSWSCreature* pCreature, unsigned long nSlot)
{
    unsigned short nProperty = pProperty->m_nPropertyName;
    if (nProperty < 0x3c && m_pRemoveFunctions[nProperty]) {
        return (this->*m_pRemoveFunctions[nProperty])(pItem, pProperty, pCreature, nSlot);
    }
    return 0;
}
