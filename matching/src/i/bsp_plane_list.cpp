// CExoArrayList<Plane>::Add (bsp.o). Own TU so bsp.cpp keeps its
// CExoArrayList::Add calls out of line.
#include "i/bsp.h"

// FUNCTION: SWKOTOR 0x004AD5E0 SYMBOL: ?Add@?$CExoArrayList@VPlane@@@@QAEXVPlane@@@Z
template <class T>
void CExoArrayList<T>::Add(T t)
{
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 8);
    }
    element[num] = t;
    num++;
}

template class CExoArrayList<Plane>;
