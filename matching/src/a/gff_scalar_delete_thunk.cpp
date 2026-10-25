#include "d/res_gff.h"

// FUNCTION: SWKOTOR 0x00506EF0
void* __fastcall gff_scalar_delete_thunk_00506ef0(void* pThis, void*, unsigned int flags)
{
    ((CResGFF*)pThis)->CResGFF::~CResGFF();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}
