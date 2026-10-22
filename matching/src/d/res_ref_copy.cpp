// the out-of-line CResRef copy constructor. It normalises through
// operator=(const char (&)[16]); res_ref.cpp keeps an inline memcpy copy
// constructor for the by-value operator= returns, so this lives in its own TU.
#include "d/exo_key_table.h"

typedef CResRef (CResRef::*PFN_ASSIGN_ARRAY)(const char (&)[16]);

// FUNCTION: SWKOTOR 0x00406da0 SYMBOL: ??0CResRef@@QAE@ABV0@@Z
CResRef::CResRef(const CResRef& cResRef)
{
    (this->*(PFN_ASSIGN_ARRAY)&CResRef::operator=)(cResRef.m_resRef);
}
