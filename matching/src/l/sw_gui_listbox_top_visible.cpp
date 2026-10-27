// CSWGuiListBox::SetTopVisible: clamps the first visible row. Kept apart
// from its only caller (sw_gui_store.cpp) so cl cannot see that the call
// leaves ECX intact.
#include "l/sw_gui_store.h"

// FUNCTION: SWKOTOR 0x006B2590
void CSWGuiListBox::SetTopVisible(short nTop)
{
    int nMax = m_nControls - m_nVisible;
    int nLimit = nMax < 0 ? 0 : nMax;
    int n = nTop;
    if (n < nLimit) {
        m_nTop = n;
        return;
    }
    m_nTop = nMax < 0 ? 0 : nMax;
}
