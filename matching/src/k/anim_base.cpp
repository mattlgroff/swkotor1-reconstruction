// CAnimBase (vtable 0x00754AE4), the one-slot base of CSWCAnimBase. Kept
// in its own file: when ~CAnimBase is visible, ~CSWCAnimBase drops the
// EH state reset before the base destructor call.

#include "k/sw_c_anim_base.h"



// FUNCTION: SWKOTOR 0x00698E00
CAnimBase::CAnimBase()
{
    m_n4 = 0;
    m_n8 = 0;
}

// FUNCTION: SWKOTOR 0x00698E20
CAnimBase::~CAnimBase()
{
}

// FUNCTION: SWKOTOR 0x00698E30
int CAnimBase::SetValue(int nIndex, int nValue)
{
    switch (nIndex) {
    case 0:
        m_n4 = nValue;
        return 1;
    case 1:
        m_n8 = nValue;
        return 1;
    case 2:
        m_nC = nValue;
        return 1;
    case 3:
        m_n10 = nValue;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00698EA0 SYMBOL: ??_GCAnimBase@@UAEPAXI@Z

