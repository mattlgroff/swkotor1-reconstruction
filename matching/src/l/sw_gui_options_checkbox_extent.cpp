// Options checkbox layout: a 25x25 box on the left, vertically centred,
// and the caption text to its right.
#include "l/sw_gui_in_game_abilities.h"

// FUNCTION: SWKOTOR 0x006DE000
void CSWGuiOptionsCheckbox::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent box = {extent.left, extent.top + (extent.height - 25) / 2 + 2, 25, 25};
    GetBorder().SetExtent(box);
    GetHilight().SetExtent(box);
    GetSelectedHilight().SetExtent(box);
    GetSelectedBorder().SetExtent(box);
    CSWGuiExtent text = extent;
    text.left += 30;
    text.width -= 30;
    m_cTextBox.SetExtent(text);
    m_extent = extent;
}
