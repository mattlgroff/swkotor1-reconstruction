// the out-of-line CExoString default ctor at 0x005B3190 (own file so that
// callers such as CSWBaseItem::CSWBaseItem keep the call).

#include "shared/exo_string.h"

// FUNCTION: SWKOTOR 0x005B3190 SYMBOL: ??0CExoString@@QAE@XZ
CExoString::CExoString()
{
    m_sString = 0;
    m_nBufferLength = 0;
}
