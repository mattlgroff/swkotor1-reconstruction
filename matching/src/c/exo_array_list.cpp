// CExoArrayList<T> out-of-line members (b005e1_exo_containers_input).

#include "c/exo_input.h"

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

template void CExoArrayList<unsigned long>::Allocate(int n);
template void CExoArrayList<CExoInputEvent>::Add(CExoInputEvent t);

// Shared by every 4-byte element instantiation (identical COMDAT folding).
// FUNCTION: SWKOTOR 0x005E03C0 SYMBOL: ?Allocate@?$CExoArrayList@K@@QAEXH@Z

// FUNCTION: SWKOTOR 0x005E0A30 SYMBOL: ?Add@?$CExoArrayList@UCExoInputEvent@@@@QAEXUCExoInputEvent@@@Z

template void CExoArrayList<CExoInputEvent>::Allocate(int n);

// FUNCTION: SWKOTOR 0x005E0410 SYMBOL: ?Allocate@?$CExoArrayList@UCExoInputEvent@@@@QAEXH@Z
