// Secondary CSWSObject view of an item: the vtable's localized-name slot.
#include "b/exo_loc_string.h"

class CSWSItemSecondaryView {
public:
    char pad_0x00[0x270];
    CExoLocString m_lsLocalizedName;
    CExoLocString* GetLocalizedName();
};

// FUNCTION: SWKOTOR 0x00553200
CExoLocString* CSWSItemSecondaryView::GetLocalizedName()
{
    return &m_lsLocalizedName;
}
