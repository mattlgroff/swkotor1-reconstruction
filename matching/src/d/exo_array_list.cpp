// CExoArrayList<CExoString> (0x00405080-0x00405d4f).
// Template instantiations all compile at the end of the TU, where every
// member is visible and gets inlined; the original only inlines the
// compiler-generated vector delete. Explicit specialisations are ordinary
// functions compiled in source order, so Allocate / Add come last and stay
// out of line in their callers. int lists: exo_array_list_int.cpp.
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x004058f0 SYMBOL: ??0?$CExoArrayList@VCExoString@@@@QAE@H@Z
template <>
CExoArrayList<CExoString>::CExoArrayList(int n)
{
    num = 0;
    array_size = 0;
    element = 0;
    if (n) {
        Allocate(n);
    }
}

// FUNCTION: SWKOTOR 0x00405920 SYMBOL: ??1?$CExoArrayList@VCExoString@@@@QAE@XZ
template CExoArrayList<CExoString>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x00405950 SYMBOL: ?SetSize@?$CExoArrayList@VCExoString@@@@QAEXH@Z
template <>
void CExoArrayList<CExoString>::SetSize(int n)
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

// NONMATCH: 11 insns; this/o swapped between ebx and edi (the int version 0x405a20
// matches with this exact body)
// FUNCTION: SWKOTOR 0x00405d10 SYMBOL: ??4?$CExoArrayList@VCExoString@@@@QAEAAV0@ABV0@@Z
template <class T>
CExoArrayList<T>& CExoArrayList<T>::operator=(const CExoArrayList<T>& o)
{
    num = 0;
    for (int i = 0; i < o.num; i++) {
        Add(o.element[i]);
    }
    return *this;
}

template CExoArrayList<CExoString>& CExoArrayList<CExoString>::operator=(const CExoArrayList<CExoString>& o);

// FUNCTION: SWKOTOR 0x004059a0 SYMBOL: ?Add@?$CExoArrayList@VCExoString@@@@QAEXVCExoString@@@Z
template <>
void CExoArrayList<CExoString>::Add(CExoString t)
{
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num++] = t;
}

// FUNCTION: SWKOTOR 0x00405080 SYMBOL: ?Allocate@?$CExoArrayList@VCExoString@@@@QAEXH@Z
template <>
void CExoArrayList<CExoString>::Allocate(int n)
{
    CExoString* pOld = element;
    array_size = n;
    element = new CExoString[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

// Only the generic operator= instantiates at the end of the TU.
#pragma auto_inline(off)
