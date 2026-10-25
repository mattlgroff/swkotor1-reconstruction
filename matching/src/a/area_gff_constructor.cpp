#include "d/res_gff.h"

class AreaGFF : public CResGFF {
public:
    AreaGFF();
};

// FUNCTION: SWKOTOR 0x00504830
AreaGFF::AreaGFF()
    : CResGFF()
{
    strncpy(m_pFileType, "ARE ", 4);
}
