#include "a/sws_creature.h"
template <class T> void CExoArrayList<T>::SetSize(int nSize)
{
    if (!nSize) {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
    } else Allocate(nSize);
    num = nSize;
}
// FUNCTION: SWKOTOR 0x004FDDD0 SYMBOL: ?SetSize@?$CExoArrayList@VCSWGuiInterfaceAction@@@@QAEXH@Z
template void CExoArrayList<CSWGuiInterfaceAction>::SetSize(int nSize);
