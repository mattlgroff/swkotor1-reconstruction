// The original destructor does not restore the derived vptr.
#define L_UPGRADE_ENTRY_DTOR
#include "l/sw_gui_upgrade.h"

// FUNCTION: SWKOTOR 0x006C5AD0
CSWUpgradeItemEntry::~CSWUpgradeItemEntry() {}
