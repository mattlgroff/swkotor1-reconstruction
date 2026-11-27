// the out-of-line CResRef copy constructor. It normalises through
// operator=(unsigned char*) (0x00406170); res_ref.cpp keeps an inline memcpy copy
// constructor for the by-value operator= returns, so this lives in its own TU.
#include "d/exo_key_table.h"

// FUNCTION: SWKOTOR 0x00406da0 SYMBOL: ??0CResRef@@QAE@ABV0@@Z
CResRef::CResRef(const CResRef& cResRef)
{
    *this = (unsigned char*)cResRef.m_resRef;
}
