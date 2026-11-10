// CSWGuiQuickPanel destructor: the original does not restore the derived
// vptr.
#define O_QUICK_PANEL_DTOR
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F0700
CSWGuiQuickPanel::~CSWGuiQuickPanel() {}
