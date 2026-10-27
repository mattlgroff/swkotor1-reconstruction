// Mini-game tracking behaviours (swmgbehavior.o, 0x006DB4F0-0x006DB650).
#include "l/sw_mg_behavior.h"

// FUNCTION: SWKOTOR 0x006DB4F0
CSWMGBehaviorTrack::CSWMGBehaviorTrack(CAurObject* pObject, CAurObject* pTo, CAurPart* pPart,
                                       CAurObject* pTarget)
    : CAurBehaviorAttach(pObject, pTo, pPart), m_vOffset(0.0f, 0.0f, 0.0f)
{
    m_bOnTarget = 0;
    m_pTarget = pTarget;
    AddDependency(pTarget);
}

// FUNCTION: SWKOTOR 0x006DB580
int CSWMGBehaviorTrack::GetType()
{
    return 0xaaac;
}

// FUNCTION: SWKOTOR 0x006DB590
CSWMGBehaviorTrackAndFire::CSWMGBehaviorTrackAndFire(CAurObject* pObject, CAurObject* pTo,
                                                     CAurPart* pPart, CAurObject* pTarget)
    : CSWMGBehaviorTrack(pObject, pTo, pPart, pTarget)
{
    m_fTimer = 0.0f;
}

// FUNCTION: SWKOTOR 0x006DB600
int CSWMGBehaviorTrackAndFire::GetType()
{
    return 0xaaaa;
}

// FUNCTION: SWKOTOR 0x006DB610 SYMBOL: ??_GCSWMGBehaviorTrack@@UAEPAXI@Z
// Emitted with the vtable (the destructor itself is in
// sw_mg_behavior_dtors_novtable.cpp).
