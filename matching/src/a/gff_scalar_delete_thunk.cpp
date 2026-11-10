#include "d/res_gff.h"

// CResIFO adds nothing to CResGFF: its complete destructor (0x00506F10, the
// Mac map's vtable entry for this thunk is CResIFO::~CResIFO) is a bare jump
// into ~CResGFF. This scalar deleting destructor calls that, not ~CResGFF.
class CResIFO : public CResGFF {
public:
    ~CResIFO();                                     // 0x00506F10
};

// FUNCTION: SWKOTOR 0x00506EF0
void* __fastcall gff_scalar_delete_thunk_00506ef0(void* pThis, void*, unsigned int flags)
{
    ((CResIFO*)pThis)->CResIFO::~CResIFO();
    if ((flags & 1) != 0)
        ::operator delete(pThis);
    return pThis;
}
