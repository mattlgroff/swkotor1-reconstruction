// CSWGuiKeyMapButton::Draw (control vtable slot 0x38): the base button, then
// the event button that shows the bound key.
#define L_GUI_CONTROL_DRAW_SLOT
#include "l/sw_gui_keymap_button.h"

// FUNCTION: SWKOTOR 0x006EC350
void CSWGuiKeyMapButton::Draw(float fDelta)
{
    CSWGuiButton::Draw(fDelta);
    m_cEventButton.Draw(fDelta);
}
