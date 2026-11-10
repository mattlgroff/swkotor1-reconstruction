// CSWGuiQuickOrCustomPanel destructor: the original does not restore the
// derived vptr.
#define O_QUICK_OR_CUSTOM_DTOR
#include "o/sw_gui_main_chargen.h"

// FUNCTION: SWKOTOR 0x006F0D50
CSWGuiQuickOrCustomPanel::~CSWGuiQuickOrCustomPanel() {}
