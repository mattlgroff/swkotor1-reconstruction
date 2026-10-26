#define L_INVENTORY_ENTRY_DTORS
#include "l/sw_gui_inventory_equip.h"

// Radio base cleanup is inlined by the original compiler in the item entry.
CSWGuiRadioButton::~CSWGuiRadioButton() {}

// FUNCTION: SWKOTOR 0x006B7FA0
CSWGuiInGameItemEntry::~CSWGuiInGameItemEntry() {}

// FUNCTION: SWKOTOR 0x006B87C0
CSWGuiStoreItemEntry::~CSWGuiStoreItemEntry() {}
