// CSWNameGen and CResHelper<CResLTR, 2036>, 0x00711090-0x00712190.
#include "o/sw_name_gen.h"

// FUNCTION: SWKOTOR 0x00711090
void CSWNameGen::UnloadNameTable()
{
    if (m_bLoaded) {
        m_bLoaded = 0;
        m_pRes->Release();
    }
}

// FUNCTION: SWKOTOR 0x007110B0 SYMBOL: ??1?$CResHelper@VCResLTR@@$0HPE@@@UAE@XZ
// FUNCTION: SWKOTOR 0x00711110 SYMBOL: ?SetResRef@?$CResHelper@VCResLTR@@$0HPE@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x00711270 SYMBOL: ??_G?$CResHelper@VCResLTR@@$0HPE@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00711290
CSWNameGen::CSWNameGen()
{
    m_bLoaded = 0;
    m_n20 = 0;
    m_n22 = 1;
}

// FUNCTION: SWKOTOR 0x007112E0
CSWNameGen::~CSWNameGen()
{
    UnloadNameTable();
}

// Returns whether the table could be demanded (callers ignore it).
// FUNCTION: SWKOTOR 0x00711340
int CSWNameGen::LoadNameTable(const CExoString& sTable)
{
    CResRef cResRef(sTable);
    SetResRef(cResRef, 0);
    if (!m_pRes->Demand()) {
        return 0;
    }
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x00712190 SYMBOL: ??_GCSWNameGen@@UAEPAXI@Z

template class CResHelper<CResLTR, 2036>;
