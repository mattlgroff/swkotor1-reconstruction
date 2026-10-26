// CExoArrayList<SafePointer<CAurTexture> >::Add (aurgrass.o).
#include "i/safe_texture_list.h"

// NONMATCH: the original reads t.m_p after storing num and keeps it in esi
// across the operator= call; ours loads it first and reloads it.
// FUNCTION: SWKOTOR 0x004A8230 SYMBOL: ?Add@?$CExoArrayList@V?$SafePointer@VCAurTexture@@@@@@QAEXV?$SafePointer@VCAurTexture@@@@@Z
template <class T>
void CExoArrayList<T>::Add(T t)
{
    if (num == array_size)
        Allocate(array_size ? array_size * 2 : 8);
    element[num++] = t;
}

template void CExoArrayList<SafePointer<CAurTexture> >::Add(SafePointer<CAurTexture>);
