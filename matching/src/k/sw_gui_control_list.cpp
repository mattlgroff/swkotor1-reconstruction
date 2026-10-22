// CExoArrayList<CSWGuiControl*>::DelIndex, kept in its own translation unit
// so the main interface's callers do not inline it.
#include "k/sw_gui_main_interface.h"

template <class T>
void CExoArrayList<T>::DelIndex(int nIndex)
{
    num--;
    for (; nIndex < num; nIndex++) {
        element[nIndex] = element[nIndex + 1];
    }
}

// FUNCTION: SWKOTOR 0x006857A0 SYMBOL: ?DelIndex@?$CExoArrayList@PAVCSWGuiControl@@@@QAEXH@Z
template void CExoArrayList<CSWGuiControl*>::DelIndex(int nIndex);
