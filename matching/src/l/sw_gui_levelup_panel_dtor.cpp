// The original level-up panel destructor does not restore the derived vptr.
#define L_LEVELUP_PANEL_DTOR
#include "l/sw_gui_levelup_chargen.h"

// FUNCTION: SWKOTOR 0x006EEE90
CSWGuiLevelUpPanel::~CSWGuiLevelUpPanel() {}
