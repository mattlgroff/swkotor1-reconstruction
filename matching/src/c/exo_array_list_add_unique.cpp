// CExoArrayList<CExoString>::AddUnique (worker C s1). Kept in its own
// translation unit so the inline count/Add bodies stay out-of-line calls.

#include "c/exo_base.h"

#pragma inline_depth(0)
template <class T>
void CExoArrayList<T>::AddUnique(T t)
{
    if (!count(t)) {
        Add(t);
    }
}

template void CExoArrayList<CExoString>::AddUnique(CExoString t);

// FUNCTION: SWKOTOR 0x005F7C10 SYMBOL: ?AddUnique@?$CExoArrayList@VCExoString@@@@QAEXVCExoString@@@Z
