// CSWGuiKeyMapButton regular destructor: the executable omits the derived
// vptr restoration and only destroys the event button and the base button.
#define L_KEYMAP_BUTTON_NOVTABLE
#include "l/sw_gui_keymap_button.h"

// FUNCTION: SWKOTOR 0x006ED570
CSWGuiKeyMapButton::~CSWGuiKeyMapButton()
{
}
