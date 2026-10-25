// Isolated destructor TU: test whether novtable removes the derived vptr
// restoration before the inferred checkbox's base teardown.
#define L_SWGUI_MATCHING_DESTRUCTOR_TU
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E00F0
CSWGuiOptionsCheckbox::~CSWGuiOptionsCheckbox()
{
}
