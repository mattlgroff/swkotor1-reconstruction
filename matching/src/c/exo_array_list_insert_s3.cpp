// CExoArrayList<unsigned long> / <unsigned short>::Insert instantiations
// emitted in nwccreaturestats.o / nwccreature.o (worker C s3).

#include "c/exo_base.h"

template <class T>
void CExoArrayList<T>::Insert(T t, int pos)
{
    int i = num;
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num] = t;
    num++;
    for (; i > pos; i--) {
        element[i] = element[i - 1];
    }
    element[pos] = t;
}

// FUNCTION: SWKOTOR 0x00616940 SYMBOL: ?Insert@?$CExoArrayList@K@@QAEXKH@Z
template void CExoArrayList<unsigned long>::Insert(unsigned long, int);

// FUNCTION: SWKOTOR 0x006169A0 SYMBOL: ?Insert@?$CExoArrayList@G@@QAEXGH@Z
template void CExoArrayList<unsigned short>::Insert(unsigned short, int);
