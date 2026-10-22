// CExoArrayList<int> / <unsigned long> members (0x00405150,
// 0x00405a20, 0x00405d50). Add/Allocate stay out of line (0x00671c00,
// 0x005e03c0), hence auto_inline(off) for the whole TU.
#include "d/exo_key_table.h"

#pragma auto_inline(off)

template <class T>
void CExoArrayList<T>::SetSize(int n)
{
    if (n == 0) {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
    } else {
        Allocate(n);
    }
    num = n;
}

template <class T>
void CExoArrayList<T>::Clear(int bFree)
{
    num = 0;
    if (bFree) {
        SetSize(0);
    }
}

template <class T>
CExoArrayList<T>& CExoArrayList<T>::operator=(const CExoArrayList<T>& o)
{
    num = 0;
    for (int i = 0; i < o.num; i++) {
        Add(o.element[i]);
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00405150 SYMBOL: ?SetSize@?$CExoArrayList@K@@QAEXH@Z
template void CExoArrayList<unsigned long>::SetSize(int n);

// FUNCTION: SWKOTOR 0x00405a20 SYMBOL: ??4?$CExoArrayList@H@@QAEAAV0@ABV0@@Z
// FUNCTION: SWKOTOR 0x00405d50 SYMBOL: ?Clear@?$CExoArrayList@H@@QAEXH@Z
template CExoArrayList<int>& CExoArrayList<int>::operator=(const CExoArrayList<int>& o);
template void CExoArrayList<int>::Clear(int bFree);
