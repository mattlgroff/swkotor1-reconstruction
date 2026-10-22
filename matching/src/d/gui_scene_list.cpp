// CExoArrayList<CAurObject*> members used by CSWGuiScene
// (0x00416770 ctor, 0x004167a0 Insert). Kept out of gui_scene.cpp so the
// scene constructor calls the list constructor out of line.
#include "d/gui_parts.h"

// FUNCTION: SWKOTOR 0x00416770 SYMBOL: ??0?$CExoArrayList@PAVCAurObject@@@@QAE@H@Z
template <class T>
CExoArrayList<T>::CExoArrayList(int nSize)
{
    num = 0;
    array_size = 0;
    element = 0;
    if (nSize) {
        Allocate(nSize);
    }
}

// FUNCTION: SWKOTOR 0x004167a0 SYMBOL: ?Insert@?$CExoArrayList@PAVCAurObject@@@@QAEXPAVCAurObject@@H@Z
template <class T>
void CExoArrayList<T>::Insert(T t, int nPos)
{
    int i = num;
    Add(t);
    for (; i > nPos; i--) {
        element[i] = element[i - 1];
    }
    element[nPos] = t;
}

template class CExoArrayList<CAurObject*>;
