// CSWCAnimBase (nwcanimbase.o) methods past worker K's range: the deleting
// destructor, PlayAnimSet and the five hilite colour slots.
#include "l/sw_c_anim_base.h"

// FUNCTION: SWKOTOR 0x006A03A0 SYMBOL: ??_GCSWCAnimBase@@UAEPAXI@Z

// This unannotated constructor emits the class vtable and its deleting
// destructor; the real constructor is worker K's 0x0069DFB0.
CSWCAnimBase::CSWCAnimBase() {}

// FUNCTION: SWKOTOR 0x006A03C0
int CSWCAnimBase::PlayAnimSet(int a, int b)
{
    if (!m_pModel) {
        return 0;
    }
    return PlayAnimSetOnModel(m_pModel, a, b);
}

// FUNCTION: SWKOTOR 0x006A03E0
int CSWCAnimBase::Hilite(const Vector* pColor, unsigned char nSlot, int nFlag)
{
    if (!m_pModel) {
        return 0;
    }
    if (nSlot >= 5) {
        return 0;
    }
    m_aVectors[nSlot] = *pColor;
    m_a90[nSlot] = 1;
    m_aA4[nSlot] = nFlag;
    RefreshHilite();
    return 1;
}

// FUNCTION: SWKOTOR 0x006A0440
int CSWCAnimBase::UnHilite(unsigned char nSlot)
{
    if (!m_pModel) {
        return 0;
    }
    if (nSlot >= 5) {
        return 0;
    }
    m_a90[nSlot] = 0;
    RefreshHilite();
    return 1;
}
