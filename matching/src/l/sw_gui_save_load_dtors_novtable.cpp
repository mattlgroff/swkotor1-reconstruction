// Implicit destructors of the save-name dialog and the save/load list
// entry. The original bodies do not restore the derived vptr, so the classes
// are declared novtable in this isolated TU; the entry's radio-button base
// destructor is inlined (vptr 0x007566D8, then ~CSWGuiButton).
#define L_SAVE_LOAD_DTORS
#define L_RADIO_BUTTON_INLINE
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006CBA90
CSWGuiSaveLoadEntry::~CSWGuiSaveLoadEntry()
{
}

// FUNCTION: SWKOTOR 0x006CC060
CSWGuiSaveNamePanel::~CSWGuiSaveNamePanel()
{
}
