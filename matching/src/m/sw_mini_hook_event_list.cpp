// CExoArrayList<CSWMiniHookEvent>::Allocate (0x24-byte hook events queued
// on CSWMiniPlayer +0x1a0 / +0x1ac). Kept apart from the auto_inline(off)
// units: the element constructor loop is only inlined without that pragma.

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x0066D190 SYMBOL: ?Allocate@?$CExoArrayList@UCSWMiniHookEvent@@@@QAEXH@Z
template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* old = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++) {
        element[i] = old[i];
    }
    if (old) {
        delete[] old;
    }
}

template void CExoArrayList<CSWMiniHookEvent>::Allocate(int n);
