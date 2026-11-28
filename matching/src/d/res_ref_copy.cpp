// CResRef::CResRef(unsigned char*), as the Mac debug map names 0x00406DA0:
// it normalises through operator=(unsigned char*) (0x00406170).
#include "shared/res_ref.h"

// FUNCTION: SWKOTOR 0x00406da0 SYMBOL: ??0CResRef@@QAE@PAE@Z
CResRef::CResRef(unsigned char* pResRef)
{
    *this = pResRef;
}
