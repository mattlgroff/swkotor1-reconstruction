// CSWGuiCustomPanel destructor: the original does not restore the derived
// vptr.
#define L_CUSTOM_PANEL_DTOR
#include "l/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006EFB40
CSWGuiCustomPanel::~CSWGuiCustomPanel() {}
