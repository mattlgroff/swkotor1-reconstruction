// CSWCArea environment-map accessor (descriptive name; no debug-map record).

#include "shared/res_ref.h"

class CSWCArea {
public:
    char pad_0x00[0xbc];
    CResRef m_cEnvMap;                          // +0xbc

    CResRef GetEnvMap();
};

// FUNCTION: SWKOTOR 0x0063BFA0
CResRef CSWCArea::GetEnvMap()
{
    return m_cEnvMap;
}
