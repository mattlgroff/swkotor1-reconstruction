// out-of-line CExoArrayList<T*> helpers shared (identical-COMDAT
// folded) by every pointer list: Add 0x00444fe0, Insert 0x00445020 and
// Remove 0x00445080. The CAurTextureBasic unit only sees their declarations
// and calls them, as the original does.
#define G_EXO_ARRAY_LIST_REMOVE
#include "g/aur_texture.h"

template <class T>
void CExoArrayList<T>::Insert(T t, int nIndex)
{
    int i = num;
    Add(t);
    for (; i > nIndex; i--)
        element[i] = element[i - 1];
    element[nIndex] = t;
}

// FUNCTION: SWKOTOR 0x00444fe0 SYMBOL: ?Add@?$CExoArrayList@PAVCAurTextureBasic@@@@QAEXPAVCAurTextureBasic@@@Z
// FUNCTION: SWKOTOR 0x00445020 SYMBOL: ?Insert@?$CExoArrayList@PAVCAurTextureBasic@@@@QAEXPAVCAurTextureBasic@@H@Z
// FUNCTION: SWKOTOR 0x00445080 SYMBOL: ?Remove@?$CExoArrayList@PAVCAurTextureBasic@@@@QAEXPAVCAurTextureBasic@@@Z
template void CExoArrayList<CAurTextureBasic*>::Add(CAurTextureBasic* t);
template void CExoArrayList<CAurTextureBasic*>::Insert(CAurTextureBasic* t, int nIndex);
template void CExoArrayList<CAurTextureBasic*>::Remove(CAurTextureBasic* t);
