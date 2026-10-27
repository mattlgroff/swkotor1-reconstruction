// Save/load list entry portrait accessor. The original copies the 16-byte
// CResRef inline, so this TU uses the implicit copy constructor.
#define L_RESREF_IMPLICIT_COPY
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006C8200
CResRef CSWGuiSaveLoadEntry::GetPlayerPortrait(int nIndex)
{
    if (nIndex >= 0 && nIndex < 3) {
        return m_aPortraits[nIndex];
    }
    return CResRef();
}
