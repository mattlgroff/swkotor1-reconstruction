// CResWave / CResLYT destructors and CLYT leaves (0x005DF120-0x005DF3EC).
#include "j/res.h"

// Emits CLYT's vftable (and so its scalar deleting destructor) here.
CLYT* NewCLYT()
{
    return new CLYT;
}

// FUNCTION: SWKOTOR 0x005DF120 SYMBOL: ??_GCLYT@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005DF140 SYMBOL: ?LoadLayout@CLYT@@QAEHABVCResRef@@@Z
int CLYT::LoadLayout(const CResRef& cResRef)
{
    CExoString sName;
    cResRef.CopyToString(sName);
    return LoadLayout(sName);
}

// FUNCTION: SWKOTOR 0x005DF1E0
CResWave::~CResWave()
{
}

// FUNCTION: SWKOTOR 0x005DF380 SYMBOL: ??_GCResWave@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005DF3C0
CResLYT::~CResLYT()
{
}

// FUNCTION: SWKOTOR 0x005DF3D0 SYMBOL: ??_GCResLYT@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x005D4C30 SYMBOL: ??0CResNCS@@QAE@XZ
CResNCS::CResNCS()
{
    m_bLoaded = 0;
    m_nNCSSize = 0;
    m_pNCS = 0;
}

// FUNCTION: SWKOTOR 0x005D4C50 SYMBOL: ??1CResNCS@@UAE@XZ
CResNCS::~CResNCS()
{
}
