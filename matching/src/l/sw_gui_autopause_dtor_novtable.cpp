// Isolated destructor TU: novtable suppresses the derived vptr restoration
// emitted by MSVC in the original AutoPause teardown body.
#define L_SWGUI_MATCHING_DESTRUCTOR_TU
#include "l/sw_gui_options_in_game.h"

// FUNCTION: SWKOTOR 0x006E7A90 SYMBOL: ??1CSWGuiInGameAutoPause@@UAE@XZ
CSWGuiInGameAutoPause::~CSWGuiInGameAutoPause()
{
}
